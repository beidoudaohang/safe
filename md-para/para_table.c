/*
name:para_table.c
author:luke
date:20170324
ver:1.0
description:
	parameter dat deal
*/


/*****************************include files***********************/
#include <string.h>
#include <stdio.h>
#include "helios.h"
#include "para_table.h"
#include "dig_band0_para_table.h"
#include "digital.h"
#include "alarm_def.h"
#include "porting.h"
//#include "adi_api.h"
#include "log.h"
#include "common_api.h"
#include "data_task_def.h"
/*****************************para define*************************/
//monitor
#if MONITOR_MODULE_ENABLE
unit_para unit_para_t;
unit_dynamic_para unit_dynamic_para_t;
unit_alarm_c u_alarm_t;
#endif
//band
#if OTHER_MODULE_ENABLE
band_para band_para_a[MOD_NUM_IN_ONE_PCB];
band_dynamic_para band_dynamic_para_a[MOD_NUM_IN_ONE_PCB];
md_alarm_c m_alarm_a[MOD_NUM_IN_ONE_PCB];
#endif
//pcb
pcb_share_para pcb_share;
usr_authorize usr_auth;

const u16 pcb_share_para_table[] = {0x29, 0x2a, 0x2b};


/*****************************static funs define********************/
static s16 check_md_adr(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_workfreq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_shiftfreq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_pin(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_pout(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_pin_op_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_pin_lp_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_pin_op(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_pin_lp(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_lna_att(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_mute_sw(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_mute_l_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_mute_h_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_agc_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_agc_att(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_gain_compensate(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_traffic(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_uldl_mix(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_traffic_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_adi_offset_freq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_adi_offset_flat(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_adi_offset_pin(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_adi_offset_pout(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_adi_gain_att(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_adi_dc_image(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_adi_freq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_sw(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_gps_longitude(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_gps_latitude(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_gps_altitude(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_reset_para(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_band_bts(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_band_ms(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_band_max_gain(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_band_max_pout(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_band_max_freq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_band_min_freq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_modem_tech(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_cell_resel_sw(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_power_calibration_voltage(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_power_calibration_power(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_ch_bandwidth(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_dig_pa_sw(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_manual_cellid(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);

//处理与业务相关的回调函数
static s16 check_relay_sw(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_center_freq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
static s16 check_relay_mode(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag);
/*****************************para table8*************************/

#if MONITOR_MODULE_ENABLE
const para_table u_para_table_a[] =
{
    {
        .index = 1,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 10,
                    .dat = (void*)(unit_para_t.md_mfrs.para_table_ver),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x10,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 40,
                    .dat = (void*)(unit_para_t.md_mfrs.factory),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x12,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 15,
                    .dat = (void*) & (unit_para_t.u_mfrs.sn),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x13,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.u_mfrs.type),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x14,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.u_mfrs.model_no),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x15,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.u_mfrs.part_no),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x16,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (unit_para_t.gps_point_t.Longitude),
                    .min = {
                        .f32l = -180.0,
                    },
                    .max = {
                        .f32l = 180.0,
                    },
                    .paradeal = check_gps_longitude,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x17,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (unit_para_t.gps_point_t.Latitude),
                    .min = {
                        .f32l = -90.0,
                    },
                    .max = {
                        .f32l = 90.0,
                    },
                    .paradeal = check_gps_latitude,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x18,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (unit_para_t.gps_pen_t.left_top.Longitude),
                    .min = {
                        .f32l = -180.0,
                    },
                    .max = {
                        .f32l = 180.0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x19,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (unit_para_t.gps_pen_t.left_top.Latitude),
                    .min = {
                        .f32l = -90.0,
                    },
                    .max = {
                        .f32l = 90.0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x1a,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (unit_para_t.gps_pen_t.right_bottom.Longitude),
                    .min = {
                        .f32l = -180.0,
                    },
                    .max = {
                        .f32l = 180.0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x1b,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (unit_para_t.gps_pen_t.right_bottom.Latitude),
                    .min = {
                        .f32l = -90.0,
                    },
                    .max = {
                        .f32l = 90.0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x1c,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.gps_offline),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 2,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x1d,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 30,
                    .dat = (void*) & (unit_para_t.site.creat_time),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    /*{
        .index = 0x1e,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (unit_para_t.usr.),
                    .min = 0,
                    .max = 0,
                    .paradeal = NULL,
                },
            }
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .
    },*/
    {
        .index = 0x20,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 30,
                    .dat = (void*) & (unit_dynamic_para_t.time),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW //PARA_RD @ccTagOK
    },
    {
        .index = 0x21,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.md_mfrs.hv),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x22,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.md_mfrs.sv),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x23,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 50,
                    .dat = (void*) & (unit_para_t.site.name),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x24,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 100,
                    .dat = (void*) & (unit_para_t.site.addr),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x25,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.site.site_no),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x26,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.site.proprietor),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x27,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.site.asset),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x28,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.site.manager),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x2c,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_dynamic_para_t.gprs_t.status),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 5,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x2d,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 50,
                    .dat = (void*) & (unit_para_t.monitor_cfg_t.gprs.net_info.apn),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x2e,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.monitor_cfg_t.gprs.net_info.usr),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x2f,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.monitor_cfg_t.gprs.net_info.pw),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x30,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 25,
                    .dat = (void*) & (unit_dynamic_para_t.gprs_t.wlan_ip),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x31,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 25,
                    .dat = (void*) & (unit_para_t.monitor_cfg_t.nms.ip),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x32,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (unit_para_t.monitor_cfg_t.nms.port),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 65535,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x33,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.monitor_cfg_t.nms.sim),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x34,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.site.sim),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x35,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.monitor_cfg_t.nms.alarm_sim),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x36,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.monitor_cfg_t.monitor_mode),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 2,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x37,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.monitor_cfg_t.gprs.timeout),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 255,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x3a,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1 * 12,
                    .dat = (void*) & (unit_para_t.band_whole.passive_offset),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 50,
                    },
                    .paradeal = check_band_bts,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x3b,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1 * 12, //1 @ccTagOK
                    .dat = (void*) & (unit_para_t.band_whole.passive_offset),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 50,
                    },
                    .paradeal = check_band_ms,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x3c,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (unit_dynamic_para_t.temperature),
                    .min = {
                        .s8l = -50,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x3d,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.unit_sundry.case_temp_regulator),
                    .min = {
                        .s8l = -25,
                    },
                    .max = {
                        .s8l = 25,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x3f,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (unit_para_t.unit_sundry.device_id),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x40,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8}, //@ccTagOK
                    .len = 12, //1 @ccTagOK
                    .dat = (void*) & (unit_para_t.band_whole.band_restrict_dl),
                    .min = {
                        .s8l = -100,
                    },
                    .max = {
                        .s8l = 120,
                    },
                    .paradeal = check_band_max_gain,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x41,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8}, //PARA_TYPE_U8 @ccTagOK
                    .len = 12, //1 @ccTagOK
                    .dat = (void*) & (unit_para_t.band_whole.band_restrict_ul),
                    .min = {
                        .s8l = -100,
                    },
                    .max = {
                        .s8l = 120,
                    },
                    .paradeal = check_band_max_gain,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x42,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12, //1 @ccTagOK
                    .dat = (void*) & (unit_para_t.band_whole.band_restrict_dl),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 53,
                    },
                    .paradeal = check_band_max_pout,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x43,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12, //1 @ccTagOK
                    .dat = (void*) & (unit_para_t.band_whole.band_restrict_ul),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 53,
                    },
                    .paradeal = check_band_max_pout,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x44,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48, //4 @ccTagOK
                    .dat = (void*) & (unit_para_t.band_whole.band_restrict_dl),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_band_max_freq,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x45,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48, //4 @ccTagOK
                    .dat = (void*) & (unit_para_t.band_whole.band_restrict_dl),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_band_min_freq,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x46,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.door_open),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x47,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.power_ac),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x48,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.power_dc),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x49,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.movement),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x4a,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.gps_offline),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x4b,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.bat_low_volt),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x4c,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.fan_alarm_1),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x4d,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.fan_alarm_2),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x4e,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.fan_alarm_3),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x4f,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.fan_alarm_4),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x50,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.ext_alarm_1),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x51,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.ext_alarm_2),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x52,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.ext_alarm_3),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x53,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.ext_alarm_4),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x54,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.ext_alarm_5),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x55,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (u_alarm_t.u_alarm.ext_alarm_6),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x56,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.unit_sundry.ext_condition.ext_1),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x57,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.unit_sundry.ext_condition.ext_2),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x58,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.unit_sundry.ext_condition.ext_3),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x59,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.unit_sundry.ext_condition.ext_4),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x5a,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.unit_sundry.ext_condition.ext_5),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x5b,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.unit_sundry.ext_condition.ext_6),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x5c,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.door_open),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x5d,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.power_ac),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x5e,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.power_dc),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x5f,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.movement),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x60,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.gps_offline),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x61,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.bat_low_volt),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x62,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.fan_alarm_1),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x63,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.fan_alarm_2),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x64,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.fan_alarm_3),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x65,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.fan_alarm_4),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x66,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.ext_alarm_1),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x67,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.ext_alarm_2),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x68,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.ext_alarm_3),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x69,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.ext_alarm_4),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x6a,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.ext_alarm_5),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x6b,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.alarm_sw.ext_alarm_6),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x6c,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_dynamic_para_t.fan.fan1),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x6d,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_dynamic_para_t.fan.fan2),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x6e,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_dynamic_para_t.fan.fan3),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x6f,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_dynamic_para_t.fan.fan4),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x70,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.fan_ctrl_t.fan_temp_h),
                    .min = {
                        .s8l = 30,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x71,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.fan_ctrl_t.fan_temp_l),
                    .min = {
                        .s8l = 20,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x72,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.bat_in_t.ctrl_mode),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x73,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_para_t.bat_in_t.cut_out_time),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x74,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_dynamic_para_t.bat_status),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 2,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x77,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 10,
                    .dat = (void*) & (unit_dynamic_para_t.gprs_t.net.plmn),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x78,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 10,
                    .dat = (void*) & (unit_dynamic_para_t.gprs_t.net.tech),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x79,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (unit_dynamic_para_t.gprs_t.net.band),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 63,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x7a,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (unit_dynamic_para_t.gprs_t.net.cell),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 65535,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x7b,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (unit_dynamic_para_t.gprs_t.net.arfcn),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 65535,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x7c,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (unit_dynamic_para_t.gprs_t.net.rxlev),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x7d,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (unit_dynamic_para_t.gprs_t.net.rsq),
                    .min = {
                        .f32l = -50,
                    },
                    .max = {
                        .f32l = 50,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x119,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (unit_para_t.gps_point_t.altitude),
                    .min = {
                        .f32l = -100,
                    },
                    .max = {
                        .f32l = 1000,
                    },
                    .paradeal = check_gps_altitude,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x11e,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 15,
                    .dat = (void*) & (unit_para_t.md_mfrs.para_table_date),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x11d,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 15,
                    .dat = (void*) & (unit_para_t.unit_sundry.prj_code),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x129,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.md_mfrs.model_no),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x12a,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (unit_para_t.md_mfrs.part_no),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x12b,
        .link_para_a = {
                .md_adr = &(unit_para_t.md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (unit_para_t.band_whole.band),
                    .min = {
                        .s8l = 1,
                    },
                    .max = {
                        .s8l = 63,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
};
#else
const para_table u_para_table_a[] = {};
#endif

#if OTHER_MODULE_ENABLE
const para_table para_table_a[] =
{
    {
        .index = 1,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 10,
                    .dat = (void*)(band_para_a[0].md_mfrs.para_table_ver),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x10,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 40,
                    .dat = (void*)(band_para_a[0].md_mfrs.factory),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x11,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 3,
                    .dat = (void*) & (band_para_a[0].md_adr_t),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = check_md_adr,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x21,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (band_para_a[0].md_mfrs.hv),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x22,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (band_para_a[0].md_mfrs.sv),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x29,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 25,
                    .dat = (void*) & (pcb_share.net.ip),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x2a,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 25,
                    .dat = (void*) & (pcb_share.net.mask),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x2b,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 25,
                    .dat = (void*) & (pcb_share.net.gateway),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x38,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1, //12 @ccTagOK
                    .dat = (void*) & (band_para_a[0].md_sundry.agc_mod),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 4, //2 @ccTagOK
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x39,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.shutdown_ul),
                    .min = {
                        .s8l = -120, //120 @ccTagOK
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x3e,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_basic.max_gain),
                    .min = {
                        .s8l = -100,
                    },
                    .max = {
                        .s8l = 120,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x75,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.reset_para),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = check_reset_para,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    /*{
        .index = 0x76,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_residue_t.reset_para),
                    .min = 0,
                    .max = 0,
                    .paradeal = NULL,
                },
            }
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .
    },*/
    {
        .index = 0x7e,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_basic.sw),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x7f,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_basic.att),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 30,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x80,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.temperature),
                    .min = {
                        .s8l = -50,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x81,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_basic.temp_ctrl.temp_regulator),
                    .min = {
                        .s8l = -25,
                    },
                    .max = {
                        .s8l = 25,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x82,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_basic.pin),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 10,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x83,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_basic.pout),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 53,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x84,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_basic.rpout),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 53,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x85,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_basic.cur_gain),
                    .min = {
                        .f32l = -100,
                    },
                    .max = {
                        .f32l = 120,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x86,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_basic.rl),
                    .min = {
                        .f32l = 1,
                    },
                    .max = {
                        .f32l = 40,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x87,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].md_sundry.pa_unique.rl_th),
                    .min = {
                        .f32l = 1,
                    },
                    .max = {
                        .f32l = 40,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x88,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].md_basic.pout.p_th),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 53,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x89,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (band_para_a[0].md_sundry.pa_unique.vgs_pa1),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x8a,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].md_sundry.pa_unique.vgs_rate_pa1),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x8b,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (band_para_a[0].md_sundry.pa_unique.vgs_pa2),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x8c,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].md_sundry.pa_unique.vgs_rate_pa2),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x8d,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (band_para_a[0].md_sundry.pa_unique.vgs_l_th_pa1),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x8e,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (band_para_a[0].md_sundry.pa_unique.vgs_h_th_pa1),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x8f,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (band_para_a[0].md_sundry.pa_unique.vgs_l_th_pa2),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x90,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (band_para_a[0].md_sundry.pa_unique.vgs_h_th_pa2),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x91,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1 * 40,
                    .dat = (void*) & (band_para_a[0].md_basic.pin.table),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 10,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x92,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2 * 40,
                    .dat = (void*) & (band_para_a[0].md_basic.pin.table),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x93,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1 * 40,
                    .dat = (void*) & (band_para_a[0].md_basic.pout.table[0]), //.table
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 53, //10
                    },
                    .paradeal = check_power_calibration_power,//NULL @ccTagOK
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x94,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2 * 40,
                    .dat = (void*) & (band_para_a[0].md_basic.pout.table[0]), //.table
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = check_power_calibration_voltage,//NULL, @ccTagOK
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x95,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1 * 40,
                    .dat = (void*) & (band_para_a[0].md_basic.rpout.table[0]), //.table
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 53, //10
                    },
                    .paradeal = check_power_calibration_power,//NULL @ccTagOK
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x96,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2 * 40,
                    .dat = (void*) & (band_para_a[0].md_basic.rpout.table[0]), //.table
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = check_power_calibration_voltage,//NULL, @ccTagOK
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x97,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_basic.pin.max_v),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x98,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_basic.pout.max_v),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x99,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_basic.rpout.max_v),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 5500,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x9a,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.rl),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x9b,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.pout),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x9c,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.pout_pre),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x9d,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.temp_h),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x9e,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.pa1),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x9f,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.pa2),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xa0,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.rl),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xa1,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.pout),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xa2,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.pout_pre),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xa3,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.temp_h),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xa4,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.pa1),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xa5,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.pa2),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xa6,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.td_unique.sw),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 3,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xa7,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.td_unique.wk_mode),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xa8,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.td_unique.work_para.major),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 167,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xa9,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.td_unique.work_para.minor),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 2,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xaa,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.td_unique.work_para.dl_ul_config),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 6,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xab,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.td_unique.work_para.cp),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xac,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.td_unique.work_para.sf_config),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 9,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xad,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.td_unique.sync_th),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xae,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.tdd_dynamic_sundry.sync_st),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xaf,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.tdd_sync_lost),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xb0,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.tdd_sync_lost),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xb1,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.td_unique.sync_keep),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 255,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xb2,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S16},
                    .len = 2,
                    .dat = (void*) & (band_para_a[0].md_sundry.td_unique.sync_offset),
                    .min = {
                        .s16l = -100,
                    },
                    .max = {
                        .s16l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xb3,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U32},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].md_sundry.td_unique.guard_time),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 10,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xb4,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4 * 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.ul[0]),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_workfreq,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x01,
        .flag = PARA_RW
    },
    {
        .index = 0xb5,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4 * 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.ul[0]),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_shiftfreq,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x0d,
        .flag = PARA_RW
    },
    {
        .index = 0xb6,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4 * 12,
                    .dat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 10.000,
                    },
                    .paradeal = check_ch_pin,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xb7,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4 * 12,
                    .dat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 10.000,
                    },
                    .paradeal = check_ch_pout,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xb8,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.ul[0]),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 10,
                    },
                    .paradeal = check_ch_pin_op_th,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x19,
        .flag = PARA_RW
    },
    {
        .index = 0xb9,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.ch_pin_ul_op),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = check_ch_pin_op,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xba,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.ul[0]),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 10,
                    },
                    .paradeal = check_ch_pin_lp_th,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x19,
        .flag = PARA_RW
    },
    {
        .index = 0xbb,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.ch_pin_ul_lp),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = check_ch_pin_lp,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xbc,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.ul[0]),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 30, //1 @ccTagOK
                    },
                    .paradeal = check_ch_lna_att,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x2f,
        .flag = PARA_RW
    },
    {
        .index = 0xbd,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.ul[0]),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = check_ch_mute_sw,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x25,
        .flag = PARA_RW
    },
    {
        .index = 0xbe,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.ul[0]),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 10,
                    },
                    .paradeal = check_ch_mute_l_th,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x35,
        .flag = PARA_RW
    },
    {
        .index = 0xbf,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.ul[0]),
                    .min = {
                        .s8l = -120,//0 @ccTagOK
                    },
                    .max = {
                        .s8l = 10, //1@ccTagOK
                    },
                    .paradeal = check_ch_mute_h_th,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x35,
        .flag = PARA_RW
    },
    {
        .index = 0xc0,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.ul[0]),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 53,
                    },
                    .paradeal = check_ch_agc_th,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x26,
        .flag = PARA_RW
    },
    {
        .index = 0xc1,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0]),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 30,
                    },
                    .paradeal = check_ch_agc_att,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xc2,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0]),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 10,
                    },
                    .paradeal = check_ch_gain_compensate,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xc3,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2 * 12,
                    .dat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0]),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 3600,
                    },
                    .paradeal = check_ch_traffic,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xc4,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].ul.freq_rx),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_adi_freq,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xc5,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].ul.freq_tx),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_adi_freq,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xc6,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].ul.adi_offset[0]),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_adi_offset_freq,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0, //0x3c @ccTagOK20170725
        .flag = PARA_RW
    },
    {
        .index = 0xc7,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].ul.adi_offset[0]),
                    .min = {
                        .f32l = -10,
                    },
                    .max = {
                        .f32l = 10,
                    },
                    .paradeal = check_adi_offset_flat,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xc8,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].ul.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pin,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xc9,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].ul.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pout,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xca,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].ul.freq_rx),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_adi_freq,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xd5,
        .flag = PARA_RW
    },
    {
        .index = 0xcb,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].ul.freq_tx),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_adi_freq,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xdb,
        .flag = PARA_RW
    },
    {
        .index = 0xcc,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].ul.adi_offset[0]),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_adi_offset_freq,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x3c,
        .flag = PARA_RW
    },
    {
        .index = 0xcd,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].ul.adi_offset[0]),
                    .min = {
                        .f32l = -10,
                    },
                    .max = {
                        .f32l = 10,
                    },
                    .paradeal = check_adi_offset_flat,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x3c,
        .flag = PARA_RW
    },
    {
        .index = 0xce,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].ul.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pin,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x3c,
        .flag = PARA_RW
    },
    {
        .index = 0xcf,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].ul.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pout,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x3c,
        .flag = PARA_RW
    },
    {
        .index = 0xd0,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.gain_equalize_rate_ul),
                    .min = {
                        .f32l = -1,
                    },
                    .max = {
                        .f32l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x3b,
        .flag = PARA_RW
    },
    {
        .index = 0xd1,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.dds_ul.adc_att),
                    .min = {
                        .u32l = 0,
                    },
                    .max = {
                        .u32l = 30,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xd2,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48,
                    .dat = (void*) & (band_para_a[0].ch_info_t.dl[0]),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_workfreq,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x68,
        .flag = PARA_RW
    },
    {
        .index = 0xd3,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48,
                    .dat = (void*) & (band_para_a[0].ch_info_t.dl[0]),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_shiftfreq,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x74,
        .flag = PARA_RW
    },
    {
        .index = 0xd4,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48,
                    .dat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 10,
                    },
                    .paradeal = check_ch_pin,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xd5,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48,
                    .dat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 53,
                    },
                    .paradeal = check_ch_pout,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xd6,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.dl[0]),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 10,
                    },
                    .paradeal = check_ch_pin_op_th,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x80,
        .flag = PARA_RW
    },
    {
        .index = 0xd7,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.ch_pin_dl_op[0]),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = check_ch_pin_op,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xd8,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.dl[0]),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 10,
                    },
                    .paradeal = check_ch_pin_lp_th,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x80,
        .flag = PARA_RW
    },
    {
        .index = 0xd9,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.ch_pin_dl_lp),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = check_ch_pin_lp,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xda,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.dl[0]),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 30,
                    },
                    .paradeal = check_ch_lna_att,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x93,
        .flag = PARA_RW
    },
    {
        .index = 0xdb,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.dl[0]),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = check_ch_sw,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x8c,
        .flag = PARA_RW
    },
    {
        .index = 0xdc,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.dl[0]),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 53,
                    },
                    .paradeal = check_ch_agc_th,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x8d,
        .flag = PARA_RW
    },
    {
        .index = 0xdd,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[0]),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 30,
                    },
                    .paradeal = check_ch_agc_att,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xde,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[0]),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 10,
                    },
                    .paradeal = check_ch_gain_compensate,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xdf,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].dl.freq_rx),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_adi_freq,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xd9,
        .flag = PARA_RW
    },
    {
        .index = 0xe0,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].dl.freq_tx),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_adi_freq,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xd7,
        .flag = PARA_RW
    },
    {
        .index = 0xe1,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].dl.adi_offset[0]),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_adi_offset_freq,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,//0x9a @ccTagOK20170725
        .flag = PARA_RW
    },
    {
        .index = 0xe2,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].dl.adi_offset[0]),
                    .min = {
                        .f32l = -10,
                    },
                    .max = {
                        .f32l = 10,
                    },
                    .paradeal = check_adi_offset_flat,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x9a,
        .flag = PARA_RW
    },
    {
        .index = 0xe3,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].dl.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pin,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x9a,
        .flag = PARA_RW
    },
    {
        .index = 0xe4,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].dl.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pout,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x9a,
        .flag = PARA_RW
    },
    {
        .index = 0xe5,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].dl.freq_rx),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xd9,
        .flag = PARA_RW
    },
    {
        .index = 0xe6,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].dl.freq_tx),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xd7,
        .flag = PARA_RW
    },
    {
        .index = 0xe7,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].dl.adi_offset[0]),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.000,
                    },
                    .paradeal = check_adi_offset_freq,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x9a,
        .flag = PARA_RW
    },
    {
        .index = 0xe8,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].dl.adi_offset[0]),
                    .min = {
                        .f32l = -10,
                    },
                    .max = {
                        .f32l = 10,
                    },
                    .paradeal = check_adi_offset_flat,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x9a,
        .flag = PARA_RW
    },
    {
        .index = 0xe9,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].dl.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pin,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x9a,
        .flag = PARA_RW
    },
    {
        .index = 0xea,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 80,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].dl.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pout,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x9a,
        .flag = PARA_RW
    },
    {
        .index = 0xeb,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.gain_equalize_rate_dl),
                    .min = {
                        .f32l = -1,
                    },
                    .max = {
                        .f32l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0x99,
        .flag = PARA_RW
    },
    {
        .index = 0xec,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.dds_dl.adc_att),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 30,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xed,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.ics_sw),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x8c,
        .flag = PARA_RW
    },
    {
        .index = 0xee,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.ics_iso_dl),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xef,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.ad80305_1),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xf0,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.ad80305_2),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xf1,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.ad80305_1),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xf2,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.ad80305_2),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xf3,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.ch_pin_ul_op),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xf4,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.ch_pin_ul_lp),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xf5,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.ch_pin_dl_op),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xf6,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.ch_pin_dl_lp),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xf7,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U32},
                    .len = 48,
                    .dat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[0]),
                    .min = {
                        .u32l = 0,
                    },
                    .max = {
                        .u32l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0xf8,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING10},
                    .len = 120,
                    .dat = (void*) & (band_para_a[0].ch_info_t.ch_remark[0]),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0xf9,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.work_mode),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 3,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0xd2,
        .flag = PARA_RW
    },
    {
        .index = 0xfa,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 2,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].ul.gain_att),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 60,
                    },
                    .paradeal = check_adi_gain_att,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0xd3,
        .flag = PARA_RW
    },
    {
        .index = 0xfb,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 2,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].ul.gain_att),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 60,
                    },
                    .paradeal = check_adi_gain_att,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0xd3,
        .flag = PARA_RW
    },
    {
        .index = 0xfc,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 2,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].dl.gain_att),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 60,
                    },
                    .paradeal = check_adi_gain_att,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0xd4,
        .flag = PARA_RW
    },
    {
        .index = 0xfd,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 2,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].dl.gain_att),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 60,
                    },
                    .paradeal = check_adi_gain_att,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0xd4,
        .flag = PARA_RW
    },
    {
        .index = 0xfe,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].ul.dc_image),
                    .min = {
                        .s8l = -100,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = check_adi_dc_image,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0xdd,
        .flag = PARA_RW
    },
    {
        .index = 0xff,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].ul.dc_image),
                    .min = {
                        .s8l = -100,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = check_adi_dc_image,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0xdd,
        .flag = PARA_RW
    },
    {
        .index = 0x100,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[0].dl.dc_image),
                    .min = {
                        .s8l = -100,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = check_adi_dc_image,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0xdf,
        .flag = PARA_RW
    },
    {
        .index = 0x101,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].adi_para_t[1].dl.dc_image),
                    .min = {
                        .s8l = -100,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = check_adi_dc_image,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0xdf,
        .flag = PARA_RW
    },
    {
        .index = 0x102,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.relay_sw),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xd0,
        .flag = PARA_RW
    },
    {
        .index = 0x103,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.temp_l),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x104,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.fpga_loaded),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x105,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.fpga_uart),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x106,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.fpga_spi),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x107,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.net_status),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x108,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.dig_pa_sw),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 2,
                    },
                    .paradeal = check_dig_pa_sw,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x109,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 40,
                    .dat = (void*) & (band_dynamic_para_a[0].md_wireless_net_t.wireless_info_t.ser_cell),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x10a,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 388,
                    .dat = (void*) & (band_dynamic_para_a[0].md_wireless_net_t.wireless_info_t.nei_cell),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x10b,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 388,
                    .dat = (void*) & (band_dynamic_para_a[0].md_wireless_net_t.after_relay),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x10c,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2 * 250,
                    .dat = (void*) & (band_para_a[0].tel_operator.plmn_t.plmn),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 65535,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x10d,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].tel_operator.band),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 64,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x10e,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_wireless_net_t.scan_tech),
                    .min = {
                        .u8l = 1,
                    },
                    .max = {
                        .u8l = 12,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x10f,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U16},
                    .len = 2,
                    .dat = (void*) & (band_dynamic_para_a[0].md_wireless_net_t.swn_manual_cell),
                    .min = {
                        .u16l = 0,
                    },
                    .max = {
                        .u16l = 503,
                    },
                    .paradeal = check_manual_cellid,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x110,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48,
                    .dat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[0]),
                    .min = {
                        .f32l = -10,
                    },
                    .max = {
                        .f32l = 10,
                    },
                    .paradeal = check_adi_offset_flat,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x111,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48,
                    .dat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pin,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x112,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48,
                    .dat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pout,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x113,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48,
                    .dat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[0]),
                    .min = {
                        .f32l = -10,
                    },
                    .max = {
                        .f32l = 10,
                    },
                    .paradeal = check_adi_offset_flat,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x114,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48,
                    .dat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pin,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x115,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 48,
                    .dat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[0]),
                    .min = {
                        .f32l = -120,
                    },
                    .max = {
                        .f32l = 100,
                    },
                    .paradeal = check_adi_offset_pout,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x116,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.cell_resel_sw),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = check_cell_resel_sw,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x117,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 15,
                    .dat = (void*) & (unit_para_t.md_mfrs.sn),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW //PARA_RD @ccTagOK
    },
    /*{
        .index = 0x118,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 15,
                    .dat = (void*) & (unit_para_t.mfrs_info.sn),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RD
    },*/
    {
        .index = 0x11a,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.temp_l),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x11b,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.ul[0]),
                    .min = {
                        .s8l = -120,
                    },
                    .max = {
                        .s8l = 10,
                    },
                    .paradeal = check_ch_traffic_th,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0x2c,
        .flag = PARA_RW
    },
    /*{
        .index = 0x11c,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.dl[0]),
                    .min = 0,
                    .max = 0,
                    .paradeal = check_ch_traffic_th,
                },
            }
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .
    },*/
    {
        .index = 0x11e,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 15,
                    .dat = (void*) & (band_para_a[0].md_mfrs.para_table_date),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x11f,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.iso_alarm_th),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 255,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x120,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.iso_alarm),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_TERMINAL,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x121,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_basic.temp_ctrl.temp_over_th),
                    .min = {
                        .s8l = -50,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x122,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_basic.temp_ctrl.temp_over_clean),
                    .min = {
                        .s8l = -50,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x123,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_basic.temp_ctrl.temp_lower_th),
                    .min = {
                        .s8l = -50,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x124,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_basic.temp_ctrl.temp_lower_clean),
                    .min = {
                        .s8l = -50,
                    },
                    .max = {
                        .s8l = 100,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x125,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_basic.temp_ctrl.temp_rf_sw),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x126,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 62,
                    },
                    .paradeal = check_ch_uldl_mix,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xe5,
        .flag = PARA_RW
    },
    {
        .index = 0x127,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.iso_alarm),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x128,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1 * 250, //@ccTag20170803
                    .dat = (void*) & (band_para_a[0].tel_operator.plmn_t.enable),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 3,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x129,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (band_para_a[0].md_mfrs.model_no),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x12a,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_STRING},
                    .len = 20,
                    .dat = (void*) & (band_para_a[0].md_mfrs.part_no),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 0,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x12c,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.att_ul.lna_att),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 30,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xee,
        .flag = PARA_RW
    },
    {
        .index = 0x12d,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.att_dl.lna_att),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 30,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xee,
        .flag = PARA_RW
    },
    {
        .index = 0x12e,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.att_ul.pa_att),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 30,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xee,
        .flag = PARA_RW
    },
    {
        .index = 0x12f,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.att_dl.pa_att),
                    .min = {
                        .s8l = 0,
                    },
                    .max = {
                        .s8l = 30,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xee,
        .flag = PARA_RW
    },
    {
        .index = 0x130,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.center_freq.ul),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.00,
                    },
                    .paradeal = check_center_freq,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x131,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4,
                    .dat = (void*) & (band_para_a[0].md_sundry.dig_sundry.center_freq.dl),
                    .min = {
                        .f32l = 0,
                    },
                    .max = {
                        .f32l = 100000.00,
                    },
                    .paradeal = check_center_freq,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x132,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_dynamic_para_a[0].md_wireless_net_t.mflag),
                    .min = {
                        .u8l = 1,
                    },
                    .max = {
                        .u8l = 2,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x133,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.tech),
                    .min = {
                        .u8l = 1,
                    },
                    .max = {
                        .u8l = 5,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x134,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 12,
                    .dat = (void*) & (band_dynamic_para_a[0].md_wireless_net_t.modem_tech_a),
                    .min = {
                        .u8l = 1,
                    },
                    .max = {
                        .u8l = 5,
                    },
                    .paradeal = check_modem_tech,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x135,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.cp_init),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x136,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.cp_init),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x137,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (m_alarm_a[0].m_alarm.ap_init),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RD
    },
    {
        .index = 0x138,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].alarm_sw.ap_init),
                    .min = {
                        .u8l = 0,
                    },
                    .max = {
                        .u8l = 1,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x139,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_U8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_sundry.relay_mode),
                    .min = {
                        .u8l = 1,
                    },
                    .max = {
                        .u8l = 2,
                    },
                    .paradeal = check_relay_mode, //null @ccTag20170807
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0,
        .flag = PARA_RW
    },
    {
        .index = 0x13a,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_FLOAT},
                    .len = 4 * 12,
                    .dat = (void*) & (band_para_a[0].ch_info_t.bandwidth),
                    .min = {
                        .f32l = 0, //.u8l = 0, //@ccTag20170808
                    },
                    .max = {
                        .f32l = 100.00, //.u8l = 100,
                    },
                    .paradeal = check_ch_bandwidth,
                },
        },
        .authority = USR_OPERATOR,
        .dig_adr = 0xe8,
        .flag = PARA_RW
    },
    {
        .index = 0x13b,
        .link_para_a = {
                .md_adr = &(band_para_a[0].md_adr_t),
                .link_para_t = {
                    .para_type_t = {PARA_TYPE_S8},
                    .len = 1,
                    .dat = (void*) & (band_para_a[0].md_augment.after_relay_rsrp_revise),
                    .min = {
                        .s8l = -127,
                    },
                    .max = {
                        .s8l = 127.00,
                    },
                    .paradeal = NULL,
                },
        },
        .authority = USR_MANUFACTURER,
        .dig_adr = 0,
        .flag = PARA_RW
    }
};
#else
const para_table para_table_a[] = {};
#endif

