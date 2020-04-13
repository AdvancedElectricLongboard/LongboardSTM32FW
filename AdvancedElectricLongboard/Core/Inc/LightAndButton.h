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
#define Lights_Pins_PORTA (GPIO_PIN_9 | GPIO_PIN_8)

//Lights output pins port B
#define Lights_Pins_PORTB (GPIO_PIN_15 | GPIO_PIN_14)

enum Direction {forward,reverse};

struct BoardLight
{
	GPIO_TypeDef *Port;
	uint16_t Red_Pin;
	uint16_t White_Pin;
	bool RedState;	//positive Logic
	bool WhiteState; //positive Logic
	void (*AllOn)(struct BoardLight Light);
	void (*AllOff)(struct BoardLight Light);
	void (*RedOn)(struct BoardLight Light);
	void (*WhiteOn)(struct BoardLight Light);
};

void AllOn(struct BoardLight Light);

void AllOff(struct BoardLight Light);

void RedOn(struct BoardLight Light);

void WhiteOn(struct BoardLight Light);

GPIO_InitTypeDef  InitStruct_Button;

static bool direction;

void CheckDirection(void);
void Button_Init(void);
void Lights_Init(void);
void UpdateLights(void);
void TurnOffLights(void);
void TurnOnLights(void);

void CheckflashBrakelight();

uint32_t flashTimer;
struct BoardLight BrakeLight;

#endif /* INC_LIGHTANDBUTTON_H_ */
