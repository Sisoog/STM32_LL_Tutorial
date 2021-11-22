#This project is created for STM32F103C8T6 microcontroller(Blue Pill board)

in this project pin1 of the portA of the Micro is connected to a push button. for every 5 times the button is pressed down, the LED that is connected to pin13 of the portC is toggled.
counting the number of the presses on the button, is done by an external interrupt.

the article and complete guidance for the code in this project can be found in sisoog.com :

https://sisoog.com/2020/11/%d8%a2%d9%85%d9%88%d8%b2%d8%b4-stm32-%d8%a8%d8%a7-%d8%aa%d9%88%d8%a7%d8%a8%d8%b9-ll-%d9%82%d8%b3%d9%85%d8%aa-%d9%87%d9%81%d8%aa%d9%85-interrupt/