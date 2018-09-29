/*
 * File      : protocol_trans.c
 * 
 *新旧协议转换
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-06-26      sujj      first version
*/
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "protocol_trans.h"
#include "helios.h"
#include "Module_Struct.h"
#include "Module_Comm.h"
#include "para_def.h"
#include "helios.h"
#include "para_table.h"
#include "dig_band0_para_table.h"
#include "digital.h"
#include "alarm_def.h"
#include "porting.h"
#include "log.h"
#include "common_api.h"
#include "data_task_def.h"
#include "module_adr_def.h"
#include "module_send.h"

para_temp_save para_temp_save_t;

extern MODULE_RP MOD_RP[SYS_NUM];

/* const band_table band_table_t[] =
{
	{1, 1920, 1980, 2110, 2170},
	{2, 1850, 1910, 1930, 1990},
	{3, 1710, 1785, 1805, 1880},
	{4, 1710, 1755, 2110, 2155},
	{5, 824, 849, 869, 894},
	{6, 830, 840, 875, 885},
	{7, 2500, 2570, 2620, 2690},
	{8, 880, 915, 925, 960},
	{9, 1750, 1785, 1845, 1880},
	{10, 1710, 1770, 2110, 2170},
	{11, 1428, 1453, 1476, 1501},
	{12, 699, 716, 728, 746},
	{13, 777, 787, 746, 757},
	{14, 788, 798, 758, 768},
	{17, 704, 716, 734, 746},
	{18, 815, 830, 860, 875},
	{19, 830, 845, 875, 890},
	{20, 832, 862, 791, 821},
	{21, 1448, 1463, 1460, 1511},
	{22, 3410, 3490, 3510, 3590},
	{23, 2000, 2020, 2180, 2200},
	{24, 1626, 1661, 1525, 1529},
	{25, 1850, 1915, 1930, 1995},
	{26, 814, 849, 859, 894},
	{27, 807, 824, 852, 869},
	{28, 703, 748, 758, 803},
	{33, 1900, 1920, 1900, 1920},
	{34, 2010, 2025, 2010, 2025},
	{35, 1850, 1910, 1850, 1910},
	{36, 1930, 1990, 1930, 1990},
	{37, 1910, 1930, 1910, 1930},
	{38, 2570, 2620, 2570, 2620},
	{39, 1880, 1920, 1880, 1920},
	{40, 2300, 2400, 2300, 2400},
	{41, 2496, 2690, 2496, 2690},
	{42, 3400, 3600, 3400, 3600},
	{43, 3600, 3800, 3600, 3800},
};
#define BAND_TABLE_T_SIZE 	(sizeof(band_table_t)/sizeof(band_table))
 */

void oldsys_band_table_init()
{
	u8 i;
	for(i=0; i<8; i++){
		if(unit_para_t.unit_augment.oldsys_band_table[i] == 0){
			unit_para_t.unit_augment.oldsys_band_table[i] = i+1;
		}
	}
	
}

//旧协议地址换算新协议模块参数存储位置
u8 oldaddr_find_mod(u8 fun, u8 addr)
{
	u8 addr_fun = 0;
	u8 addr_sys = (addr & 0x07);
	u8 addr_udl = (addr & 0x08) ? 1 : 0;

	switch (fun)
	{
		case DIG_PICO:
			addr_fun = MOD_TYPE_DIG;
			break;
		case PA_FUNC:
			addr_fun = MOD_TYPE_PA;
			break;
		default:
			return 0xff;
			break;
	}

	if(addr_fun > 0) addr_fun -= 1;
	else return 0xff;

	return  addr_sys + (addr_udl<<3) + (addr_fun<<4);
}

//
u8 newaddr_to_sysnum(md_adr_info *md_adr)
{
	u8 addr_fun = 0;
	u8 addr_sys = 0xff;
	u8 addr_udl = 0;
	u8 is_exmod = 0;
	u8 index = find_para_adr_mod(md_adr, &is_exmod);

	if(is_exmod){
		if(md_adr->mod_type > 0) addr_fun = md_adr->mod_type - 1;

		switch (md_adr->mod_adr_t.mod_sub_adr.mod_link)
		{
			case MOD_LINK_UL:
				addr_udl = 1;
				break;
			case MOD_LINK_DL:
				addr_udl = 0;
				break;
			default:
				return 0xff;
				break;
		}
  
		addr_sys = index - (addr_udl<<3) - (addr_fun<<4);
	}

	return addr_sys;
}


//通过旧协议地址和信道频率设置新协议模块地址
void set_new_mod_addr(band_para *para, u8 func, u8 addr)
{
	md_adr_info *pmd_adr = &para->md_adr_t;

	switch(func)
	{
		case DIG_PICO:
			pmd_adr->mod_type = MOD_TYPE_DIG;
			break;
		case PA_FUNC:
			pmd_adr->mod_type = MOD_TYPE_PA;
			break;
		default:
			return;
			break;
	}
	pmd_adr->mod_adr_t.mod_sub_adr.mod_id = addr>>4;
	pmd_adr->mod_adr_t.mod_sub_adr.mod_mimo = 0;
	pmd_adr->mod_adr_t.mod_sub_adr.mod_link = (addr & 0x08)>0 ? MOD_LINK_UL : MOD_LINK_DL;
	pmd_adr->mod_band = unit_para_t.unit_augment.oldsys_band_table[addr & 0x07];
}

//旧模块只设置下行频率，上行频率需要根据下行频率来计算
f32 dl_freq_to_ul_freq(u8 sysnum, f32 dl_freq)
{
	f32 ul_freq = 0;
	if(dl_freq >= 390 && dl_freq <= 396.5){
		ul_freq = dl_freq - 10;
	}else if(dl_freq >= 851 && dl_freq <= 869){
		ul_freq = dl_freq - 45;
	}else if(dl_freq >= 193 && dl_freq <= 199){
		ul_freq = dl_freq - 80;
	}else if(dl_freq >= 869.04 && dl_freq <= 870){
		ul_freq = dl_freq - 45;
	}else if(dl_freq >= 870.03 && dl_freq <= 893.97){
		ul_freq = dl_freq - 45;
	}else if(dl_freq >= 925 && dl_freq <= 960){
		ul_freq = dl_freq - 45;
	}else if(dl_freq >= 2110 && dl_freq <= 2170){
		ul_freq = dl_freq - 190;
	}else if(dl_freq >= 1805 && dl_freq <= 1880){
		ul_freq = dl_freq - 95;
	}else if (dl_freq >= 461 && dl_freq <= 466){
		ul_freq = dl_freq - 10;
	}

	return ul_freq;
}

