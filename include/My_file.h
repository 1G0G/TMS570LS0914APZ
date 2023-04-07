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
#include "sys_common.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum mode_name
{
	led_wave = 0,
	led_flashing = 1,
	led_binary_increment = 2,
}mode_t;

void mode_change(mode_t mode);
boolean button_state(void);


#ifdef __cplusplus
}
#endif /*extern "C" */

#endif
