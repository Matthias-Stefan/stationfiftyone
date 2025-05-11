#include "hub75_driver.h"

#include "defines.h"

#include <iostream>
#include <gpiod.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void gpio_write_pin(hub75_driver_s* driver, u8 pin, b8 value) 
{
    int result = gpiod_line_set_value(driver->lines[pin], value);
    if (result != 0)
    {
        int x = 5;
    }
}


static void gpio_pulse_pin(hub75_driver_s* driver, u8 pin) 
{
    gpio_write_pin(driver, pin, 1);
    gpio_write_pin(driver, pin, 0);
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
    if (!out_hub75_driver->chip)
    {
        return false;
    }

    u8 all_pins[HUB75_PIN_COUNT] = {
        pinout->pin_rgb[R1], pinout->pin_rgb[G1], pinout->pin_rgb[B1],
        pinout->pin_rgb[R2], pinout->pin_rgb[G2], pinout->pin_rgb[B2],
        pinout->pin_addr[A], pinout->pin_addr[B], pinout->pin_addr[C], pinout->pin_addr[D], pinout->pin_addr[E],
        pinout->pin_oe, pinout->pin_lat, pinout->pin_clk
    };

    for (int pin_idx = 0; pin_idx < HUB75_PIN_COUNT; ++pin_idx)
    {
        u8 pin = all_pins[pin_idx];
        out_hub75_driver->lines[pin] = gpiod_chip_get_line(out_hub75_driver->chip, pin);
        gpiod_line_request_output(out_hub75_driver->lines[pin], "stationfiftyone", 0);
    }

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
    gpio_write_pin(driver, p.pin_oe, 1);
    gpio_pulse_pin(driver, p.pin_lat);
    gpio_write_pin(driver, p.pin_oe, 0);
}


void hub75_draw_row(hub75_driver_s* driver, u8 row, const u8* rgb_data)
{
    const auto& p = driver->pinout;

    int a = (row >> A) & 1;
    int b = (row >> B) & 1;
    int c = (row >> C) & 1;
    int d = (row >> D) & 1;
    int e = (row >> E) & 1;

    gpio_write_pin(driver, p.pin_addr[A], a);
    gpio_write_pin(driver, p.pin_addr[B], b);
    gpio_write_pin(driver, p.pin_addr[C], c);
    gpio_write_pin(driver, p.pin_addr[D], d);
    gpio_write_pin(driver, p.pin_addr[E], e);

    // TODO: get right color shit
    for (int x = 0; x < MATRIX_PANEL_WIDTH; ++x) 
    {
        u8 r = rgb_data[x * 3 + 0];
        u8 g = rgb_data[x * 3 + 1];
        u8 b = rgb_data[x * 3 + 2];

        gpio_write_pin(driver, p.pin_rgb[R1], r > 127);
        gpio_write_pin(driver, p.pin_rgb[G1], g > 127);
        gpio_write_pin(driver, p.pin_rgb[B1], b > 127);

        gpio_write_pin(driver, p.pin_rgb[R2], r > 127);
        gpio_write_pin(driver, p.pin_rgb[G2], g > 127);
        gpio_write_pin(driver, p.pin_rgb[B2], b > 127);

        gpio_pulse_pin(driver, p.pin_clk);
    }

    int end = 5;
    (void)end;
}