void query_old2new(Rs485Comm *data)
{
	u8 sys_num,sys_sel, i;
	u8 index=0;

	sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
	sys_sel = (data->Addr & 0xf0)/SEL_ADDR_SKIP - SYS_ADDR_BASE ; /*选频选带模块的地址区分与其它模块的地址区分不一致*/
	index = oldaddr_find_mod(data->Func, data->Addr);
	if(index == 0xff) return ;

	set_new_mod_addr(&exmod_para_a[index], data->Func, data->Addr);
	para_temp_save_t.sysnum = sys_num;

	switch(data->Func)
	{
		case DIG_PICO:

			for(i=0; i<FREQ_CHANNEL_NUMS_MAX; ++i){
				exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = ((MOD_RP[sys_num].ICS_HT_RP.QueryRP.ICSType & 0x10) >> 4);
				exmod_dynamic_para_a[index].ch_rf_t.ul[i].agc_att = MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULCtrlDepth;
				// exmod_dynamic_para_a[index].ch_rf_t.ul[i].pin = MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULInputP;
				// exmod_dynamic_para_a[index].ch_rf_t.ul[i].pout = MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOutputP;
				exmod_dynamic_para_a[index].ch_rf_t.dl[i].agc_att = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLCtrlDepth;
				// exmod_dynamic_para_a[index].ch_rf_t.dl[i].pin = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLInputP;
				// exmod_dynamic_para_a[index].ch_rf_t.dl[i].pout = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOutputP;
				exmod_dynamic_para_a[index].md_dynamic_basic.pout.p = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOutputP;
				exmod_dynamic_para_a[index].md_dynamic_basic.pin.p = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLInputP;

				exmod_dynamic_para_a[index].alarm.ch_pin_ul_op[i] = MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x10;
				exmod_dynamic_para_a[index].alarm.ch_pin_dl_op[i] = MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x20;
			
			}
			// MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x01; //62005
			exmod_alarm_a[index].m_alarm.ad80305_1 = MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x02; //VCO1
			exmod_alarm_a[index].m_alarm.ad80305_2 = MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x04; //VCO2
			exmod_alarm_a[index].m_alarm.self_excited = MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x80; //自激告警
			exmod_dynamic_para_a[index].alarm.temp_h = MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x08;
			exmod_dynamic_para_a[index].alarm.temp_l = MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x40;

			exmod_dynamic_para_a[index].md_dynamic_sundry.temperature = MOD_RP[sys_num].ICS_HT_RP.QueryRP.FPGATp;
			
		break;
		case PA_FUNC:
			if(data->Addr & 0x08){
				exmod_dynamic_para_a[index].alarm.ch_pin_ul_op[0] = MOD_RP[sys_num].UL_PA1.st & 0x02;
				exmod_dynamic_para_a[index].alarm.temp_h = MOD_RP[sys_num].UL_PA1.st & 0x04;
				exmod_dynamic_para_a[index].alarm.rl = MOD_RP[sys_num].UL_PA1.st & 0x08; //驻波比告警
				exmod_dynamic_para_a[index].alarm.pa1 = MOD_RP[sys_num].UL_PA1.st & 0x10;
				exmod_dynamic_para_a[index].alarm.pout_pre = MOD_RP[sys_num].UL_PA1.st & 0x80;
				exmod_dynamic_para_a[index].alarm.iso_alarm = MOD_RP[sys_num].UL_PA1.st & 0x40; //自激告警 当做隔离度告警
				exmod_para_a[index].md_basic.sw = MOD_RP[sys_num].UL_PA1.st & 0x01;
				// exmod_para_a[index].md_basic.att = MOD_RP[sys_num].UL_PA1.att;
				exmod_para_a[index].md_sundry.dig_sundry.att_ul.pa_att = MOD_RP[sys_num].UL_PA1.att;
				para_temp_save_t.ul_pa_att = MOD_RP[sys_num].UL_PA1.att;

				exmod_dynamic_para_a[index].md_dynamic_sundry.temperature = MOD_RP[sys_num].UL_PA1.tp;
				exmod_dynamic_para_a[index].md_dynamic_basic.pout.p = MOD_RP[sys_num].UL_PA1.po;
				para_temp_save_t.ul_pa_pout = MOD_RP[sys_num].UL_PA1.po;
				exmod_dynamic_para_a[index].md_dynamic_basic.rpout.p = MOD_RP[sys_num].UL_PA1.npo;

			}else{
				exmod_dynamic_para_a[index].alarm.ch_pin_dl_op[0] = MOD_RP[sys_num].DL_PA1.st & 0x02;
				exmod_dynamic_para_a[index].alarm.temp_h = MOD_RP[sys_num].DL_PA1.st & 0x04;
				exmod_dynamic_para_a[index].alarm.rl = MOD_RP[sys_num].DL_PA1.st & 0x08; //驻波比告警
				exmod_dynamic_para_a[index].alarm.pa1 = MOD_RP[sys_num].DL_PA1.st & 0x10;
				exmod_dynamic_para_a[index].alarm.pout_pre = MOD_RP[sys_num].DL_PA1.st & 0x80;
				exmod_dynamic_para_a[index].alarm.iso_alarm = MOD_RP[sys_num].DL_PA1.st & 0x40; //自激告警 当做隔离度告警
				exmod_para_a[index].md_basic.sw = MOD_RP[sys_num].DL_PA1.st & 0x01;
				// exmod_para_a[index].md_basic.att = MOD_RP[sys_num].DL_PA1.att;
				exmod_para_a[index].md_sundry.dig_sundry.att_dl.pa_att = MOD_RP[sys_num].DL_PA1.att;
				para_temp_save_t.dl_pa_att = MOD_RP[sys_num].DL_PA1.att;

				exmod_dynamic_para_a[index].md_dynamic_sundry.temperature = MOD_RP[sys_num].DL_PA1.tp;
				exmod_dynamic_para_a[index].md_dynamic_basic.pout.p = MOD_RP[sys_num].DL_PA1.po;
				para_temp_save_t.dl_pa_pout = MOD_RP[sys_num].DL_PA1.po;
				exmod_dynamic_para_a[index].md_dynamic_basic.rpout.p = MOD_RP[sys_num].DL_PA1.npo;
			}
		break;
		default:break;
	}
}

