/*
 * SerialComm.h
 *
 *  Created on: Mar 1, 2024
 *      Author: Sam
 */

#ifndef INC_SERIALCOMM_H_
#define INC_SERIALCOMM_H_

#include <stdint.h>

enum
{
	MSG_TYPE_NONE = 0,
	MSG_TYPE_RESET,
	MSG_TYPE_SET_BUFFER,
	MSG_TYPE_FLIP_BUFFER,
	MSG_TYPE_SHOW_ID,
	MSG_TYPE_ALL_TYPE
};

void SerialComm_SetDeviceAddress(uint8_t nAddress);
void SerialComm_SetMessageHandler(uint8_t msgType, void(*handler)(void*));
void SerialComm_Background();

#endif /* INC_SERIALCOMM_H_ */
