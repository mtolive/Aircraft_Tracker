#ifndef DECODER_H
#define DECODER_H

#include "../include/DataConverter.h"

class Decoder {
private:
    DataConverter& converter;
     // decode Bits from range
    std::vector<uint8_t> extractBits(const std::vector<uint8_t>& message, size_t lowBit, size_t highBit);

public:
    // Constructors
    Decoder(DataConverter& converter) : converter(converter) {}

    // Decode Downlink Format (DF)
    uint8_t decodeDownlinkFormat(const std::vector<uint8_t>& message);
    // Decode Type Code (TC)
    uint8_t decodeTypeCode(const std::vector<uint8_t>& message);
    // Decode ICAO 
    std::string decodeIcao(const std::vector<uint8_t>& message);
    // Decode Callsign
    std::string decodeCallSign(const std::vector<uint8_t>& message);
    // Decode odd / even
    uint8_t decodeOddEven(const std::vector<uint8_t>& message);
    // Decode Lat/Lon
    std::string decodePosition(const std::vector<uint8_t>& message);
    // practice delete later
    std::vector<uint8_t> extractBits2(const std::vector<uint8_t>& message, size_t lowBit, size_t highBit);

   
};

#endif // Decoder_H
