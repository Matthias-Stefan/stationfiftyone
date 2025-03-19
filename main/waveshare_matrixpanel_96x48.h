#pragma once

#include <stdint.h>

void setup_i2s(uint32_t sample_rate);
void clear_framebuffer();
void draw_pixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b);
void display();
void init_gpio();

void test_pattern([[maybe_unused]] int r, [[maybe_unused]] int g, [[maybe_unused]] int b);
