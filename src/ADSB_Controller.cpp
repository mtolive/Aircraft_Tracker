#include "../include/ADSB_Controller.h"

// Public
void ADSB_Controller::analyze(const std::vector<uint8_t>& buffer) {
    message.initializeFromBuffer(buffer);    // init ADSB message

    if(filter.filterIdMsg(message.getDownlinkFormat(), message.getTypeCode())){
        analyzeIdMsg(message);        
    }else if(filter.filterPosMsg(message.getDownlinkFormat(), message.getTypeCode())){
       analyzePosMsg(message);
    }else if(filter.filterVelocityMsg(message.getDownlinkFormat(), message.getTypeCode())){
       analyzeVelMsg(message);
    }
}

// Private
/* Initialize Data */
// Initizlize Data struct
ADSB_Controller::Data ADSB_Controller::initData(ADSB_Message& message){
    Data data;
    data.icao = message.getIcao();
    data.message = message.getMessage();
    data.timestamp = std::chrono::system_clock::now();
    return data;
}
// Initialize Even Data struct - Initialize first half of tuple[0]
void ADSB_Controller::initEvenMsg(ADSB_Message& message, std::tuple<Data, Data>& tuple) {
    auto& [evenData, _] = tuple;
    evenData.oe = message.getOddEven();
    evenData.message = message.getMessage();
    evenData.icao = message.getIcao();
}
// Initialize Odd Data struct - Initialize second half of tuple[1]
void ADSB_Controller::initOddMsg(ADSB_Message& message, std::tuple<Data, Data>& tuple) {
    auto& [_, oddData] = tuple;
    oddData.oe = message.getOddEven();
    oddData.message = message.getMessage();
    oddData.icao = message.getIcao();
}

/* Analyze Message Functions */
/* Store the unique ICAO as well as call sign, if ICAO is no known. We are only displaying 
** data where a call sign is first identified. The message arg must be downlink 17, with 
** type code 1-4 inclusive.
*/
const std::vector<uint8_t>& ADSB_Controller::analyzeIdMsg(ADSB_Message& message){
    if(storedIcaos.find(message.getIcao()) == storedIcaos.end()){  // if icao is not known
        auto data = initData(message);
        message.extractCallsign(message.getMessage());
        data.callsign = message.getCallsign();
        storedIcaos[message.getIcao()] = data;
        print_hex(message.getMessage());
    }
    return message.getMessage();  
}

/* Store odd and even message position messages if ICAO is known. Position messages
** must have downlink == 17, as well as typecode 9-18 inclusive. To decode a position
** message we first need an even message then pair it with an odd message of common ICAO.
** When the decoding logic is added in, we'll also add in timestamps. Finally once a pair
** is made we need to clear the posMessages umap to keep messages up to date. 
*/
const std::vector<uint8_t>& ADSB_Controller::analyzePosMsg(ADSB_Message& message) { 
    if (storedIcaos.find(message.getIcao()) != storedIcaos.end()) { // if ICAO is known
        message.extractOe(message.getMessage()); // Determine if odd or even
        if (!message.getOddEven()) { // If message is even
            Data evenData = initData(message);
            posMessages[message.getIcao()] = std::make_tuple(evenData, Data{}); // Default odd data
            initEvenMsg(message, posMessages[message.getIcao()]);               
        } else { // If message is odd
            auto& [evenData, oddData] = posMessages[message.getIcao()];
            if (evenData.icao == message.getIcao()) {  // Only process if evenData has been initialized
                initOddMsg(message, posMessages[message.getIcao()]);
                print_hex(evenData.message);
                print_hex(oddData.message);  
                posMessages.erase(message.getIcao());  // clear       
            } 
        }
    }
    return message.getMessage(); // Consider changing this to void if not necessary
}

/*
** Velocity messages have to be downlink == 17 and typecode == 19
** Decoding will be implemented soon. Now message is sent to python 
** GUI to be decoded my pyModeS
*/
const std::vector<uint8_t>& ADSB_Controller::analyzeVelMsg(ADSB_Message& message){
    if(storedIcaos.find(message.getIcao()) != storedIcaos.end()){
        print_hex(message.getMessage());        
    }
    return message.getMessage();
}

// for future use in decoding position messages
double ADSB_Controller::timeDifferenceInSeconds(const Data& data1, const Data& data2) {
    auto duration = data2.timestamp - data1.timestamp;
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}

// Public Functions
/* Format Functions */
std::vector<uint8_t> ADSB_Controller::hexStringToVector(std::string hexString){
    size_t length = hexString.length();
    std::vector<uint8_t> byteVector;
    if(length % 2 != 0){     // ensure is multiple of 2
        std::cerr << "not a multiple of 2";
        return byteVector;   // return empty vector
    }
    for(size_t i = 0; i < length; i += 2){
        std::string byteString = hexString.substr(i, 2); // get next two characters
        uint8_t byte = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16)); // convert to long the cast uint8_t
        byteVector.push_back(byte);
    }
    return byteVector;
}

// rtl_adsb ouput is: *hexvalues; we will remove * and ; from first/last
std::string ADSB_Controller::trim(std::string hexString){
    size_t first = hexString.find_first_not_of(" \t\r\n*");
    size_t last = hexString.find_last_not_of(" \t\r\n;");
    return (first == std::string::npos) ? "" : hexString.substr(first, last - first + 1);
}

std::vector<uint8_t> ADSB_Controller::strToByteVec(std::string hexStr){
    size_t length = hexStr.size();
    std::vector<uint8_t> byteVector;
    if(length % 2 != 0){   // check that there is even amount of chars
        std::cerr << "not correct format";
        return byteVector; // return empty vec if not correct format
    }    
    for(int i = 0; i < length; i +=2){  // conversion
        std::string byte = hexStr.substr(i, 2);
        uint8_t byteStr = static_cast<uint8_t>(std::stoul(byte, nullptr, 16));  // convert to uint8_t
        byteVector.push_back(byteStr);
    }
    return byteVector;
}

// Print hex from vector<uint8_T>
void ADSB_Controller::print_hex(const std::vector<uint8_t>& buffer) const {
    size_t size = buffer.size();
    for(int i = 0; i < size; i++){
        std::cout << std::hex << 
        std::uppercase << std::setw(2) <<
        std::setfill('0') << static_cast<int>(buffer[i]);
    }
    std::cout << std::endl;
}