void set_att_old2new(Rs485Comm *data)
{
	u8 sys_num;
	u8 index=0;

	sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
	index = oldaddr_find_mod(data->Func, data->Addr);
	if(index == 0xff) return ;

	switch(data->Func)
	{
		case DIG_PICO:

		break;
		case PA_FUNC:
			if(data->Addr & 0x08){

			}else{

			}
		break;
		default:break;
	}
}

void set_freq_old2new(Rs485Comm *data)
{
	u8 sys_num;
	u8 index=0;

	sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
	index = oldaddr_find_mod(data->Func, data->Addr);
	if(index == 0xff) return ;

	switch(data->Func)
	{
		case ICS_FUNC:
		case DIG_PICO:
			if (data->Data[1] == 0x01){
				exmod_para_a[index].md_sundry.dig_sundry.center_freq.ul = MOD_RP[sys_num].ICS_HT_RP.F1FREQ.a;
			}
			else if (data->Data[1] == 0x02){
				exmod_para_a[index].md_sundry.dig_sundry.center_freq.dl = MOD_RP[sys_num].ICS_HT_RP.F2FREQ.a;
			}
		break;
		default:break;
	}
}

void set_sw_old2new(Rs485Comm *data)
{
	u8 sys_num;
	u8 index=0;

	sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
	index = oldaddr_find_mod(data->Func, data->Addr);
	if(index == 0xff) return ;

	switch(data->Func)
	{
		case PA_FUNC:

		break;
		default:break;
	}
}

void set_agc_old2new(Rs485Comm *data)
{
	u8 sys_num;
	u8 index=0;

	sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
	index = oldaddr_find_mod(data->Func, data->Addr);
	if(index == 0xff) return ;

	switch(data->Func)
	{
		case PA_FUNC:
			if(data->Addr & 0x08){

			}else{

			}
		break;
		default:break;
	}
}

void set_agcsw_old2new(Rs485Comm *data)
{
	u8 sys_num;
	u8 index=0;

	sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
	index = oldaddr_find_mod(data->Func, data->Addr);
	if(index == 0xff) return ;

	switch(data->Func)
	{
		case PA_FUNC:
		case MPA_FUNC:
			if(data->Addr & 0x08){
				if(MOD_RP[sys_num].CHECK_RP.UL_AGC_SW)
					exmod_para_a[index].md_sundry.agc_mod = AGC_TARGET_POWER;
				else
					exmod_para_a[index].md_sundry.agc_mod = AGC_MANUAL;
			}else{
				if(MOD_RP[sys_num].CHECK_RP.DL_AGC_SW)
					exmod_para_a[index].md_sundry.agc_mod = AGC_TARGET_POWER;
				else
					exmod_para_a[index].md_sundry.agc_mod = AGC_MANUAL;
			}
		break;
		default:break;
	}
}

void set_ip_old2new(Rs485Comm *data)
{
	u8 sys_num;
	u8 index=0;
	
	sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
	index = oldaddr_find_mod(data->Func, data->Addr);
	if(index == 0xff) return ;

	switch(data->Func)
	{
		case PA_FUNC:
			if(data->Addr & 0x08){
				exmod_dynamic_para_a[index].md_dynamic_basic.pin.p = MOD_RP[sys_num].PWR_RP.UL_PA1_PI;
			}else{
				exmod_dynamic_para_a[index].md_dynamic_basic.pin.p = MOD_RP[sys_num].PWR_RP.DL_PA1_PI;
			}
		break;
		default:break;
	}
}

void set_op_old2new(Rs485Comm *data)
{
	u8 sys_num;
	u8 index=0;

	sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
	index = oldaddr_find_mod(data->Func, data->Addr);
	if(index == 0xff) return ;

	switch(data->Func)
	{
		case PA_FUNC:
			if(data->Addr & 0x08){
				exmod_dynamic_para_a[index].md_dynamic_basic.pout.p = MOD_RP[sys_num].PWR_RP.UL_PA1_PO;
			}else{
				exmod_dynamic_para_a[index].md_dynamic_basic.pout.p = MOD_RP[sys_num].PWR_RP.DL_PA1_PO;
			}
		break;
		default:break;
	}
}

