/*
name:dig_para_table.h
author:luke
date:20170324
ver:1.0
description:
	dig parameter dat deal
*/


/*****************************include files***********************/
#include <string.h>
#include "helios.h"
#include "common_api.h"
#include "log.h"
#include "digital.h"
#include "dig_para_table_def.h"
#include "dig_band0_para_table.h"
#include "para_table.h"
#include "porting.h"

/*****************************para define*************************/

const dig_para_item dig_para_table_set[] =
{
	{
		.key = 0x01,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[0].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x02,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[1].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x03,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[2].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x04,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[3].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x05,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[4].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x06,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[5].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x07,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[6].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x08,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[7].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x09,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[8].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x0a,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[9].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x0b,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[10].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x0c,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[11].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x0d,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[0].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x0e,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[1].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x0f,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[2].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x10,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[3].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x11,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[4].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x12,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[5].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x13,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[6].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x14,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[7].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x15,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[8].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x16,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[9].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x17,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[10].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x18,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[11].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x19,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[0].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[0].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x1a,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[1].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[1].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x1b,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[2].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[2].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x1c,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[3].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[3].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x1d,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[4].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[4].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x1e,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[5].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[5].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x1f,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[6].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[6].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x20,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[7].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[7].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x21,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[8].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[8].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x22,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[9].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[9].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x23,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[10].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[10].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x24,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[11].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[11].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x25,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[0].mute_sw),
		.adr = DIG_ADR_BIT20, //DIG_ADR_BIT31 @ccTagOK20170725
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[1].mute_sw),
		.adr = DIG_ADR_BIT21,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[2].mute_sw),
		.adr = DIG_ADR_BIT22,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[3].mute_sw),
		.adr = DIG_ADR_BIT23,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[4].mute_sw),
		.adr = DIG_ADR_BIT24,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[5].mute_sw),
		.adr = DIG_ADR_BIT25,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[6].mute_sw),
		.adr = DIG_ADR_BIT26,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[7].mute_sw),
		.adr = DIG_ADR_BIT27,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[8].mute_sw),
		.adr = DIG_ADR_BIT28,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[9].mute_sw),
		.adr = DIG_ADR_BIT29,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[10].mute_sw),
		.adr = DIG_ADR_BIT30,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[11].mute_sw),
		.adr = DIG_ADR_BIT31,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x26,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[0].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[1].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x27,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[2].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[3].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x28,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[4].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[5].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x29,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[6].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[7].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x2a,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[8].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[9].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x2b,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[10].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[11].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x2c,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[0].traffic_th),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[1].traffic_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[2].traffic_th),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[3].traffic_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x2d,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[4].traffic_th),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[5].traffic_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[6].traffic_th),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[7].traffic_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x2e,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[8].traffic_th),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[9].traffic_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[10].traffic_th),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[11].traffic_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x2f,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[0].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[1].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x30,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[2].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[3].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x31,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[4].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[5].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x32,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[6].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[7].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x33,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[8].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[9].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x34,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[10].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[11].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x35,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[0].mute_th_h),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[0].mute_th_l),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[1].mute_th_h),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[1].mute_th_l),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x36,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[2].mute_th_h),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[2].mute_th_l),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[3].mute_th_h),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[3].mute_th_l),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x37,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[4].mute_th_h),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[4].mute_th_l),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[5].mute_th_h),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[5].mute_th_l),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x38,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[6].mute_th_h),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[6].mute_th_l),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[7].mute_th_h),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[7].mute_th_l),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x39,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[8].mute_th_h),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[8].mute_th_l),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[9].mute_th_h),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[9].mute_th_l),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x3a,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[10].mute_th_h),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[10].mute_th_l),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[11].mute_th_h),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.ul[11].mute_th_l),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x3b,
		.pdat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.dds_ul.gain_equalize),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x3c,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[0].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[1].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x3d,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[2].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[3].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x3e,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[4].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[5].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x3f,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[6].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[7].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x40,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[8].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[9].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x41,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[10].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[11].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x42,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[0].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[0].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x43,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[1].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[1].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x44,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[2].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[2].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x45,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[3].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[3].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x46,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[4].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[4].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x47,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[5].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[5].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x48,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[6].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[6].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x49,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[7].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[7].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x4a,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[8].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[8].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x4b,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[9].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[9].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x4c,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[10].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[10].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x4d,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[11].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.adi_offset[11].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x68,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[0].workfreq), //ch_info_t.ul @ccTagOK20170725
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x69,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[1].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x6a,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[2].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x6b,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[3].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x6c,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[4].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x6d,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[5].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x6e,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[6].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x6f,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[7].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x70,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[8].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x71,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[9].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x72,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[10].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x73,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[11].workfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x74,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[0].shiftfreq), //ch_info_t.ul @ccTagOK20170725
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x75,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[1].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x76,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[2].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x77,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[3].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x78,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[4].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x79,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[5].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x7a,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[6].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x7b,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[7].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x7c,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[8].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x7d,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[9].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x7e,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[10].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x7f,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[11].shiftfreq),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0x80,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[0].pin_op_th), //ch_info_t.ul @ccTagOK20170725
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[0].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x81,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[1].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[1].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x82,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[2].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[2].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x83,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[3].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[3].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x84,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[4].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[4].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x85,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[5].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[5].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x86,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[6].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[6].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x87,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[7].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[7].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x88,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[8].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[8].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x89,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[9].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[9].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x8a,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[10].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[10].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x8b,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[11].pin_op_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[11].pin_lp_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x8c,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[0].sw),
		.adr = DIG_ADR_BIT16,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[1].sw),
		.adr = DIG_ADR_BIT17,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[2].sw),
		.adr = DIG_ADR_BIT18,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[3].sw),
		.adr = DIG_ADR_BIT19,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[4].sw),
		.adr = DIG_ADR_BIT20,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[5].sw),
		.adr = DIG_ADR_BIT21,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[6].sw),
		.adr = DIG_ADR_BIT22,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[7].sw),
		.adr = DIG_ADR_BIT23,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[8].sw),
		.adr = DIG_ADR_BIT24,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[9].sw),
		.adr = DIG_ADR_BIT25,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[10].sw),
		.adr = DIG_ADR_BIT26,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[11].sw),
		.adr = DIG_ADR_BIT27,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].md_sundry.dig_sundry.ics_sw),
		.adr = DIG_ADR_BIT0,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x8d,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[0].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[1].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x8e,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[2].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[3].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x8f,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[4].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[5].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x90,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[6].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[7].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x91,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[8].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[9].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x92,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[10].agc_th),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[11].agc_th),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,
		.pdatadeal = NULL
	},
	{
		.key = 0x93,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[0].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[1].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x94,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[2].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[3].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x95,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[4].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[5].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x96,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[6].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[7].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x97,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[8].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[9].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x98,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[10].att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.dl[11].att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8,	//DIG_LEN_F12 @ccTagOK20170725
		.pdatadeal = NULL
	},
	{
		.key = 0x99,
		.pdat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.dds_dl.gain_equalize),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x9a,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[0].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[1].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x9b,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[2].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[3].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x9c,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[4].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[5].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x9d,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[6].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[7].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x9e,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[8].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[9].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x9f,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[10].flat),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[11].flat),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xa0,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[0].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[0].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xa1,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[1].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[1].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xa2,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[2].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[2].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xa3,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[3].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[3].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xa4,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[4].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[4].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xa5,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[5].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[5].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xa6,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[6].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[6].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xa7,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[7].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[7].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xa8,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[8].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[8].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xa9,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[9].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[9].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xaa,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[10].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[10].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xab,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[11].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.adi_offset[11].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xd0,
		.pdat = (void*) & (band_para_a[0].md_sundry.dig_sundry.relay_sw),
		.adr = DIG_ADR_BIT31,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0xd1,
		.pdat = (void*) & (band_para_a[0].md_sundry.dig_sundry.donor_remote),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xd2,
		.pdat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.work_mode),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
