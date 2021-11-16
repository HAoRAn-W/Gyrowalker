/*
* Project:    Challenge - Gyrowalker
* File:       main.cpp
* Author:     Haoran Wang
*/

#include <mbed.h>

#define WHO_AM_I 0x0F

#define CTRL_REG_1 0x20
#define CTRL_REG_2 0x21
#define CTRL_REG_3 0x22
#define CTRL_REG_4 0x23
#define CTRL_REG_5 0x24

#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

SPI gyroscope(PF_9, PF_8, PF_7); // mosi, miso, sclk
DigitalOut cs(PC_1);

int16_t x_data;
int16_t y_data;
int16_t z_data;

// write data to registers in gyrometer
void writeByte(uint8_t address, uint8_t data)
{
  cs = 0;
  wait_us(1); // wait some cs setup time
  gyroscope.write(address);
  gyroscope.write(data);
  cs = 1;
}

void initiateGyro()
{
  // set up gyroscope
  gyroscope.format(8, 3); // 8 bits per SPI frame; polarity 1, phase 0
  gyroscope.frequency(1000000); // clock frequency deafult 1 MHz max:10MHz

  writeByte(CTRL_REG_1, 0xff);
  writeByte(CTRL_REG_2, 0x04);
  writeByte(CTRL_REG_4, 0x30);
}

void getGyroValue()
{
  cs = 0;
  wait_us(1);
  gyroscope.write(OUT_X_L | 0x80 | 0x40);
  x_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);
  y_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);
  z_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);

  cs = 1;
}

int main()
{
  // initiateGyro();
  // cs = 1;
  // printf("data=[");
  // while (1)
  // {
  //   // getGyroValue();
  //   cs = 0;
  //   gyroscope.write(STATUS_REG | 0x80);
  //   int stat = gyroscope.write(0xff);
  //   cs = 1;


  //   printf("%d, ", stat);
  //   // wait_us(100000);
  // }
  cs = 1;
  gyroscope.format(8, 3); // 8 bits per SPI frame; polarity 1, phase 0
  gyroscope.frequency(1000000); // clock frequency deafult 1 MHz max:10MHz
  cs = 0;
  gyroscope.write(WHO_AM_I | 0x80);
  int val = gyroscope.write(0xff);
  cs = 1;
  printf("%d, ", val);

  cs = 0;
  gyroscope.write(CTRL_REG_1);
  gyroscope.write(0xff);
  cs = 1;

  cs = 0;
  gyroscope.write(CTRL_REG_2);
  gyroscope.write(0x04);
  cs = 1;

  cs = 0;
  gyroscope.write(CTRL_REG_4);
  gyroscope.write(0x30);
  cs = 1;

  while(1){
    // cs = 0;
    // gyroscope.write(STATUS_REG | 0x80);
    // int stat = gyroscope.write(0xff);
    // cs = 1;


    // printf("%d, ", stat);
    // wait_us(10000);
    cs = 0;
    wait_us(1);
    gyroscope.write(OUT_X_L | 0x80 | 0x40);
    x_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);
    y_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);
    z_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);

    cs = 1;
    // printf("%d, ", x_data);
    // printf("%d, ", y_data);
    printf("%d, ", z_data);
    
  }


}
