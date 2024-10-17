#include <iostream>
#include <vector>
#include <csignal>
#include <rtl-sdr.h>
#include <thread>
#include <chrono>
#include <iomanip>  // for hex formatting

#define BUFFER_LENGTH 262144  // 256KB buffer size
#define SAMPLE_RATE 2048000  // 2 MHz, typical for ADS-B
#define FREQUENCY 1090000000 // 1090 MHz for ADS-B signals
#define FRAME_SIZE 14        // ADS-B frame size

rtlsdr_dev_t* dev = nullptr;  // RTL-SDR device pointer
volatile std::sig_atomic_t stop = 0;

// Handle Ctrl+C signal to stop
void handle_sigint(int signum) {
    std::cout << "Stopping RTL-SDR capture..." << std::endl;
    stop = 1;
    rtlsdr_cancel_async(dev);  // Safely stop the async capture
}

// Print hex representation of the buffer
void print_hex(const unsigned char* buffer, int len) {
    for (int i = 0; i < len; i++) {
        std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]);
    }
}

// Check for downlink format 17 and typecode 4
bool is_adsb_message(const unsigned char* buffer) {
    uint8_t df = (buffer[0] >> 3) & 0x1F;  // Extract downlink format (DF)
    uint8_t tc = (buffer[4] >> 3) & 0x1F;  // Extract type code (TC)

    return (df == 17 && tc == 4);
}

// Callback function to process the samples
void rtl_callback(unsigned char* buffer, uint32_t length, void* ctx) {

    if (length >= FRAME_SIZE) {
        // Print the first few bytes to get a sense of the data
       // std::cout << "Raw Data: ";
       uint8_t df = (buffer[0] >> 3) & 0x1F;
       uint8_t tc = (buffer[4] >> 3) & 0x1F;  // Extract type code (TC)

        if(df == 17 && tc == 4){
            print_hex(buffer, FRAME_SIZE);
            std::cout << std::endl;
        }
    }
}



int main() {
    // Setup signal handler
    std::signal(SIGINT, handle_sigint);

    // Open the RTL-SDR device
    int device_index = 0;  // Default to the first device
    if (rtlsdr_open(&dev, device_index) < 0) {
        std::cerr << "Failed to open RTL-SDR device" << std::endl;
        return -1;
    }

    // Set sample rate
    rtlsdr_set_sample_rate(dev, SAMPLE_RATE);
    // Set the frequency to 1090 MHz (ADS-B)
    rtlsdr_set_center_freq(dev, FREQUENCY);
    // Set automatic gain control (AGC) mode or specific gain
    rtlsdr_set_agc_mode(dev, 1);

    rtlsdr_reset_buffer(dev);
    // rtlsdr_set_agc_mode(dev, 1);
     

    // Start capturing samples asynchronously
    /*std::cout << "Starting async read..." << std::endl;
    std::cout << "Sample Rate: " << rtlsdr_get_sample_rate(dev) << std::endl;
    std::cout << "Frequency: " << rtlsdr_get_center_freq(dev) << std::endl;
    std::cout << "Gain: " << rtlsdr_get_tuner_gain(dev) << std::endl;
    

    std::cout << "Async read started." << std::endl;
*/
    int result = rtlsdr_read_async(dev, rtl_callback, nullptr, 0, BUFFER_LENGTH);
    if (result < 0) {
        std::cerr << "Async read failed with error: " << result << std::endl;

    }


    // Wait for Ctrl+C to stop the capture
    while (!stop) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Stop the async read and close the device
    rtlsdr_cancel_async(dev);
    rtlsdr_close(dev);

    return 0;
}
