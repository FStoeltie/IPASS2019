/* File will include own communication */


#ifndef __BMP280LIB_HERKANSING_HPP__
#define __BMP280LIB_HERKANSING_HPP__
#include "bmp280.h"
#include "hwlib.hpp"

#include <math.h>
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
    US_62500 = 0x01,
    MS_125 = 0x02,
    MS_250 = 0x03,
    MS_500 = 0x04,
    S_1 = 0x05,
    S_2 = 0x06,
    S_4 = 0x07,
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
class bmp280lib_herkansing {
public:
    bmp280lib_herkansing(hwlib::i2c_bus& hbus, uint8_t address) : 
    hbus(hbus),
    address(address)
    {
        if(getDeviceId())   {
            hwlib::cout << "Device ID recognized! " << hwlib::hex << deviceId << "\n" << hwlib::flush;
        }
        retrieveCalibrationData();
        control_measurement_data = static_cast<uint8_t>(OVERSAMPLING::OVER_04) | static_cast<uint8_t>(MODE::NORMAL);
        setMode(MODE::NORMAL);
    }
    void test() {
        hbus.write(address).write(0xD0);
        uint8_t result = 0;
        hwlib::wait_ms(10);
        hbus.read(address).read(result);
        hwlib::cout << "result: " << hwlib::hex << result << "\n" << hwlib::flush;

        hwlib::wait_ms(50);
        setOversampling(OVERSAMPLING::OVER_02);
/*        uint8_t temp_config_test[2] = {0XF4, 0x23};
        hbus.write(address).write(temp_config_test, 2);*/
        //reset();
        hwlib::wait_ms(50);
        hbus.write(address).write(0XF4);
        uint8_t reg_config_val = 0;
        hbus.read(address).read(reg_config_val);
        hwlib::cout << "reg_config_val reading:\t0x" << hwlib::hex << reg_config_val << "\n" << hwlib::flush;
        //retrieveCalibrationData();
        //setStandbyTime(STANDBY_TIME::US_500);
        hwlib::wait_ms(50);
        hwlib::cout << "Dig_T1:\t" << hwlib::dec << dig_T1 << "\n" << hwlib::flush;
        hwlib::cout << "Dig_T2:\t" << hwlib::dec << dig_T2 << "\n" << hwlib::flush;
        hwlib::cout << "Dig_T3:\t" << hwlib::dec << dig_T3 << "\n" << hwlib::flush;

        hwlib::cout << "temperature guess: " << hwlib::dec << (int32_t)getTemperature() << "\n" << hwlib::flush;

        hwlib::cout << "Testing standby time!\n" << hwlib::flush;
        
        hwlib::cout << "Standby reading:\t0x" << hwlib::hex << readStandbyRegister() << "\n" << hwlib::flush;

        hwlib::cout << "Pressure guess: " << hwlib::dec << (uint32_t)getPressure() << "\n" << hwlib::flush;
    }
    // divide by 1000 to get to degrees celcius
    int32_t getTemperature()   {
        hwlib::wait_ms( 20 );
        hbus.write(address).write(0xF7);
        uint8_t result_data[6];
        hwlib::wait_ms( 20 );
        hbus.read(address).read(result_data, 6);
        int32_t raw_temp = (((int32_t) result_data[3] << 12) | ((int32_t) result_data[4] << 4) | (int32_t) result_data[5] >> 4) << (0 - 0);
/*        hwlib::cout << "result 1 " << result_data[3] << "\n" << hwlib::flush;
        hwlib::cout << "result 2 " << result_data[4] << "\n" << hwlib::flush;
        hwlib::cout << "result 3 " << result_data[5] << "\n" << hwlib::flush;*/
        int32_t temp_result = bmp280_compensate_T_int32(raw_temp);
        return temp_result;
    }
    
    uint32_t getPressure()   {
        hbus.write(address).write(0xF7);
        uint8_t result_data[6];
        hbus.read(address).read(result_data, 6);
        int32_t raw_pressure = (((int32_t) result_data[0] << 12) | ((int32_t) result_data[1] << 4) | (int32_t) result_data[2] >> 4) << (0 - 0);
        uint32_t temp_result = bmp280_compensate_P_int32(raw_pressure);
        return temp_result;
    }

    /**
    \brief Helper method that gets the altitude based on the temperature and pressure. The return value is expressed in meters.
    \param[in] The local pressure at sea level, expressed in hectopascal (hPa).
    \return float Altitude in meters.
    */
    float get_altitude(double base_pres = 1013.15);

