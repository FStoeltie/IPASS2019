/* File will include own communication */
/**
    This file contains all the requirements to use the bmp280 class.


    * \file bmp280.hpp
    * \author Ferdi Stoeltie
    * \date 2019-22-07
    * \version v1.0.0
*/
#ifndef __bmp280_HPP__
#define __bmp280_HPP__
#include "hwlib.hpp"

#include <math.h>

// Minimum temperature for BMP280
static constexpr int8_t BMP280_MIN_TEMP = -40;
static constexpr int8_t BMP280_MAX_TEMP = 85;
// Maximum temperature for BMP280

// Minimum pressure for BMP280
static constexpr uint16_t BMP280_MIN_PRESS = 300;
static constexpr uint16_t BMP280_MAX_PRESS = 1100;
// Maximum pressure for BMP280

// Known device id's for BMP280 and BME280
static constexpr uint8_t BMP280_DEVICE_ID_01 = 0x57;
static constexpr uint8_t BMP280_DEVICE_ID_02 = 0x58;
static constexpr uint8_t BMP280_DEVICE_ID_03 = 0x59;
static constexpr uint8_t BME280_DEVICE_ID_01 = 0x60;

// BMP280/BME280 registers that are used
static constexpr uint8_t REG_DEVICE_ID = 0xD0;
static constexpr uint8_t REG_RESET_BMP = 0xE0;
static constexpr uint8_t REG_CTRL_MEASUREMENT = 0xF4;
static constexpr uint8_t REG_CONFIG = 0xF5;
static constexpr uint8_t REG_PRES_DATA = 0xF7;
static constexpr uint8_t REG_TEMP_DATA = 0xFA;
static constexpr uint8_t RESET_VALUE = 0xB6;

// Registers for calibration data (set in factory, read-only)
static constexpr uint8_t REG_DIG_T1 = 0x88;
static constexpr uint8_t REG_DIG_T2 = 0x8A;
static constexpr uint8_t REG_DIG_T3 = 0x8C;
static constexpr uint8_t REG_DIG_P1 = 0x8E;
static constexpr uint8_t REG_DIG_P2 = 0x90;
static constexpr uint8_t REG_DIG_P3 = 0x92;
static constexpr uint8_t REG_DIG_P4 = 0x94;
static constexpr uint8_t REG_DIG_P5 = 0x96;
static constexpr uint8_t REG_DIG_P6 = 0x98;
static constexpr uint8_t REG_DIG_P7 = 0x9A;
static constexpr uint8_t REG_DIG_P8 = 0x9C;
static constexpr uint8_t REG_DIG_P9 = 0x9E;

static constexpr uint8_t REG_DIG_RESERVED = 0xA0;

static constexpr uint8_t STANDBY_TIME_500_US = 0x00;
static constexpr uint8_t STANDBY_TIME_62500_US = 0x01;
static constexpr uint8_t STANDBY_TIME_125_MS = 0x02;
static constexpr uint8_t STANDBY_TIME_250_MS = 0x03;
static constexpr uint8_t STANDBY_TIME_500_MS = 0x04;
static constexpr uint8_t STANDBY_TIME_1_S = 0x05;
static constexpr uint8_t STANDBY_TIME_2_S = 0x06;
static constexpr uint8_t STANDBY_TIME_4_S = 0x07;

/**
    \enum STANDBY_TIME
    \brief Enum for standby time values between measurements taken by bmp280/bme280 
    (only when bmp280 is in normal mode).
    \author Ferdi Stoeltie
*/
enum class STANDBY_TIME : uint8_t{
    US_500 = 0x00,  /**< 500 microseconds */
    US_62500 = 0x20,    /**< 62.5 milliseconds */
    MS_125 = 0x40,  /**< 125 milliseconds */
    MS_250 = 0x60,  /**< 250 milliseconds */
    MS_500 = 0x80,  /**< 500 milliseconds */
    S_1 = 0xA0, /**< 1 second */
    S_2 = 0xC0, /**< 2 seconds */
    S_4 = 0xE0, /**< 4 seconds */

    BME280_US_500   = 0b00000000, /**< BME280 500 microseconds */
    BME280_MS_10    = 0b11000000, /**< BME280 10 milliseconds */
    BME280_MS_20    = 0b11100000, /**< BME280 20 milliseconds */
    BME280_US_62500 = 0b00100000, /**< BME280 62.5 milliseconds */
    BME280_MS_125   = 0b01000000, /**< BME280 125 milliseconds */
    BME280_MS_250   = 0b01100000, /**< BME280 250 milliseconds */
    BME280_MS_500   = 0b10000000, /**< BME280 500 milliseconds */
    BME280_S_1      = 0b10100000, /**< BME280 1 second */

};

