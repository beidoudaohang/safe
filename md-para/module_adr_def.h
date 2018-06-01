/*
name:module_adr_def.h
author:salan
date:20170321
ver:1.0
description:
	module address dat struct
*/
#ifndef MODULE_ADR_DEF_H_
#define MODULE_ADR_DEF_H_

/*include files*/
#include "helios.h"

/*****************************define para*************************/


/**********************module type define*************************/
typedef enum
{
	MOD_TYPE_REV = 0,
	MOD_TYPE_PA,
	MOD_TYPE_LNA,
	MOD_TYPE_DIG,
	MOD_TYPE_MONITOR,
	MOD_TYPE_SUB_MONITOR,
	MOD_TYPE_SIGNALTOUCH,
	MOD_TYPE_RELAY,
	MOD_TYPE_OPTICAL,
	MOD_TYPE_FREQSEL_1,
	MOD_TYPE_LED
} MOD_TYPE;

/*******************mod_adr define****************************/
/*module link define*/
typedef enum {
	MOD_LINK_REV = 0,
	MOD_LINK_UL = 1,
	MOD_LINK_DL = 2,
	MOD_LINK_ALL = 3
} MOD_LINK;
/*module mimo define*/
typedef enum {
	MOD_MIMO_REV = 0,
	MOD_MIMO_1 = 1,
	MOD_MIMO_2 = 2,
	MOD_MIMO_3 = 3,
	MOD_MIMO_4 = 4,
	MOD_MIMO_5 = 5,
	MOD_MIMO_6 = 6,
	MOD_MIMO_7 = 7,
} MOD_MIMO;
/*module indentify define*/
typedef enum {
	MOD_ID_0 = 0,
	MOD_ID_1 = 1,
	MOD_ID_2 = 2,
	MOD_ID_3 = 3,
	MOD_ID_4 = 4,
	MOD_ID_5 = 5,
	MOD_ID_6 = 6,
	MOD_ID_7 = 7,
} MOD_ID;
#pragma pack(1)
typedef union {
	u8 dat;
	struct {
		u8 mod_id: 3;	//MOD_ID
		u8 mod_mimo: 3;	//MOD_MIMO
		u8 mod_link: 2;	//MOD_LINK
	} mod_sub_adr;
} mod_adr;




/*************************dat struct***************************/
/*************************module adr info define***************/
typedef struct {
	u8 mod_type;	//MOD_TYPE
	u8 mod_band;
	mod_adr mod_adr_t;
} md_adr_info;
#pragma pack()
#endif //MODULE_ADR_DEF_H_