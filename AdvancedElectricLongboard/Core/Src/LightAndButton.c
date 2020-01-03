/*
 * LightAndButton.c
 *
 *  Created on: 02.01.2020
 *      Author: Dominic
 */

#include "LightAndButton.h"

void Button_Init(void)
{
	/*Configure GPIO pins : PA5 */
	InitStruct_Button.Pin       = Button_Pin;
	InitStruct_Button.Mode      = GPIO_MODE_INPUT;
	InitStruct_Button.Pull      = GPIO_PULLDOWN;
	InitStruct_Button.Speed     = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &InitStruct_Button);
}

void Lights_Init(void)
{
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA,Lights_Pins_PORTA, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,Lights_Pins_PORTB, GPIO_PIN_SET);

	/*Configure GPIO pins : PA8 PA9 PA10 */
	InitStruct_Button.Pin       = Lights_Pins_PORTA;
	InitStruct_Button.Mode      = GPIO_MODE_OUTPUT_PP;
	InitStruct_Button.Pull      = GPIO_NOPULL;
	InitStruct_Button.Speed     = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &InitStruct_Lights_PortA);

	/*Configure GPIO pins : PB15 */
	InitStruct_Button.Pin       = Lights_Pins_PORTB;
	InitStruct_Button.Mode      = GPIO_MODE_OUTPUT_PP;
	InitStruct_Button.Pull      = GPIO_NOPULL;
	InitStruct_Button.Speed     = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &InitStruct_Lights_PortB);
}

void TurnOnLights(void)
{
	if(getReverse())
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5|GPIO_PIN_9,0);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,1);
		direction = true;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5|GPIO_PIN_9,1);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,0);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,0);
		direction = false;
	}
}
void TurnOffLights(void)
{
	HAL_GPIO_WritePin(GPIOA,Lights_Pins_PORTA, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,Lights_Pins_PORTB, GPIO_PIN_SET);
	direction = false;
}

void CheckLightDirection(void)
{
	bool val = getReverse();
	if(direction != val)
	{
		HAL_GPIO_TogglePin(GPIOA,Lights_Pins_PORTA);
		HAL_GPIO_TogglePin(GPIOA,Lights_Pins_PORTB);
		direction = val;
	}

}
