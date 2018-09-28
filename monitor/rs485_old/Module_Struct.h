
#ifndef  __MODULE_STRUCT_H__
#define  __MODULE_STRUCT_H__

#include "helios.h"

#define READ_SN_ENABLE 1
#define BANDSW_ENABLE 0
/*
*RS485通讯协议结构
*/
#define   RS485CMDDATALEN   150        /*RS485命令数据长度*/
#define   MAXRSBUFLEN       (RS485CMDDATALEN)+20     /*RS485发送,接收缓冲区长度*/
#define   ICSMAXCHNUM           24          /*京奥公司ICS模块最大信道数*/

/*自动AGC选择*/
typedef enum
{
 ModelComplete  = 0x00,  /*模块本身完成*/
 JKComplete,  	         /*监控板实现*/
 DLLNAComplete			 /*下行底噪计算*/
}AUTO_AGC_TYPE;

typedef struct {
	u8 Func;                /*模块的功能编码*/
	u8 Addr;                /*模块的地址编码*/
	u8 Cmd;                 /*命令编号*/
	u8 Ack;                 /*应答标志*/
	u8 Clen;                /*命令体长度*/
	u8 Data[RS485CMDDATALEN];  /*命令数据*/
} Rs485Comm;

typedef struct {
	u8    Head;             /*起始标志单元*/
	Rs485Comm Command;   /*命令单元*/
	u16   CheckSum;         /*校验单元*/
	u8    End;              /*结束标志单元*/
} HT485;                  /*京奥公司485协议结构*/

#define SOFD             0x7E      /*起始标志单元*/
#define EOM              0x7F      /*结束标志单元*/
/*
*模块的功能编码
*/
#define DET_FUNC         0x01      /*DET模块*/
//#define ATT_FUNC         0x02      /*ATT模块*/
#define BAND1_FUNC       0x03      /*1选频模块*/
#define BAND2_FUNC       0x04      /*2选频模块*/
#define BAND4_FUNC       0x05      /*4选频模块*/
#define XFP_FUNC         0x06      /*光盘模块*/
#define PA_FUNC          0x08      /*功放模块*/
#define MPA_FUNC         0x11      /*微放模块*/
#define LNA_FUNC         0x09      /*LNA模块*/
#define PUSH_FUNC        0x12      /*推动模块*/
#define DIG_PICO         0x13      /*数字PICO模块*/
#define ICS_FUNC         0x14      /*京奥公司ICS模块*/
#define ODU_FUNC         0x19      /*ODU模块*/
#define RF_ALARM_FUNC 0x16      /*告警指示模块*/

/*
*模块的命令编码
*/
#define QUERY             0x11          //查询模块状态
#define SETATT            0x20		      //设置衰减值
#define SETFREQ           0x21          //设置频率
#define SETSW             0x22		      //设置功放开关
#define SETALC            0x23		      //设置功放ALC开关
#define SETGAIN           0x45          //设置最大增益
#define SETGA             0x4a          //设置过功率告警门限值
#define GETOVERPT         0x4b          //查询过功率门限值
#define SETAGCGA          0xf1		      //设置查询AGC门限值
#define SETUNDERPT        0xf4          //查询欠功率门限值
#define GETUNDERPT        0xf5          //设置欠功率门限值
#define SETAGCSW          0xe0          //自动AGC开关
#define GETIP		      0xe3          //获取输入功率
#define GETOP		      0xe5          //获取输出功率
#define SETCH             0x21          //设置信道

#define SETMAXGAIN		  0xc4            //京奥公司设置查询最大增益
#define HESETAGCSW        0xcc		      //主控板AGC状态
#define HESETGAGAIN       0xcd          //设置整机AGC门限值和最大增益
#define HEGETTOTALATT     0xce		      //自动AGC时获取整机ATT

#define SETICS            0xc0          //设置ICS参数,此命令与485协议不同

