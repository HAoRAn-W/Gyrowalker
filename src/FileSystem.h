#include <mbed.h>
#include <stdio.h>
#include <errno.h>
#include <functional>
#include <BlockDevice.h>
#include <LittleFileSystem.h>

void erase();

void MountFileSystem();

void UnmountFileSystem();

// write data to file
void WriteFile(float data);

// read data from file
void ReadFile(float *record);
