/*
* Project:    Challenge - Gyrowalker
* File:       main.cpp
* Author:     Haoran Wang
*/

#include <mbed.h>

#define OUT_X_L (0x28)
#define OUT_X_H (0x29)
#define OUT_Y_L (0x2A)
#define OUT_Y_H (0x2B)
#define OUT_Z_L (0x2C)
#define OUT_Z_H (0x2D)
#define CTRL_REG_1 (0x20)
#define STATUS_REG (0x27)

SPI gyroscope(PF_9, PF_8, PF_7); // mosi, miso, sclk
DigitalOut cs(PC_1);

char buffer[6]; // raw daat from gyro
int16_t data[3]; // combined result from gyro

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
  writeByte(CTRL_REG_1, 0b00001000);// write CTRL_REG_1, power on
}

void getGyroValue(){
  cs = 0;
  gyroscope.write(OUT_X_L | 0x80);
  buffer[0] = gyroscope.write(0xff);
  cs = 1;

  cs = 0;
  gyroscope.write(OUT_X_H | 0x80);
  buffer[1] = gyroscope.write(0xff);
  cs = 1;

  cs = 0;
  gyroscope.write(OUT_Y_L | 0x80);
  buffer[2] = gyroscope.write(0xff);
  cs = 1;

  cs = 0;
  gyroscope.write(OUT_Y_H | 0x80);
  buffer[3] = gyroscope.write(0xff);
  cs = 1;

  cs = 0;
  gyroscope.write(OUT_Z_L | 0x80);
  buffer[4] = gyroscope.write(0xff);
  cs = 1;

  cs = 0;
  gyroscope.write(OUT_Z_H | 0x80);
  buffer[5] = gyroscope.write(0xff);
  cs = 1;

  for(int i = 0; i < 3; i++){
    data[i] = ((buffer[2 * i + 1] << 8) | buffer[2 * i]);
  }
}



int main()
{
  initiateGyro();
  while (1)
  {
    getGyroValue();
    for(int i = 0; i < 3; i++){
      printf("%d ", data[i]);
    }

    wait_us(100000);
  }
}

