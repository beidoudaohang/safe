
/*
name:digital.c
author:salan
date:20170331
ver:1.0
description:
	fpga模块与上位机通信模块之间的接口
*/
/*include files*/
#include <string.h> 
#include <math.h>
#include "helios.h"
#include "module_adr_def.h"
#include "para_table.h"
#include "dig_band0.h"
#include "porting.h"
#include "digital.h"
#include "log.h"
#include "common_api.h"
//#include "adi_api.h"
//#include "handle_api.h"
#include "fpgaloader_.h"
/*****************************para define*************************/
static volatile u8 relay_mode_flag[MOD_NUM_IN_ONE_PCB] = {0};

/****************************funs*************************/
void f12tof32(const s16 src, f32 *dest)
{
	f32 destdata;

	if ((NULL == dest))
		return;
	destdata = src;
	destdata = destdata / 16.0;
	*dest = destdata;
}

void f32tof12(const f32 src, s16 *dest)
{
	f32 srcdata;
	s16 destdata;

	if ((NULL == dest))
		return;

	srcdata = src * 16;
	//RLDEBUG("f32tof12:-----f32:%f, \r\n", srcdata);
	destdata = (s16)srcdata;
	//RLDEBUG("f32tof12:-----f12:%d, \r\n", destdata);
	*dest = destdata;
}

/*
return:
	0:sus
	-1:false
*/
s8 dig_comm_crc_make(s8 *dat, u8 len, u16 *crc)
{
	u8 cnt;
	u16 utemp;

	if ((NULL == dat) || (NULL == crc))
		return -1;

	utemp = 0;
	for (cnt = 0; cnt < len; cnt++) {
		utemp += (u8)(dat[cnt]);
	}

	*crc = utemp;

	return 0;
}

s8 set_adr_add(u8 adr, md_adr_info *md_adr)
{
	if (NULL == md_adr)
		return -1;

	if (0 != adr) {
		if (((band_para_a[0].md_adr_t.mod_type) == (md_adr->mod_type)) && ((band_para_a[0].md_adr_t.mod_band) == (md_adr->mod_band)) && ((band_para_a[0].md_adr_t.mod_adr_t.dat) == (md_adr->mod_adr_t.dat))) {
			if (band0_set_adr_add(adr)) {
				RLDEBUG("set_adr_add:band0_set_adr_add(adr) false \r\n");
				return -1;
			}
		}
	} else {
		RLDEBUG("set_adr_add:0 == adr \r\n");
		return -1;
	}

	return 0;
}

s8 trans_adr_add(transmit_arry *arr, md_adr_info *md_adr)
{
	if (NULL == md_adr)
		return -1;

	if (0 != arr->adr) {
		if (((band_para_a[0].md_adr_t.mod_type) == (md_adr->mod_type)) && ((band_para_a[0].md_adr_t.mod_band) == (md_adr->mod_band)) && ((band_para_a[0].md_adr_t.mod_adr_t.dat) == (md_adr->mod_adr_t.dat))) {
			if (band0_transmit_add(arr)) {
				RLDEBUG("trans_adr_add:band0_transmit_add(arr) false \r\n");
				return -1;
			}
		}
	} else {
		RLDEBUG("set_adr_add:0 == adr \r\n");
		return -1;
	}

	return 0;
}

