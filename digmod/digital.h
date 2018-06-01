/*
name:digital.h
author:salan
date:20161208
ver:1.0
description:

*/
#ifndef DIGITAL_H_
#define DIGITAL_H_
/*****************************include files***********************/
#include "helios.h"
#include "module_adr_def.h"

/*****************************para define*************************/
#define DIG_SET_ADR_BUF_SIZE	256
#define DIG_TRANSMIT_BUF_SIZE 10

/*****************************data struct define******************/
typedef union {
	u32 u32_data;
	u8 u8_arry[4];
	u16 u16_arry[2];
	s32 s32_data;
	s8 s8_arry[4];
	s16 s16_arry[2];
	struct {
		u32 BIT31: 1;
		u32 BIT30: 1;
		u32 BIT29: 1;
		u32 BIT28: 1;
		u32 BIT27: 1;
		u32 BIT26: 1;
		u32 BIT25: 1;
		u32 BIT24: 1;
		u32 BIT23: 1;
		u32 BIT22: 1;
		u32 BIT21: 1;
		u32 BIT20: 1;
		u32 BIT19: 1;
		u32 BIT18: 1;
		u32 BIT17: 1;
		u32 BIT16: 1;
		u32 BIT15: 1;
		u32 BIT14: 1;
		u32 BIT13: 1;
		u32 BIT12: 1;
		u32 BIT11: 1;
		u32 BIT10: 1;
		u32 BIT09: 1;
		u32 BIT08: 1;
		u32 BIT07: 1;
		u32 BIT06: 1;
		u32 BIT05: 1;
		u32 BIT04: 1;
		u32 BIT03: 1;
		u32 BIT02: 1;
		u32 BIT01: 1;
		u32 BIT00: 1;
	} BIT;
} dig_para_struct;
typedef union {
	u32 u32data;
	u8 u8data;
	u16 u16data;
	s32 s32data;
	s8 s8data;
	s16 s16data;
	f32 f32data;
} all_data_type;
typedef struct {
	u16 index;
	u8 para_nums;
} items_info;


typedef struct {
	u16 total;
	u8 adr[DIG_SET_ADR_BUF_SIZE];
} dig_set_adr;

typedef struct {
	u8 adr;
	union {
		u8 data[4];
		u32 dat32;
	} DAT;
} transmit_arry;
typedef struct {
	u8 total;
	transmit_arry t_arry[DIG_TRANSMIT_BUF_SIZE];
} dig_transmit;
/*****************************funs********************************/
void f12tof32(const s16 src, f32 *dest);
void f32tof12(const f32 src, s16 *dest);

s8 dig_comm_crc_make(s8 *dat, u8 len, u16 *crc);
s8 set_adr_add(u8 adr, md_adr_info *md_adr);
s32 dig_gain_processing(md_adr_info *md_adr);

void dig_adi_adjust_again(md_adr_info *md_adr);
s32 dig_adi_adjust_update(md_adr_info *md_adr);

s32 dig_tech_to_channel_index(md_adr_info* md_adr, u8 tech);
s32 dig_channel_freq_update(md_adr_info *md_adr);
s32 dig_center_freq_update(md_adr_info *md_adr);

s32 dig_para_set_again(void);

s32 dig_fpga_loader(md_adr_info *md_adr);
s32 dig_relay_mode_change(md_adr_info *md_adr);
s32 dig_relay_mode_change_check(md_adr_info *md_adr);
#endif

