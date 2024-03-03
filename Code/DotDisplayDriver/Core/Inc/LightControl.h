/*
 * LightControl.h
 *
 *  Created on: Mar 1, 2024
 *      Author: Sam
 */

#ifndef INC_LIGHTCONTROL_H_
#define INC_LIGHTCONTROL_H_

#include "main.h"

void LightControl_Init(TIM_HandleTypeDef* pTimer, uint32_t nChannel);
void LightControl_Background();

void LightControl_HandleSetBuffer(void* pBuf);
void LightControl_HandleFlipBuffer(void* pBuf);

#endif /* INC_LIGHTCONTROL_H_ */
