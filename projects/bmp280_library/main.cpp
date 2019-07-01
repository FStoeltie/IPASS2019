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

//#include "bmp280.hpp"

// possible_fixes start

// possible_fixes end


// bmp280_defs_hpp begin

/****************************************************************/
/*! @name       Header includes             */
/****************************************************************/
#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/kernel.h>
#else
#include <stdint.h>
#include <stddef.h>
#endif

/****************************************************************/
/*! @name       Common macros               */
/****************************************************************/
#ifdef __KERNEL__
#if !defined(UINT8_C) && !defined(INT8_C)
#define INT8_C(x)   S8_C(x)
#define UINT8_C(x)  U8_C(x)
#endif

#if !defined(UINT16_C) && !defined(INT16_C)
#define INT16_C(x)  S16_C(x)
#define UINT16_C(x) U16_C(x)
#endif

#if !defined(INT32_C) && !defined(UINT32_C)
#define INT32_C(x)  S32_C(x)
#define UINT32_C(x) U32_C(x)
#endif

#if !defined(INT64_C) && !defined(UINT64_C)
#define INT64_C(x)  S64_C(x)
#define UINT64_C(x) U64_C(x)
#endif
#endif

/*! @name C standard macros */
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *) 0)
#endif
#endif

/****************************************************************/
/*! @name       BMP280 Macros               */
/****************************************************************/

/*! @name Macro to disable double precision floating point compensation
 * @note Uncomment the following line to disable it
 */
#ifndef BMP280_DISABLE_DOUBLE_COMPENSATION

/* #define BMP280_DISABLE_DOUBLE_COMPENSATION */
#endif

/*! @name Macro to disable 64bit compensation
 * @note Uncomment the following line to disable it
 */
#ifndef BMP280_DISABLE_64BIT_COMPENSATION

/* #define BMP280_DISABLE_64BIT_COMPENSATION */
#endif

/*! @name Interface selection macros */
#define BMP280_SPI_INTF                      UINT8_C(0)
#define BMP280_I2C_INTF                      UINT8_C(1)

/*! @name Return codes */
/*! @name Success code*/
#define BMP280_OK                            INT8_C(0)
#define BMP280_BOND_WIRE_OK                  INT8_C(0)

/*! @name Error codes */
#define BMP280_E_NULL_PTR                    INT8_C(-1)
#define BMP280_E_DEV_NOT_FOUND               INT8_C(-2)
#define BMP280_E_INVALID_LEN                 INT8_C(-3)
#define BMP280_E_COMM_FAIL                   INT8_C(-4)
#define BMP280_E_INVALID_MODE                INT8_C(-5)
#define BMP280_E_BOND_WIRE                   INT8_C(-6)
#define BMP280_E_IMPLAUS_TEMP                INT8_C(-7)
#define BMP280_E_IMPLAUS_PRESS               INT8_C(-8)
#define BMP280_E_CAL_PARAM_RANGE             INT8_C(-9)
#define BMP280_E_UNCOMP_TEMP_RANGE           INT8_C(-10)
#define BMP280_E_UNCOMP_PRES_RANGE           INT8_C(-11)
#define BMP280_E_UNCOMP_TEMP_AND_PRESS_RANGE INT8_C(-12)
#define BMP280_E_UNCOMP_DATA_CALC            INT8_C(-13)
#define BMP280_E_32BIT_COMP_TEMP             INT8_C(-14)
#define BMP280_E_32BIT_COMP_PRESS            INT8_C(-15)
#define BMP280_E_64BIT_COMP_PRESS            INT8_C(-16)
#define BMP280_E_DOUBLE_COMP_TEMP            INT8_C(-17)
#define BMP280_E_DOUBLE_COMP_PRESS           INT8_C(-18)

/*! @name Chip IDs for samples and mass production parts */
//#define BMP280_CHIP_ID1                      UINT8_C(0x56)
#define BMP280_CHIP_ID1                      UINT8_C(0x60)
#define BMP280_CHIP_ID2                      UINT8_C(0x57)
#define BMP280_CHIP_ID3                      UINT8_C(0x58)

/*! @name I2C addresses */
#define BMP280_I2C_ADDR_PRIM                 UINT8_C(0x76)
#define BMP280_I2C_ADDR_SEC                  UINT8_C(0x77)

/*! @name Calibration parameter register addresses*/
#define BMP280_DIG_T1_LSB_ADDR               UINT8_C(0x88)
#define BMP280_DIG_T1_MSB_ADDR               UINT8_C(0x89)
#define BMP280_DIG_T2_LSB_ADDR               UINT8_C(0x8A)
#define BMP280_DIG_T2_MSB_ADDR               UINT8_C(0x8B)
#define BMP280_DIG_T3_LSB_ADDR               UINT8_C(0x8C)
#define BMP280_DIG_T3_MSB_ADDR               UINT8_C(0x8D)
#define BMP280_DIG_P1_LSB_ADDR               UINT8_C(0x8E)
#define BMP280_DIG_P1_MSB_ADDR               UINT8_C(0x8F)
#define BMP280_DIG_P2_LSB_ADDR               UINT8_C(0x90)
#define BMP280_DIG_P2_MSB_ADDR               UINT8_C(0x91)
#define BMP280_DIG_P3_LSB_ADDR               UINT8_C(0x92)
#define BMP280_DIG_P3_MSB_ADDR               UINT8_C(0x93)
#define BMP280_DIG_P4_LSB_ADDR               UINT8_C(0x94)
#define BMP280_DIG_P4_MSB_ADDR               UINT8_C(0x95)
#define BMP280_DIG_P5_LSB_ADDR               UINT8_C(0x96)
#define BMP280_DIG_P5_MSB_ADDR               UINT8_C(0x97)
#define BMP280_DIG_P6_LSB_ADDR               UINT8_C(0x98)
#define BMP280_DIG_P6_MSB_ADDR               UINT8_C(0x99)
#define BMP280_DIG_P7_LSB_ADDR               UINT8_C(0x9A)
#define BMP280_DIG_P7_MSB_ADDR               UINT8_C(0x9B)
#define BMP280_DIG_P8_LSB_ADDR               UINT8_C(0x9C)
#define BMP280_DIG_P8_MSB_ADDR               UINT8_C(0x9D)
#define BMP280_DIG_P9_LSB_ADDR               UINT8_C(0x9E)
#define BMP280_DIG_P9_MSB_ADDR               UINT8_C(0x9F)

/*! @name Other registers */
#define BMP280_CHIP_ID_ADDR                  UINT8_C(0xD0)
#define BMP280_SOFT_RESET_ADDR               UINT8_C(0xE0)
#define BMP280_STATUS_ADDR                   UINT8_C(0xF3)
#define BMP280_CTRL_MEAS_ADDR                UINT8_C(0xF4)
#define BMP280_CONFIG_ADDR                   UINT8_C(0xF5)
#define BMP280_PRES_MSB_ADDR                 UINT8_C(0xF7)
#define BMP280_PRES_LSB_ADDR                 UINT8_C(0xF8)
#define BMP280_PRES_XLSB_ADDR                UINT8_C(0xF9)
#define BMP280_TEMP_MSB_ADDR                 UINT8_C(0xFA)
#define BMP280_TEMP_LSB_ADDR                 UINT8_C(0xFB)
#define BMP280_TEMP_XLSB_ADDR                UINT8_C(0xFC)