void set_ics_old2new(Rs485Comm *data)
{
	u8 sys_num, i=0;
	u8 index=0, chnum;
	u8 tmp;
	s8 power;
	f32 workfreq[ICSMAXCHNUM];
	sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
	index = oldaddr_find_mod(data->Func, data->Addr);
	if(index == 0xff) return ;

	if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 1){
		chnum = MOD_RP[sys_num].ICS_HT_RP.WK_CH.CHN>FREQ_CHANNEL_NUMS_MAX*2 ? FREQ_CHANNEL_NUMS_MAX*2 : MOD_RP[sys_num].ICS_HT_RP.WK_CH.CHN;
		chnum >>= 1;
	}else{
		chnum = MOD_RP[sys_num].ICS_HT_RP.WK_CH.CHN>FREQ_CHANNEL_NUMS_MAX ? FREQ_CHANNEL_NUMS_MAX : MOD_RP[sys_num].ICS_HT_RP.WK_CH.CHN;
	}

	
	switch(data->Func)
	{
		case ICS_FUNC:
		break;
		case DIG_PICO:
			switch (data->Data[1])
			{
				case 0x02:
					
					if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 0){//固定选带
						for(i=0; i<chnum; ++i){
							exmod_para_a[index].ch_info_t.bandwidth[i] = 0;
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.enable = 1;
							
							exmod_para_a[index].ch_info_t.dl[i].workfreq = MOD_RP[sys_num].ICS_HT_RP.ULFREQ.Arr[i].a;
							exmod_para_a[index].ch_info_t.ul[i].workfreq = dl_freq_to_ul_freq(sys_num, MOD_RP[sys_num].ICS_HT_RP.ULFREQ.Arr[i].a);
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_min_int = INVALID_VALUE;
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_min_float = 0;
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_max_int = INVALID_VALUE;
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_max_float = 0;
							exmod_para_a[index].ch_info_t.tech[i] = INVALID_VALUE;
							exmod_dynamic_para_a[index].md_wireless_net_t.modem_tech_a[i].tech = INVALID_VALUE;
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.index = i+1; //信道从1开始
						}
					}
					// exmod_dynamic_para_a[index].ch_rf_t.freq_ul[i].workfreq = (f32)GET_FREQ(MOD_RP[sys_num].ICS_HT_RP.WK_CH.CH[i], UL, sys_num);
					// exmod_dynamic_para_a[index].ch_rf_t.freq_dl[i].workfreq = (f32)GET_FREQ(MOD_RP[sys_num].ICS_HT_RP.WK_CH.CH[i], DL, sys_num);

					if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 1){//可变选带
						for(i=0; i<(chnum<<1); ++i){
							workfreq[i] = MOD_RP[sys_num].ICS_HT_RP.ULFREQ.Arr[i].a;
						}
						for(i=0; i<chnum; ++i){
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.enable = 1;
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_min_int = INVALID_VALUE;
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_min_float = 0;
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_max_int = INVALID_VALUE;
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_max_float = 0;
							exmod_para_a[index].ch_info_t.tech[i] = INVALID_VALUE;
							exmod_dynamic_para_a[index].md_wireless_net_t.modem_tech_a[i].tech = INVALID_VALUE;
							exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.index = i+1; //信道从1开始

							exmod_para_a[index].ch_info_t.bandwidth[i] = workfreq[i*2+1] \
							- workfreq[i*2];

							exmod_para_a[index].ch_info_t.dl[i].workfreq = workfreq[i*2] \
							+ exmod_para_a[index].ch_info_t.bandwidth[i]/2;

							exmod_para_a[index].ch_info_t.ul[i].workfreq = dl_freq_to_ul_freq(sys_num, workfreq[i*2]) \
							+ exmod_para_a[index].ch_info_t.bandwidth[i]/2;
						}
					}
					
				break;
				case 0x06:
					for(i=0; i<chnum; ++i){
						if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 1){
							tmp = i*2+1;
						}else{
							tmp = i;
						}

						if( tmp < 8)
						{
							if((MOD_RP[sys_num].ICS_HT_RP.CHSW.CHSW[2] >> tmp) & 0x01){
								exmod_para_a[index].ch_info_t.dl[i].sw = 1;
							}else{
								exmod_para_a[index].ch_info_t.dl[i].sw = 0;
							}
						}
						else if( (tmp >= 8) && (tmp < 16))
						{
							tmp -= 8;
							if((MOD_RP[sys_num].ICS_HT_RP.CHSW.CHSW[1] >> tmp) & 0x01){
								exmod_para_a[index].ch_info_t.dl[i].sw = 1;
							}else{
								exmod_para_a[index].ch_info_t.dl[i].sw = 0;
							}
						}
						else if( (tmp >= 17) && (tmp < 24))
						{
							tmp -= 16;
							if((MOD_RP[sys_num].ICS_HT_RP.CHSW.CHSW[0] >> tmp) & 0x01){
								exmod_para_a[index].ch_info_t.dl[i].sw = 1;
							}else{
								exmod_para_a[index].ch_info_t.dl[i].sw = 0;
							}
						}
					}
				break;
				case 0x30:
					for(i=0; i<chnum; ++i){
						exmod_para_a[index].ch_info_t.ul[i].pin_op_th = MOD_RP[sys_num].ICS_HT_RP.ULSL.INOverGa;
						exmod_para_a[index].ch_info_t.ul[i].mute_th_h = MOD_RP[sys_num].ICS_HT_RP.ULSL.slHIGHGA;
						exmod_para_a[index].ch_info_t.ul[i].mute_th_l = MOD_RP[sys_num].ICS_HT_RP.ULSL.slLOWGA;
						exmod_para_a[index].ch_info_t.ul[i].mute_sw = MOD_RP[sys_num].ICS_HT_RP.ULSL.slSW;
					}
					exmod_para_a[index].md_augment.max_agc_th.max_agc_th_ul = MOD_RP[sys_num].ICS_HT_RP.ULSL.AGCDepth;
					exmod_para_a[index].md_sundry.dig_sundry.att_ul.lna_att = MOD_RP[sys_num].ICS_HT_RP.ULSL.ManualAtt;
					exmod_para_a[index].md_sundry.dig_sundry.ics_sw = MOD_RP[sys_num].ICS_HT_RP.DLSL.ICSSW;
				break;
				case 0x31:
					for(i=0; i<chnum; ++i){
						exmod_para_a[index].ch_info_t.dl[i].pin_op_th = MOD_RP[sys_num].ICS_HT_RP.DLSL.INOverGa;
						exmod_para_a[index].ch_info_t.dl[i].mute_th_h = MOD_RP[sys_num].ICS_HT_RP.DLSL.slHIGHGA;
						exmod_para_a[index].ch_info_t.dl[i].mute_th_l = MOD_RP[sys_num].ICS_HT_RP.DLSL.slLOWGA;
						exmod_para_a[index].ch_info_t.dl[i].mute_sw = MOD_RP[sys_num].ICS_HT_RP.DLSL.slSW;
					}
					exmod_para_a[index].md_augment.max_agc_th.max_agc_th_dl = MOD_RP[sys_num].ICS_HT_RP.DLSL.AGCDepth;
					exmod_para_a[index].md_sundry.dig_sundry.att_dl.lna_att = MOD_RP[sys_num].ICS_HT_RP.DLSL.ManualAtt;
					exmod_para_a[index].md_sundry.dig_sundry.ics_sw = MOD_RP[sys_num].ICS_HT_RP.DLSL.ICSSW;
				break;
				case 0x32:
					for(i=0; i<chnum; ++i){
						exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = ((MOD_RP[sys_num].ICS_HT_RP.QueryRP.ICSType & 0x10) >> 4);
					}
				break;
				case 0x07:
					exmod_para_a[index].md_augment.max_agc_th.max_agc_th_ul = MOD_RP[sys_num].ICS_HT_RP.ULSL.AGCDepth;
				break;
				case 0x08:
					exmod_para_a[index].md_augment.max_agc_th.max_agc_th_dl = MOD_RP[sys_num].ICS_HT_RP.DLSL.AGCDepth;
				break;
				case 0x09:
					for(i=0; i<chnum; ++i){
						exmod_para_a[index].ch_info_t.ul[i].pin_op_th = MOD_RP[sys_num].ICS_HT_RP.ULSL.INOverGa;
					}
				break;
				case 0x0a:
					for(i=0; i<chnum; ++i){
						exmod_para_a[index].ch_info_t.dl[i].pin_op_th = MOD_RP[sys_num].ICS_HT_RP.DLSL.INOverGa;
					}
				break;
				case 0x0b:
					exmod_para_a[0].md_sundry.dig_sundry.att_ul.lna_att = MOD_RP[sys_num].ICS_HT_RP.ULSL.ManualAtt;
				break;
				case 0x0c:
					exmod_para_a[0].md_sundry.dig_sundry.att_dl.lna_att = MOD_RP[sys_num].ICS_HT_RP.DLSL.ManualAtt;
				break;
				case 0x0d:
					exmod_para_a[index].md_sundry.dig_sundry.ics_sw = MOD_RP[sys_num].ICS_HT_RP.DLSL.ICSSW;
				break;
				case 0x0e:
					exmod_para_a[index].md_sundry.dig_sundry.ics_sw = MOD_RP[sys_num].ICS_HT_RP.DLSL.ICSSW;
				break;
				case 0x0f:
					for(i=0; i<chnum; ++i){
						exmod_para_a[index].ch_info_t.ul[i].mute_th_h = MOD_RP[sys_num].ICS_HT_RP.ULSL.slHIGHGA;
						exmod_para_a[index].ch_info_t.ul[i].mute_th_l = MOD_RP[sys_num].ICS_HT_RP.ULSL.slLOWGA;
					}
				break;
				case 0x10:
					for(i=0; i<chnum; ++i){
						exmod_para_a[index].ch_info_t.dl[i].mute_th_h = MOD_RP[sys_num].ICS_HT_RP.DLSL.slHIGHGA;
						exmod_para_a[index].ch_info_t.dl[i].mute_th_l = MOD_RP[sys_num].ICS_HT_RP.DLSL.slLOWGA;
					}
				break;
				case 0x11:
					for(i=0; i<chnum; ++i){
						exmod_para_a[index].ch_info_t.ul[i].mute_sw = MOD_RP[sys_num].ICS_HT_RP.ULSL.slSW;
					}
				break;
				case 0x12:
					for(i=0; i<chnum; ++i){
						exmod_para_a[index].ch_info_t.dl[i].mute_sw = MOD_RP[sys_num].ICS_HT_RP.DLSL.slSW;
					}
				break;
				case 0x13:
					for(i = 0; i < chnum; ++i){
						// if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 1){
						//     tmp = i*2;
						// }else{
						//     tmp = i;
						// }

						tmp = i;

						if(exmod_para_a[index].ch_info_t.dl[i].sw)
							exmod_dynamic_para_a[index].ch_rf_t.ul[i].pin = MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULIP[tmp];
						else 
							exmod_dynamic_para_a[index].ch_rf_t.ul[i].pin = -127;
					}
				break;
				case 0x14:
					for(i = 0; i < chnum; ++i){
						// if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 1){
						//     tmp = i*2;
						// }else{
						//     tmp = i;
						// }

						tmp = i;

						power = MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOP[tmp] + unit_para_t.band_whole.ul_gain_adjust[i] - para_temp_save_t.ul_pa_att - 1;
						if((power>=para_temp_save_t.ul_pa_pout) && (-126 != (para_temp_save_t.ul_pa_pout))){
							power = para_temp_save_t.ul_pa_pout;
						}else{
							//@luke20170729 当pa不存在时，功放的过功率门限位-126，则此时信道功率的计算以pa的att位0来计算，并显示
							power = MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOP[tmp] \
							+ unit_para_t.band_whole.ul_gain_adjust[i] - 1;
						}
						// power = MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOP[tmp] \
						// + unit_para_t.band_whole.ul_gain_adjust[i] - 1;

						if(!exmod_para_a[index].ch_info_t.dl[i].sw)
							power = -127;

						exmod_dynamic_para_a[index].ch_rf_t.ul[i].pout = power; 
					}
				break;
				case 0x15:
					for(i = 0; i < chnum; ++i){
						// if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 1){
						//     tmp = i*2;
						// }else{
						//     tmp = i;
						// }

						tmp = i;

						if(exmod_para_a[index].ch_info_t.dl[i].sw)
							exmod_dynamic_para_a[index].ch_rf_t.dl[i].pin = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLIP[tmp];
						else 
							exmod_dynamic_para_a[index].ch_rf_t.dl[i].pin = -127;
					}
				break;
				case 0x16:
					for(i = 0; i < chnum; ++i){
						// if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 1){
						//     tmp = i*2;
						// }else{
						//     tmp = i;
						// }

						tmp = i;

						power = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOP[tmp] + unit_para_t.band_whole.dl_gain_adjust[i] - para_temp_save_t.dl_pa_att - 1;
						if((power>=para_temp_save_t.dl_pa_pout) && (-126 != (para_temp_save_t.dl_pa_pout))){
							power = para_temp_save_t.dl_pa_pout;
						}else{
							//@luke20170729 当pa不存在时，功放的过功率门限位-126，则此时信道功率的计算以pa的att位0来计算，并显示
							power = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOP[tmp] \
							+ unit_para_t.band_whole.dl_gain_adjust[i] - 1;
						}
						// power = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOP[tmp] \
						// + unit_para_t.band_whole.dl_gain_adjust[i] - 1;

						if(!exmod_para_a[index].ch_info_t.dl[i].sw)
							power = -127;

						exmod_dynamic_para_a[index].ch_rf_t.dl[i].pout = power;
					}
				break;
				case 0xe0:
					for(i=0; i<chnum; ++i){
						if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 1){
							tmp = i*2+1;
						}else{
							tmp = i;
						}
						exmod_para_a[index].ch_info_t.ul[i].att = MOD_RP[sys_num].ICS_HT_RP.ch_ul_att.dat[tmp];
					}
				break;
				case 0xe1:
					for(i=0; i<chnum; ++i){
						if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 1){
							tmp = i*2+1;
						}else{
							tmp = i;
						}
						exmod_para_a[index].ch_info_t.dl[i].att = MOD_RP[sys_num].ICS_HT_RP.ch_dl_att.dat[tmp];
					}
				break;
				case 0xe2:
					for(i=0; i<chnum; ++i){
						if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 1){
							tmp = i*2+1;
						}else{
							tmp = i;
						}
						exmod_para_a[index].ch_info_t.ul[i].agc_th = MOD_RP[sys_num].ICS_HT_RP.ch_ul_agc.dat[tmp];
					}
				break;
				case 0xe3:
					for(i=0; i<chnum; ++i){
						if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = 1){
							tmp = i*2+1;
						}else{
							tmp = i;
						}
						exmod_para_a[index].ch_info_t.dl[i].agc_th = MOD_RP[sys_num].ICS_HT_RP.ch_dl_agc.dat[tmp];
					}
				break;
				case 0xe4:
					exmod_para_a[index].md_basic.blocking_compensation = MOD_RP[sys_num].ICS_HT_RP.block_compensate;
				break;
				default:
					break;
			}
		break;
		default:break;
	}
}

