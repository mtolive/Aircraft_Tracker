#include "../include/Filter.h"

    // Filter for a specific Downlink Format (DF)
    bool Filter::filterByDownlinkFormat(const ADSBFrame& frame, uint8_t expectedDf) {
        return frame.getDownlinkFormat() == expectedDf;
    }

    // Filter for a specific Type Code (TC)
    bool Filter::filterByTypeCode(const ADSBFrame& frame, uint8_t expectedTc) {
        return frame.getTypeCode() == expectedTc;
    }

    // Filter for a specific Type Code (TC) within a range
    bool Filter::filterByTypeCode(const ADSBFrame& frame, uint8_t minTc, uint8_t maxTc) {
        uint8_t tc = frame.getTypeCode(); // Get the Type Code from the frame
        return (tc >= minTc && tc <= maxTc); // Check if TC is within the range
    }


   bool Filter::filterByCallSign(const ADSBFrame& frame, const std::string& expectedCs) {
        const std::string& callsign = frame.getCallSign(); // Get the callsign from the frame
        return callsign.find(expectedCs) != std::string::npos; // Check if expectedCs is part of callsign
    }


