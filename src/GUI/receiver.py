import sys
import time
import curses
import pyModeS as pms
import config
def receive_messages():
    """Read ADS-B messages from stdin and decode them."""
    oddEvenMsg = [None, None]
    oddEvenTime = [None, None]

    try:
        for line in sys.stdin:
            if config.exit_flag:
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
                    # an even and odd message with times stamps need to be sent to airborne_position()
                    if oddEven == 0:
                        oddEvenMsg[0] = hex_message
                        oddEvenTime[0] = time.time()
                    elif oddEven == 1:
                        oddEvenMsg[1] = hex_message
                        oddEvenTime[1] = time.time()
                    # an even first and then odd to decode the position, however airborne_position will handle
                    # any order the messages sent. Clear out the list after a pair is found.
                    if oddEvenMsg[0] and oddEvenMsg[1]:
                        pos = pms.adsb.airborne_position(oddEvenMsg[0], oddEvenMsg[1], oddEvenTime[0], oddEvenTime[1])
                        oddEvenMsg = [None, None]
                        oddEvenTime = [None, None]

                if icao in config.icao_data:
                    config.icao_data[icao]['callsign'] = cs or config.icao_data[icao]['callsign']
                    config.icao_data[icao]['velocity'] = vel or config.icao_data[icao]['velocity']
                    config.icao_data[icao]['altitude'] = alt or config.icao_data[icao]['altitude']
                    if pos:
                        config.icao_data[icao]['position'] = pos

                    config.icao_data[icao]['msg_count'] += 1
                    config.icao_data[icao]['last_updated'] = time.time()
                # We are only displaying data that have callsigns.
                elif len(config.icao_data) < max_entries and cs is not None:
                    config.icao_time[icao] = time.time()
                    config.icao_data[icao] = {
                        'callsign': cs or "N/A",
                        'velocity': vel or "N/A",
                        'altitude': alt or "N/A",
                        'position': pos if pos else "N/A",
                        'msg_count': 1,
                        'last_msg' : 1,
                        'last_updated': time.time(),
                    }

            except Exception as e:
                print(f"Error decoding message: {e}")

    except KeyboardInterrupt:
        pass
