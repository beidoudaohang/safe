/*
name:para_def.h
author:luke
date:20170322
ver:1.0
description:
	paras
*/
#ifndef PARA_DEF_H_
#define PARA_DEF_H_

/*****************************include files***********************/
#include "helios.h"
#include "para_table_def.h"
#include "module_adr_def.h"
#include "usr_def.h"


/*****************************para define*************************/
#define ADI_OFFSET_TABLE_SIZE 20
#define FREQ_CHANNEL_NUMS_MAX 12
#define BANDS_IN_ONE_MONITOR 12	/*监控板最大能管理的band数*/

/*****************************整机参数*****************************/
/*unit manufacturer info*/
typedef struct
{
	u8 sn[15];			/*整机序列号*/
	u8 type;			/*整机设备类型*/
	u8 model_no[20];	/*整机类型编号*/
	u8 part_no[20];		/*整机部件编号*/
} unit_mfrs_info;
/*mannufactor info*/
typedef struct {
	u8 factory[40];	/*厂家名称*/
	u8 model_no[20];	/*设备类型编号*/
	u8 part_no[20];	/*设备部件编号*/
	u8 sn[15];	/*模块序列号*/
	u8 hv[20];	/*硬件版本号*/
	u8 sv[20];	/*软件版本号*/
	u8 para_table_ver[10];	/*参数列表版本*/
	u8 para_table_date[15];	/*参数列表最后修改日期*/
} md_mfrs_info;

/*site info*/
typedef struct {
	u8 name[50];	/*站点名称*/
	u8 addr[100];	/*站点地址*/
	u8 site_no[20];	/*站点编号*/
	u8 proprietor[20];	/*站点权属*/
	u8 asset[20];	/*站点资产编号*/
	u8 manager[20];	/*站点管理者*/
	u8 creat_time[30];	/*建站时间*/
	u8 sim[20];			/*该站点所配sim卡号码*/
} site_info;
/*gps info*/
typedef struct {
	f64	Longitude;	     /*经度*/
	//u8	Lo_hemisphere;	/*经度半球   E=东经； W=西经     */
	f64	Latitude;	     /*纬度*/
	//u8	La_hemisphere;  /*纬度半球   N-北半球 S-南半球	 */
	f32 altitude;	/*gps 高度*/
} gps_point;
typedef struct {
	gps_point left_top;	/*围栏左顶点*/
	gps_point right_bottom;	/*围栏右顶点*/
} gps_pen;
/*remote server info*/
typedef struct {
	u8 ip[25];	/*nms ip*/
	u16 port;	/*nms port*/
	u8 sim[20];	/*nms sim 卡号*/
	u8 alarm_sim[20];	/*告警上报卡号*/
} nms_info;
/*gprs net info*/
typedef struct {
	u8 apn[50];	/*gprs apn名称*/
	u8 usr[20];	/*gprs apn用户名*/
	u8 pw[20];	/*gprs gpn密码*/
} gprs_net_info;
/*gprs info*/
typedef struct {
	gprs_net_info net_info;
	u8 timeout;	//second
} gprs_info;
/*remote monitor mode*/
typedef enum
{
	RCOMM_GPRS = 0,
	RCOMM_SMS,
	RCOMM_MIX
} MONITOR_MODEL;
/*远控信息配置*/
typedef struct {
	nms_info nms;		/*nms服务器信息*/
	gprs_info gprs;		/*链接远控gprs网络配置信息*/
	u8 monitor_mode;	//MONITOR_MODEL /*远控方式*/
} monitor_cfg;
/*外部告警1-6条件*/
typedef struct {
	u8 ext_1;	/*外部告警判定规则*/
	u8 ext_2;
	u8 ext_3;
	u8 ext_4;
	u8 ext_5;
	u8 ext_6;
} ext_alarm_condition;

/*fan control*/
typedef struct {
	s8 fan_temp_h;	/*开风扇温度门限*/
	s8 fan_temp_l;	/*关风扇温度门限*/
} fan_ctrl;

/*battery info*/
typedef enum {
	BAT_IN_MANUAL = 0,
	BAT_IN_AUTO
} BAT_IN_CTRL_MODE;
typedef struct {
	u8 ctrl_mode;
	u8 cut_out_time;
} bat_in;

