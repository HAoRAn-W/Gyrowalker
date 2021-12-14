/*
* Project:    Gyrowalker
* File:       FileSystem.h
* Author:     Haoran Wang
*/

#include <mbed.h>
#include <stdio.h>
#include <errno.h>
#include <functional>
#include <BlockDevice.h>
#include <LittleFileSystem.h>

// Erase the block device
void erase();

// Mount the file system
void MountFileSystem();

// Unmount the file system
void UnmountFileSystem();

// write data to file
void WriteFile(float data);

// read data from file
void ReadFile(float *record);
