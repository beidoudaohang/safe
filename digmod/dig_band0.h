/*
name:digital.h
author:salan
date:20161208
ver:1.0
description:

*/
#ifndef DIG_BAND0_H_
#define DIG_BAND0_H_

/*include files*/
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "helios.h"
#include "dig_comm_def.h"
#include "digital.h"

/*****************************para define*************************/

/*****************************data struct define******************/
extern pthread_t band0_ts_id;
extern pthread_attr_t band0_ts_attr;
/****************************funs*******************************/
s8 band0_set_adr_add(u8 adr);
s8 band0_transmit_add(transmit_arry *trans);
void dig_band0_handle(void);
s32 dig_band0_read_sv(void);
s32 dig_band0_init(void);
void dig_read_deal(void); //@ccTag20170809
void *band0_task(void* arg);
void dig_cmd_set_deal(void);
#endif

