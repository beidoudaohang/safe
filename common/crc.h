/*
name:crc.h
author:salan
date:20170322
ver:1.0
description:
	crc struct
*/
#ifndef CRC_H_
#define CRC_H_

/*****************************include files***********************/
#include "helios.h"


/*****************************para define*************************/

/*****************************data struct define******************/

/*****************************functions***************************/
u16 crc(const s8 *data,u16 len);
u16 crc16_2(u8 *d,u8 len,u16 crc);

#endif //CRC_H_
