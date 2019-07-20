#include "bmp280lib_herkansing.hpp"

// base_pres in pascal
// in meters
float bmp280lib_herkansing::get_altitude(double base_pres) {
   base_pres *= 1000;
   double temp = 5;
   double pres = 5; // sensor pascal
   float height_in_meters = (double)(((pow(base_pres / pres, 1 / 5.257) - 1) * (temp + 273.15))) / 0.0065;
   return height_in_meters;  
}