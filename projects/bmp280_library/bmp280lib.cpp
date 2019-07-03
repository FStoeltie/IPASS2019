#include "bmp280lib.hpp"

/*!
 *  @brief Function that creates a mandatory delay required in some of the APIs such as "bmg250_soft_reset",
 *      "bmg250_set_foc", "bmg250_perform_self_test"  and so on.
 *
 *  @param[in] period_ms  : the required wait time in milliseconds.
 *  @return void.
 *
 */
void delay_ms(uint32_t period_ms)
{
    /* Implement the delay routine according to the target machine */
   //hwlib::cout << "Waiting: " << period_ms << "ms\n" << hwlib::flush;
   hwlib::wait_ms( period_ms ); 
}

/*!
 *  @brief Function for writing the sensor's registers through I2C bus.
 *
 *  @param[in] i2c_addr : sensor I2C address.
 *  @param[in] reg_addr : Register address.
 *  @param[in] reg_data : Pointer to the data buffer whose value is to be written.
 *  @param[in] length   : No of bytes to write.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
   uint8_t mydata[length + 1];
   mydata[0] = reg_addr;
   for(int i = 1; i <= length; i++) {
      mydata[i] = reg_data[i - 1];
   }
   bmp280::getInstance().i2c_bmp280_bus->i2c_bus::write(i2c_addr).write(mydata, length + 1);
   return 0;
}

/*!
 *  @brief Function for reading the sensor's registers through I2C bus.
 *
 *  @param[in] i2c_addr : Sensor I2C address.
 *  @param[in] reg_addr : Register address.
 *  @param[out] reg_data    : Pointer to the data buffer to store the read data.
 *  @param[in] length   : No of bytes to read.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
   bmp280::getInstance().i2c_bmp280_bus->i2c_bus::readbmp280(i2c_addr, reg_addr).read_from_register(reg_data, length);
   return 0;
}

/*!
 *  @brief Function for writing the sensor's registers through SPI bus.
 *
 *  @param[in] cs           : Chip select to enable the sensor.
 *  @param[in] reg_addr     : Register address.
 *  @param[in] reg_data : Pointer to the data buffer whose data has to be written.
 *  @param[in] length       : No of bytes to write.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{

    /* Not required */
    return -1;
}

/*!
 *  @brief Function for reading the sensor's registers through SPI bus.
 *
 *  @param[in] cs       : Chip select to enable the sensor.
 *  @param[in] reg_addr : Register address.
 *  @param[out] reg_data    : Pointer to the data buffer to store the read data.
 *  @param[in] length   : No of bytes to read.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{

    /* Implement the SPI read routine according to the target machine. */
    return -1;
}

/*!
 *  @brief Prints the execution status of the APIs.
 *
 *  @param[in] api_name : name of the API whose execution status has to be printed.
 *  @param[in] rslt     : error code returned by the API whose execution status has to be printed.
 *
 *  @return void.
 */
void print_rslt(const char api_name[], int8_t rslt)
{
   if (rslt != BMP280_OK)
   {
      hwlib::cout << "api_name\n" << hwlib::flush;   
      if (rslt == BMP280_E_NULL_PTR)
      {
         hwlib::cout << "Error " << rslt << " : Null pointer error\n" << hwlib::flush;
      }
      else if (rslt == BMP280_E_COMM_FAIL)
      {
         hwlib::cout << "Error " << rslt << " :Bus communication failed\n" << hwlib::flush;
      }
      else if (rslt == BMP280_E_IMPLAUS_TEMP)
      {
         hwlib::cout << "Error " << rslt << " : Invalid Temperature\n" << hwlib::flush;
      }
      else if (rslt == BMP280_E_DEV_NOT_FOUND)
      {
         hwlib::cout << "Error " << rslt << " : Device not found\n" << hwlib::flush;
      }
      else
      {
         hwlib::cout << "Error " << rslt << " : Unknown error code\n" << hwlib::flush;
      }
   }
}

bmp280::bmp280() {


}

void bmp280::Initialize(hwlib::i2c_bus_bit_banged_scl_sda& i2c_b, uint8_t i2c_addr) {
   i2c_bmp280_bus = &i2c_b;

   /* Map the delay function pointer with the function responsible for implementing the delay */
   bmp.delay_ms = delay_ms;
   bmp.dev_id = i2c_addr;
   bmp.intf = BMP280_I2C_INTF;
   bmp.read = i2c_reg_read;
   bmp.write = i2c_reg_write;

   rslt = bmp280_init(&bmp);
   rslt = bmp280_get_config(&conf, &bmp);

   conf.filter = BMP280_FILTER_COEFF_16;
   conf.os_temp = BMP280_OS_16X;
   conf.os_pres = BMP280_OS_16X;
   conf.odr = BMP280_ODR_1000_MS;

   rslt = bmp280_set_config(&conf, &bmp);
   rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);

   print_rslt(" bmp280_init status", rslt);

}

struct bmp280_dev bmp280::get_bmp280_dev_driver()  {
   rslt = bmp280_get_config(&conf, &bmp);
   return bmp;
}

void bmp280::set_bmp280_dev_driver(struct bmp280_dev bmp_d)  {
   bmp = bmp_d;
   rslt = bmp280_set_config(&conf, &bmp);
   rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);

}

struct bmp280_config bmp280::get_bmp280_conf_driver()  {
   rslt = bmp280_get_config(&conf, &bmp);
   return conf;
}

void bmp280::set_bmp280_conf_driver(struct bmp280_config bmp_c)  {
   conf = bmp_c;
   rslt = bmp280_set_config(&conf, &bmp);
   rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);

}

double bmp280::read_temperature() {
   double temp;
   rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);
   rslt = bmp280_get_comp_temp_double(&temp, ucomp_data.uncomp_temp, &bmp);
   return temp;
}

double bmp280::read_pressure()   {
   double pres = 0;
   rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);
   rslt = bmp280_get_comp_pres_double(&pres, ucomp_data.uncomp_press, &bmp);
   return pres;
}

// base_pres in pascal
// in meters
float bmp280::get_altitude(double base_pres) {
   base_pres *= 1000;
   double temp = read_temperature();
   double pres = read_pressure(); // sensor pascal
   float height_in_meters = (double)(((pow(base_pres / pres, 1 / 5.257) - 1) * (temp + 273.15))) / 0.0065;
   return height_in_meters;  
}