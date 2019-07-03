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

// These functions are implemented in bmp280lib and bound to the bmp280 driver by bosch.
void delay_ms(uint32_t period_ms);
int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
void print_rslt(const char api_name[], int8_t rslt);

constexpr uint8_t BMP280_I2C_ADDR   = 0x76;

/*!
 *  \class bmp280
 *  \brief  This library class provides an easy interface that binds the bmp280 bosch driver to hwlib, making it 'plug and play'.
 *  \version 1.0
 *  \author Ferdi Stoeltie
 */
class bmp280    {
public:
   /*
      \brief Function for the singleton pattern. Gets the only bmp280 instance.
   */
   static bmp280& getInstance()
   {
      static bmp280 instance;                        
      return instance;
   }
   bmp280(bmp280 const&)   = delete;
   void operator=(bmp280 const&)  = delete;

   /*
      \brief Initializes bmp280 with all the correct parameters. Call this method before calling any others!
      param[in] i2c_bus The hwlib i2c_bus object to communicate through.
      param[in] i2c_addr The i2c address of the BMP280 (0x76 or 0x77)
   */
   void Initialize(hwlib::i2c_bus_bit_banged_scl_sda& i2c_b, uint8_t i2c_addr = BMP280_I2C_ADDR);
   
   /*
      \brief Gets the underlying bmp280 device driver from the bosch driver library.
      \return struct bmp280_dev
   */
   struct bmp280_dev get_bmp280_dev_driver();

   
   /*
      \brief Sets the bmp280 device driver for modifying default settings (updates immediately to the bmp280 sensor).
      \param[in] bmp_d The bmp280_dev that replaces the current one. 
   */
   void set_bmp280_dev_driver(struct bmp280_dev bmp_d);

   /*
      \brief Gets the underlying bmp280 config from the bosch driver library.
      \return struct bmp280_config
   */
   struct bmp280_config get_bmp280_conf_driver();

   /*
      \brief Sets the bmp280 config for modifying default settings (updates immediately to the bmp280 sensor).
      \param[in] bmp_c The bmp280_config that replaces the current one. 
   */
   void set_bmp280_conf_driver(struct bmp280_config bmp_c);

   /*
      \brief Reads the temperature value from the bmp280 and returns it as a double (always new read).
      \return double Temperature value in degrees celcius.
   */
   double read_temperature();

   /*
      \brief Reads the pressure value from the bmp280 and returns it as a souble (always new read). The value is expressed in pascal (Pa).
   */
   double read_pressure();

   /*
      \brief Helper method that gets the altitude based on the temperature and pressure. The return value is expressed in meters.
      \param[in] The local pressure at sea level, expressed in hectopascal (hPa).
      \return float Altitude in meters.
   */
   float get_altitude(double base_pres = 1013.15);
private:
   bmp280();
   friend int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
   friend int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
   hwlib::i2c_bus_bit_banged_scl_sda* i2c_bmp280_bus;

   int8_t rslt;
   struct bmp280_dev bmp;
   struct bmp280_config conf;
   struct bmp280_uncomp_data ucomp_data;
};
#endif // __BMP280LIB_HPP__