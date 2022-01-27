<h1> Example project for using SD Card </h1>
<b> This project is created for STM32F103C8T6 microcontroller (Blue Pill board) </b>

In this project, a SD memory card is connected to the microcontroller and then ff (ELM by ChaN) library is used 
to initialize the memory and to create a new txt file in the memory. then write and read functions are tested.

| Pin map     | Function    |
| ----------- | ----------- |
| PortA.pin5  | SPI1_SCK  |
| PortA.pin6  | SPI1_MISO |
| PortA.pin7  | SPI1_MOSI |
| PortA.pin9   | USART1_TX    |
| PortA.pin10  | USART1_RX    |
| PortB.0      | Chip enable pin |