/**
    \enum IIR_RES
    \brief Enum for IIR resolution. used for changing rate of change of pressure measurement based on past measurements. 
    \author Ferdi Stoeltie
*/
enum class IIR_RES : uint8_t{
    IIR_OFF = 0x00, /**< IIR Res off */
    IIR_02 = 0b00000100,  /**< IIR Res x2 */
    IIR_04 = 0b00001000,  /**< IIR Res x4 */
    IIR_08 = 0b00001100,  /**< IIR Res x8 */
    IIR_16 = 0b00010000,  /**< IIR Res x16 */
};

/**
    \enum PRES_OVERSAMPLING
    \brief Enum for Pressure sampling setting.
    \author Ferdi Stoeltie
*/
enum class PRES_OVERSAMPLING : uint8_t{
    PRES_OS_OFF = 0x00, /**< Temperature measurement off */
    PRES_OS_01 = 0x04,  /**< Pressure measurement 1   (2.62 Pa) */
    PRES_OS_02 = 0x08,  /**< Pressure measurement 2   (1.31 Pa) */
    PRES_OS_04 = 0x0B,  /**< Pressure measurement 4   (0.66 Pa) */
    PRES_OS_08 = 0x10,  /**< Pressure measurement 8   (0.33 Pa) */
    PRES_OS_16 = 0x14,  /**< Pressure measurement 16  (0.16 Pa) */
};

/**
    \enum TEMP_OVERSAMPLING
    \brief Enum for temperature sampling setting.
    \author Ferdi Stoeltie
*/
enum class TEMP_OVERSAMPLING : uint8_t{
    TEMP_OS_OFF = 0x00, /**< Temperature measurement off */
    TEMP_OS_01 = 0x20,  /**< Temperature measurement 1  (0.00050 Degrees Celcius) */
    TEMP_OS_02 = 0x40,  /**< Temperature measurement 2  (0.00025 Degrees Celcius) */
    TEMP_OS_04 = 0x50,  /**< Temperature measurement 4  (0.00012 Degrees Celcius) */
    TEMP_OS_08 = 0x80,  /**< Temperature measurement 8  (0.00006 Degrees Celcius) */
    TEMP_OS_16 = 0xA0,  /**< Temperature measurement 16 (0.00003 Degrees Celcius) */
};

/**
    \enum OVERSAMPLING
    \brief Enum for Oversampling. Combines both temperature and pressure oversampling into a single value (mask).
    \author Ferdi Stoeltie
*/
enum class OVERSAMPLING : uint8_t   {
    OVER_01 = static_cast<uint8_t>(PRES_OVERSAMPLING::PRES_OS_01) | static_cast<uint8_t>(TEMP_OVERSAMPLING::TEMP_OS_01),
    OVER_02 = static_cast<uint8_t>(PRES_OVERSAMPLING::PRES_OS_02) | static_cast<uint8_t>(TEMP_OVERSAMPLING::TEMP_OS_02),
    OVER_04 = static_cast<uint8_t>(PRES_OVERSAMPLING::PRES_OS_04) | static_cast<uint8_t>(TEMP_OVERSAMPLING::TEMP_OS_04),
    OVER_08 = static_cast<uint8_t>(PRES_OVERSAMPLING::PRES_OS_08) | static_cast<uint8_t>(TEMP_OVERSAMPLING::TEMP_OS_08),
    OVER_16 = static_cast<uint8_t>(PRES_OVERSAMPLING::PRES_OS_16) | static_cast<uint8_t>(TEMP_OVERSAMPLING::TEMP_OS_16),
};

/**
    \enum MODE
    \brief Enum to select mode for BMP280 to operate in.
    \author Ferdi Stoeltie
*/
enum class MODE : uint8_t {
    SLEEP = 0x00, /**< BMP280 in sleep mode, won't take measurements */
    FORCED = 0x01,  /**< BMP280 will only take a measurement when temperature or pressure is requested */
    NORMAL = 0b00000011, /**< BMP280 will run in continuous mode with a standby between every measurement */
};

// To do in the future
enum class BMP280_ERROR : uint8_t{
    NO_ERROR = 0x00,
    UNKNOWN_DEVICE_ID = 0x01,
    UNEXPECTED_REG_DATA = (0x01) << 1,
    TEMP_OUT_OF_RANGE = (0x01) << 2,
    PRES_OUT_OF_RANGE = (0x01) << 3,
};
// Helper function to print floats
inline hwlib::ostream & operator<< ( hwlib::ostream & stream, const float & f ) {
    int i_value = (int)f; 
  
    float f_value = f - (float)i_value; 
    f_value *= pow(10, 2);
    stream << i_value << '.' << (unsigned int)f_value;

  return stream;
}

