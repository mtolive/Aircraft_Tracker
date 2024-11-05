import time
import config
def cleanup_inactive_entries():
    """Remove entries with only 1 message if not updated within the threshold."""
    while not config.exit_flag:
        current_time = time.time()
        to_remove = [
            icao for icao, info in list(config.icao_data.items())
            if (
                # If there is only one messaged recieved within - onemsg_time (poor signal, aircraft out of distance)
                (info['msg_count'] == 1 and (current_time - info['last_updated']) > config.onemsg_time) or
                # If the message hasn't changed for < 10 msg_count, within last_updated (stopped receiving)
                (info['last_msg'] == info['msg_count'] and (current_time - info['last_updated']) > config.inactive_time
                and (info['msg_count'] < 10)) or
                # If message count > 10 (usually good signal receiving messages) gets longer threshold nochange_time
                (info['msg_count'] >= 10) and (current_time -info['last_updated']) > config.nochange_time or
                # Make sure message counts 2/3 get removed after their threshold
                (info['msg_count'] <= 2 | 3 and (current_time - info['last_updated']) > config.inactive_time)
            )
        ]
        
        for icao in to_remove:
            del config.icao_data[icao]
        time.sleep(1)
