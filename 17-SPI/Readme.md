<h1> Example project for transmitting data using SPI unit </h1>
<b>This project is created for STM32F103C8T6 microcontroller (Blue Pill board)</b>

in this project, SPI1 of the microcontroller is activated and then is used for transmitting  data. In addition an 8-bit shift register (74HC595 IC) is used to receive the data and display it on 8 LED's.


| Pin map     | Function    |
| ----------- | ----------- |
| PortA.pin4  | RCK (in 74HC595) |
| PortA.pin5  | SPI1_SCK         |
| PortA.pin7  | SPI1_MOSI        |




