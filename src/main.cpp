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
  writeByte(CTRL_REG_1, 0b00001000);// write CTRL_REG_1, power on
}

void getGyroValue(){
  cs = 0;
  gyroscope.write(OUT_X_L | 0x80 | 0x40);
  for(int i = 0; i < 5; i++){
    buffer[i] = gyroscope.write(0xff);
  }
  cs = 1;


  data[0] = buffer[1] << 8 | buffer[0];
  data[1] = buffer[3] << 8 | buffer[2];
  data[2] = buffer[5] << 8 | buffer[4];
}



int main()
{
  initiateGyro();
  while (1)
  {
    getGyroValue();
    printf("x=%d\r\n", data[0]);
    printf("y=%d\r\n", data[1]);
    printf("z=%d\r\n", data[2]);

    wait_us(100000);
  }
}