/* 	{
		.key = 0xd3,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.gain_att.tx_att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8, // DIG_LEN_F12 @ccTag20170731
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.gain_att.rx_gain),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xd4,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.gain_att.tx_att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8, // DIG_LEN_F12 @ccTag20170731
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.gain_att.rx_gain),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xd5,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.freq_rx_int),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0xd6,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.freq_rx_frac),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xd7,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.freq_tx_int),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0xd8,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.freq_tx_frac),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xd9,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.freq_rx_int),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0xda,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.freq_rx_frac),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xdb,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.freq_tx_int),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0xdc,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.freq_tx_frac),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xdd,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.dc_image.phase),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8, //DIG_LEN_S16 @ccTag20170731
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.dc_image.amp),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8, //DIG_LEN_S16 @ccTag20170731
		.pdatadeal = NULL
	},
	{
		.key = 0xde,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.dc_image.dc_i),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8, //DIG_LEN_S16 @ccTag20170731
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_dl.dc_image.dc_q),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8, //DIG_LEN_S16 @ccTag20170731
		.pdatadeal = NULL
	},
	{
		.key = 0xdf,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.dc_image.phase),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8, //DIG_LEN_S16 @ccTag20170731
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.dc_image.amp),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8, //DIG_LEN_S16 @ccTag20170731
		.pdatadeal = NULL
	},
	{
		.key = 0xe0,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.dc_image.dc_i),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_S8, //DIG_LEN_S16 @ccTag20170731
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].adi_regulator_cur_ul.dc_image.dc_q),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_S8, //DIG_LEN_S16 @ccTag20170731
		.pdatadeal = NULL
	},*/
	{
		.key = 0xe1,
		.pdat = (void*) & (band_para_a[0].md_sundry.dig_sundry.center_freq.ul),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0xe2,
		.pdat = (void*) & (band_para_a[0].md_sundry.dig_sundry.center_freq.dl),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0xe3,
		.pdat = (void*) & (band_para_a[0].md_sundry.dig_sundry.center_freq.dl),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	},
	{
		.key = 0xe4,
		.pdat = (void*) & (band_para_a[0].md_sundry.dig_sundry.center_freq.ul),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = f32tou32
	}, 
	{
		.key = 0xe5,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[0]),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[1]),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[2]),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[3]),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xe6,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[4]),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[5]),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[6]),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[7]),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xe7,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[8]),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[9]),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[10]),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.uldl_mix_th[11]),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xe8,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[0]),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[1]),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0xe9,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[2]),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[3]),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0xea,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[4]),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[5]),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0xeb,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[6]),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[7]),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0xec,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[8]),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[9]),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0xed,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[10]),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].ch_info_t.bandwidth[11]),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32U16,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = f32tou16
	},
	{
		.key = 0xee,
		.pdat = (void*) & (band_para_a[0].md_sundry.dig_sundry.att_ul.lna_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].md_sundry.dig_sundry.att_ul.pa_att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].md_sundry.dig_sundry.att_dl.lna_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_para_a[0].md_sundry.dig_sundry.att_dl.pa_att),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,// DIG_LEN_U16 @ccTag20170808
		.pdatadeal = NULL
	},
	{//ad9370 config
		.key = 0xf4,
		.pdat = (void*) & (band_dynamic_para_a[0].ad9370_conf.chip_select),
		.adr = DIG_ADR_BIT6,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ad9370_conf.jesd204b_fb_rst),
		.adr = DIG_ADR_BIT5,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ad9370_conf.jesd204b_tx_rst),
		.adr = DIG_ADR_BIT4,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ad9370_conf.jesd204b_rx_rst),
		.adr = DIG_ADR_BIT3,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ad9370_conf.gtp_rx),
		.adr = DIG_ADR_BIT2,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ad9370_conf.gtp_tx),
		.adr = DIG_ADR_BIT1,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ad9370_conf.reset_fpga),
		.adr = DIG_ADR_BIT0,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0xff,
		.pdat = NULL,
		.adr = 0,
		.len = 0,
		.pdatadeal = NULL
	},
};

