
#ifndef  __MODULE_COMM_H__
#define  __MODULE_COMM_H__
#include <semaphore.h>
#include "helios.h"
#define OFFLINENUM      6          /*不在线次数*/

//#define GETTIME       180          /*轮询延时时间*/
//#define SETTIME       180          /*设置延时时间*/

#define GETTIME       250
#define SETTIME       250          /*设置延时时间*/

#define POLLSKIP      2            /*轮询间隔*/
#define SPECSKIP      1            /*特殊事件间隔*/

//#define MAXCHECK      72          /*最大轮询事件数*/
#define MAXSET        72          /*最大设置事件数*/
#define MAXCHECK      120


#define SETDATALEN    50          /*设置的命令体数据最大长度*/

#define MAX_LIMIT_INPUT_PWR  -10    /*最大输入功率时全衰*/

/*监控板与底层模块射频系统划分*/
typedef enum
{
 RP_DCS = 0x00,
 RP_WCDMA,
 RP_GSM,
 RP_LTE800,// RP_CDMA800,
 RP_LTE1800,//RP_LTE,
 RP_LTE700,//RP_PCS,
 RP_TETRA,//RP_CDMA400,
 RP_LTE2600,//RP_AWS,
 RP_OTHER
}MOD_SYS;

typedef enum
{
    PASW_NORMAL = 0,
    PASW_AGC_OFF = 0X01,
    PASW_DIG_PIN_OVER_OFF = 0X02,
    PASW_GPS_OFF = 0X04,
    PASW_DIG_ALARM_OFF = 0X08,
    PASW_ON = 0X80,
} PASW_FLAG;
/*
*轮询的命令格式
*/
typedef struct {
    u8  Online;       /*查询的不在线次数*/
    u8  DelayTime;    /*查询的时间间隔*/
    u8  Func;         /*模块功能编码*/
    u8  Addr;         /*模块地址编码*/
    u8  Cmd;          /*模块命令编号*/
    u8  Clen;         /*命令体长度*/
} CheckComm;
/*
*可带数据的查询格式
*/
typedef struct {
    u8 data1;         /*命令数据1*/
    u8 data2;         /*命令数据2*/
    u8  Func;         /*模块功能编码*/
    u8  Addr;         /*模块地址编码*/
    u8  Cmd;          /*模块命令编号*/
    u8  Clen;         /*命令体长度*/
} OnlyCheckComm;
/*
*轮询的总结构
*/
typedef struct {
    u8 CheckNum;   /*轮询数量*/
    u8 PollAdd ;                /*轮询自加数*/
    u32 GlobalPollNum ;        /*大循环轮询次数*/
    CheckComm Arr[MAXCHECK];/*轮询结构集合*/
} _CheckCommArr;

/*
*设置模块命令
*/
typedef struct {
    u8 Func;  /*模块功能编码*/
    u8 Addr;  /*模块地址编码*/
    u8 Cmd;   /*模块命令编码*/
    u8 Clen;  /*命令体长度*/
    u8 data[SETDATALEN]; /*命令体数据*/
} _SET_STRUCT;

/*
*设置模块命令总结构
*/
typedef struct {
    u8 SetNum;	   /*设置数量*/
    u8 SetNumAdd;  /*已经设置的数量*/
    _SET_STRUCT  Arr[MAXSET];/*设置结构集合*/
} _SET_MOD;

/*
*自动AGC使能时设置ATT命令
*/
typedef struct {
    u8 Func;  /*模块功能编码*/
    u8 Addr;  /*模块地址编码*/
    u8 Cmd;   /*模块命令编码*/
    u8 Clen;  /*命令体长度*/
    u8 data; /*ATT数据*/
} _SET_ATT_STRUCT;
/*
*自动AGC使能时设置ATT命令总结构
*/
typedef struct {
    u8 SetNum;	   /*设置数量*/
    u8 SetNumAdd;  /*已经设置的数量*/
    _SET_ATT_STRUCT  Arr[MAXSET];/*设置结构集合*/
} _SET_ATT_MOD;

/*
*设置命令总结构,使用信号量传递
*/
typedef struct {
    _SET_STRUCT SET_STRUCT[5];       /*设置命令结构*/
    sem_t SetSem;                   /*设置命令信号量*/
    u8          SetSemFlag ;      /*设置命令标志*/
} _SET_MOD_SEM;

void *rs485_thread(void *arg);
u16 GET_CHN(u32 freq,u8 uldl,u8 type);
int GET_FREQ(u16 chnum,u8 uldl,u8 type);
u8 get_rs485_mod_init_state();
#endif


