#include "panel_config.h"
#include "hub75_driver.h"


int main()
{
    adafruit_bonnet_pinout_s pinout = create_adafruit_bonnet_pinout();
    hub75_driver_s hub75_driver = {};
    hub75_create(&pinout, &hub75_driver);

    return 0;
}