/*! @name Power modes */
#define BMP280_SLEEP_MODE                    UINT8_C(0x00)
#define BMP280_FORCED_MODE                   UINT8_C(0x01)
#define BMP280_NORMAL_MODE                   UINT8_C(0x03)

/*! @name Soft reset command */
#define BMP280_SOFT_RESET_CMD                UINT8_C(0xB6)

/*! @name ODR options */
#define BMP280_ODR_0_5_MS                    UINT8_C(0x00)
#define BMP280_ODR_62_5_MS                   UINT8_C(0x01)
#define BMP280_ODR_125_MS                    UINT8_C(0x02)
#define BMP280_ODR_250_MS                    UINT8_C(0x03)
#define BMP280_ODR_500_MS                    UINT8_C(0x04)
#define BMP280_ODR_1000_MS                   UINT8_C(0x05)
#define BMP280_ODR_2000_MS                   UINT8_C(0x06)
#define BMP280_ODR_4000_MS                   UINT8_C(0x07)

/*! @name Over-sampling macros */
#define BMP280_OS_NONE                       UINT8_C(0x00)
#define BMP280_OS_1X                         UINT8_C(0x01)
#define BMP280_OS_2X                         UINT8_C(0x02)
#define BMP280_OS_4X                         UINT8_C(0x03)
#define BMP280_OS_8X                         UINT8_C(0x04)
#define BMP280_OS_16X                        UINT8_C(0x05)

/*! @name Filter coefficient macros */
#define BMP280_FILTER_OFF                    UINT8_C(0x00)
#define BMP280_FILTER_COEFF_2                UINT8_C(0x01)
#define BMP280_FILTER_COEFF_4                UINT8_C(0x02)
#define BMP280_FILTER_COEFF_8                UINT8_C(0x03)
#define BMP280_FILTER_COEFF_16               UINT8_C(0x04)

/*! @name SPI 3-Wire macros */
#define BMP280_SPI3_WIRE_ENABLE              UINT8_C(1)
#define BMP280_SPI3_WIRE_DISABLE             UINT8_C(0)

/*! @name Measurement status */
#define BMP280_MEAS_DONE                     UINT8_C(0)
#define BMP280_MEAS_ONGOING                  UINT8_C(1)

/*! @name Image update */
#define BMP280_IM_UPDATE_DONE                UINT8_C(0)
#define BMP280_IM_UPDATE_ONGOING             UINT8_C(1)

/*! @name Position and mask macros */
#define BMP280_STATUS_IM_UPDATE_POS          UINT8_C(0)
#define BMP280_STATUS_IM_UPDATE_MASK         UINT8_C(0x01)
#define BMP280_STATUS_MEAS_POS               UINT8_C(3)
#define BMP280_STATUS_MEAS_MASK              UINT8_C(0x08)
#define BMP280_OS_TEMP_POS                   UINT8_C(5)
#define BMP280_OS_TEMP_MASK                  UINT8_C(0xE0)
#define BMP280_OS_PRES_POS                   UINT8_C(2)
#define BMP280_OS_PRES_MASK                  UINT8_C(0x1C)
#define BMP280_POWER_MODE_POS                UINT8_C(0)
#define BMP280_POWER_MODE_MASK               UINT8_C(0x03)
#define BMP280_STANDBY_DURN_POS              UINT8_C(5)
#define BMP280_STANDBY_DURN_MASK             UINT8_C(0xE0)
#define BMP280_FILTER_POS                    UINT8_C(2)
#define BMP280_FILTER_MASK                   UINT8_C(0x1C)
#define BMP280_SPI3_ENABLE_POS               UINT8_C(0)
#define BMP280_SPI3_ENABLE_MASK              UINT8_C(0x01)

/*! @name Calibration parameters' relative position */
#define BMP280_DIG_T1_LSB_POS                UINT8_C(0)
#define BMP280_DIG_T1_MSB_POS                UINT8_C(1)
#define BMP280_DIG_T2_LSB_POS                UINT8_C(2)
#define BMP280_DIG_T2_MSB_POS                UINT8_C(3)
#define BMP280_DIG_T3_LSB_POS                UINT8_C(4)
#define BMP280_DIG_T3_MSB_POS                UINT8_C(5)
#define BMP280_DIG_P1_LSB_POS                UINT8_C(6)
#define BMP280_DIG_P1_MSB_POS                UINT8_C(7)
#define BMP280_DIG_P2_LSB_POS                UINT8_C(8)
#define BMP280_DIG_P2_MSB_POS                UINT8_C(9)
#define BMP280_DIG_P3_LSB_POS                UINT8_C(10)
#define BMP280_DIG_P3_MSB_POS                UINT8_C(11)
#define BMP280_DIG_P4_LSB_POS                UINT8_C(12)
#define BMP280_DIG_P4_MSB_POS                UINT8_C(13)
#define BMP280_DIG_P5_LSB_POS                UINT8_C(14)
#define BMP280_DIG_P5_MSB_POS                UINT8_C(15)
#define BMP280_DIG_P6_LSB_POS                UINT8_C(16)
#define BMP280_DIG_P6_MSB_POS                UINT8_C(17)
#define BMP280_DIG_P7_LSB_POS                UINT8_C(18)
#define BMP280_DIG_P7_MSB_POS                UINT8_C(19)
#define BMP280_DIG_P8_LSB_POS                UINT8_C(20)
#define BMP280_DIG_P8_MSB_POS                UINT8_C(21)
#define BMP280_DIG_P9_LSB_POS                UINT8_C(22)
#define BMP280_DIG_P9_MSB_POS                UINT8_C(23)
#define BMP280_CALIB_DATA_SIZE               UINT8_C(24)

/*! @name Bit-slicing macros */
#define BMP280_GET_BITS(bitname, x)                    ((x & bitname##_MASK) \
                                                        >> bitname##_POS)
#define BMP280_SET_BITS(regvar, bitname, val)          ((regvar & \
                                                         ~bitname##_MASK) | ((val << bitname##_POS) & bitname##_MASK))
