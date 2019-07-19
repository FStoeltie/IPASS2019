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
static constexpr uint8_t REG_STANDBY_TIME = 0xF5;

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
    S_4 = 0x07

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
    }
    void test() {
        hbus.write(address).write(0xD0);
        uint8_t result = 0;
        hwlib::wait_ms(10);
        hbus.read(address).read(result);
        hwlib::cout << "result: " << hwlib::hex << result << "\n" << hwlib::flush;

        hwlib::wait_ms(10);
        retrieveCalibrationData();

        hwlib::cout << "temperature guess: " << hwlib::dec << (int32_t)getTemperature() << "\n" << hwlib::flush;

        hwlib::cout << "Testing standby time!\n" << hwlib::flush;
        setStandbyTime(STANDBY_TIME::MS_250);
        hwlib::cout << "Standby reading:\t" << hwlib::hex << readStandbyRegister() << "\n" << hwlib::flush;
    }
    // divide by 1000 to get to degrees celcius
    int32_t getTemperature()   {
        hbus.write(address).write(0xF7);
        uint8_t result_data[6];
        hbus.read(address).read(result_data, 6);
        int32_t raw_temp = (((int32_t) result_data[3] << 12) | ((unsigned long) result_data[4] << 4) | (unsigned long) result_data[5] >> 4) << (0 - 0);
        int32_t temp_result = bmp280_compensate_T_int32(raw_temp);
        return temp_result;
    }
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
    void reset()    {
        uint8_t resetData[2] = {REG_RESET_BMP, RESET_VALUE};
        hbus.write(address).write(resetData, 2);
    }
    void setStandbyTime(STANDBY_TIME standby_time)   {
        uint8_t standby_data[2] = {REG_STANDBY_TIME, static_cast<uint8_t>(standby_time)};
        hbus.write(address).write(standby_data, 2);
    }
    uint8_t readStandbyRegister()   {
        hbus.write(address).write(REG_STANDBY_TIME);
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
    int32_t bmp280_compensate_T_int32(int32_t adc_T)    {
        int32_t var1, var2, T;
        var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1 ))) * ((int32_t) dig_T2 )) >> 11;
        var2 = (((((adc_T >> 4) - ((int32_t) dig_T1 )) * ((adc_T >> 4 ) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3)) >> 14;
        t_fine = var1 + var2;
        T = (t_fine * 5 + 128) >> 8;
        return T;
    }

private:
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
};
#endif // __BMP280LIB_HPP__