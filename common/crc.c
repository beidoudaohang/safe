/*
name:crc.c
author:salan
date:20170322
ver:1.0
description:
    crc cal
*/

/*****************************include files***********************/
#include "crc.h"


/*****************************para define*************************/


/*****************************data struct define******************/

/*****************************functions***************************/
u16 crc(const s8 *data, u16 len)
{
    u16 crcd;
    u16 cnt;
    u16 temp;
    u8 i;

    crcd = 0;

    for (cnt = 0; cnt < len; cnt++) {
        temp = (u8)data[cnt];

        crcd = crcd ^ (temp << 8);
        for (i = 0; i < 8; i++) {
            if (crcd & 0x8000)
                crcd = (crcd << 1) ^ 0x1021;
            else
                crcd <<= 1;
        }
    }
    return (crcd);
}