/*
* Project:    Gyrowalker
* File:       main.cpp
* Author:     Haoran Wang
*/

#include <mbed.h>
#include <stdio.h>
#include "Gyroscope.h"
#include "FileSystem.h"
#include "Button.h"

// triggered by double click button -> start recording
bool start_flag = false;

// triggered by long press button -> erase
bool erase_flag = false; 

// An extended button API
Button button(USER_BUTTON);

// double click to start recording
void onDoubleClick()
{
  start_flag = true;
}

// long press to erase
void onLongPress()
{
  erase_flag = true;
}

// click button, doing nothing
void onClick(){

}

int main()
{

  button.onClick(&onClick);
  // attach ISR for button double click
  button.onDoubleClick(&onDoubleClick);

  // attach ISR for button long press
  button.onLongClick(&onLongPress);

  // gyroscope raw data
  Gyroscope_RawData raw_data;

  // distance record in meter
  float record = 0.0f;

  // read last distance record from file
  MountFileSystem();
  ReadFile(&record);
  UnmountFileSystem();

  while (1)
  {
    printf("\rLast record: %f m", record);
     fflush(stdout);

    // record process
    if (start_flag)
    {
      // set up gyroscope initiation parameters
      Gyroscope_Init_Parameters init_parameters;
      init_parameters.odr = ODR_800_CUTOFF_110;
      init_parameters.hpf = ODR_800_HIGH_PASS_56;
      init_parameters.fullscale = FULL_SCALE_500;

      InitiateGyroscope(&init_parameters, &raw_data);

      //todo sample faster and move calculate to later
      int16_t Z_Data_Array[400];
      for (int i = 0; i < 400; i++)
      {
        GetCalibratedRawData();
        Z_Data_Array[i] = raw_data.z_raw;
        wait_us(50000);
        // GetCalibratedRawData();
        // velocity = ConvertToVelocity(raw_data.z_raw);
        // // printf("%f, \r\n", velocity);
        // distance += abs(velocity * sample_interval);
        // // printf("distance: %d/%f, \r\n", i, distance);
      }
      
      float distance = GetDistance(Z_Data_Array);

      // write to file
      MountFileSystem();
      WriteFile(distance);
      UnmountFileSystem(); // unmount file system

      // reset flag
      start_flag = false;

      // update print result
      record = distance;

      // turn off gyroscope
      PowerOff();
    }

    // erase process
    if (erase_flag)
    {
      erase();
      erase_flag = false;
    }
  }
}