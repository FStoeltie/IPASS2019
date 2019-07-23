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
/*   display 
   << "\f" << "Hello world!!" 
   << "\n" << "second line.."
   << "\t0305" << "pos 3 line 5"
   << hwlib::flush;     */

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

/*   bmp280 mytest(i2c_bus, 0x76);
   mytest.configure();
   hwlib::cout << "Error: " << mytest.getErrors() << "\n" << hwlib::flush;

   while(1) {
      mytest.test();
      hwlib::wait_ms( 200 );
   }*/
   bmp280 bmp_test(i2c_bus, 0x76);
   bmp_test.configure();
   bmp280_test test;

   test.test01(bmp_test);   
   test.test02(bmp_test);  
   test.test03(bmp_test);  // Does not work properly on BME280 (standby times are different)
   bmp_test.setIIR(IIR_RES::IIR_08);
   while(1) {
      hwlib::cout << "temp is:\t" << hwlib::dec << bmp_test.getTemperature() << "\n" << hwlib::flush;
      hwlib::cout << "Pressure is:\t" << hwlib::dec << bmp_test.getPressure() << "\n" << hwlib::flush;
      hwlib::cout << "Altitude: " << hwlib::dec << bmp_test.getAltitude(1016.2) << "m\n" << hwlib::flush;
      display 
      << "\f" << "Temp: " << bmp_test.getTemperature() 
      << "\n" << "Pres: " << bmp_test.getPressure()
      << "\n" << "Alt:  " << bmp_test.getAltitude(1016.2)
      << hwlib::flush;     
      hwlib::wait_ms(2000);
   }

}

