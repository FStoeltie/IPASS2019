// ==========================================================================
//
// OLED demo on DB103 (LPC1114)
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt) 
//
// ==========================================================================

#include "hwlib.hpp"
#include "Scene.hpp"
#include "array"
void my_test_code(hwlib::glcd_oled& window)  {
   //std::array<hwlib::xy, 3> my_array = {hwlib::xy(1, 1), hwlib::xy(1,10)};
   
   /*GraphicsObjectSquare GOS(window);
   GOS.setSquareData(hwlib::xy(15, 15), 10, 10, 75);
   GOS.draw();*/

   GraphScene GS(window, hwlib::xy(1, 1), 30, 30);
   GS.draw();
}
int main( void ){
       
   namespace target = hwlib::target;
   
   auto scl           = hwlib::target::pin_oc{ hwlib::target::pins::scl };
   auto sda           = hwlib::target::pin_oc{ hwlib::target::pins::sda };
   auto i2c_bus       = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
   
   //auto oled_channel  = i2c_bus.channel( 0x3c );
   auto oled          = hwlib::glcd_oled( i2c_bus );
   
   auto w1            = hwlib::window_part( 
      oled, hwlib::xy( 0, 0 ), hwlib::xy( 128, 16 ));  
   auto font1         = hwlib::font_default_8x8();
   auto console1      = hwlib::terminal_from( w1, font1 );
   
   auto w2            = hwlib::window_part( 
      oled, hwlib::xy( 0, 16 ), hwlib::xy( 128, 48 ));   
   auto font2         = hwlib::font_default_16x16();
   auto console2      = hwlib::terminal_from( w2, font2 );
   
   oled.flush();
   oled.clear();
   auto line = hwlib::line( 
      hwlib::xy{  5,  5 },
      hwlib::xy{ 10, 5 }, 
      oled.foreground 
   );
   line.draw(oled);
   oled.flush(); 
   hwlib::wait_ms( 1000 );      
     
   my_test_code(oled);


   console1
      << "\f" << "Test world!!" 
      << "\n" << "Hiya world"
      << hwlib::flush;     
      
   console2
      << "\f" << "Ey Ey Ey"
      << "\n" << "said"
      << "\n" << "Let me tell you a story..."
      << hwlib::flush;     
}