#define SETSN             0xee          //设置读取模块系列号
/*
*模块的地址编码
*/
#define UL   0x08
#define DL   0x00
#define SYS_ADDR_BASE    0x00    /*模块起始地址*/
#define SYS_ADDR_SKIP    0x01    /*各系统地址间隔*/

#if OLD_SEL
#define SEL_ADDR_SKIP    0x01   /*各选频系统地址间隔*/
#else
#define SEL_ADDR_SKIP    0x10
#endif

#define SubMod1 0x00
#define SubMod2 0x01
#define SubMod3 0x02
#define SubMod4 0x03


//extern u8 PICO_ENABLE[SYS_NUM];

/*
*命令编号
*/
//#define QUERY             0x11         /*查询模块状态*/
//#define SETATT            0x20		     /*设置衰减值*/
//#define SETSW             0x22		     /*设置功放开关*/
//#define SETALC            0x23		     /*设置功放ALC开关*/
//#define SETGAIN           0x45         /*设置最大增益*/
//#define SETGA             0x4a         /*设置过功率告警门限值*/
//#define SETAGCGA          0xf1		     /*设置查询AGC门限值*/
//#define SETAGC            0xe0         /*自动AGC开关*/
//#define QUERY_IN_PO		    0xe3         /*获取输入功率*/
//#define QUERY_PO		      0xe5         /*获取输出功率*/
//#define SETCH             0x21         /*设置信道*/

//#define SETMAXGAIN		    0xc4         /*京奥公司设置查询最大增益*/
//#define HESETAGCSW        0xcc		     /*主控板AGC状态*/
//#define HESETGAGAIN       0xcd         /*设置整机AGC门限值和最大增益*/
//#define HEGETTOTALATT     0xce		     /*自动AGC时获取获取整机ATT*/

///*
//*应答标志编码定义
//*/
//typedef enum
//{
//  ACK_OK    = 0,     /*成功*/
//  COMMAND_ERR,       /*命令号错*/
//  DATA_ERR,          /*命令数据错*/
//  OPERATE_ERR,       /*操作失败*/
//  OTHER_ERR	         /*保留*/
//}Rs485AckStatus;

/*
*功放模块回应数据
*/
typedef struct {
	u8 st;        //模块状态
	s8 npo;       //反向功率
	s8 tp;        //温度
	s8 alc;       //ALC值
	s8 swr;       //驻波比
	u8 att;       //衰减值
	s8 po;        //前向功率
} _PA_RP;
/*
*上行微放模块回应数据
*/
typedef struct {
	u8 st;     //模块状态
	u8 att;    //衰减
	s8 pi;     //输入功率
	s8 po;     //输出功率
} _ULMPA_RP;
/*
*下行微放模块回应数据
*/
typedef struct {
	u8 st1;   //模块状态1
	u8 st2;	  //模块状态2
	u8 att;	  //衰减值
	s8 pi;	  //输入功率
	s8 po;    //输出正向功率
	s8 npo;   //输出反向功率
	s8 relo;  //回波损耗
	s8 tp;    //模块温度

} _DLMPA_RP;
#pragma pack(1)
typedef union {
	float a;
	u32 b;
} FREQ;
#pragma pack()
/*
*微放信道模块回应数据
*/
typedef struct {
	u8   st;      //读写标志
	u8   chn;     //信道号
	FREQ FREQArr[2];    //频点的频率
} _MPACH_RP;
/*
*微放输出欠 过功率门限
*/
typedef struct {
	s8 DL_UNDER_PT; //下行输出欠功率门限
	s8 DL_OVER_PT;	 //下行输出过功率门限
} _MPAPT_RP;
/*
*低噪模块回应数据
*/
typedef struct {
	u8 st;    //模块状态
	u8 att;   //衰减值
	u8 gain;  //最大增益
} _LNA_RP;
/*
*DET模块回应数据
*/
typedef struct {
	u8 st;   //模块状态
	s8 po;   //输出功率
} _DET_RP;
/*
*选频模块回应数据
*/
typedef struct {
	u8 st1;
	u8 st2;
	u8 CHN;
	FREQ FreqArr[4];
	u8 SFCHN;
} _FREQ_RP;
/*选频模块步进频率回应数据*/
typedef struct {
	u8 UL_SKIP[8];
	u8 DL_SKIP[8];
} _SKIP_RP;
/*
*光盘模块回应数据
*/
typedef struct {
	u8 st;	     //光模块状态
	s8 TXPower;     //发光功率
	u8 LDBC;	     //LD偏置电流
	u8 LDRC;	     //LD制冷电流
	s8 RXPower;      //收光功率
	s8 FSK_TXPower;	 //FSK发送功率
	s8 FSK_RXPower;  //FSK接收功率
	FREQ FSK_Freq;	 //FSK频率
	u8 att;    	 //光盘ATT值
	u8 gain;        //光盘最大增益
#if GY_XFP_ENABLE
	u8 AlarmStatus;   /*1-8路光模块收无光告警*/
	u8 XfpSW;         /*1-8路光模块开关*/
	s8 RPower[8];	   /*1-8跑光模块收光功率*/
#endif
	//u8 stateBak;    //备用光模块状态
} _XFP_RP;

