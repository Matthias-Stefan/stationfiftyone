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

#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define MATRIX_WIDTH 96
#define MATRIX_HEIGHT 48
#define SCAN_LINES 24

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


#include <stdint.h>

void set_pixel(uint8_t x, uint8_t y, uint8_t r, int8_t g, int8_t b)
{
    /** select row */
    gpio_set_level(OE_PIN_DEFAULT, 1);

    uint8_t y24 = y;
    if (y > 23)
    {
        y24 = y - 24;
    }

    gpio_set_level(A_PIN_DEFAULT, (y24 & 0b00001));
    gpio_set_level(B_PIN_DEFAULT, (y24 & 0b00010));
    gpio_set_level(C_PIN_DEFAULT, (y24 & 0b00100));
    gpio_set_level(D_PIN_DEFAULT, (y24 & 0b01000));
    gpio_set_level(E_PIN_DEFAULT, (y24 & 0b10000));
    
    ESP_LOGI(__FUNCTION__, "x = %d | A=%d, B=%d, C=%d, D=%d, E=%d",
            x,
            gpio_get_level(A_PIN_DEFAULT),
            gpio_get_level(B_PIN_DEFAULT),
            gpio_get_level(C_PIN_DEFAULT),
            gpio_get_level(D_PIN_DEFAULT),
            gpio_get_level(E_PIN_DEFAULT));

    /** select column */
    for (uint8_t column = 0; column < MATRIX_WIDTH; ++column)
    {
        gpio_set_level(R1_PIN_DEFAULT, 0);
        gpio_set_level(G1_PIN_DEFAULT, 0);
        gpio_set_level(B1_PIN_DEFAULT, 0);
        gpio_set_level(R2_PIN_DEFAULT, 0);
        gpio_set_level(G2_PIN_DEFAULT, 0);
        gpio_set_level(B2_PIN_DEFAULT, 0);

        gpio_set_level(OE_PIN_DEFAULT, 0);

        if (column == x)
        {
            if (y < 24)
            {
                ESP_LOGI(__FUNCTION__, "Top");
                gpio_set_level(R1_PIN_DEFAULT, r);
                gpio_set_level(G1_PIN_DEFAULT, g);
                gpio_set_level(B1_PIN_DEFAULT, b);
            }
            else 
            {
                ESP_LOGI(__FUNCTION__, "Bottom");
                gpio_set_level(R2_PIN_DEFAULT, r);
                gpio_set_level(G2_PIN_DEFAULT, g);
                gpio_set_level(B2_PIN_DEFAULT, b);        
            }
        }
       
        gpio_set_level(CLK_PIN_DEFAULT, 1);
        gpio_set_level(CLK_PIN_DEFAULT, 0);
    }

    gpio_set_level(LAT_PIN_DEFAULT, 1);
    gpio_set_level(LAT_PIN_DEFAULT, 0);
}


void app_main()
{
    // **Setze GPIOs zurück und konfiguriere sie als OUTPUT**
    gpio_reset_pin(A_PIN_DEFAULT);
    gpio_set_direction(A_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_reset_pin(B_PIN_DEFAULT);
    gpio_set_direction(B_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_reset_pin(C_PIN_DEFAULT);
    gpio_set_direction(C_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_reset_pin(D_PIN_DEFAULT);
    gpio_set_direction(D_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_reset_pin(E_PIN_DEFAULT);
    gpio_set_direction(E_PIN_DEFAULT, GPIO_MODE_OUTPUT);

    gpio_reset_pin(R1_PIN_DEFAULT);
    gpio_set_direction(R1_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_reset_pin(G1_PIN_DEFAULT);
    gpio_set_direction(G1_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_reset_pin(B1_PIN_DEFAULT);
    gpio_set_direction(B1_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_reset_pin(R2_PIN_DEFAULT);
    gpio_set_direction(R2_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_reset_pin(G2_PIN_DEFAULT);
    gpio_set_direction(G2_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_reset_pin(B2_PIN_DEFAULT);
    gpio_set_direction(B2_PIN_DEFAULT, GPIO_MODE_OUTPUT);

    gpio_reset_pin(CLK_PIN_DEFAULT);
    gpio_set_direction(CLK_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_reset_pin(LAT_PIN_DEFAULT);
    gpio_set_direction(LAT_PIN_DEFAULT, GPIO_MODE_OUTPUT);
    gpio_reset_pin(OE_PIN_DEFAULT);
    gpio_set_direction(OE_PIN_DEFAULT, GPIO_MODE_OUTPUT);

    // **Setze alle Signale in einen definierten Zustand**
    gpio_set_level(OE_PIN_DEFAULT, 0);  // Panel aktivieren
    gpio_set_level(LAT_PIN_DEFAULT, 0); // Latch auf neutral
    gpio_set_level(CLK_PIN_DEFAULT, 0); // Clock auf neutral

    ESP_LOGI("DEBUG", "Start LED Test");

    // **Test: Setze alle Zeilen-Pins manuell auf 1, dann wieder auf 0**
    ESP_LOGI("DEBUG", "Setze A, B, C, D, E auf 1");
    gpio_set_level(A_PIN_DEFAULT, 1);
    gpio_set_level(B_PIN_DEFAULT, 1);
    gpio_set_level(C_PIN_DEFAULT, 1);
    gpio_set_level(D_PIN_DEFAULT, 1);
    gpio_set_level(E_PIN_DEFAULT, 1);
    vTaskDelay(pdMS_TO_TICKS(1000));

    ESP_LOGI("DEBUG", "Setze A, B, C, D, E auf 0");
    gpio_set_level(A_PIN_DEFAULT, 0);
    gpio_set_level(B_PIN_DEFAULT, 0);
    gpio_set_level(C_PIN_DEFAULT, 0);
    gpio_set_level(D_PIN_DEFAULT, 0);
    gpio_set_level(E_PIN_DEFAULT, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));

    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t c = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    while (1)
    {
        switch (c) 
        {
            case 0:
                r = 1;
                g = 0;
                b = 0;
                break;
            case 1:
                r = 0;
                g = 1;
                b = 0;
                break;
            case 2:
                r = 0;
                g = 0;
                b = 1;
                break;
        }
        c = (c + 1) % 3;
        ESP_LOGI(__FUNCTION__, "Color: %d", c);
        
        // **Anzeige aktivieren**
        gpio_set_level(OE_PIN_DEFAULT, 0);
        ESP_LOGI(__FUNCTION__, "OE-Pin: %d", gpio_get_level(OE_PIN_DEFAULT));

        set_pixel(x, y, r, g, b);

        x = (x + 1) % MATRIX_WIDTH;
        if (x == 0)
        { 
            y = (y + 1) % MATRIX_HEIGHT;
        }

        ESP_LOGI(__FUNCTION__, "x: %d, y: %d", x, y);


        vTaskDelay(pdMS_TO_TICKS(20)); // Verlangsamen für Debugging
    }
}
