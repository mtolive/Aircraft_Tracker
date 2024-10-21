#include "../include/Filter.h"
    // Will decode callsigns
    // Filter for a specific Downlink Format (DF)

    // extract ac type

    std::string Filter::decodeCallSign(const std::vector<uint8_t>& message) {
        auto callsignBits = extractBits(message, 41, 88);
        std::string callsign = decoder.binToAlpha(callsignBits);
        return callsign;
    }
  
   std::string Filter::extractIcao(const std::vector<uint8_t>& message) {
    auto icaoBits = extractBits(message, 9, 32);
   /* for (auto bit : icaoBits) {
        std::cout << std::bitset<8>(bit) << " "; // Display bits in binary for verification
    } */
    std::string icao = decoder.binaryToHex(icaoBits);
    return icao;
}


   uint8_t Filter::extractDownlinkFormat(const std::vector<uint8_t>& message) {
    if (message.size() < 5) {
        std::cerr << "Error: Message vector is too short." << std::endl;
        return 0; // o
    }

    uint8_t df = (message[0] >> 3) & 0x1F;  // Extract Downlink Format
    //std::cout << "df = " << static_cast<int>(df) << std::endl; // Cast to int for proper output
    return df;
}

    uint8_t Filter::extractTypeCode(const std::vector<uint8_t>& message){
    if (message.size() < 5) {
    std::cerr << "Error: Message vector is too short." << std::endl;
    return 0; 
    }
        uint8_t tc = (message[4] >> 3) & 0x1F;
        //std::cout << "tc = " << static_cast<int>(tc) << std::endl; // Cast to int for proper output
        return tc;
    }

    // Private functions

std::vector<uint8_t> Filter::extractBits(const std::vector<uint8_t>& message, size_t lowBit, size_t highBit) {
    // Adjust inputs to match 0-based indexing
    lowBit -= 1;  // Subtract 1 to account for 1-based indexing
    highBit -= 1; // Subtract 1 for consistency
    
    size_t numBits = highBit - lowBit + 1;
    size_t numBytes = (numBits + 7) / 8;
    size_t startingBit = lowBit;
    std::vector<uint8_t> extractedBits(numBytes, 0);

    for (; lowBit <= highBit; ++lowBit) {
        size_t messageIndex = lowBit / 8;
        size_t messageBitIndex = lowBit % 8;
        uint8_t extractedBit = (message[messageIndex] >> (7 - messageBitIndex)) & 0x1;

        size_t extractedIndex = (lowBit - startingBit) / 8;
        size_t extractedBitIndex = (lowBit - startingBit) % 8;

        // Place the bit in the correct position of extractedBits
        extractedBits[extractedIndex] |= (extractedBit << (7 - extractedBitIndex));
    }

    return extractedBits;
}

std::vector<uint8_t> extractBits2(const std::vector<uint8_t>& message, size_t lowBit, size_t highBit){

    // offset
    lowBit -= 1;
    highBit -= 1;
    const size_t lowBitValue = lowBit;
    const size_t size = message.size();
    std::vector<uint8_t> extractedBits;

    for(size_t i = 0; i < size; ++i){

        size_t messageIndex = lowBit / 8;    // index of extractedbit within the message
        size_t messageBitIndex = lowBit % 8; // index of the extracted bit within message[messageIndex]

        // extract the bit
        uint8_t extractedBit = (message[messageIndex] >> (7 - lowBit)) & 0x1;
        // determine index of eb vector as well as bit placement
        size_t extractedIndex = (lowBit - lowBitValue) / 8;
        size_t extractedBitIndex = (lowBit - lowBitValue) % 8;
        
        extractedBits[extractedIndex] |= (extractedBit << 7 - lowBit);

        return extractedBits;
    };




}
    


   

    

    
    /*
    
    // extract downlink format (DF) from the first byte
   uint8_t df = (buffer[0] >> 3) & 0x1F; // first 5 bits

   a function that returns an uint8_t, (takes in range -- can be 1-1 or 22-22 etc, const std::vector<uint8_t>& buffer)
        size_t buffer.size();
        lv = 22 // low value
        hv = 31 // high value
        8
        lv = 22  // low value
        // determine buffer index low value
        low_buffer = (lv / 8)  // 2
        // determine shift
        low_shift = (lv % 8)       // 6

        // determine buffer index high value
        high_buffer = (hv / 8)  // 3
        // determine shift
        high_shift = (hv % 8)   //  7



        00000000 0
        00000000 1
        00000000 2
        00000000 3
    
    */

