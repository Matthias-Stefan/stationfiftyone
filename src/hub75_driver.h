#pragma once

#include "panel_config.h"

#include <gpiod.h>

// Color resolution in this mode:
// Bits per channel | Levels | Total colors
// 2                |   4    |      64
// 3                |   8    |     512
// 4                |  16    |    4096
// 8 (full color)   | 256    | 16.7 million

#define PWM_STEPS 4

typedef struct _hub75_driver_s 
{
    adafruit_bonnet_pinout_s pinout;
    gpiod_chip* chip;
    gpiod_line* lines[32];
    gpiod_line_bulk rgb_bulk;
} hub75_driver_s;


b8 hub75_create(const adafruit_bonnet_pinout_s* pinout, hub75_driver_s* out_hub75_driver);

void hub75_destroy(hub75_driver_s* driver);

void hub75_show(hub75_driver_s* driver);

void hub75_draw_row(hub75_driver_s* driver, u8 row, const u8* rgb_data);
