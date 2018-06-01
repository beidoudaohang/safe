/*
name:monitor_task.h
author:luke
date:20170419
ver:1.0
description:
	monitor task api
*/
#ifndef PROTOCOL_TASK_H_
#define PROTOCOL_TASK_H_

/*****************************include files***********************/
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "helios.h"
#include "protocol_def.h"

/*****************************para define*************************/
extern frame frame_send;
extern frame frame_local_recv;
extern frame frame_remote_recv;
extern para_stream para_stream_t;


extern pthread_t protocol_ts_id;
extern pthread_attr_t protocol_ts_attr;

extern pthread_t frame_send_ts_id;
extern pthread_attr_t frame_send_ts_attr;

/*****************************data struct define******************/
typedef struct
{
	u32 local_recv_len;	/*数据包长度*/
	u32 remote_recv_len;
	u32 send_len;	/*待发送数据包长度*/
} frame_info;
extern frame_info frame_info_t;
/*****************************funs********************************/
void *protocol_task(void* arg);
void *frame_send_task(void* arg);

#endif //PROTOCOL_TASK_H_
