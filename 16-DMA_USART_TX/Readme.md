#This project is created for STM32F103C8T6 microcontroller (Blue Pill board)

in this project, USART1 of the microcontroller is activated and an arbitrary message ("This is a test message") is transmitted throuth USART1.TX. this data can be received by a TTL to USB converter and we can see it on computer by using a Serial Terminal (such Real Term). DMA (Direct Memory Access) unit is used to manage the data transmission.

Pin map:
PortA.pin9 is used for USART1.TX


