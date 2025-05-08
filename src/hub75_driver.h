#pragma once

#include "panel_config.h"

struct hub75_driver_s;

hub75_driver_s* hub75_create(const adafruit_bonnet_pinout_s* pinout);

void hub75_destroy(hub75_driver_s* driver);

void hub75_show(hub75_driver_s* driver);

void hub75_draw_row(hub75_driver_s* driver, u8 row, const u8* rgb_data);
