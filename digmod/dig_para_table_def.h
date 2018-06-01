/*
name:dig_para_table_def.h
author:salan
date:20170324
ver:1.0
description:
	module parameter dat struct
*/
#ifndef DIG_PARA_TABLE_DEF_H_
#define DIG_PARA_TABLE_DEF_H_

/*****************************include files***********************/
#include "helios.h"


/*****************************para define*************************/
/*参数在32bit内的地址，高位在前，地位在后*/
typedef enum {
	DIG_ADR_BYTE_0 = 0,
	DIG_ADR_BYTE_8,
	DIG_ADR_BYTE_16,
	DIG_ADR_BYTE_24,
	DIG_ADR_BIT0,
	DIG_ADR_BIT1,
	DIG_ADR_BIT2,
	DIG_ADR_BIT3,
	DIG_ADR_BIT4,
	DIG_ADR_BIT5,
	DIG_ADR_BIT6,
	DIG_ADR_BIT7,
	DIG_ADR_BIT8,
	DIG_ADR_BIT9,
	DIG_ADR_BIT10,
	DIG_ADR_BIT11,
	DIG_ADR_BIT12,
	DIG_ADR_BIT13,
	DIG_ADR_BIT14,
	DIG_ADR_BIT15,
	DIG_ADR_BIT16,
	DIG_ADR_BIT17,
	DIG_ADR_BIT18,
	DIG_ADR_BIT19,
	DIG_ADR_BIT20,
	DIG_ADR_BIT21,
	DIG_ADR_BIT22,
	DIG_ADR_BIT23,
	DIG_ADR_BIT24,
	DIG_ADR_BIT25,
	DIG_ADR_BIT26,
	DIG_ADR_BIT27,
	DIG_ADR_BIT28,
	DIG_ADR_BIT29,
	DIG_ADR_BIT30,
	DIG_ADR_BIT31,
} DIG_ADR;
typedef enum
{
	DIG_LEN_U8 = 0,
	DIG_LEN_U16,
	DIG_LEN_U32,
	DIG_LEN_F12,
	DIG_LEN_BIT,
	DIG_LEN_S8,
	DIG_LEN_S16,
	DIG_LEN_S32,
	DIG_LEN_U32U16, //@ccTag20170808
} DIG_LEN;
typedef struct {
	u8 key;		/*参数在fpga通信协议中的地址编号*/
	void *pdat;	/*参数在内存的地址*/
	DIG_ADR adr;		/*参数在fpga通信协议中的32bit中的位置*/
	DIG_LEN len;		/*参数在fpga通信协议中的长度*/
	void (*pdatadeal)(const void *src, void *dest);
} dig_para_item;

/*****************************data struct define******************/

/*****************************funs********************************/

#endif //DIG_PARA_TABLE_DEF_H_