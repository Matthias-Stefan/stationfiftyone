#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CLOCKGUSTO_NUM_LEDS 114
#define CLOCKGUSTO_BYTES_PER_LED 3

typedef enum _clock_word_t
{
    CLOCK_WORD_ES,                  // 0b 00000000 00000000 00000000 00000001   
    CLOCK_WORD_IST,                 // 0b 00000000 00000000 00000000 00000010
    CLOCK_WORD_FUENF_1,             // 0b 00000000 00000000 00000000 00000100
    CLOCK_WORD_ZEHN_1,              // 0b 00000000 00000000 00000000 00001000
    CLOCK_WORD_ZWANZIG,             // 0b 00000000 00000000 00000000 00010000
    CLOCK_WORD_DREIVIERTEL,         // 0b 00000000 00000000 00000000 00100000
    CLOCK_WORD_VIERTEL,             // 0b 00000000 00000000 00000000 01000000
    CLOCK_WORD_VOR,                 // 0b 00000000 00000000 00000000 10000000
    CLOCK_WORD_NACH,                // 0b 00000000 00000000 00000001 00000000
    CLOCK_WORD_HALB,                // 0b 00000000 00000000 00000010 00000000
    CLOCK_WORD_EIN,                 // 0b 00000000 00000000 00000100 00000000
    CLOCK_WORD_EINS,                // 0b 00000000 00000000 00001000 00000000
    CLOCK_WORD_ZWEI,                // 0b 00000000 00000000 00010000 00000000
    CLOCK_WORD_DREI,                // 0b 00000000 00000000 00100000 00000000
    CLOCK_WORD_VIER,                // 0b 00000000 00000000 01000000 00000000
    CLOCK_WORD_FUENF_2,             // 0b 00000000 00000000 10000000 00000000
    CLOCK_WORD_SECHS,               // 0b 00000000 00000001 00000000 00000000
    CLOCK_WORD_SIEBEN,              // 0b 00000000 00000010 00000000 00000000
    CLOCK_WORD_ACHT,                // 0b 00000000 00000100 00000000 00000000
    CLOCK_WORD_NEUN,                // 0b 00000000 00001000 00000000 00000000
    CLOCK_WORD_ZEHN_2,              // 0b 00000000 00010000 00000000 00000000
    CLOCK_WORD_ELF,                 // 0b 00000000 00100000 00000000 00000000
    CLOCK_WORD_ZWOELF,              // 0b 00000000 01000000 00000000 00000000
    CLOCK_WORD_UHR,                 // 0b 00000000 10000000 00000000 00000000

    CLOCK_MINUTE_1,                 // 0b 00000001 00000000 00000000 00000000
    CLOCK_MINUTE_2,                 // 0b 00000010 00000000 00000000 00000000
    CLOCK_MINUTE_3,                 // 0b 00000100 00000000 00000000 00000000
    CLOCK_MINUTE_4,                 // 0b 00001000 00000000 00000000 00000000
    
    CLOCK_WORD_COUNT
} clock_word_t;

typedef struct _clock_word_str_t
{
    char* data;
    int16_t size;
} clock_word_str_t;

static clock_word_str_t clock_word_str[] = {
    { .data = "ES", .size = 2 }, 
    { .data = "IST", .size = 3 }, 
    { .data = "FÜNF1", .size = 5 }, 
    { .data = "ZEHN1", .size = 5 }, 
    { .data = "ZWANZIG", .size = 7 }, 
    { .data = "DREIVIERTEL", .size = 11 }, 
    { .data = "VIERTEL", .size = 7 }, 
    { .data = "VOR", .size = 3 }, 
    { .data = "NACH", .size = 4 }, 
    { .data = "HALB", .size = 4 },
    { .data = "EIN", .size = 3 }, 
    { .data = "EINS", .size = 4 }, 
    { .data = "ZWEI", .size = 4 }, 
    { .data = "DREI", .size = 4 }, 
    { .data = "VIER", .size = 4 },
    { .data = "FÜNF2", .size = 5 },
    { .data = "SECHS", .size = 5 },
    { .data = "SIEBEN", .size = 6 },
    { .data = "ACHT", .size = 4 },
    { .data = "NEUN", .size = 4 },
    { .data = "ZEHN2", .size = 5 },
    { .data = "ELF", .size = 3 },  
    { .data = "ZWOELF", .size = 6 }, 
    { .data = "UHR", .size = 3 }, 
    { .data = "min1", .size = 4 }, 
    { .data = "min2", .size = 4 }, 
    { .data = "min3", .size = 4 }, 
    { .data = "min4", .size = 4 },
};

typedef struct _clock_word_boundary_t
{
    uint16_t index;
    uint16_t size;  
} clock_word_boundary_t;

typedef struct _led_t
{
    bool on;
    bool locked;
} led_t;

typedef struct _clock_board_t
{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint32_t time_mask;
    bool flip;

    clock_word_boundary_t clock_word_boundary_table[CLOCK_WORD_COUNT];
    led_t leds[CLOCKGUSTO_NUM_LEDS];
} clock_board_t;

/** */
void clockgusto_startup();

/** */
void clockgusto_update();

/** */
void clockgusto_show();

/** */
void clockgusto_reset();





