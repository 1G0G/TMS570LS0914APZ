/** @file My_file.h
 *   @brief my header file
 *   @date 07-March-2023
 *   @version 07.04.23
 *
 *   This file contains:
 *   - Definitions
 *   - Types
 *   - Interface Prototypes
 */

#ifndef my_file_h
#define my_file_h

#include "rti.h"
#include "gio.h"
#include "het.h"

#ifdef __cplusplus
extern "C" {
#endif

static boolean  butt_state = 0, calc_direction = 0;
static uint32_t count_bounce = 0, last_tick = 0, port = 0;
static int8_t   led_index = 0;

typedef enum mode_name
{
    led_wave = 0,
    led_pong = 1,
    led_binary_increment = 2,
} mode_t;

static mode_t mode = led_wave;

void mode_change(mode_t mode);
boolean button_state(void);
void wave(void);
void pong(void);
void b_inc(void);

#ifdef __cplusplus
}
#endif /*extern "C" */

#endif
