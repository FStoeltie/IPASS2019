#include "bmp280lib_herkansing.hpp"

// base_pres in pascal
// in meters
bmp280::bmp280(hwlib::i2c_bus& hbus, uint8_t address) : 
hbus(hbus),
address(address)
{
    control_measurement_data = static_cast<uint8_t>(OVERSAMPLING::OVER_02) | static_cast<uint8_t>(MODE::FORCED);
}
void bmp280::configure()    {
    device_id = read_dev_id_reg();
    if(device_id != BMP280_DEVICE_ID_01 || 
    device_id != BMP280_DEVICE_ID_02 || 
    device_id != BMP280_DEVICE_ID_03 || 
    device_id != BME280_DEVICE_ID_01)  {
        error |= static_cast<uint8_t>(BMP280_ERROR::UNKNOWN_DEVICE_ID);;
    }
    retrieveCalibrationData();
    setMode(MODE::FORCED);
    if (control_measurement_data != read_ctrl_reg())   {
        error |= static_cast<uint8_t>(BMP280_ERROR::UNEXPECTED_REG_DATA);
    }
    setStandbyTime(STANDBY_TIME::MS_500);
    if (config_data != read_conf_reg())   {
        error |= static_cast<uint8_t>(BMP280_ERROR::UNEXPECTED_REG_DATA);
    }
    setIIR(IIR_RES::IIR_02);
/*    setStandbyTime(STANDBY_TIME::S_4);
    setIIR(IIR_RES::IIR_02);
    if(setMode(MODE::NORMAL))    {
        hwlib::cout << "Mode set to SLEEP succesfully!\n" << hwlib::flush;
    }
    else    {
        hwlib::cout << "Mode set to SLEEP has failed!\n" << hwlib::flush;            
    }*/
}
float bmp280::getAltitude(double sea_level_pres) {
   sea_level_pres *= 1000;
   double temp = getTemperature() / 10;
   double pres = getPressure() * 10; // sensor pascal
   float height_in_meters = (double)(((pow(sea_level_pres / pres, 1 / 5.257) - 1) * (temp + 273.15))) / 0.0065;
   return height_in_meters;  
}

// divide by 100 to get to degrees celcius
float bmp280::getTemperature()   {

    if( (control_measurement_data & 0x03 ) == static_cast<uint8_t>(MODE::FORCED) ) {
        setMode(MODE::FORCED);
    }
    hbus.write(address).write(REG_PRES_DATA);
    uint8_t result_data[6];
    
    hbus.read(address).read(result_data, 6);
    int32_t raw_temp = (((int32_t) result_data[3] << 12) | ((int32_t) result_data[4] << 4) | (int32_t) result_data[5] >> 4) << (0 - 0);
    int32_t temp_result = bmp280_compensate_T_int32(raw_temp);
    if((control_measurement_data & 0x03) == static_cast<uint8_t>(MODE::FORCED))  {
        error |= static_cast<uint8_t>(BMP280_ERROR::TEMP_OUT_OF_RANGE);
    }
    return (float)temp_result / 100;
}

uint32_t bmp280::getPressure()   {
    if( (control_measurement_data & 0x03 ) == static_cast<uint8_t>(MODE::FORCED) ){
        setMode(MODE::FORCED);
    }
    hbus.write(address).write(REG_PRES_DATA);
    uint8_t result_data[6];
    hbus.read(address).read(result_data, 6);
    int32_t raw_pressure = (((int32_t) result_data[0] << 12) | ((int32_t) result_data[1] << 4) | (int32_t) result_data[2] >> 4) << (0 - 0);
    uint32_t temp_result = bmp280_compensate_P_int32(raw_pressure);
    if(temp_result < BMP280_MIN_PRESS && temp_result > BMP280_MAX_PRESS)  {
        error |= static_cast<uint8_t>(BMP280_ERROR::TEMP_OUT_OF_RANGE);
    }
    return temp_result;
}

