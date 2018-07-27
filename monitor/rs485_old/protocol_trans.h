/*
 * File      : protocol_trans.h
 * 
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-06-26      sujj      first version
*/
#ifndef __PROTOCOL_TRANS_H__
#define __PROTOCOL_TRANS_H__

#include "Module_Struct.h"
#include "helios.h"
#include "module_adr_def.h"

typedef struct {
    u8 band;
    f32 ul_low_freq;
    f32 ul_high_freq;
    f32 dl_low_freq;
    f32 dl_high_freq;
} band_table;

void oldsys_band_table_init();
void protocol_old2new(Rs485Comm *Rstr);
s16 check_ul_workfreq_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_dl_workfreq_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_ul_ch_agc_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_dl_ch_agc_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_ul_ch_att_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_dl_ch_att_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_ch_sw_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_ul_ch_pin_op_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_dl_ch_pin_op_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_shiftfreq_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_ch_mute_h_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_ch_mute_l_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_ch_pin_lp_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_ul_center_freq_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_dl_center_freq_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_ch_bandwidth_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
s16 check_ch_traffic_th_exmod(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);

#endif