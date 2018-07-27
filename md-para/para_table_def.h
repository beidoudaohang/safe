/*
name:module_para_def.h
author:salan
date:20170322
ver:1.0
description:
	module parameter dat struct
*/
#ifndef PARA_TABLE_DEF_H_
#define PARA_TABLE_DEF_H_

/*****************************include files***********************/
#include "helios.h"
#include "para_def.h"
#include "module_adr_def.h"
#include "para_express.h"

/*****************************para define*************************/
typedef enum {
	PARA_RD = 0,
	PARA_RW
} PARA_RDRW_FLAG;

typedef union {
	u8 u8l;
	s8 s8l;
	u16 u16l;
	s16 s16l;
	u32 u32l;
	s32 s32l;
	f32 f32l;
	f64 f64l;
	u8 arry[8];
} para_limit;

typedef struct {
	para_type para_type_t;	/*参数类型*/
	u16 len;				/*参数长度*/
	void *dat;				/*参数内存首地址*/
	u16 dat_size;
	para_limit min;				/*参数最小值*/
	para_limit max;				/*参数最大值*/
	s16 (*paradeal)(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);	//flag=PARA_RDRW_FLAG /*参数特殊处理函数指针*/
} link_para;

typedef s16 (*paradeal)(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);

typedef struct {
	md_adr_info *md_adr;	/*模块信息类型*/
	link_para link_para_t;			/*参数*/
} md_link_para;

typedef struct {
	u16 index;		/*参数编号*/
	md_link_para link_para_a;
	u8 authority;	//USR_AUTHORITY	/*操作该参数需要的权限*/
	u8 dig_adr;			/*是否需要发送给fpga*/
	u8 flag;			/*读写标识*/
} para_table;

/*****************************data struct define******************/

/*****************************funs********************************/

#endif //PARA_TABLE_DEF_H_