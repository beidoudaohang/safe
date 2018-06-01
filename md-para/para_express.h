/*
name:para_express.h
author:salan
date:20170322
ver:1.0
description:
	module parameter dat struct
*/
#ifndef PARA_EXPRESS_H_
#define PARA_EXPRESS_H_

/*****************************include files***********************/
#include "helios.h"


/*****************************para define*************************/
/*para err define*/
typedef enum
{
	PARA_SUS = 0,
	PARA_TYPE_ERR,
	PARA_LEN_ERR,
	PARA_ADR_INVALID,
	PARA_ADR_PERMISSION_DENIED,
	PARA_ADR_OTHER_ERR,
	PARA_OVERRANGE
} PARA_ERR;

typedef enum {
	PARA_TYPE_U8 = 0,
	PARA_TYPE_S8,
	PARA_TYPE_U16,
	PARA_TYPE_S16,
	PARA_TYPE_U32,
	PARA_TYPE_S32,
	PARA_TYPE_FLOAT,
	PARA_TYPE_DOUBLE,
	PARA_TYPE_STRING,
	PARA_TYPE_U64,
	PARA_TYPE_S64,
	PARA_TYPE_STRING10
} PARA_TYPE;
/*****************************data struct define******************/
/*para express*/
#pragma pack(1)
typedef union {
	u8 dat;
	struct {
		u8 type: 4;	//PARA_TYPE;
		u8 st: 4;	//PARA_ERR;
	} st_t;
} para_type;

typedef struct {
	u16 para_adr;
	para_type para_type_t;	//para_type
	u16 para_len;
} para;
#pragma pack()

/*****************************funs********************************/
#define GET_PARA_TYPE(A) 		((A)&0x0f)
#define SET_PARA_TYPE(A,B) 		((A)|=((B)&0x0f))
#define GET_PARA_ERR_CODE(A) 	(((A)&0xf0)>>4)
#define SET_PARA_ERR_CODE(A,B) 	((A)|=(((B)&0x0f)<<4))
#endif //PARA_EXPRESS_H_