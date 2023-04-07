/** @file sys_main.c 
 *   @brief Application main file
 *   @date 11-Dec-2018
 *   @version 04.07.01
 *
 *   This file contains an empty main function,
 *   which can be used for the application.
 */

/* 
 * Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "gio.h"
#include "rti.h"
#include "het.h"
// #include "My_file.h"
/* USER CODE END */

/** @fn void main(void)
 *   @brief Application main function
 *   @note This function is empty by default.
 *
 *   This function is called after startup.
 *   The user can use this function to implement the application.
 */

/* USER CODE BEGIN (2) */
bool calc_direction = 0;
bool butt_state = 0;
int8_t led_index = 0;
uint8_t mode = 0;
uint32_t port = 0;
uint32_t count_bounce = 0;
uint32_t last_tick = 0;
/*
void mode_change(mode)
{
    led_index = -1;
    gioSetPort(gioPORTA, 0xFFFFFFFF);
    switch (mode)
    {
    case 0:
        rtiSetPeriod(rtiCOMPARE0, 2500000);
        break;
    case 1:
        rtiSetPeriod(rtiCOMPARE0, 2000000);
        break;
    case 2:
        rtiSetPeriod(rtiCOMPARE0, 1000000);
        break;
    default:
        mode = 0;
        break;
    }
    rtiResetCounter(rtiCOUNTER_BLOCK0);
}
*/

/*
bool button_state()
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
     *
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
*/

/* USER CODE END */

int main(void)
{
    /* USER CODE BEGIN (3) */

    hetInit();
    gioInit();
    rtiInit();
    rtiEnableNotification(rtiNOTIFICATION_COMPARE0);
    _enable_IRQ();
    rtiStartCounter(rtiCOUNTER_BLOCK0);

    while (1)
    {
        if (button_state() == true)
        {
            mode_change();
            mode++;
        }

    }

    /* USER CODE END */
}

/* USER CODE BEGIN (4) */
void rtiNotification(uint32 notification)
{
    switch (mode)
    {
    case 0:

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
        break;

    case 1:
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
        break;
    case 2:
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
        break;
    default:
        mode = 0;
        break;
    }
}

/* USER CODE END */
