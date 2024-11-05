import sys
import time
import threading
import curses
import signal
from gui import gui_loop
from receiver import receive_messages
from data_cleanup import cleanup_inactive_entries
import config

# Flag to signal the threads to exit
exit_flag = False

def signal_handler(sig, frame):
    """Handle Ctrl+C and trigger a clean exit."""
    config.exit_flag = True
    print("\nExiting...")

if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)

    try:
        # sleep until initial output from rtl_adsb is displayed
        time.sleep(1)
        gui_thread = threading.Thread(target=lambda: curses.wrapper(gui_loop))
        gui_thread.start()
        cleanup_thread = threading.Thread(target=cleanup_inactive_entries)
        cleanup_thread.start()
        receive_messages()

    except KeyboardInterrupt:
        config.exit_flag = True
        gui_thread.join()
        cleanup_thread.join()
        sys.exit(0)

    finally:
        curses.endwin()
