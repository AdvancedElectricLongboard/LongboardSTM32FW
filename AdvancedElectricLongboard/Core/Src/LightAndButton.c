/*
 * LightAndButton.c
 *
 *  Created on: 02.01.2020
 *      Author: Dominic
 */

#include "LightAndButton.h"
#include "CAN.h"

struct BoardLight Head = {GPIOB,14,15,false,false,AllOn,AllOff,RedOn,WhiteOn};
struct BoardLight Tail = {GPIOA,8,9,false,false,AllOn,AllOff,RedOn,WhiteOn};

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
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15|GPIO_PIN_14, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

	  /*Configure GPIO pin : PB15 PB14 */
	  GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  /*Configure GPIO pins : PA8 PA9 */
	  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void AllOn(struct BoardLight Light)
{
	HAL_GPIO_WritePin(Light.Port, Light.Red_Pin,0);	//negativ logic
	HAL_GPIO_WritePin(Light.Port, Light.White_Pin,0);
	Light.RedState = true;
	Light.WhiteState = true;
}

void AllOff(struct BoardLight Light)
{
	HAL_GPIO_WritePin(Light.Port, Light.Red_Pin,1);	//negativ logic
	HAL_GPIO_WritePin(Light.Port, Light.White_Pin,1);
	Light.RedState = false;
	Light.WhiteState = false;
}

void RedOn(struct BoardLight Light)
{
	HAL_GPIO_WritePin(Light.Port, Light.Red_Pin,0);	//negativ logic
	HAL_GPIO_WritePin(Light.Port, Light.White_Pin,1);
	Light.RedState = true;
	Light.WhiteState = false;
}

void WhiteOn(struct BoardLight Light)
{
	HAL_GPIO_WritePin(Light.Port, Light.Red_Pin,1);	//negativ logic
	HAL_GPIO_WritePin(Light.Port, Light.White_Pin,0);
	Light.RedState = false;
	Light.WhiteState = true;
}

void CheckDirection(void)
{
	if(getReverse())
		direction = reverse;
	else
		direction = forward;
}

void TurnOnLights(void)
{
	if(direction == reverse)
	{
		Head.RedOn(Head);		//Tail
		Tail.WhiteOn(Tail);
	}
	else
	{
		Head.WhiteOn(Head);		//Tail
		Tail.RedOn(Tail);
	}
}
void TurnOffLights(void)
{
	Tail.AllOff(Tail);
	Head.AllOff(Head);
}

void UpdateLights(void)
{
	bool val = getReverse();
	if(direction != val)
	{
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_9);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);
		direction = val;
	}

}

void CheckflashBrakelight()
{
	if(direction == forward)
		BrakeLight = Tail;
	else
		BrakeLight = Head;

	if(getThrottle()<-5)
	{
		BrakeLight.AllOff(BrakeLight);
		flashTimer = HAL_GetTick();
		if(flashTimer+10 < HAL_GetTick())
			BrakeLight.RedOn(BrakeLight);
	}
	else
		BrakeLight.RedOn(BrakeLight);
}
