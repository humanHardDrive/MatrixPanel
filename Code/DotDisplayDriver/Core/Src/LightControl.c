/*
 * LightControl.c
 *
 *  Created on: Mar 1, 2024
 *      Author: Sam
 */
#include <stdint.h>
#include <string.h>

#include "LightControl.h"

typedef struct
{
	//Order of members in this struct aligns with the device
	uint8_t nRed;
	uint8_t nGreen;
	uint8_t nBlue;
	uint8_t nWhite;
}ColorStruct;

#define NUM_BUFFERS	4
#define NUM_LEDS	16

#define LED_BIT_0	5
#define LED_BIT_1	10

static ColorStruct aBuffer[NUM_BUFFERS][NUM_LEDS] = {0};
static uint8_t bRedraw = 0;

static TIM_HandleTypeDef* pLEDTimer = NULL;
static uint32_t nTimerChannel = 0;

//Size of 1 color * number of bits per byte * number of LEDs
static uint32_t DMABuffer[sizeof(ColorStruct) * 8 * NUM_LEDS];

void LightControl_HandleSetBuffer(void* pBuf)
{
	typedef struct
	{
		uint8_t nBuffer;
		ColorStruct colors[NUM_LEDS];
	}SetBufferStruct;

	SetBufferStruct* pSB = (SetBufferStruct*)pBuf;

	memcpy(aBuffer[pSB->nBuffer], pSB->colors, NUM_LEDS * sizeof(ColorStruct));
}

void LightControl_HandleFlipBuffer(void* pBuf)
{
	typedef struct
	{
		uint8_t nBuffer;
		uint16_t nBlendTime;
	}FlipBufferStruct;

	FlipBufferStruct* pFB = (FlipBufferStruct*)pBuf;

	memcpy(aBuffer[0], aBuffer[pFB->nBuffer], NUM_LEDS * sizeof(ColorStruct));
}

void LightControl_Init(TIM_HandleTypeDef* pTimer, uint32_t nChannel)
{
	pLEDTimer = pTimer;
	nTimerChannel = nChannel;
}

static void l_ColorToTiming(ColorStruct* pColorBuffer, uint16_t nCount, uint32_t* pTimingBuffer)
{
	//Every LED
	for(uint16_t i = 0; i < nCount; i++)
	{
		//Every color
		for(uint8_t j = 0; j < sizeof(ColorStruct); j++)
		{
			//Every bit
			//MSB first
			uint8_t nMask = 0x80;
			for(uint8_t k = 0; k < 8; k++)
			{
				if(((uint8_t*)&pColorBuffer[i])[j] & nMask)
					(*pTimingBuffer++) = LED_BIT_1;
				else
					(*pTimingBuffer++) = LED_BIT_0;

				nMask >>= 1;
			}
		}
	}
}

void LightControl_Background()
{
	if(bRedraw)
	{
		l_ColorToTiming(aBuffer[0], NUM_LEDS, DMABuffer);
		HAL_TIM_PWM_Start_DMA(pLEDTimer, nTimerChannel, DMABuffer, sizeof(DMABuffer));
		bRedraw = 0;
	}
}
