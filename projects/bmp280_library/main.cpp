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

#include "bmp280.h"
#include "bmp280lib.hpp"

// possible_fixes start

// possible_fixes end


// bmp280_defs_hpp begin


// bmp280_defs_hpp end

// bmp280_hpp start

/*!
 * @brief This API reads the data from the given register address of the
 * sensor.
 *
 * @param[in] reg_addr : Register address from where the data to be read
 * @param[out] reg_data : Pointer to data buffer to store the read data.
 * @param[in] len : No of bytes of data to be read.
 * @param[in] dev : Structure instance of bmp280_dev.
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, const struct bmp280_dev *dev);

/*!
 * @brief This API writes the given data to the register addresses
 * of the sensor.
 *
 * @param[in] reg_addr : Register address from where the data to be written.
 * @param[in] reg_data : Pointer to data buffer which is to be written
 * in the sensor.
 * @param[in] len : No of bytes of data to write..
 * @param[in] dev : Structure instance of bmp280_dev.
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len, const struct bmp280_dev *dev);

/*!
 * @brief This API triggers the soft reset of the sensor.
 *
 * @param[in] dev : Structure instance of bmp280_dev.
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise..
 */
//int8_t bmp280_soft_reset(const struct bmp280_dev *dev);

/*!
 *  @brief This API is the entry point.
 *  It reads the chip-id and calibration data from the sensor.
 *
 *  @param[in,out] dev : Structure instance of bmp280_dev
 *
 *  @return Result of API execution
 *  @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
//int8_t bmp280_init(struct bmp280_dev *dev);

/*!
 * @brief This API reads the data from the ctrl_meas register and config
 * register. It gives the currently set temperature and pressure over-sampling
 * configuration, power mode configuration, sleep duration and
 * IIR filter coefficient.
 *
 * @param[out] conf : Current configuration of the bmp280
 * conf.osrs_t, conf.osrs_p = BMP280_OS_NONE, BMP280_OS_1X,
 *     BMP280_OS_2X, BMP280_OS_4X, BMP280_OS_8X, BMP280_OS_16X
 * conf.odr = BMP280_ODR_0_5_MS, BMP280_ODR_62_5_MS, BMP280_ODR_125_MS,
 *     BMP280_ODR_250_MS, BMP280_ODR_500_MS, BMP280_ODR_1000_MS,
 *     BMP280_ODR_2000_MS, BMP280_ODR_4000_MS
 * conf.filter = BMP280_FILTER_OFF, BMP280_FILTER_COEFF_2,
 *     BMP280_FILTER_COEFF_4, BMP280_FILTER_COEFF_8, BMP280_FILTER_COEFF_16
 * conf.spi3w_en = BMP280_SPI3_WIRE_ENABLE, BMP280_SPI3_WIRE_DISABLE
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_get_config(struct bmp280_config *conf, struct bmp280_dev *dev);

/*!
 * @brief This API writes the data to the ctrl_meas register and config register.
 * It sets the temperature and pressure over-sampling configuration,
 * power mode configuration, sleep duration and IIR filter coefficient.
 *
 * @param[in] conf : Desired configuration to the bmp280
 * conf.osrs_t, conf.osrs_p = BMP280_OS_NONE, BMP280_OS_1X,
 *     BMP280_OS_2X, BMP280_OS_4X, BMP280_OS_8X, BMP280_OS_16X
 * conf.odr = BMP280_ODR_0_5_MS, BMP280_ODR_62_5_MS, BMP280_ODR_125_MS,
 *     BMP280_ODR_250_MS, BMP280_ODR_500_MS, BMP280_ODR_1000_MS,
 *     BMP280_ODR_2000_MS, BMP280_ODR_4000_MS
 * conf.filter = BMP280_FILTER_OFF, BMP280_FILTER_COEFF_2,
 *     BMP280_FILTER_COEFF_4, BMP280_FILTER_COEFF_8, BMP280_FILTER_COEFF_16
 * conf.spi3w_en = BMP280_SPI3_WIRE_ENABLE, BMP280_SPI3_WIRE_DISABLE
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_set_config(const struct bmp280_config *conf, struct bmp280_dev *dev);

/*!
 * @brief This API reads the status register
 *
 * @param[out] status : Status of the sensor
 * status.measuring = BMP280_MEAS_DONE, BMP280_MEAS_ONGOING
 * status.im_update = BMP280_IM_UPDATE_DONE, BMP280_IM_UPDATE_ONGOING
 * @param[in] dev : structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_get_status(struct bmp280_status *status, const struct bmp280_dev *dev);

/*!
 * @brief This API reads the power mode.
 *
 * @param[out] mode : BMP280_SLEEP_MODE, BMP280_NORMAL_MODE,
 *     BMP280_FORCED_MODE
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_get_power_mode(uint8_t *mode, const struct bmp280_dev *dev);

/*!
 * @brief This API writes the power mode.
 *
 * @param[out] mode : BMP280_SLEEP_MODE, BMP280_NORMAL_MODE,
 *     BMP280_FORCED_MODE
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_set_power_mode(uint8_t mode, struct bmp280_dev *dev);

/*!
 * @brief This API reads the temperature and pressure data registers.
 * It gives the raw temperature and pressure data.
 *
 * @param[in] uncomp_data : Structure instance of bmp280_uncomp_data
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_get_uncomp_data(struct bmp280_uncomp_data *uncomp_data, const struct bmp280_dev *dev);

/*!
 * @brief This API is used to get the compensated temperature from
 * uncompensated temperature. This API uses 32 bit integers.
 * Temperature in degC, resolution is 0.01 DegC. output value of
 * "5123" equals 51.23 degree Celsius
 *
 * @param[out] comp_temp : 32 bit compensated temperature
 * @param[in] uncomp_temp : Raw temperature values from the sensor
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_get_comp_temp_32bit(int32_t *comp_temp, int32_t uncomp_temp, struct bmp280_dev *dev);

/*!
 * @brief This API is used to get the compensated pressure from
 * uncompensated pressure. This API uses 32 bit integers.
 * Pressure in Pa as unsigned 32 bit integer
 * output value of "96386" equals 96386 Pa = 963.86 hPa
 *
 * @param[out] comp_pres : 32 bit compensated pressure
 * @param[in] uncomp_pres : Raw pressure values from the sensor
 * @param[in] dev : structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_get_comp_pres_32bit(uint32_t *comp_pres, int32_t uncomp_pres, const struct bmp280_dev *dev);

#ifndef BMP280_DISABLE_64BIT_COMPENSATION

/*!
 * @brief This API is used to get the compensated pressure from
 * uncompensated pressure. This API uses 64 bit integers.
 * Pressure in Pa as unsigned 32 bit integer in Q24.8 format
 * (24 integer bits and 8 fractional bits). Output value of "24674867"
 * represents 24674867/256 = 96386.2 Pa = 963.862 hPa
 *
 * @param[out] pressure : compensated pressure
 * @param[in] uncomp_pres : Raw pressure values from the sensor
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_get_comp_pres_64bit(uint32_t *pressure, int32_t uncomp_pres, const struct bmp280_dev *dev);

#endif /* BMP280_DISABLE_64BIT_COMPENSATION */

