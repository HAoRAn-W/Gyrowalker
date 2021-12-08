/*
* Project:    Challenge - Gyrowalker
* File:       gyroscope.cpp
* Author:     Haoran Wang
*/

#include <mbed.h>
#include "Gyroscope.h"

SPI gyroscope(PF_9, PF_8, PF_7); // mosi, miso, sclk
DigitalOut cs(PC_1);

DigitalOut led(LED3); // indicate initialize

int16_t x_threshold;
int16_t y_threshold;
int16_t z_threshold;

int16_t x_sample;
int16_t y_sample;
int16_t z_sample;

float sensitivity = 0.0f;

Gyroscope_RawData *gyro_raw;
Gyroscope_DPS gyro_dps;

void WriteByte(uint8_t address, uint8_t data)
{
  cs = 0;
  gyroscope.write(address);
  gyroscope.write(data);
  cs = 1;
}

void GetGyroValue(Gyroscope_RawData *rawdata)
{
  cs = 0;
  gyroscope.write(OUT_X_L | 0x80 | 0x40);
  rawdata->x_raw = gyroscope.write(0xff) | gyroscope.write(0xff) << 8;
  rawdata->y_raw = gyroscope.write(0xff) | gyroscope.write(0xff) << 8;
  rawdata->z_raw = gyroscope.write(0xff) | gyroscope.write(0xff) << 8;
  cs = 1;
}

void CalibrateGyroscope(Gyroscope_RawData *rawdata)
{
  int16_t sumX = 0;
  int16_t sumY = 0;
  int16_t sumZ = 0;
  printf("========[Calibrating...]========\r\n");
  for (int i = 0; i < 128; i++)
  {
    GetGyroValue(rawdata);
    sumX += rawdata->x_raw;
    sumY += rawdata->y_raw;
    sumZ += rawdata->z_raw;
    x_threshold = max(x_threshold, rawdata->x_raw);
    y_threshold = max(y_threshold, rawdata->y_raw);
    z_threshold = max(z_threshold, rawdata->z_raw);
    wait_us(10000);
  }

  x_sample = sumX >> 7;
  y_sample = sumY >> 7;
  z_sample = sumZ >> 7;
  printf("========[Calibration finish.]========\r\n");
}

void InitiateGyroscope(Gyroscope_Init_Parameters *init_parameters, Gyroscope_RawData *init_raw_data)
{
  printf("\r\n========[Initializing gyroscope...]========\r\n");
  led = !led;
  gyro_raw = init_raw_data;
  cs = 1;
  // set up gyroscope
  gyroscope.format(8, 3);       // 8 bits per SPI frame; polarity 1, phase 0
  gyroscope.frequency(1000000); // clock frequency deafult 1 MHz max:10MHz

  WriteByte(CTRL_REG_1, init_parameters->odr);       // ODR  Bandwidth , enable all 3 axises
  WriteByte(CTRL_REG_2, init_parameters->hpf);       // no high pass filter
  WriteByte(CTRL_REG_4, init_parameters->fullscale); // LSB, full sacle selection: 500dps

  switch (init_parameters->fullscale)
  {
  case FULL_SCALE_245:
    sensitivity = SENSITIVITY_245;
    break;

  case FULL_SCALE_500:
    sensitivity = SENSITIVITY_500;
    break;

  case FULL_SCALE_2000:
    sensitivity = SENSITIVITY_2000;
    break;

  case FULL_SCALE_2000_ALT:
    sensitivity = SENSITIVITY_2000;
    break;
  }

  CalibrateGyroscope(gyro_raw); // calibrate the gyroscope and find the threshold for x, y, and z.
  led = !led;
  printf("========[Initiation finish.]========\r\n");
}

float ConvertTOVelocity(int16_t rawdata)
{
  float velocity = rawdata * sensitivity * DEGREE_TO_RAD * MY_LEG;
  return velocity;
}

void GetCalibratedRawData()
{
  GetGyroValue(gyro_raw);
  gyro_raw->x_raw -= x_sample;
  gyro_raw->y_raw -= y_sample;
  gyro_raw->z_raw -= z_sample;
  if (abs(gyro_raw->x_raw) < abs(x_threshold))
    gyro_raw->x_raw = 0;
  if (abs(gyro_raw->y_raw) < abs(y_threshold))
    gyro_raw->y_raw = 0;
  if (abs(gyro_raw->z_raw) < abs(z_threshold))
    gyro_raw->z_raw = 0;
}


//poweroff
void PowerOff(){
  WriteByte(CTRL_REG_1, 0x00);
}