/*****************************data struct define******************/
#define U_PARA_TABLE_SIZE 	(sizeof(u_para_table_a)/sizeof(para_table))
#define M_PARA_TABLE_SIZE 	(sizeof(para_table_a)/sizeof(para_table))
/*****************************processing para funs********************************/
static s16 check_md_adr(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	md_adr_info *pmd_adr_old, *pmd_adr_new;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	pmd_adr_old = (md_adr_info*)local;
	pmd_adr_new = (md_adr_info*)remote;

	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		if ((pmd_adr_new->mod_type) != (pmd_adr_old->mod_type))
			return -1;

		pmd_adr_old->mod_band = pmd_adr_new->mod_band;
		pmd_adr_old->mod_adr_t.dat = pmd_adr_new->mod_adr_t.dat;
	}

	return 0;
}
static s16 check_workfreq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
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
			set_adr_add(dig_adr + cnt, md_adr);
			// adi_adjust_once(md_adr);
			// dig_adi_adjust_update(md_adr);
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pf[cnt] = pch[cnt].workfreq;
		}
	} else {
		return -1;
	}

	return 0;
}
static s16 check_shiftfreq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
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
			set_adr_add(dig_adr + cnt, md_adr);
			// adi_adjust_once(md_adr);
			// dig_adi_adjust_update(md_adr);
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pf[cnt] = pch[cnt].shiftfreq;
		}
	} else {
		return -1;
	}

	return 0;
}
static s16 check_ch_pin(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *pf;
	ch_link_power *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	pf = (f32*)remote;
	pch = (ch_link_power*)local;

	if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pf[cnt] = pch[cnt].pin;
		}
	} else {
		return -1;
	}

	return 0;
}
static s16 check_ch_pout(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *pf;
	ch_link_power *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	pf = (f32*)remote;
	pch = (ch_link_power*)local;

	if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pf[cnt] = pch[cnt].pout;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_pin_op_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8* p;//f32 *p;//@ccTagOK
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;//p = (f32*)remote; //@ccTagOK
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].pin_op_th = p[cnt];
			set_adr_add(dig_adr + cnt, md_adr);
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].pin_op_th;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_pin_lp_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8* p;//f32 *p; //@ccTagOK
	ch_link *pch;
	u32 tem = 0;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	p = (s8*)remote;//p = (f32*)remote;
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].pin_lp_th = p[cnt];
			set_adr_add(dig_adr + cnt, md_adr);
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].pin_lp_th;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_pin_op(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u8 *pr, *pl;
	//ch_link_power *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	pr = (u8*)remote;
	pl = (u8*)local;
	//pch = (ch_link_power*)local;

	if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pr[cnt] = pl[cnt];
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_pin_lp(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u8 *pr, *pl;
	//ch_link_power *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	pr = (u8*)remote;
	pl = (u8*)local;
	//pch = (ch_link_power*)local;

	if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pr[cnt] = pl[cnt];
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_lna_att(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u8 *p;//f32 *p; //@ccTagOK
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	p = (u8*)remote;//p = (f32*)remote;//@ccTagOK
	pch = (ch_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt].att = p[cnt];
			if (!(cnt % 2)) {
				set_adr_add((dig_adr + (cnt >> 1)), md_adr);
			}
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].att;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_mute_sw(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
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
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].mute_sw;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_mute_l_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
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
			if (!(cnt % 2)) {
				set_adr_add((dig_adr + (cnt >> 1)), md_adr);
			}
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].mute_th_l;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_mute_h_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
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
			if (!(cnt % 2)) {
				set_adr_add((dig_adr + (cnt >> 1)), md_adr);
			}
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].mute_th_h;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_agc_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
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
			if (!(cnt % 2)) {
				set_adr_add((dig_adr + (cnt >> 1)), md_adr);
			}
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].agc_th;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_agc_att(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u8 *p;
	ch_link_power *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;
	p = (u8*)remote;
	pch = (ch_link_power*)local;

	if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].agc_att;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_gain_compensate(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u8 *p;
	ch_link_power *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;
	p = (u8*)remote;
	pch = (ch_link_power*)local;

	if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].gain_compensate;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_traffic(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u16 *p;
	ch_link_power *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;
	p = (u16*)remote;
	pch = (ch_link_power*)local;

	if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].traffic;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_uldl_mix(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u8 *p; //u16 *p; //@ccTagOK
	u8 *pch;//ch_link_power *pch; //@ccTagOK

	if ((NULL == remote) || (NULL == local) || (NULL == md_adr))
		return -1;

	p = (u8*)remote;//p = (u16*)remote; @ccTagOK
	pch = (u8*)local; //(ch_link_power*)local; @ccTagOK

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt] = p[cnt]; //pch[cnt].traffic = p[cnt]; @ccTagOK
			if (!(cnt % 4)) {
				set_adr_add((dig_adr + (cnt >> 2)), md_adr);
			}
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt]; //pch[cnt].traffic @ccTagOK
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_ch_traffic_th(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
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
			if (!(cnt % 4)) {
				set_adr_add((dig_adr + (cnt >> 2)), md_adr);
			}
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

static s16 check_adi_offset_freq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *p;
	adi_offset_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	p = (f32*)remote;
	pch = (adi_offset_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < ADI_OFFSET_TABLE_SIZE; cnt++) {
			pch[cnt].freq = p[cnt];
		}
		// adi_adjust_once(md_adr);
		// dig_adi_adjust_update(md_adr);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < ADI_OFFSET_TABLE_SIZE; cnt++) {
			p[cnt] = pch[cnt].freq;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_adi_offset_flat(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *p;
	adi_offset_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;
	dig_adr = dig_adr;

	p = (f32*)remote;
	pch = (adi_offset_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < ADI_OFFSET_TABLE_SIZE; cnt++) {
			pch[cnt].flat = p[cnt];
		}
		// adi_adjust_once(md_adr);
		// dig_adi_adjust_update(md_adr);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < ADI_OFFSET_TABLE_SIZE; cnt++) {
			p[cnt] = pch[cnt].flat;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_adi_offset_pin(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *p;
	adi_offset_link *pch;

	if ((NULL == remote) || (NULL == local) || (NULL == md_adr))
		return -1;
	dig_adr = dig_adr;

	p = (f32*)remote;
	pch = (adi_offset_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < ADI_OFFSET_TABLE_SIZE; cnt++) {
			pch[cnt].pin = p[cnt];
		}
		// adi_adjust_once(md_adr);
		// dig_adi_adjust_update(md_adr);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < ADI_OFFSET_TABLE_SIZE; cnt++) {
			p[cnt] = pch[cnt].pin;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_adi_offset_pout(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *p;
	adi_offset_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;
	dig_adr = dig_adr;

	p = (f32*)remote;
	pch = (adi_offset_link*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < ADI_OFFSET_TABLE_SIZE; cnt++) {
			pch[cnt].pout = p[cnt];
		}
		// adi_adjust_once(md_adr);
		// dig_adi_adjust_update(md_adr);
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < ADI_OFFSET_TABLE_SIZE; cnt++) {
			p[cnt] = pch[cnt].pout;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_adi_gain_att(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u8 *p;
	adi_gain_att *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;
	dig_adr = dig_adr;

	dig_adr = dig_adr;
	p = (u8*)remote;
	pch = (adi_gain_att*)local;

	if (PARA_RW == flag) {
		pch->rx_gain = p[0];
		pch->tx_att = p[1];
		// adi_adjust_once(md_adr);
		// dig_adi_adjust_update(md_adr);
	} else if (PARA_RD == flag) {
		p[0] = pch->rx_gain;
		p[1] = pch->tx_att;
	} else {
		return -1;
	}

	return 0;
}

static s16 check_adi_dc_image(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	s8 *p;
	adi_dc_image *pch;

	if ((NULL == remote) || (NULL == local) || (NULL == md_adr))
		return -1;
	dig_adr = dig_adr;

	p = (s8*)remote;
	pch = (adi_dc_image*)local;

	if (PARA_RW == flag) {
		pch->phase = p[0];
		pch->amp = p[1];
		pch->dc_i = p[2]; //p[3]
		pch->dc_q = p[3]; //p[4]
		// adi_adjust_once(md_adr);
		// dig_adi_adjust_update(md_adr);
	} else if (PARA_RD == flag) {
		p[0] = pch->phase;
		p[1] = pch->amp;
		p[2] = pch->dc_i;
		p[3] = pch->dc_q;
	} else {
		return -1;
	}

	return 0;
}

static s16 check_adi_freq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;
	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		*(f32*)local = *(f32*)remote;
		// adi_adjust_once(md_adr);
		// dig_adi_adjust_update(md_adr);
	} else if (PARA_RD == flag) {
		*(f32*)remote = *(f32*)local;
	} else {
		return -1;
	}
	return 0;
}


static s16 check_ch_sw(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
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
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			p[cnt] = pch[cnt].sw;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_gps_longitude(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *p;
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		memcpy((void*)(local), (void*) (remote), sizeof(f32));
		memcpy((void*) & (unit_dynamic_para_t.gps_t.gps_cur), (void*) & (unit_para_t.gps_point_t), sizeof(gps_point));
	} else {
		memcpy((void*)(remote), (void*) & (unit_dynamic_para_t.gps_t.gps_cur.Longitude), sizeof(f32));
	}

	return 0;
}
static s16 check_gps_latitude(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *p;
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		memcpy((void*)(local), (void*) (remote), sizeof(f32));
		memcpy((void*) & (unit_dynamic_para_t.gps_t.gps_cur), (void*) & (unit_para_t.gps_point_t), sizeof(gps_point));
	} else {
		memcpy((void*)(remote), (void*) & (unit_dynamic_para_t.gps_t.gps_cur.Latitude), sizeof(f32));
	}

	return 0;
}
static s16 check_gps_altitude(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *p;
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		memcpy((void*)(local), (void*) (remote), sizeof(f32));
		memcpy((void*) & (unit_dynamic_para_t.gps_t.gps_cur), (void*) & (unit_para_t.gps_point_t), sizeof(gps_point));
	} else {
		memcpy((void*)(remote), (void*) & (unit_dynamic_para_t.gps_t.gps_cur.altitude), sizeof(f32));
	}

	return 0;
}
//added @ccTagOK
static s16 check_power_calibration_power(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	power_table *pch;


	if ((NULL == remote) || (NULL == local) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;
	p = (s8*)remote;
	pch = (power_table*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < POWER_DETECT_TABLE_SIZE; cnt++) {
			pch[cnt].p = p[cnt];
			//if (!(cnt % 2)) {
			//	set_adr_add((dig_adr + (cnt >> 1)), md_adr);
			//}
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < POWER_DETECT_TABLE_SIZE; cnt++) {
			p[cnt] = pch[cnt].p;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_power_calibration_voltage(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u16 *p;
	power_table *pch;

	if ((NULL == remote) || (NULL == local) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;
	p = (u16*)remote;
	pch = (power_table*)local;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < POWER_DETECT_TABLE_SIZE; cnt++) {
			pch[cnt].v = p[cnt];
			//if (!(cnt % 2)) {
			//	set_adr_add((dig_adr + (cnt >> 1)), md_adr);
			//}
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < POWER_DETECT_TABLE_SIZE; cnt++) {
			p[cnt] = pch[cnt].v;
		}
	} else {
		return -1;
	}

	return 0;
}

static s16 check_reset_para(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	u8 *p;
	ch_link *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	return 0;
}

static s16 check_band_bts(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	passive_offset *po;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	po = (passive_offset*)local;
	p = (s8*)remote;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			po[cnt].bts = p[cnt];
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			p[cnt] = po[cnt].bts;
		}
	} else {
		return -1;
	}
	return 0;
}
static s16 check_band_ms(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	passive_offset *po;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	po = (passive_offset*)local;
	p = (s8*)remote;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			po[cnt].ms = p[cnt];
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			p[cnt] = po[cnt].ms;
		}
	} else {
		return -1;
	}
	return 0;
}
static s16 check_band_max_gain(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	band_restrict *br;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	br = (band_restrict*)local;
	p = (s8*)remote;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			br[cnt].max_gain = p[cnt];
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			p[cnt] = br[cnt].max_gain;
		}
	} else {
		return -1;
	}
	return 0;
}
static s16 check_band_max_pout(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	band_restrict *br;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	br = (band_restrict*)local;
	p = (s8*)remote;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			br[cnt].max_pout = p[cnt];
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			p[cnt] = br[cnt].max_pout;
		}
	} else {
		return -1;
	}
	return 0;
}
static s16 check_band_max_freq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *p;
	band_restrict *br;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	br = (band_restrict*)local;
	p = (f32*)remote;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			br[cnt].max_freq = p[cnt];
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			p[cnt] = br[cnt].max_freq;
		}
	} else {
		return -1;
	}
	return 0;
}
static s16 check_band_min_freq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	s8 *p;
	band_restrict *br;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	br = (band_restrict*)local;
	p = (s8*)remote;

	if (PARA_RW == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			br[cnt].min_freq = p[cnt];
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < BANDS_IN_ONE_MONITOR; cnt++) {
			p[cnt] = br[cnt].min_freq;
		}
	} else {
		return -1;
	}
	return 0;
}
static s16 check_modem_tech(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	s32 index;
	s32 cnt;
	modem_tech *l = NULL;
	u8 *r = NULL;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	l = (modem_tech*)local;
	r = (u8*)remote;

	if (PARA_RD == flag) {
		for (cnt = 0; cnt < MODEM_SUPPORT_TECH_MAX; cnt++) {
			r[cnt] = l[cnt].tech;
		}

	} else {
		return -1;
	}

	return 0;
}

