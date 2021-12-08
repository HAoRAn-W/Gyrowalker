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

int main()
{
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
  fflush(stdout);
  UnmountFileSystem();

  while (1)
  {
    printf("\rLast record: %f m", record);

    // record process
    if (start_flag)
    {
      // set up gyroscope initiation parameters
      Gyroscope_Init_Parameters init_parameters;
      init_parameters.odr = 0xff;
      init_parameters.hpf = 0x00;
      init_parameters.fullscale = 0x10;

      InitiateGyroscope(&init_parameters, &raw_data);

      float distance = 0.0f;
      float velocity = 0.0f;

      for (int i = 0; i < 40; i++)
      {
        wait_us(500000);
        GetCalibratedRawData();
        velocity = ConvertToVelocity(raw_data.z_raw);
        distance += abs(velocity / 2);
        printf("distance: %d/%f, \r\n", i, distance);
      }

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