// file system to store data
// init file system and mount
// open file and output to terminal on start
// hit button
// clear file, new file, write into file
// save
// do i need to unmount everytime?
#include <mbed.h>
#include <stdio.h>
#include <BlockDevice.h>
#include <LittleFileSystem.h>

#define FORCE_REFORMAT true

BlockDevice *bd = BlockDevice::get_default_instance();
LittleFileSystem fs("fs");

void MountFileSystem();

void UnmountFileSystem();

// write data to file
void WriteFile(int data);

// read data from file
void ReadFile();