#ifndef ADSBMESSAGE_H
#define ADSBMESSAGE_H

#include <vector>
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <map>
#include <tuple>
#include <unordered_map> // for std::unordered_map
#include <unordered_set>
#include <set> // for std::set
#include "../include/Decoder.h"

class ADSBMessage {

private:
    Decoder& decoder;

    struct ADSBData {
    std::vector<uint8_t> message; // 14-byte ADSB message   
    uint8_t df;        // Downlink format
    uint8_t tc;        // Type code
    uint8_t oddEven;   // Odd/Even flag for velocity decoding
    std::string icao;  // ICAO address
    std::string callsign; // Call sign
};

    using Message = std::tuple<std::vector<uint8_t>, std::string, uint8_t>;
    using Pair = std::tuple<std::vector<uint8_t>, std::string, uint8_t>;
    ADSBData data;  // The struct instance for message-specific data
    std::vector<uint8_t> rawData;
    std::map<Message, Message> positionMsg; 
    std::unordered_map<std::string, ADSBData> icaoData;
    std::unordered_set<std::string> storedIcaos;
    static const size_t FRAME_SIZE;

public:

 // Constructor injects Decoder instance
    ADSBMessage(Decoder& decoderInstance)
        : decoder(decoderInstance) {}

    // Method to initialize frame data from buffer
    void initializeFromBuffer(const std::vector<uint8_t>& buffer);
    bool filterByDownlinkFormat(uint8_t expectedDf);
    bool filterByTypeCode(uint8_t expectedTc);
    bool filterByTypeCode(uint8_t minTc, uint8_t maxTc);
    bool filterByIcao(const std::string& expectedIcao);
    bool filterByCallSign(const std::string& expectedCs);
    std::tuple<std::vector<uint8_t>, std::vector<uint8_t>> filterLocation(ADSBData& message);

    void setBuffer(const std::vector<uint8_t>& buffer);
    void setDownlinkFormat(uint8_t df);
    void setTypeCode(uint8_t tc);
    void setIcao(std::string icao);
    void setCallsign(std::string callsign);
    void setOddEven(uint8_t oddEven);

    uint8_t getDownlinkFormat() const;
    uint8_t getTypeCode() const;
    std::string getIcao() const;
    std::string getCallSign() const;
    uint8_t getOddEven() const;
    size_t getFrameSize() const;

    void print_hex(const std::vector<uint8_t>& buffer);
};

#endif