static s16 check_cell_resel_sw(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		*((u8*) local) = *((u8*)remote);
		reset_auto_manual_scan_flag(md_adr);
	} else if (PARA_RD == flag) {
		*((u8*)remote) = *((u8*) local);
	} else {
		return -1;
	}

	return 0;
}

static s16 check_freq_scan_sw(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		reset_auto_manual_scan_flag(md_adr);
	}

	return 0;
}

static s16 check_ch_bandwidth(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	u8 cnt;
	f32 *pf;
	f32 *pch;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	pf = (f32*)remote;
	pch = (f32*)local;

	if (PARA_RW == flag) {
		dig_adr = 0xe8;
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pch[cnt] = pf[cnt];
			if (!(cnt % 2)) {
				set_adr_add((dig_adr + (cnt >> 1)), md_adr);
			}
		}
	} else if (PARA_RD == flag) {
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			pf[cnt] = pch[cnt];
		}
	} else {
		return -1;
	}

	return 0;
}
static s16 check_dig_pa_sw(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	s8 cnt;
	s32 mod_index;


	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		return -1;
	} else if (PARA_RD == flag) {
		mod_index = md_adr_to_index(md_adr);
		if (mod_index < 0) {
			return -1;
		}

		if ((band_dynamic_para_a[mod_index].md_dynamic_sundry.dig_dynamic_sundry.dds_ul.dig_pa_sw) && (band_dynamic_para_a[mod_index].md_dynamic_sundry.dig_dynamic_sundry.dds_dl.dig_pa_sw)) {
			band_dynamic_para_a[mod_index].md_dynamic_sundry.dig_dynamic_sundry.dig_pa_sw = DIG_PA_UL_ON_DL_ON;
		} else if (band_dynamic_para_a[mod_index].md_dynamic_sundry.dig_dynamic_sundry.dds_dl.dig_pa_sw) {
			band_dynamic_para_a[mod_index].md_dynamic_sundry.dig_dynamic_sundry.dig_pa_sw = DIG_PA_UL_OFF_DL_ON;
		} else {
			band_dynamic_para_a[mod_index].md_dynamic_sundry.dig_dynamic_sundry.dig_pa_sw = DIG_PA_UL_OFF_DL_OFF;
		}

		*(u8*)remote = band_dynamic_para_a[mod_index].md_dynamic_sundry.dig_dynamic_sundry.dig_pa_sw;
	} else {
		return -1;
	}

	return 0;
}

