# IPASS 2019 Herkansing
## About
This project is a library that provides functionality that makes it easy to use a bmp280 with bmpwk and hwlib.<br/>
All three modes(SLEEP, FORCED, NORMAL) work. IIR, standby (also for bme280) and oversampling works.<br/>
A bme280 can also be used.However, humidity does not work on a bme280.<br/>
## Requirements
### Software requirements
Bmptk and hwlib.
### Hardware requirements
BME280 or BMP280.

## How to use
The library is fairly easy to use.<br/>
```
   // hwlib functionality
   auto scl           = hwlib::target::pin_oc{ hwlib::target::pins::scl };
   auto sda           = hwlib::target::pin_oc{ hwlib::target::pins::sda };
   auto i2c_bus       = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
   
   // bmp280 functionality
   bmp280 bmpInstance(i2c_bus, 0x76);
   bmpInstance.configure()       // configures bmp280 settings. Required before calling any other methods.
   bmpInstance.getTemperature(); // get temperature as degrees celcius float.
   bmpInstance.getPressure();    // get pressure in Pa.
   bmpInstance.getAltitude();    // get altitude in meters.
   bmpInstance.setMode(MODE);    // set bmp mode.
   bmpInstance.setIIR(IIR_RES);  // set iir resolution for pressure.
   bmpInstance.setOversampling(OVERSAMPLING);   // set oversampling of measurements.
   bmpInstance.setStandbyTime(STANDBY_TIME);    // set standby time in normal mode.
   bmpInstance.getErrors();      // Get possible errors.
   ```
## Changes made
## Result
## Resources
## Future
