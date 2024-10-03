#ifndef FILTER_H
#define FILTER_H

#include "../include/ADSBFrame.h"

class Filter {
public:
    // Filter for a specific Downlink Format (DF)
    static bool filterByDownlinkFormat(const ADSBFrame& frame, uint8_t expectedDf);

    // Filter for a specific Type Code (TC)
    static bool filterByTypeCode(const ADSBFrame& frame, uint8_t expectedTc);

    // Filter for a specific Type Code Range (TC)
    static bool filterByTypeCode(const ADSBFrame& frame, uint8_t minTc, uint8_t maxTc);

    // Filter by Callsign
    static bool filterByCallSign(const ADSBFrame& frame, const std::string& expectedCs);

};

#endif // FILTER_H
