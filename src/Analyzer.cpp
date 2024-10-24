#include "../include/Analyzer.h"

void Analyzer::analyze(const std::vector<uint8_t>& buffer) {
    message.initializeFromBuffer(buffer);
}

std::vector<uint8_t> Analyzer::hexStringToVector(std::string hexString){
    size_t length = hexString.length();
    std::vector<uint8_t> byteVector;
    // ensure is multiple of 2
    if(length % 2 != 0){
        std::cerr << "not a multiple of 2";
        return byteVector; // return empty vector
    }
    for(size_t i = 0; i < length; i += 2){
        std::string byteString = hexString.substr(i, 2); // get next two characters
        uint8_t byte = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16)); // convert to long the cast uint8_t
        byteVector.push_back(byte);
    }
    return byteVector;
}

std::string Analyzer::trim(std::string hexString){
    size_t first = hexString.find_first_not_of(" \t\r\n*");
    size_t last = hexString.find_last_not_of(" \t\r\n;");
    return (first == std::string::npos) ? "" : hexString.substr(first, last - first + 1);
}

std::vector<uint8_t> Analyzer::strToByteVec(std::string hexStr){
    size_t length = hexStr.size();
    std::vector<uint8_t> byteVector;
    // check that there is even amount of chars
    if(length % 2 != 0){
        std::cerr << "not correct format";
        return byteVector; // return empty vec if not correct format
    }    
    // conversion
    for(int i = 0; i < length; i +=2){
        // first grap first two characters
        std::string byte = hexStr.substr(i, 2);
        // convert to uint8_t
        uint8_t byteStr = static_cast<uint8_t>(std::stoul(byte, nullptr, 16));
        // now add to vector
        byteVector.push_back(byteStr);
    }
    return byteVector;
}

void Analyzer::print_hex(const std::vector<uint8_t>& buffer) const {
    size_t size = buffer.size();
    for(int i = 0; i < size; i++){
        std::cout << std::hex << 
        std::uppercase << std::setw(2) <<
        std::setfill('0') << static_cast<int>(buffer[i]);
    }
    std::cout << std::endl;
}