static s16 check_manual_cellid(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	s8 cnt;
	s32 mod_index;
	s8 tech_index;

	printf("check_manual_cellid() start \r\n");

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		*((u16*)local) = *((u16*)remote);
	} else if (PARA_RD == flag) {
		printf("check_manual_cellid() read \r\n");
		mod_index = md_adr_to_index(md_adr);
		if (mod_index < 0) {
			printf("check_manual_cellid() md_adr_to_index() false \r\n");
			return -1;
		}
		//tech_index = tech_to_tech_index(md_adr, band_dynamic_para_a[mod_index].md_wireless_net_t.scan_tech);
		printf("check_manual_cellid() tech_to_tech_index(md_adr,tech=%d)\r\n", band_dynamic_para_a[mod_index].md_wireless_net_t.scan_tech);
		if (tech_index < 0) {
			return -1;
		}

		*(u16*)remote = band_para_a[mod_index].tel_operator.wireless_info_t[tech_index].ser_cell[2];
		printf("check_manual_cellid:manual_cell_id=%d \r\n", band_para_a[mod_index].tel_operator.wireless_info_t[tech_index].ser_cell[2]);
	} else {
		return -1;
	}

	return 0;
}

/**********************处理与流程相关的回调函数*************************/
extern s32 md_adr_to_index(md_adr_info *md_adr);
static s16 check_relay_sw(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	s32 index;
	s32 cnt;
	u8 *l = NULL;
	u8 *r = NULL;

	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	l = (u8*)local;
	r = (u8*)remote;

	if (PARA_RW == flag) {
		*l = *r;
		index = md_adr_to_index(md_adr);
		if (index < 0) {
			return -1;
		}
		//小区不可重选切换到小区可以重选模式时，需要重新扫描
		if (RELAY_CELL_RESEL_ENABLE == (*(l))) {
			for (cnt = 0; cnt < MODEM_SUPPORT_TECH_MAX; cnt++) {
				CLRBIT(band_dynamic_para_a[index].md_wireless_net_t.modem_tech_a[cnt].scan_status, MODEM_TECH_ASCAN_FINISH);
				CLRBIT(band_dynamic_para_a[index].md_wireless_net_t.modem_tech_a[cnt].scan_status, MODEM_TECH_ASCAN_FALSE);
			}
		}

	} else if (PARA_RD == flag) {
		*r = *l;
	} else {
		return -1;
	}

	return 0;
}


