#include "bmp280lib_herkansing.hpp"

// base_pres in pascal
// in meters

float bmp280lib_herkansing::getAltitude(double sea_level_pres) {
   sea_level_pres *= 1000;
   double temp = getTemperature() / 1000;
   double pres = getPressure() * 10; // sensor pascal
   float height_in_meters = (double)(((pow(sea_level_pres / pres, 1 / 5.257) - 1) * (temp + 273.15))) / 0.0065;
   return height_in_meters;  
}
// divide by 1000 to get to degrees celcius
int32_t bmp280lib_herkansing::getTemperature()   {
    hwlib::wait_ms( 20 );
    hbus.write(address).write(0xF7);
    uint8_t result_data[6];
    hwlib::wait_ms( 20 );
    hbus.read(address).read(result_data, 6);
    int32_t raw_temp = (((int32_t) result_data[3] << 12) | ((int32_t) result_data[4] << 4) | (int32_t) result_data[5] >> 4) << (0 - 0);
    int32_t temp_result = bmp280_compensate_T_int32(raw_temp);
    if(temp_result < BMP280_MIN_TEMP && temp_result > BMP280_MAX_TEMP)  {
        error |= static_cast<uint8_t>(BMP280_ERROR::TEMP_OUT_OF_RANGE);
    }
    return temp_result;
}
uint32_t bmp280lib_herkansing::getPressure()   {
    hbus.write(address).write(0xF7);
    uint8_t result_data[6];
    hbus.read(address).read(result_data, 6);
    int32_t raw_pressure = (((int32_t) result_data[0] << 12) | ((int32_t) result_data[1] << 4) | (int32_t) result_data[2] >> 4) << (0 - 0);
    uint32_t temp_result = bmp280_compensate_P_int32(raw_pressure);
    if(temp_result < BMP280_MIN_PRESS && temp_result > BMP280_MAX_PRESS)  {
        error |= static_cast<uint8_t>(BMP280_ERROR::TEMP_OUT_OF_RANGE);
    }
    return temp_result;
}

void bmp280lib_herkansing::retrieveCalibrationData()  {
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

uint16_t bmp280lib_herkansing::readCalibrationRegister(const uint8_t reg_address)   {
    uint8_t dig_result[2];
    hbus.write(address).write(reg_address);
    hbus.read(address).read(dig_result, 2);
    unsigned short result = ((unsigned int)dig_result[1]) << 8 | (unsigned int)dig_result[0];
    return result;
}

void bmp280lib_herkansing::reset()   {
    uint8_t resetData[2] = {REG_RESET_BMP, RESET_VALUE};
    hbus.write(address).write(resetData, 2);
}
void bmp280lib_herkansing::setOversampling(OVERSAMPLING os)  {
    control_measurement_data = (control_measurement_data & ~static_cast<uint8_t>(os)) | static_cast<uint8_t>(os);
    uint8_t control_measurement[2] = {REG_CTRL_MEASUREMENT, control_measurement_data};
    hbus.write(address).write(control_measurement, 2);
}

void bmp280lib_herkansing::setMode(MODE m)  {
    control_measurement_data = (control_measurement_data & ~static_cast<uint8_t>(m)) | static_cast<uint8_t>(m);
    uint8_t control_measurement[2] = {REG_CTRL_MEASUREMENT, control_measurement_data};
    hbus.write(address).write(control_measurement, 2);
}

void bmp280lib_herkansing::setStandbyTime(STANDBY_TIME standby_time)   {
    config_data = (config_data & ~static_cast<uint8_t>(standby_time)) | static_cast<uint8_t>(standby_time);
    uint8_t standby_data[2] = {REG_CONFIG, config_data};
    hbus.write(address).write(standby_data, 2);
}
void bmp280lib_herkansing::setIIR(IIR_RES res)   {
    config_data = (config_data & ~static_cast<uint8_t>(res)) | static_cast<uint8_t>(res);
    uint8_t standby_data[2] = {REG_CONFIG, config_data};
    hbus.write(address).write(standby_data, 2);
}

uint8_t bmp280lib_herkansing::readConfigRegister()   {
    hbus.write(address).write(REG_CONFIG);
    uint8_t standby_time = 0;
    hbus.read(address).read(standby_time);
    return standby_time;
}

uint8_t bmp280lib_herkansing::readCtrlRegister()  {
    hbus.write(address).write(REG_CTRL_MEASUREMENT);
    uint8_t ctrl_measurement = 0;
    hbus.read(address).read(ctrl_measurement);
    return ctrl_measurement;
}
uint8_t bmp280lib_herkansing::readDeviceIdRegister()  {
    hbus.write(address).write(REG_DEVICE_ID);
    uint8_t device_id = 0;
    hbus.read(address).read(device_id);
    return device_id;
}
bool bmp280lib_herkansing::getDeviceId()  {
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
int32_t bmp280lib_herkansing::bmp280_compensate_T_int32(int32_t adc_T)    {
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1 ))) * ((int32_t) dig_T2 )) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t) dig_T1 )) * ((adc_T >> 4 ) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
uint32_t bmp280lib_herkansing::bmp280_compensate_P_int32(int32_t adc_P)
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