/*
fun:数字版温度增益补偿
para:
	band:指定要补偿的系统
return:
	0---sus
	-1---err
*/
s32 dig_gain_processing(md_adr_info *md_adr)
{
	static s8 stemp = 0;
	f32 temp_gain = 0;
	s32 index;

	if (NULL == md_adr) {
		return -1;
	}

	index = md_adr_to_index(md_adr);
	if (index < 0) {
		RLDEBUG("dig_gain_processing:md_adr_to_index(md_adr) false \r\n");
		return -1;
	}

	if (MOD_TYPE_RELAY == (band_para_a[index].md_adr_t.mod_type)) {
		if (((band_dynamic_para_a[index].md_dynamic_sundry.temperature) > (stemp + 1)) || ((band_dynamic_para_a[index].md_dynamic_sundry.temperature) < (stemp + 1))) {
			//ul
			stemp = (band_dynamic_para_a[index].md_dynamic_sundry.temperature) - 30;
			temp_gain = stemp * (band_para_a[index].md_sundry.dig_sundry.gain_equalize_rate_ul);
			band_dynamic_para_a[index].md_dynamic_sundry.dig_dynamic_sundry.dds_ul.gain_equalize = temp_gain;
			set_adr_add(0x3b, &(band_para_a[index].md_adr_t));
			//dl
			temp_gain = stemp * (band_para_a[index].md_sundry.dig_sundry.gain_equalize_rate_dl);
			band_dynamic_para_a[index].md_dynamic_sundry.dig_dynamic_sundry.dds_dl.gain_equalize = temp_gain;
			set_adr_add(0x99, &(band_para_a[index].md_adr_t));
		}
	}

	return 0;
}
/*
fun:重新设置adi80305校准值
para:
	需要重新校准的模块地址
return:
	0---sus
	-1---err
*/
s32 dig_adi_adjust_update(md_adr_info *md_adr)
{
	s32 index = 0;
	u32 cnt = 0;
	u8 adr = 0;

	if (NULL == md_adr) {
		return -1;
	}

	index = md_adr_to_index(md_adr);
	if (index < 0)
		return -1;


	if (MOD_TYPE_RELAY == band_para_a[index].md_adr_t.mod_type) {
		//ul flat
		adr = 0x3c;
		for (cnt = 0; cnt < (FREQ_CHANNEL_NUMS_MAX / 2); cnt++) {
			set_adr_add(adr + cnt, &(band_para_a[index].md_adr_t));
		}
		//ul pin&pout
		adr = 0x42;
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			set_adr_add(adr + cnt, &(band_para_a[index].md_adr_t));
		}
		//dl flat
		adr = 0x9a;
		for (cnt = 0; cnt < (FREQ_CHANNEL_NUMS_MAX / 2); cnt++) {
			set_adr_add(adr + cnt, &(band_para_a[index].md_adr_t));
		}
		//dl pin&pout
		adr = 0xa0;
		for (cnt = 0; cnt < FREQ_CHANNEL_NUMS_MAX; cnt++) {
			set_adr_add(adr + cnt, &(band_para_a[index].md_adr_t));
		}

		//other ad80305 para
		adr = 0xd3;
		for (cnt = 0xd3; cnt <= 0xd4; cnt++) {
			set_adr_add(cnt, &(band_para_a[index].md_adr_t));
		}
		adr = 0xdd;
		for (cnt = 0xdd; cnt <= 0xe0; cnt++) {
			set_adr_add(cnt, &(band_para_a[index].md_adr_t));
		}
	} else {
		return -1;
	}

	return 0;
}