/*整机告警信息*/
typedef struct {
	u8 door_open;	/*门开*/
	u8 power_ac;	/*ac掉电*/
	u8 power_dc;	/*dc掉电*/
	u8 movement;	/*站点被移动*/
	u8 gps_offline;	/*gps离线*/
	u8 bat_low_volt;	/*内置电池低电压*/

	//fan
	u8 fan_alarm_1;	/*风扇告警*/
	u8 fan_alarm_2;
	u8 fan_alarm_3;
	u8 fan_alarm_4;
	//extern alarm
	u8 ext_alarm_1;	/*外部告警*/
	u8 ext_alarm_2;
	u8 ext_alarm_3;
	u8 ext_alarm_4;
	u8 ext_alarm_5;
	u8 ext_alarm_6;
} unit_alarm;

typedef struct {
	ext_alarm_condition ext_condition;
	s8 case_temp_regulator;		/*外壳温度校准*/
	u16 device_id;	/*设备编号，用来标识近远端机*/
	u8 prj_code[15];	/*项目编码*/
} unit_sundry_para;

/******************************整机单band系统信息**************************/
/*ms bts passive device offset*/
typedef struct {
	s8 ms;		/*ms端无缘补偿*/
	s8 bts;		/*bts端无缘补偿*/
} passive_offset;
/*band restrict info*/
typedef struct {
	s8 max_gain;		/*最大增益*/
	s8 max_pout;		/*最大输出功率*/
	f32 max_freq;	/*最大输出频率*/
	f32 min_freq;	/*最小输出频率*/
} band_restrict;


typedef struct
{
	passive_offset passive_offset[BANDS_IN_ONE_MONITOR];
	band_restrict band_restrict_ul[BANDS_IN_ONE_MONITOR];
	band_restrict band_restrict_dl[BANDS_IN_ONE_MONITOR];
	u8 band[BANDS_IN_ONE_MONITOR];	/*该band支持的具体band值*/
} band_whole_para;


typedef struct {
	site_info site;
	gps_pen gps_pen_t;
	monitor_cfg monitor_cfg_t;
	bat_in bat_in_t;
	fan_ctrl fan_ctrl_t;
	unit_mfrs_info u_mfrs;
	gps_point gps_point_t;
	unit_sundry_para unit_sundry;
	unit_alarm alarm_sw;
	band_whole_para band_whole;
	md_adr_info md_adr_t;
	md_mfrs_info md_mfrs;
	u8 oldsys_band_table[8];
} unit_para;

/******************************模块信息*********************************/
#define SUPORT_PLMN_MAX_NUMS 		250
/*operator info*/
typedef enum {
	TECH_GSM = 1,
	TECH_WCDMA,
	TECH_TDSCDMA,
	TECH_FDDLTE,
	TECH_TDDLTE,
	TECH_MAX
} OPERATOR_TECH;

/***************************支持的plmn列表********************************/
typedef union {
	u8 u_u8;
	struct {
		u8 factory_en: 1;
		u8 operator_en: 1;
	} PLMN_EN;
} plmn_enable;
typedef struct {
	u16 plmn[SUPORT_PLMN_MAX_NUMS];	/*运营商代码*/
	plmn_enable enable[SUPORT_PLMN_MAX_NUMS];	/*运营商代码使能*/
} plmn_para;
/*****************************主小区，邻小区信息************************/
#define SER_CELL_LEN 	40
#define NEI_CELL_LEN	388
typedef struct
{
	u16 ser_cell[SER_CELL_LEN];		//主小区
	u16 nei_cell[NEI_CELL_LEN];		//邻小区
} wireless_info;

/**************************配置的网络参数*******************************/
typedef struct {
	plmn_para plmn_t;	/*运营商代码*/
	u8 band;	/*band代码*/
	//u8 cell;	/*手动模式下需要驻留的小区id*/
	//u8 enable;		/*是否使能plmn配置*/
	wireless_info wireless_info_t[TECH_MAX];	/*每个制式下最后一次正常搜网*/
} telecom_operator;


/*temperature alarm th*/
typedef struct {
	s8 temp_regulator;		/*模块温度校准*/
	s8 temp_over_th;		/*高温告警门限*/
	s8 temp_lower_th;		/*低温告警门限*/
	s8 temp_over_clean;		/*高温告警清除门限*/
	s8 temp_lower_clean;	/*低温告警清除门限*/
	u8 temp_rf_sw;			/*高低温是否关闭射频标识*/
} temp_ctrl;


