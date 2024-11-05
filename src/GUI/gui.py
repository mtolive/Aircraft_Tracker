import curses
import time
import config
spinner_idx = 0  # Index for the spinner animation

def update_screen(stdscr):
    """Update the terminal with the latest data from icao_data dictionary."""
    global spinner_idx
    stdscr.clear()  # Clear the screen before updating

    curses.start_color()
    curses.init_pair(1, curses.COLOR_BLUE, curses.COLOR_BLACK)

    header = f"ICAO     Callsign     Velocity (kt)    Heading     Altitude   Lat       Lon       Messages  Live  {config.spinner[spinner_idx]}"
    stdscr.attron(curses.color_pair(1))
    stdscr.addstr(0, 0, header)
    stdscr.attroff(curses.color_pair(1))

    sorted_data = sorted(config.icao_data.items(), key=lambda item: item[1]['msg_count'], reverse=True)
    row = 1
    # Break from the function if displayed information is at capacity. Wait until entry(s) are removed
    for icao, info in sorted_data:
        if row >= curses.LINES - 1:
            break

        callsign = str(info.get('callsign', 'N/A'))
        # If there is no callsign we don't display the data
        if info.get('callsign') == 'N/A':
            continue

        # velocity can be (None | tuple of 4), where velocity and heading are first two indices
        # and can either be (None | Float). From pyModeS function adsb.velocity()
        velocity_data = info.get('velocity', ('N/A', 'N/A'))
        if isinstance(velocity_data, tuple):
            velocity = f"{velocity_data[0]:.1f}" if velocity_data[0] else "N/A"
            heading = f"{velocity_data[1]:.0f}°" if velocity_data[1] else "N/A"
        else:
            velocity = "N/A"
            heading = "N/A"
        
        # position can be (None | tuple[float, float])
        position_data = info.get('position', ('N/A', 'N/A'))
        if isinstance(position_data, tuple):
            lat = f"{position_data[0]:.3f}" if position_data[0] else "N/A"
            lon = f"{position_data[1]:.3f}" if position_data[1] else "N/A"
        else:
            lat = "N/A"
            lon = "N/A"
        
        # altitude can be (None | Float) -- msg_count starts at int 1
        altitude = str(info.get('altitude', 'N/A'))[:8] + "ft" if info.get('altitude') != 'N/A' else "N/A"
        msg_count = str(info.get('msg_count', 'N/A'))

        live_time = int(time.time() - config.icao_time.get(icao, time.time()))
        live_time_display = f"{live_time} sec"

        display_line = f"{icao:<8} {callsign:<12} {velocity:<16} {heading:<11} {altitude:<10} {lat:<9} {lon:<9} {msg_count:<9} {live_time_display:<5}"
        stdscr.addstr(row, 0, display_line)

        row += 1

    stdscr.refresh()
    spinner_idx = (spinner_idx + 1) % len(config.spinner)

def gui_loop(stdscr):
    """Continuously update the terminal screen."""
    curses.curs_set(0)
    while not config.exit_flag:
        update_screen(stdscr)
        time.sleep(1)