/**
    \brief  This class provides functionality to easily interface with a bmp280.
            All modes (SLEEP, FORCED, STANDBY) work.
    \author Ferdi Stoeltie
    \date 2019-23-07
    \version 1.0.0
*/
class bmp280 {
public:
    /**
        \brief Constructor of BMP280. 
        Requires a i2c bus to use for communication with the bmp280 and the i2c address,
        (usually 0x76 | 0x77) of the BMP280.
        Default values that are set for the BMP280 are normal mode and oversampling x2.
        This constructor does not perform any read or write operations with the BMP280.
        \param[in] hwlib::i2c_bus& hbus The hwlib i2c bus to use.
        \param[in] uint8_t Address The i2c address for the BMP280.
    */
    bmp280(hwlib::i2c_bus& hbus, uint8_t address);

    /**
        \brief Configures the bmp280 by retrieving required data from the bmp280 and setting register values.  
        Performs read and write operations to the BMP280.
        \warning Retrieving temperature and pressure before calling this method will result in undefined behaviour (wrong measurements).
    */
    void configure();

    /**
        \brief Get the temperature in degrees celcius .
        Performs a read operation to the BMP280.
        \return float Degrees celcius of measured temperature.
    */
    float getTemperature();
    
    /**
        \brief Get the pressure in pA.
        Performs a read operation to the BMP280.
        \return uint32_t Pressure in unit of pA.
    */
    uint32_t getPressure();

    /**
        \brief Helper method that gets the altitude based on the temperature and pressure. The return value is expressed in meters.
        Performs write operations to the BMP280.
        \param[in] sea_level_pres The local pressure at sea level, expressed in hectopascal (hPa).
        \return float Altitude in meters.
    */
    float getAltitude(double sea_level_pres = 1013.15);

    /**
        \brief Resets the BMP280 to factory default.
        Performs a write operation to the BMP280.
    */
    void reset();

    /**
        \brief Sets the oversampling for obtaining measurements, Default is 2.
        Performs a read and write operation to the BMP280.
        \param[in] OVERSAMPLING The oversampling to use.
        \return bool Returns true when value has been set succesfully and false if failed.
    */
    bool setOversampling(OVERSAMPLING os);

    /**
        \brief Mode that the BMP280 should operate in. Default is normal mode.
        Performs a read and write operation to the BMP280.
        \param[in] MODE Mode that the BMP280 should operate in.
        \return bool Returns true when value has been set succesfully and false if failed.
    */
    bool setMode(MODE m);

    /**
        \brief Sets the filtering coefficient for pressure.
        Performs a read and write operation to the BMP280.
        \param[in] IIR_RES resolution of the filter.
        \return bool Returns true when value has been set succesfully and false if failed.
    */
    bool setIIR(IIR_RES res);

    /**
        \brief Set the standby time beween measurements when in normal mode.
        Performs a read and write operation to the BMP280.
        \param[in] STANDBY_TIME standby time enum for standby time between measurements.
        \return bool Returns true when value has been set succesfully and false if failed.
    */
    bool setStandbyTime(STANDBY_TIME standby_time);

    /**
        \brief Gets the device id that has been found for the BMP.
        \return  uint8_t The device id (not i2c address!) for the bmp280 at address 0xD0.
    */
    uint8_t getDeviceId();

    /**
        \brief Returns uint8_t that can contain error types.
        \return uint8_t Error types of error that have been found.
    */
    uint8_t getErrors();
private:
    // Helper function for setting registers and validating if set
    bool set_reg(uint8_t reg, uint8_t val);
    // Reads control register data
    uint8_t read_ctrl_reg();

    // Reads configuration register data
    uint8_t read_conf_reg();

    // Reads device id of BMP/BME
    uint8_t read_dev_id_reg();

    // Retrieves all the calibration data from the calibration registers.
    void retrieveCalibrationData();

    // Retrieves calibration data of a single calibration register.
    uint16_t readCalibrationRegister(const uint8_t reg_address);

    // Returns temperature as degrees celcius * 100
    int32_t bmp280_compensate_T_int32(int32_t adc_T);

    // Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
    uint32_t bmp280_compensate_P_int32(int32_t adc_P);

    // The i2c bus that is used to communicate with the BMP280
    hwlib::i2c_bus& hbus;

    // I2c Address of the BMP280
    uint8_t address;

    int32_t t_fine;

    // Internal id of the BMP280
    uint8_t device_id = 0;
    // Calibration values
    // Calibration values temperature
    uint16_t    dig_T1;
    int16_t     dig_T2, dig_T3;

    // Calibration values pressure
    uint16_t    dig_P1;
    int16_t     dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9; 

    // Control measurement data
    uint8_t control_measurement_data = 0x00;
    // Config data
    uint8_t config_data = 0x00;

    // Contains error codes
    uint8_t error = 0x00;

    // True: Checks every write operation to control and config reg to see if succesfully set.
    const bool reg_check = true;
};
#endif // __BMP280LIB_HPP__