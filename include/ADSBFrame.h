#ifndef ADSBFRAME_H
#define ADSBFRAME_H

#include <vector>
#include <iostream>
#include <cstdint>
#include <iomanip>
#include "../include/Filter.h"

class ADSBFrame {
private:
    uint8_t df;  // Downlink format
    uint8_t tc;  // Type code
    std::string callsign; // call sign
    std::string hexValue;
    std::string icao;
    std::vector<uint8_t> rawData;
    static const size_t FRAME_SIZE;
    Filter& filter;

public:

 // Constructor injects filter instance
    ADSBFrame(Filter& filterInstance)
        : filter(filterInstance), df(0), tc(0) {}

    // Method to initialize frame data from buffer
    void initializeFromBuffer(const std::vector<uint8_t>& buffer);



    bool filterByDownlinkFormat(uint8_t expectedDf);
    bool filterByTypeCode(uint8_t expectedTc);
    bool filterByTypeCode(uint8_t minTc, uint8_t maxTc);
    bool filterByIcao(const std::string& expectedIcao);
    bool filterByCallSign(const std::string& expectedCs);

    void setBuffer(const std::vector<uint8_t>& buffer);
    void setDownlinkFormat(uint8_t df);
    void setTypeCode(uint8_t tc);
    void setIcao(std::string icao);
    void setCallsign(std::string callsign);

    uint8_t getDownlinkFormat() const;
    uint8_t getTypeCode() const;
    std::string getIcao() const;
    std::string getCallSign() const;
    size_t getFrameSize() const;


    void printHex() const;
    void print_bits(uint8_t byte) const;
    void print_binary(const std::vector<uint8_t>& buffer, int len) const;
    void print_hex(const std::vector<uint8_t>& buffer);
};

#endif