void set_sn_old2new(Rs485Comm *data)
{
	u8 sys_num;
	u8 index=0;

	sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
	index = oldaddr_find_mod(data->Func, data->Addr);
	if(index == 0xff) return ;

	set_new_mod_addr(&exmod_para_a[index], data->Func, data->Addr);
	para_temp_save_t.sysnum = sys_num;

	switch(data->Func)
	{
		case ICS_FUNC:
		case DIG_PICO:
		case PA_FUNC:
			memset(exmod_para_a[index].md_mfrs.sn, 0, sizeof(exmod_para_a[index].md_mfrs.sn));
			memcpy(exmod_para_a[index].md_mfrs.sn, &data->Data[1], 10);
		break;
		default:break;
	}
}

void protocol_old2new(Rs485Comm *Rstr)
{
	//pthread_mutex_lock(&exmod_para_mutex);
	switch(Rstr->Cmd)
	{
	  	case QUERY:          /*模块状态查询*/
	  		query_old2new(Rstr);
	  	break;
	  	case SETATT:         /*设置ATT*/
	  		set_att_old2new(Rstr);
	  	break;  
	  	case SETFREQ:         /*设置频率*/
	  		set_freq_old2new(Rstr);
	  	break;
	  	case SETSW:         /*设置功放及信道开关*/
	  		set_sw_old2new(Rstr);
	  	break;	
		#if 0
	  	case SETGA:        /*设置功率门限*/
	  	case GETOVERPT:    /*获取功率门限*/
	  		SETGA_RP(Rstr);	
	  	break;
		
	  	case SETAGCGA:     /*设置AGC门限*/
	  		set_agc_old2new(Rstr);	
	  	break;
		#endif
		 
	  	// case GETUNDERPT:     /*查询欠功率门限值*/
	  	// case SETUNDERPT:     /*设置过功率门限值*/	
	  	// 	UNDERPT_RP(Rstr);	
	  	// break;
		 
	  	case SETAGCSW:       /*设置查询自动AGC开关*/
	  		set_agcsw_old2new(Rstr);
	  	break;
	  	case GETIP:          /*获取输入功率*/
	  		set_ip_old2new(Rstr);
	  	break;
	  	case GETOP:          /*获取输出功率*/
	  		set_op_old2new(Rstr);
	  	break;
		
	  	// case HEGETTOTALATT:  /*获取整机总ATT值*/
	  	// 	HEGETTOTALATT_RP(Rstr);
	  	// break;	

		case SETICS:
			set_ics_old2new(Rstr);       /*与ICS相关设置*/
		break;	
		// case 0x3d:
		// SET_BANDSW_RP(Rstr);/*设置单独子带开关*/
		// break;
		case SETSN:
			set_sn_old2new(Rstr);      /*读取,设置模块系列号*/
		break;
		// case 0x91:
		//     SETODU_RP(Rstr);/*与ODU相关设置*/
		// break;  	
		default:break;
	} 
	//pthread_mutex_unlock(&exmod_para_mutex);
}

