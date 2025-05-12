#include "panel_config.h"

adafruit_bonnet_pinout_s create_adafruit_bonnet_pinout()
{
    adafruit_bonnet_pinout_s result = {};

    result.pin_rgb[R1] = 5;
    result.pin_rgb[G1] = 13;
    result.pin_rgb[B1] = 6;
    result.pin_rgb[R2] = 12;
    result.pin_rgb[G2] = 16;
    result.pin_rgb[B2] = 23;

    result.pin_addr[A_Addr] = 22;
    result.pin_addr[B_Addr] = 26;
    result.pin_addr[C_Addr] = 27;
    result.pin_addr[D_Addr] = 20;
    result.pin_addr[E_Addr] = 8;

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