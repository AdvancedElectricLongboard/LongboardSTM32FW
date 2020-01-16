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
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15|GPIO_PIN_14, GPIO_PIN_SET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);

	  /*Configure GPIO pin : PB15 PB14 */
	  GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  /*Configure GPIO pins : PA8 PA9 */
	  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void TurnOnLights(void)
{
	if(getReverse())
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,0);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,0);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,1);
		direction = true;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,1);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,0);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,0);
		direction = false;
	}
}
void TurnOffLights(void)
{
	HAL_GPIO_WritePin(GPIOA,Lights_Pins_PORTA, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,Lights_Pins_PORTB, GPIO_PIN_SET);
	direction = false;
}

void CheckLightDirection(void)
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