/*
fun:重新设置fpga中心频率和adi80305校准值
para:
	需要重新校准的模块地址
return:
	0---sus
	-1---err
*/
s32 dig_center_freq_update(md_adr_info *md_adr)
{
	s32 err;
	s32 index = 0;
	u32 cnt = 0;
	u8 adr = 0;

	if (NULL == md_adr) {
		return -1;
	}

	#if 1
	index = md_adr_to_index(md_adr);
	if (index < 0) {
		RLDEBUG("dig_center_freq_update:md_adr_to_index() false \r\n");
		return -1;
	}

	if (MOD_TYPE_BROADBAND == band_para_a[index].md_adr_t.mod_type) {
		//cal center freq int&float
/* 		err = adi_center_freq_cal(&(band_para_a[index].md_sundry.dig_sundry.center_freq.ul), \
		                          & (band_dynamic_para_a[index].adi_regulator_cur_ul));
		if (err < 0) {
			RLDEBUG("dig_center_freq_update:ul adi_center_freq_cal() false \r\n");
			return -1;
		}

		band_dynamic_para_a[index].adi_regulator_cur_ul.freq_rx = band_para_a[index].md_sundry.dig_sundry.center_freq.ul;
		band_dynamic_para_a[index].adi_regulator_cur_ul.freq_tx = band_para_a[index].md_sundry.dig_sundry.center_freq.ul;

		err = adi_center_freq_cal(&(band_para_a[index].md_sundry.dig_sundry.center_freq.dl), \
		                          & (band_dynamic_para_a[index].adi_regulator_cur_dl));
		if (err < 0) {
			RLDEBUG("dig_center_freq_update:ul adi_center_freq_cal() false \r\n");
			return -1;
		}

		band_dynamic_para_a[index].adi_regulator_cur_dl.freq_rx = band_para_a[index].md_sundry.dig_sundry.center_freq.dl;
		band_dynamic_para_a[index].adi_regulator_cur_dl.freq_tx = band_para_a[index].md_sundry.dig_sundry.center_freq.dl;

		//center freq int&float
		adr = 0xd5;
		for (cnt = 0xd5; cnt <= 0xdc; cnt++) {
			set_adr_add(cnt, &(band_para_a[index].md_adr_t));
		}
		adr = 0xe1;
		for (cnt = 0xe1; cnt <= 0xe4; cnt++) {
			set_adr_add(cnt, &(band_para_a[index].md_adr_t));
		}
		adi_adjust_once(md_adr);
		dig_adi_adjust_update(md_adr); */

		
	} else {
		RLDEBUG("dig_center_freq_update: mod_type!=MOD_TYPE_BROADBAND \r\n");
		return -1;
	}
	#endif

	return 0;
}