//重新设置信道agc
void reset_ch_agc_val()
{
	u8 cnt;
	ch_link link[FREQ_CHANNEL_NUMS_MAX];

	RLDEBUG("ul_ch_agc: ");
	for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
		link[cnt].agc_th = unit_para_t.band_whole.band_restrict_ul[unit_dynamic_para_t.band_current].ch_agc_th[cnt] \
			- unit_para_t.band_whole.band_restrict_ul[0].max_gain + unit_para_t.band_whole.band_restrict_ul[0].ch_max_gain[0] \
			+ para_temp_save_t.ul_pa_att + 1 - unit_para_t.band_whole.passive_offset[0].bts;
		
		RLDEBUG("%d ", link[cnt].agc_th);
	}
	RLDEBUG("\n");

	rs485_ch_agc_send(para_temp_save_t.sysnum, (void*)link, UL);

	RLDEBUG("dl_ch_agc: ");
	for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
		link[cnt].agc_th = unit_para_t.band_whole.band_restrict_dl[unit_dynamic_para_t.band_current].ch_agc_th[cnt] \
			- unit_para_t.band_whole.band_restrict_dl[0].max_gain + unit_para_t.band_whole.band_restrict_dl[0].ch_max_gain[0] \
			+ para_temp_save_t.dl_pa_att + 1 - unit_para_t.band_whole.passive_offset[0].ms;

		RLDEBUG("%d ", link[cnt].agc_th);
	}
	RLDEBUG("\n");
	rs485_ch_agc_send(para_temp_save_t.sysnum, (void*)link, DL);
}

