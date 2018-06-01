/*
name:data_task_def.h
author:luke
date:20170517
ver:1.0
description:
	module parameter dat struct
*/
#ifndef DATA_TASK_H_
#define DATA_TASK_H_

/*****************************include files***********************/
#include "helios.h"
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "data_task_def.h"
/*****************************para define*************************/



/*****************************data struct define******************/
extern pthread_t data_ts_id;
extern pthread_attr_t data_ts_attr;
/*****************************funs********************************/
void data_update(DATA_TYPE type);
void *data_task(void* arg);
s32 data_update_check(void);

#endif //DATA_TASK_H_