/*合路推动模块回应数据*/
typedef struct {
	u8 st;   //状态
	u8 att; //ATT
	u8 pi;  //输入功率
} _PUSH_RP;




/*
*京奥公司ICS模块工作信道回应数据
*/
typedef struct {
	u8  CHN;
	u16 CH[ICSMAXCHNUM];
} ICS_CH;

/*
*京奥公司单信道AGC ATT
*/
typedef struct {
	u8  chn;
	s8  dat[ICSMAXCHNUM];
} ICS_CH_PARA;

/*数字PICO信道频率回应数据*/
typedef struct {
	u8   CHN;     //信道号
	FREQ Arr[ICSMAXCHNUM];    //频点的频率
} DIG_PICO_FREQ;

/*
*京奥公司ICS模块信道开关回应数据
*/
typedef struct {
	u8 CHN;
	u8 CHSW[3];
} ICS_CH_SW;
/*
*京奥公司ICS模块静噪开关等回应数据
*/
typedef struct {
	u8 slSWFlag;            /*静噪开关标识符*/
	u8 slSW;                /*静噪开关*/
	u8 ADCIPFlag;           /*ADC输入功率标识符*/
	s8 ADCIP;               /*ADC输入功率*/
	s8 CtrlDepth;           /*数字前端控深*/
	u8 slReviseFlag;        /*静噪修正,门限值标识符*/
	s8 slRevise;            /*静噪修正值*/
	s8 slHIGHGA;            /*静噪高门限*/
	s8 slLOWGA;             /*静噪低门限*/
	u8 DVGAFlag;            /*lna，dvga设置标识符*/
	s8 INOverGa;            /*输入过载门限*/
	u8 ManualAtt;           /*手动ATT*/
	u8 DVGASW;              /*DVGA开关*/
	u8 ICSFlag;             /*ICS参数标识符*/
	u8 ICSSW;               /*ICS开关*/
	u8 ICSGain;             /*ICS整机增益*/
	s8 ICSIsolation;        /*ICS隔离度*/
	u8 InputPFlag;          /*输入功率标识符*/
	s8 InputP;              /*输入功率*/
	s8 InputPRevise;        /*输入功率修正值*/
	u8 OutputPFlag;          /*输出功率标识符*/
	s8 OutputP;              /*输出功率*/
	s8 OutputPRevise;        /*输出功率修正值*/
	u8 CarrierFlag;          /*载波1能量标识符*/
	s8 Carrier1P;            /*载波1能量*/
	u8 CarrierReviseFlag;    /*载波能量修正值*/
	s8 CarrierOPRevise;      /*载频输出功率修正值*/
	s8 CarrierIPRevise;      /*载频输入功率修正值*/
	u8 AGCSWFlag;            /*AGC开关标识符*/
	u8 AGCSW[3];      /*载波AGC开关*/
	u8 AGCGAFlag;            /*AGC门限标识符*/
	s8 CarrierAGCGA;      /*载波AGC门限*/
	u8 AGCDepthFlag;     /*AGC深度标识符*/
	s8 AGCDepth;            /*AGC深度*/
} ICS_SL;
/*
*京奥公司ICS模块回应数据
*/
typedef struct {
	u8 ICSType;     //模块类型
	u8 Alarm;       //告警
	u8 ERL[2];		   //话务量统计
	u8 FPGAVer;        //FPGA固件号
	s8 FPGATp;         //FPGA温度
	s8 ULADCIP;     //上行ADC输入功率
	s8 ULCtrlDepth; //上行数字前端探深
	s8 ULIsolation; //上行隔离度
	s8 ULInputP;    //上行输入功率
	s8 ULOutputP;   //上行输出功率
	//	u8 ULCarrierNum;  //上行载波数
	//	s8 ULCarrier1Gain;//上行载波1增益
	//	s8 ULCarrier2Gain;//上行载波2增益
	//	s8 ULCarrier3Gain;//上行载波3增益
	//	s8 ULCarrier1IP;   //上行载波1输入功率
	//	s8 ULCarrier2IP;   //上行载波2输入功率
	//	s8 ULCarrier3IP;   //上行载波3输入功率
	s8 DLADCIP;     //下行ADC输入功率
	s8 DLCtrlDepth; //下行数字前端探深
	s8 DLIsolation; //下行隔离度
	s8 DLInputP;    //下行输入功率
	s8 DLOutputP;   //下行输出功率
	s8 ULOP[ICSMAXCHNUM];    //每载波输出功率
	s8 ULIP[ICSMAXCHNUM];    //每载波输入功率
	s8 DLOP[ICSMAXCHNUM];    //每载波输出功率
	s8 DLIP[ICSMAXCHNUM];    //每载波输入功率
	//	u8 DLCarrierNum;  //下行载波数
	//	s8 DLCarrier1Gain;//下行载波1增益
	//	s8 DLCarrier2Gain;//下行载波2增益
	//	s8 DLCarrier3Gain;//下行载波3增益
	//	s8 DLCarrier1IP;   //下行载波1输入功率
	//	s8 DLCarrier2IP;   //下行载波2输入功率
	//	s8 DLCarrier3IP;   //下行载波3输入功率

} ICS_RP;


