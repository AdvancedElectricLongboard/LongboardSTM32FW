/*

  WS2812B CPU and memory efficient library

  Date: 28.9.2016

  Author: Martin Hubacek
  	  	  http://www.martinhubacek.cz
  	  	  @hubmartin

  Licence: MIT License

*/

#include <stdint.h>

#include "stm32f1xx_hal.h"
#include "ws2812b.h"
#include "visEffect.h"
#include "CAN.h"
#include "stdlib.h"


uint8_t CheckStateOfCharge(void);
void visClearStrip(uint8_t *frameBuffer, uint32_t frameBufferSize);

// RGB Framebuffers
uint8_t frameBuffer[3*WS2812B_NUMBER_OF_LEDS];
uint8_t frameBuffer2[3*WS2812B_NUMBER_OF_LEDS];

// defines

//test
extern int32_t testthrottle;
extern uint32_t testbat;

// Helper defines
#define newColor(r, g, b) (((uint32_t)(r) << 16) | ((uint32_t)(g) <<  8) | (b))
#define Red(c) ((uint8_t)((c >> 16) & 0xFF))
#define Green(c) ((uint8_t)((c >> 8) & 0xFF))
#define Blue(c) ((uint8_t)(c & 0xFF))


uint32_t Wheel(uint8_t WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return newColor(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return newColor(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return newColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void visThrottle(uint8_t *frameBuffer, uint32_t frameBufferSize)
{
	int32_t throttle_val = getThrottle()*100;
	uint8_t i,nLeds = 0;
	if(throttle_val > 0)
	{
		for(i = 0;i < 28;i++)
		{
			if(throttle_val <= ((i+1)*358) && throttle_val >= (i*358))
			{
				nLeds = i+1;
				break;
			}
		}
		if(nLeds <= 14)
		{
			for( i = 0; i < 28; i++)
			{
				if(i < nLeds)
				{
					frameBuffer[i*3 + 0] = 19*i;
					frameBuffer[i*3 + 1] = 255;
				}
				else
				{
					frameBuffer[i*3 + 0] = 0;
					frameBuffer[i*3 + 1] = 0;
				}
				frameBuffer[i*3 + 2] = 0;
			}
		}
		else
		{
			for( i = 0; i < 14; i++)
			{
				frameBuffer[i*3 + 0] = 19*i;
				frameBuffer[i*3 + 1] = 255;
				frameBuffer[i*3 + 2] = 0;
			}
			for( i = 14; i < 28; i++)
			{
				if(i < nLeds)
				{
					frameBuffer[i*3 + 0] = 255;
					frameBuffer[i*3 + 1] = 252-(i-14)*19;
				}
				if(i >= nLeds)
				{
					frameBuffer[i*3 + 0] = 0;
					frameBuffer[i*3 + 1] = 0;
				}
				frameBuffer[i*3 + 2] = 0;
			}
		}
	}
	else if(throttle_val < 0)
	{
		for(i = 0;i < 28;i++)
		{
			if(throttle_val >= ((i+1)*-358) && throttle_val <= (i*-358))
			{
				nLeds = i+1;
				break;
			}
		}
		for( i = 0; i < 28; i++)
		{
			frameBuffer[i*3 + 0] = 0;
			frameBuffer[i*3 + 1] = 0;
			if(i < nLeds)frameBuffer[i*3 + 2] = 255;
			if(i >= nLeds)frameBuffer[i*3 + 2] = 0;
		}
	}
	else
	{
		visClearStrip(frameBuffer, frameBufferSize);
	}
}

void visRainbow(uint8_t *frameBuffer, uint32_t frameBufferSize, uint32_t effectLength)
{
	uint32_t i;
	static uint8_t x = 0;

	x += 1;

	if(x == 256*5)
		x = 0;

	for( i = 0; i < frameBufferSize / 3; i++)
	{
		uint32_t color = Wheel(((i * 256) / effectLength + x) & 0xFF);

		frameBuffer[i*3 + 0] = color & 0xFF;
		frameBuffer[i*3 + 1] = color >> 8 & 0xFF;
		frameBuffer[i*3 + 2] = color >> 16 & 0xFF;
	}
}

void visWhiteWithLights(uint8_t *frameBuffer, uint32_t frameBufferSize, uint8_t visMaxBrightness)
{

	uint32_t i;
	if(!getReverse())
	{
		for( i = 0; i < 4; i++)
		{
			frameBuffer[i*3 + 0] = visMaxBrightness;
			frameBuffer[i*3 + 1] = 0;
			frameBuffer[i*3 + 2] = 0;
		}
		for( i = 4; i < ((frameBufferSize) / 3); i++)
		{
				frameBuffer[i*3 + 0] = visMaxBrightness;
				frameBuffer[i*3 + 1] = visMaxBrightness;
				frameBuffer[i*3 + 2] = visMaxBrightness;
		}

	}
	else
	{

		for(i = (frameBufferSize / 3)-4;i < frameBufferSize/3;i++)
		{
			frameBuffer[i*3 + 0] = visMaxBrightness;
			frameBuffer[i*3 + 1] = 0;
			frameBuffer[i*3 + 2] = 0;
		}
		for( i = 0; i < ((frameBufferSize) / 3)-4; i++)
		{
				frameBuffer[i*3 + 0] = visMaxBrightness;
				frameBuffer[i*3 + 1] = visMaxBrightness;
				frameBuffer[i*3 + 2] = visMaxBrightness;
		}
	}

}

void visWhite(uint8_t *frameBuffer, uint32_t frameBufferSize, uint8_t maxBirghtness)
{
	uint32_t i;
	for( i = 0; i < (frameBufferSize / 3); i++)
	{

			frameBuffer[i*3 + 0] = maxBirghtness;
			frameBuffer[i*3 + 1] = maxBirghtness;
			frameBuffer[i*3 + 2] = maxBirghtness;

	}
}
void visBatteryStatus(uint8_t *frameBuffer, uint32_t frameBufferSize, uint8_t MaxBrightness)
{
	uint32_t batSoC = CheckStateOfCharge();
	uint32_t i;
	for( i = 0; i < batSoC; i++)
	{

		frameBuffer[i*3 + 0] = 0;
		frameBuffer[i*3 + 1] = MaxBrightness;
		frameBuffer[i*3 + 2] = 0;

	}
	for( i = batSoC; i < ((frameBufferSize) / 3); i++)
	{
		frameBuffer[i*3 + 0] = MaxBrightness/2;
		frameBuffer[i*3 + 1] = 0;
		frameBuffer[i*3 + 2] = 0;
	}
}


//void visDots(uint8_t *frameBuffer, uint32_t frameBufferSize, uint32_t random, uint32_t fadeOutFactor)
//{
//	uint32_t i;
//	for( i = 0; i < frameBufferSize / 3; i++)
//	{
//
//		if(rand() % random == 0)
//		{
//			frameBuffer[i*3 + 0] = 255;
//			frameBuffer[i*3 + 1] = 255;
//			frameBuffer[i*3 + 2] = 255;
//		}
//
//
//		if(frameBuffer[i*3 + 0] > fadeOutFactor)
//			frameBuffer[i*3 + 0] -= frameBuffer[i*3 + 0]/fadeOutFactor;
//		else
//			frameBuffer[i*3 + 0] = 0;
//
//		if(frameBuffer[i*3 + 1] > fadeOutFactor)
//			frameBuffer[i*3 + 1] -= frameBuffer[i*3 + 1]/fadeOutFactor;
//		else
//			frameBuffer[i*3 + 1] = 0;
//
//		if(frameBuffer[i*3 + 2] > fadeOutFactor)
//			frameBuffer[i*3 + 2] -= frameBuffer[i*3 + 2]/fadeOutFactor;
//		else
//			frameBuffer[i*3 + 2] = 0;
//	}
//}



void visInit()
{
	// Set output channel/pin, GPIO_PIN_0 = 0, for GPIO_PIN_5 = 5 - this has to correspond to WS2812B_PINS
	ws2812b.item[0].channel = 2;
	// Your RGB framebuffer
	ws2812b.item[0].frameBufferPointer = frameBuffer;
	// RAW size of framebuffer
	ws2812b.item[0].frameBufferSize = sizeof(frameBuffer);


	// If you need more parallel LED strips, increase the WS2812_BUFFER_COUNT value
	ws2812b.item[1].channel = 3;
	ws2812b.item[1].frameBufferPointer = frameBuffer2;
	ws2812b.item[1].frameBufferSize = sizeof(frameBuffer2);


	ws2812b_init();
}
void visClearStrip(uint8_t *frameBuffer, uint32_t frameBufferSize)
{
	uint32_t i;
	for( i = 0; i < ((frameBufferSize) / 3); i++)
	{
		frameBuffer[i*3 + 0] = 0;
		frameBuffer[i*3 + 1] = 0;
		frameBuffer[i*3 + 2] = 0;
	}
}

//void visParty(uint8_t *frameBuffer, uint32_t frameBufferSize)
//{
//	uint32_t i;
//	for( i = 0; i < (frameBufferSize / 3); i++)
//	{
//
//			frameBuffer[i*3 + 0] = ;
//			frameBuffer[i*3 + 1] = ;
//			frameBuffer[i*3 + 2] = ;
//
//	}
//}


void visHandle(uint8_t visMode, uint8_t visMaxBrightness)
{
	static uint8_t visModeOld = 0;
	static uint32_t timestamp;
	if(ws2812b.transferComplete)
	{
		// Update your framebuffer here or swap buffers

		if(visMode != visModeOld)
		{
			visClearStrip(frameBuffer,sizeof(frameBuffer));
			visClearStrip(frameBuffer2,sizeof(frameBuffer2));
			visModeOld = visMode;
		}
		if(HAL_GetTick() - timestamp > 10)
		{
			timestamp = HAL_GetTick();
			switch(visMode)
			{
				case 0:		//Animation:
				{
					visBatteryStatus(frameBuffer,sizeof(frameBuffer),visMaxBrightness);
					visBatteryStatus(frameBuffer2,sizeof(frameBuffer2),visMaxBrightness);
					break;
				}
				case 1:		//Animation:
				{
					visThrottle(frameBuffer,sizeof(frameBuffer));
					visThrottle(frameBuffer2,sizeof(frameBuffer2));
					//visParty(frameBuffer,sizeof(frameBuffer));
					//visParty(frameBuffer2,sizeof(frameBuffer2));
					break;
				}
				case 2:		//Animation:
				{
					visThrottle(frameBuffer,sizeof(frameBuffer));
					visThrottle(frameBuffer2,sizeof(frameBuffer2));
					break;
				}
				case 3:		//Animation:
				{
					visWhiteWithLights(frameBuffer,sizeof(frameBuffer),visMaxBrightness);
					visWhiteWithLights(frameBuffer2,sizeof(frameBuffer2),visMaxBrightness);
					break;
				}
				case 4:		//Animation:
				{
					visRainbow(frameBuffer, sizeof(frameBuffer), 30);
					visRainbow(frameBuffer2, sizeof(frameBuffer), 30);
					break;
				}
				default:	//Animation: white
				{
					visWhiteWithLights(frameBuffer, sizeof(frameBuffer),visMaxBrightness);
					visWhiteWithLights(frameBuffer2, sizeof(frameBuffer),visMaxBrightness);
					break;
				}

			}
		}

		// Signal that buffer is changed and transfer new data
		ws2812b.startTransfer = 1;
		ws2812b_handle();
	}
}

uint8_t CheckStateOfCharge(void)
{
	uint32_t BatteryVoltage = getBatteryVoltage()/NumberOfBatteries;
	if(BatteryVoltage <= 4200 && BatteryVoltage > 4000)	return 28;
	if(BatteryVoltage <= 4000 && BatteryVoltage >= 3720)return 27;
	if(BatteryVoltage < 3720 && BatteryVoltage > 3210)				//linearer Bereich der Entladekurve
	{
		uint32_t val = (3720-BatteryVoltage)*100;
		uint8_t i;
		for(i = 0;i < 19;i++)
		{
			if(val <= ((i+1)*2684) && val >= (i*2684))
			{
				return 28-(i+2);
				break;
			}
		}
	}
	if(BatteryVoltage <= 3210 && BatteryVoltage > 3160)	return 7;
	if(BatteryVoltage <= 4000 && BatteryVoltage > 3110)	return 6;
	if(BatteryVoltage <= 3110 && BatteryVoltage > 3050)	return 5;
	if(BatteryVoltage <= 3050 && BatteryVoltage > 3000)	return 4;
	if(BatteryVoltage <= 3000 && BatteryVoltage > 2950)	return 3;
	if(BatteryVoltage <= 2950 && BatteryVoltage > 2880)	return 2;
	if(BatteryVoltage <= 2880 && BatteryVoltage > 2700)	return 1;
	else return 0;
}


