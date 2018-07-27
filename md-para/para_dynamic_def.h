/*
name:PARA_DYNAMIC_DEF.h
author:salan
date:20170322
ver:1.0
description:
	module dynamic parameter dat struct
*/
#ifndef PARA_DYNAMIC_DEF_H_
#define PARA_DYNAMIC_DEF_H_

/*****************************include files***********************/
#include "helios.h"
#include "para_def.h"
#include <stdlib.h>


/*****************************para define*************************/
/****************************整机信息*****************************/
/*实时gps信息*/
typedef enum {
	GPS_NONE = 0,	/*没有gps模块*/
	GPS_ONLINE,		/*gps工作正常*/
	GPS_OFFLINE 	/*gps接收不到卫星信号*/
} GPS_STATUS;
typedef struct {
	gps_point gps_cur;	/*当前gps位置*/
	u8 gps_status;	//GPS_STATUS /*当前gps状态*/
} gps;

/*实时gprs信息*/
typedef struct {
	u8 plmn[10];	/*远控状态下的plmn*/
	u8 tech[10];	/*远控状态下的通信制式*/
	u8 band;		/*远控状态下的band*/
	u16 cell;		/*远控状态下的小区id*/
	u16 arfcn;		/*远控状态下的通信频率*/
	f32 rxlev;		/*远控状态下的接收场强*/
	f32 rsq;		/*远控状态下的信号质量*/
} wireless_net;

/*内置锂电池状态*/
typedef enum {
	BAT_NONE = 0,
	BAT_CHARGING,
	BAT_NORMAL
} BAT_STATUS;

typedef enum {
	GPRS_NONE = 0,
	GPRS_2G,
	GPRS_3G,
	GPRS_4G,
	SMS,
	GPRS_OFFLINE
} GPRS_STATUS;
typedef struct {
	wireless_net net;	/*远控状态下的无线网络信息*/
	u8 wlan_ip[25];				/*动态ip地址*/
	u8 status;		//GPRS_STATUS/*当前连接状态*/
} gprs;
/*fan running status*/
typedef enum {
	FAN_STOP = 0,
	FAN_RUNNING
} FAN_STATUS;
typedef struct
{
	u8 fan1;	//FAN_STATUS;
	u8 fan2;
	u8 fan3;
	u8 fan4;
} fan_check;

/*整机动态参数*/
typedef struct {
	u8 time[30];	/*local time*/
	u8 bat_status;	//BAT_STATUS
	gps gps_t;
	gprs gprs_t;
	u8 temperature;		/*整机外壳温度*/
	fan_check fan;		//FAN_STATUS /*风扇状态*/
	unit_alarm alarm;
} unit_dynamic_para;

/*****************************************模块动态信息****************************/
/*信道回读信息*/
typedef union {
	u32 data;
	struct {
		u32 rev: 2;				/*保留*/
		u32 band_max_float: 4;
		u32 band_max_int: 8;	/*信道最大带宽*/
		u32 band_min_float: 4;
		u32 band_min_int: 8;	/*信道最小带宽*/
		u32 index: 4;			/*信道序号*/
		u32 bandsel_type: 1;	/*信道是可变选带还是固定选带*/
		u32 enable: 1;			/*信道是否有效*/
	} FEATURE;
} ch_feature;
typedef struct {
	f32 pin;
	f32 pout;
	u8 pin_op;	/*信道过功率*/
	u8 pin_lp;	/*信道欠功率*/
	u8 agc_att;		/*agc起控深度*/
	u8 gain_compensate;	/*信道增益补偿*/
	u16 traffic;		/*话务量*/
} ch_link_power;
typedef struct {
	f32 workfreq;
	f32 shiftfreq;
} ch_freq;
typedef union
{
	u32 dat;
	struct
	{
		u32 readback: 1;		//已经从fpga内读取信道属性
		u32 swn_finish: 1;		//已经完成扫频
	} status;
} ch_status;
/*信道动态参数*/
typedef struct {
	ch_feature feature[FREQ_CHANNEL_NUMS_MAX];
	ch_link_power ul[FREQ_CHANNEL_NUMS_MAX];
	ch_link_power dl[FREQ_CHANNEL_NUMS_MAX];
	ch_freq freq_ul[FREQ_CHANNEL_NUMS_MAX];
	ch_freq freq_dl[FREQ_CHANNEL_NUMS_MAX];
	ch_status ch_s[FREQ_CHANNEL_NUMS_MAX];
} ch_rf;
/*adi 信道实时校准参数*/
typedef struct {
	f32	freq_rx;	/*adi rx中心频率*/
	f32 freq_tx;	/*adi tx中心频率*/
	u16 freq_rx_int;	/*adi rx中心频率整数部分*/
	u32 freq_rx_frac;	/*adi rx中心频率小数部分*/
	u16 freq_tx_int;	/*adi rx中心频率整数部分*/
	u32 freq_tx_frac;	/*adi rx中心频率小数部分*/
	adi_gain_att gain_att;
	adi_dc_image dc_image;	/*tx adi的镜像与dc*/
	adi_offset_link adi_offset[FREQ_CHANNEL_NUMS_MAX];	/*12信道当前校准值*/
	u8 flag;	/*需要重新做校准标志位*/
} adi_link_regulator_cur;

/*tdd特殊参数*/
typedef struct
{
	u8 sync_st;
} tdd_dynamic_sundry_para;


