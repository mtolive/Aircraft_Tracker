#ifndef ADSBFRAME_H
#define ADSBFRAME_H

#include <vector>
#include <iostream>
#include <cstdint>
#include <iomanip>

class ADSBFrame {
private:
    uint8_t df;  // Downlink format
    uint8_t tc;  // Type code
    std::string callsign; // call sign
    std::string hexValue;
    std::vector<uint8_t> rawData;
    static const size_t FRAME_SIZE;

public:
    ADSBFrame(const std::vector<uint8_t>& buffer);
    void extractCallSign();
    uint8_t getDownlinkFormat() const;
    uint8_t getTypeCode() const;
    size_t getFrameSize() const;
    std::string getCallSign() const;
    void printHex() const;
    void print_bits(uint8_t byte) const;
    void print_binary(const std::vector<uint8_t>& buffer, int len) const;
};

#endif