const dig_para_item dig_para_table_read[] =
{
	{
		.key = 0x4e,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0].traffic),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[1].traffic),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0x4f,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[2].traffic),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[3].traffic),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0x50,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[4].traffic),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[5].traffic),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0x51,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[6].traffic),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[7].traffic),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0x52,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[8].traffic),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[9].traffic),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0x53,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[10].traffic),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[11].traffic),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U16,
		.pdatadeal = NULL
	},
	{
		.key = 0x54,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[1].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[1].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x55,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[2].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[2].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[3].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[3].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x56,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[4].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[4].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[5].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[5].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x57,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[6].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[6].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[7].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[7].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x58,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[8].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[8].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[9].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[9].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x59,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[10].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[10].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[11].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[11].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x5a,
		.pdat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.dds_ul.adc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0].pin_op),
		.adr = DIG_ADR_BIT12,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[1].pin_op),
		.adr = DIG_ADR_BIT13,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[2].pin_op),
		.adr = DIG_ADR_BIT14,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[3].pin_op),
		.adr = DIG_ADR_BIT15,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[4].pin_op),
		.adr = DIG_ADR_BIT16,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[5].pin_op),
		.adr = DIG_ADR_BIT17,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[6].pin_op),
		.adr = DIG_ADR_BIT18,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[7].pin_op),
		.adr = DIG_ADR_BIT19,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[8].pin_op),
		.adr = DIG_ADR_BIT20,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[9].pin_op),
		.adr = DIG_ADR_BIT21,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[10].pin_op),
		.adr = DIG_ADR_BIT22,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[11].pin_op),
		.adr = DIG_ADR_BIT23,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0].pin_lp),
		.adr = DIG_ADR_BIT0,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[1].pin_lp),
		.adr = DIG_ADR_BIT1,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[2].pin_lp),
		.adr = DIG_ADR_BIT2,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[3].pin_lp),
		.adr = DIG_ADR_BIT3,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[4].pin_lp),
		.adr = DIG_ADR_BIT4,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[5].pin_lp),
		.adr = DIG_ADR_BIT5,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[6].pin_lp),
		.adr = DIG_ADR_BIT6,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[7].pin_lp),
		.adr = DIG_ADR_BIT7,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[8].pin_lp),
		.adr = DIG_ADR_BIT8,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[9].pin_lp),
		.adr = DIG_ADR_BIT9,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[10].pin_lp),
		.adr = DIG_ADR_BIT10,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[11].pin_lp),
		.adr = DIG_ADR_BIT11,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x5b,
		.pdat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.dds_ul.dig_pa_sw),
		.adr = DIG_ADR_BIT31,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].alarm.ad80305_1),
		.adr = DIG_ADR_BIT30,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x5c,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[1].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x5d,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[2].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[3].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x5e,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[4].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[5].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x5f,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[6].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[7].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x60,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[8].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[9].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x61,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[10].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[11].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x62,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[0].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[1].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x63,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[2].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[3].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x64,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[4].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[5].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x65,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[6].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[7].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x66,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[8].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[9].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x67,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[10].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.ul[11].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	//dl read para
	{
		.key = 0xac,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[0].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[0].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[1].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[1].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xad,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[2].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[2].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[3].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[3].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xae,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[4].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[4].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[5].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[5].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xaf,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[6].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[6].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[7].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[7].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xb0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[8].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[8].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[9].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[9].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xb1,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[10].agc_att),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[10].gain_compensate),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[11].agc_att),
		.adr = DIG_ADR_BYTE_8,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[11].gain_compensate),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0xb2,
		.pdat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.ics_iso_dl),
		.adr = DIG_ADR_BYTE_24,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.dds_dl.adc_att),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_U8,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.dds_dl.dig_pa_sw),
		.adr = DIG_ADR_BIT15,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].alarm.ad80305_2),
		.adr = DIG_ADR_BIT14,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0xb3,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[11].pin_op),
		.adr = DIG_ADR_BIT23,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[10].pin_op),
		.adr = DIG_ADR_BIT22,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[9].pin_op),
		.adr = DIG_ADR_BIT21,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[8].pin_op),
		.adr = DIG_ADR_BIT20,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[7].pin_op),
		.adr = DIG_ADR_BIT19,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[6].pin_op),
		.adr = DIG_ADR_BIT18,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[5].pin_op),
		.adr = DIG_ADR_BIT17,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[4].pin_op),
		.adr = DIG_ADR_BIT16,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[3].pin_op),
		.adr = DIG_ADR_BIT15,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[2].pin_op),
		.adr = DIG_ADR_BIT14,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[1].pin_op),
		.adr = DIG_ADR_BIT13,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[0].pin_op),
		.adr = DIG_ADR_BIT12,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[11].pin_lp),
		.adr = DIG_ADR_BIT11,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[10].pin_lp),
		.adr = DIG_ADR_BIT10,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[9].pin_lp),
		.adr = DIG_ADR_BIT9,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[8].pin_lp),
		.adr = DIG_ADR_BIT8,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[7].pin_lp),
		.adr = DIG_ADR_BIT7,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[6].pin_lp),
		.adr = DIG_ADR_BIT6,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[5].pin_lp),
		.adr = DIG_ADR_BIT5,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[4].pin_lp),
		.adr = DIG_ADR_BIT4,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[3].pin_lp),
		.adr = DIG_ADR_BIT3,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[2].pin_lp),
		.adr = DIG_ADR_BIT2,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[1].pin_lp),
		.adr = DIG_ADR_BIT1,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[0].pin_lp),
		.adr = DIG_ADR_BIT0,
		.len = DIG_LEN_BIT,
		.pdatadeal = NULL
	},
	{
		.key = 0xb4,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[0].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[1].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xb5,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[2].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[3].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xb6,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[4].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[5].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xb7,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[6].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[7].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xb8,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[8].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[9].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xb9,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[10].pout),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[11].pout),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xba,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[0].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[1].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xbb,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[2].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[3].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xbc,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[4].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[5].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xbd,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[6].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[7].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xbe,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[8].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[9].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xbf,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[10].pin),
		.adr = DIG_ADR_BYTE_16,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0x00,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.dl[11].pin),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_F12,
		.pdatadeal = NULL
	},
	{
		.key = 0xc0,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[0].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xc1,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[1].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xc2,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[2].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xc3,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[3].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xc4,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[4].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xc5,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[5].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xc6,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[6].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xc7,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[7].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xc8,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[8].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xc9,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[9].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xca,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[10].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL,
	},
	{
		.key = 0xcb,
		.pdat = (void*) & (band_dynamic_para_a[0].ch_rf_t.feature[11].data),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xfd,
		.pdat = (void*) & (band_dynamic_para_a[0].md_dynamic_sundry.dig_dynamic_sundry.sv),
		.adr = DIG_ADR_BYTE_0,
		.len = DIG_LEN_U32,
		.pdatadeal = NULL
	},
	{
		.key = 0xff,
		.pdat = NULL,
		.adr = 0,
		.len = 0,
		.pdatadeal = NULL
	}
};

