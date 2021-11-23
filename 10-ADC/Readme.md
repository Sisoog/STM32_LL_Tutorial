<h1> Example project for using Analog to Digital unit </h1>
<b>This project is created for STM32F103C8T6 microcontroller (Blue Pill board)</b>

in this project, we activate ADC1 of the microcontroller and configure it for Continuous Conversion Mode. the analog voltage exerted on input of the ADC is then converted, and after calibration is displayed on serial terminal(via USART1).


| Pin map     | function    |
| ----------- | ----------- |
| PortA.pin0  | ADC input   |
| PortA.pin9  | USART1.TX   |
| PortA.pin10 | USART1.RX   |    


the article and complete guidance for the code in this project can be found in sisoog.com: [link](https://sisoog.com/2021/02/%d8%a2%d9%85%d9%88%d8%b2%d8%b4-stm32-%d8%a8%d8%a7-%d8%aa%d9%88%d8%a7%d8%a8%d8%b9-ll-%d9%82%d8%b3%d9%85%d8%aa-%d8%af%d9%87%d9%85-%d9%85%d8%a8%d8%af%d9%84-%d8%a2%d9%86%d8%a7%d9%84%d9%88%da%af-%d8%a8/)

