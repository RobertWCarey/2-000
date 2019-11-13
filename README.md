# Cell Stretcher Firmware
## Summary
This is the source code for the first revision of the cell stretcher control box hardware. The code runs on the Arduino Nano board which uses the ATMEL ATmega328P mcu with the "new" bootloader. Please note that to flash any firmware the Reset button will need to be pressed as there is a 10uF capacitor between the RST and GND pin to prevent the USB interface from resetting the device.
## Development Environment
To develop the code the following environments were used:

Microsoft Visual Studio Code, version: 1.39.2

PlatformIO IDE for VSCode, version: 1.9.3
## Control Box Wiring
![Rev1Wiring](https://user-images.githubusercontent.com/38249951/68767511-e4092900-0674-11ea-8e95-ac4f27fcd33e.PNG)
