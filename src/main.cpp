#include "defines.h"
#include "panel_config.h"
#include "hub75_driver.h"

#include <chrono>
#include <iostream>
#include <unistd.h> 

// vcgencmd measure_temp


#define TARGET_FPS 60
#define FRAME_TIME_US (1000000 / TARGET_FPS) 

static u8 framebuffer[MATRIX_PANEL_HEIGHT][MATRIX_PANEL_WIDTH][3];

int main()
{
    adafruit_bonnet_pinout_s pinout = create_adafruit_bonnet_pinout();
    hub75_driver_s driver = {};
    
    if (!hub75_create(&pinout, &driver)) 
    {
        return 1;
    }

    for (int y = 0; y < MATRIX_PANEL_HEIGHT; ++y)
    {
        u8 red = 64;
        if (y < 12)
        {
            red = 128;
        }
        u8 green = 255;
        u8 blue = 0;

        for (int x = 0; x < MATRIX_PANEL_WIDTH; ++x)
        {
            framebuffer[y][x][r] = red;
            framebuffer[y][x][g] = green;
            framebuffer[y][x][b] = blue;
        }
    }

    auto t_start = std::chrono::steady_clock::now();
    u64 frame_count = 0;

    while (true) 
    {
        for (int pwm_level = 0; pwm_level < PWM_STEPS; ++pwm_level)
        {
            for (int y = 0; y < MATRIX_PANEL_HEIGHT_HALF; ++y)
            {
                u8 row_data[MATRIX_PANEL_WIDTH * 6] = {};
                for (int x = 0; x < MATRIX_PANEL_WIDTH; ++x)
                {
                    u8 r1 = framebuffer[y][x][r] >> 6;
                    u8 g1 = framebuffer[y][x][g] >> 6;
                    u8 b1 = framebuffer[y][x][b] >> 6;

                    u8 r2 = framebuffer[y + MATRIX_PANEL_HEIGHT_HALF][x][r] >> 6;
                    u8 g2 = framebuffer[y + MATRIX_PANEL_HEIGHT_HALF][x][g] >> 6;
                    u8 b2 = framebuffer[y + MATRIX_PANEL_HEIGHT_HALF][x][b] >> 6;

                    row_data[x * 6 + 0] = (r1 > pwm_level) ? 255 : 0;
                    row_data[x * 6 + 1] = (g1 > pwm_level) ? 255 : 0;
                    row_data[x * 6 + 2] = (b1 > pwm_level) ? 255 : 0;
                    row_data[x * 6 + 3] = (r2 > pwm_level) ? 255 : 0;
                    row_data[x * 6 + 4] = (g2 > pwm_level) ? 255 : 0;
                    row_data[x * 6 + 5] = (b2 > pwm_level) ? 255 : 0;
                }

                hub75_draw_row(&driver, y, row_data);
                hub75_show(&driver);
            }
        }
        frame_count++;

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - t_start).count() >= 1000)
        {
            std::cout << "FPS: " << frame_count << std::endl;
            frame_count = 0;
            t_start = now;
        }
    }

    hub75_destroy(&driver);
    return 0;
}

