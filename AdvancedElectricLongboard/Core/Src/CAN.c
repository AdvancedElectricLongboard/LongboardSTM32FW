/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : CAN.h
  * @brief          : Header for Main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_C
#define __CAN_C

#ifdef __cplusplus
extern "C" {
#endif

#include "CAN.h"
#include "stm32f1xx_hal.h"

//privat variables
static bool reverse = false;
static uint32_t voltage = 0;
static int32_t throttle = 0;
static int32_t brakeCurrent = 0;
static int32_t brakeCurrentRel = 0;
static int32_t RPM = 0;

static const uint32_t FAKE_RPM = 0xAABBCCDD;//8000; //[1]
static const uint16_t FAKE_CURRENT = 10000; //mA
static const uint16_t FAKE_DUTY = 100; //%


void CAN_INIT(CAN_HandleTypeDef *hcan)
{
	CanFilterConfig.FilterBank= 0;
	CanFilterConfig.FilterMode= CAN_FILTERMODE_IDMASK;
	CanFilterConfig.FilterScale= CAN_FILTERSCALE_32BIT;
	CanFilterConfig.FilterIdHigh= 0x0000;
	CanFilterConfig.FilterIdLow= 0x0000;
	CanFilterConfig.FilterMaskIdHigh= 0x0000;
	CanFilterConfig.FilterMaskIdLow= 0x0000;
	CanFilterConfig.FilterFIFOAssignment= CAN_RX_FIFO0;
	CanFilterConfig.FilterActivation= ENABLE;
	CanFilterConfig.SlaveStartFilterBank = 14;

	HAL_CAN_ConfigFilter(hcan, &CanFilterConfig);

	TxHeader.StdId= 0;	//1
	TxHeader.ExtId = 0;	//1
	TxHeader.RTR= CAN_RTR_DATA;
	TxHeader.IDE= 0;	//4
	TxHeader.DLC= 8;
	TxHeader.TransmitGlobalTime = DISABLE;
}

void setBatteryVoltage(uint32_t val) //mV
{
	voltage = val;
}

uint32_t getBatteryVoltage() //mV
{
	return voltage;
}

void setReverse(bool rev)
{
	reverse = rev;
}

bool getReverse()
{
	return reverse;
}

void setThrottle(int32_t val)	//Percentage
{
	throttle = val;
}

int32_t getThrottle()
{
	return throttle;
}

void setRPM(int32_t rpm)
{
	RPM = rpm;
}

int32_t getRPM()
{
	return RPM;
}

void setBrakeCurrent(int32_t val)	//mA
{
	brakeCurrent = val;
}

int32_t getBrakeCurrent()
{
	return brakeCurrent;
}

void setBrakeCurrentRel(int32_t val)	//mA
{
	brakeCurrentRel = val;
}

int32_t getBrakeCurrentRel()
{
	return -60000;
}


void setCanDestinationIDE(uint32_t ide)
{
	TxHeader.IDE = ide;
}

void setCanDestinationEID(uint32_t eid)
{
	TxHeader.ExtId = eid;
}

int32_t CAN_SEND_STATUS(CAN_HandleTypeDef *hcan)
{
	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0)
		return 0;

	TxHeader.ExtId = ((TxHeader.ExtId&0xFF)|((CAN_PACKET_STATUS)<<8));
	TxHeader.StdId = ((TxHeader.StdId&0xFF)|((CAN_PACKET_STATUS)<<8));

	int32_t send_index = 0;
	for(size_t i = 0; i < 8;i++)
		TxData[i] = 0x00;
	buffer_append_int32(TxData,FAKE_RPM,&send_index);
	buffer_append_int16(TxData,FAKE_CURRENT,&send_index);
	buffer_append_int16(TxData,FAKE_DUTY,&send_index);

	if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailbox) != HAL_OK)
	{
		Error_Handler();
	}
	return 1;
}

int32_t CAN_SEND_BRAKE_CURRENT(CAN_HandleTypeDef *hcan,float bcurrent)	//in Ampere
{
	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0)
		return 0;

	TxHeader.ExtId &= ((TxHeader.ExtId&0xFF)|(CAN_PACKET_SET_CURRENT_BRAKE<<8));

	int32_t send_index = 0;
	buffer_append_int32(TxData,(int32_t)(bcurrent * 1000),&send_index);

	if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailbox) != HAL_OK)
	{
		Error_Handler();
	}
	return 1;
}

bool CAN_RECEIVED_PACKAGE(CAN_HandleTypeDef *hcan)	//in Ampere
{
	status = HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);

	setCanDestinationIDE(RxHeader.IDE);
	setCanDestinationEID((RxHeader.ExtId&&0xFF00)>>8);


	if(status == HAL_OK)
	{
		uint8_t cmd = ((RxHeader.ExtId&0xFF00)>>8);
		int32_t index = 0;

		switch(cmd)
		{
			case CAN_PACKET_SET_CURRENT_BRAKE:
				index = 0;
				setBrakeCurrent(buffer_get_int32(RxData,&index));
				break;

			case CAN_PACKET_SET_CURRENT_BRAKE_REL:
				index = 0;
				setBrakeCurrentRel(buffer_get_int32(RxData,&index));
				break;

			case CAN_PACKET_SET_BATTERY_VOLTAGE:
				index = 0;
				setBatteryVoltage(buffer_get_uint32(RxData,&index));
				break;

			case CAN_PACKET_SET_THROTTLE:
				index = 0;
				setThrottle(buffer_get_int32(RxData,&index));
				break;

			case CAN_PACKET_SET_REVERSE:
				if(RxData[1]&0x01)
					setReverse(true);
				else
					setReverse(false);
				break;
			case CAN_PACKET_STATUS_5:
				id = (RxData[7]<<8) | (RxData[6]);
				break;
			case CAN_PACKET_SET_RPM:
				index = 0;
				setRPM(buffer_get_int32(RxData,&index));
				break;
		}
		return true;
	}

	return false;
}


#ifdef __cplusplus
}
#endif

#endif /* __CAN_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
