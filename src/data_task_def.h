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
	DATA_TYPE_MOD1,
	DATA_TYPE_MOD2,
	DATA_TYPE_MOD3,
	DATA_TYPE_UNIT,
	DATA_TYPE_PCB,
	DATA_TYPE_EXMOD
} DATA_TYPE;


/*****************************data struct define******************/

/*****************************funs********************************/

#endif //DATA_TASK_DEF_H_