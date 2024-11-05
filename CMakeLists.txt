# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(AircraftTracker)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add executable for the C++ tracker
add_executable(tracker
    src/ADSBMessage.cpp
    src/Analyzer.cpp
    src/DataConverter.cpp
    src/Decoder.cpp
    src/Filter.cpp
    src/main.cpp
)

# set_target_properties(tracker PROPERTIES COMPILE_FLAGS "-Wall")
# Custom target to run the pipeline
add_custom_target(acTracker
    COMMAND rtl_adsb | ${CMAKE_BINARY_DIR}/tracker | python3 ${CMAKE_SOURCE_DIR}/src/GUI/main.py
    DEPENDS tracker
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}  # Set this to the project root
    COMMENT "Running the rtl_adsb pipeline with tracker and Python GUI"
)
