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
    gpiod_line_set_value(driver->lines[pin], value);
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
        pinout->pin_addr[A_Addr], pinout->pin_addr[B_Addr], pinout->pin_addr[C_Addr], pinout->pin_addr[D_Addr], pinout->pin_addr[E_Addr],
        pinout->pin_oe, pinout->pin_lat, pinout->pin_clk
    };

    for (int pin_idx = 0; pin_idx < HUB75_PIN_COUNT; ++pin_idx)
    {
        u8 pin = all_pins[pin_idx];
        gpiod_line* line = gpiod_chip_get_line(out_hub75_driver->chip, pin);
        out_hub75_driver->lines[pin] = line;
    }

    for (int pin_idx = RGB_INDEX_MAX; pin_idx < HUB75_PIN_COUNT; ++pin_idx)
    {
        u8 pin = all_pins[pin_idx];
        gpiod_line* line = out_hub75_driver->lines[pin];

        if (!line || gpiod_line_request_output(line, "hub75_driver", 0) != 0)
        {
            return false;
        }
    }

    gpiod_line_bulk_init(&out_hub75_driver->rgb_bulk);

    gpiod_line_bulk_add(&out_hub75_driver->rgb_bulk, out_hub75_driver->lines[pinout->pin_rgb[R1]]);
    gpiod_line_bulk_add(&out_hub75_driver->rgb_bulk, out_hub75_driver->lines[pinout->pin_rgb[G1]]);
    gpiod_line_bulk_add(&out_hub75_driver->rgb_bulk, out_hub75_driver->lines[pinout->pin_rgb[B1]]);
    gpiod_line_bulk_add(&out_hub75_driver->rgb_bulk, out_hub75_driver->lines[pinout->pin_rgb[R2]]);
    gpiod_line_bulk_add(&out_hub75_driver->rgb_bulk, out_hub75_driver->lines[pinout->pin_rgb[G2]]);
    gpiod_line_bulk_add(&out_hub75_driver->rgb_bulk, out_hub75_driver->lines[pinout->pin_rgb[B2]]);

    struct gpiod_line_request_config config = {
        .consumer = "hub75_driver",
        .request_type = GPIOD_LINE_REQUEST_DIRECTION_OUTPUT,
        .flags = 0
    };

    int default_vals[] = { 0, 0, 0, 0, 0, 0 };
    if (gpiod_line_request_bulk(&out_hub75_driver->rgb_bulk, &config, default_vals) < 0) 
    {
        return false;
    }

    if (gpiod_line_bulk_num_lines(&out_hub75_driver->rgb_bulk) != 6)
    {   
        return false;
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

    gpio_write_pin(driver, p.pin_addr[A_Addr], (row >> 0) & 1);
    gpio_write_pin(driver, p.pin_addr[B_Addr], (row >> 1) & 1);
    gpio_write_pin(driver, p.pin_addr[C_Addr], (row >> 2) & 1);
    gpio_write_pin(driver, p.pin_addr[D_Addr], (row >> 3) & 1);
    gpio_write_pin(driver, p.pin_addr[E_Addr], (row >> 4) & 1);

    int values[6] = {0};

    for (int x = 0; x < MATRIX_PANEL_WIDTH; ++x) 
    {
        const u8* px = &rgb_data[x * 6];

        values[0] = px[0] == 255 ? 1 : 0; // R1
        values[1] = px[1] == 255 ? 1 : 0; // G1
        values[2] = px[2] == 255 ? 1 : 0; // B1
        values[3] = px[3] == 255 ? 1 : 0; // R2
        values[4] = px[4] == 255 ? 1 : 0; // G2
        values[5] = px[5] == 255 ? 1 : 0; // B2

        int result = gpiod_line_set_value_bulk(&driver->rgb_bulk, values);
        gpio_pulse_pin(driver, p.pin_clk);
    }
}