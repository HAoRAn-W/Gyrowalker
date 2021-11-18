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

Ticker ticker;


int16_t x_data;
int16_t y_data;
int16_t z_data;

int16_t x_linear_speed;
int16_t y_linear_speed;
int16_t z_linear_speed;
float sampledata[20];
int idx = 0;
bool flag = false;

// write data to registers in gyrometer
void WriteByte(uint8_t address, uint8_t data)
{
  cs = 0;
  gyroscope.write(address);
  gyroscope.write(data);
  cs = 1;
}

void InitiateGyroscope()
{
  cs = 1;
  // set up gyroscope
  gyroscope.format(8, 3); // 8 bits per SPI frame; polarity 1, phase 0
  gyroscope.frequency(1000000); // clock frequency deafult 1 MHz max:10MHz

  WriteByte(CTRL_REG_1, 0xff); // ODR 800Hz Bandwidth 110, enable all 3 axises
  WriteByte(CTRL_REG_2, 0x04); // High pass filter mode: normal mode, cutoff frequency 4 Hz(ODR 800Hz)
  WriteByte(CTRL_REG_4, 0x30); // LSB, full sacle selection: 2000dps
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

int WhoAmI(){
  cs = 0;
  gyroscope.write(STATUS_REG | 0x80);
  int stat = gyroscope.write(0xff);
  return stat;
}

void TestSample(){
  flag = true;

}

int main()
{
  InitiateGyroscope();
  // ticker.attach(&TestSample, 0.5);

  while(1){
    GetGyroValue();
    // if(flag){
      
    //   // if(idx == 20){
    //   //   idx = 0;
    //   // }
    //   // sampledata[idx] = (z_data>>4) / 100000;
    //   // idx++;
    //   // flag = false;
    //   printf("%d, ", z_data);
    //   flag = false;
    // }
    

    GetGyroValue();
    printf("%d, ", z_data);
    // if(idx == 20){
    //   for(int i = 0; i < 20; i++){
    //     printf("%f, ", sampledata[i]);
    //   }
    //   printf("\r\n");
    // }
  }


}
