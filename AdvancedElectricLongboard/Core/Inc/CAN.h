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
#ifndef __CAN_H
#define __CAN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stdbool.h"
#include "buffer.h"


#define CAN_ID 1

// CAN commands
typedef enum {
	CAN_PACKET_SET_DUTY = 0,
	CAN_PACKET_SET_CURRENT,
	CAN_PACKET_SET_CURRENT_BRAKE,
	CAN_PACKET_SET_RPM,
	CAN_PACKET_SET_POS,
	CAN_PACKET_FILL_RX_BUFFER,
	CAN_PACKET_FILL_RX_BUFFER_LONG,
	CAN_PACKET_PROCESS_RX_BUFFER,
	CAN_PACKET_PROCESS_SHORT_BUFFER,
	CAN_PACKET_STATUS,
	CAN_PACKET_SET_CURRENT_REL,
	CAN_PACKET_SET_CURRENT_BRAKE_REL,
	CAN_PACKET_SET_CURRENT_HANDBRAKE,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
	CAN_PACKET_STATUS_2,
	CAN_PACKET_STATUS_3,
	CAN_PACKET_STATUS_4,
	CAN_PACKET_PING,
	CAN_PACKET_PONG,
	CAN_PACKET_DETECT_APPLY_ALL_FOC,
	CAN_PACKET_DETECT_APPLY_ALL_FOC_RES,
	CAN_PACKET_CONF_CURRENT_LIMITS,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS,
	CAN_PACKET_CONF_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_FOC_ERPMS,
	CAN_PACKET_CONF_STORE_FOC_ERPMS,
	CAN_PACKET_STATUS_5,
	CAN_PACKET_SET_BATTERY_VOLTAGE,
	CAN_PACKET_SET_THROTTLE,
	CAN_PACKET_SET_REVERSE
} CAN_PACKET_ID;

CAN_FilterTypeDef CanFilterConfig;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;
static const uint32_t FAKE_RPM = 8000; //[1]
static const uint16_t FAKE_CURRENT = 10000; //mA
static const uint16_t FAKE_DUTY = 50; //%

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

	TxHeader.StdId= 0;
	TxHeader.ExtId = CAN_ID;
	TxHeader.RTR= CAN_RTR_DATA;
	TxHeader.IDE= CAN_ID_STD;
	TxHeader.DLC= 2;
	TxHeader.TransmitGlobalTime = DISABLE;
}

int32_t CAN_SEND_STATUS(CAN_HandleTypeDef *hcan)
{
	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0)
		return 0;

	TxHeader.ExtId &= ((TxHeader.ExtId&0xFF)|(CAN_PACKET_STATUS<<8));

	int32_t send_index = 0;
	buffer_append_int32(TxData,FAKE_RPM,&send_index);
	buffer_append_int16(TxData,FAKE_CURRENT,&send_index);
	buffer_append_int16(TxData,FAKE_CURRENT,&send_index);

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
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	{
		uint8_t cmd = ((RxHeader.ExtId&0xFF00)>>8);

		switch(cmd)
		{
			case CAN_PACKET_SET_CURRENT_BRAKE:
				break;

			case CAN_PACKET_SET_BATTERY_VOLTAGE:
				break;

			case CAN_PACKET_SET_THROTTLE:
				break;

			case CAN_PACKET_SET_REVERSE:
				break;
		}
		return true;
	}
	return false;
}


#ifdef __cplusplus
}
#endif

#endif /* __CAN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
