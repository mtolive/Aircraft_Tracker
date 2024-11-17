#ifndef ADSB_MESSAGE_H
#define ADSB_MESSAGE_H

#include <vector>
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <map>
#include <tuple>
#include <unordered_map> // for std::unordered_map
#include <unordered_set>
#include <set> // for std::set

#include "../include/ADSB_Decoder.h"
#include "/home/m/Aircraft_Tracker/include/interfaces/ModeS_Message.h"


class ADSB_Message : public ModeS_Message {
private:
    ADSB_Decoder& decoder;
    std::vector<uint8_t> message; // 14-byte ADSB message   
    uint8_t df;        // Downlink format
    uint8_t tc;        // Type code
    uint8_t oddEven;   // Odd/Even flag for velocity decoding
    std::string icao;  // ICAO address
    std::string callsign; // Call sign
    static const size_t FRAME_SIZE;

public:
 // Constructor injects Decoder instance
    ADSB_Message(ADSB_Decoder& decoderInstance) : decoder(decoderInstance) {}
    /* Initialize DF/TC/ICAO */
    void initializeFromBuffer(const std::vector<uint8_t>& buffer);
    
    /* Extract Methods */
    void extractIcao(const std::vector<uint8_t>& message);
    void extractTc(const std::vector<uint8_t>& message);
    void extractDf(const std::vector<uint8_t>& message);
    void extractOe(const std::vector<uint8_t>& message);
    void extractCallsign(const std::vector<uint8_t>& message);
    
    /* Set Methods */
    void setBuffer(const std::vector<uint8_t>& buffer);
    void setDownlinkFormat(const uint8_t df);
    void setTypeCode(const uint8_t tc);
    void setIcao(const std::string& icao);
    void setCallsign(const std::string& callsign);
    void setOddEven(const uint8_t oddEven);
    
    /* Get Methods */
    const std::vector<uint8_t>& getMessage() const;
    const uint8_t getDownlinkFormat() const;
    const uint8_t getTypeCode() const;
    const std::string& getIcao() const;
    const std::string& getCallsign() const;
    const uint8_t getOddEven() const;
    const size_t getFrameSize() const;
};
#endif