static s16 check_center_freq(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	flag = flag;
	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		*(f32*)(local) = *(f32*)(remote);
		dig_center_freq_update(md_adr);
	} else if (PARA_RD == flag) {
		*(f32*)(remote) = *(f32*)(local);
	} else {
		return -1;
	}

	return 0;
}

static s16 check_relay_mode(void* local, void* remote, md_adr_info *md_adr, u8 dig_adr, u8 flag)
{
	if ((NULL == local) || (NULL == remote) || (NULL == md_adr))
		return -1;

	dig_adr = dig_adr;

	if (PARA_RW == flag) {
		if ((*(u8*)(local)) != (*(u8*)(remote))) {
			dig_relay_mode_change(md_adr);
		}
		*(u8*)(local) = *(u8*)(remote);
	} else if (PARA_RD == flag) {
		*(u8*)(remote) = *(u8*)(local);
	} else {
		return -1;
	}

	return 0;
}

/******************************processing table funs*****************************/
s16 get_para_table_size(void)
{
	return (M_PARA_TABLE_SIZE);
}

s32 find_para_adr(const para_table *table, u16 tlen, u16 adr)
{
	u16 low, high, mid;

	if (NULL == table)
		return -1;
	if (0 == tlen)
		return -1;

	low = 0;
	high = tlen - 1;

	while (low < high) {
		if (adr == table[low].index) {
			return low;
		}
		if (adr == table[high].index) {
			return high;
		}
		mid = low + (high - low) / 2;
		if (adr == table[mid].index)
			return mid;

		if (table[mid].index > adr)
			high = mid - 1;
		else
			low = mid + 1;
	}

	return -1;
}