#define MAX_PWR_LIST_NUM     1024   /*最大采样数量*/
#define SAMP_OUT             0x00    /*输出*/
#define SAMP_IN              0x01    /*输入*/

typedef struct {
	u32 CenterFreq;    /*中心频率 *100*/
	u8 BandWidth;      /*带宽 00--15.36M;01--30.72;02--61.44*/
} _Cfg_Samp;
/*
*频谱回应数据
*/
typedef struct {
	u8  SampStatus;    /*采样状态 1--接到采集指令;2--FPGA开始采集;3--等待FPGA采集完成;4--FPGA采集完成 6--模块数据准备就绪,可供查询*/
	u8  SampSel;       /*采样选择 00--输出,01--输入*/
	_Cfg_Samp SampCfg[2];/*输出,输入配置*/
	s16 PwrOffsetOut;    /*输入功率修正值*/

	u16  FrameIndex;    /*帧索引*/
	u16 PwrList[MAX_PWR_LIST_NUM];     /*功率列表*/
} _FreqList;

/*
*京奥公司ICS模块全部回应数据
*/
typedef struct {
	ICS_CH     WK_CH;  //工作信道
	ICS_CH     FS_CH;  //移频信道
	DIG_PICO_FREQ ULFREQ;  //上行工作频率
	DIG_PICO_FREQ DLFREQ;
	ICS_CH_SW  CHSW;   //信道开关
	ICS_SL     ULSL;   //上行静噪开关等信息
	ICS_SL     DLSL;   //下行静噪开关等信息
	ICS_RP     QueryRP;//模块状态查询回应
	FREQ       F1FREQ; //F1中心频率
	FREQ       F2FREQ; //F2中心频率
	u8         PICO_ENABLE;/*判断新板数字模块是否当PICO使用*/

	ICS_CH_PARA ch_ul_att;
	ICS_CH_PARA ch_dl_att;
	ICS_CH_PARA ch_ul_agc;
	ICS_CH_PARA ch_dl_agc;
	u8 block_compensate;
} _ICS_HT_INFO;

