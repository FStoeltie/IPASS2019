// ==========================================================================
//
// BMP280 Temp Graph
//
// (c) Ferdi Stoeltie 1665045 ferdi.stoeltie@student.hu.nl Jul-2019
//
// ==========================================================================

#include "hwlib.hpp"
#include "Scene.hpp"
#include "array"

#include "bmp280.h"
#include "bmp280lib.hpp"
#include "bmp280lib_herkansing.hpp"
int main( void ){
       
   namespace target = hwlib::target;
   hwlib::wait_ms( 1'000 );
   hwlib::cout << "Entering main...\n" << hwlib::flush;
   auto scl           = hwlib::target::pin_oc{ hwlib::target::pins::scl };
   auto sda           = hwlib::target::pin_oc{ hwlib::target::pins::sda };
   auto i2c_bus       = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
   
   //auto oled          = hwlib::glcd_oled( i2c_bus );

/*   GraphScene GS(oled, hwlib::xy(1, 1), 30, 30);
   GS.push_back(1);
   GS.push_back(2);
   GS.push_back(3);
   GS.push_back(4);
   GS.push_back(5);
   GS.set_range_scale(20, 30);
   GS.draw();
   hwlib::wait_ms( 1'000 );*/
   //bmp280::getInstance().Initialize(i2c_bus);
/*   while (1)
   {
      int instance_temp = bmp280::getInstance().read_temperature();
      hwlib::cout << "instance_temp: " << hwlib::dec << instance_temp << "\n" << hwlib::flush;*/
      
   /*      int instance_pressure = bmp280::getInstance().read_pressure();
      hwlib::cout << "instance_pressure: " << hwlib::dec << instance_pressure << "\n" << hwlib::flush;*/

   /*      int alt = bmp280::getInstance().get_altitude();
      hwlib::cout << "altitude: " << hwlib::dec << alt << "\n" << hwlib::flush;*/
/*      GS.push_back(instance_temp);
      GS.draw();
      hwlib::wait_ms( 2500 ); 
   }*/

   bmp280lib_herkansing mytest(i2c_bus, 0x76);
   while(1) {
      mytest.test();
      hwlib::wait_ms( 1000 );
   }
   
}

