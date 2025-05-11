#include "defines.h"
#include "panel_config.h"
#include "hub75_driver.h"

#include <chrono>
#include <iostream>
#include <unistd.h> 



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
        int Color = 255;
        for (int x = 0; x < MATRIX_PANEL_WIDTH; ++x) 
        {
            framebuffer[y][x][0] = Color;
            framebuffer[y][x][1] = 0;
            framebuffer[y][x][2] = Color;
        }
    }

    auto t_start = std::chrono::steady_clock::now();
    u64 frame_count = 0;

    while (true) 
    {
        for (int y = 0; y < 24; ++y)
        {
            u8 row_data[MATRIX_PANEL_WIDTH * 3] = {};
            for (int x = 0; x < MATRIX_PANEL_WIDTH; ++x)
            {
                row_data[x * 3 + 0] = framebuffer[y][x][0];
                row_data[x * 3 + 1] = framebuffer[y][x][1];
                row_data[x * 3 + 2] = framebuffer[y][x][2];
            }

            hub75_draw_row(&driver, y, row_data);
            hub75_show(&driver);
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

