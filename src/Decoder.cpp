#include "../include/Decoder.h"
#include <bitset>

std::string Decoder::decodeCallSign(const std::vector<uint8_t>& message) {
    auto callsignBits = extractBits(message, 41, 88);
    std::string callsign = converter.binToAlpha(callsignBits);
    return callsign;
}
  
std::string Decoder::decodeIcao(const std::vector<uint8_t>& message) {
    auto icaoBits = extractBits(message, 9, 32);
    std::string icao = converter.binaryToHex(icaoBits);
    return icao;
}

uint8_t Decoder::decodeDownlinkFormat(const std::vector<uint8_t>& message) {
    auto dfVector = extractBits(message, 1, 5);  // Extract Downlink Format
    uint8_t df = dfVector[0];
    return df;
}

uint8_t Decoder::decodeTypeCode(const std::vector<uint8_t>& message){
    auto tcVector = extractBits(message, 33, 37);
    uint8_t tc = tcVector[0];
    return tc;
}

uint8_t Decoder::decodeOddEven(const std::vector<uint8_t>& message){
    auto oeVector = extractBits(message, 54, 54);
    uint8_t oddEven = oeVector[0];
    return oddEven;
}

std::tuple<long, long> Decoder::decodePosition(const std::tuple<std::vector<uint8_t>&, std::vector<uint8_t>&>){
    std::tuple<long, long> location = std::make_tuple(0.0,0.0);

    return location;
}

// Private functions
std::vector<uint8_t> Decoder::extractBits(const std::vector<uint8_t>& message, size_t lowBit, size_t highBit) {
    // Adjust inputs to match 0-based indexing
    lowBit -= 1;
    highBit -= 1;
    // Determine Vector size, determine the starting bit      
    size_t numBits = highBit - lowBit + 1;
    size_t numBytes = (numBits + 7) / 8;
    const size_t startingBit = lowBit;
    std::vector<uint8_t> extractedBits(numBytes, 0);

    for (; lowBit <= highBit; ++lowBit) {
        size_t messageIndex = lowBit / 8;        // Determine what byte index the bit is in message[]
        size_t messageBitIndex = lowBit % 8;     // Determine what bit we need within the byte index
        // Isolate the bit, making it LSB, then mask to capture it. W
        // extractedBit will be either 00000000 or 00000001
        uint8_t extractedBit = (message[messageIndex] >> (7 - messageBitIndex)) & 0x1;   
                                                                                        
        size_t extractedIndex = (lowBit - startingBit) / 8;     // Determine byte index where we'll store extractedBit
        size_t extractedBitIndex = (lowBit - startingBit) % 8;  // Determine what bit within the byte we need to append extractedBit

        // Place the extractedBit in the correct position of extractedBits vector
        extractedBits[extractedIndex] |= (extractedBit << (7 - extractedBitIndex));
    }
    // Adjust the first byte if less than 8 bits were extracted
    if (numBits < 8) {
        extractedBits[0] >>= (8 - numBits); // Right shift to remove unnecessary bits
    }
    return extractedBits;
}