/*ODU模块回应数据*/
typedef struct {
	u8    st;	        //模块状态
	u8    AlarmStatus;	//告警状态
	u8    ODULinkSN;	//ODU模块链路识别号
	u16   ID;			//ID编号
	s8    tp;			//ODU温度
	u8    WorkV;		//ODU工作电压
	FREQ  SendF;       //微波频段发送频率
	FREQ  RecvF;       //微波频段接收频率
	u16   FreqSkip;    //微波频段频率间隔
	u8    SendCh;      //ODU发送信道频点
	u8    RecvCh;		//ODU接收信道频点
	s16   RecvV;       //微波频段接收电平
	u8    SendAtt;     //微波发送链路增益衰减
	u8    RecvAtt;  	//微波接收链路增益衰减
	s8    ULGain;      //上行增益
	s8    DLGain;		//下行增益
	s16   DLPIV;       //下行输入功率电平
	s16   DLPOV_SET;   //下行输出功率电平设置值
	s16   DLPOV;       //下行输出功率电平实际值
	s16   ULPOV;       //上行输出功率电平
	u8    CHNum;       //设备的实际信道总数
	u8    ULAGC;		//上行AGC
	u8    DLAGC;		//下行AGC
} _ODU_RP;
/*
*模块的输入输出功率指示
*/
typedef struct {
	s8 UL_LNA_PO;   /*上行底噪输出功率*/
	s8 DL_LNA_PO;   /*下行底噪输出功率*/
	s8 UL_LNA_PI;   /*上行底噪输入功率*/
	s8 DL_LNA_PI;   /*下行底噪输入功率*/
	s8 UL_PA1_PO;   /*上行功放1输出功率*/
	s8 DL_PA1_PO;   /*下行功放1输出功率*/
	s8 UL_PA1_PI;   /*上行功放1输入功率*/
	s8 DL_PA1_PI;   /*下行功放1输入功率*/
} _PWR_RP;
/*
*整机中经常需要被上层查询的数据
*/
typedef struct {
	u8 UL_PA1_SW;     /*上行功放1开关*/
	u8 DL_PA1_SW;     /*下行功放1开关*/
	u8 UL_PA1_ATT;    /*上行功放1ATT*/
	u8 DL_PA1_ATT;    /*下行功放1ATT*/
	u8 UL_LNA_ATT;    /*上行底噪ATT*/
	u8 DL_LNA_ATT;    /*下行底噪ATT*/
	u8 UL_PO_GA1;     /*上行输出功率1告警上门限*/
	u8 DL_PO_GA1;     /*下行输出功率1告警上门限*/
	u8 UL_AGC_SW;     /*上行自动AGC开关*/
	u8 DL_AGC_SW;     /*下行自动AGC开关*/
} _CHECK_RP;
/*
*与自动AGC相关的数据
*/
typedef struct {

	u8 Total_Att;  /*总衰减值*/
	/*
	*由监控板实现自动AGC时用到的变量
	*/
	u8 StartAgc;  /*启动AGC的条件 1.计算ATT 0.未满足自动AGC条件*/
	s8 IPVar;
} _AUTO_AGC;

#if READ_SN_ENABLE

#define MAX_MOD_NUM   16//射频系统所带最多模块数

/*
*模块系列号
*/
typedef struct {
	u8 Func;
	u8 Addr;
	u8 SN[10];
} _MOD_SN;
/*
* 整机中每个射频系统所带模块总数及其各自系列号
*/
typedef struct {
	u8 Mod_Num;
	_MOD_SN MOD_SN[MAX_MOD_NUM];
} _RF_MOD_SN;
#endif

