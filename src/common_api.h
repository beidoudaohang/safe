/*
name:common_api.h
author:luke
date:20170620
ver:1.0
description:
	module parameter dat struct
*/
#ifndef COMMON_API_H_
#define COMMON_API_H_

/*****************************include files***********************/
#include "helios.h"
#include "module_adr_def.h"

/*****************************para define*************************/


/*****************************data struct define******************/

/*****************************funs********************************/
void f32tou32(const void *src, void *dest);
void f32tou16(const void *src, void *dest);
void f32toieee754(const f32 src, u32 *dest);
void f64toieee754(const f64 src, u64 *dest);
void ieee754tof32(const u32 src, f32 *dest);
void ieee754tof64(const u64 src, f64 *dest);
s32 md_adr_to_index(md_adr_info *md_adr);
s32 tech_to_tech_index(md_adr_info *md_adr, u8 tech);
s32 reset_auto_manual_scan_flag(md_adr_info *md_adr);
s32 ipv4_address_check(const char *str);
s32 ipv4_mask_check(char* subnet);

#endif //COMMON_API_H_