#include "../include/ExtractBits.h"

std::vector<uint8_t> ExtractBits::getBitRange(size_t lowBit, size_t highBit, const std::vector<uint8_t>& binVector){
    
    // Local variables
    std::vector<uint8_t> extractedBits;
    uint8_t startingBit = lowBit;
    size_t bitRange = (highBit - lowBit) + 1;

    if(lowBit > highBit)
        return extractedBits;
    
    // account for 0 index
    lowBit -= 1;
    highBit -= 1;

    for(; lowBit <= highBit; lowBit++){
        // Determine starting vector index / bit index in binVector
        size_t binIndex = lowBit / 8;
        size_t binBitIndex = lowBit % 8;
        // isolate the bit and extract
        uint8_t extractedBit = (binVector[binIndex] >> (7 - binBitIndex)) & 0x1;
        // Determine vector index / bit index of extractedBits vector
        size_t extIndex = (lowBit - startingBit) / 8;
        size_t extBinIndex = (lowBit - startingBit) % 8;
        /*
             10001100
        */
        extractedBits[extIndex] |= (extractedBit << (7 - extBinIndex));
    }

    if(bitRange < 8){
        extractedBits[0] >>= (8 - bitRange);
    }
    return extractedBits;
}

// overload for string input
std::vector<uint8_t> ExtractBits::getBitRange(size_t lowBit, size_t highBit, const std::string& binStr){
    auto extractedBits = stov(binStr);
    return getBitRange(lowBit, highBit, extractedBits);
}

// String to vector
std::vector<uint8_t> ExtractBits::stov(const std::string& str) {
    std::vector<uint8_t> binaryVector;
    // Validate that the string is binary
    if (!isBinary(str)) {
        throw std::invalid_argument("Input string is not binary");
    }
    // Process the string in chunks of 8 bits
    for (size_t i = 0; i < str.size(); i += 8) {
        std::string byteStr = str.substr(i, 8); // Take 8 characters
        uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 2)); // Convert to uint8_t
        binaryVector.push_back(byte);
    }
    return binaryVector;
}

// is a binary string
bool ExtractBits::isBinary(const std::string& str){
    return str.find_first_not_of("01") == std::string::npos;
}


