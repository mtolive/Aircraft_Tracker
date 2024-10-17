
import pyModeS as pms
import sys

def process_hex_line(hex_line):
    hex_line = hex_line.strip()
    if len(hex_line) != 28:
        print(f"msg length incorrect: {str(len(hex_line))}")
        return None
    try:
        return pms.tell(hex_line)
    except KeyError as e:
        print(f"KeyError: {e} - invalid message type")
        return None
    except ValueError:
        print("Invalid hex line")
        return None
    except Exception as e:
        print(f"Unexpected error: {e}")
        return None

for line in sys.stdin:
    result = process_hex_line(line)
    if result is not None:
        print(result)
   
        
