#This project is created for STM32F103C8T6 microcontroller(Blue Pill board)

in this project, SPI1 of the microcontroller is activated and then is used for transmitting  data. In addition an 8-bit shift register (74HC595 IC) is used to receive the data and display it on 8 LED's.

Pin map:
PortA.pin4 is used for RCK (in 74HC595)
PortA.pin5 is used for SPI1_SCK
PortA.pin7 is used for SPI1_MOSI


