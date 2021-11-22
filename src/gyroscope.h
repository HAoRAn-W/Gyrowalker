/*
* Project:    Challenge - Gyrowalker
* File:       gyroscope.h
* Author:     Haoran Wang
*/

#include <mbed.h>

// Register addresses
#define WHO_AM_I 0x0F

#define CTRL_REG_1 0x20
#define CTRL_REG_2 0x21
#define CTRL_REG_3 0x22
#define CTRL_REG_4 0x23
#define CTRL_REG_5 0x24

#define STATUS_REG 0x27

#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

// Sensitivities dps/digit
#define SENSITIVITY_245 0.00875f
#define SENSITIVITY_500 0.0175f
#define SENSITIVITY_2000 0.07f

// Fullscale selections
#define FULL_SCALE_245 0x00
#define FULL_SCALE_245 0x10
#define FULL_SCALE_2000 0x20
#define FULL_SCALE_2000_alt 0x30

// Data rate and bandwidth


// Convert constants
#define MY_LEG 0.8f // put board on left leg 0.8m above ground
#define DEGREE_TO_RAD 0.0175f // rad = dgree * (pi / 180) 

// Initialization parameters
typedef struct
{
    uint8_t odr;
    uint8_t hpf;
    uint8_t fullscale;
} Gyroscope_Init_Parameters;


// Raw data from gyroscope
typedef struct
{
    int16_t x_raw;
    int16_t y_raw;
    int16_t z_raw;
} Gyroscope_RawData;

// Data in dps
typedef struct
{
    int16_t x_dps;
    int16_t y_dps;
    int16_t z_dps;
} Gyroscope_DPS;

// Function prototypes

// Write IO
void WriteByte(uint8_t address, uint8_t data);

// Read IO
void GetGyroValue(Gyroscope_RawData *rawdata);

// Gyroscope calibration
void CalibrateGyroscope(Gyroscope_RawData *rawdata);

// Gyroscope initialization
void InitiateGyroscope(Gyroscope_Init_Parameters *init_parameters, Gyroscope_RawData *init_raw_data);

// Data conversion: dps -> m/s
float ConvertTOVelocity(int16_t rawdata);

// Get data
void GetCalibratedRawData();