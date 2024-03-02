/*
 * SerialComm.c
 *
 *  Created on: Mar 1, 2024
 *      Author: Sam
 */
#include <stdlib.h>
#include <string.h>

#include "SerialComm.h"

#define MAX_PACKET_PAYLOAD	128

typedef struct
{
	uint8_t nMsgType;
	uint8_t nLen;
	uint8_t nDestination;
	uint8_t nChecksum;
	uint8_t payload[MAX_PACKET_PAYLOAD];
}SERIAL_PACKET;

static SERIAL_PACKET buildPacket;
static uint8_t* pBuildPacket;

enum
{
	MSG_STATE_STX = 0,
	MSG_STATE_PACKET,
	MSG_STATE_ETX
};

#define PACKET_STX	0x55
#define PACKET_ETX	0xAA

#define BROADCAST_ADDR	0xFF

static uint8_t nDeviceAddr = BROADCAST_ADDR;
static void (*aMsgHandler[MSG_TYPE_ALL_TYPE])(void*) = {NULL};

static uint8_t nMessageState = MSG_STATE_STX;

void SerialComm_SetDeviceAddress(uint8_t nAddress)
{
	nDeviceAddr = nAddress;
}

void SerialComm_SetMessageHandler(uint8_t msgType, void(*handler)(void*))
{
	if(msgType < MSG_TYPE_ALL_TYPE)
		aMsgHandler[msgType] = handler;
}

static uint8_t l_CalculateChecksum(SERIAL_PACKET* pPacket)
{
	uint8_t nChecksum = 0;

	for(uint8_t i = 0; i < pPacket->nLen; i++)
		nChecksum += pPacket->payload[i];

	nChecksum = ~nChecksum;

	return nChecksum;
}

void SerialComm_Background()
{
	uint8_t c;

	//New character available from buffer
	if(0)
	{
		switch(nMessageState)
		{
		case MSG_STATE_STX:
			if(c == PACKET_STX)
			{
				pBuildPacket = (uint8_t*)&buildPacket;
				memset(pBuildPacket, 0, sizeof(SERIAL_PACKET));
				nMessageState = MSG_STATE_PACKET;
			}
			break;

		case MSG_STATE_PACKET:
			*(pBuildPacket++) = c;

			//Packet complete
			if((pBuildPacket > &(buildPacket.nLen)) && ((pBuildPacket - buildPacket.payload) > buildPacket.nLen))
				nMessageState = MSG_STATE_ETX;

			break;

		case MSG_STATE_ETX:
			if(c == PACKET_ETX)
			{
				if((l_CalculateChecksum(&buildPacket) == buildPacket.nChecksum) && //Valid checksum
					((buildPacket.nDestination == nDeviceAddr) || (buildPacket.nDestination == BROADCAST_ADDR)) && //Message is for this board
					aMsgHandler[buildPacket.nMsgType]) //There is a handler for this message
				{
					aMsgHandler[buildPacket.nMsgType](buildPacket.payload);
				}
			}
			nMessageState = MSG_STATE_STX;
			break;
		}
	}
}
