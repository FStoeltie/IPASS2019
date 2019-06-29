#include "hwlib.hpp"

int main( void ){   
  
   namespace target = hwlib::target;
   hwlib::wait_ms( 1'000 );
   while(true) {
      hwlib::cout << "test\n" << hwlib::flush;
      hwlib::wait_ms( 1'000 );
   }
}