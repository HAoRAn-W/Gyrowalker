/*
* Project:    Challenge - Gyrowalker
* File:       main.cpp
* Author:     Haoran Wang
*/

#include <mbed.h>
#include <stm32f429xx.h>
#include "gyro.h"


SPI gyroscope(PF_9, PF_8, PF_7); // mosi, miso, sclk
DigitalOut cs(PC_1);

char buffer[6]; // raw daat from gyro
int16_t data[3]; // combined result from gyro
int16_t x_data;
int16_t y_data;
int16_t z_data;

float resolution = 1.0f;

// write data to registers in gyrometer
void writeByte(uint8_t address, uint8_t data)
{
  cs = 0;
  gyroscope.write(address);
  gyroscope.write(data);
  cs = 1;
}

void initiateGyro(){
  // set up gyroscope
  gyroscope.format(8, 3); // 8 bits per SPI frame; polarity 1, phase 0, clock frequency deafult 1 MHz
  // gyroscope.frequency(100);
  writeByte(CTRL_REG_1, 0b00001111);// write CTRL_REG_1, power on
  writeByte(CTRL_REG_2, 0b00000000);
  writeByte(CTRL_REG_3, 0b00000000);
  writeByte(CTRL_REG_4, 0b00000000);
  writeByte(CTRL_REG_5, 0b00000000);
}

void getGyroValue(){
  cs = 0;
  gyroscope.write(OUT_X_L | 0x80 | 0x40);
  // for(int i = 0; i < 5; i++){
  //   buffer[i] = gyroscope.write(0xff);
  // }
  x_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);
  y_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);
  z_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);
  cs = 1;


  // data[0] = buffer[1] << 8 | buffer[0];
  // data[1] = buffer[3] << 8 | buffer[2];
  // data[2] = buffer[5] << 8 | buffer[4];
}



int main()
{
  initiateGyro();
  while (1)
  {
    getGyroValue();
    printf("x=%d\r\n", x_data);
    printf("y=%d\r\n", y_data);
    printf("z=%d\r\n", z_data);

    //wait_us(100000);
  }
}

