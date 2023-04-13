/** @file My_file.c
 *    @brief my header file
 *    @date 07-March-2023
 *    @version 07.04.23
 *
 *   This file contains:
 *   - API Functions
 *   .
 */
#include <led_blink.h>

static boolean calc_direction = 0;
static int8_t led_index = 0;
mode_t mode = ledWave;

/*  Change mode period
 *  Set all bits APORT to 0
 *  Reset led_index from ChangeLedIndex
 *  interrupts rtiNotification for the duration of the function
 */
const int led_period_ms[] = { [ledWave]=250, [ledPong]=200, [ledBinaryIncrement
                                      ]=200 };

void mode_change(mode_t mode)
{
    led_index = -1;
    gioSetPort(gioPORTA, 0xFFFFFFFF);
    rtiDisableNotification(rtiNOTIFICATION_COMPARE0);
    rtiStopCounter(rtiCOUNTER_BLOCK0);
    if (mode < LedOut)
    {
        rtiSetPeriod(rtiCOMPARE0, led_period_ms[mode] * 10000);
    }
    rtiStartCounter(rtiCOUNTER_BLOCK0);
    rtiEnableNotification(rtiNOTIFICATION_COMPARE0);
}

/*
 * Increment from 0 to 3
 * and then reverce
 */
static void ChangeLedIndex(void)
{
    if (calc_direction)
    {
        led_index++;
    }
    else
    {
        led_index--;
    }

    if (led_index > 3 || led_index < 0)
    {
        calc_direction = !calc_direction;
    }
}

/*
 * Wave led mode
 * from 1-4 led (0-3bit)
 */
void wave(void)
{
    ChangeLedIndex();
    gioToggleBit(gioPORTA, led_index);
}

/*
 * Ping-pong led mode
 * from 1-4 led (0-3bit)
 * only 1 led at tick ON
 */
void pong(void)
{

    gioSetPort(gioPORTA, 0xFFFFFFFF);
    ChangeLedIndex();
    gioToggleBit(gioPORTA, led_index);
}

/*
 * Binary increment led mode
 * 4bit number based on 1-4led(0-3bit)
 * from 0 to 15
 */
static uint32_t port = 0;
void b_inc(void)
{
    gioSetPort(gioPORTA, 0b00000000);
    if (port <= 0b00001111)
    {
        port++;
    }
    else
    {
        port = 0b0000000;
    }
    gioSetPort(gioPORTA, ~port);
}

/*
 *  Button polling every 30ms.
 *
 *  Return true when "unpressed"
 *  "unpressed" while after 6+ positive mathces
 *  go 6+ negative matches and butt_state == 1
 *
 *  Duty (6+6)*30 = 360ms
 *
 */
static uint32_t butt_state = 0, count_bounce = 0, last_tick = 0;
boolean isButtonClick(void)
{
    if (gioGetBit(hetPORT1, button_pin_bit) == butt_state
            && rtiGetCurrentTick(rtiCOMPARE0) - last_tick
                    >= button_poling_ms * 10000)
    {
        last_tick = rtiGetCurrentTick(rtiCOMPARE0);
        count_bounce++;
    }
    if (count_bounce > button_bounce)
    {
        if (butt_state == 0)
        {
            butt_state = 1;
            count_bounce = 0;
        }
        else
        {
            butt_state = 0;
            count_bounce = 0;
            return 1;
        }
    }
    return 0;
}