/*chanel data*/
typedef struct {
	f32 workfreq;	/*工作频率*/
	f32 shiftfreq;	/*移频频率*/
	u8 att;			/*信道att*/
	s8 pin_op_th;		/*输入过功率门限*/
	s8 pin_lp_th;		/*输入欠功率门限*/
	u8 mute_sw;			/*静噪开关*/
	s8 mute_th_h;		/*静噪高门限*/
	s8 mute_th_l;		/*静噪低门限*/
	s8 agc_th;			/*agc门限*/
	s8 traffic_th;		/*话务量判别门限*/
	u8 sw;				/*信道开关*/
} ch_link;
typedef struct {
	u8 remark[10];
} ch_remark;
typedef struct {
	u8 uldl_mix_th[FREQ_CHANNEL_NUMS_MAX];		/*上下行功率控制联动最大值*/
	ch_link ul[FREQ_CHANNEL_NUMS_MAX];
	ch_link dl[FREQ_CHANNEL_NUMS_MAX];
	u8 tech[FREQ_CHANNEL_NUMS_MAX];	//OPERATOR_TECH;/*制式代码*/
	ch_remark ch_remark[FREQ_CHANNEL_NUMS_MAX];	/*信道备注*/
	f32 bandwidth[FREQ_CHANNEL_NUMS_MAX];	/*channel bandwidth*/
} ch_info;

/*ad80305 校准参数*/
/*
上行链路：ad80305-1_rx+AD80305-2_TX
下行链路：ad80305-2_rx+AD80305-1_TX
 */
typedef struct {
	f32 freq;	/*校准频点*/
	f32 flat;	/*增益校准*/
	f32 pin;	/*输入读数校准*/
	f32 pout;	/*输出读数校准*/
} adi_offset_link;
#pragma pack(1) //@ccTagOK
typedef struct {
	s8 phase;	/*相位*/
	s8 amp;		/*幅度*/
	s8 dc_i;	/*dc-i*/
	s8 dc_q;	/*dc-q*/
} adi_dc_image;
#pragma pack()
typedef struct {
	u8	rx_gain;	/*校准频点对应的rx增益*/
	u8 	tx_att;		/*校准频点对应的tx att*/
} adi_gain_att;
typedef struct {
	f32	freq_rx;	/*adi rx中心频率*/
	f32 freq_tx;	/*adi tx中心频率*/
	adi_gain_att gain_att;
	adi_dc_image dc_image;		/*tx adi的镜像与dc*/
	adi_offset_link adi_offset[ADI_OFFSET_TABLE_SIZE];	/*20格校准表格*/
} adi_link_regulator;
typedef struct {
	adi_link_regulator ul;		/*上线链路校准值*/
	adi_link_regulator dl;		/*下行链路校准值*/
} adi_para;

/*数字模块参数杂项*/
typedef enum
{
	RELAY_CELL_RESEL_DISABLE = 0,
	RELAY_CELL_RESEL_ENABLE
} RELAY_CELL_RESEL;
typedef enum
{
	RELAY_SW_OFF = 0,
	RELAY_SW_ON
} RELAY_SW;

typedef struct {
	u8 lna_att;		/*数字版lna 硬件att*/
	u8 pa_att;		/*数字版pa 硬件att*/
} dig_att;
typedef struct
{
	f32 ul;
	f32 dl;
} dig_center_freq;
typedef struct {
	u8 ics_sw;				/*ics开关*/
	u8 relay_sw;			/*干扰消除开关*/	//RELAY_SW
	u8 cell_resel_sw;		/*小区重选开关*/	//RELAY_CELL_RESEL
	f32 gain_equalize_rate_ul;	/*上行增益温度校准斜率*/
	f32 gain_equalize_rate_dl;
	u8 donor_remote;	/*近远端标识*/
	u8 iso_alarm_th;	/*隔离度告警门限*/
	s8 shutdown_ul;		/*上行关闭门限（以下行输入功率为准）*/
	dig_att att_ul;
	dig_att att_dl;
	dig_center_freq center_freq;
} dig_sundry_para;

/*模块基本射频信息*/
/*功率检测数据结构定义*/
#define POWER_DETECT_TABLE_SIZE 40
typedef struct {
	s8 p;	/*power*/
	u16 v;	/*volt*/
} power_table;
typedef struct {
	power_table table[POWER_DETECT_TABLE_SIZE];		/*功率检测建表*/
	f32 p_th;		/*过功率门限*/
} power_detect;

