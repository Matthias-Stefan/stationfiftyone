#include "panel_config.h"
#include "hub75_driver.h"

#include <unistd.h> 

int main()
{
    adafruit_bonnet_pinout_s pinout = create_adafruit_bonnet_pinout();
    hub75_driver_s hub75_driver = {};
    
    if (!hub75_create(&pinout, &hub75_driver)) 
    {
        return 1;
    }

    u8 row_data[96 * 3];
    for (int i = 0; i < 96; ++i) 
    {
        row_data[i * 3 + 0] = 255;
        row_data[i * 3 + 1] = 0;
        row_data[i * 3 + 2] = 0;
    }

    while (true) 
    {
        hub75_draw_row(&hub75_driver, 0, row_data);
        hub75_show(&hub75_driver);

        usleep(20000);
    }

    hub75_destroy(&hub75_driver);
    return 0;
}

