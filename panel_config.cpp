#include "panel_config.h"

adafruit_bonnet_pinout_s create_adafruit_bonnet_pinout()
{
    adafruit_bonnet_pinout_s result = {};
    result.pin_rgb = { 5, 13, 6, 12, 16, 23 };
    result.pin_addr = { 22, 26, 27, 20, 24 };
    result.pin_oe = 4;
    result.pin_clk = 17;
    result.pin_lat = 21;
    result.clk_bit = 1u << result.pin_clk;
    result.lat_bit = 1u << result.pin_lat;
    result.oe_bit  = 1u << result.pin_oe;
    result.oe_active = 0;
    result.oe_inactive = result.oe_bit;
    result.post_oe_delay    = 0;
    result.post_latch_delay = 0;
    result.post_addr_delay  = 5;
    return result;
}