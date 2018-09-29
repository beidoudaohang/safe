/*
name:common_api.c
author:luke
date:20170620
ver:1.0
description:
	common api
*/
/***************************include files****************************/
#include <string.h>
#include "helios.h"
#include "common_api.h"
#include "para_table.h"
#include <arpa/inet.h>
#include <stdio.h>

/***************************para define*******************************/
typedef union {
	f32 f;
	u32 u;
} ieee754_32;
typedef union {
	f64 f;
	u64 u;
} ieee754_64;

/***************************funs**************************************/
void f32tou32(const void *src, void *dest)
{
	f32 srcdata;
	u32 destdata;

	if ((NULL == dest) || (NULL == src))
		return;

	srcdata = *(f32*)src;
	srcdata = srcdata * 1000;
	destdata = (u32)srcdata;
	memcpy((void*)dest, (void*)&destdata, sizeof(destdata));
}

void f32tou16(const void *src, void *dest)
{
	f32 srcdata;
	u16 destdata;

	if ((NULL == dest) || (NULL == src))
		return;

	srcdata = *(f32*)src;
	srcdata = srcdata * 1000;
	destdata = (u16)srcdata;
	memcpy((void*)dest, (void*)&destdata, sizeof(destdata));
}

void f32toieee754(const f32 src, u32 *dest)
{
	ieee754_32 i754;

	i754.f = src;
	*dest = i754.u;
}
void f64toieee754(const f64 src, u64 *dest)
{
	ieee754_64 i754;

	i754.f = src;
	*dest = i754.u;
}
void ieee754tof32(const u32 src, f32 *dest)
{
	ieee754_32 i754;

	i754.u = src;
	*dest = i754.f;
}
void ieee754tof64(const u64 src, f64 *dest)
{
	ieee754_64 i754;

	i754.u = src;
	*dest = i754.f;
}
/*
fun:寻找模块地址对应的系统索引
para：
	md_adr：模块地址
return：
	0< ---返回的系统索引
	-1 ---err
 */
s32 md_adr_to_index(md_adr_info *md_adr)
{
	s32 index;

	if (NULL == md_adr)
		return -1;
	for (index = 0; index < MOD_NUM_IN_ONE_PCB; index++) {
		if (!memcmp(&(band_para_a[index].md_adr_t), md_adr, sizeof(md_adr_info)))
			break;
	}

	if (index >= MOD_NUM_IN_ONE_PCB)
		return -1;

	return index;
}

/*
fun:记录作为relay的制式的主小区信息
para：
	md_adr:模块地址
return:
	>=0---sus
	-1---err
 */
s32 reset_auto_manual_scan_flag(md_adr_info *md_adr)
{
	s32 err;
	s32 index;

	if (NULL == md_adr)
		return -1;
	index = md_adr_to_index(md_adr);
	if (index < 0)
		return -1;

	for (err = 0; err < MODEM_SUPPORT_TECH_MAX; err++) {
		band_dynamic_para_a[index].md_wireless_net_t.modem_tech_a[err].scan_status = 0;
		band_dynamic_para_a[index].md_wireless_net_t.rflag = RELAY_STATUS_OFF;
	}

	return 0;
}

//返回值大于0正确
s32 ipv4_address_check(const char *str)
{
    struct in_addr addr;
    int ret;

    ret = inet_pton(AF_INET, str, &addr);

/*     if (ret > 0);
    else if (ret < 0)
        printf("EAFNOSUPPORT: %d\n", strerror(local_errno));
    else
        printf("\"%s\" is not a valid IPv4 address\n", str); */
    return ret;
}

//返回1正确
s32 ipv4_mask_check(char* subnet)
{
    if(ipv4_address_check (subnet))
    {
        unsigned int b = 0, i, n[4];
        sscanf(subnet, "%u.%u.%u.%u", &n[3], &n[2], &n[1], &n[0]);
        for(i = 0; i < 4; ++i) //将子网掩码存入32位无符号整型
            b += n[i] << (i * 8); 
        b = ~b + 1;
        if((b & (b - 1)) == 0)   //判断是否为2^n
            return 1;
    }
    
    return 0;
}
