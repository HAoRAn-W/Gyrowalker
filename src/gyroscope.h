/*
* Project:    Gyrowalker
* File:       Gyroscope.h
* Author:     Haoran Wang
*/

#include <mbed.h>

// Register addresses

#define WHO_AM_I 0x0F

#define CTRL_REG_1 0x20 // control register 1
#define CTRL_REG_2 0x21 // control register 2
#define CTRL_REG_3 0x22 // control register 3
#define CTRL_REG_4 0x23 // control register 4
#define CTRL_REG_5 0x24 // control register 5

#define STATUS_REG 0x27 // status register

#define OUT_X_L 0x28 // X-axis angular rate data Low
#define OUT_X_H 0x29 // X-axis angular rate data high
#define OUT_Y_L 0x2A // Y-axis angular rate data low
#define OUT_Y_H 0x2B // Y-axis angular rate data high
#define OUT_Z_L 0x2C // Z-axis angular rate data low
#define OUT_Z_H 0x2D // Z-axis angular rate data high

// Fullscale selections

#define FULL_SCALE_245 0x00 // full scale 245 dps
#define FULL_SCALE_500 0x10 // full scale 500 dps
#define FULL_SCALE_2000 0x20 // full scale 2000 dps
#define FULL_SCALE_2000_ALT 0x30 // full scale 2000 dps

// Sensitivities in dps/digit

#define SENSITIVITY_245 0.00875f // 245 dps typical sensitivity
#define SENSITIVITY_500 0.0175f // 500 dps typical sensitivity
#define SENSITIVITY_2000 0.07f // 2000 dps typical sensitivity

// Data rate and bandwidth


// Convert constants
#define MY_LEG 0.9f // put board on left leg 0.8m above ground
#define DEGREE_TO_RAD 0.0175f // rad = dgree * (pi / 180) 
#define POWEROFF 0x00

#define SAMPLE_TIME_20 20
#define SAMPLE_INTERVAL_0_05 0.005f

// Initialization parameters
typedef struct
{
    uint8_t odr; // output data rate
    uint8_t hpf; // high pass filter selection
    uint8_t fullscale; // full sacle selection
} Gyroscope_Init_Parameters;


// Raw data
typedef struct
{
    int16_t x_raw; // X-axis raw data
    int16_t y_raw; // Y-axis raw data
    int16_t z_raw; // Z-axis raw data
} Gyroscope_RawData;

// Calibrated data
typedef struct {
    int16_t x_calibrated; // X-axis calibrated data
    int16_t y_calibrated; // Y-axis calibrated data
    int16_t z_calibrated; // Z-axis calibrated data
} Gyroscope_CalibratedData;

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
float ConvertToVelocity(int16_t rawdata);

// Calculate distance from raw data array;
float GetDistance(int16_t arr[]);

// Get calibrated data
void GetCalibratedRawData();

// Turn off the gyroscope
void PowerOff();
