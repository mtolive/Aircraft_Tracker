#include "../include/ADSBMessage.h"

const size_t ADSBMessage::FRAME_SIZE = 14;

/*
std::unordered_map<std::string, ADSBMessage> icaoData;
std::unordered_set<std::string> storedIcaos;
*/
void ADSBMessage::initializeFromBuffer(const std::vector<uint8_t>& buffer) {
    if (buffer.size() >= getFrameSize()) {
        setBuffer(buffer);
        data.df = decoder.decodeDownlinkFormat(buffer);
        data.tc = decoder.decodeTypeCode(buffer);
        data.icao = decoder.decodeIcao(buffer);

        if (filterByDownlinkFormat(17) && filterByTypeCode(1, 4)) {
            data.callsign = decoder.decodeCallSign(buffer);
            icaoData[data.icao].callsign = getCallSign();
            std::cout << "Initial msg (" << getCallSign() << ") :";
            print_hex(buffer);
        } else if (filterByDownlinkFormat(17) && filterByTypeCode(9, 22)) {
            if (icaoData.find(data.icao) != icaoData.end()) {
                std::cout << "Related message: (" << getCallSign() <<") ";
                print_hex(buffer);
            }
        }
    } else {
        throw std::invalid_argument("Invalid frame size");
    }
}


/*Filter functions*/

bool ADSBMessage::filterByDownlinkFormat(uint8_t expectedDf) {
    return data.df == expectedDf;
}

bool ADSBMessage::filterByTypeCode(uint8_t expectedTc) {
    return data.tc == expectedTc;
}

bool ADSBMessage::filterByTypeCode(uint8_t minTc, uint8_t maxTc) {
    return data.tc >= minTc && data.tc <= maxTc;
}

bool ADSBMessage::filterByIcao(const std::string& expectedIcao) {
    return data.icao.find(expectedIcao) != std::string::npos;
}

bool ADSBMessage::filterByCallSign(const std::string& expectedCs) {
    return data.callsign.find(expectedCs) != std::string::npos;
}



/**** Setter Functions ****/
void ADSBMessage::setBuffer(const std::vector<uint8_t>& buffer) {
    rawData = buffer; // rawData is still a class member variable
}

void ADSBMessage::setDownlinkFormat(uint8_t df) {
    data.df = df; // Access the df member inside the ADSBData struct
}

void ADSBMessage::setTypeCode(uint8_t tc) {
    data.tc = tc; // Access the tc member inside the ADSBData struct
}

void ADSBMessage::setIcao(std::string icao) {
    data.icao = std::move(icao); // Access the icao member inside the ADSBData struct
}

void ADSBMessage::setCallsign(std::string callsign) {
    data.callsign = std::move(callsign); // Access the callsign member inside the ADSBData struct
}

/**** Getter Functions ****/
uint8_t ADSBMessage::getDownlinkFormat() const {
    return data.df; // Access the df member inside the ADSBData struct
}

uint8_t ADSBMessage::getTypeCode() const {
    return data.tc; // Access the tc member inside the ADSBData struct
}

std::string ADSBMessage::getIcao() const {
    return data.icao; // Access the icao member inside the ADSBData struct
}

std::string ADSBMessage::getCallSign() const {
    return data.callsign; // Access the callsign member inside the ADSBData struct
}

size_t ADSBMessage::getFrameSize() const {
    return FRAME_SIZE; // Assuming this is a class constant and hasn't changed
}

/**** Print Functions ****/

void ADSBMessage::printHex() const {
    for (size_t i = 0; i < rawData.size(); i++) {
        std::cout << std::hex << std::uppercase << std::setw(2) 
                  << std::setfill('0') << static_cast<int>(rawData[i]);

        // Print new line after every 14 bytes
        if ((i + 1) % 14 == 0) {
            std::cout << std::endl; // Print a new line after every 14 bytes
        }
    }
    if (rawData.size() % 14 != 0) {
        std::cout << std::dec << std::endl; // Final newline if needed
    }
}

void ADSBMessage::print_bits(uint8_t byte) const {
    for (size_t i = 7; i < 8; i--) {
        std::cout << ((byte >> i) & 1);
    }
}

void ADSBMessage::print_binary(const std::vector<uint8_t>& buffer, int len) const {
    for (size_t i = 0; i < len; i++) {
        print_bits(buffer[i]); // Pass in each byte
        std::cout << " ";
    }
    std::cout << std::endl;
}

void ADSBMessage::print_hex(const std::vector<uint8_t>& buffer) {
    for (int i = 0; i < buffer.size(); i++) {
        std::cout << std::hex << std::uppercase << std::setw(2) 
                  << std::setfill('0') << static_cast<int>(buffer[i]);

        // Add a newline after every 14 bytes
        }
        std::cout << std::dec << std::endl;
}
