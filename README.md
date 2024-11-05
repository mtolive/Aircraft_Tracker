# Aircraft Tracker

This project captures, decodes, and displays ADS-B messages transmitted by aircraft using a combination of C++ and Python. The system processes incoming data streams, extracting and displaying: ICAO code, Callsign, Velocity, Heading, Altitude, Latitude, and longitude - in real time. The architecture employs a modular approach with dedicated classes for structured decoding and analysis.

## Table of Contents
- [Overview](#overview)
- [Architecture](#architecture)
- [Features](#features)
- [Setup](#setup)
- [Usage](#usage)
- [Modules](#modules)
- [Classes Overview](#classes-overview)
- [Configuration](#configuration)
- [Future Enhancements](#future-enhancements)
- [Acknowledgments](#acknowledgments)

## Overview
The project utilizes an RTL-SDR dongle to receive raw ADS-B data, which is piped through a C++ program for initial processing and then into a Python-based GUI for visualization. The command structure for running the system is:

```bash
rtl_adsb | ./adsb_processor | python main.py
```

## Architecture
The program is divided into two main components:

1. **C++ Program**: Captures and processes ADS-B data from the RTL-SDR dongle.
2. **Python Program**: Decodes the ADS-B messages and displays the aircraft information in a terminal-based GUI.

## Features
- **ADS-B Message Parsing**: The `ADSBMessage` class reads raw ADS-B message frames, extracting the downlink format, type code, ICAO code, and other key fields.
- **Message Filtering**: The `Filter` class enables selective filtering of ADS-B messages based on type codes and downlink formats.
- **Real-Time Analysis**: The `Analyzer` class processes filtered messages and updates aircraft data as new messages arrive.
- **Data Conversion**: The `DataConverter` class provides functions for converting binary data to hex and decimal formats, aiding in message decoding and displaying callsigns.
- **Decoder**: The `Decoder` class provides functions to decode call sign, ICAO code, downlink format, and type code. Velocity, Altitude and Location are in development.
- **Terminal-based GUI**: The Python program shows live aircraft data in a structured table with updates based on unique ICAO addresses.

## Setup
### Prerequisites
- **Hardware**: RTL-SDR dongle and suitable antenna.
- **Software**: 
  - Python 3
  - RTL-SDR software
  - C++ compiler (e.g., GCC)
  - Python packages: `pyModeS`, `curses`
  
### Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/your-username/adsb-message-decoder.git
   cd adsb-message-decoder
   ```

2. **Install RTL-SDR Software**
   Follow [RTL-SDR installation instructions](https://osmocom.org/projects/rtl-sdr/wiki/Rtl-sdr) to set up the `rtl_adsb` tool.

3. **Install Python Dependencies**
   ```bash
   pip install pyModeS
   ```

4. **Compile the C++ Program**
   ```bash
   g++ -std=c++17 -o adsb_processor src/*.cpp -I include -lrtlsdr
   ```

## Usage
Run the program with the following command to start capturing and visualizing ADS-B messages:
```bash
rtl_adsb | ./adsb_processor | python main.py
```

Press `Ctrl+C` to exit.

## Modules
### C++ Program
Handles the raw ADS-B message processing:
- **ADSBMessage**: Class for representing and decoding ADS-B messages.
- **Analyzer**: Analyzes message types and initializes data structures.
- **DataConverter**: Converts data formats for easier processing.
- **Filter**: Filters ADS-B messages based on specified criteria.

### Python Program
Organizes and displays aircraft information:
- **config.py**: Initializes shared data and configuration settings.
- **data_cleanup.py**: Cleans up inactive entries based on message activity.
- **gui.py**: Displays the terminal-based GUI with live data.
- **receiver.py**: Receives and decodes messages from standard input.
- **main.py**: Initializes threads for receiving messages, cleanup, and GUI display.

## Classes Overview
1. **ADSBMessage**
   - Handles initialization and field extraction of 14-byte ADS-B messages.
   - Main Functions: `initializeFromBuffer`, `extractIcao`, `extractTc`, `extractDf`, `extractCallsign`.

2. **Analyzer**
   - Processes ADS-B messages and groups them based on type.
   - Main Functions: `analyze`, `initData`, `analyzeIdMsg`, `analyzePosMsg`, `analyzeVelMsg`.

3. **DataConverter**
   - Converts ADS-B data between formats.
   - Main Functions: `binaryToHex`, `hexToBinary`, `binaryToDecimal`.

4. **Filter**
   - Filters ADS-B messages to select only those relevant to the analysis.

## Configuration
The following configurations are available in `config.py`:
- **`icao_data`**: Dictionary to store aircraft data by ICAO address.
- **`inactive_time`**: Time threshold for removing inactive entries.
- **`nochange_time`**: Time threshold for entries with more than 10 messages.
- **`onemsg_time`**: Time threshold for entries with exactly one message.
- **`spinner`**: Characters for the spinner animation.

## Future Enhancements
- **Position and Velocity Decoding**: Implement decoding for position/altitude, and velocity within C++ software - to remove dependecy of pyModeS. (currently in development)
- **Develop ADS-B data capture**: Implement error check (Cyclic redudancy check), as well preamble filter to finish software (currently in development)
- **Real-Time Display**: Develop a GUI to visualize real-time aircraft data, showing call signs and locations.
- **Enhanced Filtering**: Support filtering for additional message types and parameters.

## Acknowledgments
- **RTL-SDR**: Open-source software-defined radio library for ADS-B data capture.
- **pyModeS**: Python library for decoding ADS-B Mode-S messages.
