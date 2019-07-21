/* File will include own communication */


#ifndef __BMP280LIB_HERKANSING_HPP__
#define __BMP280LIB_HERKANSING_HPP__
#include "bmp280.h"
#include "hwlib.hpp"

#include <math.h>

static constexpr int8_t BMP280_MIN_TEMP = -40;
static constexpr int8_t BMP280_MAX_TEMP = 85;

static constexpr uint16_t BMP280_MIN_PRESS = 300;
static constexpr uint16_t BMP280_MAX_PRESS = 1100;

static constexpr uint8_t BMP280_DEVICE_ID_01 = 0x57;
static constexpr uint8_t BMP280_DEVICE_ID_02 = 0x58;
static constexpr uint8_t BMP280_DEVICE_ID_03 = 0x59;
static constexpr uint8_t BME280_DEVICE_ID_01 = 0x60;

static constexpr uint8_t REG_DEVICE_ID = 0xD0;
static constexpr uint8_t REG_RESET_BMP = 0xE0;
static constexpr uint8_t REG_CTRL_MEASUREMENT = 0xF4;
static constexpr uint8_t REG_CONFIG = 0xF5;

static constexpr uint8_t RESET_VALUE = 0xB6;

static constexpr uint8_t STANDBY_TIME_500_US = 0x00;
static constexpr uint8_t STANDBY_TIME_62500_US = 0x01;
static constexpr uint8_t STANDBY_TIME_125_MS = 0x02;
static constexpr uint8_t STANDBY_TIME_250_MS = 0x03;
static constexpr uint8_t STANDBY_TIME_500_MS = 0x04;
static constexpr uint8_t STANDBY_TIME_1_S = 0x05;
static constexpr uint8_t STANDBY_TIME_2_S = 0x06;
static constexpr uint8_t STANDBY_TIME_4_S = 0x07;

// standby time between measurements in normal mode
enum class STANDBY_TIME : uint8_t{
    US_500 = 0x00,
    US_62500 = 0x20,
    MS_125 = 0x40,
    MS_250 = 0x60,
    MS_500 = 0x80,
    S_1 = 0xA0,
    S_2 = 0xC0,
    S_4 = 0xE0,
};
enum class IIR_RES : uint8_t{
    IIR_OFF = 0x00,
    IIR_02 = 0x02,
    IIR_04 = 0x04,
    IIR_08 = 0x08,
    IIR_16 = 0x10,
};
enum class PRES_OVERSAMPLING : uint8_t{
    PRES_OS_OFF = 0x00,
    PRES_OS_01 = 0x04,
    PRES_OS_02 = 0x08,
    PRES_OS_04 = 0x0B,
    PRES_OS_08 = 0x10,
    PRES_OS_16 = 0x14,
};
enum class TEMP_OVERSAMPLING : uint8_t{
    TEMP_OS_OFF = 0x00,
    TEMP_OS_01 = 0x20,
    TEMP_OS_02 = 0x40,
    TEMP_OS_04 = 0x50,
    TEMP_OS_08 = 0x80,
    TEMP_OS_16 = 0xA0,
};
enum class OVERSAMPLING : uint8_t   {
    OVER_01 = static_cast<uint8_t>(PRES_OVERSAMPLING::PRES_OS_01) | static_cast<uint8_t>(TEMP_OVERSAMPLING::TEMP_OS_01),
    OVER_02 = static_cast<uint8_t>(PRES_OVERSAMPLING::PRES_OS_02) | static_cast<uint8_t>(TEMP_OVERSAMPLING::TEMP_OS_02),
    OVER_04 = static_cast<uint8_t>(PRES_OVERSAMPLING::PRES_OS_04) | static_cast<uint8_t>(TEMP_OVERSAMPLING::TEMP_OS_04),
    OVER_08 = static_cast<uint8_t>(PRES_OVERSAMPLING::PRES_OS_08) | static_cast<uint8_t>(TEMP_OVERSAMPLING::TEMP_OS_08),
    OVER_16 = static_cast<uint8_t>(PRES_OVERSAMPLING::PRES_OS_16) | static_cast<uint8_t>(TEMP_OVERSAMPLING::TEMP_OS_16),
};

enum class MODE : uint8_t {
    SLEEP = 0x00,
    FORCED = 0x01,
    NORMAL = 0x11,
};