#define BMP280_SET_BITS_POS_0(reg_data, bitname, data) ((reg_data & \
                                                         ~(bitname##_MASK)) | (data & bitname##_MASK))
#define BMP280_GET_BITS_POS_0(bitname, reg_data)       (reg_data & \
                                                        (bitname##_MASK))

/*! @brief Macros holding the minimum and maximum for trimming values */

#define BMP280_ST_DIG_T1_MIN UINT16_C(19000)
#define BMP280_ST_DIG_T1_MAX UINT16_C(35000)
#define BMP280_ST_DIG_T2_MIN UINT16_C(22000)
#define BMP280_ST_DIG_T2_MAX UINT16_C(30000)
#define BMP280_ST_DIG_T3_MIN INT16_C(-3000)
#define BMP280_ST_DIG_T3_MAX INT16_C(-1000)
#define BMP280_ST_DIG_P1_MIN UINT16_C(30000)
#define BMP280_ST_DIG_P1_MAX UINT16_C(42000)
#define BMP280_ST_DIG_P2_MIN INT16_C(-12970)
#define BMP280_ST_DIG_P2_MAX INT16_C(-8000)
#define BMP280_ST_DIG_P3_MIN INT16_C(-5000)
#define BMP280_ST_DIG_P3_MAX UINT16_C(8000)
#define BMP280_ST_DIG_P4_MIN INT16_C(-10000)
#define BMP280_ST_DIG_P4_MAX UINT16_C(18000)
#define BMP280_ST_DIG_P5_MIN INT16_C(-500)
#define BMP280_ST_DIG_P5_MAX UINT16_C(1100)
#define BMP280_ST_DIG_P6_MIN INT16_C(-1000)
#define BMP280_ST_DIG_P6_MAX UINT16_C(1000)
#define BMP280_ST_DIG_P7_MIN INT16_C(-32768)
#define BMP280_ST_DIG_P7_MAX UINT16_C(32767)
#define BMP280_ST_DIG_P8_MIN INT16_C(-30000)
#define BMP280_ST_DIG_P8_MAX UINT16_C(10000)
#define BMP280_ST_DIG_P9_MIN INT16_C(-10000)
#define BMP280_ST_DIG_P9_MAX UINT16_C(30000)

#define BMP280_GET_BITSLICE(regvar, bitname) \
    ((regvar & bitname##__MSK) >> bitname##__POS)

/*! @brief Macros to read out API revision number */
/*Register holding custom trimming values */
#define BMP280_ST_TRIMCUSTOM_REG               UINT8_C(0x87)
#define BMP280_ST_TRIMCUSTOM_REG_APIREV__POS   UINT8_C(1)
#define BMP280_ST_TRIMCUSTOM_REG_APIREV__MSK   UINT8_C(0x06)
#define BMP280_ST_TRIMCUSTOM_REG_APIREV__LEN   UINT8_C(2)
#define BMP280_ST_TRIMCUSTOM_REG_APIREV__REG   BMP280_ST_TRIMCUSTOM_REG

/* highest API revision supported is revision 0. */
#define BMP280_ST_MAX_APIREVISION              UINT8_C(0x00)

/*! @brief Macros holding the minimum and maximum for trimming values */
/* 0x00000 is minimum output value */
#define BMP280_ST_ADC_T_MIN                    INT32_C(0x00000)

/* 0xFFFF0 is maximum 20-bit output value without over sampling */
#define BMP280_ST_ADC_T_MAX                    INT32_C(0xFFFF0)

/* 0x00000 is minimum output value */
#define BMP280_ST_ADC_P_MIN                    INT32_C(0x00000)

/* 0xFFFF0 is maximum 20-bit output value without over sampling */
#define BMP280_ST_ADC_P_MAX                    INT32_C(0xFFFF0)

/*! @brief Macros holding the bound values for the temperature in degree celsius */

#define BMP280_ST_PLAUSIBLE_TEMP_MIN           UINT8_C(0)
#define BMP280_ST_PLAUSIBLE_TEMP_MAX           UINT8_C(40)

/*! @brief Macros holding the bound values for the pressure in hPa */

#define BMP280_ST_PLAUSIBLE_PRESS_MIN          UINT8_C(900)
#define BMP280_ST_PLAUSIBLE_PRESS_MAX          UINT8_C(1100)

/*! @brief Macros holding integer resolution for pressure in hpa and temperature in C measurement */

#define BMP280_ST_TEMPERATURE_RESOLUTION_INT32 UINT8_C(100)
#define BMP280_ST_PRESSURE_RESOLUTION_INT32    UINT8_C(100)

/*! @name Function pointer type definitions */
typedef int8_t (*bmp280_com_fptr_t)(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
typedef void (*bmp280_delay_fptr_t)(uint32_t period);

/*! @name Calibration parameters' structure */
struct bmp280_calib_param
{
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;
    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
    int32_t t_fine;
};

/*! @name Sensor configuration structure */
struct bmp280_config
{
    uint8_t os_temp;
    uint8_t os_pres;
    uint8_t odr;
    uint8_t filter;
    uint8_t spi3w_en;
};

/*! @name Sensor status structure */
struct bmp280_status
{
    uint8_t measuring;
    uint8_t im_update;
};

/*! @name Uncompensated data structure */
struct bmp280_uncomp_data
{
    int32_t uncomp_temp;
    int32_t uncomp_press;
};

/*! @name API device structure */
struct bmp280_dev
{
    uint8_t chip_id;
    uint8_t dev_id;
    uint8_t intf;
    bmp280_com_fptr_t read;
    bmp280_com_fptr_t write;
    bmp280_delay_fptr_t delay_ms;
    struct bmp280_calib_param calib_param;
    struct bmp280_config conf;
};

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
int8_t bmp280_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, const struct bmp280_dev *dev);

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
int8_t bmp280_set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len, const struct bmp280_dev *dev);

/*!
 * @brief This API triggers the soft reset of the sensor.
 *
 * @param[in] dev : Structure instance of bmp280_dev.
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise..
 */
int8_t bmp280_soft_reset(const struct bmp280_dev *dev);

/*!
 *  @brief This API is the entry point.
 *  It reads the chip-id and calibration data from the sensor.
 *
 *  @param[in,out] dev : Structure instance of bmp280_dev
 *
 *  @return Result of API execution
 *  @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmp280_init(struct bmp280_dev *dev);

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
int8_t bmp280_get_config(struct bmp280_config *conf, struct bmp280_dev *dev);

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
int8_t bmp280_set_config(const struct bmp280_config *conf, struct bmp280_dev *dev);

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
int8_t bmp280_get_status(struct bmp280_status *status, const struct bmp280_dev *dev);

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
int8_t bmp280_get_power_mode(uint8_t *mode, const struct bmp280_dev *dev);

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
int8_t bmp280_set_power_mode(uint8_t mode, struct bmp280_dev *dev);

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
int8_t bmp280_get_uncomp_data(struct bmp280_uncomp_data *uncomp_data, const struct bmp280_dev *dev);

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
int8_t bmp280_get_comp_temp_32bit(int32_t *comp_temp, int32_t uncomp_temp, struct bmp280_dev *dev);

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
int8_t bmp280_get_comp_pres_32bit(uint32_t *comp_pres, int32_t uncomp_pres, const struct bmp280_dev *dev);

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
int8_t bmp280_get_comp_pres_64bit(uint32_t *pressure, int32_t uncomp_pres, const struct bmp280_dev *dev);

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
int8_t bmp280_get_comp_temp_double(double *temperature, int32_t uncomp_temp, struct bmp280_dev *dev);

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
int8_t bmp280_get_comp_pres_double(double *pressure, int32_t uncomp_pres, const struct bmp280_dev *dev);

#endif /* BMP280_DISABLE_DOUBLE_COMPENSATION */

/*!
 * @brief This API computes the measurement time in milliseconds for the
 * active configuration
 *
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Measurement time for the active configuration in milliseconds
 */
uint8_t bmp280_compute_meas_time(const struct bmp280_dev *dev);

/*!
 * @brief This API performs self-test to check whether compensated temperature and pressure are within the range
 *
 * @param[in] dev : structure instance of bmp280_dev
 *
 * @return status of test results such as init, reset, config, adc read, compensation
 * and non-compensation output range of both temperature and pressure
 */
int8_t bmp280_selftest(struct bmp280_dev *dev);

// bmp280_hpp end

// bmp280_cpp start

/********************** Static function declarations ************************/

/*!
 * @brief This internal API is used to check for null-pointers in the device
 * structure.
 *
 * @param[in] dev : Structure instance of bmp280_dev.
 *
 * @return Result of API execution status
 * @retval Zero for Success, non-zero otherwise.
 */
static int8_t null_ptr_check(const struct bmp280_dev *dev);

/*!
 * @brief This internal API interleaves the register addresses and respective
 * register data for a burst write
 *
 * @param[in] reg_addr: Register address array
 * @param[out] temp_buff: Interleaved register address and data array
 * @param[in] reg_addr: Register data array
 * @param[in] len : Length of the reg_addr and reg_data arrays
 *
 * @return Result of API execution status
 * @retval Zero for Success, non-zero otherwise.
 */
static void interleave_data(const uint8_t *reg_addr, uint8_t *temp_buff, const uint8_t *reg_data, uint8_t len);

/*!
 * @brief This API is used to read the calibration parameters used
 * for calculating the compensated data.
 *
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution
 * @retval Zero for Success, non-zero otherwise.
 */
static int8_t get_calib_param(struct bmp280_dev *dev);

/*!
 * @brief This internal API to reset the sensor, restore/set conf, restore/set mode
 *
 * @param[in] mode: Desired mode
 * @param[in] conf : Desired configuration to the bmp280
 * conf.os_temp, conf.os_pres = BMP280_OS_NONE, BMP280_OS_1X,
 *     BMP280_OS_2X, BMP280_OS_4X, BMP280_OS_8X, BMP280_OS_16X
 * conf.mode = BMP280_SLEEP_MODE, BMP280_FORCED_MODE, BMP280_NORMAL_MODE
 * conf.odr = BMP280_ODR_0_5_MS, BMP280_ODR_62_5_MS, BMP280_ODR_125_MS,
 *     BMP280_ODR_250_MS, BMP280_ODR_500_MS, BMP280_ODR_1000_MS,
 *     BMP280_ODR_2000_MS, BMP280_ODR_4000_MS
 * conf.filter = BMP280_FILTER_OFF, BMP280_FILTER_COEFF_2,
 *     BMP280_FILTER_COEFF_4, BMP280_FILTER_COEFF_8, BMP280_FILTER_COEFF_16
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution status
 * @retval Zero for Success, non-zero otherwise.
 */
static int8_t conf_sensor(uint8_t mode, const struct bmp280_config *conf, struct bmp280_dev *dev);

/*!
 * @brief This internal API checks whether the calibration parameters are within the given range
 *
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution status
 * @retval Zero for Success, non-zero otherwise.
 */
static int8_t st_check_cal_param(const struct bmp280_dev *dev);

/*!
 * @This internal API checks whether the uncompensated temperature and pressure are within the range
 *
 * @param[in] utemperature : uncompensated temperature
 * @param[in] upressure : uncompensated pressure
 *
 * @return Result of API execution status
 * @retval Zero for Success, non-zero otherwise.
 */
static int8_t st_check_boundaries(int32_t utemperature, int32_t upressure);

/*!
 * @brief This internal API checks whether the compensated temperature and pressure are within the given range based
 * on the input parameters compensated temperature and pressure
 *
 * @param[in] temperature : compensated temperature
 * @param[in] pressure : compensated pressure
 *
 * @return Result of API execution status
 * @retval Zero for Success, non-zero otherwise.
 */
static int8_t st_check_sensor_range(int32_t temperature, uint32_t pressure);

/*!
 * @brief This internal API configures the device for self test
 *
 * @param[in] conf : Configures conf.os_temp = BMP280_OS_1X, conf.os_pres = BMP280_OS_1X, BMP280_FORCED_MODE,
 * @param[in] dev : Structure instance of bmp280_dev
 *
 * @return Result of API execution status
 * @retval Zero for Success, non-zero otherwise.
 */
static int8_t st_set_config(struct bmp280_config *conf, struct bmp280_dev *dev);

/****************** User Function Definitions *******************************/

/*!
 * @brief This API reads the data from the given register address of the
 * sensor.
 */
int8_t bmp280_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, const struct bmp280_dev *dev)
{
    int8_t rslt;

    rslt = null_ptr_check(dev);
    if ((rslt == BMP280_OK) && (reg_data != NULL))
    {
      hwlib::cout << "hello world\n" << hwlib::flush;
        /* Mask the register address' MSB if interface selected is SPI */
        if (dev->intf == BMP280_SPI_INTF)
        {
            reg_addr = reg_addr | 0x80;
        }
        rslt = dev->read(dev->dev_id, reg_addr, reg_data, len);

        /* Check for communication error and mask with an internal error code */
        if (rslt != BMP280_OK)
        {
            rslt = BMP280_E_COMM_FAIL;
        }
    }
    else
    {
        rslt = BMP280_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API writes the given data to the register addresses
 * of the sensor.
 */
int8_t bmp280_set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len, const struct bmp280_dev *dev)
{
    int8_t rslt;
    uint8_t temp_buff[8]; /* Typically not to write more than 4 registers */
    uint16_t temp_len;
    uint8_t reg_addr_cnt;

    if (len > 4)
    {
        len = 4;
    }
    
    rslt = null_ptr_check(dev);

    if ((rslt == BMP280_OK) && (reg_addr != NULL) && (reg_data != NULL))
    {
        if (len != 0)
        {
            temp_buff[0] = reg_data[0];

            /* Mask the register address' MSB if interface selected is SPI */
            if (dev->intf == BMP280_SPI_INTF)
            {
                /* Converting all the reg address into proper SPI write address
                 * i.e making MSB(R/`W) bit 0
                 */
                for (reg_addr_cnt = 0; reg_addr_cnt < len; reg_addr_cnt++)
                {
                    reg_addr[reg_addr_cnt] = reg_addr[reg_addr_cnt] & 0x7F;
                }
            }

            /* Burst write mode */
            if (len > 1)
            {
                /* Interleave register address w.r.t data for burst write*/
                interleave_data(reg_addr, temp_buff, reg_data, len);
                temp_len = ((len * 2) - 1);
            }
            else
            {
                temp_len = len;
            }

            rslt = dev->write(dev->dev_id, reg_addr[0], temp_buff, temp_len);

            /* Check for communication error and mask with an internal error code */
            if (rslt != BMP280_OK)
            {
                rslt = BMP280_E_COMM_FAIL;
            }
        }
        else
        {
            rslt = BMP280_E_INVALID_LEN;
        }
    }
    else
    {
        rslt = BMP280_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API triggers the soft reset of the sensor.
 */
int8_t bmp280_soft_reset(const struct bmp280_dev *dev)
{
    int8_t rslt;
    uint8_t reg_addr = BMP280_SOFT_RESET_ADDR;
    uint8_t soft_rst_cmd = BMP280_SOFT_RESET_CMD;

    rslt = null_ptr_check(dev);
    if (rslt == BMP280_OK)
    {
        rslt = bmp280_set_regs(&reg_addr, &soft_rst_cmd, 1, dev);

        /* As per the datasheet, startup time is 2 ms. */
        dev->delay_ms(2);
    }

    return rslt;
}

/*!
 * @brief This API is the entry point.
 * It reads the chip-id and calibration data from the sensor.
 */
int8_t bmp280_init(struct bmp280_dev *dev)
{
    int8_t rslt;

    /* Maximum number of tries before timeout */
    uint8_t try_count = 5;

    rslt = null_ptr_check(dev);
    if (rslt == BMP280_OK)
    {
      hwlib::cout << "BMP280_OK\n" << hwlib::flush;
        while (try_count)
        {
            rslt = bmp280_get_regs(BMP280_CHIP_ID_ADDR, &dev->chip_id, 1, dev);

            /* Check for chip id validity */
            if ((rslt == BMP280_OK) &&
                (dev->chip_id == BMP280_CHIP_ID1 || dev->chip_id == BMP280_CHIP_ID2 || dev->chip_id == BMP280_CHIP_ID3))
            {
               hwlib::cout << "BMP280_OK and test ok\n" << hwlib::flush;
                rslt = bmp280_soft_reset(dev);
                if (rslt == BMP280_OK)
                {
                    rslt = get_calib_param(dev);
                }
                break;
            }

            /* Wait for 10 ms */
            dev->delay_ms(10);
            --try_count;
        }

        /* Chip id check failed, and timed out */
        if (!try_count)
        {
            rslt = BMP280_E_DEV_NOT_FOUND;
        }
        if (rslt == BMP280_OK)
        {
            /* Set values to default */
            dev->conf.filter = BMP280_FILTER_OFF;
            dev->conf.os_pres = BMP280_OS_NONE;
            dev->conf.os_temp = BMP280_OS_NONE;
            dev->conf.odr = BMP280_ODR_0_5_MS;
            dev->conf.spi3w_en = BMP280_SPI3_WIRE_DISABLE;
        }
    }

    return rslt;
}

/*!
 * @brief This API reads the data from the ctrl_meas register and config
 * register. It gives the currently set temperature and pressure over-sampling
 * configuration, power mode configuration, sleep duration and
 * IIR filter coefficient.
 */
int8_t bmp280_get_config(struct bmp280_config *conf, struct bmp280_dev *dev)
{
    int8_t rslt;
    uint8_t temp[2] = { 0, 0 };

    rslt = null_ptr_check(dev);
    if ((rslt == BMP280_OK) && (conf != NULL))
    {
        rslt = bmp280_get_regs(BMP280_CTRL_MEAS_ADDR, temp, 2, dev);
        if (rslt == BMP280_OK)
        {
            conf->os_temp = BMP280_GET_BITS(BMP280_OS_TEMP, temp[0]);
            conf->os_pres = BMP280_GET_BITS(BMP280_OS_PRES, temp[0]);
            conf->odr = BMP280_GET_BITS(BMP280_STANDBY_DURN, temp[1]);
            conf->filter = BMP280_GET_BITS(BMP280_FILTER, temp[1]);
            conf->spi3w_en = BMP280_GET_BITS_POS_0(BMP280_SPI3_ENABLE, temp[1]);
            dev->conf = *conf;
        }
    }
    else
    {
        rslt = BMP280_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API writes the data to the ctrl_meas register and config register.
 * It sets the temperature and pressure over-sampling configuration,
 * power mode configuration, sleep duration and IIR filter coefficient.
 */
int8_t bmp280_set_config(const struct bmp280_config *conf, struct bmp280_dev *dev)
{
    return conf_sensor(BMP280_SLEEP_MODE, conf, dev);
}

/*!
 * @brief This API reads the status register
 */
int8_t bmp280_get_status(struct bmp280_status *status, const struct bmp280_dev *dev)
{
    int8_t rslt;
    uint8_t temp;

    rslt = null_ptr_check(dev);
    if ((rslt == BMP280_OK) && (status != NULL))
    {
        rslt = bmp280_get_regs(BMP280_STATUS_ADDR, &temp, 1, dev);
        status->measuring = BMP280_GET_BITS(BMP280_STATUS_MEAS, temp);
        status->im_update = BMP280_GET_BITS_POS_0(BMP280_STATUS_IM_UPDATE, temp);
    }
    else
    {
        rslt = BMP280_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API reads the power mode.
 */
int8_t bmp280_get_power_mode(uint8_t *mode, const struct bmp280_dev *dev)
{
    int8_t rslt;
    uint8_t temp;

    rslt = null_ptr_check(dev);
    if ((rslt == BMP280_OK) && (mode != NULL))
    {
        rslt = bmp280_get_regs(BMP280_CTRL_MEAS_ADDR, &temp, 1, dev);
        *mode = BMP280_GET_BITS_POS_0(BMP280_POWER_MODE, temp);
    }
    else
    {
        rslt = BMP280_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API writes the power mode.
 */
int8_t bmp280_set_power_mode(uint8_t mode, struct bmp280_dev *dev)
{
    int8_t rslt;

    rslt = null_ptr_check(dev);
    if (rslt == BMP280_OK)
    {
        rslt = conf_sensor(mode, &dev->conf, dev);
    }

    return rslt;
}

/*!
 * @brief This API reads the temperature and pressure data registers.
 * It gives the raw temperature and pressure data .
 */
int8_t bmp280_get_uncomp_data(struct bmp280_uncomp_data *uncomp_data, const struct bmp280_dev *dev)
{
    int8_t rslt;
    uint8_t temp[6] = { 0 };

    rslt = null_ptr_check(dev);
    if ((rslt == BMP280_OK) && (uncomp_data != NULL))
    {
        rslt = bmp280_get_regs(BMP280_PRES_MSB_ADDR, temp, 6, dev);
        if (rslt == BMP280_OK)
        {
            uncomp_data->uncomp_press =
                (int32_t) ((((uint32_t) (temp[0])) << 12) | (((uint32_t) (temp[1])) << 4) | ((uint32_t) temp[2] >> 4));
            uncomp_data->uncomp_temp =
                (int32_t) ((((int32_t) (temp[3])) << 12) | (((int32_t) (temp[4])) << 4) | (((int32_t) (temp[5])) >> 4));
            rslt = st_check_boundaries((int32_t)uncomp_data->uncomp_temp, (int32_t)uncomp_data->uncomp_press);
        }
        else
        {
            rslt = BMP280_E_UNCOMP_DATA_CALC;
        }
    }
    else
    {
        rslt = BMP280_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the compensated temperature from
 * uncompensated temperature. This API uses 32 bit integers.
 */
int8_t bmp280_get_comp_temp_32bit(int32_t *comp_temp, int32_t uncomp_temp, struct bmp280_dev *dev)
{
    int32_t var1, var2;
    int8_t rslt;

    rslt = null_ptr_check(dev);
    if (rslt == BMP280_OK)
    {
        var1 =
            ((((uncomp_temp / 8) - ((int32_t) dev->calib_param.dig_t1 << 1))) * ((int32_t) dev->calib_param.dig_t2)) /
            2048;
        var2 =
            (((((uncomp_temp / 16) - ((int32_t) dev->calib_param.dig_t1)) *
               ((uncomp_temp / 16) - ((int32_t) dev->calib_param.dig_t1))) / 4096) *
             ((int32_t) dev->calib_param.dig_t3)) /
            16384;
        dev->calib_param.t_fine = var1 + var2;
        *comp_temp = (dev->calib_param.t_fine * 5 + 128) / 256;
        rslt = BMP280_OK;
    }
    else
    {
        *comp_temp = 0;
        rslt = BMP280_E_32BIT_COMP_TEMP;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the compensated pressure from
 * uncompensated pressure. This API uses 32 bit integers.
 */
int8_t bmp280_get_comp_pres_32bit(uint32_t *comp_pres, int32_t uncomp_pres, const struct bmp280_dev *dev)
{
    int32_t var1, var2;
    int8_t rslt;

    rslt = null_ptr_check(dev);
    if (rslt == BMP280_OK)
    {
        var1 = (((int32_t) dev->calib_param.t_fine) / 2) - (int32_t) 64000;
        var2 = (((var1 / 4) * (var1 / 4)) / 2048) * ((int32_t) dev->calib_param.dig_p6);
        var2 = var2 + ((var1 * ((int32_t) dev->calib_param.dig_p5)) * 2);
        var2 = (var2 / 4) + (((int32_t) dev->calib_param.dig_p4) * 65536);
        var1 =
            (((dev->calib_param.dig_p3 * (((var1 / 4) * (var1 / 4)) / 8192)) / 8) +
             ((((int32_t) dev->calib_param.dig_p2) * var1) / 2)) / 262144;
        var1 = ((((32768 + var1)) * ((int32_t) dev->calib_param.dig_p1)) / 32768);
        *comp_pres = (((uint32_t) (((int32_t)1048576) - uncomp_pres) - (var2 / 4096))) * 3125;

        /* Avoid exception caused by division with zero */
        if (var1 != 0)
        {
            /* Check for overflows against UINT32_MAX/2; if pres is left-shifted by 1 */
            if (*comp_pres < 0x80000000)
            {
                *comp_pres = (*comp_pres << 1) / ((uint32_t) var1);
            }
            else
            {
                *comp_pres = (*comp_pres / (uint32_t) var1) * 2;
            }
            var1 = (((int32_t) dev->calib_param.dig_p9) * ((int32_t) (((*comp_pres / 8) * (*comp_pres / 8)) / 8192))) /
                   4086;
            var2 = (((int32_t) (*comp_pres / 4)) * ((int32_t) dev->calib_param.dig_p8)) / 8192;
            *comp_pres = (uint32_t) ((int32_t) *comp_pres + ((var1 + var2 + dev->calib_param.dig_p7) / 16));
            rslt = BMP280_OK;
        }
        else
        {
            *comp_pres = 0;
            rslt = BMP280_E_32BIT_COMP_PRESS;
        }
    }

    return rslt;
}

#ifndef BMP280_DISABLE_64BIT_COMPENSATION

/*!
 * @brief This API is used to get the compensated pressure from
 * uncompensated pressure. This API uses 64 bit integers.
 */
int8_t bmp280_get_comp_pres_64bit(uint32_t *pressure, int32_t uncomp_pres, const struct bmp280_dev *dev)
{
    int64_t var1, var2, p;
    int8_t rslt;

    rslt = null_ptr_check(dev);
    if (rslt == BMP280_OK)
    {
        var1 = ((int64_t) (dev->calib_param.t_fine)) - 128000;
        var2 = var1 * var1 * (int64_t) dev->calib_param.dig_p6;
        var2 = var2 + ((var1 * (int64_t) dev->calib_param.dig_p5) * 131072);
        var2 = var2 + (((int64_t) dev->calib_param.dig_p4) * 34359738368);
        var1 = ((var1 * var1 * (int64_t) dev->calib_param.dig_p3) / 256) +
               ((var1 * (int64_t) dev->calib_param.dig_p2) * 4096);
        var1 = ((INT64_C(0x800000000000) + var1) * ((int64_t) dev->calib_param.dig_p1)) / 8589934592;
        if (var1 != 0)
        {
            p = 1048576 - uncomp_pres;
            p = (((((p << 31)) - var2) * 3125) / var1);
            var1 = (((int64_t) dev->calib_param.dig_p9) * (p / 8192) * (p / 8192)) / 33554432;
            var2 = (((int64_t) dev->calib_param.dig_p8) * p) / 524288;
            p = ((p + var1 + var2) / 256) + (((int64_t)dev->calib_param.dig_p7) * 16);
            *pressure = (uint32_t)p;
            rslt = BMP280_OK;
        }
        else
        {
            *pressure = 0;
            rslt = BMP280_E_64BIT_COMP_PRESS;
        }
    }

    return rslt;
}

#endif /* BMP280_DISABLE_64BIT_COMPENSATION */

#ifndef BMP280_DISABLE_DOUBLE_COMPENSATION

/*!
 * @brief This API is used to get the compensated temperature from
 * uncompensated temperature. This API uses double floating precision.
 */
int8_t bmp280_get_comp_temp_double(double *temperature, int32_t uncomp_temp, struct bmp280_dev *dev)
{
    double var1, var2;
    int8_t rslt;

    rslt = null_ptr_check(dev);
    if (rslt == BMP280_OK)
    {
        var1 = (((double) uncomp_temp) / 16384.0 - ((double) dev->calib_param.dig_t1) / 1024.0) *
               ((double) dev->calib_param.dig_t2);
        var2 =
            ((((double) uncomp_temp) / 131072.0 - ((double) dev->calib_param.dig_t1) / 8192.0) *
             (((double) uncomp_temp) / 131072.0 - ((double) dev->calib_param.dig_t1) / 8192.0)) *
            ((double) dev->calib_param.dig_t3);
        dev->calib_param.t_fine = (int32_t) (var1 + var2);
        *temperature = ((var1 + var2) / 5120.0);
    }
    else
    {
        *temperature = 0;
        rslt = BMP280_E_DOUBLE_COMP_TEMP;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the compensated pressure from
 * uncompensated pressure. This API uses double floating precision.
 */
int8_t bmp280_get_comp_pres_double(double *pressure, int32_t uncomp_pres, const struct bmp280_dev *dev)
{
    double var1, var2;
    int8_t rslt;

    rslt = null_ptr_check(dev);
    if (rslt == BMP280_OK)
    {
        var1 = ((double) dev->calib_param.t_fine / 2.0) - 64000.0;
        var2 = var1 * var1 * ((double) dev->calib_param.dig_p6) / 32768.0;
        var2 = var2 + var1 * ((double) dev->calib_param.dig_p5) * 2.0;
        var2 = (var2 / 4.0) + (((double) dev->calib_param.dig_p4) * 65536.0);
        var1 =
            (((double) dev->calib_param.dig_p3) * var1 * var1 / 524288.0 + ((double) dev->calib_param.dig_p2) * var1) /
            524288.0;
        var1 = (1.0 + var1 / 32768.0) * ((double) dev->calib_param.dig_p1);
        uncomp_pres = (uint32_t)(1048576.0 - (double) uncomp_pres);
        if (var1 < 0 || var1 > 0)
        {
            uncomp_pres = (uint32_t)((uncomp_pres - (var2 / 4096.0)) * 6250.0 / var1);
            var1 = ((double) dev->calib_param.dig_p9) * uncomp_pres * uncomp_pres / 2147483648.0;
            var2 = uncomp_pres * ((double) dev->calib_param.dig_p8) / 32768.0;
            *pressure = (uncomp_pres + (var1 + var2 + ((double) dev->calib_param.dig_p7)) / 16.0);
        }
        else
        {
            *pressure = 0;
            rslt = BMP280_E_DOUBLE_COMP_PRESS;
        }
    }

    return rslt;
}

#endif /* BMP280_DISABLE_DOUBLE_COMPENSATION */

/*!
 * @brief This API computes the measurement time in milliseconds for the
 * active configuration
 */
uint8_t bmp280_compute_meas_time(const struct bmp280_dev *dev)
{
    uint32_t period = 0;
    uint32_t t_dur = 0, p_dur = 0, p_startup = 0;
    const uint32_t startup = 1000, period_per_osrs = 2000; /* Typical timings in us. Maximum is +15% each */
    int8_t rslt;

    rslt = null_ptr_check(dev);
    if (rslt == BMP280_OK)
    {
        t_dur = period_per_osrs * ((UINT32_C(1) << dev->conf.os_temp) >> 1);
        p_dur = period_per_osrs * ((UINT32_C(1) << dev->conf.os_pres) >> 1);
        p_startup = (dev->conf.os_pres) ? 500 : 0;

        /* Increment the value to next highest integer if greater than 0.5 */
        period = startup + t_dur + p_startup + p_dur + 500;
        period /= 1000; /* Convert to milliseconds */
    }

    return (uint8_t)period;
}

/*!
 * @brief This API performs self-test to check whether compensated temperature and pressure are within the range
 */
int8_t bmp280_selftest(struct bmp280_dev *dev)
{
    struct bmp280_config conf;
    struct bmp280_uncomp_data uncom;
    int32_t temperature = 0;
    uint32_t pressure = 0;
    int8_t rslt;

    /* reset sensor to reset all settings to defaults */
    rslt = bmp280_soft_reset(dev);
    if (rslt == BMP280_OK)
    {
        rslt = st_set_config(&conf, dev);
        dev->delay_ms(10);
        if (rslt == BMP280_OK)
        {
            /* read uncompensated results */
            rslt = bmp280_get_uncomp_data(&uncom, dev);
            if (rslt == BMP280_OK)
            {
                /* Get compensated temperature */
                rslt = bmp280_get_comp_temp_32bit(&temperature, uncom.uncomp_temp, dev);
                if (rslt == BMP280_OK)
                {
                    /* Get compensated pressure */
                    rslt = bmp280_get_comp_pres_32bit(&pressure, uncom.uncomp_press, dev);
                    if (rslt == BMP280_OK)
                    {
                        /* Checks compensated temperature and pressure are within the range */
                        rslt = st_check_sensor_range(temperature, pressure);
                    }
                }
            }
        }
    }

    return rslt;
}

/****************** Static Function Definitions *******************************/

/*!
 * @brief This internal API is used to check for null-pointers in the device
 * structure.
 */
static int8_t null_ptr_check(const struct bmp280_dev *dev)
{
    int8_t rslt;

    if ((dev == NULL) || (dev->read == NULL) || (dev->write == NULL) || (dev->delay_ms == NULL))
    {
        /* Null-pointer found */
        rslt = BMP280_E_NULL_PTR;
    }
    else
    {
        rslt = BMP280_OK;
    }

    return rslt;
}

/*!
 * @brief This internal API interleaves the register addresses and respective
 * register data for a burst write
 */
static void interleave_data(const uint8_t *reg_addr, uint8_t *temp_buff, const uint8_t *reg_data, uint8_t len)
{
    uint8_t index;

    for (index = 1; index < len; index++)
    {
        temp_buff[(index * 2) - 1] = reg_addr[index];
        temp_buff[index * 2] = reg_data[index];
    }
}

/*!
 * @brief This API is used to read the calibration parameters used
 * for calculating the compensated data.
 */
static int8_t get_calib_param(struct bmp280_dev *dev)
{
    int8_t rslt;
    uint8_t temp[BMP280_CALIB_DATA_SIZE] = { 0 };

    rslt = null_ptr_check(dev);
    if (rslt == BMP280_OK)
    {
        rslt = bmp280_get_regs(BMP280_DIG_T1_LSB_ADDR, temp, BMP280_CALIB_DATA_SIZE, dev);
        if (rslt == BMP280_OK)
        {
            dev->calib_param.dig_t1 =
                (uint16_t) (((uint16_t) temp[BMP280_DIG_T1_MSB_POS] << 8) | ((uint16_t) temp[BMP280_DIG_T1_LSB_POS]));
            dev->calib_param.dig_t2 =
                (int16_t) (((int16_t) temp[BMP280_DIG_T2_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_T2_LSB_POS]));
            dev->calib_param.dig_t3 =
                (int16_t) (((int16_t) temp[BMP280_DIG_T3_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_T3_LSB_POS]));
            dev->calib_param.dig_p1 =
                (uint16_t) (((uint16_t) temp[BMP280_DIG_P1_MSB_POS] << 8) | ((uint16_t) temp[BMP280_DIG_P1_LSB_POS]));
            dev->calib_param.dig_p2 =
                (int16_t) (((int16_t) temp[BMP280_DIG_P2_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P2_LSB_POS]));
            dev->calib_param.dig_p3 =
                (int16_t) (((int16_t) temp[BMP280_DIG_P3_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P3_LSB_POS]));
            dev->calib_param.dig_p4 =
                (int16_t) (((int16_t) temp[BMP280_DIG_P4_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P4_LSB_POS]));
            dev->calib_param.dig_p5 =
                (int16_t) (((int16_t) temp[BMP280_DIG_P5_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P5_LSB_POS]));
            dev->calib_param.dig_p6 =
                (int16_t) (((int16_t) temp[BMP280_DIG_P6_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P6_LSB_POS]));
            dev->calib_param.dig_p7 =
                (int16_t) (((int16_t) temp[BMP280_DIG_P7_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P7_LSB_POS]));
            dev->calib_param.dig_p8 =
                (int16_t) (((int16_t) temp[BMP280_DIG_P8_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P8_LSB_POS]));
            dev->calib_param.dig_p9 =
                (int16_t) (((int16_t) temp[BMP280_DIG_P9_MSB_POS] << 8) | ((int16_t) temp[BMP280_DIG_P9_LSB_POS]));
        }
    }

    return rslt;
}

/*!
 * @brief This internal API to reset the sensor, restore/set conf, restore/set mode
 */
static int8_t conf_sensor(uint8_t mode, const struct bmp280_config *conf, struct bmp280_dev *dev)
{
    int8_t rslt;
    uint8_t temp[2] = { 0, 0 };
    uint8_t reg_addr[2] = { BMP280_CTRL_MEAS_ADDR, BMP280_CONFIG_ADDR };

    rslt = null_ptr_check(dev);
    if ((rslt == BMP280_OK) && (conf != NULL))
    {
        rslt = bmp280_get_regs(BMP280_CTRL_MEAS_ADDR, temp, 2, dev);
        if (rslt == BMP280_OK)
        {
            /* Here the intention is to put the device to sleep
             * within the shortest period of time
             */
            rslt = bmp280_soft_reset(dev);
            if (rslt == BMP280_OK)
            {
                temp[0] = BMP280_SET_BITS(temp[0], BMP280_OS_TEMP, conf->os_temp);
                temp[0] = BMP280_SET_BITS(temp[0], BMP280_OS_PRES, conf->os_pres);
                temp[1] = BMP280_SET_BITS(temp[1], BMP280_STANDBY_DURN, conf->odr);
                temp[1] = BMP280_SET_BITS(temp[1], BMP280_FILTER, conf->filter);
                temp[1] = BMP280_SET_BITS_POS_0(temp[1], BMP280_SPI3_ENABLE, conf->spi3w_en);
                rslt = bmp280_set_regs(reg_addr, temp, 2, dev);
                if (rslt == BMP280_OK)
                {
                    dev->conf = *conf;
                    if (mode != BMP280_SLEEP_MODE)
                    {
                        /* Write only the power mode register in a separate write */
                        temp[0] = BMP280_SET_BITS_POS_0(temp[0], BMP280_POWER_MODE, mode);
                        rslt = bmp280_set_regs(reg_addr, temp, 1, dev);
                    }
                }
            }
        }
    }
    else
    {
        rslt = BMP280_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This internal API checks whether the calibration parameters are within the given range
 */
static int8_t st_check_cal_param(const struct bmp280_dev *dev)
{
    int8_t rslt = 0;

    /* check if calibration parameters are in permitted range */
    if ((dev->calib_param.dig_t1 < BMP280_ST_DIG_T1_MIN) || (dev->calib_param.dig_t1 > BMP280_ST_DIG_T1_MAX))
    {
        rslt = BMP280_E_CAL_PARAM_RANGE;
    }
    if ((dev->calib_param.dig_t2 < BMP280_ST_DIG_T2_MIN) || (dev->calib_param.dig_t2 > BMP280_ST_DIG_T2_MAX))
    {
        rslt = BMP280_E_CAL_PARAM_RANGE;
    }
    if ((dev->calib_param.dig_t3 < BMP280_ST_DIG_T3_MIN) || (dev->calib_param.dig_t3 > BMP280_ST_DIG_T3_MAX))
    {
        rslt = BMP280_E_CAL_PARAM_RANGE;
    }
    if ((dev->calib_param.dig_p1 < BMP280_ST_DIG_P1_MIN) || (dev->calib_param.dig_p1 > BMP280_ST_DIG_P1_MAX))
    {
        rslt = BMP280_E_CAL_PARAM_RANGE;
    }
    if ((dev->calib_param.dig_p2 < BMP280_ST_DIG_P2_MIN) || (dev->calib_param.dig_p2 > BMP280_ST_DIG_P2_MAX))
    {
        rslt = BMP280_E_CAL_PARAM_RANGE;
    }
    if ((dev->calib_param.dig_p3 < BMP280_ST_DIG_P3_MIN) || (dev->calib_param.dig_p3 > BMP280_ST_DIG_P3_MAX))
    {
        rslt = BMP280_E_CAL_PARAM_RANGE;
    }
    if ((dev->calib_param.dig_p4 < BMP280_ST_DIG_P4_MIN) || (dev->calib_param.dig_p4 > BMP280_ST_DIG_P4_MAX))
    {
        rslt = BMP280_E_CAL_PARAM_RANGE;
    }
    if ((dev->calib_param.dig_p5 < BMP280_ST_DIG_P5_MIN) || (dev->calib_param.dig_p5 > BMP280_ST_DIG_P5_MAX))
    {
        rslt = BMP280_E_CAL_PARAM_RANGE;
    }
    if ((dev->calib_param.dig_p6 < BMP280_ST_DIG_P6_MIN) || (dev->calib_param.dig_p6 > BMP280_ST_DIG_P6_MAX))
    {
        rslt = BMP280_E_CAL_PARAM_RANGE;
    }
    if ((dev->calib_param.dig_p8 < BMP280_ST_DIG_P8_MIN) || (dev->calib_param.dig_p8 > BMP280_ST_DIG_P8_MAX))
    {
        rslt = BMP280_E_CAL_PARAM_RANGE;
    }
    if ((dev->calib_param.dig_p9 < BMP280_ST_DIG_P9_MIN) || (dev->calib_param.dig_p9 > BMP280_ST_DIG_P9_MAX))
    {
        rslt = BMP280_E_CAL_PARAM_RANGE;
    }

    return rslt;
}

/*!
 * @This internal API checks whether the uncompensated temperature and pressure are within the range
 */
static int8_t st_check_boundaries(int32_t utemperature, int32_t upressure)
{
    int8_t rslt = 0;

    /* check UT and UP for valid range */
    if ((utemperature <= BMP280_ST_ADC_T_MIN || utemperature >= BMP280_ST_ADC_T_MAX) &&
        (upressure <= BMP280_ST_ADC_P_MIN || upressure >= BMP280_ST_ADC_P_MAX))
    {
        rslt = BMP280_E_UNCOMP_TEMP_AND_PRESS_RANGE;
    }
    else if (utemperature <= BMP280_ST_ADC_T_MIN || utemperature >= BMP280_ST_ADC_T_MAX)
    {
        rslt = BMP280_E_UNCOMP_TEMP_RANGE;
    }
    else if (upressure <= BMP280_ST_ADC_P_MIN || upressure >= BMP280_ST_ADC_P_MAX)
    {
        rslt = BMP280_E_UNCOMP_PRES_RANGE;
    }
    else
    {
        rslt = BMP280_OK;
    }

    return rslt;
}

/*!
 * @brief This internal API checks whether the compensated temperature and pressure are within the given range
 */
static int8_t st_check_sensor_range(int32_t temperature, uint32_t pressure)
{
    int8_t rslt = -1;

    /* check for temperature measurement plausibility */
    if ((temperature < BMP280_ST_PLAUSIBLE_TEMP_MIN) ||
        (temperature > (BMP280_ST_PLAUSIBLE_TEMP_MAX * BMP280_ST_TEMPERATURE_RESOLUTION_INT32)))
    {
        /* implausible temperature */
        rslt = BMP280_E_IMPLAUS_TEMP;
    }
    if (rslt != BMP280_E_IMPLAUS_TEMP)
    {
        /* check for pressure measurement plausibility */
        if ((pressure < (BMP280_ST_PLAUSIBLE_PRESS_MIN * BMP280_ST_PRESSURE_RESOLUTION_INT32)) ||
            (pressure > (BMP280_ST_PLAUSIBLE_PRESS_MAX * BMP280_ST_PRESSURE_RESOLUTION_INT32)))
        {
            /* implausible pressure */
            rslt = BMP280_E_IMPLAUS_PRESS;
        }
        else
        {
            rslt = BMP280_OK;
        }
    }

    return rslt;
}

/*!
 * @brief This internal API configures the device for self test
 */
static int8_t st_set_config(struct bmp280_config *conf, struct bmp280_dev *dev)
{
    int8_t rslt;

    /* check validity of calibration parameters against known bounds, skip check */
    /* if bounds are newer than this code (indicated by api_revision number) */
    rslt = st_check_cal_param(dev);
    if (rslt == BMP280_OK)
    {
        /* communication and trimming are OK, proceed to measurement */
        conf->os_temp = BMP280_OS_1X;
        conf->os_pres = BMP280_OS_1X;
        rslt = bmp280_set_config(conf, dev);
        if (rslt == BMP280_OK)
        {
            /* set forced mode, wait for completion, read results */
            rslt = bmp280_set_power_mode(BMP280_FORCED_MODE, dev);
        }
    }

    return rslt;
}

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
   bmp.delay_ms = delay_ms;

   /* Assign device I2C address based on the status of SDO pin (GND for PRIMARY(0x76) & VDD for SECONDARY(0x77)) */
   //bmp.dev_id = BMP280_I2C_ADDR_PRIM;
   bmp.dev_id = 0x76;
   /* Select the interface mode as I2C */
   bmp.intf = BMP280_I2C_INTF;

   /* Map the I2C read & write function pointer with the functions responsible for I2C bus transfer */
   bmp.read = i2c_reg_read;
   bmp.write = i2c_reg_write;

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
   i2c_bus_global->i2c_bus::write(i2c_addr).write(mydata, length + 1);
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
   //i2c_bus_global->i2c_bus::read(i2c_addr).read(reg_data, length);
   
   //i2c_bus_global->i2c_bus::readbmp280(i2c_addr).read(reg_data, length);
   uint8_t a_value = 0;
   /*uint8_t r_value = *///i2c_bus_global->i2c_bus::readbmp280(i2c_addr, reg_addr).read(reg_data, length / sizeof( uint8_t ) );
   i2c_bus_global->i2c_bus::readbmp280(i2c_addr, reg_addr).read_from_register(reg_addr, reg_data, length);
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