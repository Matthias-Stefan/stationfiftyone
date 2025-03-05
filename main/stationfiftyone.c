/* ========================================================================
   $Project: stationfiftyone $
   $Date: 05.03.2025 $
   $Revision: $
   $Creator: Matthias Stefan $
   $Version: 1.0.0 $
   ======================================================================== */

/** sudo chmod a+rw /dev/ttyUSB0
 *  idf.py -p /dev/ttyUSB0 flash -b 115200 
 *  idf.py -p /dev/ttyUSBO monitor */

 #include "freertos/FreeRTOS.h"
 #include "freertos/task.h"
 #include "esp_log.h"
 #include "driver/rmt_tx.h"



void app_main(void)
{
    ESP_LOGI(__FUNCTION__, "Create RMT TX channel");   
}



