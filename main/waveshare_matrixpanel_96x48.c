#include "waveshare_matrixpanel_96x48.h"

#include <driver/gpio.h>
#include <driver/i2s_std.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>


#define MATRIX_HEIGHT   48
#define MATRIX_WIDTH    96
#define CHAIN_LENGTH     1
#define SCAN_LINES      24

#define A_PIN_DEFAULT   23
#define B_PIN_DEFAULT   22
#define C_PIN_DEFAULT    5
#define D_PIN_DEFAULT   17
#define E_PIN_DEFAULT   32

#define R1_PIN_DEFAULT  25
#define G1_PIN_DEFAULT  26
#define B1_PIN_DEFAULT  27
#define R2_PIN_DEFAULT  14
#define G2_PIN_DEFAULT  12
#define B2_PIN_DEFAULT  13

#define CLK_PIN_DEFAULT 16
#define LAT_PIN_DEFAULT  4
#define OE_PIN_DEFAULT  15

#ifndef MATRIX_ROWS_IN_PARALLEL
    #define MATRIX_ROWS_IN_PARALLEL 2
#endif

#ifndef PIXEL_COLOR_DEPTH_BITS
    #define PIXEL_COLOR_DEPTH_BITS 8
#endif

#ifndef COLOR_CHANNELS_PER_PIXEL
    #define COLOR_CHANNELS_PER_PIXEL 3
#endif

#define DMA_BUFFER_SIZE (MATRIX_WIDTH * 2)

static uint8_t framebuffer[MATRIX_WIDTH * MATRIX_HEIGHT * COLOR_CHANNELS_PER_PIXEL];
static i2s_chan_handle_t i2s_chan_handle;

// NOTE: enabled
void setup_i2s(uint32_t sample_rate)
{
    i2s_chan_config_t i2s_chan_config = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER); 
    i2s_new_channel(&i2s_chan_config, &i2s_chan_handle, NULL);

    i2s_std_config_t i2s_config = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(sample_rate),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = CLK_PIN_DEFAULT,
            .ws = I2S_GPIO_UNUSED,
            .dout = B1_PIN_DEFAULT,
            .din = I2S_GPIO_UNUSED
        },
    };

    i2s_channel_init_std_mode(i2s_chan_handle, &i2s_config);
    i2s_channel_enable(i2s_chan_handle);
}

void clear_framebuffer()
{
    memset(framebuffer, 0, sizeof(framebuffer));
}

void draw_pixel(int32_t x, int32_t y, 
                uint8_t red, uint8_t green, uint8_t blue)
{
    if (x < 0 || x >= MATRIX_WIDTH || y < 0 || y >= MATRIX_HEIGHT)
    {
        return;
    }

    int32_t index = (y * MATRIX_WIDTH + x) * COLOR_CHANNELS_PER_PIXEL;
    framebuffer[index] = red;
    framebuffer[index] = green;
    framebuffer[index] = blue;
}

void display()
{
    static int row = 0;

    gpio_set_level(OE_PIN_DEFAULT, 1);

    gpio_set_level(A_PIN_DEFAULT, (row >> 0) & 1);
    gpio_set_level(B_PIN_DEFAULT, (row >> 1) & 1);
    gpio_set_level(C_PIN_DEFAULT, (row >> 2) & 1);
    gpio_set_level(D_PIN_DEFAULT, (row >> 3) & 1);
    gpio_set_level(E_PIN_DEFAULT, (row >> 4) & 1);

    uint16_t line_buffer[MATRIX_WIDTH] = { 0 };
    for (int32_t x = 0; x < MATRIX_WIDTH; ++x)
    {
        int32_t index = (row * MATRIX_WIDTH + x) * COLOR_CHANNELS_PER_PIXEL;
        uint8_t r = framebuffer[index];
        uint8_t g = framebuffer[index + 1];
        uint8_t b = framebuffer[index + 2];

        uint16_t pixel_out = 0;
        pixel_out |= ((r & 0b10000000) ? (1 << 0) : 0); // R1
        pixel_out |= ((g & 0b10000000) ? (1 << 1) : 0); // G1
        pixel_out |= ((b & 0b10000000) ? (1 << 2) : 0); // B1

        pixel_out |= ((r & 0b10000000) ? (1 << 8) : 0); // R2
        pixel_out |= ((g & 0b10000000) ? (1 << 9) : 0); // G2
        pixel_out |= ((b & 0b10000000) ? (1 << 10) : 0); // B2
        
        line_buffer[x] = pixel_out;

        gpio_set_level(CLK_PIN_DEFAULT, 1);
        gpio_set_level(CLK_PIN_DEFAULT, 0);
    }

    ESP_LOGI(__FUNCTION__, "Update row %d", row);
    i2s_channel_write(i2s_chan_handle, line_buffer, sizeof(line_buffer), NULL, portMAX_DELAY);

    gpio_set_level(LAT_PIN_DEFAULT, 1);
    gpio_set_level(LAT_PIN_DEFAULT, 0);

    gpio_set_level(OE_PIN_DEFAULT, 0);

    row = (row + 1) % SCAN_LINES;
}

// NOTE: enabled
void init_gpio()
{
    gpio_set_direction(A_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_set_direction(B_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_set_direction(C_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_set_direction(D_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_set_direction(E_PIN_DEFAULT, GPIO_MODE_OUTPUT);

    gpio_set_direction(R1_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_set_direction(G1_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_set_direction(B1_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_set_direction(R2_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_set_direction(G2_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_set_direction(B2_PIN_DEFAULT, GPIO_MODE_OUTPUT);

    gpio_set_direction(CLK_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_set_direction(LAT_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_set_direction(OE_PIN_DEFAULT, GPIO_MODE_OUTPUT);
}

void throw_if_failed(gpio_num_t gpio, uint32_t level)
{
    esp_err_t error = gpio_set_level(gpio, level);
    if (error != ESP_OK)
    {
        ESP_LOGE("test_pattern", "Error with gpio: %d", gpio);
    }
}

// NOTE: enabled
void test_pattern(int r, int g, int b)
{
    ESP_LOGI(__FUNCTION__, "Start test pattern");
    
    throw_if_failed(OE_PIN_DEFAULT, 1);

    throw_if_failed(R1_PIN_DEFAULT, r);
    throw_if_failed(G1_PIN_DEFAULT, g);
    throw_if_failed(B1_PIN_DEFAULT, b);
    throw_if_failed(R2_PIN_DEFAULT, r);
    throw_if_failed(G2_PIN_DEFAULT, g);
    throw_if_failed(B2_PIN_DEFAULT, b);

    throw_if_failed(OE_PIN_DEFAULT, 0); // Anzeige aktivieren
    throw_if_failed(LAT_PIN_DEFAULT, 1);
    vTaskDelay(pdMS_TO_TICKS(1));
    throw_if_failed(LAT_PIN_DEFAULT, 0);
}


