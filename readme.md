# Gyrowalker
A embbed project to measure distance with gyroscope
## Functions
walk in straight line

support different speed

calculate distance

LCD display(optional) to show distance

a start/stop button

LED indicates movement

## Hardware & Framework
Board: STM32F429I

Gyroscope: I3G4250D

Framework: Mbed

## Conigurations
| field      | value |
| ----------- | ----------- |
| polarity      | 1       |
| phase   | 1        |
| scale   | 425 TODO      |
| frequency   | 1MHz TODO       |
|cutoff | TODO|


## Steps
0.button control start(todo)

1.set up SPI device

2.configure gyrometer control registers

3.read from gyrometer every t seconds

4.convert the raw data to readble value by resolution

5.display on LCD

6.collect 20 seconds of velocity data, sampled at 0.5 second intervals

7.calculate distance with timer