//工作频率
s16 check_ul_workfreq_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt, sysnum;
	f32 *pf;
	ch_link *pch;


	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	sysnum = newaddr_to_sysnum(md_adr);


	if(sysnum == 0xff) return -1;

	pf = (f32*)remote;
	pch = (ch_link*)local;
	

	if (PARA_RW == flag) {
		// for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
		// 	pch[cnt].workfreq = pf[cnt];
		// }

		//rs485_workfreq_send(newaddr_to_sysnum(md_adr), local);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pf[cnt] = pch[cnt].workfreq;
		}
	} else {
		return -1;
	}

	return 0;
}
//工作频率
s16 check_dl_workfreq_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt, sysnum;
	f32 *pf;
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	sysnum = newaddr_to_sysnum(md_adr);

	if(sysnum == 0xff) return -1;

	pf = (f32*)remote;
	pch = (ch_link*)local;
	
	if (PARA_RW == flag) {
		printf("web set workfreq:");
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].workfreq = pf[cnt];
			printf("%8.4f,", pch[cnt].workfreq );
		}
		printf("\n");
		rs485_workfreq_send(newaddr_to_sysnum(md_adr), local);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pf[cnt] = pch[cnt].workfreq;
		}
	} else {
		return -1;
	}

	return 0;
}
//上行模块信道agc门限
s16 check_ul_ch_agc_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	ch_link *pch;

	if ((NULL == remote) || (NULL == local) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		if(!ch_agc_check(p, md_adr, UL)){
			for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
				unit_para_t.band_whole.band_restrict_ul[unit_dynamic_para_t.band_current].ch_agc_th[cnt] = p[cnt];

				pch[cnt].agc_th = p[cnt] - unit_para_t.band_whole.band_restrict_ul[0].max_gain + \
				unit_para_t.band_whole.band_restrict_ul[0].ch_max_gain[0] + para_temp_save_t.ul_pa_att + 1 - unit_para_t.band_whole.passive_offset[0].bts;
			}
			rs485_ch_agc_send(newaddr_to_sysnum(md_adr), local, UL);
			data_update(DATA_TYPE_UNIT);
		}else{
			return -1;
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			//p[cnt] = pch[cnt].agc_th;
			p[cnt] = unit_para_t.band_whole.band_restrict_ul[unit_dynamic_para_t.band_current].ch_agc_th[cnt];
		}
	} else {
		return -1;
	}

	return 0;
}
//下行模块信道agc门限
s16 check_dl_ch_agc_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	ch_link *pch;

	if ((NULL == remote) || (NULL == local) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		if(!ch_agc_check(p, md_adr, DL)){
			for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
				unit_para_t.band_whole.band_restrict_dl[unit_dynamic_para_t.band_current].ch_agc_th[cnt] = p[cnt];

				pch[cnt].agc_th = p[cnt] - unit_para_t.band_whole.band_restrict_dl[0].max_gain + \
				unit_para_t.band_whole.band_restrict_dl[0].ch_max_gain[0] + para_temp_save_t.dl_pa_att + 1 - unit_para_t.band_whole.passive_offset[0].ms;
			}
			rs485_ch_agc_send(newaddr_to_sysnum(md_adr), local, DL);
			data_update(DATA_TYPE_UNIT);
		}else{
			return -1;
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			// p[cnt] = pch[cnt].agc_th;
			p[cnt] = unit_para_t.band_whole.band_restrict_dl[unit_dynamic_para_t.band_current].ch_agc_th[cnt];
		}
	} else {
		return -1;
	}

	return 0;
}
//上行att门限
s16 check_ul_ch_att_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	ch_link *pch;

	if ((NULL == remote) || (NULL == local) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].att = p[cnt];
		}
		rs485_ch_att_send(newaddr_to_sysnum(md_adr), local, UL);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].att;
		}
	} else {
		return -1;
	}

	return 0;
}
//下行att门限
s16 check_dl_ch_att_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	ch_link *pch;

	if ((NULL == remote) || (NULL == local) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].att = p[cnt];
		}
		rs485_ch_att_send(newaddr_to_sysnum(md_adr), local, DL);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].att;
		}
	} else {
		return -1;
	}

	return 0;
}
//信道开关
s16 check_ch_sw_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u8 *p;
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	p = (u8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		if(!ch_agc_check_sw(p, md_adr, UL) && !ch_agc_check_sw(p, md_adr, DL)){
			for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
				pch[cnt].sw = p[cnt];
			}
			rs485_ch_sw_send(newaddr_to_sysnum(md_adr), local);
		}else{
			return -1;
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].sw;
		}
	} else {
		return -1;
	}

	return 0;
}
//上行输入过功率门限
s16 check_ul_ch_pin_op_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8* p;
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].pin_op_th = p[cnt];
		}
		rs485_ch_pin_op_th_send(newaddr_to_sysnum(md_adr), local, UL);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].pin_op_th;
		}
	} else {
		return -1;
	}

	return 0;
}
//下行输入过功率门限
s16 check_dl_ch_pin_op_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8* p;
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].pin_op_th = p[cnt];
		}
		rs485_ch_pin_op_th_send(newaddr_to_sysnum(md_adr), local, DL);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].pin_op_th;
		}
	} else {
		return -1;
	}

	return 0;
}
//移频频率
s16 check_shiftfreq_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *pf;
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	pf = (f32*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].shiftfreq = pf[cnt];
		}
		//TODO:
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pf[cnt] = pch[cnt].shiftfreq;
		}
	} else {
		return -1;
	}

	return 0;
}
//静燥开关
s16 check_ch_mute_sw_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u8 *p;
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	p = (u8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].mute_sw = p[cnt];
		}
		rs485_ch_mute_sw_send(newaddr_to_sysnum(md_adr), local);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].mute_sw;
		}
	} else {
		return -1;
	}

	return 0;
}
//静燥高门限
s16 check_ch_mute_h_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	ch_link *pch;

	if ((NULL == remote) || (NULL == local) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].mute_th_h = p[cnt];
		}
		//rs485_ch_mute_th_send(newaddr_to_sysnum(md_adr), local);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].mute_th_h;
		}
	} else {
		return -1;
	}

	return 0;
}
//静燥低门限
s16 check_ch_mute_l_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	ch_link *pch;

	if ((NULL == remote) || (NULL == local) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].mute_th_l = p[cnt];
		}
		rs485_ch_mute_th_send(newaddr_to_sysnum(md_adr), local);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].mute_th_l;
		}
	} else {
		return -1;
	}

	return 0;
}
//输出欠功率
s16 check_ch_pin_lp_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8* p;
	ch_link *pch;
	u32 tem = 0;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].pin_lp_th = p[cnt];
		}
		//TODO:
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].pin_lp_th;
		}
	} else {
		return -1;
	}

	return 0;
}

s16 check_ul_center_freq_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		*(f32*)(local) = *(f32*)(remote);
		rs485_center_freq_send(newaddr_to_sysnum(md_adr), local, UL);
	} else if (PARA_RD == flag) {
		*(f32*)(remote) = *(f32*)(local);
	} else {
		return -1;
	}

	return 0;
}

s16 check_dl_center_freq_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		*(f32*)(local) = *(f32*)(remote);
		rs485_center_freq_send(newaddr_to_sysnum(md_adr), local, DL);
	} else if (PARA_RD == flag) {
		*(f32*)(remote) = *(f32*)(local);
	} else {
		return -1;
	}

	return 0;
}

s16 check_ch_bandwidth_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *pf;
	f32 *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	pf = (f32*)remote;
	pch = (f32*)local;

	if (PARA_RW == flag) {
		// dig_adr = 0xe8;
		RLDEBUG("web set bandwidth:");
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt] = pf[cnt];
			printf("%8.4f,", pch[cnt]);
		}
		RLDEBUG("\n");
		// rs485_ch_bandwidth_send(newaddr_to_sysnum(md_adr), local);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pf[cnt] = pch[cnt];
		}
	} else {
		return -1;
	}

	return 0;
}

s16 check_ch_traffic_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].traffic_th = p[cnt];
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].traffic_th;
		}
	} else {
		return -1;
	}

	return 0;
}

s16 check_blocking_compensation_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 *blocking_compensation, *src;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	blocking_compensation = (u8*)local;
	src = (u8*)remote;

	if (PARA_RW == flag) {
		*blocking_compensation = *src;
		rs485_blocking_compensation_send(newaddr_to_sysnum(md_adr), local);
	} else if (PARA_RD == flag) {
		*src = *blocking_compensation;
	} else {
		return -1;
	}

	return 0;
}