#define DIG_PARA_SET_TABLE_SIZE (sizeof(dig_para_table_set)/sizeof(dig_para_item))
#define DIG_PARA_READ_TABLE_SIZE (sizeof(dig_para_table_read)/sizeof(dig_para_item))
/*****************************data struct define******************/


/*
para:
	adr:参数地址
	items:返回的对应adr的索引和该地址下的参数个数
return：
	=0 sus
	-1--err
 */
static s8 find_para_table(u8 adr, items_info *items)
{
	s16 cnt;

	if (0 == adr)
		return -1;
	if (NULL == items)
		return -1;

	for (cnt = 0; cnt < ((s16)(DIG_PARA_SET_TABLE_SIZE)); cnt++) {
		if (adr == dig_para_table_set[cnt].key) {
			//RLDEBUG("find_para_table:adr == dig_para_table_set[cnt].key---->adr is:%x---index is:%x", adr, cnt);
			break;
		}
	}

	if (cnt >= ((s16)(DIG_PARA_SET_TABLE_SIZE))) {
		RLDEBUG("find_para_table:cnt >= ((s16)(DIG_PARA_SET_TABLE_SIZE))1 \r\n");
		return -1;
	} else {
		items->index = cnt;
		for (cnt = 1; cnt < ((s16)(DIG_PARA_SET_TABLE_SIZE)); cnt++) {
			if (dig_para_table_set[items->index + cnt].key) {
				items->para_nums = cnt;
				break;
			}
		}

		if (cnt >= ((s16)(DIG_PARA_SET_TABLE_SIZE))) {
			RLDEBUG("find_para_table:cnt >= ((s16)(DIG_PARA_SET_TABLE_SIZE))2 \r\n");
			return -1;
		}
	}

	return 0;
}
/*
para:
	adr:参数地址
return：
	>0 地址索引值
	-1--err
 */
