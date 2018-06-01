/*
name:local_recv_task.h
author:luke
date:20170419
ver:1.0
description:
	本控数据包接收线程
*/
#ifndef LOCAL_RECV_TASK_H_
#define LOCAL_RECV_TASK_H_

/*****************************include files***********************/
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "helios.h"
#include "protocol_def.h"

/*****************************para define*************************/
extern frame frame_send;
extern frame frame_local_recv;

extern pthread_t local_recv_ts_id;
extern pthread_attr_t local_recv_ts_attr;

/*****************************data struct define******************/

/*****************************funs********************************/
void *local_recv_task(void* arg);

#endif //LOCAL_RECV_TASK_H_
