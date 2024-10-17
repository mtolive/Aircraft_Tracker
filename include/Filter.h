#ifndef FILTER_H
#define FILTER_H

#include "../include/Decoder.h"

class Filter {
private:
    Decoder& decoder;
     // Extract Bits from range
    std::vector<uint8_t> extractBits(const std::vector<uint8_t>& message, size_t lowBit, size_t highBit);

public:
    // Constructors
    Filter(Decoder& decoder) : decoder(decoder) {}

    // Extract Downlink Format (DF)
    uint8_t extractDownlinkFormat(const std::vector<uint8_t>& message);
    // Extract Type Code (TC)
    uint8_t extractTypeCode(const std::vector<uint8_t>& message);
    // Extract ICAO 
    std::string extractIcao(const std::vector<uint8_t>& message);
    // Extract Callsign
    std::string decodeCallSign(const std::vector<uint8_t>& message);
    
    // practice delete later
    std::vector<uint8_t> Filter::extractBits2(const std::vector<uint8_t>& message, size_t lowBit, size_t highBit);

   
};

#endif // FILTER_H