    void retrieveCalibrationData()  {
        dig_T1 = readCalibrationRegister(0x88);
        dig_T2 = (int16_t) readCalibrationRegister(0x8A);
        dig_T3 = (int16_t) readCalibrationRegister(0x8C);
        dig_P1 = readCalibrationRegister(0x8E);
        dig_P2 = (int16_t) readCalibrationRegister(0x90);
        dig_P3 = (int16_t) readCalibrationRegister(0x92);
        dig_P4 = (int16_t) readCalibrationRegister(0x94);
        dig_P5 = (int16_t) readCalibrationRegister(0x96);
        dig_P6 = (int16_t) readCalibrationRegister(0x98);
        dig_P7 = (int16_t) readCalibrationRegister(0x9A);
        dig_P8 = (int16_t) readCalibrationRegister(0x9C);
        dig_P9 = (int16_t) readCalibrationRegister(0x9E);
    }
    uint16_t readCalibrationRegister(const uint8_t reg_address)   {
        uint8_t dig_result[2];
        hbus.write(address).write(reg_address);
        hbus.read(address).read(dig_result, 2);
        unsigned short result = ((unsigned int)dig_result[1]) << 8 | (unsigned int)dig_result[0];
        return result;
    }
    void reset()   {
        uint8_t resetData[2] = {REG_RESET_BMP, RESET_VALUE};
        hbus.write(address).write(resetData, 2);
    }

    void setOversampling(OVERSAMPLING os)  {
        control_measurement_data = (control_measurement_data & ~static_cast<uint8_t>(os)) | static_cast<uint8_t>(os);
        uint8_t control_measurement[2] = {REG_CTRL_MEASUREMENT, control_measurement_data};
        hbus.write(address).write(control_measurement, 2);
    }
    void setMode(MODE m)  {
        control_measurement_data = (control_measurement_data & ~static_cast<uint8_t>(m)) | static_cast<uint8_t>(m);
        uint8_t control_measurement[2] = {REG_CTRL_MEASUREMENT, control_measurement_data};
        hbus.write(address).write(control_measurement, 2);
    }

    void setStandbyTime(STANDBY_TIME standby_time)   {
        config_data = ((config_data & 0x1F) | (static_cast<uint8_t>(standby_time) << 5));

        uint8_t standby_data[2] = {REG_CONFIG, config_data};
        hbus.write(address).write(standby_data, 2);
    }
    uint8_t readStandbyRegister()   {
        hbus.write(address).write(REG_CONFIG);
        uint8_t standby_time = 0;
        hbus.read(address).read(standby_time);
        return standby_time;
    }
    bool getDeviceId()  {
        hbus.write(address).write(REG_DEVICE_ID);
        uint8_t result = 0;
        hbus.read(address).read(result);
        if(result == BMP280_DEVICE_ID_01 || 
            result == BMP280_DEVICE_ID_02 || 
            result == BMP280_DEVICE_ID_03 || 
            result == BME280_DEVICE_ID_01)  {
            deviceId = result;
            return true;
        }
        else    {
            return false;
        }
    }
    // Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
    // t_fine carries fine temperature as global value
    int32_t t_fine;
/*    int32_t bmp280_compensate_T_int32(int32_t adc_T, int32_t dig_T1, int32_t dig_T2, int32_t dig_T3)
    {
        int32_t var1, var2, T;
        var1 = ((((adc_T>>3) – ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
        var2 = (((((adc_T>>4) – ((int32_t)dig_T1)) * ((adc_T>>4) – ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
        t_fine = var1 + var2;
        T = (t_fine * 5 + 128) >> 8;
        return T;
    }*/

private:

    int32_t bmp280_compensate_T_int32(int32_t adc_T)    {
        int32_t var1, var2, T;
        var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1 ))) * ((int32_t) dig_T2 )) >> 11;
        var2 = (((((adc_T >> 4) - ((int32_t) dig_T1 )) * ((adc_T >> 4 ) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3)) >> 14;
        t_fine = var1 + var2;
        T = (t_fine * 5 + 128) >> 8;
        return T;
    }
    // Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
    uint32_t bmp280_compensate_P_int32(int32_t adc_P)
    {
        int32_t var1, var2;
        uint32_t p;
        //var1 = (((int32_t)t_fine) >> 1) – (int32_t)64000;
        var1 = (((int32_t) t_fine) >> 1) - (int32_t) 64000;
        var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)dig_P6);
        var2 = var2 + ((var1*((int32_t)dig_P5))<<1);
        var2 = (var2>>2)+(((int32_t)dig_P4)<<16);
        var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)dig_P2) * var1)>>1))>>18;
        var1 =((((32768+var1))*((int32_t)dig_P1))>>15);
        if (var1 == 0)
        {
            return 0; // avoid exception caused by division by zero
        }
            p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;
        if (p < 0x80000000)
        {
            p = (p << 1) / ((uint32_t)var1);
        }
        else
        {
            p = (p / (uint32_t)var1) * 2;
        }
        var1 = (((int32_t)dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
        var2 = (((int32_t)(p>>2)) * ((int32_t)dig_P8))>>13;
        p = (uint32_t)((int32_t)p + ((var1 + var2 + dig_P7) >> 4));
        return p;
    }
    
    hwlib::i2c_bus& hbus;
    uint8_t address;

    uint8_t deviceId = 0;
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

};
#endif // __BMP280LIB_HPP__