static s8 find_para_table_read(u8 adr, items_info *items)
{
	s16 cnt;

	if (0 == adr)
		return -1;
	if (NULL == items)
		return -1;

	for (cnt = 0; cnt < ((s16)(DIG_PARA_READ_TABLE_SIZE)); cnt++) {
		if (adr == dig_para_table_read[cnt].key) {
			//RLDEBUG("find_para_table_read:adr in the read table index is:%d \r\n", cnt);
			break;
		}
	}

	if (cnt >= ((s16)(DIG_PARA_READ_TABLE_SIZE))) {
		RLDEBUG("find_para_table_read:cnt >= ((s16)(DIG_PARA_READ_TABLE_SIZE)) ERR1 \r\n");
		RLDEBUG("find_para_table_read:adr is:%x,cnt is:%d \r\n", adr, cnt);
		return -1;
	}
	else {
		items->index = cnt;
		for (cnt = 1; cnt < ((s16)(DIG_PARA_READ_TABLE_SIZE)); cnt++) {
			if (dig_para_table_read[items->index + cnt].key) {
				items->para_nums = cnt;
				break;
			}
		}

		//RLDEBUG("find_para_table_read:read para table size is:%d \r\n", DIG_PARA_READ_TABLE_SIZE);
		//RLDEBUG("find_para_table_read:adr begain index:%d \r\n", items->index);
		//RLDEBUG("find_para_table_read:read para numbers are:%d \r\n", cnt);

		if (cnt >= ((s16)(DIG_PARA_READ_TABLE_SIZE))) {
			RLDEBUG("find_para_table_read:cnt >= ((s16)(DIG_PARA_READ_TABLE_SIZE)) ERR2 \r\n");
			return -1;
		}
	}
	return 0;
}

