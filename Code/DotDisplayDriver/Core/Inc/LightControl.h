/*
 * LightControl.h
 *
 *  Created on: Mar 1, 2024
 *      Author: Sam
 */

#ifndef INC_LIGHTCONTROL_H_
#define INC_LIGHTCONTROL_H_

void LightControl_Background();

void LightControl_HandleSetBuffer(void* pBuf);
void LightControl_HandleFlipBuffer(void* pBuf);

#endif /* INC_LIGHTCONTROL_H_ */
