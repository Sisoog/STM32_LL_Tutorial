<h1> Example project for transmitting data via USART unit using DMA </h1>
<b>This project is created for STM32F103C8T6 microcontroller (Blue Pill board)</b>

in this project, USART1 of the microcontroller is activated and an arbitrary message ("This is a test message") is transmitted throuth USART1.TX. this data can be received by a TTL to USB converter and we can see it on computer by using a Serial Terminal (such Real Term). DMA (Direct Memory Access) unit is used to manage the data transmission.

| Pin map     | Function    |
| ----------- | ----------- |
| PortA.pin9  | USART1.TX   |



