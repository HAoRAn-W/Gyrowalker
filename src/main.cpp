/*
* Project:    Challenge - Gyrowalker
* File:       main.cpp
* Author:     Haoran Wang
*/

#include <mbed.h>
#include "gyroscope.h"

int main()
{
  Gyroscope_Init_Parameters init_parameters;
  init_parameters.odr = 0xcf;
  init_parameters.hpf = 0x00;
  init_parameters.fullscale = 0x10;
  Gyroscope_RawData raw_data;

  InitiateGyroscope(&init_parameters, &raw_data);
  

  while(1){
    GetCalibratedRawData();
    printf("%d, ", raw_data.z_raw);
  }
}