s16 check_ul_max_agc_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	s8 *max_agc_th, *src;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	max_agc_th = (s8*)local;
	src = (s8*)remote;

	if (PARA_RW == flag) {
		*max_agc_th = *src;
		rs485_max_agc_th_send(newaddr_to_sysnum(md_adr), local, UL);
	} else if (PARA_RD == flag) {
		*src = *max_agc_th;
	} else {
		return -1;
	}

	return 0;
}

s16 check_dl_max_agc_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	s8 *max_agc_th, *src;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	max_agc_th = (s8*)local;
	src = (s8*)remote;

	if (PARA_RW == flag) {
		*max_agc_th = *src;
		rs485_max_agc_th_send(newaddr_to_sysnum(md_adr), local, DL);
	} else if (PARA_RD == flag) {
		*src = *max_agc_th;
	} else {
		return -1;
	}

	return 0;
}

s16 check_ul_att_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 *att_th, *src;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	att_th = (u8*)local;
	src = (u8*)remote;

	if (PARA_RW == flag) {
		*att_th = *src;
		rs485_att_th_send(newaddr_to_sysnum(md_adr), local, UL);
	} else if (PARA_RD == flag) {
		*src = *att_th;
	} else {
		return -1;
	}

	return 0;
}

s16 check_dl_att_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 *att_th, *src;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	att_th = (u8*)local;
	src = (u8*)remote;

	if (PARA_RW == flag) {
		*att_th = *src;
		rs485_att_th_send(newaddr_to_sysnum(md_adr), local, DL);
	} else if (PARA_RD == flag) {
		*src = *att_th;
	} else {
		return -1;
	}

	return 0;
}

s16 check_pa_att_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 *att_th, *src, cnt;
	// s8 agc[FREQ_CHANNEL_NUMS_MAX];
	ch_link link[FREQ_CHANNEL_NUMS_MAX];

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	att_th = (u8*)local;
	src = (u8*)remote;

	if (PARA_RW == flag) {
		*att_th = *src;
		if(md_adr->mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_UL){
			rs485_pa_att_th_send(newaddr_to_sysnum(md_adr), local, UL);
			para_temp_save_t.ul_pa_att = *att_th;
			for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
				link[cnt].agc_th = unit_para_t.band_whole.band_restrict_ul[unit_dynamic_para_t.band_current].ch_agc_th[cnt] \
				 - unit_para_t.band_whole.band_restrict_ul[0].max_gain + unit_para_t.band_whole.band_restrict_ul[0].ch_max_gain[0] \
				 + *att_th + 1 - unit_para_t.band_whole.passive_offset[0].bts;
			}
			rs485_ch_agc_send(newaddr_to_sysnum(md_adr), &link, UL);
		}else if(md_adr->mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_DL){
			rs485_pa_att_th_send(newaddr_to_sysnum(md_adr), local, DL);
			para_temp_save_t.dl_pa_att = *att_th;
			for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
				link[cnt].agc_th = unit_para_t.band_whole.band_restrict_dl[unit_dynamic_para_t.band_current].ch_agc_th[cnt] \
				 - unit_para_t.band_whole.band_restrict_dl[0].max_gain + unit_para_t.band_whole.band_restrict_dl[0].ch_max_gain[0] \
				 + *att_th + 1 - unit_para_t.band_whole.passive_offset[0].ms;
			}
			rs485_ch_agc_send(newaddr_to_sysnum(md_adr), &link, DL);
		}else 
			return -1;
	} else if (PARA_RD == flag) {
		*src = *att_th;
	} else {
		return -1;
	}

	return 0;
}






s8 exmod_data_restore(md_adr_info *md_adr, u8 mod_index)
{
	u8 sys_num;

	if(md_adr == NULL) return -1;

	sys_num = newaddr_to_sysnum(md_adr);

	switch (md_adr->mod_type){
		case MOD_TYPE_DIG:
			rs485_workfreq_send(sys_num, (void*)exmod_para_a[mod_index].ch_info_t.dl);
			rs485_ch_agc_send(sys_num, (void*)exmod_para_a[mod_index].ch_info_t.ul, UL);
			rs485_ch_agc_send(sys_num, (void*)exmod_para_a[mod_index].ch_info_t.dl, DL);
			rs485_ch_att_send(sys_num, (void*)exmod_para_a[mod_index].ch_info_t.ul, UL);
			rs485_ch_att_send(sys_num, (void*)exmod_para_a[mod_index].ch_info_t.dl, DL);
			rs485_ch_sw_send(sys_num, (void*)exmod_para_a[mod_index].ch_info_t.dl);
			rs485_ch_pin_op_th_send(sys_num, (void*)exmod_para_a[mod_index].ch_info_t.ul, UL);
			rs485_ch_pin_op_th_send(sys_num, (void*)exmod_para_a[mod_index].ch_info_t.dl, DL);
			rs485_center_freq_send(sys_num, (void*)&exmod_para_a[mod_index].md_sundry.dig_sundry.center_freq.ul, UL);
			rs485_center_freq_send(sys_num, (void*)&exmod_para_a[mod_index].md_sundry.dig_sundry.center_freq.dl, DL);
			rs485_blocking_compensation_send(sys_num, (void*)&exmod_para_a[mod_index].md_basic.blocking_compensation);
			rs485_max_agc_th_send(sys_num, (void*)&exmod_para_a[mod_index].md_augment.max_agc_th.max_agc_th_ul, UL);
			rs485_max_agc_th_send(sys_num, (void*)&exmod_para_a[mod_index].md_augment.max_agc_th.max_agc_th_dl, DL);
			rs485_att_th_send(sys_num, (void*)&exmod_para_a[mod_index].md_sundry.dig_sundry.att_ul.lna_att, UL);
			rs485_att_th_send(sys_num, (void*)&exmod_para_a[mod_index].md_sundry.dig_sundry.att_dl.lna_att, DL);
			rs485_ch_mute_sw_send(sys_num, (void*)exmod_para_a[mod_index].ch_info_t.ul);
			rs485_ch_mute_th_send(sys_num, (void*)exmod_para_a[mod_index].ch_info_t.ul);
			break;
		case MOD_TYPE_PA:
			if(md_adr->mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_UL){
				rs485_pa_att_th_send(sys_num, (void*)&exmod_para_a[mod_index].md_sundry.dig_sundry.att_ul.pa_att, UL);
			}else if(md_adr->mod_adr_t.mod_sub_adr.mod_link == MOD_LINK_DL){
				rs485_pa_att_th_send(sys_num, (void*)&exmod_para_a[mod_index].md_sundry.dig_sundry.att_dl.pa_att, DL);
			}
			break;
		default:
			break;
	}

	return 0;
}