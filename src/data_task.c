/*
name:data_task.c
author:luke
date:20170517
ver:1.0
description:
	数据保存
*/
/***************************include files****************************/
#include "helios.h"
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include <semaphore.h>
#include "porting.h"
#include "para_table.h"
#include "data_task.h"
#include "log.h"
/****************************data def********************************/
pthread_t data_ts_id;
pthread_attr_t data_ts_attr;
sem_t data_write_sem;

static volatile u32 data_update_flag = 0;
/*****************************funs***********************************/
/*
参数读取
return:
	0---sus
	-1---err
 */
s32 data_init(void)
{
	int err;
	char read_buf[100] = {0};
	int read_len;
	u8 mod_index;
	memset(&unit_para_t, 0, sizeof(unit_para));
	memset(&unit_dynamic_para_t, 0, sizeof(unit_dynamic_para));
	memset(&band_para_a, 0, (MOD_NUM_IN_ONE_PCB * sizeof(band_para)));
	memset(&band_dynamic_para_a, 0, (MOD_NUM_IN_ONE_PCB * sizeof(band_dynamic_para)));

	err = sem_init(&(data_write_sem), 0, 0);
	if (err) {
		RLDEBUG("data_init:creat data_write_sem sem err\r\n");
		exit(EXIT_FAILURE);
	}

	//first run,para file need init
	err = unit_file_init();
	if (err < 0) {
		RLDEBUG("relay unit file init false\r\n!");
		return -1;
	}


	err = band_file_init();
	if (err < 0) {
		RLDEBUG("relay band file init false\r\n!");
		return -1;
	}

	err = pcb_file_init();
	if (err < 0) {
		RLDEBUG("relay pcb file init false\r\n!");
		return -1;
	}

	//read data
	//RLDEBUG("relay read para!\r\n");

	err = unit_file_read(&unit_para_t);
	if (err < 0) {
		RLDEBUG("read unit para false!\r\n");
		return -1;
	}

	for (mod_index=0; mod_index<MOD_NUM_IN_ONE_PCB; mod_index++){
		err = band_file_read((band_para*)&band_para_a[mod_index], mod_index);
		if (err < 0) {
			RLDEBUG("read band para false!\r\n");
			return -1;
		}
	}
	err = pcb_file_read((pcb_share_para*)&pcb_share);
	if (err < 0) {
		RLDEBUG("read pcb para false!\r\n");
		return -1;
	}


	//check module adr
	if (MOD_TYPE_MONITOR != (unit_para_t.md_adr_t.mod_type)) {
		unit_para_t.md_adr_t.mod_type = MOD_TYPE_MONITOR;
		unit_para_t.md_adr_t.mod_band = 1;
		unit_para_t.md_adr_t.mod_adr_t.mod_sub_adr.mod_link = MOD_LINK_DL;
		unit_para_t.md_adr_t.mod_adr_t.mod_sub_adr.mod_mimo = MOD_MIMO_REV;
		unit_para_t.md_adr_t.mod_adr_t.mod_sub_adr.mod_id = MOD_ID_0;
	}
	if (MOD_TYPE_RELAY != (band_para_a[0].md_adr_t.mod_type)) {
		band_para_a[0].md_adr_t.mod_type = MOD_TYPE_RELAY;
		band_para_a[0].md_adr_t.mod_band = 1;
		band_para_a[0].md_adr_t.mod_adr_t.mod_sub_adr.mod_link = MOD_LINK_DL;
		band_para_a[0].md_adr_t.mod_adr_t.mod_sub_adr.mod_mimo = MOD_MIMO_REV;
		band_para_a[0].md_adr_t.mod_adr_t.mod_sub_adr.mod_id = MOD_ID_0;
	}
	#if PA_MODULE_ENABLE
	if (MOD_TYPE_PA != (band_para_a[1].md_adr_t.mod_type)) {
		band_para_a[1].md_adr_t.mod_type = MOD_TYPE_PA;
		band_para_a[1].md_adr_t.mod_band = 1;
		band_para_a[1].md_adr_t.mod_adr_t.mod_sub_adr.mod_link = MOD_LINK_UL;
		band_para_a[1].md_adr_t.mod_adr_t.mod_sub_adr.mod_mimo = MOD_MIMO_REV;
		band_para_a[1].md_adr_t.mod_adr_t.mod_sub_adr.mod_id = MOD_ID_0;
	}

	if (MOD_TYPE_PA != (band_para_a[2].md_adr_t.mod_type)) {
		band_para_a[2].md_adr_t.mod_type = MOD_TYPE_PA;
		band_para_a[2].md_adr_t.mod_band = 1;
		band_para_a[2].md_adr_t.mod_adr_t.mod_sub_adr.mod_link = MOD_LINK_DL;
		band_para_a[2].md_adr_t.mod_adr_t.mod_sub_adr.mod_mimo = MOD_MIMO_REV;
		band_para_a[2].md_adr_t.mod_adr_t.mod_sub_adr.mod_id = MOD_ID_0;
	}
	#endif

	band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.dig_pa_sw = DIG_PA_UL_OFF_DL_OFF;

	return 0;
}
/*TODO:It needs to be redesigned
fun: update data flag
para:
	type: which data file need to update
return:

 */
void data_update(DATA_TYPE type)
{
	SETBIT(data_update_flag, type);
}

/*
fun: update data flag
para:

return:
	0---no data need save
	1---something need save
 */
// s32 data_update_check(void)
// {
// 	if ((GETBIT(data_update_flag, DATA_TYPE_UNIT)) || (GETBIT(data_update_flag, DATA_TYPE_MOD)) || (GETBIT(data_update_flag, DATA_TYPE_PCB))) {
// 		return 1;
// 	}
// 	return 0;
// }


void *data_task(void* arg)
{
	s32 cnt, err;
	u8 mod_index;
	//RLDEBUG("data task start!\r\n");
	arg = arg;

	while (1) {
		//timedelay(0, 0, 100, 0);
		err = sem_wait(&(data_write_sem));
		if (err) {
			RLDEBUG("data_task:sem_wait() false \r\n");
			timedelay(0, 0, 100, 0);
			continue;
		}else{
			if (GETBIT(data_update_flag, DATA_TYPE_UNIT)) {
				RLDEBUG("data task:sava unit file\r\n");
				unit_file_write(&unit_para_t);
				CLRBIT(data_update_flag, DATA_TYPE_UNIT);
			}
			for(mod_index=0; mod_index<MOD_NUM_IN_ONE_PCB; mod_index++){
				if (GETBIT(data_update_flag, mod_index)) {
					RLDEBUG("data task:sava band file\r\n");
					band_file_write(&band_para_a[mod_index], mod_index);
					CLRBIT(data_update_flag, mod_index);
				}
			}

			if (GETBIT(data_update_flag, DATA_TYPE_PCB)) {
				RLDEBUG("data task:sava pcb file\r\n");
				pcb_file_write(&pcb_share);
				CLRBIT(data_update_flag, DATA_TYPE_PCB);
			}
		}
	}
}