/*
para:
	adr:参数地址
return：
	0--sus
	-1--err
 */
s8 fill_one_adr(u8 adr, void *dest)
{
	u8 items;
	s16 index;
	all_data_type data;
	dig_para_struct para_st;
	dig_para_item *pitem;
	items_info items_info_t;

	if (NULL == dest) {
		RLDEBUG("fill_one_adr: NULL == dest\r\n");
		return -1;
	}
	if ((0 == adr) || (0xff == adr)) {
		RLDEBUG("fill_one_adr: (0 == adr) || (0xff == adr)\r\n");
		RLDEBUG("fill_one_adr: adr=%x \r\n", adr);
		return -1;
	}

	index = find_para_table(adr, &items_info_t);
	if (index < 0) {
		RLDEBUG("fill_one_adr: index = find_para_table(adr, &items_info_t); false\r\n");
		return -1;
	}

	//RLDEBUG("fill_one_adr: para adr is:%x\r\n", adr);
	//RLDEBUG("fill_one_adr: para index is:%x\r\n", items_info_t.index);
	//RLDEBUG("fill_one_adr: para numbers is:%x\r\n", items_info_t.para_nums);

	memset((void*)&para_st, 0, sizeof(para_st));

	for (index = 0; index < (items_info_t.para_nums); index++) {
		pitem = (dig_para_item*) & (dig_para_table_set[items_info_t.index + index]);

		if (DIG_LEN_U8 == pitem->len) {
			data.u8data = * ((u8*)(pitem->pdat));
			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.u8data), &(data.u8data));
			}
			para_st.u8_arry[pitem->adr - DIG_ADR_BYTE_0] = data.u8data;
		} else if (DIG_LEN_U16 == pitem->len) {
			data.u16data = * ((u16*)(pitem->pdat));
			//RLDEBUG("fill_one_adr: DIG_LEN_U16 == pitem->len:----%x\r\n", *(u16*)(pitem->pdat));
			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.u16data), &(data.u16data));
			}
			if (DIG_ADR_BYTE_0 == (pitem->adr)) {
				para_st.u16_arry[0] = data.u16data;
			} else if (DIG_ADR_BYTE_16 == (pitem->adr)) {
				para_st.u16_arry[1] = data.u16data;
			}
		} else if (DIG_LEN_U32 == pitem->len) {
			data.u32data = * ((u32*)(pitem->pdat));
			//RLDEBUG("fill_one_adr: DIG_LEN_U32 == pitem->len:----%f\r\n", *(f32*)(pitem->pdat));
			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.u32data), &(data.u32data));
			}
			if (DIG_ADR_BYTE_0 == (pitem->adr)) {
				para_st.u32_data = data.u32data;
			}
		} else if (DIG_LEN_F12 == pitem->len) {
			data.f32data = * ((f32*)(pitem->pdat));
			//RLDEBUG("fill_one_adr: DIG_LEN_F12 == pitem->len:----%f\r\n", *(f32*)(pitem->pdat));
			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.f32data), &(data.u16data));
			}
			f32tof12(* ((f32*)(pitem->pdat)), (&data.s16data));
			//RLDEBUG("fill_one_adr: DIG_LEN_F12 == pitem->len:----%x\r\n", data.u16data);
			//RLDEBUG("fill_one_adr: DIG_LEN_F12 == pitem->len:----%d\r\n", data.u16data);
			if (DIG_ADR_BYTE_0 == (pitem->adr)) {
				para_st.u16_arry[0] = data.u16data;
			} else if (DIG_ADR_BYTE_16 == (pitem->adr)) {
				para_st.u16_arry[1] = data.u16data;
			}
		} else if (DIG_LEN_S8 == pitem->len) {
			data.s8data = * ((s8*)(pitem->pdat));
			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.s8data), &(data.s8data));
			}
			para_st.u8_arry[pitem->adr - DIG_ADR_BYTE_0] = data.s8data;
		} else if (DIG_LEN_S16 == pitem->len) {
			data.s16data = * ((s16*)(pitem->pdat));
			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.s16data), &(data.s16data));
			}
			if (DIG_ADR_BYTE_0 == (pitem->adr)) {
				para_st.u16_arry[1] = data.s16data;
			} else if (DIG_ADR_BYTE_16 == (pitem->adr)) {
				para_st.u16_arry[0] = data.s16data;
			}
		} else if (DIG_LEN_S32 == pitem->len) {
			data.s32data = * ((s32*)(pitem->pdat));
			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.s32data), &(data.s32data));
			}
			if (DIG_ADR_BYTE_0 == (pitem->adr)) {
				para_st.u32_data = data.s32data;
			}
		} else if (DIG_LEN_BIT == pitem->len) {
			data.u8data =  (* (u8*)(pitem->pdat));
			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.u8data), &(data.u8data));
			}
			if (data.u8data) {
				SETBIT(para_st.u32_data, (pitem->adr - DIG_ADR_BIT0));
			}
		} else if (DIG_LEN_U32U16 == pitem->len) { //@ccTag20170808
			data.u32data = * ((u32*)(pitem->pdat));

			//RLDEBUG("fill_one_adr: DIG_LEN_U32 == pitem->len:----%f\r\n", *(f32*)(pitem->pdat));
			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.u32data), &(data.u16data));
			}

			if (DIG_ADR_BYTE_0 == (pitem->adr)) {
				para_st.u16_arry[0] = data.u16data;
			} else if (DIG_ADR_BYTE_16 == (pitem->adr)) {
				para_st.u16_arry[1] = data.u16data;
			}
		}
	}

	memcpy((void*)dest, (void*)(para_st.u8_arry), sizeof(para_st));

	return 0;
}