s32 find_para_adr_mod( md_adr_info *md)
{
	u8 cnt;

	if (NULL == md)
		return -1;

	for (cnt = 0; cnt < MOD_NUM_IN_ONE_PCB; cnt++) {
		if (((md->mod_type) == (band_para_a[cnt].md_adr_t.mod_type)) && \
		        ((md->mod_band) == (band_para_a[cnt].md_adr_t.mod_band)) && \
		        ((md->mod_adr_t.dat) == (band_para_a[cnt].md_adr_t.mod_adr_t.dat))) {
			        return cnt;
		}
	}

	return -1;
}

/*
para:
	para---参数列表记录的信息
	dat---待设置的参数值
return:
	0---sus
	-1---err
*/
static s32 check_para_range(link_para *linkpara, void *dat)
{
	para_limit para_dat;
	u8 para_type;

	memset(&para_dat, 0, sizeof(para_dat));

	para_type = linkpara->para_type_t.dat;

	//RLDEBUG("check_para_range: para type is:%d\r\n", para_type);
	switch (para_type) {
	case PARA_TYPE_U8: {
		memcpy((para_dat.arry), dat, 1);
		if (linkpara->min.u8l == linkpara->max.u8l) {
			return 0;
		} else {
			para_dat.u8l = *((u8*)dat);

			if ((para_dat.u8l < (linkpara->min.u8l)) || (para_dat.u8l > (linkpara->max.u8l)))
				return -1;
		}
		break;
	}
	case PARA_TYPE_S8: {
		memcpy((para_dat.arry), dat, 1);
		if (linkpara->min.s8l == linkpara->max.s8l) {
			return 0;
		} else {
			para_dat.s8l = *((s8*)dat);

			if ((para_dat.s8l < (linkpara->min.s8l)) || (para_dat.s8l > (linkpara->max.s8l)))
				return -1;
		}
		break;
	}
	case PARA_TYPE_U16: {
		memcpy((para_dat.arry), dat, 2);
		if (linkpara->min.u16l == linkpara->max.u16l) {
			return 0;
		} else {
			para_dat.u16l = *((u16*)dat);

			if ((para_dat.u16l < (linkpara->min.u16l)) || (para_dat.u16l > (linkpara->max.u16l)))
				return -1;
		}
		break;
	}
	case PARA_TYPE_S16: {
		memcpy((para_dat.arry), dat, 2);
		if (linkpara->min.s16l == linkpara->max.s16l) {
			return 0;
		} else {
			para_dat.s16l = *((s16*)dat);

			if ((para_dat.s16l < (linkpara->min.s16l)) || (para_dat.s16l > (linkpara->max.s16l)))
				return -1;
		}
		break;
	}
	case PARA_TYPE_U32: {
		memcpy((para_dat.arry), dat, 4);
		if (linkpara->min.u32l == linkpara->max.u32l) {
			return 0;
		} else {
			if ((para_dat.u32l < (linkpara->min.u32l)) || (para_dat.u32l > (linkpara->max.u32l)))
				return -1;
		}
		break;
	}
	case PARA_TYPE_S32: {
		memcpy((para_dat.arry), dat, 4);
		if (linkpara->min.s32l == linkpara->max.s32l) {
			return 0;
		} else {
			if ((para_dat.s32l < (linkpara->min.s32l)) || (para_dat.s32l > (linkpara->max.s32l)))
				return -1;
		}
		break;
	}
	case PARA_TYPE_FLOAT: {
		memcpy((para_dat.arry), dat, 4);
		if (linkpara->min.f32l == linkpara->max.f32l) {
			return 0;
		} else {
			//para_dat.f32l = *((f32*)dat); //will call bus error
			if ((para_dat.f32l < (linkpara->min.f32l)) || (para_dat.f32l > (linkpara->max.f32l)))
				return -1;
		}
		break;
	}
	case PARA_TYPE_DOUBLE: {
		memcpy((para_dat.arry), dat, 8);
		if (linkpara->min.f64l == linkpara->max.f64l) {
			return 0;
		} else {
			para_dat.f64l = *((f64*)dat);

			if ((para_dat.f64l < (linkpara->min.f64l)) || (para_dat.f64l > (linkpara->max.f64l)))
				return -1;
		}
		break;
	}
	case PARA_TYPE_STRING: {

		break;
	}
	case PARA_TYPE_U64: {

		break;
	}
	case PARA_TYPE_S64: {

		break;
	}
	case PARA_TYPE_STRING10: {

		break;
	}
	default:
		break;
	}

	return 0;
}

