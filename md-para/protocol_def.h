/*
name:monitor_comm_def.h
author:salan
date:20170322
ver:1.0
description:
	module communication dat struct
*/
#ifndef MONITOR_PROTOCOL_H_
#define MONITOR_PROTOCOL_H_

/*****************************include files***********************/
#include "helios.h"
#include "module_adr_def.h"


/*****************************para define*************************/
#define PARA_BUF_SIZE 		(1024)
#define FRAME_BUF_SIZE 		(48+1024)
#define FRAME_HEADER_LEN	(12)
#define FRAME_CMD_LEN 		(24)
#define FRAME_START			"HS1.0"
#define FRAME_END			"HE1.0"
#define FRAME_START_LEN 		(sizeof(FRAME_START)-1)
#define FRAME_END_LEN 		(sizeof(FRAME_END)-1)
#define PROTOCOL_VER 			0x0100	//v1.0


/*frame ack codes*/
typedef enum {
	FRAME_ACK_SUS = 0,
	FRAME_ACK_REPEATER_ID,
	FRAME_ACK_CRC,
	FRAME_ACK_PACKET_LOST,
	FRAME_ACK_CMD,
	FRAME_ACK_BAND,
	FRAME_ACK_MOD_ADR,
	FRAME_ACK_USR_AUTHO,
	FRAME_ACK_MOD_NO_RESPONSE,
	FRAME_ACK_USR_PASS_ERR,
	FRAME_ACK_USR_UNLOGIN,
	FRAME_ACK_OTHER_ERR = 0XFE
} FRAME_ACKS;
/*para address ack codes*/
typedef enum {
	PARA_ADR_ACK_NORMAL = 0,
	PARA_ADR_ACK_PARA_TYPE,
	PARA_ADR_ACK_PARA_LEN,
	PARA_ADR_ACK_PARA_INVALID
} PARA_ADR_ACKS;
/*the source codes where the frame come from*/
typedef enum {
	FRAME_SOURCE_SELF = 0,
	FRAME_SOURCE_LOCAL,
	FRAME_SOURCE_MODULE,
	FRAME_SOURCE_REMOTE,
} FRAME_SOURCE;
/*communication command define*/
typedef enum {
	FRAME_CMD_ALARM = 0X01,
	FRAME_CMD_HEARTBEAT = 0X11,
	FRAME_CMD_READ_PARA = 0X20,
	FRAME_CMD_READ_MOD_INFO = 0X21,
	FRAME_CMD_SET_PARA = 0X30,
	FRAME_CMD_LOGIN = 0X40,
	FRAME_CMD_UPDATE = 0X50,
	FRAME_CMD_REPLACE = 0X60
} FRAME_CMD;
/*****************************data struct define******************/
typedef enum {
	PARA_STREAM_NORMAL = 0,
	PARA_STREAM_BUSY
} PARA_STREAM_FLAGS;

#pragma pack(1)
typedef struct {
	u8 data[PARA_BUF_SIZE];		/*para data*/
	u16 paralen;				/*para data len*/
	u16 device_id;				/*device id*/
	u8 remote_code;				/*remote operation code*/
	u8 cmd;						/*frame command*/
	u8 authority;					/*operate authority*/
	u16 total;					/*total frames*/
	u16 index;					/*current frames*/
	u8 ack;						/*acknowledge*/
	FRAME_SOURCE source;					/*the source where the frame come from*/
	md_adr_info md_adr;			/*the module address which the frame want to operate*/
	u8 flag;
	s8 *next;
} para_stream;

typedef struct {
	u8 start[5];
	u16 len;
	u8 remote_code;
	u16 device_id;
	u16 crc;
	u8 data[FRAME_BUF_SIZE - 12];
} frame_header;

typedef struct {
	u8 reserve[12];
	u16 total;
	u16 index;
	u16 ver;
	u8 cmd;
	md_adr_info md_adr;
	u8 authority;	//USR_AUTHORITY;
	u8 rev[12];
	u8 ack;
	u8 data[PARA_BUF_SIZE + 5];
} frame_cmd;

typedef union {
	u8 arr[FRAME_BUF_SIZE];
	frame_header header;
	frame_cmd cmd;
} frame;
#pragma pack()
#endif //MONITOR_PROTOCOL_H_

