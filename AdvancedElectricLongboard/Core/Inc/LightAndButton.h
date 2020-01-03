/*
 * LightAndButton.h
 *
 *  Created on: 02.01.2020
 *      Author: Dominic
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_LIGHTANDBUTTON_H_
#define INC_LIGHTANDBUTTON_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stdbool.h"

// Button output pin
#define Button_Pin GPIO_PIN_4
// Lights output pins port A
#define Lights_Pins_PORTA (GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10)
//Lights output pins port B
#define Lights_Pins_PORTB GPIO_PIN_15


GPIO_InitTypeDef  InitStruct_Button;
GPIO_InitTypeDef  InitStruct_Lights_PortA;
GPIO_InitTypeDef  InitStruct_Lights_PortB;

static bool direction;


void Button_Init(void);
void Lights_Init(void);






#endif /* INC_LIGHTANDBUTTON_H_ */
