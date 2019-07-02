/*
    This file contains the library contents that binds the bmp280 bosch driver to hwlib.
    It provides functinality to access the sensor information.

    https://github.com/BoschSensortec/BMP280_driver/

*/
#include "bmp280.h"
#include "hwlib.hpp"

#include <math.h>

#ifndef __BMP280LIB_HPP__
#define __BMP280LIB_HPP__

void delay_ms(uint32_t period_ms);
int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
//int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
//int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
void print_rslt(const char api_name[], int8_t rslt);



class bmp280    {
public:

   static bmp280& getInstance(int8_t i2c_address = 0x76)
   {
      static bmp280 instance(i2c_address);
                              
      return instance;
   }

   void set_i2c_bus(hwlib::i2c_bus_bit_banged_scl_sda& i2c_b)  {
      i2c_bmp280_bus = &i2c_b;
   }
   bmp280(bmp280 const&)   = delete;
   void operator=(bmp280 const&)  = delete;
   void test() {

      rslt = bmp280_init(&bmp);

      
      rslt = bmp280_get_config(&conf, &bmp);
      print_rslt(" bmp280_get_config status", rslt);
      
      /* configuring the temperature oversampling, filter coefficient and output data rate */
      /* Overwrite the desired settings */
      conf.filter = BMP280_FILTER_COEFF_16;

      /* Pressure oversampling set at 4x */
      conf.os_pres = BMP280_OS_4X;

      /* Temperature oversampling set at 4x */
      conf.os_temp = BMP280_OS_4X;

      /* Pressure oversampling set at 4x */
      conf.os_pres = BMP280_OS_16X;

      /* Setting the output data rate as 1HZ(1000ms) */
      conf.odr = BMP280_ODR_1000_MS;

      rslt = bmp280_set_config(&conf, &bmp);

      /* Always set the power mode after setting the configuration */
      rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);

      print_rslt(" bmp280_init status", rslt);
   }  

   double readTemperature() {

      //uint32_t pres32, pres64;
      //double pres;
      //int32_t temp32;
      double temp;

      /* Reading the raw data from sensor */
      rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);

      /* Getting the 32 bit compensated temperature */
      //rslt = bmp280_get_comp_temp_32bit(&temp32, ucomp_data.uncomp_temp, &bmp);

      /* Getting the compensated temperature as floating point value */
      rslt = bmp280_get_comp_temp_double(&temp, ucomp_data.uncomp_temp, &bmp);
      return temp;



   }

   double readPressure()   {

      double pres = 0;

      /* Reading the raw data from sensor */
      rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);

      /* Getting the compensated pressure as floating point value */
      rslt = bmp280_get_comp_pres_double(&pres, ucomp_data.uncomp_press, &bmp);

      return pres;
   }

   float get_altitude() {
      double temp = readTemperature();
      double pres = readPressure() * 1000; // hA to pascal

      double base_pres = 101315; // pascal
      float r_value = ((pow(base_pres / pres, 1 / 5.257) - 1) * (temp + 273.15));
      return r_value;  
   }
   int get_altitude_in_cm() {
      double temp = readTemperature();
      double pres = readPressure(); // hA to pascal
      hwlib::cout << "pressure is: " << hwlib::dec << (int) pres << "\n" << hwlib::flush;
      double base_pres = 102380/*101315*/; // pascal
      float r_value = ((pow(base_pres / pres, 1 / 5.257) - 1) * (temp + 273.15));
      return r_value * 100;  
   }
private:
   bmp280(int8_t i2c_address = 0x76) {

      /* Map the delay function pointer with the function responsible for implementing the delay */
      bmp.delay_ms = delay_ms;

      /* Assign device I2C address based on the status of SDO pin (GND for PRIMARY(0x76) & VDD for SECONDARY(0x77)) */
      //bmp.dev_id = BMP280_I2C_ADDR_PRIM;
      bmp.dev_id = 0x76;
      /* Select the interface mode as I2C */
      bmp.intf = BMP280_I2C_INTF;

      /* Map the I2C read & write function pointer with the functions responsible for I2C bus transfer */
      bmp.read = i2c_reg_read;
      bmp.write = i2c_reg_write;



   }
   friend int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
   friend int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
/*   bmp280(bmp280 const&);              
   void operator=(bmp280 const&); */
   hwlib::i2c_bus_bit_banged_scl_sda* i2c_bmp280_bus;


   int8_t rslt;
   struct bmp280_dev bmp;
   struct bmp280_config conf;
   struct bmp280_uncomp_data ucomp_data;
};
#endif // __BMP280LIB_HPP__