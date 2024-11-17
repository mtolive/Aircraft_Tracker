#include <iostream>
#include <vector>
#include <unistd.h>
#include <string>
#include <csignal>
#include <algorithm> // Include this header for std::remove
//#include "../include/ADSB_Decoder.h"
//#include "../include/DataConverter.h"
#include "../include/ADSB_Controller.h"
#include "../include/ADSB_Message.h"

#define BUFFER_SIZE 16384  // 16KB

volatile std::sig_atomic_t stop = 0;

void handle_sigint(int signum) {
    stop = 1;
}

int main() {
    std::vector<uint8_t> buffer(BUFFER_SIZE);
    ssize_t len;
    // create instances for injection / pass references
    DataConverter converter;
    ADSB_Decoder decoder(converter);
    ADSB_Message message(decoder); 
    Filter filter; 
    ADSB_Controller controller(message, filter);

    // Setup signal handler for SIGINT (Ctrl+C)
    std::signal(SIGINT, handle_sigint);

    std::string hexString;
    // Read from standard input (pipe from rtl_adsb)
    while (std::getline(std::cin, hexString)) {

        // Trim any whitespace characters and format
        hexString = controller.trim(hexString);
        // Convert the hex string to a byte vector
        std::vector<uint8_t> byteVector = controller.hexStringToVector(hexString);
        if (!byteVector.empty()) {
            // Analyze the hex string
            controller.analyze(byteVector);
        } else {
            std::cerr << "Failed to parse hex string!" << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
