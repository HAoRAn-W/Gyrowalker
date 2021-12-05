/*
* Project:    Challenge - Gyrowalker
* File:       main.cpp
* Author:     Haoran Wang
*/

#include <mbed.h>
#include <stdio.h>
#include "gyroscope.h"

bool start_flag = false;

InterruptIn irq(USER_BUTTON);

void flip()
{
  start_flag = true;
}

int main()
{
  Gyroscope_RawData raw_data;
  irq.fall(&flip);

  // InitiateGyroscope(&init_parameters, &raw_data);
  

  //init file system

  //read from file, output data
  float result = 0.0f;

  // sample 20 s  and stop, repeat output to terminal the distance.
  while (1)
  {
    printf("\rLast record: ");
    fflush(stdout);
    // read data

    if (start_flag)
    {
      Gyroscope_Init_Parameters init_parameters;
      init_parameters.odr = 0xff;
      init_parameters.hpf = 0x00;
      init_parameters.fullscale = 0x10;

      InitiateGyroscope(&init_parameters, &raw_data);

      float distance = 0.0f;
      float v = 0.0f;
      for(int i = 0; i < 20; i++){
        wait_us(500000);
        GetCalibratedRawData();
        v = ConvertTOVelocity(raw_data.z_raw);
        distance += abs(v / 2);
        printf("distance: %d/%f, \r\n", i, distance);
      }
      
      
      // write to file
      start_flag = false;
      // update print result
      PowerOff(); // turn off gyroscope

    }
  }
}