static s16 get_para_type_len(para_type type)
{
	s16 err = 0;

	switch (type.dat) {
	case PARA_TYPE_U8: {
		err = 1;
		break;
	}
	case PARA_TYPE_S8: {
		err = 1;
		break;
	}
	case PARA_TYPE_U16: {
		err = 2;
		break;
	}
	case PARA_TYPE_S16: {
		err = 2;
		break;
	}
	case PARA_TYPE_U32: {
		err = 4;
		break;
	}
	case PARA_TYPE_S32: {
		err = 4;
		break;
	}
	case PARA_TYPE_FLOAT: {
		err = 4;
		break;
	}
	case PARA_TYPE_DOUBLE: {
		err = 8;
		break;
	}
	case PARA_TYPE_STRING: {
		err = 1;
		break;
	}
	case PARA_TYPE_U64: {
		err = 8;
		break;
	}
	case PARA_TYPE_S64: {
		err = 8;
		break;
	}
	case PARA_TYPE_STRING10: {
		err = 10;
		break;
	}
	default:
		err = -1;

	};

	return err;
}

/*
读单个地址处理函数
 */
s8 one_para_adr_read_processing(const u16 adr, para_stream *ps)
{
	s32 adr_index, mod_index;
	s16 para_num, para_type_len;
	u16 cnt = 0;
	u8 err = 0;
	para *_para = NULL;
	paradeal parafun = NULL;
	para_table *ptable = NULL;
	u16 para_table_size = 0;
	USR_AUTHORITY adr_permission = 0;
    md_adr_info *ptable_para_adr;
    void *ptable_para_dat;

	if ((NULL == ps) || (NULL == (ps->next)))
		return -1;

	_para = (para*)(ps->next);

	//RLDEBUG("one_para_adr_read_processing\r\n");
	if (MOD_TYPE_MONITOR == (ps->md_adr.mod_type))
		mod_index = 0;
	else
		mod_index = find_para_adr_mod( &(ps->md_adr));

	if (mod_index < 0)
	{
		RLDEBUG("one_para_adr_read_processing can't find para adr system\r\n");
		SET_PARA_ERR_CODE(err, PARA_ADR_OTHER_ERR);
		goto ONE_PARA_ADR_READ_ERR;
	}

	if (MOD_TYPE_MONITOR == (ps->md_adr.mod_type)) {
#if MONITOR_MODULE_ENABLE
		ptable = (para_table*)u_para_table_a;
		para_table_size = U_PARA_TABLE_SIZE;
		//RLDEBUG("one_para_adr_read_processing:match unit para table\r\n");
#else
		ptable = NULL;
#endif
	} else {
#if OTHER_MODULE_ENABLE
		ptable = (para_table*)para_table_a;
		para_table_size = M_PARA_TABLE_SIZE;
		//RLDEBUG("one_para_adr_read_processing:match band para table\r\n");
#else
		ptable = NULL;
#endif
	}

	if (NULL == ptable) {
		RLDEBUG("one_para_adr_read_processing can't find para_table\r\n");
		SET_PARA_ERR_CODE(err, PARA_ADR_OTHER_ERR);
		goto ONE_PARA_ADR_READ_ERR;
	}

	adr_index = find_para_adr(ptable, para_table_size, adr);

	if (0 > adr_index) {
		RLDEBUG("one_para_adr_read_processing can't find para adr\r\n");
		SET_PARA_ERR_CODE(err, PARA_ADR_INVALID);
		goto ONE_PARA_ADR_READ_ERR;
	}

	//RLDEBUG("one_para_adr_read_processing:adr index is:%d\r\n", adr_index);
	
    if (MOD_TYPE_MONITOR != (ps->md_adr.mod_type)) {
        ptable_para_adr = &band_para_a[mod_index].md_adr_t;
        ptable_para_dat = ptable[adr_index].link_para_a.link_para_t.dat + sizeof(band_para)*mod_index;
    }


	//RLDEBUG("one_para_adr_read_processing:adr mod index is:%d\r\n", mod_index);
	/*check permission*/
#ifdef RL_PERMISSION_CHECK
	adr_permission = ptable[adr_index].authority;

	if (USR_LOCAL != ps->authority) {
		if (ps->authority < adr_permission)
		{
			RLDEBUG("one_para_adr_read_processing adr:%d permission denied\r\n", adr);
			SET_PARA_ERR_CODE(err, PARA_ADR_PERMISSION_DENIED);
			goto ONE_PARA_ADR_READ_ERR;
		}

	} else {
		if (usr_auth.authorize < adr_permission)
		{
			RLDEBUG("one_para_adr_read_processing adr:%d permission denied\r\n", adr);
			SET_PARA_ERR_CODE(err, PARA_ADR_PERMISSION_DENIED);
			goto ONE_PARA_ADR_READ_ERR;
		}
	}

#endif
	cnt = ptable[adr_index].link_para_a.link_para_t.para_type_t.dat; //para size

	para_type_len = get_para_type_len(ptable[adr_index].link_para_a.link_para_t.para_type_t);

	//RLDEBUG("one_para_adr_read_processing:para type len is:%d\r\n", para_type_len);
	//para type err
	if (!para_type_len) {
		SET_PARA_ERR_CODE(err, PARA_TYPE_ERR);
		goto ONE_PARA_ADR_READ_ERR;
	} else {
		_para->para_adr = adr;
		_para->para_len = ptable[adr_index].link_para_a.link_para_t.len;
		_para->para_type_t.st_t.st = 0;
		_para->para_type_t.st_t.type = (u8)(0x000f & cnt);
	}
	//read dat
	//RLDEBUG("one_para_adr_read_processing:begin read para,para len is:%d\r\n", ptable[adr_index].link_para_a.link_para_t.len);
	ps->next = ps->next + sizeof(para);
	if (NULL != (ptable[adr_index].link_para_a.link_para_t.paradeal)) {
		parafun = (ptable[adr_index].link_para_a.link_para_t.paradeal);
		para_num = parafun((ptable_para_dat), (ps->next), (ptable_para_adr), (ptable[adr_index].dig_adr), PARA_RD);
		if (para_num < 0) {
			//err = ((0xf0 & (PARA_ADR_OTHER_ERR << 4))|(0x0f&((para_table_a[adr_index].link_para_a.link_para_t.para_type_t.dat))));
			SET_PARA_ERR_CODE(err, PARA_ADR_OTHER_ERR);
			SET_PARA_TYPE(err, (ptable[adr_index].link_para_a.link_para_t.para_type_t.dat));
			goto ONE_PARA_ADR_READ_ERR;
		}
	} else {
		memcpy((void*)(ps->next), (void*)(ptable_para_dat), (ptable[adr_index].link_para_a.link_para_t.len));
	}
	ps->next = ps->next + (ptable[adr_index].link_para_a.link_para_t.len);
	ps->paralen += (sizeof(para) + (ptable[adr_index].link_para_a.link_para_t.len));

	return 0;

ONE_PARA_ADR_READ_ERR:
	_para->para_adr = adr;
	_para->para_len = 0;
	_para->para_type_t.dat = err;
	ps->next = ps->next + sizeof(para);
	ps->paralen += sizeof(para);
	return 0;
}


void set_table_para_dat(void ** des, void * src, u32 offset, u16 adr)
{
    u16 i;
    
    for(i=0; i<sizeof(pcb_share_para_table); i++){
        if(adr == pcb_share_para_table[i]) return;
    }

    *des = src + offset;
}


