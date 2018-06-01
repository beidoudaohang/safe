/*
name:dig_comm_def.h
author:salan
date:20161208
ver:1.0
description:

*/

#ifndef DIG_COMM_DEF_H_
#define DIG_COMM_DEF_H_

/*include files*/
#include "helios.h"

/*para*/
#define DIG_COMM_START 			0X7E
#define DIG_COMM_CRC_ERR		0X40
#define DIG_COMM_KEY_NUM_MAX	0x04
#define DIG_COMM_DAT_LEN		24

/*与fpga通信的相关数据结构*/
#pragma pack(1)
typedef struct {
	u8 comm_start;
	u8 comm_cmd;
	u8 comm_adr[4];
	u32 comm_dat[4];
	u16 comm_crc;;
} dig_comm_write;
typedef struct {
	u8 comm_start;
	u8 comm_cmd;
	u8 comm_adr[4];
	u16 comm_crc;;
} dig_comm_read;

typedef union {
	u8 arr[32];
	dig_comm_write dig_comm_w;
	dig_comm_read dig_comm_r;
} dig_comm_frame;
#pragma pack()

typedef struct {
	u16 index[4];	/*参数在待处理指令数组里的索引值*/
	u8 adr[4];		/*参数地址编号*/
} dig_adrs;

typedef enum {
	DIG_CMD_READ = 0X00,
	DIG_CMD_WRITE = 0x80
} DIG_COMM_CMD;



#endif


