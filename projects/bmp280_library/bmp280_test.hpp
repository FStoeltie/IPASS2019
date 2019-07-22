#include "bmp280.hpp"
#include "hwlib.hpp"
/**
    Class to test all bmp280 functionality
*/
static constexpr uint8_t address01 = 0x76; 
class bmp280_test   {
public:
    
    void test01(bmp280& bmp01) {
        hwlib::cout << "====== *Configuration Test* =====\n" << hwlib::flush;
        bmp01.configure();  
        if(bmp01.getErrors() == 0)  {
            hwlib::cout << "No errors during configuring of bmp on address:\t0x" << hwlib::hex << address01 << "\n" << hwlib::flush;
            hwlib::cout << "DeviceID of bmp01:0x" << hwlib::hex << bmp01.getDeviceId() << "\n" << hwlib::flush;
        }
        else    {
            hwlib::cout << "Error occured!\n" << hwlib::flush;
            hwlib::cout << "Error: " << hwlib::hex << bmp01.getErrors() << "\n" << hwlib::flush;
        }
    }

    void test02(bmp280& bmp01)   {
        hwlib::cout << "====== *Different modes Test* =====\n" << hwlib::flush;
        hwlib::cout << "Temperature before sleep: " << hwlib::dec << bmp01.getTemperature() << "\n" << hwlib::flush;
        bmp01.setMode(MODE::SLEEP);
        hwlib::cout << "Temperature during sleep: " << hwlib::dec << bmp01.getTemperature() << "\n" << hwlib::flush;
        hwlib::wait_ms(5000);
        hwlib::cout << "Temperature during sleep 5000ms: " << hwlib::dec << bmp01.getTemperature() << "\n" << hwlib::flush;
        bmp01.setMode(MODE::NORMAL);
        hwlib::cout << "Temperature after sleep: " << hwlib::dec << bmp01.getTemperature() << "\n" << hwlib::flush;

        bmp01.setMode(MODE::FORCED);
        hwlib::cout << "Temperature forced mode 1: " << hwlib::dec << bmp01.getTemperature() << "\n" << hwlib::flush;
        hwlib::wait_ms(3000);
        hwlib::cout << "Temperature forced mode 2: " << hwlib::dec << bmp01.getTemperature() << "\n" << hwlib::flush;
        hwlib::wait_ms(3000);
        hwlib::cout << "Temperature forced mode 3: " << hwlib::dec << bmp01.getTemperature() << "\n" << hwlib::flush;
        hwlib::cout << "Error: " << hwlib::hex << bmp01.getErrors() << "\n" << hwlib::flush;
    }

    void test03(bmp280& bmp01)  {
        bmp01.setMode(MODE::NORMAL);
        hwlib::cout << "====== *Different standby times* =====\n" << hwlib::flush;
        bmp01.setStandbyTime(STANDBY_TIME::US_500);
        for(int i = 0; i < 10; i++)  {
            hwlib::cout << "Standby time 400us: " << hwlib::dec << i << ", " << bmp01.getTemperature() << "\n" << hwlib::flush;  
            hwlib::wait_ms(100);  
        }
        bmp01.setStandbyTime(STANDBY_TIME::S_4);
        for(int i = 0; i < 10; i++)  {
            hwlib::cout << "Standby time 500ms: " << hwlib::dec << i << ", " << bmp01.getTemperature() << "\n" << hwlib::flush;  
            hwlib::wait_ms(500);  
        }
/*      hwlib::cout << "Standby time 100ms: " << hwlib::dec << bmp01.getTemperature() << "\n" << hwlib::flush;
        hwlib::cout << "Temperature during sleep: " << hwlib::dec << bmp01.getTemperature() << "\n" << hwlib::flush;
        hwlib::wait_ms(5000);
        hwlib::cout << "Temperature during sleep 5000ms: " << hwlib::dec << bmp01.getTemperature() << "\n" << hwlib::flush;
        hwlib::cout << "Temperature after sleep: " << hwlib::dec << bmp01.getTemperature() << "\n" << hwlib::flush;*/
    }
};