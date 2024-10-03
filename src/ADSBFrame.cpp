#include "../include/ADSBFrame.h"

const size_t ADSBFrame::FRAME_SIZE = 14;

ADSBFrame::ADSBFrame(const std::vector<uint8_t>& buffer) {
        if (buffer.size() >= getFrameSize()) {
            rawData = buffer;
            df = (buffer[0] >> 3) & 0x1F;  // Extract DF
            tc = (buffer[4] >> 3) & 0x1F;  // Extract TC
        } else {
            throw std::invalid_argument("Invalid frame size");
        }
    }

// Reminder: bit number is 1 greater than the index. 
void ADSBFrame::extractCallSign() {
    callsign.clear();
    hexValue.clear();  // Clear hexValue at the beginning

    // Mask to extract bits 0-7
    uint8_t mask = 0xFF; // 0b11111111

    // Loop through bytes 5 to 10 (inclusive)
    for (int i = 5; i < 11; ++i) {
        // Create a hex string for the current byte
        std::ostringstream hexStream;
        hexStream << std::hex << std::uppercase << std::setw(2) 
                  << std::setfill('0') << (rawData[i] & mask);
        
        // Append the hex value to hexValue string
        hexValue += hexStream.str();
    }
    // Print the accumulated hex value
    std::cout << "Hex Value: " << hexValue << std::endl;
}



    std::string ADSBFrame::getCallSign() const {
        return callsign;
    }

    uint8_t ADSBFrame::getDownlinkFormat() const {
        return df;
    }

    size_t ADSBFrame::getFrameSize() const {
        return FRAME_SIZE;
    }

    uint8_t ADSBFrame::getTypeCode() const {
        return tc;
    }

    void ADSBFrame::printHex() const {
        for (size_t i = 0; i < rawData.size(); i++) {
            std::cout << std::hex << std::uppercase << std::setw(2) 
                      << std::setfill('0') << static_cast<int>(rawData[i]);
        
            // Print new line after every 14 bytes
            if ((i + 1) % 14 == 0) {
                std::cout << std::endl; // Print a new line after every 14 bytes
            }
        }
    }


    // right shift i to least significant bit mask using 1.
    void ADSBFrame::print_bits(uint8_t byte) const {
        for(int i = 7; i >= 0; i--){
        std::cout << ((byte >> i) & 1);
        }
    }
    // print binary
    void ADSBFrame::print_binary(const std::vector<uint8_t>& buffer, int len) const{   
        for(int i = 0; i < len; i++){
        print_bits(buffer[i]); // pass in each byte
        std::cout << " ";
        }
        std::cout << std::endl;
    }

