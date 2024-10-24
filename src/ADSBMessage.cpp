#include "../include/ADSBMessage.h"

const size_t ADSBMessage::FRAME_SIZE = 14;

/*
std::unordered_map<std::string, ADSBMessage> icaoData;
std::unordered_set<std::string> storedIcaos;
*/
void ADSBMessage::initializeFromBuffer(const std::vector<uint8_t>& buffer) {
    if (buffer.size() >= getFrameSize()) {
        setBuffer(buffer);
        setDownlinkFormat(decoder.decodeDownlinkFormat(buffer));
        setTypeCode(decoder.decodeTypeCode(buffer));
        setIcao(decoder.decodeIcao(buffer));

        if (filterByDownlinkFormat(17) && filterByTypeCode(1, 4)) {
            setCallsign(decoder.decodeCallSign(buffer));
            icaoData[data.icao].callsign = getCallSign();
            std::cout << "Initial msg ICAO (" << getIcao() << "), Callsign (" << getCallSign() << "): ";
            print_hex(buffer);
        } else if (filterByDownlinkFormat(17) && filterByTypeCode(9, 18)) {
            if (icaoData.find(data.icao) != icaoData.end()) {
                setOddEven(decoder.decodeOddEven(buffer));
                std::cout << "Related message ICAO (" << getIcao() << "), Callsign (" 
                          << icaoData[data.icao].callsign << ") (" << "OE (" << static_cast<int>(getOddEven()) << ") : ";
                print_hex(buffer);
            }
        }
    } else {
        throw std::invalid_argument("Invalid frame size");
    }
}

/*Filter functions*/

// add in map logic -- possibly move to new class
std::tuple<std::vector<uint8_t>, std::vector<uint8_t>> ADSBMessage::filterLocation(ADSBData& data){
    // auto = std::tuple<std::vector<uint8_t>, std::vector<uint8_t>>
    auto location = std::make_tuple(std::vector<uint8_t>{}, std::vector<uint8_t>{});

    // Filter typecode and downlink format
    if(filterByDownlinkFormat(17) && filterByTypeCode(9,18)){
        // Check if ICAO is stored
        if(icaoData.find(data.icao) != icaoData.end()){
            // find if odd or even
            data.oddEven = decoder.decodeOddEven(data.message);
            // if even
            if(!data.oddEven){
                 Message even = std::make_tuple(std::vector<uint8_t>{}, "", 0);
                 std::get<0>(even) = data.message;
                 std::get<1>(even) = data.icao;
                 std::get<2>(even) = data.oddEven;


            }else if(data.oddEven){
                 Message odd = std::make_tuple(std::vector<uint8_t>{}, "", 0);
                 std::get<0>(odd) = data.message;
                 std::get<1>(odd) = data.icao;
                 std::get<2>(odd) = data.oddEven;
            }else{
                std::cerr << "No value for data.oddEven";
            }
        }    
    }
    return location;    
}

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

/**** Set Functions ****/
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

void ADSBMessage::setOddEven(uint8_t oddEven){
    data.oddEven = oddEven;
}

/**** Get Functions ****/
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

uint8_t ADSBMessage::getOddEven() const{
    return data.oddEven;    
}

size_t ADSBMessage::getFrameSize() const {
    return FRAME_SIZE; // Assuming this is a class constant and hasn't changed
}

/**** Print Functions ****/
void ADSBMessage::print_hex(const std::vector<uint8_t>& buffer) {
    for (int i = 0; i < buffer.size(); i++) {
        std::cout << std::hex << std::uppercase << std::setw(2) 
                  << std::setfill('0') << static_cast<int>(buffer[i]);

        // Add a newline after every 14 bytes
        }
        std::cout << std::dec << std::endl;
}