void set_write_data_file_flag(u8 mod_type, u16 adr)
{
    u16 i;

    if (MOD_TYPE_MONITOR == (mod_type)) {
        data_update(DATA_TYPE_UNIT);
    }else{
        for(i=0; i<sizeof(pcb_share_para_table); i++){
            if(adr == pcb_share_para_table[i]){
                data_update(DATA_TYPE_PCB);
                return;
            } 
        }

        data_update(DATA_TYPE_MOD);
    }

}

/*
return:
	0---sus
	-1---err
 */
s8 one_para_adr_set_processing(const s8 *src, para_stream *ps)
{
	s32 adr_index, mod_index = 0;
	s16 para_num, para_type_len;
	u16 cnt;
	u8 err = 0;
	para *_para = NULL;
	paradeal parafun = NULL;
	para_table *ptable = NULL;
	u16 para_table_size = 0;
	USR_AUTHORITY adr_permission = 0;
    md_adr_info *ptable_para_adr;
    void *ptable_para_dat;

	if ((NULL == ps) || (NULL == (ps->next)) || (NULL == src)) {
		return -1;
	}

	_para = (para*)src;
    RLDEBUG("one_para_adr_set_processing: find mod_index\r\n");

    if (MOD_TYPE_MONITOR == (ps->md_adr.mod_type)) {
		mod_index = 0;
	} else {
		mod_index = find_para_adr_mod( &(ps->md_adr));
	}

	if (mod_index < 0) {
		RLDEBUG("one_para_adr_set_processing: adr mod index err\r\n");
		SET_PARA_ERR_CODE(err, PARA_ADR_OTHER_ERR);
		goto ONE_PARA_ADR_SET_ERR;
	}

	if (MOD_TYPE_MONITOR == (ps->md_adr.mod_type)) {
#if MONITOR_MODULE_ENABLE
		ptable = (para_table*)u_para_table_a;
		para_table_size = U_PARA_TABLE_SIZE;
		RLDEBUG("one_para_adr_set_processing: unit para table,the size is:%d\r\n", para_table_size);
#else
		ptable = NULL;
		para_table_size = 0;
#endif
	} else {
#if OTHER_MODULE_ENABLE
		ptable = (para_table*)para_table_a;
		para_table_size = M_PARA_TABLE_SIZE;
		RLDEBUG("one_para_adr_set_processing: band para table;the size is:%d\r\n", para_table_size);
#else
		ptable = NULL;
		para_table_size = 0;
#endif
	}

	if (NULL == ptable) {
		RLDEBUG("one_para_adr_set_processing: can't match para table\r\n");
		SET_PARA_ERR_CODE(err, PARA_ADR_OTHER_ERR);
		goto ONE_PARA_ADR_SET_ERR;
	}

	adr_index = find_para_adr(ptable, para_table_size, _para->para_adr);
	if (0 > adr_index) {
		RLDEBUG("one_para_adr_set_processing: can't fina adr index\r\n");
		SET_PARA_ERR_CODE(err, PARA_ADR_INVALID);
		goto ONE_PARA_ADR_SET_ERR;
	}

    if (MOD_TYPE_MONITOR != (ps->md_adr.mod_type)) {
        ptable_para_adr = &band_para_a[mod_index].md_adr_t;
        // ptable_para_dat = ptable[adr_index].link_para_a.link_para_t.dat + sizeof(band_para)*mod_index;
        set_table_para_dat(&ptable_para_dat, ptable[adr_index].link_para_a.link_para_t.dat, sizeof(band_para)*mod_index, _para->para_adr);
    }

	/*check permission*/
#ifdef RL_PERMISSION_CHECK
	adr_permission = ptable[adr_index].authority;

	if (USR_LOCAL != ps->authority) {
		if (ps->authority < adr_permission)
		{
			RLDEBUG("one_para_adr_set_processing adr:%d permission denied\r\n", adr);
			SET_PARA_ERR_CODE(err, PARA_ADR_PERMISSION_DENIED);
			goto ONE_PARA_ADR_SET_ERR;
		}

	} else {
		if (usr_auth.authorize < adr_permission)
		{
			RLDEBUG("one_para_adr_set_processing adr:%d permission denied\r\n", adr);
			SET_PARA_ERR_CODE(err, PARA_ADR_PERMISSION_DENIED);
			goto ONE_PARA_ADR_SET_ERR;
		}
	}

#endif
	//check rw suport
	if (PARA_RW != (ptable[adr_index].flag)) {
		RLDEBUG("one_para_adr_set_processing adr:only read\r\n");
		SET_PARA_ERR_CODE(err, PARA_ADR_OTHER_ERR);
		goto ONE_PARA_ADR_SET_ERR;
	}
	//check para type
	cnt = ptable[adr_index].link_para_a.link_para_t.para_type_t.dat;
	if (cnt != _para->para_type_t.dat) {
		RLDEBUG("one_para_adr_set_processing:para type err,set_para=%d,expected_para=%d\r\n", _para->para_type_t.dat, cnt);
		SET_PARA_ERR_CODE(err, PARA_TYPE_ERR);
		SET_PARA_TYPE(err, cnt);
		goto ONE_PARA_ADR_SET_ERR;
	}
	//check para len
	cnt = ptable[adr_index].link_para_a.link_para_t.len;
	if (cnt != _para->para_len) {
		RLDEBUG("one_para_adr_set_processing:para len err,set_para_len=%d,expected_para_len=%d\r\n", _para->para_len, cnt);
		SET_PARA_ERR_CODE(err, PARA_LEN_ERR);
		goto ONE_PARA_ADR_SET_ERR;
	}

	//set dat
	if (NULL != (ptable[adr_index].link_para_a.link_para_t.paradeal)) {
		/*check range*/
		para_type_len = get_para_type_len(_para->para_type_t);
		if (para_type_len < 0) {
			RLDEBUG("one_para_adr_set_processing:para type err\r\n");
			SET_PARA_ERR_CODE(err, PARA_TYPE_ERR);
			SET_PARA_TYPE(err, _para->para_type_t.dat);
			goto ONE_PARA_ADR_SET_ERR;
		}

		RLDEBUG("one_para_adr_set_processing:para type len is:%d \r\n", para_type_len);
		RLDEBUG("one_para_adr_set_processing:para type len is:%d \r\n", (_para->para_len / para_type_len));
		for (cnt = 0; cnt < (_para->para_len / para_type_len); cnt++) {

			RLDEBUG("one_para_adr_set_processing:para range check index is:%d \r\n", cnt);
			if (check_para_range(&(ptable[adr_index].link_para_a.link_para_t), (void*)(src + sizeof(para) + cnt * para_type_len))) {
				RLDEBUG("one_para_adr_set_processing:para range err\r\n");
				SET_PARA_ERR_CODE(err, PARA_OVERRANGE);
				SET_PARA_TYPE(err, _para->para_type_t.dat);
				goto ONE_PARA_ADR_SET_ERR;
			}
		}

		parafun = (ptable[adr_index].link_para_a.link_para_t.paradeal);
		para_num = parafun((void*)(ptable_para_dat), (void*)(src + sizeof(para)), (ptable_para_adr), (ptable[adr_index].dig_adr), PARA_RW);
		if (para_num < 0) {
			RLDEBUG("one_para_adr_set_processing:parafun() err\r\n");
			SET_PARA_ERR_CODE(err, PARA_ADR_OTHER_ERR);
			goto ONE_PARA_ADR_SET_ERR;
		}
	} else {
		/*最大最小值判断*/
		if (check_para_range(&(ptable[adr_index].link_para_a.link_para_t), (void*)(src + sizeof(para))))
		{
			RLDEBUG("one_para_adr_set_processing:para range err\r\n");
			SET_PARA_ERR_CODE(err, PARA_OVERRANGE);
			SET_PARA_TYPE(err, _para->para_type_t.dat);
			goto ONE_PARA_ADR_SET_ERR;
		}
		/*复制参数*/
		memcpy((void*)(ptable_para_dat), (void*)(src + sizeof(para)), (ptable[adr_index].link_para_a.link_para_t.len));
	}

	if (ptable[adr_index].dig_adr) {
		set_adr_add(ptable[adr_index].dig_adr , ptable_para_adr);
	}
    
    //TODO: need to move it out
    set_write_data_file_flag(ps->md_adr.mod_type, _para->para_adr);

	ps->next = ps->next + (ptable[adr_index].link_para_a.link_para_t.len);

	return 0;
    
ONE_PARA_ADR_SET_ERR:
    //err return format:para adr,err + data type, data len
	RLDEBUG("one_para_adr_set_processing err,the err adr is:%x \r\n", _para->para_adr);
	*(u16*)(ps->next) = _para->para_adr;	//para adr
	ps->next = ps->next + 2;
	if ((PARA_ADR_INVALID == GET_PARA_ERR_CODE(err)) || (PARA_ADR_OTHER_ERR == GET_PARA_ERR_CODE(err))) {
		*(u8*)(ps->next) = err;
	} else {
		*(u8*)(ps->next) = err | (ptable[adr_index].link_para_a.link_para_t.para_type_t.dat);		//para err
	}
	ps->next = ps->next + 1;
	if (PARA_LEN_ERR == GET_PARA_ERR_CODE(err))
		*(u16*)(ps->next) = (ptable[adr_index].link_para_a.link_para_t.len);
	else
		*(u16*)(ps->next) = 0;
	ps->next = ps->next + 2;

	ps->paralen += sizeof(para);
	return -1;
}

