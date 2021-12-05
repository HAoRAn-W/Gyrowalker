/*
* Project:    Challenge - Gyrowalker
* File:       main.cpp
* Author:     Haoran Wang
*/

#include <mbed.h>
#include "gyroscope.h"

bool flag = false;
DigitalOut led(LED3);
int idx = 0;

void flip(){
  flag = true;
  led = !led;
}

int main()
{
  Gyroscope_Init_Parameters init_parameters;
  init_parameters.odr = 0xff;
  init_parameters.hpf = 0x00;
  init_parameters.fullscale = 0x10;

  Gyroscope_RawData raw_data;

  // float buffer[40];
  // Ticker ticker;
  // ticker.attach(&flip, 1000ms);


  InitiateGyroscope(&init_parameters, &raw_data);
  float distance = 0.0f;

  // sample 20 s  and stop, repeat output to terminal the distance.
  while(1){
    // GetCalibratedRawData();
    // printf("%d, ", raw_data.z_raw);
    wait_us(500000);
    GetCalibratedRawData();

    float v = ConvertTOVelocity(raw_data.z_raw);
    distance += abs(v/2);
    printf("distance: %f, \r\n", distance);
    // if(idx == 40){
    //   idx = 0;
    //   for(int i = 0; i < 40; i++){
    //     printf("%f, ", buffer[i]);
    //   }
    // }
    // if(flag){
      
    //   // printf("%d, ", raw_data.z_raw);
    //   buffer[idx++] = raw_data.z_raw;
    //   flag = false;
    //   led = !led;
    // }
    
  }
}