/*
*所有挂载在485总线上的模块回应信息          最经常用到
*/
typedef struct {
	/*
	*与微放模块相关的回应信息
	*/
	_ULMPA_RP   UL_MPA;
	_DLMPA_RP   DL_MPA;
	_MPACH_RP   UL_MPACH_RP;
	_MPACH_RP   DL_MPACH_RP;
	_MPAPT_RP   MPAPT_RP;
	/*
	*与功放模块相关的回应信息
	*/
	_PA_RP      UL_PA1;
	_PA_RP      DL_PA1;
	/*
	*与底噪模块相关的回应信息
	*/
	_LNA_RP     UL_LNA;
	_LNA_RP     DL_LNA;
	/*
	*与DET模块相关的回应信息
	*/
	_DET_RP     UL_DET;
	_DET_RP     DL_DET;
	/*
	*京奥公司ICS模块
	*/
	_ICS_HT_INFO ICS_HT_RP;
	/*
	*与功放 低噪的输入输出功率相关
	*/
	_PWR_RP     PWR_RP;
	/*
	*选频模块回应数据
	*/
	_FREQ_RP    UL_FREQ[4];
	_FREQ_RP    DL_FREQ[4];
	_SKIP_RP    SKIP_RP;
	/*
	*光盘模块回应数据
	*/
	_XFP_RP   XFP_RP;
	/*
	*合路推动回应数据
	*/
	_PUSH_RP UL_PUSH;	   //上行合路推动没有输入功率
	_PUSH_RP DL_PUSH;
	/*
	*ODU模块回应数据
	*/
	_ODU_RP ODU_RP;
	/*
	*整机中经常需要被上层查询的数据
	*/
	_CHECK_RP   CHECK_RP;
	/*
	*与自动AGC相关的数据
	*/
	_AUTO_AGC   AUTO_AGC;
	/*
	*射频系统各模块系列号
	*/
	_RF_MOD_SN RF_MOD_SN;

	u8 NEW_LNA_FLAG;     /*新旧底噪区别*/

} MODULE_RP;

typedef struct
{
	u16 offset;
	u8 UL_PA1_SW;  /*上行功放1开关*/
	u8 DL_PA1_SW;  /*下行功放1开关*/
	u8 UL_PA1_ATT; /*上行功放1ATT*/
	u8 DL_PA1_ATT; /*下行功放1ATT*/
	u8 UL_LNA_ATT; /*上行底噪ATT*/
	u8 DL_LNA_ATT; /*下行底噪ATT*/
	s8 UL_PO_GA1;  /*上行输出功率1告警上门限*/
	s8 DL_PO_GA1;  /*下行输出功率1告警上门限*/
	u8 UL_AGC_SW;  /*上行自动AGC开关*/
	u8 DL_AGC_SW;  /*下行自动AGC开关*/

	u8 UL_MAX_GAIN;   /*上行整机最大增益*/
	u8 DL_MAX_GAIN;   /*下行整机最大增益*/
	u8 UL_PA1_LOSS;   /*上行整机线损*/
	u8 DL_PA1_LOSS;   /*下行整机线损*/
	u8 UL_BAND_SW[4]; /*上行选频开关*/
	u8 DL_BAND_SW[4]; /*下行选频开关*/

	u8 UL_CH1_MAX_GAIN; /*载波1上行最大增益*/
	u8 UL_CH2_MAX_GAIN; /*载波2上行最大增益*/
	u8 UL_CH3_MAX_GAIN; /*载波3上行最大增益*/
	u8 DL_CH1_MAX_GAIN; /*载波1下行最大增益*/
	u8 DL_CH2_MAX_GAIN; /*载波2下行最大增益*/
	u8 DL_CH3_MAX_GAIN; /*载波3下行最大增益*/

	u8 Bak[30]; /*保留*/
} _GA_SW;