#ifndef BMP280_DISABLE_DOUBLE_COMPENSATION

/*!
 * @brief This API is used to get the compensated temperature from
 * uncompensated temperature. This API uses double floating precision.
 * Temperature in degree Celsius , double precision. output value
 * of "51.23" equals 51.23 degC.
 *
 * @param[out] temperature : compensated temperature
 * @param[in] uncomp_temp : Raw temperature values from the sensor
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_get_comp_temp_double(double *temperature, int32_t uncomp_temp, struct bmp280_dev *dev);

/*!
 * @brief This API is used to get the compensated pressure from
 * uncompensated pressure. This API uses double floating precision.
 * Pressure in Pa as double. Output value of "96386.2"
 * equals 96386.2 Pa = 963.862 hPa
 *
 * @param[out] pressure : compensated pressure
 * @param[in] uncomp_pres : Raw pressure values from the sensor
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
//int8_t bmp280_get_comp_pres_double(double *pressure, int32_t uncomp_pres, const struct bmp280_dev *dev);

#endif /* BMP280_DISABLE_DOUBLE_COMPENSATION */

/*!
 * @brief This API computes the measurement time in milliseconds for the
 * active configuration
 *
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Measurement time for the active configuration in milliseconds
 */
//uint8_t bmp280_compute_meas_time(const struct bmp280_dev *dev);

/*!
 * @brief This API performs self-test to check whether compensated temperature and pressure are within the range
 *
 * @param[in] dev : structure instance of bmp280_dev
 *
 * @return status of test results such as init, reset, config, adc read, compensation
 * and non-compensation output range of both temperature and pressure
 */
//int8_t bmp280_selftest(struct bmp280_dev *dev);

// bmp280_hpp end

// bmp280_cpp start


// bmp280_cpp end





void delay_ms(uint32_t period_ms);
int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
void print_rslt(const char api_name[], int8_t rslt);

