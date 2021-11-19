#include <mbed.h>
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

typedef struct
{
    int16_t x_raw;
    int16_t y_raw;
    int16_t z_raw;
} Gyroscope_RawData;

typedef struct
{
    int16_t x_dps;
    int16_t y_dps;
    int16_t z_dps;
} Gyroscope_DPS;
