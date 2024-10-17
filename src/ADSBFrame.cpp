#include "../include/ADSBFrame.h"

const size_t ADSBFrame::FRAME_SIZE = 14;

void ADSBFrame::initializeFromBuffer(const std::vector<uint8_t>& buffer) {
    if (buffer.size() >= getFrameSize()) {
        setBuffer(buffer);
        df = filter.extractDownlinkFormat(buffer); // Use Filter to extract DF
        tc = filter.extractTypeCode(buffer);        // Use Filter to extract TC
        icao = filter.extractIcao(buffer);
        std::cout << "df = " << static_cast<int>(df) << std::endl;
        std::cout << "tc = " << static_cast<int>(tc) << std::endl;
        std::cout << "icao = " << icao << std::endl;

        if (filterByDownlinkFormat(17) && filterByTypeCode(1,4)) {
            callsign = filter.decodeCallSign(buffer); // Extract callsign using its own method
        }
    } else {
        throw std::invalid_argument("Invalid frame size");
    }
}

/*Filter functions*/

// Filter for a specific Downlink Format (DF)
bool ADSBFrame::filterByDownlinkFormat(uint8_t expectedDf) {
    return getDownlinkFormat() == expectedDf;
}

// Filter for a specific Type Code (TC)
bool ADSBFrame::filterByTypeCode(uint8_t expectedTc) {
    return getTypeCode() == expectedTc;
}

// Filter for a specific Type Code (TC) within a range
bool ADSBFrame::filterByTypeCode(uint8_t minTc, uint8_t maxTc) {
    uint8_t tc = getTypeCode();
    return (tc >= minTc && tc <= maxTc); // Check if TC is within the range
}

// Filter for a specific ICAO address (ICAO)
bool ADSBFrame::filterByIcao(const std::string& expectedIcao){
    const std::string& icao = getIcao(); // Get the icao
    return icao.find(expectedIcao) != std::string::npos; // Check if expectedCs is part of ICAO   
}

// Filter by CallSign
bool ADSBFrame::filterByCallSign(const std::string& expectedCs) {
    const std::string& callsign = getCallSign(); // Get the callsign from the frame
    return callsign.find(expectedCs) != std::string::npos; // Check if expectedCs is part of callsign
}


/**** Setter Functions ****/
void ADSBFrame::setBuffer(const std::vector<uint8_t>& buffer) {
    rawData = buffer; // Access the rawData member variable
}

void ADSBFrame::setDownlinkFormat(uint8_t df) {
    this->df = df; // 'this' is valid here in a non-static member function
}

void ADSBFrame::setTypeCode(uint8_t tc) {
    this->tc = tc; // 'this' is valid here
}

void ADSBFrame::setIcao(std::string icao) {
    this->icao = std::move(icao); // 'this' is valid here
}

void ADSBFrame::setCallsign(std::string callsign) {
    this->callsign = std::move(callsign); // 'this' is valid here
}



/**** Getter Functions ****/
uint8_t ADSBFrame::getDownlinkFormat() const {
    return df;
}

uint8_t ADSBFrame::getTypeCode() const {
    return tc;
}

std::string ADSBFrame::getIcao() const {
    return icao;
}

std::string ADSBFrame::getCallSign() const {
    return callsign;
}

size_t ADSBFrame::getFrameSize() const {
    return FRAME_SIZE;
}

/**** Print Functions ****/

void ADSBFrame::printHex() const {
    for (size_t i = 0; i < rawData.size(); i++) {
        std::cout << std::hex << std::uppercase << std::setw(2) 
                  << std::setfill('0') << static_cast<int>(rawData[i]);

        // Print new line after every 14 bytes
        if ((i + 1) % 14 == 0) {
            std::cout << std::endl; // Print a new line after every 14 bytes
        }
    }
    if (rawData.size() % 14 != 0) {
        std::cout << std::endl; // Final newline if needed
    }
}

void ADSBFrame::print_bits(uint8_t byte) const {
    for (size_t i = 7; i < 8; i--) {
        std::cout << ((byte >> i) & 1);
    }
}

void ADSBFrame::print_binary(const std::vector<uint8_t>& buffer, int len) const {
    for (size_t i = 0; i < len; i++) {
        print_bits(buffer[i]); // Pass in each byte
        std::cout << " ";
    }
    std::cout << std::endl;
}

void ADSBFrame::print_hex(const std::vector<uint8_t>& buffer) {
    for (int i = 0; i < buffer.size(); i++) {
        std::cout << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]);

        // Add a newline after every 14 bytes
        if ((i + 1) % 14 == 0) {
            std::cout << std::endl;
        }
    }
    if (buffer.size() % 14 != 0) {
        std::cout << std::endl;
    }
}
