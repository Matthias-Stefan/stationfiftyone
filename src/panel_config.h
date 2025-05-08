#pragma once

#include "defines.h"

typedef u8 gpio_pin;

typedef enum _rgb_index_e
{
    R1 = 0,
    G1,
    B1,
    R2,
    G2,
    B2,

    RGB_INDEX_MAX
} rgb_index_e;

typedef enum _addr_index_e {
    A = 0,
    B,
    C,
    D,
    E,

    ADDR_INDEX_MAX
} addr_index_e;

typedef struct _adafruit_bonnet_pinout_s 
{
    gpio_pin pin_rgb[RGB_INDEX_MAX];
    gpio_pin pin_addr[ADDR_INDEX_MAX];  

    gpio_pin pin_oe;
    gpio_pin pin_clk;
    gpio_pin pin_lat;

    u32 clk_bit;
    u32 lat_bit;
    u32 oe_bit;

    u32 oe_active;
    u32 oe_inactive;

    u32 post_oe_delay;
    u32 post_latch_delay;
    u32 post_addr_delay;
} adafruit_bonnet_pinout_s;

adafruit_bonnet_pinout_s create_adafruit_bonnet_pinout();