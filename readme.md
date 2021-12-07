# Gyrowalker
A embbed project to measure distance with gyroscope
## Functions
calculate distance with and only with gyroscope

a start/erase button

LED indication when calibrating gyroscope and resetting storage

## Hardware & Framework
Board: STM32F429I

Gyroscope: I3G4250D

Framework: Mbed

Editor: VS Code + PlatformIO

## Conigurations
| field      | value |
| ----------- | ----------- |
| scale   | 500      |
|output data rate | 800Hz|
|cutoff frequency | 30Hz|


## Steps
I. Connect board with PC and see last distance record from terminal

II. Double click user button to start recording (20s)

III. If storage is coruptted, long press user button to erase block storage

## Process of gyroscope
1.set up SPI device

2.configure gyroscope control registers and calibrate(128 samples)

3.read from gyroscope every 0.5 seconds for 20 seconds

4.convert the raw data to dps than rad/s than linear velocity

5.calculate the distance

6.write record to file