/*
*选频选带信道号  	结构长度: 106
*/
typedef struct
{
	u16 offset;
	u16 UL_CHN[16];
	u16 DL_CHN[16];
	u8 UL_CHN_SW[4];
	u8 DL_CHN_SW[4];
	u8 Bak[32];
} _SAVE_CHN;

/*
*保存自动AGC前各模块的ATT值,用于关闭AGC后恢复 结构长度:22
*/
typedef struct
{
	u16 offset;
	u8 ULAutoAgcEnable;
	u8 DLAutoAgcEnable;
	u8 UL_PA1_SAVE_ATT;
	u8 DL_PA1_SAVE_ATT;
	u8 UL_PA2_SAVE_ATT;
	u8 DL_PA2_SAVE_ATT;
	u8 UL_LNA_SAVE_ATT;
	u8 DL_LNA_SAVE_ATT;
	u8 UL_ICS_AUTO_AGC_ENABLE;
	u8 DL_ICS_AUTO_AGC_ENABLE;
	s8 UL_ICS_AGC_GA; //上行ICS AGC门限
	s8 DL_ICS_AGC_GA; //下行ICS AGC门限
	//#if GPS_ENABLE
	u8 UL_PA1_SAVE_SW;
	u8 DL_PA1_SAVE_SW;
	u8 SAVE_ATT_BAK[6]; //保留扩展之用
	//#else
   //u8  SAVE_ATT_BAK[8];	//保留扩展之用
	//#endif
} _SAVE_ATT;

typedef union {
	struct
	{

		u32 REC_NO_LI : 1;    //收无光
		u32 DL_LAN_FAULT : 1; //下行低噪放故障
		u32 UL_LAN_FAULT : 1; //上行低噪放故障
		u32 AC_DOWN : 1;      //电源掉电
		u32 DOOR_OPEN : 1;    //门襟告警
		u32 DL_SELF : 1;      //上行自激
		u32 DL_CH1_ULK : 1;   //下行信道1本振失锁
		u32 UL_CH1_ULK : 1;   //上行信道1本振失锁

		u32 : 1;             //收发信号告警
		u32 DL_PA1_VSWR : 1; //下行功放1驻波门限告警
		u32 UL_PA1_VSWR : 1; //上行功放1驻波门限告警
		u32 DL_PA1_OT : 1;   //下行功放1过温
		u32 UL_PA1_OT : 1;   //上行功放1过温
		u32 DL_PA1_OPO : 1;  //下行功放1过功率
		u32 UL_PA1_OPO : 1;  //上行功放1过功率
		u32 DC_FAULT : 1;    //电源模块故障

		u32 DL_PA1_FAULT : 1; //下行功放1坏
		u32 UL_PA1_FAULT : 1; //上行功放1坏
		u32 DL_CH3_ULK : 1;   //下行信道3本振失锁
		u32 UL_CH3_ULK : 1;   //上行信道3本振失锁
		u32 SEND_NO_LI : 1;   //发无光
		u32 UL_SELF : 1;      //下行自激
		u32 DL_CH2_ULK : 1;   //下行信道2本振失锁
		u32 UL_CH2_ULK : 1;   //上行信道2本振失锁

		u32 DL_PA2_OPO : 1; //下行功放2过功率
		u32 UL_PA2_OPO : 1; //上行功放2过功率
		u32 DL_CH6_ULK : 1; //下行信道6本振失锁
		u32 UL_CH6_ULK : 1; //上行信道6本振失锁
		u32 DL_CH5_ULK : 1; //下行信道5本振失锁
		u32 UL_CH5_ULK : 1; //上行信道5本振失锁
		u32 DL_CH4_ULK : 1; //下行信道4本振失锁
		u32 UL_CH4_ULK : 1; //上行信道4本振失锁

		u32 DL_CH7_ULK : 1;   //下行信道7本振失锁
		u32 UL_CH7_ULK : 1;   //上行信道7本振失锁
		u32 DL_PA2_FAULT : 1; //下行功放2坏
		u32 UL_PA2_FAULT : 1; //上行功放2坏
		u32 DL_PA2_VSWR : 1;  //下行功放2驻波门限告警
		u32 UL_PA2_VSWR : 1;  //上行功放2驻波门限告警
		u32 DL_PA2_OT : 1;    //下行功放2过温
		u32 UL_PA2_OT : 1;    //上行功放2过温

		u32 REC_NO_LI2 : 1;  //光模块2收无光
		u32 SEND_NO_LI2 : 1; //光模块2发无光
		u32 BAT_LINK : 1;    //电池链路告警
		u32 BAT_DOOR : 1;    //电池门禁告警
		u32 BAT_PV : 1;      //电池电压过底告警
		u32 BAT_OT : 1;      //电池过温度告警
		u32 UL_CH8_ULK : 1;  //上行信道8本振失锁
		u32 DL_CH8_ULK : 1;  //下行信道8本振失锁

	} BITS;
	u64 ALL_ALARM;
} DR_ALARM_TYPE;
/*
* 保存的告警使能 结构体长度：26
*/
typedef struct
{
	u16 offset;                /*结构长度*/
	DR_ALARM_TYPE AlarmEnable; /*告警使能*/
#if FAN_EXT_ALARM_ENABLE
	_FAN_EXT_ALARM EXT_FAN_ALARM; /*风扇与外部告警使能*/
	u8 AlarmEnableBak[14];        /*保留扩展之用*/
#else
	u8 AlarmEnableBak[16]; /*保留扩展之用*/
#endif
} _Alarm;

