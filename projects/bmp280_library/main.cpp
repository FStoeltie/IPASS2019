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

#include "bmp280.hpp"
#include "bmp280_test.hpp"
int main( void ){
       
   namespace target = hwlib::target;
   hwlib::wait_ms( 1'000 );
   hwlib::cout << "Entering main...\n" << hwlib::flush;
   auto scl           = hwlib::target::pin_oc{ hwlib::target::pins::scl };
   auto sda           = hwlib::target::pin_oc{ hwlib::target::pins::sda };
   auto i2c_bus       = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
   auto font          = hwlib::font_default_8x8();
   auto oled          = hwlib::glcd_oled( i2c_bus );
   auto display       = hwlib::terminal_from( oled, font );

   GraphScene GS(oled, hwlib::xy(1, 1), 128, 64);
   GS.set_range_scale(28, 34);
   GS.draw();
   hwlib::wait_ms( 1'000 );

   bmp280 bmp_test(i2c_bus, 0x76);
   bmp_test.configure();
   //bmp280_test test;

   //test.test01(bmp_test);   
   //test.test02(bmp_test);  
   //test.test03(bmp_test);  // Does not work properly on BME280 (standby times are different)
   bmp_test.setIIR(IIR_RES::IIR_08);
   while(1) {
      hwlib::cout << "temp is:\t" << hwlib::dec << bmp_test.getTemperature() << "\n" << hwlib::flush;
      hwlib::cout << "Pressure is:\t" << hwlib::dec << bmp_test.getPressure() << "\n" << hwlib::flush;
      hwlib::cout << "Altitude: " << hwlib::dec << bmp_test.getAltitude(1016.2) << "m\n" << hwlib::flush;
      GS.push_back(bmp_test.getTemperature());
      GS.draw();
/*      display 
      << "\f" << "Temp: " << bmp_test.getTemperature() 
      << "\n" << "Pres: " << bmp_test.getPressure()
      << "\n" << "Alt:  " << bmp_test.getAltitude(1016.2)
      << hwlib::flush;*/   
      hwlib::wait_ms( 100 ); 
   }

}

