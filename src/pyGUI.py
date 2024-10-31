import sys
import time
import threading
import curses
import signal
# from pyModeS.decoder.bds.bds08 import callsign
# from pyModeS.decoder.adsb import velocity, altitude
import pyModeS as pms


# Dictionary to hold the ICAO data
icao_data = {}

# Animation
spinner = ['|', '/', '-', '\\']
spinner_idx = 0  # Index for the spinner animation

# Flag to signal the threads to exit
exit_flag = False

def signal_handler(sig, frame):
    """Handle Ctrl+C and trigger a clean exit."""
    global exit_flag
    exit_flag = True
    print("\nExiting...")

def update_screen(stdscr):
    """Update the terminal with the latest data from icao_data dictionary."""
    stdscr.clear()  # Clear the screen before updating

    # Display the header with the spinner
    header = f"ICAO     Callsign     Velocity    Heading     Altitude   Messages  {spinner[spinner_idx]}"
    stdscr.addstr(0, 0, header)

    # Loop through the dictionary and display each ICAO entry
    for row, (icao, info) in enumerate(icao_data.items(), start=1):
        # Break if the row exceeds the terminal's height
        if row >= curses.LINES - 1:
            break

        # Retrieve callsign and ensure it doesn't default to N/A if data exists
        callsign = str(info.get('callsign')) if info.get('callsign') is not None else "N/A"

        # Extract velocity as a tuple and format for separate velocity and heading columns
        velocity_data = info.get('velocity', ('N/A', 'N/A'))
        if isinstance(velocity_data, tuple):
            velocity = f"{velocity_data[0]:.1f}" if velocity_data[0] is not None else "N/A"
            heading = f"{velocity_data[1]:.0f}°" if velocity_data[1] is not None else "N/A"
        else:
            velocity = str(velocity_data)[:8]  # Limit to 8 characters if it's not a tuple
            heading = "N/A"

        # Truncate altitude to fit in 8 characters
        altitude = str(info.get('altitude', 'N/A'))[:8]
        
        msg_count = str(info.get('msg_count', 'N/A'))

        # Format and display the line
        display_line = f"{icao:8} {callsign:12} {velocity:10} {heading:10} {altitude:10} {msg_count:8}"
        stdscr.addstr(row, 0, display_line)

    stdscr.refresh()  # Refresh the screen to show the updated data

def gui_loop(stdscr):
    """Continuously update the terminal screen."""
    global spinner_idx  # Declare spinner_idx as global
    curses.curs_set(0)  # Hide the cursor for a cleaner interface
    while not exit_flag:
        update_screen(stdscr)

        # Cycle through the spinner characters
        spinner_idx = (spinner_idx + 1) % len(spinner)
        time.sleep(1)  # Update every second

def receive_messages():
    """Read ADS-B messages from stdin and decode them."""
    try:
        for line in sys.stdin:
            if exit_flag:
                break

            hex_message = line.strip()  # Get the 14-byte hex message

            if len(hex_message) != 28:  # Check if it's the expected 14-byte (28 hex chars)
                continue

            try:
                cs, vel, alt = None, None, None

                tc = pms.adsb.typecode(hex_message)
                
                # Extract the ICAO address (first 6 hex characters)

                icao = pms.adsb.icao(hex_message)

                # Decode callsign (if applicable, using pyModeS)
                if tc in range(1,5):
                    cs = pms.adsb.callsign(hex_message)

                # Decode velocity (if applicable, using pyModeS)
                if tc == 19:
                    vel = pms.adsb.velocity(hex_message)

                # Decode altitude (if applicable, using pyModeS)
                if tc in range(9,19):
                    alt = pms.adsb.altitude(hex_message)

                # Update the ICAO dictionary with the new data and increment the message count
                if icao not in icao_data:
                    icao_data[icao] = {
                        'callsign': cs or "N/A",
                        'velocity': vel or "N/A",
                        'altitude': alt or "N/A",
                        'msg_count': 1  # Start the message count at 1
                    }
                else:
                    # Update existing data and increment message count
                    icao_data[icao]['callsign'] = cs or icao_data[icao]['callsign']
                    icao_data[icao]['velocity'] = vel or icao_data[icao]['velocity']
                    icao_data[icao]['altitude'] = alt or icao_data[icao]['altitude']
                    icao_data[icao]['msg_count'] += 1  # Increment the message count

            except Exception as e:
                print(f"Error decoding message: {e}")

    except KeyboardInterrupt:
        pass  # Handle the interrupt silently

if __name__ == "__main__":
    # Set up signal handler to catch Ctrl+C and stop cleanly
    signal.signal(signal.SIGINT, signal_handler)

    try:
        # Start the curses-based display thread
        gui_thread = threading.Thread(target=lambda: curses.wrapper(gui_loop))
        gui_thread.start()

        # Read ADS-B messages from stdin
        receive_messages()

    except KeyboardInterrupt:
        # Set the exit flag to stop all threads
        exit_flag = True

        # Wait for the curses thread to finish
        gui_thread.join()

        # Exit cleanly without traceback or extra output
        sys.exit(0)

    finally:
        # Make sure to reset the terminal in case of any exceptions
        curses.endwin()