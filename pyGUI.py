import sys
import time
import threading
import curses
import signal
from pyModeS.decoder.bds.bds08 import callsign
from pyModeS.decoder.adsb import velocity, altitude

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
    header = f"ICAO     Callsign     Velocity    Altitude   Messages  {spinner[spinner_idx]}"
    stdscr.addstr(0, 0, header)

    # Loop through the dictionary and display each ICAO entry
    for row, (icao, info) in enumerate(icao_data.items(), start=1):
        display_line = f"{icao:8} {info['callsign']:12} {info['velocity']:10} {info['altitude']:10} {info['msg_count']:8}"
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
                # Extract the ICAO address (first 6 hex characters)
                icao = hex_message[:6].upper()

                # Decode callsign (if applicable, using pyModeS)
                cs = callsign(hex_message)

                # Decode velocity (if applicable, using pyModeS)
                vel = velocity(hex_message)

                # Decode altitude (if applicable, using pyModeS)
                alt = altitude(hex_message)

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
