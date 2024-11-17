#ifndef DECODER_I
#define DECODER_I

#include <iostream>
#include <vector>

class Decoder{
public:
    virtual uint8_t decodeDownlinkFormat(const std::vector<uint8_t>& message) = 0;
    virtual uint8_t decodeTypeCode(const std::vector<uint8_t>& message) = 0;
    virtual std::string decodeIcao(const std::vector<uint8_t>& message) = 0;
};

#endif