#pragma once

#include "panel_config.h"

#include <gpiod.h>

typedef struct _hub75_driver_s 
{
    adafruit_bonnet_pinout_s pinout;
    gpiod_chip* chip;
} hub75_driver_s;


b8 hub75_create(const adafruit_bonnet_pinout_s* pinout, hub75_driver_s* out_hub75_driver);

void hub75_destroy(hub75_driver_s* driver);

void hub75_show(hub75_driver_s* driver);

void hub75_draw_row(hub75_driver_s* driver, u8 row, const u8* rgb_data);
