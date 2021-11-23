<h1> Example project for converting analog values via ADC unit and stroing them using DMA </h1>
<b>This project is created for STM32F103C8T6 microcontroller (Blue Pill board)</b>

in this project, ADC1 of the microcontroller is activated and is configured in Continuous Coversion Mode receive and convert data on pins in0 and in1. In this project DMA (Direct Memory Access) unit is used to manage the data transmission. 

| Pin map     | Function    |
| ----------- | ----------- |
| PortA.pin0  | ADC1.in0    |
| PortA.pin1  | ADC1.in1    |
| PortA.pin9  | USART1.TX   |
| PortA.pin10 | USART1.RX   |