/*
para:
	adr:参数地址
return：
	0--sus
	-1--err
 */
s8 analyze_one_adr(u8 adr, s8 *src)
{
	s16 index;
	all_data_type data;
	dig_para_struct para_st;
	dig_para_item *pitem;
	items_info items_info_t;

	if (NULL == src)
		return -1;
	if ((0 == adr) || (0xff == adr))
		return -1;

	//RLDEBUG("analyze_one_adr: start \r\n");
	//RLDEBUG("analyze_one_adr: adr is:%x\r\n", adr);

	memset(&items_info_t, 0, sizeof(items_info));
	index = find_para_table_read(adr, &items_info_t);
	if (index < 0)
	{
		RLDEBUG("analyze_one_adr: find_para_table_read() err \r\n");
		return -1;
	}

	memset((void*)&para_st, 0, sizeof(para_st));
	memcpy((void*)(para_st.u8_arry), (void*)src, sizeof(para_st));

	//RLDEBUG("analyze_one_adr: data is:%x \r\n", para_st.u32_data);
	//RLDEBUG("analyze_one_adr: para index begin is:%x \r\n", (items_info_t.index));
	//RLDEBUG("analyze_one_adr: para numbers is:%x \r\n", items_info_t.para_nums);

	for (index = 0; index < (items_info_t.para_nums); index++) {
		pitem = (dig_para_item*) & (dig_para_table_read[index + items_info_t.index]);
		//RLDEBUG("analyze_one_adr: para index is:%x \r\n", (index + items_info_t.index));
		if (DIG_LEN_U8 == pitem->len) {

			if ((DIG_ADR_BYTE_0 != (pitem->adr)) && (DIG_ADR_BYTE_8 != (pitem->adr)) && (DIG_ADR_BYTE_16 != (pitem->adr)) && (DIG_ADR_BYTE_24 != (pitem->adr))) {
				RLDEBUG("analyze_one_adr: DIG_LEN_U8 == pitem->len \r\n");
				return -1;
			}

			data.u8data = para_st.u8_arry[pitem->adr - DIG_ADR_BYTE_0];

			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.u8data), &(data.u8data));
			}
			*(u8*)(pitem->pdat) = data.u8data;
		} else if (DIG_LEN_U16 == pitem->len) {
			if (DIG_ADR_BYTE_0 == (pitem->adr)) {
				data.u16data = para_st.u16_arry[0]; //para_st.u16_arry[1] @ccTagOK20170725
			} else if (DIG_ADR_BYTE_16 == (pitem->adr)) {
				data.u16data = para_st.u16_arry[1]; //para_st.u16_arry[0] @ccTagOK20170725
			} else
				return -1;

			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.u16data), &(data.u16data));
			}
			*(u16*)(pitem->pdat) = data.u16data;
		} else if (DIG_LEN_U32 == pitem->len) {
			if (DIG_ADR_BYTE_0 == (pitem->adr))
				data.u32data = para_st.u32_data;
			else
				return -1;

			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.u32data), &(data.u32data));
			}

			*(u32*)(pitem->pdat) = data.u32data;
		} else if (DIG_LEN_F12 == pitem->len) {
			if (DIG_ADR_BYTE_0 == (pitem->adr)) {
				if (0x0800 & (para_st.u16_arry[0]))
					data.s16data = (para_st.u16_arry[0]) | 0xf000;
				else
					data.s16data = para_st.u16_arry[0];
			} else if (DIG_ADR_BYTE_16 == (pitem->adr)) {
				if (0x0800 & (para_st.u16_arry[1]))
					data.s16data = (para_st.u16_arry[1]) | 0xf000;
				else
					data.s16data = para_st.u16_arry[1];
			} else {
				RLDEBUG("analyze_one_adr: DIG_ADR_BYTE_0 != (pitem->adr) && DIG_ADR_BYTE_16 != (pitem->adr)\r\n");
				return -1;
			}

			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.f32data), &(data.u16data));
			}
			f12tof32(data.s16data, (f32*)(pitem->pdat));
		} else if (DIG_LEN_S8 == pitem->len) {
			if ((DIG_ADR_BYTE_0 != (pitem->adr)) && (DIG_ADR_BYTE_8 != (pitem->adr)) && (DIG_ADR_BYTE_16 != (pitem->adr)) && (DIG_ADR_BYTE_24 != (pitem->adr)))
				return -1;

			data.s8data = para_st.s8_arry[pitem->adr - DIG_ADR_BYTE_24];

			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.s8data), &(data.s8data));
			}
			*(s8*)(pitem->pdat) = data.s8data;
		} else if (DIG_LEN_S16 == pitem->len) {
			if (DIG_ADR_BYTE_0 == (pitem->adr)) {
				data.s16data = para_st.s16_arry[0]; // para_st.s16_arry[1] @ccTagOK20170725
			} else if (DIG_ADR_BYTE_16 == (pitem->adr)) {
				data.s16data = para_st.s16_arry[1]; //para_st.s16_arry[0] @ccTagOK20170725
			} else
				return -1;

			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.s16data), &(data.s16data));
			}
			*(s16*)(pitem->pdat) = data.s16data;
		} else if (DIG_LEN_S32 == pitem->len) {
			if (DIG_ADR_BYTE_0 == (pitem->adr))
				data.s32data = para_st.s32_data;
			else
				return -1;

			if (NULL != pitem->pdatadeal) {
				pitem->pdatadeal(&(data.s32data), &(data.s32data));
			}

			*(s32*)(pitem->pdat) = data.s32data;
		} else if (DIG_LEN_BIT == pitem->len) {
			if ((DIG_ADR_BIT0 > (pitem->adr)) || (DIG_ADR_BIT31 < (pitem->adr))) {
				RLDEBUG("analyze_one_adr: (DIG_ADR_BIT0 < (pitem->adr)) || (DIG_ADR_BIT31 < (pitem->adr)) \r\n");
				return -1;
			}

			data.u32data = para_st.u32_data;
			//RLDEBUG("analyze_one_adr: data.u32data =:%x \r\n", (data.u32data));
			if (GETBIT(data.u32data, (pitem->adr - DIG_ADR_BIT0))) {
				*(u8*)(pitem->pdat) = 1;
			} else {
				*(u8*)(pitem->pdat) = 0;
			}
		}
	}
	return 0;
}

u16 get_dig_set_table_size(void)
{
	return (DIG_PARA_SET_TABLE_SIZE);
}

u16 get_dig_read_table_size(void)
{
	return (DIG_PARA_READ_TABLE_SIZE);
}

/*
para:
	begin: begin index
	newindex: the next begin index
	dest: the available adrs need to read,the buffer must big then 4bytes
return:
	0--sus
	-1--err
 */
s8 get_dig_read_table_next_adrs(u16 begin, u16 *newindex, s8 *dest)
{
	u16 cnt, i;

	if ((NULL == dest) || (NULL == newindex))
		return -1;
	if (begin >= DIG_PARA_READ_TABLE_SIZE)
		return -1;
	i = 0;
	for (cnt = begin; cnt < DIG_PARA_READ_TABLE_SIZE; cnt++) {
		if ((dig_para_table_read[cnt].key) && (0xff != (dig_para_table_read[cnt].key))) {
			dest[i] = dig_para_table_read[cnt].key;
			i++;
			if (i > 3) {
				*newindex = cnt;
				break;
			}
		}
	}
	return 0;
}

