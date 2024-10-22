#include <iostream>
#include <vector>
#include <unistd.h>
#include <string>
#include <csignal>
#include <algorithm> // Include this header for std::remove
#include "../include/Analyzer.h"
#include "../include/DataConverter.h"
#include "../include/Decoder.h"
#include "../include/ADSBMessage.h"

#define BUFFER_SIZE 16384  // 16KB

volatile std::sig_atomic_t stop = 0;

void handle_sigint(int signum) {
    stop = 1;
}

int main() {
    std::vector<uint8_t> buffer(BUFFER_SIZE);
    ssize_t len;

    DataConverter converter;
    Decoder decoder(converter); // Create an instance of Filter
    ADSBMessage message(decoder);  
    Analyzer analyzer(message); // Pass the filter instance to Analyzer

    // Setup signal handler for SIGINT (Ctrl+C)
    std::signal(SIGINT, handle_sigint);

    std::string hexString;

    // Read from standard input (pipe from rtl_adsb)
    while (std::getline(std::cin, hexString)) {

        // Trim any whitespace characters and format
        hexString = analyzer.trim(hexString);
        // Convert the hex string to a byte vector
        std::vector<uint8_t> byteVector = analyzer.hexStringToVector(hexString);
        if (!byteVector.empty()) {
            // Analyze the hex string
            analyzer.analyze(byteVector);
        } else {
            std::cerr << "Failed to parse hex string!" << std::endl;
        }
    }
    
   // for single line echo test
   /*std::getline(std::cin, hexString);

        // Trim any whitespace characters and format
        hexString = analyzer.trim(hexString);
        // Convert the hex string to a byte vector
        std::vector<uint8_t> byteVector = analyzer.hexStringToVector(hexString);
        if (!byteVector.empty()) {
            // Analyze the hex string
            analyzer.analyze(byteVector);
        } else {
            std::cerr << "Failed to parse hex string!" << std::endl;
        }*/
    

    return EXIT_SUCCESS;
}