typedef enum
{
	FS = 1,      /*选频*/
	BS,          /*选带*/
	OTHER
}FREQ_Diff;

typedef struct
{
	u8 UL_EN[4];
	u8 DL_EN[4];
} _SubAddr;

typedef struct
{
	u8 Online;
	FREQ_Diff Diff;
	_SubAddr SubAddr;
} FREQ_Online;

/*
*京奥公司ICS模块
*/
typedef struct
{
	u8 Online;
	u8 Num;
} _ICS_Online;
/*
*监控板所支持的整机类型
*/
typedef struct
{
	u16 offset;
	u8 General_Online; /*通用版本*/
	u8 MPA_Online;     /*微放*/
	u8 TA_Online;      /*干放*/
	FREQ_Online FREQ1; /*单选频选带*/
	FREQ_Online FREQ2; /*双选频选带*/
	FREQ_Online FREQ4; /*四选频选带*/
	u8 DIG_SEL_Online; /*数字八选频*/
	_ICS_Online ICS;   /*ICS模块*/
	u8 UL_PA1_Online;
	u8 DL_PA1_Online;
	u8 UL_LNA_Online;
	u8 DL_LNA_Online;
	u8 ODU_Online; /*有ODU模块,用于微波移频*/
	u8 Bak[59];
} _Mod_Online;
/*
*ICS模块需要保存在监控板上的信息
*/
typedef struct
{
	u8 UL_MAX_GAIN[24];
	s8 CH_GAIN_ADJ[24];
	u8 UL_BAK[24];
	u8 DL_MAX_GAIN[24];
	s8 UL_AGC[24];
	s8 DL_AGC[24];
} _ICS_INFO;

typedef struct
{
	_GA_SW GA_SW;
	_SAVE_CHN SAVE_CHN;
	_SAVE_ATT SAVE_ATT;
	_Alarm ALARM;
	_Mod_Online MOD_ONLINE;
	_ICS_INFO ICS_INFO;
	u8 Agc_Enable;   //是否在监控板上实现AGC功能
	u8 MIMO_Enable;  /*MIMO替换使能*/
	u8 MIMO_Replace; /*MINO替换系统*/
	u8 Bak[127];
} _RF_Var;

typedef struct
{
	_RF_Var RF_SYS; /*要保存的射频数据*/
} _SYS_Var;


u8 Search_Module_Online(u8 Func, u8 Addr);








#endif

