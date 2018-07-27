/*
name:para_table.h
author:luke
date:20170324
ver:1.0
description:
	para table deal
*/
#ifndef PARA_TABLE_H_
#define PARA_TABLE_H_

/*****************************include files***********************/
#include "helios.h"
#include "para_table_def.h"
#include "protocol_def.h"
#include "para_dynamic_def.h"
#include "alarm_def.h"


/*****************************para define*************************/

#if MONITOR_MODULE_ENABLE
extern unit_para unit_para_t;
extern unit_dynamic_para unit_dynamic_para_t;
extern unit_alarm_c u_alarm_t;

extern band_para exmod_para_a[MONITOR_MOD_NUM];
extern band_dynamic_para exmod_dynamic_para_a[MONITOR_MOD_NUM];
extern md_alarm_c exmod_alarm_a[MONITOR_MOD_NUM];

extern pthread_mutex_t exmod_para_mutex;
#endif
#if OTHER_MODULE_ENABLE
extern band_para band_para_a[MOD_NUM_IN_ONE_PCB];
extern band_dynamic_para band_dynamic_para_a[MOD_NUM_IN_ONE_PCB];
extern md_alarm_c m_alarm_a[MOD_NUM_IN_ONE_PCB];
#endif

extern pcb_share_para pcb_share;
extern usr_authorize usr_auth;

/*****************************data struct define******************/

/*****************************funs********************************/
s16 get_para_table_size(void);
s32 find_para_adr(const para_table *table, u16 tlen, u16 adr);
s32 find_para_adr_mod( md_adr_info *md, u8 *is_exmod);
s8 one_para_adr_read_processing(const u16 adr, para_stream *ps);
s8 one_para_adr_set_processing(const s8 *src, para_stream *ps);


#endif //PARA_TABLE_H_
