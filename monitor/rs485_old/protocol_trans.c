/*
 * File      : protocol_trans.c
 * 
 *新旧协议转换
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-06-26      sujj      first version
*/
#include "protocol_trans.h"
#include "helios.h"
#include "Module_Struct.h"
#include "Module_Comm.h"
#include "para_def.h"
#include <string.h>
#include <stdio.h>
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
        if(unit_para_t.oldsys_band_table[i] == 0){
            unit_para_t.oldsys_band_table[i] = i+1;
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
    pmd_adr->mod_band = unit_para_t.oldsys_band_table[addr & 0x07];
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
            }
            //TODO:alarm
            // MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x01; //62005
            // MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x02; //VCO1
            // MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x04; //VCO2
            // MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x80; //自激告警
            exmod_dynamic_para_a[index].alarm.temp_h = MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x08;
            exmod_dynamic_para_a[index].alarm.temp_l = MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x40;
            // exmod_dynamic_para_a[index].alarm.ch_pin_ul_op = MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x10;
            // exmod_dynamic_para_a[index].alarm.ch_pin_dl_op = MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x20;
            MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm & 0x80;
            exmod_dynamic_para_a[index].md_dynamic_sundry.temperature = MOD_RP[sys_num].ICS_HT_RP.QueryRP.FPGATp;
            
        break;
        case PA_FUNC:
            if(data->Addr & 0x08){
                //TODO:alarm
                // exmod_dynamic_para_a[index].alarm.ch_pin_ul_op = MOD_RP[sys_num].UL_PA1.st & 0x02;
                exmod_dynamic_para_a[index].alarm.temp_h = MOD_RP[sys_num].UL_PA1.st & 0x04;
                //MOD_RP[sys_num].UL_PA1.st & 0x08; //驻波比告警
                exmod_dynamic_para_a[index].alarm.pa1 = MOD_RP[sys_num].UL_PA1.st & 0x10;
                exmod_dynamic_para_a[index].alarm.pout_pre = MOD_RP[sys_num].UL_PA1.st & 0x80;
                exmod_para_a[index].md_basic.sw = MOD_RP[sys_num].UL_PA1.st & 0x01;
                exmod_para_a[index].md_basic.att = MOD_RP[sys_num].UL_PA1.att;
                // exmod_para_a[index].md_basic.att = MOD_RP[sys_num].UL_PA1.att;
                exmod_dynamic_para_a[index].md_dynamic_sundry.temperature = MOD_RP[sys_num].UL_PA1.tp;
                exmod_dynamic_para_a[index].md_dynamic_basic.pout.p = MOD_RP[sys_num].UL_PA1.po;
                exmod_dynamic_para_a[index].md_dynamic_basic.rpout.p = MOD_RP[sys_num].UL_PA1.npo;
                //TODO:SWR
            }else{
                //TODO:alarm
                // exmod_dynamic_para_a[index].alarm.ch_pin_dl_op = MOD_RP[sys_num].UL_PA1.st & 0x02;
                exmod_dynamic_para_a[index].alarm.temp_h = MOD_RP[sys_num].UL_PA1.st & 0x04;
                //MOD_RP[sys_num].UL_PA1.st & 0x08; //驻波比告警
                exmod_dynamic_para_a[index].alarm.pa1 = MOD_RP[sys_num].UL_PA1.st & 0x10;
                exmod_dynamic_para_a[index].alarm.pout_pre = MOD_RP[sys_num].UL_PA1.st & 0x80;
                exmod_para_a[index].md_basic.sw = MOD_RP[sys_num].DL_PA1.st & 0x01;
                exmod_para_a[index].md_basic.att = MOD_RP[sys_num].DL_PA1.att;
                // exmod_para_a[index].md_basic.att = MOD_RP[sys_num].DL_PA1.att;
                exmod_dynamic_para_a[index].md_dynamic_sundry.temperature = MOD_RP[sys_num].DL_PA1.tp;
                exmod_dynamic_para_a[index].md_dynamic_basic.pout.p = MOD_RP[sys_num].DL_PA1.po;
                exmod_dynamic_para_a[index].md_dynamic_basic.rpout.p = MOD_RP[sys_num].DL_PA1.npo;
                //TODO:SWR alc
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
	u8 sys_num, i;
	u8 index=0, chnum;

	sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
    index = oldaddr_find_mod(data->Func, data->Addr);
    if(index == 0xff) return ;

    chnum = MOD_RP[sys_num].ICS_HT_RP.WK_CH.CHN>FREQ_CHANNEL_NUMS_MAX ? FREQ_CHANNEL_NUMS_MAX : MOD_RP[sys_num].ICS_HT_RP.WK_CH.CHN;
    
	switch(data->Func)
	{
        case ICS_FUNC:
        break;
	    case DIG_PICO:
            switch (data->Data[1])
            {
                case 0x02:
                    for(i=0; i<chnum; ++i){
                        exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.enable = 1;
                        exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_min_int = 0;
                        exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_min_float = 0;
                        exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_max_int = 255;
                        exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_max_float = 0;
                        exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.band_start_end = i%2;
                        exmod_dynamic_para_a[index].ch_rf_t.freq_ul[i].workfreq = (f32)GET_FREQ(MOD_RP[sys_num].ICS_HT_RP.WK_CH.CH[i], UL, sys_num);
                        exmod_dynamic_para_a[index].ch_rf_t.freq_dl[i].workfreq = (f32)GET_FREQ(MOD_RP[sys_num].ICS_HT_RP.WK_CH.CH[i], DL, sys_num);
                        exmod_para_a[index].ch_info_t.ul[i].workfreq = (f32)GET_FREQ(MOD_RP[sys_num].ICS_HT_RP.WK_CH.CH[i], UL, sys_num);
                        exmod_para_a[index].ch_info_t.dl[i].workfreq = (f32)GET_FREQ(MOD_RP[sys_num].ICS_HT_RP.WK_CH.CH[i], DL, sys_num);
                    }
                    
                break;
                case 0x06:
                    for(i=0; i<chnum; ++i){
                        if( i < 8)
                        {
                            if((MOD_RP[sys_num].ICS_HT_RP.CHSW.CHSW[2] >> i) & 0x01){
                                exmod_para_a[index].ch_info_t.dl[i].sw = 1;
                            }else{
                                exmod_para_a[index].ch_info_t.dl[i].sw = 0;
                            }
                        }
                        else if( (i >= 8) && (i < 16))
                        {
                            if((MOD_RP[sys_num].ICS_HT_RP.CHSW.CHSW[1] >> i) & 0x01){
                                exmod_para_a[index].ch_info_t.ul[i].sw = 1;
                                exmod_para_a[index].ch_info_t.dl[i].sw = 1;
                            }else{
                                exmod_para_a[index].ch_info_t.ul[i].sw = 0;
                                exmod_para_a[index].ch_info_t.dl[i].sw = 0;
                            }
                        }
                    }
                break;
                case 0x30:
                    for(i=0; i<chnum; ++i){
                        // exmod_para_a[index].ch_info_t.ul[i].agc_th = MOD_RP[sys_num].ICS_HT_RP.ULSL.AGCDepth;//TODO:AGC
                        exmod_para_a[index].ch_info_t.ul[i].pin_op_th = MOD_RP[sys_num].ICS_HT_RP.ULSL.INOverGa;
                        // exmod_para_a[index].ch_info_t.ul[i].att = MOD_RP[sys_num].ICS_HT_RP.ULSL.ManualAtt;
                        exmod_para_a[index].ch_info_t.ul[i].mute_th_h = MOD_RP[sys_num].ICS_HT_RP.ULSL.slHIGHGA;
                        exmod_para_a[index].ch_info_t.ul[i].mute_th_l = MOD_RP[sys_num].ICS_HT_RP.ULSL.slLOWGA;
                        exmod_para_a[index].ch_info_t.ul[i].mute_sw = MOD_RP[sys_num].ICS_HT_RP.ULSL.slSW;
                    }
                    exmod_para_a[index].md_sundry.dig_sundry.ics_sw = MOD_RP[sys_num].ICS_HT_RP.DLSL.ICSSW;
                break;
                case 0x31:
                    for(i=0; i<chnum; ++i){
                        // exmod_para_a[index].ch_info_t.dl[i].agc_th = MOD_RP[sys_num].ICS_HT_RP.DLSL.AGCDepth;//TODO:AGC
                        exmod_para_a[index].ch_info_t.dl[i].pin_op_th = MOD_RP[sys_num].ICS_HT_RP.DLSL.INOverGa;
                        // exmod_para_a[index].ch_info_t.dl[i].att = MOD_RP[sys_num].ICS_HT_RP.DLSL.ManualAtt;
                        exmod_para_a[index].ch_info_t.dl[i].mute_th_h = MOD_RP[sys_num].ICS_HT_RP.DLSL.slHIGHGA;
                        exmod_para_a[index].ch_info_t.dl[i].mute_th_l = MOD_RP[sys_num].ICS_HT_RP.DLSL.slLOWGA;
                        exmod_para_a[index].ch_info_t.dl[i].mute_sw = MOD_RP[sys_num].ICS_HT_RP.DLSL.slSW;
                    }
                    exmod_para_a[index].md_sundry.dig_sundry.ics_sw = MOD_RP[sys_num].ICS_HT_RP.DLSL.ICSSW;
                break;
                case 0x32:
                    for(i=0; i<chnum; ++i){
                        exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.bandsel_type = ((MOD_RP[sys_num].ICS_HT_RP.QueryRP.ICSType & 0x10) >> 4);
                    }
                break;
                case 0x07:
                    // for(i=0; i<chnum; ++i){
                    //     exmod_para_a[index].ch_info_t.ul[i].agc_th = MOD_RP[sys_num].ICS_HT_RP.ULSL.AGCDepth;
                    // }
                break;
                case 0x08:
                    // for(i=0; i<chnum; ++i){
                    //     exmod_para_a[index].ch_info_t.dl[i].agc_th = MOD_RP[sys_num].ICS_HT_RP.DLSL.AGCDepth;
                    // }
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
                    // for(i=0; i<chnum; ++i){
                    //     exmod_para_a[index].ch_info_t.ul[i].att = MOD_RP[sys_num].ICS_HT_RP.ULSL.ManualAtt;
                    // }
                break;
                case 0x0c:
                    // for(i=0; i<chnum; ++i){
                    //     exmod_para_a[index].ch_info_t.dl[i].att = MOD_RP[sys_num].ICS_HT_RP.DLSL.ManualAtt;
                    // }
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
                case 0x15:
				    for(i = 0; i < data->Data[2]; ++i){
                        exmod_dynamic_para_a[index].ch_rf_t.dl[i].pin = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLIP[i];
					}
                break;
                case 0x16:
				    for(i = 0; i < data->Data[2]; ++i){
                        exmod_dynamic_para_a[index].ch_rf_t.dl[i].pout = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOP[i];
					}
                break;
                case 0xe0:
                    for(i=0; i<chnum; ++i){
                        exmod_para_a[index].ch_info_t.ul[i].att = MOD_RP[sys_num].ICS_HT_RP.ch_ul_att.dat[i];
                    }
                break;
                case 0xe1:
                    for(i=0; i<chnum; ++i){
                        exmod_para_a[index].ch_info_t.dl[i].att = MOD_RP[sys_num].ICS_HT_RP.ch_dl_att.dat[i];
                    }
                break;
                case 0xe2:
                    for(i=0; i<chnum; ++i){
                        exmod_para_a[index].ch_info_t.ul[i].agc_th = MOD_RP[sys_num].ICS_HT_RP.ch_ul_agc.dat[i];
                    }
                break;
                case 0xe3:
                    for(i=0; i<chnum; ++i){
                        exmod_para_a[index].ch_info_t.dl[i].agc_th = MOD_RP[sys_num].ICS_HT_RP.ch_dl_agc.dat[i];
                    }
                break;
                case 0xe4:
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
        // case SETSN:
        //     set_sn_old2new(Rstr);      /*读取,设置模块系列号*/
        // break;
        // case 0x91:
        //     SETODU_RP(Rstr);/*与ODU相关设置*/
        // break;  	
        default:break;
	} 
}
//工作频率
s16 check_workfreq_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
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
			pch[cnt].workfreq = pf[cnt];
		}
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
//上行agc门限
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
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].agc_th = p[cnt];
		}
        rs485_ch_agc_send(newaddr_to_sysnum(md_adr), local, UL);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].agc_th;
		}
	} else {
		return -1;
	}

	return 0;
}
//下行agc门限
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
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].agc_th = p[cnt];
		}
        rs485_ch_agc_send(newaddr_to_sysnum(md_adr), local, DL);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].agc_th;
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
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].sw = p[cnt];
		}
        rs485_ch_sw_send(newaddr_to_sysnum(md_adr), local);
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
        //TODO:
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
        //TODO:
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