/*
fun:重新初始化数字版参数
return:
	0---sus
	-1---err,more then one system init false
*/
s32 dig_channel_freq_update(md_adr_info *md_adr)
{
	s32 err = 0;
	s32 index;
	s32 cnt;

	if (NULL == md_adr) {
		return -1;
	}
	index = md_adr_to_index(md_adr);
	if (index < 0)
		return -1;

	//ul workfreq
	for (cnt = 0x01; cnt <= 0x0c; cnt++) {
		if (set_adr_add(cnt, &(band_para_a[index].md_adr_t))) {
			err = -1;
			break;
		}
	}
	//ul shiftfreq
	for (cnt = 0x0d; cnt <= 0x18; cnt++) {
		if (set_adr_add(cnt, &(band_para_a[index].md_adr_t))) {
			err = -1;
			break;
		}
	}
	//dl workfreq
	for (cnt = 0x68; cnt <= 0x73; cnt++) {
		if (set_adr_add(cnt, &(band_para_a[index].md_adr_t))) {
			err = -1;
			break;
		}
	}
	//dl shiftfreq
	for (cnt = 0x74; cnt <= 0x7f; cnt++) {
		if (set_adr_add(cnt, &(band_para_a[index].md_adr_t))) {
			err = -1;
			break;
		}
	}

	//bandwidth
	for (cnt = 0xe8; cnt <= 0xed; cnt++) {
		if (set_adr_add(cnt, &(band_para_a[index].md_adr_t))) {
			err = -1;
			break;
		}
	}

	return err;
}
/*
fun:重新初始化数字版参数
return:
	0---sus
	-1---err,more then one system init false
*/
s32 dig_para_set_again(void)
{
	u32 mod_index = 0;
	u32 cnt = 0;
	s32 err = 0;
	transmit_arry arry;

	#if 0
	for (mod_index = 0; mod_index < MOD_NUM_IN_ONE_PCB; mod_index++) {
		memset(&arry, 0, sizeof(arry));
		//adi adjust
		adi_adjust_once(&(band_para_a[mod_index].md_adr_t));
		dig_adi_adjust_update(&(band_para_a[mod_index].md_adr_t));

		//adi center freq
		dig_center_freq_update(&(band_para_a[mod_index].md_adr_t));

		/*****************************ul para************************/
		//ul workfreq
		for (cnt = 0x01; cnt <= 0x0c; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//ul shiftfreq
		for (cnt = 0x0d; cnt <= 0x18; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//ul pin op th & lp th
		for (cnt = 0x19; cnt <= 0x24; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//ul mute sw
		if (set_adr_add(25, &(band_para_a[mod_index].md_adr_t))) {
			err = -1;
			break;
		}
		//ul agc th
		for (cnt = 0x26; cnt <= 0x2b; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//traffic th
		for (cnt = 0x2c; cnt <= 0x2e; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//ch att
		for (cnt = 0x2f; cnt <= 0x34; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//ul mute high th,low th
		for (cnt = 0x35; cnt <= 0x3a; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//ul gain equalize rate
		if (set_adr_add(0x3b, &(band_para_a[mod_index].md_adr_t))) {
			err = -1;
			break;
		}
		/*****************************ul para************************/
		//dl workfreq
		for (cnt = 0x68; cnt <= 0x73; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//dl shiftfreq
		for (cnt = 0x74; cnt <= 0x7f; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//dl pin op th & lp th
		for (cnt = 0x80; cnt <= 0x8b; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//ch sw& ics sw
		if (set_adr_add(0x8c, &(band_para_a[mod_index].md_adr_t))) {
			err = -1;
			break;
		}
		//dl agc th
		for (cnt = 0x8d; cnt <= 0x92; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//dl ch att
		for (cnt = 0x93; cnt <= 0x98; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//dl equalize rate
		if (set_adr_add(0x99, &(band_para_a[mod_index].md_adr_t))) {
			err = -1;
			break;
		}

		//relay sw
		if (set_adr_add(0xd0, &(band_para_a[mod_index].md_adr_t))) {
			err = -1;
			break;
		}
		//donor or remote
		if (set_adr_add(0xd1, &(band_para_a[mod_index].md_adr_t))) {
			err = -1;
			break;
		}
		//test mode
		if (set_adr_add(0xd2, &(band_para_a[mod_index].md_adr_t))) {
			err = -1;
			break;
		}
		//ad80305
		for (cnt = 0xd3; cnt <= 0xe4; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}
		//ul&dl mix th
		for (cnt = 0xe5; cnt <= 0xe7; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}

		//ch bandwidth set
		for (cnt = 0xe8; cnt <= 0xed; cnt++) {
			if (set_adr_add(cnt, &(band_para_a[mod_index].md_adr_t))) {
				err = -1;
				break;
			}
		}

		//dlul att
		set_adr_add(0xee, &(band_para_a[mod_index].md_adr_t));

		//all para set
		arry.adr = 0xff;
		arry.DAT.dat32 = 0xaaaaaaaaul;
		if (trans_adr_add(&arry, &(band_para_a[mod_index].md_adr_t))) {
			err = -1;
			break;
		}

	}
	#endif

	return err;
}

/*
fun:寻找指定制式的信道序号
para：
	md_adr：要加载的模块地址
	tech：指定的制式
return：
	>0---sus
	-1---err
 */
s32 dig_tech_to_channel_index(md_adr_info* md_adr, u8 tech)
{
	s32 err;
	s32 index;

	if (NULL == md_adr)
		return -1;
	index = md_adr_to_index(md_adr);
	if (index < 0)
		return -1;

	for (err = 0; err < FREQ_CHANNEL_NUMS_MAX; err++)
	{
		if ((tech == band_para_a[index].ch_info_t.tech[err]) && (0 != tech))
			return err;
	}

	return -1;
}

/*
fun:fpga加载
para：
	md_adr：要加载的模块地址
return：
	0---sus
	-1---err
 */
#define FPGA_LOAD_TRY_TIMES	10
#define FPGA_PSS_PATH "/system/bin/X2Relay_PSS_Link1_1TRx_B3_A5_V1.0.1.rbf"
#define FPGA_CRS_PATH "/system/bin/X2Relay_CRS_Link1_1TRx_B3_A5_V1.0.1.rbf"

s32 dig_fpga_loader(md_adr_info *md_adr)
{
	s32 cnt;
	s32 err;
	s32 mod_index;

	mod_index = md_adr_to_index(md_adr);
	if (mod_index < 0)
		return -1;

	//check is digital modules?
	if (MOD_TYPE_RELAY != band_para_a[mod_index].md_adr_t.mod_type) {
		//continue;
		;
	}

	//band0
	//loader fpga
	if ((RELAY_PSS == band_para_a[mod_index].md_sundry.relay_mode) && (0 == mod_index)) {
		RLDEBUG("dig_fpga_loader:PSS loader start \r\n");
		for (cnt = 0; cnt < FPGA_LOAD_TRY_TIMES; cnt++) {
			mod_index = 0;
			err = fpgaloader_start_fpga_load(FPGA_PSS_PATH);
			if (err < 0) {
				continue;
			} else {
BAND0_PSS_FPGA_LOAD_WAIT:
				if (mod_index > 3) {
					continue;
				}
				timedelay(0, 5, 0, 0);
				err = fpgaloader_get_fpga_load_finish();
				if (0 == err) {
					continue;
				} else if (1 == err) {
					mod_index++;
					goto BAND0_PSS_FPGA_LOAD_WAIT;
				} else if (2 == err) {
					continue;
				} else if (3 == err) {
					RLDEBUG("dig_fpga_loader:PSS loader sus \r\n");
					break;
				} else {
					continue;
				}
			}
		}

		if (cnt >= FPGA_LOAD_TRY_TIMES) {
			band_dynamic_para_a[0].alarm.fpga_loaded = 1;
			return -1;
		} else {
			band_dynamic_para_a[0].alarm.fpga_loaded = 0;
		}
	} else if ((RELAY_CRS == (band_para_a[mod_index].md_sundry.relay_mode)) && (0 == mod_index)) {
		RLDEBUG("dig_fpga_loader:CRS loader start \r\n");
		for (cnt = 0; cnt < FPGA_LOAD_TRY_TIMES; cnt++) {
			mod_index = 0;
			err = fpgaloader_start_fpga_load(FPGA_CRS_PATH);
			if (err < 0) {
				continue;
			} else {
BAND0_CRS_FPGA_LOAD_WAIT:
				if (mod_index > 3) {
					continue;
				}
				timedelay(0, 5, 0, 0);
				err = fpgaloader_get_fpga_load_finish();
				if (0 == err) {
					continue;
				} else if (1 == err) {
					mod_index++;
					goto BAND0_CRS_FPGA_LOAD_WAIT;
				} else if (2 == err) {
					continue;
				} else if (3 == err) {
					RLDEBUG("dig_fpga_loader:CRS loader sus \r\n");
					break;
				} else {
					continue;
				}
			}
		}

		if (cnt >= FPGA_LOAD_TRY_TIMES) {
			band_dynamic_para_a[0].alarm.fpga_loaded = 1;
			return -1;
		} else {
			band_dynamic_para_a[0].alarm.fpga_loaded = 0;
		}
	} else {
		RLDEBUG("dig_fpga_loader:err case: mod_index=%d,relay_mode=%d \r\n", mod_index, band_para_a[mod_index].md_sundry.relay_mode);
	}

	return 0;
}

/*
fun:relay模式切换
para：
	md_adr：要加载的模块地址
return：
	0---sus
	-1---err
 */
s32 dig_relay_mode_change(md_adr_info *md_adr)
{
	s32 err;
	s32 mode_index;

	mode_index = md_adr_to_index(md_adr);
	if (mode_index < 0) {
		return -1;
	}

	relay_mode_flag[mode_index] = 1;

	RLDEBUG("dig_relay_mode_change:run \r\n" );

	return 0;
}

/*
fun:relay模式切换
para：
	md_adr：要加载的模块地址
return：
	1---need restart dig task
	0---no changed
	-1---err
 */
s32 dig_relay_mode_change_check(md_adr_info *md_adr)
{
	s32 err;
	s32 mode_index;

	#if 0
	mode_index = md_adr_to_index(md_adr);
	if (mode_index < 0)
		return -1;

	if (relay_mode_flag[mode_index]) {
		relay_mode_flag[mode_index] = 0;
		set_relay_mode();
		return 1;
	}
	#endif

	return 0;
}