/*模块基本射频信息*/
typedef struct {
	f32 p;		/*power*/
	u16 max_v;	/*从开机到目前检测到的最大电压值*/
} power_detect_dynamic;
typedef struct
{
	power_detect_dynamic pin;
	power_detect_dynamic pout;
	power_detect_dynamic rpout;
	f32 cur_gain;		/*work gain*/
	f32 rl;				/*return lost*/
} md_dynamic_basic;
/*数字模块动态参数杂项*/
typedef struct
{
	u8 fpga_rf_sw;	/*fpga内部射频开关*/
	f32 gain_equalize;	/*上行温度-增益补偿值，由gain_equalize_rate_ul与温度计算所得*/
	u8 adc_att;			/*输入太强，防止adc饱和而衰减的att值*/
	u8 dig_pa_sw;	//DIG_PA_SW_STATU
} dig_link_dynamic_sundry_para;
typedef enum
{
	DIG_PA_UL_ON_DL_ON = 0,
	DIG_PA_UL_OFF_DL_OFF,
	DIG_PA_UL_OFF_DL_ON
} DIG_PA_SW_STATU;
typedef struct {
	u32 sv;
	u8 ics_iso_dl;			/*ics隔离度*/
	dig_link_dynamic_sundry_para dds_ul;
	dig_link_dynamic_sundry_para dds_dl;
	u8 dig_pa_sw;	//DIG_PA_SW_STATU
} dig_dynamic_sundry_para;

/*work mode*/
typedef enum {
	WORK_NORMAL = 0,	/*正常工作模式*/
	WORK_TEST,			/*工程测试模式*/
	WORK_ADC_DAC_PASS,	/*adc-dac直通模式*/
	WORK_DAC_TEST		/*dac测试模式*/
} WORK_MODE;

/*net status*/
typedef enum
{
	RELAY_NET_OFFLINE = 0,
	RELAY_NET_ONLINE
} RELAY_NET_STATUS;
/*模块动态参数杂项*/
typedef struct
{
	u8 work_mode; 	//WORK_MODE
	u8 reset_para;		/*恢复出厂设置参数*/
	s8 temperature;		/*模块温度*/
	u8 net_status;		/*网络状态 RELAY_NET_STATUS*/
	dig_dynamic_sundry_para dig_dynamic_sundry;
	tdd_dynamic_sundry_para tdd_dynamic_sundry;
} md_dynamic_sundry;

/***************************点前无线网络信息******************************/
/*扫频状态*/
#define MODEM_SUPPORT_TECH_MAX 12
typedef enum
{
	SWN_AUTO_NORMAL = 0,
	SWN_AUTO_SCAN_AGIAN,
	SWN_AUTO_SCAN_DONE,
	SWN_AUTO_FILTER_DONE,
	SWN_AUTO_FINISH,
} SWN_AUTO_STATUS;

typedef enum
{
	SWN_MANUAL_NORMAL = 0,
	SWN_MANUAL_SCAN_START,
	SWN_MANUAL_SCAN_STOP,
	SWN_MANUAL_SETPARA,
	SWN_MANUAL_RESTART,
	SWN_MANUAL_CHECK_ONLINE,
	SWN_MANUAL_READBACK,
	SWN_MANUAL_CHOSECELL,
	SWN_MANUAL_FINISH,
	SWN_MANUAL_FALSE
} SWN_MANNUAL_STATUS;
/*系统制式搜索状态*/
typedef enum {
	MODEM_TECH_SCAN_NORMAL = 0,
	MODEM_TECH_ASCAN_FALSE = 1,
	MODEM_TECH_ASCAN_FINISH = 2,
	MODEM_TECH_MSCAN_FALSE = 3,
	MODEM_TECH_MSCAN_FINISH = 4
} MODEM_TECH_SCAN_STATUS;
/*modem支持的系统制式描述*/
typedef struct {
	MODEM_TECH_SCAN_STATUS scan_status;
	u8 scan_times;
	u8 tech;
} modem_tech;

typedef enum
{
	RELAY_STATUS_OFF = 0,
	RELAY_STATUS_ON
} RELAY_STATUS;

typedef struct
{
	u16 plmn;		/*当前plmn*/
	u8 band;		/*当前band*/
	u8 tech;		/*当前tech*/
	u16 cell;		/*当前主cell*/
	u32 earfcn_dl;		/*当前主earfcn*/
	u32 earfcn_ul;
	u32 bandwidth;		/*带宽*/
	f32 rxlev;		/*当前主接收场强*/
	f32 rsq;		/*当前主信号强度*/
	s16 rssi;
	s16 prtp;		//preambleInitialReceivedTargetPower
	s16 rf_signal_power;	//referenceSignalPower
	s16 path_loss;	//»�ו¶˵ł·̰
	s16 snr;			//хի±Ƭµ¥λ£º 1dB ·¶Χ£º (-40.. 40)
	wireless_info wireless_info_t;	/*主小区，邻小区信息*/
	u16 after_relay[388];		/*relay后临区测量结果*/
	u16 swn_manual_cell;	/*手动模式下需要驻留的小区id*/
	SWN_AUTO_STATUS aflag;		//SWN_AUTO_STATUS
	SWN_MANNUAL_STATUS mflag;	//SWN_MANNUAL_STATUS
	u8 scan_tech;			//待扫频的系统制式 OPERATOR_TECH
	modem_tech modem_tech_a[MODEM_SUPPORT_TECH_MAX];		//modem支持的制式
	RELAY_STATUS rflag;		/*标识relay功能是否已经打开*/
} md_wireless_net;

typedef struct {

	ch_rf ch_rf_t;
	md_dynamic_basic md_dynamic_basic;
	md_dynamic_sundry md_dynamic_sundry;
	adi_link_regulator_cur adi_regulator_cur_ul;	/*上行adi信道实时校准参数*/
	adi_link_regulator_cur adi_regulator_cur_dl;
	md_alarm alarm;
	md_wireless_net md_wireless_net_t;
} band_dynamic_para;

/*****************************data struct define******************/

/*****************************funs********************************/

#endif //PARA_DYNAMIC_DEF_H_
