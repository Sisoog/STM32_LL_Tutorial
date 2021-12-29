<h1> Example project for using LSM6DS3 accelerometer and gyroscope module through SPI unit </h1>
<b> This project is created for STM32F103RET6 microcontroller </b>


In this project, SPI1 of the microcontroller is activated and then is used for configuring LSM6DS3 module and receiving data from it. received data is then transmitted through USART1 unit. 


| Pin map     | Function    |
| ----------- | ----------- |
| PortA.pin5  | SPI1_SCK  |
| PortA.pin6  | SPI1_MISO |
| PortA.pin7  | SPI1_MOSI |
| PortA.pin9  | USART1_TX |
| PortA.pin10 | USART1_RX |
| PortC.pin0  | LSM6DS3_CS |



