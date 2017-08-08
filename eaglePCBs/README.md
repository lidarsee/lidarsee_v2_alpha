# lidarsee_v2_hardware

Motherboard_STM32F405RG_r1 ist the mainboard for the head unit. 
### Features
* XV-11 LIDAR compartible
* STM32F405 Microprocessor
* ESP32 WIFI/Bluetooth-Module
* Single cell lipo power supply
* MPU6050 IMU
* We used a single cell 2500mAh LiPo battery

### To Do
* Rework of the LiPo Boost converter (larger inductor? )
* IMU not tested, it is recommend to use an external IMU connected to the I2C port. Soldering the IMU by hand is quite difficult and there are a lot of cheap relaiable breakout boards available. The BNO055 sensor for example also delivers easy to use euler angles. 
* using beter handsolderable parts 


## Wiring
![assemblyMainbaord](../docs/images/MotherboardImage.jpg)