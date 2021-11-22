# Gyrowalker
A embbed project to measure distance with gyroscope
## Functions
calculate distance with and only with gyroscope

LCD display(todo) to show distance

a start/stop button(todo)

LED indicates movement(todo)

## Hardware & Framework
Board: STM32F429I

Gyroscope: I3G4250D

Framework: Mbed

## Conigurations
| field      | value |
| ----------- | ----------- |
| scale   | 500      |
|output data rate | 800Hz|
|cutoff frequency | 30Hz|


## Steps
0.press user button to start(todo), and LED flash when recording

1.set up SPI device

2.configure gyrometer control registers and calibrate(128 samples)

3.read from gyrometer every t seconds

4.convert the raw data linear velocity

5.display on LCD

6.collect 20 seconds of velocity data, sampled at 0.5 second intervals

7.calculate distance with timer

