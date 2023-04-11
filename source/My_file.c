/** @file My_file.c
*    @brief my header file
*    @date 07-March-2023
*    @version 07.04.23
* 
 *   This file contains:
 *   - API Functions
 *   .
 */
#include "My_file.h"
/*
 *  Button polling every 30ms.
 *
 *  Return true when "unpressed"
 *  or dnt return anything.
 *
 *  "unpressed" while after 6+ positive mathces
 *  go 6+ negative matches and butt_state == 0
 *
 *  Duty (6+6)*30 = 360ms
 *
 *  // - uncom for debug
 */

void mode_change(mode_t mode)
{
    led_index = -1;
    gioSetPort(gioPORTA, 0xFFFFFFFF);
    rtiStopCounter(rtiCOMPARE0);
    switch (mode)
    {
    case led_wave:
        rtiSetPeriod(rtiCOMPARE0, 2500000);
        break;
    case led_pong:
        rtiSetPeriod(rtiCOMPARE0, 2000000);
        break;
    case led_binary_increment:
        rtiSetPeriod(rtiCOMPARE0, 1000000);
        break;
    default:
        mode = led_wave;
        break;
    }
    rtiStartCounter(rtiCOMPARE0);
}

void wave(void){

    if (calc_direction == 1)
    {
        led_index++;
    }
    else
    {
        led_index--;
    }

    if (led_index > 3)
    {
        calc_direction = !calc_direction;
    }

    if (led_index < 0)
    {
        calc_direction = !calc_direction;
    }
    gioToggleBit(gioPORTA, led_index);
}

void pong(void){

    gioSetPort(gioPORTA, 0xFFFFFFFF);
    if (calc_direction == 1)
    {
        led_index++;
    }
    else
    {
        led_index--;
    }
    if (led_index > 3)
    {
        calc_direction = !calc_direction;
    }
    if (led_index < 0)
    {
        calc_direction = !calc_direction;
    }
    gioToggleBit(gioPORTA, led_index);
}

void b_inc(void){
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



boolean button_state(void)
{
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


    if (gioGetBit(hetPORT1, 12) == butt_state
            && rtiGetCurrentTick(rtiCOMPARE0) - last_tick >= 300000)
    {
        last_tick = rtiGetCurrentTick(rtiCOMPARE0);
        count_bounce++;
//gioToggleBit(gioPORTA, 0);            // 0 bit - bounce
    }
    if (count_bounce > 6)
    {
        if (butt_state == 0)
        {
            butt_state = 1;
            count_bounce = 0;
//gioSetBit(gioPORTA,1,!butt_state);    // 1 bit - set ON if pressed
        }
        else
        {
            butt_state = 0;
            count_bounce = 0;
//gioSetBit(gioPORTA,1,!butt_state);    // 1 bit - set OFF if dnt pressed
//gioToggleBit(gioPORTA,2);             // 2 bit - flag on/of
            return 1;
        }
    }
    return 0;
}


