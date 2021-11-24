<h1> Example project for displaying an animation on serial LCD (Nokia 5110) </h1>
<b> This project is created for STM32F103C8T6 microcontroller (Blue Pill board) </b>

In this project, SPI1 of the microcontroller is activated and then by configuring DMA unit, SPI is used for transmitting frames of an animation to be displayed on a Nokia 5110 serial LCD. Some additional header files for fonts, utility functions, and frames of the animations, are used in this project.

| Pin map     | Function    |
| ----------- | ----------- |
| PortA.pin0  | LCD_CE    |
| PortA.pin3  | LCD_Reset |
| PortA.pin4  | LCD_DC    |
| PortA.pin5  | SPI1_SCK  |
| PortA.pin7  | SPI1_MOSI |



