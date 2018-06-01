/*
name:data_task_def.h
author:luke
date:20170517
ver:1.0
description:
	module parameter dat struct
*/
#ifndef DATA_TASK_DEF_H_
#define DATA_TASK_DEF_H_

/*****************************include files***********************/
#include "helios.h"
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
/*****************************para define*************************/
typedef enum
{
	DATA_TYPE_UNIT = 1,
	DATA_TYPE_MOD = 2,
	DATA_TYPE_PCB = 4
} DATA_TYPE;


/*****************************data struct define******************/

/*****************************funs********************************/

#endif //DATA_TASK_DEF_H_