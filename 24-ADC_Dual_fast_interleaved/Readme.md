<h1> Example project for using 2 ADCs in dual-fast-interleaved mode in order to achive higher sampling speed</h1>
<b> This project is created for STM32F103C8T6 microcontroller (Blue Pill board) </b>

In this project, ADC1 and ADC2 of the microcontroller are activated and then by configuring them in Dual fast interleaved mode, we can achive higher sampling speed (up to 2Ms/s)

| Pin map     | Function    |
| ----------- | ----------- |
| PortA.pin0   | ADC1_IN0 / ADC2_IN0   |
| PortA.pin8   | Tim1_CH1    |
| PortB.pin10  | USART3_TX    |
| PortB.pin11  | USART3_RX    |





