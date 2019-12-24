# simon
Simon game

A simon game Implementation in C language, using STM32F103 microcontroller family from Microchip.
Version 1.0 has the basic features only, but allows the 4 difficulty levels to be played.
The difficulty level should be set up in main function during the simon game object creation.

This implementation makes use of the folowing open source libraries:
- FreeRTOS
- libopencm3
- CGG

And also the folowing design patterns:
- Dinamic memory allocation
- Finite state machine using linked lists
- Organization of code around objects

If you got interested on the above topics, this is a great project to study.

Please also note that to compile and run this code you will need to download the code from the book Beggining STM32, Developing with FreeRTOS, libopencm3 and GCC.
The code can be found here: https://github.com/ve3wwg/stm32f103c8t6

I suggest reading the folwoing chapters of the book prior to start working on this project:
- Chapter 1 - Introduction
- Chapter 2 - Software installation
- Chapter 3 - Power up and blink

Those chapters explain step by step how to install and test the opensource toolkit required.

You can buy an e-book here: 
https://www.amazon.com/Beginning-STM32-Developing-FreeRTOS-libopencm3-ebook/dp/B07DGGHZN6/ref=sr_1_fkmr0_1?keywords=Beggining+STM32%2C+Developing+with+FreeRTOS%2C+libopencm3+and+GCC&qid=1577222240&sr=8-1-fkmr0

For any questions or suggestions, please reach out to camargo.advanced@gmail.com

See you around,
Marcelo Camargo.