void my_test_code(hwlib::glcd_oled& window)  {
   //std::array<hwlib::xy, 3> my_array = {hwlib::xy(1, 1), hwlib::xy(1,10)};
   
   /*GraphicsObjectSquare GOS(window);
   GOS.setSquareData(hwlib::xy(15, 15), 10, 10, 75);
   GOS.draw();*/

   GraphScene GS(window, hwlib::xy(1, 1), 30, 30);
   GS.draw();
}




hwlib::i2c_bus_bit_banged_scl_sda* i2c_bus_global;
int main( void ){
       
   namespace target = hwlib::target;
   hwlib::wait_ms( 1'000 );
   hwlib::cout << "Entering main...\n" << hwlib::flush;
   auto scl           = hwlib::target::pin_oc{ hwlib::target::pins::scl };
   auto sda           = hwlib::target::pin_oc{ hwlib::target::pins::sda };
   auto i2c_bus       = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
   i2c_bus_global     = &i2c_bus;
   


   //auto oled_channel  = i2c_bus.channel( 0x3c );
/*   auto oled          = hwlib::glcd_oled( i2c_bus );
   
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
      << hwlib::flush;*/  
   hwlib::wait_ms( 1'000 );
   hwlib::cout << "Setting bmp280 settings...\n" << hwlib::flush;
   int8_t rslt;
   struct bmp280_dev bmp;
   struct bmp280_config conf;
   struct bmp280_uncomp_data ucomp_data;
   //uint32_t pres32, pres64;
   //double pres;
   int32_t temp32;
   double temp;

   /* Map the delay function pointer with the function responsible for implementing the delay */
   //bmp.delay_ms = delay_ms;

   /* Assign device I2C address based on the status of SDO pin (GND for PRIMARY(0x76) & VDD for SECONDARY(0x77)) */
   //bmp.dev_id = BMP280_I2C_ADDR_PRIM;
  // bmp.dev_id = 0x76;
   /* Select the interface mode as I2C */
  // bmp.intf = BMP280_I2C_INTF;

   /* Map the I2C read & write function pointer with the functions responsible for I2C bus transfer */
   //bmp.read = i2c_reg_read;
  // bmp.write = i2c_reg_write;

   hwlib::cout << "testing singleton\n" << hwlib::flush;

bmp280::getInstance().set_i2c_bus(i2c_bus);
   bmp280::getInstance().test();

   hwlib::cout << "end of testing singleton\n" << hwlib::flush;
   /* To enable SPI interface: comment the above 4 lines and uncomment the below 4 lines */

   /*
   * bmp.dev_id = 0;
   * bmp.read = spi_reg_read;
   * bmp.write = spi_reg_write;
   * bmp.intf = BMP280_SPI_INTF;
   */
   rslt = bmp280_init(&bmp);
   hwlib::cout << "Done...\n" << hwlib::flush;
   print_rslt(" bmp280_init status", rslt);

   rslt = bmp280_get_config(&conf, &bmp);
   print_rslt(" bmp280_get_config status", rslt);


   /* configuring the temperature oversampling, filter coefficient and output data rate */
   /* Overwrite the desired settings */
   conf.filter = BMP280_FILTER_COEFF_2;

   /* Pressure oversampling set at 4x */
   conf.os_pres = BMP280_OS_4X;

   /* Temperature oversampling set at 4x */
   conf.os_temp = BMP280_OS_4X;

   /* Setting the output data rate as 1HZ(1000ms) */
   conf.odr = BMP280_ODR_1000_MS;
   rslt = bmp280_set_config(&conf, &bmp);
   print_rslt(" bmp280_set_config status", rslt);

   /* Always set the power mode after setting the configuration */
   rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);
   print_rslt(" bmp280_set_power_mode status", rslt);
 while (1)
 {
      /* Reading the raw data from sensor */
      rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);

      /* Getting the 32 bit compensated temperature */
      rslt = bmp280_get_comp_temp_32bit(&temp32, ucomp_data.uncomp_temp, &bmp);

      /* Getting the compensated temperature as floating point value */
      rslt = bmp280_get_comp_temp_double(&temp, ucomp_data.uncomp_temp, &bmp);
      //printf("UT: %ld, T32: %ld, T: %f \r\n", ucomp_data.uncomp_temp, temp32, temp);
      hwlib::cout << "Temp: " << hwlib::dec << (int)temp <<  "\n" << hwlib::flush;
      /* Sleep time between measurements = BMP280_ODR_1000_MS */
      bmp.delay_ms(1000);
   }

}

