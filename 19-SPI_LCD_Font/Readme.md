<h1> Example project for displaying font on serial LCD (Nokia 5110) </h1>
<b> This project is created for STM32F103C8T6 microcontroller (Blue Pill board) </b>

In this project, SPI1 of the microcontroller is activated and then is used for transmitting data, that is sending arrays for letters to display words on a Nokia 5110 serial LCD. Some additional header files for fonts and utility functions are used in this project.

| Pin map     | Function    |
| ----------- | ----------- |
| PortA.pin0  | LCD_CE    |
| PortA.pin3  | LCD_Reset |
| PortA.pin4  | LCD_DC    |
| PortA.pin5  | SPI1_SCK  |
| PortA.pin7  | SPI1_MOSI |



