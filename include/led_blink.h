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



/*
 * duty=button_poling_ms*button_bounce*2 - ms
 * duty=30*6*2=360ms
 */
boolean isButtonClick(void);
#define button_poling_ms    30
#define button_bounce       6
#define button_pin_bit      12


typedef enum Led_mode_t
{
    ledWave,
    ledPong,
    ledBinaryIncrement,
    LedOut
} mode_t;


void ModeChange(mode_t mode);
void Wave(void);
void Pong(void);
void BinaryInc(void);




#ifdef __cplusplus
}
#endif /*extern "C" */

#endif
