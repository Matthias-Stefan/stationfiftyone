#include "hub75_driver.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

b8 hub75_create(const adafruit_bonnet_pinout_s* pinout, hub75_driver_s* out_hub75_driver)
{
    if (pinout == nullptr || out_hub75_driver == nullptr)
    {
        return false;
    }

    memset(out_hub75_driver, 0, sizeof(hub75_driver_s));
    memcpy(&out_hub75_driver->pinout, pinout, sizeof(adafruit_bonnet_pinout_s));
    
    return true;
}

void hub75_destroy(hub75_driver_s* driver)
{
}

void hub75_show(hub75_driver_s* driver)
{

}

void hub75_draw_row(hub75_driver_s* driver, u8 row, const u8* rgb_data)
{

}
