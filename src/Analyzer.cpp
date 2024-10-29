#include "../include/Analyzer.h"

void Analyzer::analyze(const std::vector<uint8_t>& buffer) {
    message.initializeFromBuffer(buffer);
    // set Data struct
    setIcao(message.getIcao());
    setMessage(message.getMessage());
    // The plan: Create a Filter class and Filter here. ADSB will decode.
    // upon initialization message will set icao / df / tc
    if(filter.filterIdMsg(message.getDownlinkFormat(), message.getTypeCode())){                         // if is correct message type 
        if(storedIcaos.find(message.getIcao()) == storedIcaos.end()){  // if icao is not known
            message.extractCallsign(message.getMessage());
            setCallsign(message.getCallsign());
            storedIcaos[message.getIcao()].callsign = message.getCallsign();
            
            std::cout << "Initial msg ICAO (" << message.getIcao() << "), Callsign (" << message.getCallsign() << "): ";
            print_hex(message.getMessage());
            }  
    }else if(filter.filterVelocityMsg(message.getDownlinkFormat(), message.getTypeCode())){
        if(storedIcaos.find(message.getIcao()) != storedIcaos.end()){  // if icao is not known
            message.extractOe(message.getMessage());
            setOe(message.getOddEven());

            std::cout << "Related message ICAO (" << message.getIcao() << "), Callsign (" 
                 << storedIcaos[message.getIcao()].callsign << ") (" << "OE (" << static_cast<int>(getOe()) << ") : ";
                print_hex(buffer);
        }
    }
}

/* Format Functions */
std::vector<uint8_t> Analyzer::hexStringToVector(std::string hexString){
    size_t length = hexString.length();
    std::vector<uint8_t> byteVector;
    // ensure is multiple of 2
    if(length % 2 != 0){
        std::cerr << "not a multiple of 2";
        return byteVector; // return empty vector
    }
    for(size_t i = 0; i < length; i += 2){
        std::string byteString = hexString.substr(i, 2); // get next two characters
        uint8_t byte = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16)); // convert to long the cast uint8_t
        byteVector.push_back(byte);
    }
    return byteVector;
}

std::string Analyzer::trim(std::string hexString){
    size_t first = hexString.find_first_not_of(" \t\r\n*");
    size_t last = hexString.find_last_not_of(" \t\r\n;");
    return (first == std::string::npos) ? "" : hexString.substr(first, last - first + 1);
}

std::vector<uint8_t> Analyzer::strToByteVec(std::string hexStr){
    size_t length = hexStr.size();
    std::vector<uint8_t> byteVector;
    // check that there is even amount of chars
    if(length % 2 != 0){
        std::cerr << "not correct format";
        return byteVector; // return empty vec if not correct format
    }    
    // conversion
    for(int i = 0; i < length; i +=2){
        // first grap first two characters
        std::string byte = hexStr.substr(i, 2);
        // convert to uint8_t
        uint8_t byteStr = static_cast<uint8_t>(std::stoul(byte, nullptr, 16));
        // now add to vector
        byteVector.push_back(byteStr);
    }
    return byteVector;
}

void Analyzer::print_hex(const std::vector<uint8_t>& buffer) const {
    size_t size = buffer.size();
    for(int i = 0; i < size; i++){
        std::cout << std::hex << 
        std::uppercase << std::setw(2) <<
        std::setfill('0') << static_cast<int>(buffer[i]);
    }
    std::cout << std::endl;
}

/* Set Functions */
void Analyzer::setIcao(const std::string& icao){
    data.icao = icao;
}

void Analyzer::setMessage(const std::vector<uint8_t>& message){
    data.message = message;
}

void Analyzer::setCallsign(const std::string& callsign){
    data.callsign = callsign;
}
    
void Analyzer::setOe(const uint8_t oe){
    data.oe = oe;
}

/* Get Functions */
const std::string& Analyzer::getIcao() const{
    return data.icao;
}

const std::vector<uint8_t>& Analyzer::getMessage() const{
    return data.message;
}

const std::string& Analyzer::getCallsign() const{
    return data.callsign;
}

const uint8_t Analyzer::getOe() const{
    return data.oe;
}