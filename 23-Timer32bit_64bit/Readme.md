<h1> Example project for making 32bit and 64bit timer by connecting 16bit timers </h1>
<b> This project is created for STM32F103C8T6 microcontroller (Blue Pill board) </b>

In this project, timer 1 to 4 of the microcontroller are activated and then by connecting clock of each timer to be trigered by another timer, larger timers are created.then as an example applicaton, frequency of an arbitrary signal is measured with this larger timer that we have created.

| Pin map     | Function    |
| ----------- | ----------- |
| PortA.pin9   | USART_TX    |
| PortA.pin10  | USART_RX    |
| PortA.pin12  | Tim1_ETR    |
| PortB.pin6   | Tim4_CH1    |




