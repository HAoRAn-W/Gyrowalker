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

bool start_flag = false; // trigger start record
bool erase_flag = false; // trigger erase function

Button button(USER_BUTTON);
// double click to start
void onDoubleClick()
{
  start_flag = true;
}

// long press to reset
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

  Gyroscope_RawData raw_data;
  float record = 0.0f;

  MountFileSystem();
  ReadFile(&record);
  fflush(stdout);
  UnmountFileSystem();

  while (1)
  {
    printf("\rLast record: %f m", record);

    if (start_flag)
    {
      Gyroscope_Init_Parameters init_parameters;
      init_parameters.odr = 0xff;
      init_parameters.hpf = 0x00;
      init_parameters.fullscale = 0x10;

      InitiateGyroscope(&init_parameters, &raw_data);

      float distance = 0.0f;
      float v = 0.0f;
      for (int i = 0; i < 40; i++)
      {
        wait_us(500000);
        GetCalibratedRawData();
        v = ConvertTOVelocity(raw_data.z_raw);
        distance += abs(v / 2);
        printf("distance: %d/%f, \r\n", i, distance);
      }
      MountFileSystem();
      // write to file
      WriteFile(distance);
      UnmountFileSystem(); // unmount file system
      start_flag = false;
      // update print result
      record = distance;
      PowerOff(); // turn off gyroscope
    }
    if (erase_flag)
    {
      erase();
      erase_flag = false;
    }
  }
}