#pragma once

#include <stdbool.h>

#define MATRIX_PANEL_WIDTH 96

#define MATRIX_PANEL_HEIGHT 48

#define MATRIX_PANEL_HEIGHT_HALF 24


typedef enum _rgb_e
{
    r,
    g,
    b,

    RGB_MAX
} rgb_e;

// unsigned int types.

/** @brief unsigned 8-bit integer */
typedef unsigned char u8;

/** @brief unsigned 16-bit integer */
typedef unsigned short u16;

/** @brief unsigned 32-bit integer */
typedef unsigned int u32;

/** @brief unsigned 64-bit integer */
typedef unsigned long long u64;

// signed int types.

/** @brief signed 8-bit integer */
typedef signed char s8;

/** @brief signed 16-bit integer */
typedef signed short s16;

/** @brief signed 32-bit integer */
typedef signed int s32;

/** @brief signed 64-bit integer */
typedef signed long long s64;

// floating point types

/** @brief 32-bit floating point number */
typedef float f32;

/** @brief 64-bit floating point number */
typedef double f64;

// boolean types

/** @brief 32-bit boolean type, used for APIs which require it */
typedef int b32;

/** @brief 8-bit boolean type */
typedef bool b8;