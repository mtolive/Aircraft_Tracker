/*#include <iostream>
#include <vector>
#include <csignal>
#include <unistd.h>
#include <iomanip>  // for hex formatting
#include <cstdint>

#define BUFFER_SIZE 16384  // 16KB
#define FRAME_SIZE 14 // ADS-B frame size is 14 bytes

// handle exit
volatile std::sig_atomic_t stop = 0;

void handle_sigint(int signum){
   stop = 1;
}

// right shift i to least significant bit mask using 1.
void print_bits(uint8_t byte) {
   for(int i = 7; i >= 0; i--){
      std::cout << ((byte >> i) & 1);
   }
}

// print bits
void print_bin(const std::vector<uint8_t>& buffer, int len){   
   for(int i = 0; i < len; i++){
      print_bits(buffer[i]); // pass in each byte
      std::cout << " ";
   }
   
   std::cout << std::endl;
}

// print hex
void print_hex(const std::vector<uint8_t>& buffer, int len){
   for(int i = 0; i < len; i++){
      std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]);
   }
   
   std::cout << std::endl;
}

void analyze(const std::vector<uint8_t>& buffer, int len){
   if(len < FRAME_SIZE){
      return;
   }
   // extract downlink format (DF) from the first byte
   uint8_t df = (buffer[0] >> 3) & 0x1F; // first 5 bits
   // extract type code (TC) from byte 5
   uint8_t tc = (buffer[4] >> 3) & 0x1F; // bits 33-37    

   if(df == 17){
      print_hex(buffer, FRAME_SIZE);
   }
}

void capture_single_frame(std::vector<uint8_t>& buffer, int len){
  // len is FRAME_SIZE
  ssize_t frame;
  if((frame = read(STDIN_FILENO, buffer.data(), len)) > 0){
     analyze(buffer, len);
  }
  
  if(frame < 0){
      perror("read");
  }
}

int main(){
   std::vector<uint8_t> buffer(BUFFER_SIZE);
   ssize_t len;

   // Setup signal handler for SIGINT (Ctrl+C)
   std::signal(SIGINT, handle_sigint);
  
   while (!stop && (len = read(STDIN_FILENO, buffer.data(), BUFFER_SIZE)) > 0){
     analyze(buffer, len);
   }

   if(len < 0){
      perror("read");
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
*/