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

#define SENSITIVITY 0.07f
#define MY_LEG 0.8f

SPI gyroscope(PF_9, PF_8, PF_7); // mosi, miso, sclk
DigitalOut cs(PC_1);

int16_t x_data = 0;
int16_t y_data = 0;
int16_t z_data = 0;

int16_t x_threshold;
int16_t y_threshold;
int16_t z_threshold;

int16_t x_sample;
int16_t y_sample;
int16_t z_sample;

// write data to registers in gyrometer
void WriteByte(uint8_t address, uint8_t data)
{
  cs = 0;
  gyroscope.write(address);
  gyroscope.write(data);
  cs = 1;
}

void GetGyroValue()
{
  cs = 0;
  gyroscope.write(OUT_X_L | 0x80 | 0x40);
  x_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);
  y_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);
  z_data = gyroscope.write(0xff) << 8 | gyroscope.write(0xff);
  cs = 1;
}

void CalibrateGyroscope(){
  int16_t sumX = 0;
  int16_t sumY = 0;
  int16_t sumZ = 0;

  for(int i = 0; i < 128; i++){
    GetGyroValue();
    sumX += x_data;
    sumY += y_data;
    sumZ += z_data;
    x_threshold = max(x_threshold, x_data);
    y_threshold = max(y_threshold, y_data);
    z_threshold = max(z_threshold, z_data);
    wait_us(10000);
  }

  x_sample = sumX >> 7;
  y_sample = sumY >> 7;
  z_sample = sumZ >> 7;
}

void InitiateGyroscope()
{
  cs = 1;
  // set up gyroscope
  gyroscope.format(8, 3); // 8 bits per SPI frame; polarity 1, phase 0
  gyroscope.frequency(1000000); // clock frequency deafult 1 MHz max:10MHz

  WriteByte(CTRL_REG_1, 0x0f); // ODR  Bandwidth , enable all 3 axises
  WriteByte(CTRL_REG_2, 0x00); // no high pass filter
  WriteByte(CTRL_REG_4, 0x30); // LSB, full sacle selection: 2000dps

  CalibrateGyroscope();  // calibrate the gyroscope and find the threshold for x, y, and z.
}



int WhoAmI(){
  cs = 0;
  gyroscope.write(STATUS_REG | 0x80);
  int stat = gyroscope.write(0xff);
  return stat;
}


void GetCalibratedRawData(){
  GetGyroValue();
  x_data = x_data - x_sample;
  y_data = y_data - y_sample;
  z_data = z_data - z_sample;
  if(abs(x_data) < abs(x_threshold)) x_data = 0;
  if(abs(y_data) < abs(y_threshold)) y_data = 0;
  if(abs(z_data) < abs(z_threshold)) z_data = 0;
}

int main()
{
  InitiateGyroscope();
  while(1){
    GetCalibratedRawData();
    printf("%d, ", z_data);
  }
}
