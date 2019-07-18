/* File will include own communication */


#ifndef __BMP280LIB_HERKANSING_HPP__
#define __BMP280LIB_HERKANSING_HPP__
#include "bmp280.h"
#include "hwlib.hpp"

#include <math.h>

class bmp280lib_herkansing {
public:
    bmp280lib_herkansing(hwlib::i2c_bus& hbus, uint8_t address) : 
    hbus(hbus),
    address(address)
    {}
    void test() {
        hbus.write(address).write(0xD0);
        uint8_t result = 0;
        hwlib::wait_ms(10);
        hbus.read(address).read(result);
        hwlib::cout << "result: " << hwlib::hex << result << "\n" << hwlib::flush;

        hwlib::wait_ms(10);
        retrieveCalibrationData();
/*      uint8_t dig_result[2];
        hbus.write(address).write(0x88);
        hwlib::wait_ms(10);
        hbus.read(address).read(dig_result, 2);
        unsigned short dig_T1 = ((unsigned int)dig_result[1]) << 8 | (unsigned int)dig_result[0];
        hwlib::cout << "dig1: " << hwlib::dec << dig_T1 << "\n" << hwlib::flush;
        hwlib::wait_ms(10);

        hbus.write(address).write(0x8A);
        hwlib::wait_ms(10);
        hbus.read(address).read(dig_result, 2);
        signed short dig_T2 = (int)(dig_result[1]) << 8 | (int)dig_result[0];
        hwlib::cout << "dig2: " << hwlib::dec << dig_T2 << "\n" << hwlib::flush;
        hwlib::wait_ms(10);

        hbus.write(address).write(0x8C);
        hwlib::wait_ms(10);
        hbus.read(address).read(dig_result, 2);
        signed short dig_T3 = (int)(dig_result[1]) << 8 | (int)dig_result[0];
        hwlib::cout << "dig3: " << hwlib::dec << dig_T3 << "\n" << hwlib::flush;
        hwlib::wait_ms(10);*/

        /*hbus.write(address).write(0x7A);
        hwlib::wait_ms(10);
        uint8_t result_data[3];
        hbus.read(address).read(result_data, 3);
        int32_t up = (((unsigned long) result_data[0] << 11) | ((unsigned long) result_data[1] << 4) | (unsigned long) result_data[2]) >> (0 - 0);
        int32_t temp_result = bmp280_compensate_T_int32(up, dig_T1, dig_T2, dig_T3);
        hwlib::cout << "temperature guess: " << hwlib::dec << (int32_t)temp_result << "\n" << hwlib::flush;

        hwlib::wait_ms(10);
        double factor = pow(2, 4);
        double uT = (( (result_data[0] *256.0) + result_data[1] + (result_data[2]/256.0))) * factor;   //20bit UT
        hwlib::cout << "uT result hwlib: " << hwlib::dec << (int32_t)uT << "\n" << hwlib::flush;
        double temp_result2 = 0;
        calcTemperature(temp_result2, uT, dig_T1, dig_T2, dig_T3);
        hwlib::cout << "temperature guess 2: " << hwlib::dec << (int32_t)temp_result2 << "\n" << hwlib::flush;*/

        hbus.write(address).write(0xF7);
        hwlib::wait_ms(10);
        uint8_t result_data[6];
        hbus.read(address).read(result_data, 6);
        int32_t up = (((int32_t) result_data[3] << 12) | ((unsigned long) result_data[4] << 4) | (unsigned long) result_data[5] >> 4) << (0 - 0);
        int32_t temp_result = bmp280_compensate_T_int32(up);
        hwlib::cout << "temperature guess: " << hwlib::dec << (int32_t)temp_result << "\n" << hwlib::flush;

        hwlib::wait_ms(10);
        double factor = pow(2, 4);
        double uT = (( (result_data[3] *256.0) + result_data[4] + (result_data[5]/256.0))) * factor;   //20bit UT
        hwlib::cout << "uT result hwlib: " << hwlib::dec << (int32_t)uT << "\n" << hwlib::flush;
        double temp_result2 = 0;
        calcTemperature(temp_result2, uT);
        hwlib::cout << "temperature guess 2: " << hwlib::dec << (int32_t)temp_result2 << "\n" << hwlib::flush;
    }

    // Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
    // t_fine carries fine temperature as global value
    int32_t t_fine;

    /*
    ** temperature calculation
    ** @param : T  = stores the temperature value after calculation.
    ** @param : uT = the uncalibrated temperature value.
    */
    char calcTemperature(double &T, double uT)
    {
        double adc_T = uT ;
        //Serial.print("adc_T = "); Serial.println(adc_T,DEC);
            
        double var1 = (((double)adc_T)/16384.0-((double)dig_T1)/1024.0)*((double)dig_T2);
        double var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0)*(((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0))*((double)dig_T3);
        t_fine = (long signed int)(var1+var2);
            
        T = (var1+var2)/5120.0;
        
        if(T>100.0 || T <-100.0)return 0;
        
        return (1);
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
    uint16_t readCalibrationRegister(uint8_t reg_address)   {
        uint8_t dig_result[2];
        hbus.write(address).write(reg_address);
        hwlib::wait_ms(10);
        hbus.read(address).read(dig_result, 2);
        unsigned short result = ((unsigned int)dig_result[1]) << 8 | (unsigned int)dig_result[0];
        hwlib::wait_ms(10);
        return result;
    }

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

    // Calibration values
    // Calibration values temperature
    uint16_t    dig_T1;
    int16_t     dig_T2, dig_T3;

    // Calibration values pressure
    uint16_t    dig_P1;
    int16_t     dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9; 
};
#endif // __BMP280LIB_HPP__