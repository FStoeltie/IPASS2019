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
   /* Implement the I2C write routine according to the target machine. */
   //auto scl           = hwlib::target::pin_oc{ hwlib::target::pins::scl };
   //auto sda           = hwlib::target::pin_oc{ hwlib::target::pins::sda };
   //auto i2c_bus       = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
   //i2c_bus* my_bus = i2c_bus_global;
   uint8_t mydata[length + 1];
   mydata[0] = reg_addr;
   for(int i = 1; i <= length; i++) {
      mydata[i] = reg_data[i - 1];
   }
   hwlib::cout << "data to write: reg_addr: " << reg_addr << ", reg_data: " << reg_data << ", length: " << length << "\n" << hwlib::flush;
   //i2c_bus_global->i2c_bus::write(i2c_addr).write(reg_addr);
   bmp280::getInstance().i2c_bmp280_bus->i2c_bus::write(i2c_addr).write(mydata, length + 1);
   return 0;
   //return -1;
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

   /* Implement the I2C read routine according to the target machine. */
   //uint8_t my_array[length];
   //i2c_bus_global->i2c_bus::read(i2c_addr).read(reg_data, length);
   //i2c_bus_global->i2c_bus::write(i2c_addr).write(reg_addr);
   //i2c_bus_global->i2c_bus::read(i2c_addr).read(reg_data, length/ sizeof(uint8_t));
   
   //i2c_bus_global->i2c_bus::readbmp280(i2c_addr).read(reg_data, length);
   uint8_t a_value = 0;
   /*uint8_t r_value = */ //i2c_bus_global->i2c_bus::readbmp280(i2c_addr, reg_addr).read(reg_data, length / sizeof( uint8_t ) );
   hwlib::cout << "i2c_addr: " << hwlib::hex << i2c_addr << "\n" << hwlib::flush;
   bmp280::getInstance().i2c_bmp280_bus->i2c_bus::readbmp280(i2c_addr, reg_addr).read_from_register(reg_data, length);
   hwlib::cout << "data to read: reg_addr: " << hwlib::hex << reg_addr << ", a_value: " << a_value <<", length: " << length << "\n" << hwlib::flush;
/*   for(int i = 0; i < length; i++)  {
      hwlib::cout << (char)my_array[length];
   }*/
   return 0;
   //return -1;
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

    /* Implement the SPI write routine according to the target machine. */
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
     //printf("%s\t", api_name);
   /*         hwlib::cout 
   << "Hello world!\n" 
   << hwlib::flush;*/
         /*         hwlib::cout 
   << "Hello world!\n" 
   << hwlib::flush;*/
      hwlib::cout << "api_name\n" << hwlib::flush;   
      if (rslt == BMP280_E_NULL_PTR)
      {
         //printf("Error [%d] : Null pointer error\r\n", rslt);
         hwlib::cout << "Error " << rslt << " : Null pointer error\n" << hwlib::flush;
      }
      else if (rslt == BMP280_E_COMM_FAIL)
      {
         //printf("Error [%d] : Bus communication failed\r\n", rslt);
         hwlib::cout << "Error " << rslt << " :Bus communication failed\n" << hwlib::flush;
      }
      else if (rslt == BMP280_E_IMPLAUS_TEMP)
      {
         //printf("Error [%d] : Invalid Temperature\r\n", rslt);
         hwlib::cout << "Error " << rslt << " : Invalid Temperature\n" << hwlib::flush;
      }
      else if (rslt == BMP280_E_DEV_NOT_FOUND)
      {
         //printf("Error [%d] : Device not found\r\n", rslt);
         hwlib::cout << "Error " << rslt << " : Device not found\n" << hwlib::flush;
      }
      else
      {
         /* For more error codes refer "*_defs.h" */
         //printf("Error [%d] : Unknown error code\r\n", rslt);
         hwlib::cout << "Error " << rslt << " : Unknown error code\n" << hwlib::flush;
      }
   }
   else  {
      hwlib::cout << "Everything OK\n" << hwlib::flush;
   }
}