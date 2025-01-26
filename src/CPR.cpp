#include "../include/CPR.h"

/*
    for both even and odd timestamps: auto = std::chrono::time_point<std::chrono::system_clock>
    extractBits(message, 54,54); to get oddEven
*/

// private
ADSB_Message msg;
uint8_t NZ = 15;
double lat = 0.0;
double DLatE = (360/60);
double DLatO = (360/59);
double numerator = (2*M_PI);
double denominator = acos(
    1 - (1-cos(M_PI / (2*NZ))) /
        (pow(cos((M_PI/180)*lat), 2))
                           
);

std::tuple<long, long> CPR::decodePosition(std::string odd, std::string even, auto oddTimeStamp, auto evenTimeStamp){
    std::tuple<double, double> latLon = std::make_tuple(0.0, 0.0);

    return latLon;
}

std::tuple<long, long> CPR::decodePosition(std::vector<uint8_t> odd, std::vector<uint8_t> even, auto oddTimeStamp, auto evenTimeStamp){
    std::tuple<double, double> latLon = std::make_tuple(0.0, 0.0);

    return latLon;
}

uint8_t CPR::NL(double coordinate){
/*
    Denotes the "numberj of longitude zones", function, given the latitude angle lat
    The returned integer value is constrained within [1,59], calculated as:
*/
if(!getDenominator())
    return floor(getNumerator()/getDenominator());
else
    return 0;
}

// computer NI for longitude - accepts either latEven or latOdd
double CPR::computeNI(double lat){
    double nl = NL(lat);
    if(nl > 1){
        return nl;
    }else
        return 1;
}

double CPR::computeDLon(double ni){
    return 360/ni;
}

// computes M for longitude - accepts either latEven or latOdd
double CPR::computeM(double LON_CPR_EVEN, double LON_CPR_ODD, double lat){
    return floor((LON_CPR_EVEN*(NL(lat) - 1)) - (LON_CPR_ODD*(NL(lat) + 0.5)));
}
// LON_CPR depending if even or odd
double CPR::longitude(double DLon, double m, double ni, double LON_CPR){
    return DLon*(mod(m,ni) + LON_CPR);
}


// if time1 is greater than time2
bool CPR::timeIsGreater(const auto time1, const auto time2){
        return time1 > time2;

}

double CPR::timeDifferenceInSeconds(const auto timestamp1, const auto timestamp2){
    auto duration =  timestamp1 - timestamp2;
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}

double CPR::mod(double x, double y){
    if(y != 0)
        return (x-y)*floor(x/y);
    else    
        return 0;
}

/////////////////////////
/////////////////////////
double CPR::getLatEven(){
    return LAT_CPR_EVEN;
}
double CPR::getLatOdd(){
    return LAT_CPR_ODD;
}
double CPR::getLonEven(){
    return LON_CPR_EVEN;
}
double CPR::getLonOdd(){
    return LON_CPR_ODD;
}
double CPR::getNumerator(){
    return numerator;
}
double CPR::getDenominator(){
    return denominator;
}
double CPR::getLat(){
    return lat;
}

/*
    LAT-CPR = bits [55,71] (17)
    LON-CPR = bits [72,88] (17)
*/
void CPR::setLatEven(const std::vector<uint8_t>& message){
    // check if even
    auto oddEven = extractor.getBitRange(54,54, message);
    if(oddEven[0]){
        std::vector<uint8_t> latEven = extractor.getBitRange(55,71,message);
    }

}
void CPR::setLatOdd(const std::vector<uint8_t>& message){
    // check if odd
    auto oddEven = extractor.getBitRange(54,54, message);
    if(!oddEven[0]){
        std::vector<uint8_t> latEven = extractor.getBitRange(55,71,message);
    }

}
void CPR::setLonEven(const std::vector<uint8_t>& message){
    // check if even
    auto oddEven = extractor.getBitRange(54,54, message);
    if(oddEven[0]){
        std::vector<uint8_t> latEven = extractor.getBitRange(72,88,message);
    }

}
void CPR::setLonOdd(const std::vector<uint8_t>& message){
    // check if odd
    auto oddEven = extractor.getBitRange(54,54, message);
    if(!oddEven[0]){
        std::vector<uint8_t> latEven = extractor.getBitRange(72,88,message);
    }

}
