#include "hub75_driver.h"

#include "defines.h"

#include <gpiod.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void gpio_write_pin(gpiod_chip* chip, u8 pin, b8 value) 
{
    gpiod_line* line = gpiod_chip_get_line(chip, pin);
    gpiod_line_request_output(line, "stationfiftyone", 0);
    gpiod_line_set_value(line, value);
    gpiod_line_release(line);
}

static void gpio_pulse_pin(gpiod_chip* chip, u8 pin) 
{
    gpio_write_pin(chip, pin, 1);
    usleep(1);
    gpio_write_pin(chip, pin, 0);
}



b8 hub75_create(const adafruit_bonnet_pinout_s* pinout, hub75_driver_s* out_hub75_driver)
{
    if (pinout == nullptr || out_hub75_driver == nullptr)
    {
        return false;
    }

    memset(out_hub75_driver, 0, sizeof(hub75_driver_s));
    memcpy(&out_hub75_driver->pinout, pinout, sizeof(adafruit_bonnet_pinout_s));
    
    out_hub75_driver->chip = gpiod_chip_open_by_name("gpiochip0");

    return true;
}

void hub75_destroy(hub75_driver_s* driver)
{
    if (!driver) 
    {
        return;
    }
    gpiod_chip_close(driver->chip);
    delete driver;
}

void hub75_show(hub75_driver_s* driver)
{
    const auto& p = driver->pinout;
    gpio_write_pin(driver->chip, p.pin_oe, 1);
    gpio_pulse_pin(driver->chip, p.pin_lat);
    gpio_write_pin(driver->chip, p.pin_oe, 0);
}

void hub75_draw_row(hub75_driver_s* driver, u8 row, const u8* rgb_data)
{
    const auto& p = driver->pinout;

    for (int i = 0; i < ADDR_INDEX_MAX; ++i) 
    {
        gpio_write_pin(driver->chip, p.pin_addr[i], (row >> i) & 1);
    }

    for (int col = 0; col < 96; ++col) 
    {
        u8 r = rgb_data[col * 3 + 0];
        u8 g = rgb_data[col * 3 + 1];
        u8 b = rgb_data[col * 3 + 2];

        gpio_write_pin(driver->chip, p.pin_rgb[R1], r > 127);
        gpio_write_pin(driver->chip, p.pin_rgb[G1], g > 127);
        gpio_write_pin(driver->chip, p.pin_rgb[B1], b > 127);

        gpio_write_pin(driver->chip, p.pin_rgb[R2], 0);
        gpio_write_pin(driver->chip, p.pin_rgb[G2], 0);
        gpio_write_pin(driver->chip, p.pin_rgb[B2], 0);

        gpio_pulse_pin(driver->chip, p.pin_clk);
    }
}
