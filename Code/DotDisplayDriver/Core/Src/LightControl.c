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
	uint8_t nRed;
	uint8_t nGreen;
	uint8_t nBlue;
	uint8_t nWhite;
}ColorStruct;

#define NUM_BUFFERS	4
#define NUM_LEDS	16

static ColorStruct aBuffer[NUM_BUFFERS][NUM_LEDS] = {0};

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

void LightControl_Background()
{
}
