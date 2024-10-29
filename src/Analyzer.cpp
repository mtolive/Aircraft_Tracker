#include "../include/Analyzer.h"

void Analyzer::analyze(const std::vector<uint8_t>& buffer) {
    message.initializeFromBuffer(buffer);    // init ADSB message
    //nitData(message);                       // init data structure
    // If it is ID message type, and is NOT known
    if(filter.filterIdMsg(message.getDownlinkFormat(), message.getTypeCode())){
        analyzeIdMsg(message);        
    // Else if is velocity message type, and IS known
    }else if(filter.filterPosMsg(message.getDownlinkFormat(), message.getTypeCode())){
        // if message is Even '0', then store position message in map
       analyzePosMsg(message);
    }
}

/* Initialize Data */
Analyzer::Data Analyzer::initData(ADSBMessage& message){
    Data data;
    data.icao = message.getIcao();
    data.message = message.getMessage();
    data.timestamp = std::chrono::system_clock::now();
    return data;
}

void Analyzer::initEvenMsg(ADSBMessage& message, std::tuple<Data, Data>& tuple) {
    Data& evenData = std::get<0>(tuple);
    evenData.oe = message.getOddEven(); // Ensure this reflects the message state
    evenData.message = message.getMessage();
    evenData.icao = message.getIcao();
}

void Analyzer::initOddMsg(ADSBMessage& message, std::tuple<Data, Data>& tuple) {
    Data& oddData = std::get<1>(tuple);
    oddData.oe = message.getOddEven(); // Should be consistent with message state
    oddData.message = message.getMessage();
    oddData.icao = message.getIcao();
}

/* Analyze Message Function */
const std::vector<uint8_t>& Analyzer::analyzeIdMsg(ADSBMessage& message){
    if(storedIcaos.find(message.getIcao()) == storedIcaos.end()){  // if icao is not known
        auto data = initData(message);
        message.extractCallsign(message.getMessage());
        data.callsign = message.getCallsign();
        storedIcaos[message.getIcao()] = data;
    }
    return message.getMessage();  
}

const std::vector<uint8_t>& Analyzer::analyzePosMsg(ADSBMessage& message) { 
    if (storedIcaos.find(message.getIcao()) != storedIcaos.end()) {
        message.extractOe(message.getMessage()); // Ensure this is working as expected
        if (!message.getOddEven()) { // If message is even
            Data evenData = initData(message);
            posMessages[message.getIcao()] = std::make_tuple(evenData, Data{}); // Default odd data
            initEvenMsg(message, posMessages[message.getIcao()]);               
        } else { // If message is odd
            auto& [evenData, oddData] = posMessages[message.getIcao()];
            // Only process if evenData has been initialized
            if (evenData.icao == message.getIcao()) { 
                initOddMsg(message, posMessages[message.getIcao()]);
                std::cout << "Even Data - OE: " << static_cast<int>(evenData.oe)
                          << ", ICAO: " << evenData.icao << ", CS: " << storedIcaos[message.getIcao()].callsign 
                          << " , Message: ";
                print_hex(evenData.message);
                
                std::cout << "Odd Data - OE: " << static_cast<int>(oddData.oe)
                          << ", ICAO: " << oddData.icao<< ", CS: " << storedIcaos[message.getIcao()].callsign
                           << " , Message: ";
                print_hex(oddData.message);
                posMessages.erase(message.getIcao());            
            } 
        }
    }
    return message.getMessage(); // Consider changing this to void if not necessary
}

const std::vector<uint8_t>& Analyzer::analyzeVelMsg(ADSBMessage& message){
    return message.getMessage();
}

double Analyzer::timeDifferenceInSeconds(const Data& data1, const Data& data2) {
    auto duration = data2.timestamp - data1.timestamp;
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}

/* Public: */
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