typedef struct
{
	u8 max_gain;		/*模块最大增益*/
	u8 sw;				/*模块开关*/
	u8 att;				/*模块衰减*/
	power_detect pin;
	power_detect pout;
	power_detect rpout;
	temp_ctrl temp_ctrl;
} md_basic_para;
/*模块参数杂项*/
/*pa unique para*/
typedef struct
{
	f32 rl_th;			/*回波损耗告警门限*/
	u8 vgs_pa1;			/*功放管1初始栅压*/
	f32 vgs_rate_pa1;	/*功放管1栅压补偿斜率*/
	u8 vgs_pa2;			/*功放管2初始栅压*/
	f32 vgs_rate_pa2;	/*功放管2栅压补偿斜率*/
	u16 vgs_h_th_pa1;	/*功放管1栅压告警高门限*/
	u16 vgs_l_th_pa1;	/*功放管1栅压告警低门限*/
	u16 vgs_h_th_pa2;
	u16 vgs_l_th_pa2;
} pa_unique_para;
/*tdd unique para*/
typedef enum {
	UL_DL_OFF = 0,
	UL_ON,
	DL_ON,
	UL_DL_ON,
} TDD_SW;
typedef enum {
	TDD_WK_AUTO = 0,
	TDD_WK_MANUAL
} TDD_WORK_MODE;
typedef struct
{
	u8 major;	/*主同步码*/
	u8 minor;	/*辅同步码*/
	u8 dl_ul_config;	/*上下行配置*/
	u8 cp;				/*cp配置*/
	u8 sf_config;		/*特殊指针配置*/
} tdd_work_para;
typedef struct {
	u8 sw;						//TDD_SW;
	u8 wk_mode;					//TDD_WORK_MODE;
	tdd_work_para work_para;	/*tdd手动同步配置参数*/
	s8 sync_th;					/*同步门限*/
	u8 sync_keep;				/*同步保持时间*/
	s16 sync_offset;			/*帧同步校准*/
	u32 guard_time;				/*上下行开关保护时间*/
} td_unique_para;

/*agc control mode*/
typedef enum {
	AGC_TARGET_POWER = 0,	/*目标功率控制*/
	AGC_MANUAL,				/*增益手动控制*/
	AGC_ULDL_MIX,			/*上下行增益联动*/
	AGC_BLOCK_ADJ,			/*阻塞补偿*/
	AGC_RS					/*目标rs功率控制*/
} AGC_MODE;

/*relay mode*/
typedef enum
{
	RELAY_PSS = 1,
	RELAY_CRS,
} REALY_MODE;

typedef struct
{
	u8 agc_mod;			//AGC_MODE /*功率控制模式*/
	u8 relay_mode;		//REALY_MODE
	pa_unique_para pa_unique;
	td_unique_para td_unique;
	dig_sundry_para dig_sundry;
	u8 mcu_fw_name[100];
	u8 fpga_fw_name[100];
} md_sundry_para;

/*模块告警参数*/
typedef struct {
	u8 rl;	/*reverse loss alarm*/
	u8 pout;	/*power outpot alarm*/
	u8 pout_pre;	/*power output pre alarm*/
	u8 temp_h;		/*over temperature*/
	u8 temp_l;		/*lower temperature*/
	u8 pa1;			/*pa1 alarm*/
	u8 pa2;			/*pa2 alarm*/
	u8 tdd_sync_lost;	/*tdd失步告警*/
	u8 ad80305_1;
	u8 ad80305_2;
	u8 ch_pin_ul_op[FREQ_CHANNEL_NUMS_MAX];		/*上行信道输入过功率*/
	u8 ch_pin_ul_lp[FREQ_CHANNEL_NUMS_MAX];		/*上行信道输入欠功率*/
	u8 ch_pin_dl_op[FREQ_CHANNEL_NUMS_MAX];
	u8 ch_pin_dl_lp[FREQ_CHANNEL_NUMS_MAX];
	u8 fpga_loaded;		/*fpga加载失败*/
	u8 fpga_uart;		/*fpga uart通信失败*/
	u8 fpga_spi;		/*fpga spi通信失败*/
	u8 iso_alarm;		/*隔离度告警*/
	u8 cp_init;			/*cp init false alarm*/
	u8 ap_init;			/*ap init false alarm*/
} md_alarm;

#define ADI_ADJUST_TABLE_SIZE 2	/*ad80305У?μ???μμ??*/


typedef struct
{
	s8 after_relay_rsrp_revise;		/*relay后rsrp修正值*/
	//s8 recv;
} md_augment_para;
typedef struct {
	md_adr_info md_adr_t;
	md_mfrs_info md_mfrs;
	md_basic_para md_basic;
	md_sundry_para md_sundry;
	md_alarm alarm_sw;
	ch_info ch_info_t;
	adi_para adi_para_t[ADI_ADJUST_TABLE_SIZE];
	telecom_operator tel_operator;
	md_augment_para md_augment;
} band_para;

/*****************************特殊参数****************************/
/*local net info*/
typedef struct {
	u8 ip[25];
	u8 mask[25];
	u8 gateway[25];
} net_info;
typedef struct
{
	usr_group usr;
	net_info net;
} pcb_share_para;
/*****************************data struct define******************/

/*****************************funs********************************/

#endif //PARA_DEF_H_
