/*
name:module_para_def.h
author:luke
date:20170505
ver:1.0
description:
	module parameter dat struct
*/
#ifndef ALARM_DEF_H_
#define ALARM_DEF_H_

/*****************************include files***********************/
#include "helios.h"
#include "para_def.h"
#include "module_adr_def.h"

#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

/*****************************para define*************************/
/*告警次数统计*/
typedef struct {
	unit_alarm u_alarm;
	unit_alarm u_alarm_cnt;
	u32 alarm_times_oneday;
} unit_alarm_c;
typedef struct {
	md_alarm m_alarm;
	md_alarm m_alarm_cnt;
	u32 alarm_times_oneday;
} md_alarm_c;

/*告警统计多少次后上报*/
#define ALARM_CENSUS_TIME		5
#define MAX_ALARM_TIMES_ONEDAY	30
/*****************************data struct define******************/
extern unit_alarm_c u_alarm_t;
extern md_alarm_c m_alarm_a[MOD_NUM_IN_ONE_PCB];

extern pthread_t alarm_ts_id;
extern pthread_attr_t alarm_ts_attr;
/*****************************funs********************************/

void *alarm_task(void* arg);

#endif //ALARM_DEF_H_