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
#include "rti.h"
#include "gio.h"
#include "het.h"
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
//    led_index = -1;
    gioSetPort(gioPORTA, 0xFFFFFFFF);
    switch (mode)
    {
    case led_wave:
        rtiSetPeriod(rtiCOMPARE0, 2500000);
        break;
    case led_flashing:
        rtiSetPeriod(rtiCOMPARE0, 2000000);
        break;
    case led_binary_increment:
        rtiSetPeriod(rtiCOMPARE0, 1000000);
        break;
    default:
        mode = led_wave;
        break;
    }
    rtiResetCounter(rtiCOUNTER_BLOCK0);
}


static boolean  butt_state  = 0;
static uint32_t count_bounce= 0;
static uint32_t last_tick   = 0;


boolean button_state(void)
{
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