void bmp280::retrieveCalibrationData()  {
    dig_T1 = readCalibrationRegister(REG_DIG_T1);
    dig_T2 = (int16_t) readCalibrationRegister(REG_DIG_T2);
    dig_T3 = (int16_t) readCalibrationRegister(REG_DIG_T3);
    dig_P1 = readCalibrationRegister(REG_DIG_P1);
    dig_P2 = (int16_t) readCalibrationRegister(REG_DIG_P2);
    dig_P3 = (int16_t) readCalibrationRegister(REG_DIG_P3);
    dig_P4 = (int16_t) readCalibrationRegister(REG_DIG_P4);
    dig_P5 = (int16_t) readCalibrationRegister(REG_DIG_P5);
    dig_P6 = (int16_t) readCalibrationRegister(REG_DIG_P6);
    dig_P7 = (int16_t) readCalibrationRegister(REG_DIG_P7);
    dig_P8 = (int16_t) readCalibrationRegister(REG_DIG_P8);
    dig_P9 = (int16_t) readCalibrationRegister(REG_DIG_P8);
}

uint16_t bmp280::readCalibrationRegister(const uint8_t reg_address)   {
    uint8_t dig_result[2];
    hbus.write(address).write(reg_address);
    hbus.read(address).read(dig_result, 2);
    unsigned short result = ((unsigned int)dig_result[1]) << 8 | (unsigned int)dig_result[0];
    return result;
}

void bmp280::reset()   {
    uint8_t resetData[2] = {REG_RESET_BMP, RESET_VALUE};
    hbus.write(address).write(resetData, 2);
}
bool bmp280::setOversampling(OVERSAMPLING os)  {
    uint8_t cast_os = static_cast<uint8_t>(os);
    control_measurement_data = (control_measurement_data & ~cast_os) | cast_os;
    return set_reg(REG_CTRL_MEASUREMENT, control_measurement_data);
/*    uint8_t control_measurement[2] = {REG_CTRL_MEASUREMENT, control_measurement_data};
    hbus.write(address).write(control_measurement, 2);

    if(always_check)    {
        if(!(cos & read_ctrl_reg()))    {
            error |= static_cast<uint8_t>(BMP280_ERROR::UNEXPECTED_REG_DATA);
            return false;
        }
    }
    return true;*/
}

bool bmp280::setMode(MODE m)  {
    uint8_t cast_mode = static_cast<uint8_t>(m);
    control_measurement_data = (control_measurement_data & ~cast_mode) | cast_mode;
    return set_reg(REG_CTRL_MEASUREMENT, control_measurement_data);
}

bool bmp280::setStandbyTime(STANDBY_TIME standby_time)   {
    uint8_t cast_st = static_cast<uint8_t>(standby_time);
    config_data = (config_data & ~cast_st) | cast_st;
    return set_reg(REG_CONFIG, config_data);
}
bool bmp280::setIIR(IIR_RES res)   {
    uint8_t cast_res = static_cast<uint8_t>(res);
    config_data = (config_data & ~cast_res) | cast_res;
    return set_reg(REG_CONFIG, config_data);
}

bool bmp280::set_reg(uint8_t reg, uint8_t val) {
    uint8_t data[] = {reg, val};
    hbus.write(address).write(data, 2);
    if(reg_check)    {
        if(!(val & reg))    {
            error |= static_cast<uint8_t>(BMP280_ERROR::UNEXPECTED_REG_DATA);
            return false;
        }
    }
    return true;
}
uint8_t bmp280::getErrors() {
    return static_cast<uint8_t>(error);
}
uint8_t bmp280::read_conf_reg()   {
    hbus.write(address).write(REG_CONFIG);
    uint8_t standby_time = 0;
    hbus.read(address).read(standby_time);
    return standby_time;
}

uint8_t bmp280::read_ctrl_reg()  {
    hbus.write(address).write(REG_CTRL_MEASUREMENT);
    uint8_t ctrl_measurement = 0;
    hbus.read(address).read(ctrl_measurement);
    return ctrl_measurement;
}
uint8_t bmp280::read_dev_id_reg()  {
    hbus.write(address).write(REG_DEVICE_ID);
    uint8_t device_id = 0;
    hbus.read(address).read(device_id);
    return device_id;
}
uint8_t bmp280::getDeviceId() {
    return device_id;
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
int32_t bmp280::bmp280_compensate_T_int32(int32_t adc_T)    {
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1 ))) * ((int32_t) dig_T2 )) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t) dig_T1 )) * ((adc_T >> 4 ) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
uint32_t bmp280::bmp280_compensate_P_int32(int32_t adc_P)
{
    int32_t var1, var2;
    uint32_t p;
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