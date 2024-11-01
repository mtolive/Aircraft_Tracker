import sys
import time
import threading
import curses
import signal
import pyModeS as pms

# Dictionary to hold the ICAO data
icao_data = {}
icao_time = {}

# Animation
spinner = ['|', '/', '-', '\\']
spinner_idx = 0  # Index for the spinner animation

# Flag to signal the threads to exit
exit_flag = False

# Time threshold to remove inactive entries (in seconds)
inactive_time = 45
nochange_time = 120
onemsg_time = 20

def signal_handler(sig, frame):
    """Handle Ctrl+C and trigger a clean exit."""
    global exit_flag
    exit_flag = True
    print("\nExiting...")

def update_screen(stdscr):
    """Update the terminal with the latest data from icao_data dictionary."""
    global spinner_idx
    stdscr.clear()  # Clear the screen before updating

    # Set header color
    curses.start_color()
    curses.init_pair(1, curses.COLOR_BLUE, curses.COLOR_BLACK)

    # Display the header with the spinner
    header = f"ICAO     Callsign     Velocity (kt)    Heading     Altitude   Lat       Lon       Messages  Live  {spinner[spinner_idx]}"
    
    # Set color on for header
    stdscr.attron(curses.color_pair(1))
    stdscr.addstr(0, 0, header)
    stdscr.attroff(curses.color_pair(1))

    # Sort the dictionary by msg_count in descending order before displaying
    sorted_data = sorted(icao_data.items(), key=lambda item: item[1]['msg_count'], reverse=True)

    # Row counter
    row = 1
    # Loop through the dictionary and display each ICAO entry
    for icao, info in sorted_data:
        if row >= curses.LINES - 1:
            break

        callsign = str(info.get('callsign', 'N/A'))
        if info.get('callsign') == 'N/A':
            continue

        # Extract airborne velocity data
        velocity_data = info.get('velocity', ('N/A', 'N/A'))
        if isinstance(velocity_data, tuple):
            velocity = f"{velocity_data[0]:.1f}" if velocity_data[0] is not None else "N/A"
            heading = f"{velocity_data[1]:.0f}°" if velocity_data[1] is not None else "N/A"
        else:
            velocity = str(velocity_data)[:8]
            heading = "N/A"

        # Extract lat/lon with 3 decimal places
        position_data = info.get('position', ('N/A', 'N/A'))
        if isinstance(position_data, tuple):
            lat = f"{position_data[0]:.3f}" if position_data[0] is not None else "N/A"
            lon = f"{position_data[1]:.3f}" if position_data[1] is not None else "N/A"
        else:
            lat = "N/A"
            lon = "N/A"

        altitude = str(info.get('altitude', 'N/A'))[:8]
        if altitude != 'N/A':
            altitude += "ft"

        msg_count = str(info.get('msg_count', 'N/A'))
        live_time = int(time.time() - icao_time.get(icao, time.time()))
        live_time_display = f"{live_time} sec"  # Display as seconds

        # Construct the line to display
        display_line = f"{icao:<8} {callsign:<12} {velocity:<16} {heading:<11} {altitude:<10} {lat:<9} {lon:<9} {msg_count:<9} {live_time_display:<5}"
        stdscr.addstr(row, 0, display_line)

        row += 1

    stdscr.refresh()
    spinner_idx = (spinner_idx + 1) % len(spinner)


def gui_loop(stdscr):
    """Continuously update the terminal screen."""
    curses.curs_set(0)
    while not exit_flag:
        update_screen(stdscr)
        time.sleep(1)

def receive_messages():
    """Read ADS-B messages from stdin and decode them."""
    oddEvenMsg = [None, None]  # index 0 for even, 1 for odd
    oddEvenTime = [None, None]  # index 0 for even, 1 for odd

    try:
        for line in sys.stdin:
            if exit_flag:
                break

            hex_message = line.strip()
            if len(hex_message) != 28:
                continue

            try:
                max_entries = curses.LINES - 1
                cs, vel, alt, pos = None, None, None, None
                tc = pms.adsb.typecode(hex_message)
                icao = pms.adsb.icao(hex_message)

                if tc in range(1, 5):
                    cs = pms.adsb.callsign(hex_message)

                if tc == 19:
                    vel = pms.adsb.velocity(hex_message)

                if tc in range(9, 19):
                    alt = pms.adsb.altitude(hex_message)
                    oddEven = pms.adsb.oe_flag(hex_message)

                    # Collect odd and even messages
                    if oddEven == 0:
                        oddEvenMsg[0] = hex_message
                        oddEvenTime[0] = time.time()
                    elif oddEven == 1:
                        oddEvenMsg[1] = hex_message
                        oddEvenTime[1] = time.time()

                    # Calculate position if both messages are present
                    if oddEvenMsg[0] and oddEvenMsg[1]:
                        pos = pms.adsb.airborne_position(oddEvenMsg[0], oddEvenMsg[1], oddEvenTime[0], oddEvenTime[1])
                        oddEvenMsg = [None, None]  # Reset after processing
                        oddEvenTime = [None, None]

                if icao in icao_data:
                    # Update existing ICAO entry
                    icao_data[icao]['callsign'] = cs or icao_data[icao]['callsign']
                    icao_data[icao]['velocity'] = vel or icao_data[icao]['velocity']
                    icao_data[icao]['altitude'] = alt or icao_data[icao]['altitude']
                    if pos:
                        icao_data[icao]['position'] = pos  # Update only if position is calculated
                    icao_data[icao]['msg_count'] += 1
                    icao_data[icao]['last_updated'] = time.time()

                elif len(icao_data) < max_entries and cs is not None:
                    # Add a new ICAO entry
                    icao_time[icao] = time.time()

                    icao_data[icao] = {
                        'callsign': cs or "N/A",
                        'velocity': vel or "N/A",
                        'altitude': alt or "N/A",
                        'position': pos if pos else "N/A",
                        'msg_count': 1,
                        'last_updated': time.time(),
                        'last_msg': 1  # Initialize 'last_msg' to 1
                    }

            except Exception as e:
                print(f"Error decoding message: {e}")

    except KeyboardInterrupt:
        pass


def cleanup_inactive_entries():
    """Remove entries with only 1 message if not updated within the threshold."""
    while not exit_flag:
        current_time = time.time()
        to_remove = [
            icao for icao, info in list(icao_data.items())
            if (
                (info['msg_count'] == 1 and (current_time - info['last_updated']) > onemsg_time) or
                (info['last_msg'] == info['msg_count'] and (current_time - info['last_updated']) > inactive_time
                and (info['msg_count'] < 10)) or
                (info['msg_count'] >= 10) and (current_time -info['last_updated']) > nochange_time or
                (info['msg_count'] <= 2 | 3 and (current_time - info['last_updated']) > inactive_time)
            )
        ]
        
        # Remove the entries
        for icao in to_remove:
            del icao_data[icao]
        time.sleep(1)

if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)

    try:
        time.sleep(1)
        gui_thread = threading.Thread(target=lambda: curses.wrapper(gui_loop))
        gui_thread.start()

        cleanup_thread = threading.Thread(target=cleanup_inactive_entries)
        cleanup_thread.start()

        receive_messages()

    except KeyboardInterrupt:
        exit_flag = True
        gui_thread.join()
        cleanup_thread.join()
        sys.exit(0)

    finally:
        curses.endwin()