enum class BMP280_ERROR : uint8_t{
    NO_ERROR = 0x00,
    UNKNOWN_DEVICE_ID = 0x01,
    UNEXPECTED_REG_DATA = (0x01) << 1,
    TEMP_OUT_OF_RANGE = (0x01) << 2,
    PRES_OUT_OF_RANGE = (0x01) << 3,
};
  
inline hwlib::ostream & operator<< ( hwlib::ostream & stream, const float & f ) {
    int i_value = (int)f; 
  
    float f_value = f - (float)i_value; 
    f_value *= pow(10, 6);
    //uint8_t old_value = (uint8_t) f;
    //stream << old_value << "." << (uint8_t) ((f - old_value) * 10); 
    stream << i_value << '.' << (int)f_value;
/*    if (mantisse_value != 0) {

    }
*/
  return stream;
}
class bmp280lib_herkansing {
public:
    bmp280lib_herkansing(hwlib::i2c_bus& hbus, uint8_t address);
    void configure();
    void test() {
        hbus.write(address).write(0xD0);
        uint8_t result = 0;
        hbus.read(address).read(result);
        hwlib::cout << "result: " << hwlib::hex << result << "\n" << hwlib::flush;

        setOversampling(OVERSAMPLING::OVER_02);

        hbus.write(address).write(REG_CTRL_MEASUREMENT);
        uint8_t reg_config_val = 0;
        hbus.read(address).read(reg_config_val);
        hwlib::cout << "reg_config_val reading:\t0x" << hwlib::hex << readCtrlRegister() << "\n" << hwlib::flush;

        setStandbyTime(STANDBY_TIME::S_4);
        setIIR(IIR_RES::IIR_16);
        hwlib::cout << "Dig_T1:\t" << hwlib::dec << dig_T1 << "\n" << hwlib::flush;
        hwlib::cout << "Dig_T2:\t" << hwlib::dec << dig_T2 << "\n" << hwlib::flush;
        hwlib::cout << "Dig_T3:\t" << hwlib::dec << dig_T3 << "\n" << hwlib::flush;

        hwlib::cout << "temperature guess: " << hwlib::dec << (int32_t)getTemperature() << "\n" << hwlib::flush;
        hwlib::cout << "temperature guess with a floating point value: " << hwlib::dec << (float)getTemperature()/1000 << "\n" << hwlib::flush;

        hwlib::cout << "Testing standby time!\n" << hwlib::flush;
        
        hwlib::cout << "Standby reading:\t0x" << hwlib::hex << readConfigRegister() << "\n" << hwlib::flush;

        hwlib::cout << "Pressure guess: " << hwlib::dec << (uint32_t)getPressure() << "\n" << hwlib::flush;
        hwlib::cout << "Altitude guess: " << hwlib::dec << (int32_t)getAltitude(1022.7) << "\n" << hwlib::flush;
        hwlib::cout << "testing a float value:\t" << hwlib::dec << (float)5.5 << "\n" << hwlib::flush;
    }
    // divide by 1000 to get to degrees celcius
    int32_t getTemperature();
    
    uint32_t getPressure();

    /**
    \brief Helper method that gets the altitude based on the temperature and pressure. The return value is expressed in meters.
    \param[in] The local pressure at sea level, expressed in hectopascal (hPa).
    \return float Altitude in meters.
    */
    float getAltitude(double sea_level_pres = 1013.15);

    void setIIR(IIR_RES res);
    void reset();

    void setOversampling(OVERSAMPLING os);
    void setMode(MODE m);

    void setStandbyTime(STANDBY_TIME standby_time);
    
    uint8_t getDeviceId();

private:
    
    uint8_t readCtrlRegister();
    uint8_t readConfigRegister();
    uint8_t readDeviceIdRegister();

    void retrieveCalibrationData();
    uint16_t readCalibrationRegister(const uint8_t reg_address);

    int32_t bmp280_compensate_T_int32(int32_t adc_T);
    // Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
    uint32_t bmp280_compensate_P_int32(int32_t adc_P);

    hwlib::i2c_bus& hbus;
    uint8_t address;
    int32_t t_fine;
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

    uint8_t error = 0x00;
};
#endif // __BMP280LIB_HPP__