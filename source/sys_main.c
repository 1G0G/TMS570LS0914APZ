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
/* USER CODE END */

/** @fn void main(void)
 *   @brief Application main function
 *   @note This function is empty by default.
 *
 *   This function is called after startup.
 *   The user can use this function to implement the application.
 */

/* USER CODE BEGIN (2) */
char LEDindex = 0;
char MODE = 0;
bool LEDcount = 0;
uint32 port = 0;
uint32 cont_bounce = 0;
uint32 last_tick = 0;
bool BUTTstate;


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
        if (gioGetBit(hetPORT1, 12) != BUTTstate){
            BUTTstate=0;
            cont_bounce++;
        last_tick = rtiGetCurrentTick(rtiCOMPARE0);
        }
        if(!BUTTstate&&(rtiGetCurrentTick(rtiCOMPARE0)-last_tick>3000000))
        {
            BUTTstate=1;
            mode_change();
        }


/*
        if (gioGetBit(hetPORT1, 12) != BUTTstate)
        {

            if (rtiGetCurrentTick(rtiCOMPARE0) - last_tick >= 1000000)
            {
                last_tick = rtiGetCurrentTick(rtiCOMPARE0);
                if (gioGetBit(hetPORT1, 12) != BUTTstate)
                {
                    BUTTstate = gioGetBit(hetPORT1, 12);
                    mode_change();
                }
            }
        }
*/
    }

    /* USER CODE END */
}

/* USER CODE BEGIN (4) */
void rtiNotification(uint32 notification)
{

    if (MODE == 0)
    {
        if (LEDcount == 1)
        {
            LEDindex++;
        }
        else
        {
            LEDindex--;
        }
        if (LEDindex > 3)
        {
            LEDcount = !LEDcount;
        }
        if (LEDindex < 0)
        {
            LEDcount = !LEDcount;
        }
        gioToggleBit(gioPORTA, LEDindex);
    }

    if (MODE == 1)
    {
        gioSetPort(gioPORTA, 0xFFFFFFFF);
        if (LEDcount == 1)
        {
            LEDindex++;
        }
        else
        {
            LEDindex--;
        }
        if (LEDindex > 3)
        {
            LEDcount = !LEDcount;
        }
        if (LEDindex < 0)
        {
            LEDcount = !LEDcount;
        }
        gioToggleBit(gioPORTA, LEDindex);
    }
    if (MODE == 2)
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
}

void mode_change()
{
    if (MODE > 2) { MODE = 0;}
    MODE++;
    LEDindex = -1;
    gioSetPort(gioPORTA, 0xFFFFFFFF);
    if (MODE == 0)
    {
        rtiSetPeriod(rtiCOMPARE0, 2500000);
    }
    if (MODE == 1)
    {
        rtiSetPeriod(rtiCOMPARE0, 2000000);
    }
    if (MODE == 2)
    {
        rtiSetPeriod(rtiCOMPARE0, 1000000);
    }
    rtiResetCounter(rtiCOUNTER_BLOCK0);
}
/* USER CODE END */
