/*
* Project:    Challenge - Gyrowalker
* File:       main.cpp
* Author:     Haoran Wang
*/

#include <mbed.h>
#include <stdio.h>
#include "gyroscope.h"
#include "fs.h"

bool start_flag = true; // use reset button to start recording
// Set up the button to trigger an erase
InterruptIn irq(BUTTON1);

static auto erase_event = mbed_event_queue() -> make_user_allocated_event(erase);
int main()
{
  irq.fall(std::ref(erase_event));

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
  }
}