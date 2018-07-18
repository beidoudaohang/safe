#include <math.h>
#include <string.h>

#include "helios.h"
#include "Module_Comm.h"
#include "Module_Struct.h"
#include "crc.h"
#include "para_table.h"
#include "porting.h"
#include "log.h"
#include "protocol_trans.h"

#define SPI_Flash_Write

const CheckComm  InvalidCmd = {0x00, POLLSKIP, 0x06, 0x00, 0x11, 0x00};

u8 RSSendBuf[MAXRSBUFLEN];      /*RS485发送缓冲区*/
u8 RSRecvBuf[MAXRSBUFLEN];      /*RS485接收缓冲区*/
int RSSendLen;                   /*RS485发送数据长度*/
int RSRecvLen;                   /*RS485接收数据长度*/

_CheckCommArr CheckCommArr;    /*轮询事件数组*/
//u8 PollAdd = 0;                /*轮询自加数*/
//u32 GlobalPollNum = 1;        /*大循环轮询次数*/

_SET_MOD SetModArr;              /*设置模块事件数组*/
_SET_MOD_SEM  SetSem;            /*使用信号量设置*/
_SET_ATT_MOD  SetAttArr;         /*自动AGC使能时设置ATT数组*/

//_DIG_TYPE DIG_TYPE[SYS_NUM];
u8  ICS_HT_FUNC = ICS_FUNC;     /*判断是新板数字模块还是旧版数字模块*/
//u8  PICO_ENABLE[SYS_NUM];              /*判断新板数字模块是否当PICO使用*/

u32 Try_Check_Time  = 0  ;     /*每隔一天重新查询不在线模块*/
_SYS_Var  SYS_Var[SYS_NUM];
MODULE_RP MOD_RP[SYS_NUM];

u8 MPA_RcvBuf[100];
u8 MPA_RcvLen;
u8 ChangeFreqSucceed;
int RecvPackIndex ;

/*add by salan @20161227*/
PASW_FLAG pasw_flag_t[SYS_NUM] = {PASW_NORMAL};
u8 DelayFlag = 0;

//extern unit_para unit_para_t;

s8 FillPollData(u8 *buf, CheckComm *data, u8 type);
u8 Mod_RP_Handle(u8 *SendBuf, u8 *RecvBuf, u8 RecvLen);


#define EGSM_UL_FREQ      880.20
#define EGSM_DL_FREQ      925.20
#define GSM_UL_FREQ       890.00
#define GSM_DL_FREQ       935.00

#define DCS_UL_FREQ       1710.20
#define DCS_DL_FREQ       1805.20

#define WCDMA_FREQ_START  1810.00
//#define WCDMA_FREQ_START  5.00

#define WCDMA_UL_FREQ     824.00
#define WCDMA_DL_FREQ     869.00

#define ECDMA_UL_FREQ	  824.040
#define ECDMA_DL_FREQ	  869.040
#define CDMA_UL_FREQ	  825.000
#define CDMA_DL_FREQ	  870.000

#define PCS_UL_FREQ       1850.2
#define PCS_DL_FREQ       1930.2

#define FREQ_SKIP         0.2

#define FREQ_SKIP_CDMA    0.03

#define CELL_UL_FREQ    380.0125
#define CELL_DL_FREQ    390.0125
#define FREQ_SKIP_CELL  0.025
/*
*********************************************************************************************************
*	函 数 名: GET_CHN
*	功能说明: 根据中心频率获取信道号
*	形    参：freq：中心频率 uldl:上下行　type: 射频系统
*             uldl:上下行
*	返 回 值: 中心频率对应的信道号
*********************************************************************************************************
*/
u16 GET_CHN(u32 freq,u8 uldl,u8 type)
{
    u16 chnum = 0xffff;
    static FREQ temp;

    temp.b = freq;
    memcpy((u8 *)&temp.b, (u8 *)&freq, 4);

    if (type == RP_WCDMA)
        temp.a = temp.a + 0.02;
    /*        DCS频率转换信道号处理　　　　　*/
    if (type == RP_DCS)
    {
        temp.a = temp.a + 0.1;
        if (uldl == UL)
        {
            if (temp.a >= DCS_UL_FREQ)
                chnum = (temp.a - DCS_UL_FREQ) / FREQ_SKIP + 512;
        }
        else if (uldl == DL)
        {
            if (temp.a >= DCS_DL_FREQ)
                chnum = (temp.a - DCS_DL_FREQ) / FREQ_SKIP + 512;
        }
    }
    /*        WCDMA频率转换信道号处理　　　　　*/
    else if (type == RP_WCDMA)
    {
        if (uldl == UL)
        {
            if (temp.a >= WCDMA_FREQ_START)
                chnum = (temp.a + 190.0) / FREQ_SKIP;
            /*20140109 GSM信道并入WCDMA*/
            else if (temp.a >= GSM_UL_FREQ)
                chnum = (temp.a - GSM_UL_FREQ) / FREQ_SKIP;
            else if (temp.a >= EGSM_UL_FREQ)
                chnum = (temp.a - EGSM_UL_FREQ) / FREQ_SKIP + 975;

            else if (temp.a >= WCDMA_UL_FREQ)
                chnum = (temp.a - WCDMA_UL_FREQ) / FREQ_SKIP + 4120;
        }
        else if (uldl == DL)
        {
            if (temp.a >= WCDMA_FREQ_START)
                chnum = temp.a / FREQ_SKIP;
            else if (temp.a >= GSM_DL_FREQ)
                chnum = (temp.a - GSM_DL_FREQ) / FREQ_SKIP;
            else if (temp.a >= EGSM_DL_FREQ)
                chnum = (temp.a - EGSM_DL_FREQ) / FREQ_SKIP + 975;
            else if (temp.a >= WCDMA_DL_FREQ)
                chnum = (temp.a - WCDMA_DL_FREQ) / FREQ_SKIP + 4345;
        }
    }
    /*        GSM频率转换信道号处理　　　　　*/
    else if (type == RP_GSM)
    {
        temp.a = temp.a + 0.1;
        /*20131010把DCS 信道也并入GSM 中*/
        if (uldl == UL)
        {
            if (temp.a >= DCS_UL_FREQ)
                chnum = (temp.a - DCS_UL_FREQ) / FREQ_SKIP + 512;
            else if (temp.a >= GSM_UL_FREQ)
                chnum = (temp.a - GSM_UL_FREQ) / FREQ_SKIP;
            else if (temp.a >= EGSM_UL_FREQ)
                chnum = (temp.a - EGSM_UL_FREQ) / FREQ_SKIP + 975;
        }
        else if (uldl == DL)
        {
            if (temp.a >= DCS_DL_FREQ)
                chnum = (temp.a - DCS_DL_FREQ) / FREQ_SKIP + 512;
            else if (temp.a >= GSM_DL_FREQ)
                chnum = (temp.a - GSM_DL_FREQ) / FREQ_SKIP;
            else if (temp.a >= EGSM_DL_FREQ)
                chnum = (temp.a - EGSM_DL_FREQ) / FREQ_SKIP + 975;
        }
    }
    /*        CDMA800和CELL频率转换信道号处理　　　　　*/
    else if ((type == RP_LTE800) /*||(type == RP_Cell)*/)
    {
        if (uldl == UL)
        {
            if ((temp.a >= 832.00) && (temp.a <= 861.9))
                chnum = (temp.a + 0.005 - 832.00) / 0.1 + 24150;
        }
        else if (uldl == DL)
        {
            if ((temp.a >= 791.0) && (temp.a <= 821.0))
            {
                chnum = (temp.a + 0.005 - 791.0) / 0.1 + 6150;
                if (chnum > 6449)
                    chnum = 6449;
            }
        }
    }
    /*        PCS频率转换信道号处理　　　　　*/
    else if (type == RP_LTE700)
    {
        if (uldl == UL)
        {
            if ((temp.a >= 777.00) && (temp.a <= 786.9))
                chnum = (temp.a + 0.005 - 777.00) / 0.1 + 23180;
        }
        else if (uldl == DL)
        {
            if ((temp.a >= 746.00) && (temp.a <= 755.9))
            {
                chnum = (temp.a + 0.005 - 746.00) / 0.1 + 5180;
            }
        }
    }
    /*        LTE频率转换信道号处理　　　　　*/
    else if (type == RP_LTE1800)
    {
        if (uldl == UL)
        {
            if ((temp.a >= 1710.00) && (temp.a <= 1784.9))
                chnum = (temp.a + 0.005 - 1710.00) / 0.1 + 19200;
        }
        else if (uldl == DL)
        {
            if ((temp.a >= 1805.0) && (temp.a <= 1879.9))
            {
                chnum = (temp.a + 0.005 - 1805.0) / 0.1 + 1200;
            }
        }
    }
    /*        AWS频率转换信道号处理　　　　　*/
    else if (type == RP_LTE2600)
    {
        if (uldl == UL)
        {
            if ((temp.a >= 2500.00) && (temp.a <= 2569.9))
                chnum = (temp.a + 0.005 - 2500.00) / 0.1 + 20750;
        }
        else if (uldl == DL)
        {
            if ((temp.a >= 2620.00) && (temp.a <= 2689.90))
                chnum = (temp.a + 0.005 - 2620.00) / 0.1 + 2750;
        }
    }
    /*        CDMA400频率转换信道号处理　　　　　*/
    else if (type == RP_TETRA)
    {
        if (uldl == UL)
        {
            if (temp.a >= 451.00)
                chnum = (temp.a - 451.00) / 0.02 + 1024;
            else if (temp.a >= CELL_UL_FREQ)
                chnum = (temp.a - 290 - 0.0125) / FREQ_SKIP_CELL;
        }
        else if (uldl == DL)
        {
            if (temp.a >= 461.00)
                chnum = (temp.a - 461.00) / 0.02 + 1024;
            else if (temp.a >= CELL_DL_FREQ)
                chnum = (temp.a - 300 - 0.0125) / FREQ_SKIP_CELL;
        }
    }
    else if (type == RP_OTHER)
    {
    }

    return chnum;
}
/*
*********************************************************************************************************
*	函 数 名: GET_FREQ
*	功能说明: 根据信道号获取中心频率
*	形    参：chnum：信道号 uldl:上下行　type: 射频系统
*	返 回 值: 信道号对应的中心频率
*********************************************************************************************************
*/
int GET_FREQ(u16 chnum, u8 uldl, u8 type)
{
    f32 freq;

    if(chnum == 0xffff) return 0;

    freq = 0;

    if (type == RP_DCS)
    {
        if (uldl == UL)
        {
            if (chnum >= 512)
                freq = (chnum - 512) * FREQ_SKIP + DCS_UL_FREQ;
        }
        else if (uldl == DL)
        {
            if (chnum >= 512)
                freq = (chnum - 512) * FREQ_SKIP + DCS_DL_FREQ;
        }
    }
    else if (type == RP_WCDMA)
    {
        if (uldl == UL)
        {
            if (chnum >= 10000)
                freq = chnum * FREQ_SKIP - 190.00;
            else if (chnum >= 4120)
                freq = (chnum - 4120) * FREQ_SKIP + WCDMA_UL_FREQ;
            else if ((chnum >= 975) && (chnum <= 1023))
                freq = (chnum - 975) * FREQ_SKIP + EGSM_UL_FREQ;
            else
                freq = (chnum)*FREQ_SKIP + GSM_UL_FREQ;
        }
        else if (uldl == DL)
        {
            if (chnum >= 10000)
                freq = chnum * FREQ_SKIP;
            else if (chnum >= 4345)
                freq = (chnum - 4345) * FREQ_SKIP + WCDMA_DL_FREQ;
            else if ((chnum >= 975) && (chnum <= 1023))
                freq = (chnum - 975) * FREQ_SKIP + EGSM_DL_FREQ;
            else
                freq = (chnum)*FREQ_SKIP + GSM_DL_FREQ;
        }
    }
    else if (type == RP_GSM)
    {
        if (uldl == UL)
        {
            if ((chnum >= 975) && (chnum <= 1023))
                freq = (chnum - 975) * FREQ_SKIP + EGSM_UL_FREQ;
            else if (chnum >= 512)
                freq = (chnum - 512) * FREQ_SKIP + DCS_UL_FREQ;
            else /*if(chnum >= 0)*/
                freq = (chnum)*FREQ_SKIP + GSM_UL_FREQ;
        }
        else if (uldl == DL)
        {
            if ((chnum >= 975) && (chnum <= 1023))
                freq = (chnum - 975) * FREQ_SKIP + EGSM_DL_FREQ;
            else if (chnum >= 512)
                freq = (chnum - 512) * FREQ_SKIP + DCS_DL_FREQ;
            else /*if(chnum >= 0)*/
                freq = (chnum)*FREQ_SKIP + GSM_DL_FREQ;
        }
    }
    else if ((type == RP_LTE800) /*||(type == RP_Cell)*/)
    {
        if (uldl == DL)
        {
            if ((chnum >= 6150) && (chnum <= 6449))
            {
                freq = (chnum - 6150) * 0.1 + 791.0;
            }
        }
        else if (uldl == UL)
        {
            if ((chnum >= 24150) && (chnum <= 24449))
                freq = (chnum - 24150) * 0.1 + 832.0;
        }
    }
    else if (type == RP_LTE700)
    {
        if (uldl == DL)
        {
            if ((chnum >= 5180) && (chnum <= 5279))
            {
                freq = (chnum - 5180) * 0.1 + 746.0;
            }
        }
        else if (uldl == UL)
        {
            if ((chnum >= 23180) && (chnum <= 23279))
                freq = (chnum - 23180) * 0.1 + 777.0;
        }
    }
    else if (type == RP_LTE1800)
    {
        if (uldl == UL)
        {
            if ((chnum >= 19200) && (chnum <= 19949))
            {
                freq = (chnum - 19200) * 0.1 + 1710.0;
            }
        }
        else if (uldl == DL)
        {
            if ((chnum >= 1200) && (chnum <= 1949))
                freq = (chnum - 1200) * 0.1 + 1805.0;
        }
    }
    else if (type == RP_LTE2600)
    {
        if (uldl == DL)
        {
            if ((chnum >= 2750) && (chnum <= 3449))
            {
                freq = (chnum - 2750) * 0.1 + 2620.0;
            }
        }
        else if (uldl == UL)
        {
            if ((chnum >= 20750) && (chnum <= 21449))
                freq = (chnum - 20750) * 0.1 + 2500.0;
        }
    }
    /*        CDMA400频率转换信道号处理　　　　　*/
    else if (type == RP_TETRA)
    {
        if (uldl == UL)
        {
            if ((chnum >= 1024) && (chnum <= 1473))
                freq = (chnum - 1024) * 0.02 + 451.000;
            else if ((chnum >= 3600) && (chnum <= 3999))
                freq = 290 + chnum * FREQ_SKIP_CELL + 0.0125;
        }
        else if (uldl == DL)
        {
            if ((chnum >= 1024) && (chnum <= 1473))
                freq = (chnum - 1024) * 0.02 + 461.000;
            else if ((chnum >= 3600) && (chnum <= 3999))
                freq = 300 + chnum * FREQ_SKIP_CELL + 0.0125;
        }
    }
    else if (type == RP_OTHER)
    {
    }

    return (int)freq;
}

/*
*********************************************************************************************************
*	函 数 名: ByteSwap
*	功能说明: u16 高低位互换
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ByteSwap(u8 *Addr)
{
    u8 temp,temp1;
    temp = *Addr;
    temp1 = *(Addr+1);
    *Addr = temp1;
    *(Addr+1) = temp;
}
/*
*********************************************************************************************************
*	函 数 名: Check_Module_Online
*	功能说明: 根据模块使能位确定需查询的模块数目与参数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Check_Module_Online(void)
{
    u8 i, j, ULModAddr, DLModAddr, ULSelAddr, DLSelAddr;
    memset((u8 *)&CheckCommArr, 0x00, sizeof(CheckCommArr));
    CheckCommArr.GlobalPollNum = 1;
    for( i = 0; i < SYS_NUM; i++) {
        ULModAddr = SYS_ADDR_BASE + i * SYS_ADDR_SKIP + UL;
        DLModAddr = SYS_ADDR_BASE + i * SYS_ADDR_SKIP + DL;
        ULSelAddr = SYS_ADDR_BASE + i * SEL_ADDR_SKIP + UL;
        DLSelAddr = SYS_ADDR_BASE + i * SEL_ADDR_SKIP + DL;
        //if((SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Online == 0x01))
        {
            CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = POLLSKIP;   //轮询时间
            CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = PA_FUNC;   //模块功能编码
            CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = ULModAddr; //模块地址编码
            CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd       = QUERY;     //查询上行功放状态
            CheckCommArr.CheckNum += 1;
            CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = POLLSKIP;   //轮询时间
            CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = PA_FUNC;   //模块功能编码
            CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = ULModAddr; //模块地址编码
            CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd       = GETIP;     //查询上行功放输入功率
            CheckCommArr.CheckNum += 1;

            CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = POLLSKIP;   //轮询时间
            CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = PA_FUNC;   //模块功能编码
            CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = DLModAddr; //模块地址编码
            CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd       = QUERY;     //查询下行功放状态
            CheckCommArr.CheckNum += 1;
            CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = POLLSKIP;   //轮询时间
            CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = PA_FUNC;   //模块功能编码
            CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = DLModAddr; //模块地址编码
            CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd       = GETIP;     //查询下行功放输入功率
            CheckCommArr.CheckNum += 1;
            #if 0
            if(SYS_Var[i].RF_SYS.Agc_Enable == ModelComplete) {
                CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = POLLSKIP;   //轮询时间
                CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = PA_FUNC;   //模块功能编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = ULModAddr; //模块地址编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd      = SETAGCGA;     //查询上行功放AGC门限
                CheckCommArr.Arr[CheckCommArr.CheckNum].Clen      = 0x02;      //命令体长度
                CheckCommArr.CheckNum += 1;
                CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = POLLSKIP;   //轮询时间
                CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = PA_FUNC;   //模块功能编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = ULModAddr; //模块地址编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd      = SETAGCSW;     //查询上行功放自动AGC开关
                CheckCommArr.Arr[CheckCommArr.CheckNum].Clen      = 0x02;      //命令体长度
                CheckCommArr.CheckNum += 1;

                CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = POLLSKIP;   //轮询时间
                CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = PA_FUNC;   //模块功能编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = DLModAddr; //模块地址编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd      = SETAGCGA;     //查询下行功放AGC门限
                CheckCommArr.Arr[CheckCommArr.CheckNum].Clen      = 0x02;      //命令体长度
                CheckCommArr.CheckNum += 1;
                CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = POLLSKIP;   //轮询时间
                CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = PA_FUNC;   //模块功能编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = DLModAddr; //模块地址编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd      = SETAGCSW;     //查询下行功放自动AGC开关
                CheckCommArr.Arr[CheckCommArr.CheckNum].Clen      = 0x02;      //命令体长度
                CheckCommArr.CheckNum += 1;
            }
            #endif
        }
        //if(SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Online == 0x01)
        { //ICS模块不需要查询低噪放信息
            CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = POLLSKIP;       //轮询时间
            CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = ICS_HT_FUNC;   //模块功能编码
            CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = DLModAddr;     //模块地址编码
            CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd       = QUERY;         //查询ICS状态
            CheckCommArr.CheckNum += 1;

        }

        /*2012-6-5为了区分自动AGC是检测下行底噪输入功率或下行功放输出功率,在这里面先手动填值,如果存在下行底噪输入功率,则用下行底噪输入功率来计算ATT
          否则用下行功放输出功率来计算ATT*/
        MOD_RP[i].PWR_RP.DL_LNA_PI = -126;

    }//end for( i = 0;i < SYSNUM;i++)
}

/*
*********************************************************************************************************
*	函 数 名: Only_Check_Module_Online
*	功能说明: 只在开机初始化时查询一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Only_Check_Module_Online(void)
{
    u8 j;
    u8 i , ULModAddr, DLModAddr;
    u8 ULSelAddr, DLSelAddr;
    OnlyCheckComm *OnlyCheck;
    ///////////////20130705加入判断新旧数字模块的方法////////////////////////////////
    memset((u8 *)&CheckCommArr, 0x00, sizeof(CheckCommArr));
    for( i = 0; i < SYS_NUM; i++) {
        ULModAddr = SYS_ADDR_BASE + i * SYS_ADDR_SKIP + UL;
        DLModAddr = SYS_ADDR_BASE + i * SYS_ADDR_SKIP + DL;
        ULSelAddr = SYS_ADDR_BASE + i * SEL_ADDR_SKIP + UL;
        DLSelAddr = SYS_ADDR_BASE + i * SEL_ADDR_SKIP + DL;

        //if( (SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Online == 0x01) )
        {
            // OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
            // OnlyCheck->Func = ICS_FUNC;
            // OnlyCheck->Addr = DLModAddr;
            // OnlyCheck->Cmd  = QUERY;
            // CheckCommArr.CheckNum += 1;
            OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
            OnlyCheck->Func = DIG_PICO;
            OnlyCheck->Addr = DLModAddr;
            OnlyCheck->Cmd  = QUERY;
            CheckCommArr.CheckNum += 1;
        }
    }

    for( i = 0; i < CheckCommArr.CheckNum; i++ ) {
        memset(RSSendBuf, 0x00, sizeof(RSSendBuf));
        if( (RSSendLen = FillPollData(RSSendBuf, &CheckCommArr.Arr[i], 0x01)) == -1)
            continue;
        else {
            RS485_SEND(RSSendBuf, RSSendLen);
            RSRecvLen = RS485_RECV(RSRecvBuf, sizeof(RSRecvBuf), GETTIME);
            if(RSRecvLen != 0x00) {
                Mod_RP_Handle(RSSendBuf, RSRecvBuf, RSRecvLen);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    memset((u8 *)&CheckCommArr, 0x00, sizeof(CheckCommArr));
    for( i = 0; i < SYS_NUM; i++) {
        ULModAddr = SYS_ADDR_BASE + i * SYS_ADDR_SKIP + UL;
        DLModAddr = SYS_ADDR_BASE + i * SYS_ADDR_SKIP + DL;
        ULSelAddr = SYS_ADDR_BASE + i * SEL_ADDR_SKIP + UL;
        DLSelAddr = SYS_ADDR_BASE + i * SEL_ADDR_SKIP + DL;

        //if(SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Online == 0x01)
        {
            OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
            OnlyCheck->Func = PA_FUNC;
            OnlyCheck->Addr = ULModAddr;
            OnlyCheck->Cmd  = SETSN;
            OnlyCheck->Clen = 0x0B;
            OnlyCheck->data1 = 0x00;
            CheckCommArr.CheckNum += 1;

            OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
            OnlyCheck->Func = PA_FUNC;
            OnlyCheck->Addr = DLModAddr;
            OnlyCheck->Cmd  = SETSN;
            OnlyCheck->Clen = 0x0B;
            OnlyCheck->data1 = 0x00;
            CheckCommArr.CheckNum += 1;

            OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
            OnlyCheck->Func = ICS_HT_FUNC;
            OnlyCheck->Addr = DLModAddr;
            OnlyCheck->Cmd  = SETSN;
            OnlyCheck->Clen = 0x0B;
            OnlyCheck->data1 = 0x00;
            CheckCommArr.CheckNum += 1;

            for(j = 0; j < 2; j++) {
                OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                OnlyCheck->Func = ICS_HT_FUNC;    //ICS模块的F1中心频率
                OnlyCheck->Addr = DLModAddr;
                OnlyCheck->Cmd = SETFREQ;
                OnlyCheck->Clen = 0x06;
                OnlyCheck->data1 = 0x01;
                CheckCommArr.CheckNum += 1;
                OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的F2中心频率
                OnlyCheck->Addr = DLModAddr;
                OnlyCheck->Cmd = SETFREQ;
                OnlyCheck->Clen = 0x06;
                OnlyCheck->data1 = 0x02;
                CheckCommArr.CheckNum += 1;
                if(ICS_HT_FUNC == ICS_FUNC) { /*旧版本的数字模块*/
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的工作信道号
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03 + SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num * 2;
                    OnlyCheck->data1 = 0x32;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的移频信道号
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03 + SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num * 2;
                    OnlyCheck->data1 = 0x4a;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的信道开关
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x06;
                    OnlyCheck->data1 = 0x01;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的上行静噪开关等信息
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = sizeof(ICS_SL);
                    OnlyCheck->data1 = 0x62;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的下行静噪开关等信息
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = sizeof(ICS_SL);
                    OnlyCheck->data1 = 0x67;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的类型
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03;
                    OnlyCheck->data1 = 0xff;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的上行ATT
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03 + SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    OnlyCheck->data1 = 0xe0;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的下行ATT
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03 + SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    OnlyCheck->data1 = 0xe1;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的上行AGC
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03 + SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    OnlyCheck->data1 = 0xe2;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的下行AGC
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03 + SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    OnlyCheck->data1 = 0xe3;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的阻塞最大补偿值
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x06;
                    OnlyCheck->data1 = 0xe4;
                    CheckCommArr.CheckNum += 1;
                } else {
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS下行工作信道频率设置
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03 + SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num * 4;
                    OnlyCheck->data1 = 0x02;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS下行信道开关
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x06;
                    OnlyCheck->data1 = 0x06;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS上行可见参数
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 12;
                    OnlyCheck->data1 = 0x30;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS下行可见参数
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 12;
                    OnlyCheck->data1 = 0x31;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块类型
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03;
                    OnlyCheck->data1 = 0x32;
                    CheckCommArr.CheckNum += 1;

                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的上行ATT
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03 + SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    OnlyCheck->data1 = 0xe0;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的下行ATT
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03 + SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    OnlyCheck->data1 = 0xe1;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的上行AGC
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03 + SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    OnlyCheck->data1 = 0xe2;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的下行AGC
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x03 + SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    OnlyCheck->data1 = 0xe3;
                    OnlyCheck->data2 = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
                    CheckCommArr.CheckNum += 1;
                    OnlyCheck = (OnlyCheckComm *)&CheckCommArr.Arr[CheckCommArr.CheckNum];
                    OnlyCheck->Func = ICS_HT_FUNC;   //ICS模块的阻塞最大补偿值
                    OnlyCheck->Addr = DLModAddr;
                    OnlyCheck->Cmd = SETICS;
                    OnlyCheck->Clen = 0x06;
                    OnlyCheck->data1 = 0xe4;
                    CheckCommArr.CheckNum += 1;
                }
            }
        }
    }

    for( i = 0; i < CheckCommArr.CheckNum; i++ ) {
        memset(RSSendBuf, 0x00, sizeof(RSSendBuf));
        if( (RSSendLen = FillPollData(RSSendBuf, &CheckCommArr.Arr[i], 0x01)) == -1)
            continue;
        else {
            RS485_SEND(RSSendBuf, RSSendLen);
            //memset(RSRecvBuf,0x00,sizeof(RSRecvBuf));
            RSRecvLen = RS485_RECV(RSRecvBuf, sizeof(RSRecvBuf), GETTIME);
            if(RSRecvLen != 0x00) {
                Mod_RP_Handle(RSSendBuf, RSRecvBuf, RSRecvLen);
            }
        }
    }
}
/*
*********************************************************************************************************
*	函 数 名: Search_Module_Online
*	功能说明: 搜索模块是否在线
*	形    参：无
*	返 回 值: 模块在线状态 返回模块未在线次数 1:模块在线 0:模块不在线
*********************************************************************************************************
*/
u8 Search_Module_Online(u8 Func, u8 Addr)
{
    u8 i;
    for( i = 0; i < CheckCommArr.CheckNum; i++) {
        if((CheckCommArr.Arr[i].Func == Func) && (CheckCommArr.Arr[i].Addr == Addr) && (CheckCommArr.Arr[i].Cmd == QUERY)) {
            if(CheckCommArr.Arr[i].Online < OFFLINENUM)
                return 1;
            else
                return 0;
        }
    }
    return 0;
}

#if 0
/*
*********************************************************************************************************
*	函 数 名: Auto_AGC_Check
*	功能说明: 自动AGC需查询的命令
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Auto_AGC_Check(void)
{
    u8 i, DLModAddr;

    for( i = 0; i < SYS_NUM; i++ ) {
        //ULModAddr = SYS_ADDR_BASE + i * SYS_ADDR_SKIP+UL;
        DLModAddr = SYS_ADDR_BASE + i * SYS_ADDR_SKIP + DL;
        if(((SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Online == 0x01) && (MOD_RP[i].ICS_HT_RP.PICO_ENABLE)) || (SYS_Var[i].RF_SYS.MOD_ONLINE.MPA_Online == 0x01)) //ICS 微放类型不需要自动AGC
            continue;
        if((SYS_Var[i].RF_SYS.Agc_Enable == ModelComplete) && (SYS_Var[i].RF_SYS.GA_SW.DL_AGC_SW == 0x01)) { //模块本身实现自动AGC不需要查询
            //salan add at 20160310
            if(Search_Module_Online(ICS_HT_FUNC, DLModAddr)) {
                CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = SPECSKIP;   //轮询时间
                CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = ICS_HT_FUNC;  //模块功能编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = DLModAddr; //模块地址编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd      = QUERY;     //查询下行低噪输入功率
                CheckCommArr.CheckNum += 1;
            }
            //the end of add
        } else if((SYS_Var[i].RF_SYS.Agc_Enable == JKComplete) && (SYS_Var[i].RF_SYS.GA_SW.DL_AGC_SW == 0x01)) { //由监控板实现需要查询下行低噪输入功率
            if(Search_Module_Online(LNA_FUNC, DLModAddr)) {
                CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = SPECSKIP;   //轮询时间
                CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = LNA_FUNC;  //模块功能编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = DLModAddr; //模块地址编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd      = GETIP;     //查询下行低噪输入功率
                CheckCommArr.CheckNum += 1;
            }
            if(SYS_Var[i].RF_SYS.MOD_ONLINE.TA_Online == 0x01) {
                //20101027京奥公司的干放中没有低噪，输入功率检测在推动中完成
                if(Search_Module_Online(PUSH_FUNC, DLModAddr)) {
                    CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = SPECSKIP;   //轮询时间
                    CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = PUSH_FUNC;  //模块功能编码
                    CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = DLModAddr; //模块地址编码
                    CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd      = GETIP;     //查询下行低噪输入功率
                    CheckCommArr.CheckNum += 1;
                }
                //	           //20110504如果没有下行低噪,也没有推动,自动AGC需要查询下行功放输入功率
                //			   if(Search_Module_Online(PA_FUNC,DLModAddr))
                //			     {
                //		           CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = SPECSKIP;   //轮询时间
                //		           CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = PA_FUNC;  //模块功能编码
                //		           CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = DLModAddr; //模块地址编码
                //		           CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd      = GETIP;     //查询下行功放输入功率
                //		           CheckCommArr.CheckNum += 1;
                //                 }
            }
        } else if((SYS_Var[i].RF_SYS.Agc_Enable == DLLNAComplete) && (SYS_Var[i].RF_SYS.GA_SW.DL_AGC_SW == 0x01)) { //由下行低噪实现需查询总衰减值
            if(Search_Module_Online(LNA_FUNC, DLModAddr)) {
                CheckCommArr.Arr[CheckCommArr.CheckNum].DelayTime = SPECSKIP;   //轮询时间
                CheckCommArr.Arr[CheckCommArr.CheckNum].Func      = LNA_FUNC;  //模块功能编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Addr      = DLModAddr; //模块地址编码
                CheckCommArr.Arr[CheckCommArr.CheckNum].Cmd      = HEGETTOTALATT;  //自动AGC时获取整机ATT
                CheckCommArr.CheckNum += 1;
            }
        }
    }
}
#endif
/*
*******************************************************************************************
** 函数名称: FillPollData
** 功能描述:根据射频模块RS485接口通信协议组装要发送的查询数据
** 参    数: buf:发送缓冲区  data:填充数据区 type: 0x00--不带数据的轮询 0x01--带数据的查询
** 返 回 值: -1:模块不在线 未到轮询时间  其它:要发送的长度
*******************************************************************************************
*/
s8 FillPollData(u8 *buf, CheckComm *data, u8 type)
{
    u8 *begin;
    u16 crccode;

    if(data->DelayTime && (CheckCommArr.GlobalPollNum % data->DelayTime) && (type == 0x00))	//未到轮询时间
        return -1;


    begin = buf ;

    //salan add at 20160719 测试在轮询完wcdma旧功放后插入延时，是否能恢复新版功放的通信
    if((PA_FUNC == data->Func) && (0x0c == data->Addr) && (0x11 == data->Cmd))
        timedelay(0, 2, 0, 0);
    //the end of add
    
    *begin++ = SOFD;
    *begin++ = data->Func;
    *begin++ = data->Addr;
    *begin++ = data->Cmd;
    *begin++ = 0x00;
    *begin++ = data->Clen;
    memset(begin, 0x00, data->Clen);

    if((type == 0x01) && (data->Clen >= 0x03)) { //在只查询一次的命令中可能包含有一些数据
        *begin++ = 0x00;
        *begin++ = data->Online;
        *begin++ = data->DelayTime;
        begin += (data->Clen - 0x03);
    } else if((type == 0x01) && (data->Clen == 0x02)) { //在只查询一次的命令中可能包含有一些数据
        *begin++ = 0x00;
        *begin++ = data->Online;
        begin += (data->Clen - 0x02);
    } else {
        begin += data->Clen;
    }
    
    crccode = crc16_2((buf + 1), 5 + data->Clen, 0);
    *begin++ = crccode & 0xff;
    *begin++ = crccode >> 8;
    *begin++ = EOM;

    return 9 + data->Clen;
}
/*
*******************************************************************************************
** 函数名称: FillSetData
** 功能描述:根据射频模块RS485接口通信协议组装要发送的设置数据
** 参    数: buf:发送缓冲区  data:填充数据区
** 返 回 值: -1:模块不在线 未到轮询时间  其它:要发送的长度
*******************************************************************************************
*/
u8 FillSetData(u8 *buf, _SET_STRUCT *data)
{
    u8 *begin;
    u16 crccode;

    begin = buf ;

    *begin++ = SOFD;
    *begin++ = data->Func;
    *begin++ = data->Addr;
    *begin++ = data->Cmd;
    *begin++ = 0x00;
    *begin++ = data->Clen;
    memcpy(begin, data->data, data->Clen);
    begin += data->Clen;

    crccode = crc16_2((buf + 1), 5 + data->Clen, 0);
    *begin++ = crccode & 0xff;
    *begin++ = crccode >> 8;
    *begin++ = EOM;

    return 9 + data->Clen;
}
/*
*****************************************************************************************************
** 函数名称: Calculation_Att
** 功能描述:计算出上下行模块要衰减的ATT值
** 参    数: total_att:总衰减值 buf:buf[0]--下行低噪和上行功放衰减值 buf[1]--上行低噪和下行功放衰减值
** 返 回 值:无
******************************************************************************************************
*/
void Calculation_Att(s8 total_att, u8 *buf, u8 sys)
{
    u8 max_att;
    if(MOD_RP[sys].NEW_LNA_FLAG == 0x01)
        max_att = 25;
    else
        max_att = 31;
    if(total_att <= 0)
        total_att = 0;
    if(MOD_RP[sys].NEW_LNA_FLAG == 0x01) {
        if(total_att >= 50)
            total_att = 50;
    } else {
        if(total_att >= 62)
            total_att = 62;
    }
    if(total_att <= max_att) {
        buf[0] = total_att ;
        buf[1] = 0 ;
    } else {
        buf[0] = max_att ;
        buf[1] = total_att - max_att;
    }
}
/*
*********************************************************************************************************
*	函 数 名: void Auto_AGC_Set(void)
*	功能说明: 自动AGC时计算需衰减值
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
/*
salan add at 20160311
bit0--1,表示数字模块输入功率过高关闭uldlpa开关；
bit1--1，表示因为agc功能关闭uldlpa开关
the end off add
*/

void Auto_AGC_Set(void)
{
    u8 i, ULModAddr, DLModAddr;
    u8 SetAtt[2];
    static s8 digatt[SYS_NUM] = {0};
    s8 temp_att = 0;

    if(SetAttArr.SetNum)
        return;

    for( i = 0; i < SYS_NUM; i++) {
        ULModAddr = SYS_ADDR_BASE + i * SYS_ADDR_SKIP + UL;
        DLModAddr = SYS_ADDR_BASE + i * SYS_ADDR_SKIP + DL;

        //salan add at 20160311
        //if((0x01 == SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Online))
        {
            //temp_att = SYS_Var[i].RF_SYS.GA_SW.UL_PA1_LOSS;
            temp_att = MOD_RP[i].ICS_HT_RP.QueryRP.DLInputP;

            if(MOD_RP[i].ICS_HT_RP.DLSL.INOverGa < temp_att) {
                if(!(pasw_flag_t[i])) {	//20160418防止反复写sw
                    pasw_flag_t[i] |= PASW_DIG_PIN_OVER_OFF;
                    //close pa
                    SetAttArr.Arr[SetAttArr.SetNum].Func = PA_FUNC;
                    SetAttArr.Arr[SetAttArr.SetNum].Addr = ULModAddr;
                    SetAttArr.Arr[SetAttArr.SetNum].Cmd  = SETSW;
                    SetAttArr.Arr[SetAttArr.SetNum].Clen = 0x01;
                    SetAttArr.Arr[SetAttArr.SetNum].data = OFF;
                    SetAttArr.SetNum += 1;

                    SetAttArr.Arr[SetAttArr.SetNum].Func = PA_FUNC;
                    SetAttArr.Arr[SetAttArr.SetNum].Addr = DLModAddr;
                    SetAttArr.Arr[SetAttArr.SetNum].Cmd  = SETSW;
                    SetAttArr.Arr[SetAttArr.SetNum].Clen = 0x01;
                    SetAttArr.Arr[SetAttArr.SetNum].data = OFF;
                    SetAttArr.SetNum += 1;
                } else {
                    pasw_flag_t[i] |= PASW_DIG_PIN_OVER_OFF;
                }
            } else if(((MOD_RP[i].ICS_HT_RP.DLSL.INOverGa - 6) > temp_att)) {
                if((pasw_flag_t[i])) {
                    pasw_flag_t[i] &= ~PASW_DIG_PIN_OVER_OFF;

                    if(!(pasw_flag_t[i])) {
                        //recover pa sw
                        if(SYS_Var[i].RF_SYS.GA_SW.UL_PA1_SW != SYS_Var[i].RF_SYS.SAVE_ATT.UL_PA1_SAVE_SW) {
                            SetAttArr.Arr[SetAttArr.SetNum].Func = PA_FUNC;
                            SetAttArr.Arr[SetAttArr.SetNum].Addr = ULModAddr;
                            SetAttArr.Arr[SetAttArr.SetNum].Cmd  = SETSW;
                            SetAttArr.Arr[SetAttArr.SetNum].Clen = 0x01;
                            SetAttArr.Arr[SetAttArr.SetNum].data = SYS_Var[i].RF_SYS.SAVE_ATT.UL_PA1_SAVE_SW;
                            SetAttArr.SetNum += 1;
                        }
                        if(SYS_Var[i].RF_SYS.GA_SW.DL_PA1_SW != SYS_Var[i].RF_SYS.SAVE_ATT.DL_PA1_SAVE_SW) {
                            SetAttArr.Arr[SetAttArr.SetNum].Func = PA_FUNC;
                            SetAttArr.Arr[SetAttArr.SetNum].Addr = DLModAddr;
                            SetAttArr.Arr[SetAttArr.SetNum].Cmd  = SETSW;
                            SetAttArr.Arr[SetAttArr.SetNum].Clen = 0x01;
                            SetAttArr.Arr[SetAttArr.SetNum].data = SYS_Var[i].RF_SYS.SAVE_ATT.DL_PA1_SAVE_SW;
                            SetAttArr.SetNum += 1;
                        }
                    }
                }
            }
        }
        //the end of add

        //ICS 微放 ODU微波移频类型不需要自动AGC
        // if((SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Online == 0x01))
        //     continue;
    }
}

/*
*********************************************************************************************************
*	? ? ?: GET_DIG_PICO_POWER
*	????: ????PICO???????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
void GET_DIG_PICO_POWER(void)
{
    u8 i, DLModAddr;
    static u8 OnlyCheckFlag = 0;

    for( i = 0; i < SYS_NUM; i++) {
        DLModAddr = SYS_ADDR_BASE + i * SYS_ADDR_SKIP + DL;
        //if(SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Online == 0x01) 
        { 
            SetModArr.Arr[SetModArr.SetNum].Func = ICS_HT_FUNC;
            SetModArr.Arr[SetModArr.SetNum].Addr = DLModAddr;
            SetModArr.Arr[SetModArr.SetNum].Cmd  = SETICS;
            SetModArr.Arr[SetModArr.SetNum].Clen = 0x03;
            SetModArr.Arr[SetModArr.SetNum].data[0] = 0x00;
            SetModArr.Arr[SetModArr.SetNum].data[1] = 0x15;
            SetModArr.Arr[SetModArr.SetNum].data[2] = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
            SetModArr.SetNum += 1;
            SetModArr.Arr[SetModArr.SetNum].Func = ICS_HT_FUNC;
            SetModArr.Arr[SetModArr.SetNum].Addr = DLModAddr;
            SetModArr.Arr[SetModArr.SetNum].Cmd  = SETICS;
            SetModArr.Arr[SetModArr.SetNum].Clen = 0x03;
            SetModArr.Arr[SetModArr.SetNum].data[0] = 0x00;
            SetModArr.Arr[SetModArr.SetNum].data[1] = 0x16;
            SetModArr.Arr[SetModArr.SetNum].data[2] = SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num;
            SetModArr.SetNum += 1;


        }
    }
}
/*
*********************************************************************************************************
*	函 数 名: void Query_RP(Rs485Comm *data)
*	功能说明: 查询模块状态返回值
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Query_RP(Rs485Comm *data)
{
    u8 sys_num, sys_sel, submod, i;
    s16 IPTemp;

    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
    sys_sel = (data->Addr & 0xf0) / SEL_ADDR_SKIP - SYS_ADDR_BASE ; /*选频选带模块的地址区分与其它模块的地址区分不一致*/
    submod  = data->Addr & 0x07;                                  /*选频选带模块的子地址*/

    switch(data->Func) {
    case DET_FUNC:      /*DET模块*/
        if(data->Addr & 0x08) { /*上行*/
            memcpy((u8 *)&MOD_RP[sys_num].UL_DET, data->Data, 2/*data->Clen*/);
            MOD_RP[sys_num].PWR_RP.UL_LNA_PO = MOD_RP[sys_num].UL_DET.po;
        } else {
            memcpy((u8 *)&MOD_RP[sys_num].DL_DET, data->Data, 2/*data->Clen*/);
            MOD_RP[sys_num].PWR_RP.DL_LNA_PO = MOD_RP[sys_num].DL_DET.po;
        }
        break;

    case BAND1_FUNC:    /*1选频模块*/
    case BAND2_FUNC:    /*2选频模块*/
    case BAND4_FUNC:	/*4选频模块*/
        if(data->Addr & 0x08) { /*上行*/
            memcpy((u8 *)&MOD_RP[sys_sel].UL_FREQ[submod], data->Data, data->Clen);
            if((data->Func == BAND1_FUNC)  || (data->Func == BAND2_FUNC)) {
                SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod * 2] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[0].b, UL, sys_sel);
                if(data->Func == BAND2_FUNC)
                    SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod * 2 + 1 ] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[1].b, UL, sys_sel);
            }
            if(data->Func == BAND4_FUNC) {
                if(submod >= 2)
                    return;
                SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod * 4] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[0].b, UL, sys_sel);
                SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod * 4 + 1 ] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[1].b, UL, sys_sel);
                SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod * 4 + 2 ] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[2].b, UL, sys_sel);
                SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod * 4 + 3] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[3].b, UL, sys_sel);
            }
        } else {
            memcpy((u8 *)&MOD_RP[sys_sel].DL_FREQ[submod], data->Data, data->Clen);
            /*ODU模块下行信道用来存放移频信道*/
            if(SYS_Var[sys_sel].RF_SYS.MOD_ONLINE.ODU_Online == 0x01) {

            } else {
                if((data->Func == BAND1_FUNC)  || (data->Func == BAND2_FUNC)) {
                    SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod * 2] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[0].b, DL, sys_sel);
                    if(data->Func == BAND2_FUNC)
                        SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod * 2 + 1 ] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[1].b, DL, sys_sel);
                }
                if(data->Func == BAND4_FUNC) {
                    if(submod >= 2)
                        return;
                    SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod * 4] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[0].b, DL, sys_sel);
                    SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod * 4 + 1 ] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[1].b, DL, sys_sel);
                    SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod * 4 + 2 ] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[2].b, DL, sys_sel);
                    SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod * 4 + 3 ] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[3].b, DL, sys_sel);
                }
            }
        }
        break;

    case XFP_FUNC:	/*光盘模块*/
        memcpy((u8 *)&MOD_RP[sys_num].XFP_RP, data->Data, data->Clen);
#if GY_XFP_ENABLE
        if(MOD_RP[sys_num].XFP_RP.AlarmStatus)
            MOD_RP[sys_num].XFP_RP.st = MOD_RP[sys_num].XFP_RP.st | 0x04;
#endif
        break;

    case PA_FUNC:   /*功放模块*/
        if(data->Addr & 0x08) { /*上行*/
            memcpy((u8 *)&MOD_RP[sys_num].UL_PA1, data->Data, 7/*data->Clen*/);

            if(MOD_RP[sys_num].UL_PA1.st & 0x01)
                SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_SW = ON;
            else
                SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_SW = OFF;
            //if(SYS_Var[sys_num].RF_SYS.Agc_Enable == ModelComplete)
            SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_ATT = MOD_RP[sys_num].UL_PA1.att;
            if(MOD_RP[sys_num].UL_PA1.po == -127)
                MOD_RP[sys_num].PWR_RP.UL_PA1_PO = MOD_RP[sys_num].UL_PA1.po;
            else
                MOD_RP[sys_num].PWR_RP.UL_PA1_PO = MOD_RP[sys_num].UL_PA1.po - SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_LOSS;
        } else {
            memcpy((u8 *)&MOD_RP[sys_num].DL_PA1, data->Data, 7/*data->Clen*/);
            if(MOD_RP[sys_num].DL_PA1.st & 0x01)
                SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_SW = ON;
            else
                SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_SW = OFF;
            //if(SYS_Var[sys_num].RF_SYS.Agc_Enable == ModelComplete)
            SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_ATT = MOD_RP[sys_num].DL_PA1.att;
            if(MOD_RP[sys_num].DL_PA1.po == -127)
                MOD_RP[sys_num].PWR_RP.DL_PA1_PO = MOD_RP[sys_num].DL_PA1.po;
            else {
                //			    if( ((MOD_RP[sys_num].DL_PA1.po < (SYS_Var[sys_num].RF_SYS.GA_SW.DL_PO_GA1 - 1)) || (MOD_RP[sys_num].DL_PA1.po > (SYS_Var[sys_num].RF_SYS.GA_SW.DL_PO_GA1 + 1)))
                //				   && (MOD_RP[sys_num].PWR_RP.DL_LNA_PI == -126) && (SYS_Var[sys_num].RF_SYS.GA_SW.DL_AGC_SW) )
                //			   	  {
                //				   MOD_RP[sys_num].AUTO_AGC.IPVar = MOD_RP[sys_num].DL_PA1.po;
                //	  			   MOD_RP[sys_num].AUTO_AGC.StartAgc = 0x02;
                //				  }
                MOD_RP[sys_num].PWR_RP.DL_PA1_PO = MOD_RP[sys_num].DL_PA1.po - SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_LOSS;
            }
        }
        break;

    case LNA_FUNC:   /*LNA模块*/
        if(data->Addr & 0x08) { /*上行*/
            memcpy((u8 *)&MOD_RP[sys_num].UL_LNA, data->Data, 3/*data->Clen*/);
            if(SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ODU_Online == 0x01)	//ODU模块
                SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_ATT = MOD_RP[sys_num].UL_LNA.att;
            else
                SYS_Var[sys_num].RF_SYS.GA_SW.UL_LNA_ATT = MOD_RP[sys_num].UL_LNA.att;
        } else {
            memcpy((u8 *)&MOD_RP[sys_num].DL_LNA, data->Data, 3/*data->Clen*/);
            if(data->Clen == 10)/*新版底噪状态指令长度与旧板底噪不一样*/
                MOD_RP[sys_num].NEW_LNA_FLAG = 0x01;
            //if(SYS_Var[sys_num].RF_SYS.Agc_Enable == ModelComplete)
            SYS_Var[sys_num].RF_SYS.GA_SW.DL_LNA_ATT = MOD_RP[sys_num].DL_LNA.att;
        }
        break;

    case MPA_FUNC:   /*微放模块*/
        if(data->Addr & 0x08) { /*上行*/
            memcpy((u8 *)&MOD_RP[sys_num].UL_MPA, data->Data, data->Clen);

            if(MOD_RP[sys_num].UL_MPA.st & 0x01)
                SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_SW = ON;
            else
                SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_SW = OFF;
            if(SYS_Var[sys_num].RF_SYS.Agc_Enable == ModelComplete)
                SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_ATT = MOD_RP[sys_num].UL_MPA.att;

        } else {
            memcpy((u8 *)&MOD_RP[sys_num].DL_MPA, data->Data, data->Clen);

            if(MOD_RP[sys_num].DL_MPA.st1 & 0x01)
                SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_SW = ON;
            else
                SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_SW = OFF;
            if(SYS_Var[sys_num].RF_SYS.Agc_Enable == ModelComplete)
                SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_ATT = MOD_RP[sys_num].DL_MPA.att;
        }
        break;
    case PUSH_FUNC:   /*京奥公司合路推动模块*/
        if(data->Addr & 0x08) { /*上行*/
            memcpy((u8 *)&MOD_RP[sys_num].UL_PUSH, data->Data, 2/*data->Clen*/);
            SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_ATT = MOD_RP[sys_num].UL_PUSH.att;
        } else {
            memcpy((u8 *)&MOD_RP[sys_num].DL_PUSH, data->Data, 3/*data->Clen*/);
            if(SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ODU_Online == 0x01)	//ODU模块
                SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_ATT = MOD_RP[sys_num].DL_PUSH.att;
            else
                SYS_Var[sys_num].RF_SYS.GA_SW.DL_LNA_ATT = MOD_RP[sys_num].DL_PUSH.att;
            MOD_RP[sys_num].PWR_RP.DL_LNA_PI = MOD_RP[sys_num].DL_PUSH.pi;
        }
        break;
    case DIG_PICO: /*京奥公司数字PICO模块*/
        ICS_HT_FUNC = DIG_PICO;
        // for( i = 0; i < SYS_NUM; i++) {
        //     SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num = 2;
        // }

        memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.QueryRP, data->Data, 6);
        // 			 if(MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm)
        // 			 {
        // 				 u8 sendbufcom[30];
        // 				 memset(sendbufcom,0x00,sizeof(sendbufcom));
        // 				 sprintf(sendbufcom,"ICS ALARM : %d \r\n",MOD_RP[sys_num].ICS_HT_RP.QueryRP.Alarm);
        // 				 printf(sendbufcom);
        // 			 }
        // 			 else
        // 			 {
        // 				 if(sys_num == 2)
        // 				 printf("ICS NO ALARM \r\n");
        // 			 }
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULCtrlDepth = data->Data[6];
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULIsolation = data->Data[7];
        memcpy((u8 *)&IPTemp, (u8 *)&data->Data[8], 2);
        ByteSwap((u8 *)&IPTemp);
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULInputP = 	IPTemp / 16;
        memcpy((u8 *)&IPTemp, (u8 *)&data->Data[10], 2);
        ByteSwap((u8 *)&IPTemp);
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOutputP = IPTemp / 16;
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLCtrlDepth = data->Data[12];
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLIsolation = data->Data[13];
        memcpy((u8 *)&IPTemp, (u8 *)&data->Data[14], 2);
        ByteSwap((u8 *)&IPTemp);
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLInputP = 	IPTemp / 16;
        memcpy((u8 *)&IPTemp, (u8 *)&data->Data[16], 2);
        ByteSwap((u8 *)&IPTemp);
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOutputP = IPTemp / 16;

        if(MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULInputP != -127)
            MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULInputP = MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULInputP + SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_LOSS;
        if(MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLInputP != -127)
            MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLInputP = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLInputP + SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_LOSS;
        //         if(MOD_RP[sys_num].ICS_HT_RP.PICO_ENABLE)//数字PICO,输出也需要加上线损
        // 				  {
        // 			  	 if(MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOutputP != -127)
        // 			        MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOutputP = MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOutputP - SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_LOSS;
        // 		       if(MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOutputP != -127)
        // 			        MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOutputP = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOutputP - SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_LOSS;
        // 				  }
        //salan add at 20160310
        MOD_RP[sys_num].AUTO_AGC.StartAgc = 0x04;	//数字模块 + pa 计算att
        //the end off add

        break;
    case ICS_FUNC:   /*京奥公司ICS模块*/
        ICS_HT_FUNC = ICS_FUNC;

        //@sujj 西班牙移频机初始化信道数6
        // for( i = 0; i < SYS_NUM; i++) {
        //     SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num = 6;
        // }


        memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.QueryRP, data->Data, data->Clen);
        if(MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULInputP != -127)
            MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULInputP = MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULInputP + SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_LOSS;
        if(MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLInputP != -127)
            MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLInputP = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLInputP + SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_LOSS;
        //		 /**/
        //		 if(MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOutputP != -127)
        //			MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOutputP = MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOutputP + 2;
        //		  if(MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOutputP != -127)
        //			MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOutputP = MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOutputP + 2;
        //salan add at 20160310
        MOD_RP[sys_num].AUTO_AGC.StartAgc = 0x04;	//数字模块 + pa 计算att
        //the end off add
        break;
    case ODU_FUNC:    /*ODU模块*/
        memcpy((u8 *)&MOD_RP[sys_num].ODU_RP, data->Data, data->Clen);
        /*2字节表示的数据需要高低字节互换*/
        ByteSwap((u8 *)&MOD_RP[sys_num].ODU_RP.FreqSkip);
        ByteSwap((u8 *)&MOD_RP[sys_num].ODU_RP.RecvV);
        ByteSwap((u8 *)&MOD_RP[sys_num].ODU_RP.DLPIV);
        ByteSwap((u8 *)&MOD_RP[sys_num].ODU_RP.DLPOV);
        ByteSwap((u8 *)&MOD_RP[sys_num].ODU_RP.ULPOV);
        SYS_Var[sys_num].RF_SYS.SAVE_CHN.DL_CHN[0] = MOD_RP[sys_num].ODU_RP.RecvCh;
        SYS_Var[sys_num].RF_SYS.GA_SW.UL_LNA_ATT = MOD_RP[sys_num].ODU_RP.SendAtt;
        SYS_Var[sys_num].RF_SYS.GA_SW.DL_LNA_ATT = MOD_RP[sys_num].ODU_RP.RecvAtt;
        break;
    default:
        break;
    }

}
/*
*********************************************************************************************************
*	函 数 名: void SETATT_RP(Rs485Comm *data)
*	功能说明: 设置ATT命令/命令回应的命令数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SETATT_RP(Rs485Comm *data)
{
    u8 sys_num;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
    switch(data->Func) {
    case XFP_FUNC:/*光模块*/
        MOD_RP[sys_num].XFP_RP.att = data->Data[0];
        break;
    case PA_FUNC:   /*功放模块*/
        break;
    case MPA_FUNC:  /*微放模块*/
        if(data->Addr & 0x08) {
            MOD_RP[sys_num].CHECK_RP.UL_PA1_ATT = data->Data[0] & 0x7F;
            SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_ATT = data->Data[0] & 0x7F;
            MOD_RP[sys_num].UL_MPA.att = data->Data[0] & 0x7F;
        } else {
            MOD_RP[sys_num].CHECK_RP.DL_PA1_ATT = data->Data[0] & 0x7F;
            SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_ATT = data->Data[0] & 0x7F;
            MOD_RP[sys_num].DL_MPA.att = data->Data[0] & 0x7F;
        }
        break;
    case LNA_FUNC:  /*低噪模块*/
        if(data->Addr & 0x08) {
            if(SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ODU_Online == 0x01) {	//ODU模块
                MOD_RP[sys_num].CHECK_RP.UL_PA1_ATT = data->Data[0] & 0x7F;
                SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_ATT = data->Data[0] & 0x7F;
            } else {
                MOD_RP[sys_num].CHECK_RP.UL_LNA_ATT = data->Data[0] & 0x7F;
                SYS_Var[sys_num].RF_SYS.GA_SW.UL_LNA_ATT = data->Data[0] & 0x7F;
            }
        } else {
            MOD_RP[sys_num].CHECK_RP.DL_LNA_ATT = data->Data[0] & 0x7F;
            SYS_Var[sys_num].RF_SYS.GA_SW.DL_LNA_ATT = data->Data[0] & 0x7F;
        }
        break;
    case PUSH_FUNC:  /*合路推动模块*/
        if(SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ODU_Online == 0x01) {	//ODU模块
            if(data->Addr & 0x08) {
                MOD_RP[sys_num].CHECK_RP.UL_PA1_ATT = data->Data[0] & 0x7F;
                SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_ATT = data->Data[0] & 0x7F;
            } else {
                MOD_RP[sys_num].CHECK_RP.DL_PA1_ATT = data->Data[0] & 0x7F;
                SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_ATT = data->Data[0] & 0x7F;
            }
        } else {
            if(data->Addr & 0x08) {
                MOD_RP[sys_num].CHECK_RP.UL_PA1_ATT = data->Data[0] & 0x7F;
                SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_ATT = data->Data[0] & 0x7F;
            } else {
                MOD_RP[sys_num].CHECK_RP.DL_LNA_ATT = data->Data[0] & 0x7F;
                SYS_Var[sys_num].RF_SYS.GA_SW.DL_LNA_ATT = data->Data[0] & 0x7F;
            }
        }
        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*	函 数 名: void SETFREQ_RP(Rs485Comm *data)
*	功能说明: 设置频率命令/命令回应的命令数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SETFREQ_RP(Rs485Comm *data)
{
    u8 sys_num/*,sys_sel,submod*/;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
    //sys_sel = (data->Addr & 0xf0)/SEL_ADDR_SKIP - SYS_ADDR_BASE ; /*选频选带模块的地址区分与其它模块的地址区分不一致*/
    //submod  = data->Addr & 0x07;                                  /*选频选带模块的子地址*/

    switch(data->Func) {
    case BAND1_FUNC:    /*1选频模块*/
    case BAND2_FUNC:    /*2选频模块*/
    case BAND4_FUNC:		/*4选频模块*/
        if(data->Addr & 0x08) { /*上行*/
            /*单选频模块设置频率时返回的频率为0*/
            ChangeFreqSucceed = 1;

            /*memcpy((u8*)&MOD_RP[sys_sel].UL_FREQ[submod].CHN,data->Data,data->Clen);
            if((data->Func == BAND1_FUNC)  || (data->Func == BAND2_FUNC))
              {
            SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod*2] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[0].b,UL,sys_sel);
            if(data->Func == BAND2_FUNC)
              	  SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod*2 + 1 ] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[1].b,UL,sys_sel);
              }
            if(data->Func == BAND4_FUNC)
              {
               if(submod >= 2)
              return;
               SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod*4] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[0].b,UL,sys_sel);
              	SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod*4 + 1 ] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[1].b,UL,sys_sel);
            SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod*4 + 2 ] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[2].b,UL,sys_sel);
            SYS_Var[sys_sel].RF_SYS.SAVE_CHN.UL_CHN[submod*4 + 3] = GET_CHN(MOD_RP[sys_sel].UL_FREQ[submod].FreqArr[3].b,UL,sys_sel);
              }
              */

        } else {
            ChangeFreqSucceed = 1;
            /*memcpy((u8*)&MOD_RP[sys_sel].DL_FREQ[submod].CHN,data->Data,data->Clen);
            if((data->Func == BAND1_FUNC)  || (data->Func == BAND2_FUNC))
              {
            SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod*2] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[0].b,DL,sys_sel);
            if(data->Func == BAND2_FUNC)
              	  SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod*2 + 1 ] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[1].b,DL,sys_sel);
              }
            if(data->Func == BAND4_FUNC)
              {
               if(submod >= 2)
              return;
               SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod*4] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[0].b,DL,sys_sel);
              	SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod*4 + 1 ] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[1].b,DL,sys_sel);
            SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod*4 + 2 ] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[2].b,DL,sys_sel);
            SYS_Var[sys_sel].RF_SYS.SAVE_CHN.DL_CHN[submod*4 + 3 ] = GET_CHN(MOD_RP[sys_sel].DL_FREQ[submod].FreqArr[3].b,DL,sys_sel);
              }*/
        }
        break;
    case MPA_FUNC:		/*微放模块*/
        if(data->Addr & 0x08) { /*上行*/
            if(data->Data[1] == 0x01)
                memcpy((u8 *)&MOD_RP[sys_num].UL_MPACH_RP.FREQArr[0], &data->Data[2], 4);
            else if(data->Data[1] == 0x02)
                memcpy((u8 *)&MOD_RP[sys_num].UL_MPACH_RP.FREQArr[1], &data->Data[2], 4);
        } else {
            if(data->Data[1] == 0x01)
                memcpy((u8 *)&MOD_RP[sys_num].DL_MPACH_RP.FREQArr[0], &data->Data[2], 4);
            else if(data->Data[1] == 0x02)
                memcpy((u8 *)&MOD_RP[sys_num].DL_MPACH_RP.FREQArr[1], &data->Data[2], 4);
        }
        break;
    case ICS_FUNC:   /*京奥公司ICS模块*/
    case DIG_PICO:   /*京奥公司数字PICO模块*/
        if(data->Data[1] == 0x01) {
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.F1FREQ, &data->Data[2], 4);
            MOD_RP[sys_num].ICS_HT_RP.F1FREQ.a += 0.0005;
        } else if(data->Data[1] == 0x02) {
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.F2FREQ, &data->Data[2], 4);
            MOD_RP[sys_num].ICS_HT_RP.F2FREQ.a += 0.0005;
        }
        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*	函 数 名: void SETSW_RP(Rs485Comm *data)
*	功能说明: 设置功放开关信道开关命令/命令回应的命令数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SETSW_RP(Rs485Comm *data)
{
    u8 sys_num, sys_sel, submod;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
    sys_sel = (data->Addr & 0xf0) / SEL_ADDR_SKIP - SYS_ADDR_BASE ; /*选频选带模块的地址区分与其它模块的地址区分不一致*/
    submod  = data->Addr & 0x07;                                  /*选频选带模块的子地址*/
    switch(data->Func) {
    case BAND1_FUNC:    /*1选频模块*/
    case BAND2_FUNC:    /*2选频模块*/
    case BAND4_FUNC:		/*4选频模块*/
        if(data->Addr & 0x08) { /*上行*/
            if((SYS_Var[sys_sel].RF_SYS.MOD_ONLINE.FREQ4.SubAddr.UL_EN[submod] == 0x01)) {	/* 4选频模块1占用1 2 两个信道开关*/
                SYS_Var[sys_sel].RF_SYS.GA_SW.UL_BAND_SW[submod]	= data->Data[1];
                SYS_Var[sys_sel].RF_SYS.GA_SW.UL_BAND_SW[submod + 1]	= data->Data[1];
            } else if((SYS_Var[sys_sel].RF_SYS.MOD_ONLINE.FREQ4.SubAddr.UL_EN[submod] == 0x01)) { /* 4选频模块2占用3 4 两个信道开关*/
                SYS_Var[sys_sel].RF_SYS.GA_SW.UL_BAND_SW[submod + 1]	= data->Data[1];
                SYS_Var[sys_sel].RF_SYS.GA_SW.UL_BAND_SW[submod + 2]	= data->Data[1];
            } else {
                SYS_Var[sys_sel].RF_SYS.GA_SW.UL_BAND_SW[submod]	= data->Data[1];
            }
            // if(data->Data[0] == 0x01)
            //     SPI_Flash_Write(SYS_Var[sys_sel].Base_Addr, (u8 *)&SYS_Var[sys_sel], sizeof(SYS_Var[sys_sel]));
        } else {
            if((SYS_Var[sys_sel].RF_SYS.MOD_ONLINE.FREQ4.SubAddr.DL_EN[submod] == 0x01)) {	/* 4选频模块1占用1 2 两个信道开关*/
                SYS_Var[sys_sel].RF_SYS.GA_SW.DL_BAND_SW[submod]	= data->Data[1];
                SYS_Var[sys_sel].RF_SYS.GA_SW.DL_BAND_SW[submod + 1]	= data->Data[1];
            } else if((SYS_Var[sys_sel].RF_SYS.MOD_ONLINE.FREQ4.SubAddr.DL_EN[submod] == 0x01)) { /* 4选频模块2占用3 4 两个信道开关*/
                SYS_Var[sys_sel].RF_SYS.GA_SW.DL_BAND_SW[submod + 1]	= data->Data[1];
                SYS_Var[sys_sel].RF_SYS.GA_SW.DL_BAND_SW[submod + 2]	= data->Data[1];
            } else {
                SYS_Var[sys_sel].RF_SYS.GA_SW.DL_BAND_SW[submod]	= data->Data[1];
            }
            // if(data->Data[0] == 0x01)
            //     SPI_Flash_Write(SYS_Var[sys_sel].Base_Addr, (u8 *)&SYS_Var[sys_sel], sizeof(SYS_Var[sys_sel]));
        }
        break;
    case PA_FUNC:   /*功放模块*/
        break;	//20160418 防止胜华功放在设置sw时，不回传sw值，导致开关值出现错误
    case MPA_FUNC:  /*微放模块*/
        if(data->Addr & 0x08) { /*上行*/
            MOD_RP[sys_num].CHECK_RP.UL_PA1_SW = data->Data[0];
            SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_SW = data->Data[0];
        } else {
            MOD_RP[sys_num].CHECK_RP.DL_PA1_SW = data->Data[0];
            SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_SW = data->Data[0];
        }
        break;
    default:
        break;
    }
}

/*
*********************************************************************************************************
*	函 数 名: void SETGA_RP(Rs485Comm *data)
*	功能说明: 设置过功率告警门限命令/命令回应的命令数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SETGA_RP(Rs485Comm *data)
{
    u8 sys_num;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/

    switch(data->Func) {
    case PA_FUNC:   /*功放模块*/
    case MPA_FUNC:  /*微放模块*/
        if(data->Addr & 0x08) { /*上行*/
            MOD_RP[sys_num].CHECK_RP.UL_PO_GA1 = data->Data[1];
            //SYS_Var[sys_num].RF_SYS.GA_SW.UL_PO_GA1= data->Data[1];
        } else {
            MOD_RP[sys_num].CHECK_RP.DL_PO_GA1 = data->Data[1];
            //SYS_Var[sys_num].RF_SYS.GA_SW.DL_PO_GA1= data->Data[1];
        }
        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*	函 数 名: void SETAGCGA_RP(Rs485Comm *data)
*	功能说明: 设置AGC门限命令/命令回应的命令数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SETAGCGA_RP(Rs485Comm *data)
{
    u8 sys_num;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/

    switch(data->Func) {
    case PA_FUNC:   /*功放模块*/
        if(data->Addr & 0x08) { /*上行*/
            MOD_RP[sys_num].CHECK_RP.UL_PO_GA1 = data->Data[1];
        } else {
            MOD_RP[sys_num].CHECK_RP.DL_PO_GA1 = data->Data[1];
        }
        break;
    case MPA_FUNC:  /*微放模块*/
        if(data->Addr & 0x08) { /*上行*/
        } else {
            MOD_RP[sys_num].MPAPT_RP.DL_OVER_PT = data->Data[1];
        }
        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*	函 数 名: void UNDERPT_RP(Rs485Comm *data)
*	功能说明: 查询设置欠功率告警门限命令/命令回应的命令数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void UNDERPT_RP(Rs485Comm *data)
{
    u8 sys_num;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/

    switch(data->Func) {
    case MPA_FUNC:  /*微放模块*/
        if(data->Addr & 0x08) { /*上行*/
        } else {
            MOD_RP[sys_num].MPAPT_RP.DL_UNDER_PT = data->Data[1];
        }
        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*	函 数 名: void SETAGCSW_RP(Rs485Comm *data)
*	功能说明: 查询设置自动AGC开关命令/命令回应的命令数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SETAGCSW_RP(Rs485Comm *data)
{
    u8 sys_num;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/

    switch(data->Func) {
    case PA_FUNC:   /*功放模块*/
    case MPA_FUNC:  /*微放模块*/
        if(data->Addr & 0x08) { /*上行*/
            MOD_RP[sys_num].CHECK_RP.UL_AGC_SW = data->Data[1];
        } else {
            MOD_RP[sys_num].CHECK_RP.DL_AGC_SW = data->Data[1];
        }
        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*	函 数 名: void GETIP_RP(Rs485Comm *data)
*	功能说明: 查询功放及低噪的输入功率
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void GETIP_RP(Rs485Comm *data)
{
    u8 sys_num;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/

    switch(data->Func) {
    case PA_FUNC:   /*功放模块*/
        if(data->Addr & 0x08) { /*上行*/
            MOD_RP[sys_num].PWR_RP.UL_PA1_PI = data->Data[0];
        } else {
            MOD_RP[sys_num].PWR_RP.DL_PA1_PI = data->Data[0];
            if( ((MOD_RP[sys_num].PWR_RP.DL_PA1_PI < (MOD_RP[sys_num].AUTO_AGC.IPVar - 1)) || (MOD_RP[sys_num].PWR_RP.DL_PA1_PI > (MOD_RP[sys_num].AUTO_AGC.IPVar + 1)))
                    && (MOD_RP[sys_num].PWR_RP.DL_LNA_PI == -126) && (SYS_Var[sys_num].RF_SYS.GA_SW.DL_AGC_SW) && (!(SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Online)) ) {
                MOD_RP[sys_num].AUTO_AGC.IPVar = MOD_RP[sys_num].PWR_RP.DL_PA1_PI;
                MOD_RP[sys_num].AUTO_AGC.StartAgc = 0x03;
            }
        }
        break;
    case LNA_FUNC:   /*低噪模块*/
    case PUSH_FUNC:  /*推动模块*/
        if(data->Addr & 0x08) { /*上行*/
            MOD_RP[sys_num].PWR_RP.UL_LNA_PI = data->Data[0];
            if(MOD_RP[sys_num].PWR_RP.UL_LNA_PI != -127) {
                MOD_RP[sys_num].PWR_RP.UL_LNA_PI = MOD_RP[sys_num].PWR_RP.UL_LNA_PI + SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_LOSS;
            }
        } else {
            MOD_RP[sys_num].PWR_RP.DL_LNA_PI = data->Data[0];
            if(MOD_RP[sys_num].PWR_RP.DL_LNA_PI != -127) {
                MOD_RP[sys_num].PWR_RP.DL_LNA_PI = MOD_RP[sys_num].PWR_RP.DL_LNA_PI + SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_LOSS;
            }
            if( (MOD_RP[sys_num].AUTO_AGC.IPVar < (MOD_RP[sys_num].PWR_RP.DL_LNA_PI - 1)) || (MOD_RP[sys_num].AUTO_AGC.IPVar > (MOD_RP[sys_num].PWR_RP.DL_LNA_PI + 1))) {
                MOD_RP[sys_num].AUTO_AGC.IPVar = MOD_RP[sys_num].PWR_RP.DL_LNA_PI;
                MOD_RP[sys_num].AUTO_AGC.StartAgc = 0x01;
            }
        }
        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*	函 数 名: void GETOP_RP(Rs485Comm *data)
*	功能说明: 查询功放及低噪的输出功率
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void GETOP_RP(Rs485Comm *data)
{
    u8 sys_num;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/

    switch(data->Func) {
    case PA_FUNC:   /*功放模块*/
        if(data->Addr & 0x08) { /*上行*/
            MOD_RP[sys_num].PWR_RP.UL_PA1_PO = data->Data[0];
        } else {
            MOD_RP[sys_num].PWR_RP.DL_PA1_PO = data->Data[0];
        }
        break;
    case LNA_FUNC:   /*低噪模块*/
    case PUSH_FUNC:  /*推动模块*/
        if(data->Addr & 0x08) { /*上行*/
            MOD_RP[sys_num].PWR_RP.UL_LNA_PO = data->Data[0];
        } else {
            MOD_RP[sys_num].PWR_RP.DL_LNA_PO = data->Data[0];
        }
        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*	函 数 名: void SETICS_RP(Rs485Comm *data)
*	功能说明: 设置查询ICS相关信息
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SETICS_RP(Rs485Comm *data)
{
    u8 sys_num , i;
    u16 Temp16;

    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/

    switch(data->Func) {
    case ICS_FUNC:
        if(data->Data[1] == 0x32)/*工作信道*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.WK_CH, &data->Data[2], data->Data[2] * 2 + 1);
        else if(data->Data[1] == 0x4a)/*移频信道*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.FS_CH, &data->Data[2], data->Data[2] * 2 + 1);
        else if(data->Data[1] == 0x01)/*信道开关*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.CHSW, &data->Data[2], 4);
        else if(data->Data[1] == 0x62)/*上行静噪开关等信息*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.ULSL, &data->Data[1], sizeof(MOD_RP[sys_num].ICS_HT_RP.ULSL));
        else if(data->Data[1] == 0x67)/*下行静噪开关等信息*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.DLSL, &data->Data[1], sizeof(MOD_RP[sys_num].ICS_HT_RP.DLSL));
        else if(data->Data[1] == 0xff)/*模块类型*/
            MOD_RP[sys_num].ICS_HT_RP.QueryRP.ICSType = data->Data[2];
        else if(data->Data[1] == 0x64) { /* //设置上行手动ATT 输入过载门限*/
            MOD_RP[sys_num].ICS_HT_RP.ULSL.INOverGa  = data->Data[2];
            MOD_RP[sys_num].ICS_HT_RP.ULSL.ManualAtt = data->Data[3];
            MOD_RP[sys_num].ICS_HT_RP.ULSL.DVGASW    = data->Data[4];
        } else if(data->Data[1] == 0x69) { /* //设置下行手动ATT 输入过载门限*/
            MOD_RP[sys_num].ICS_HT_RP.DLSL.INOverGa  = data->Data[2];
            MOD_RP[sys_num].ICS_HT_RP.DLSL.ManualAtt = data->Data[3];
            MOD_RP[sys_num].ICS_HT_RP.DLSL.DVGASW    = data->Data[4];
        } else if(data->Data[1] == 0x8a) { /* //设置上行AGC门限值*/
            MOD_RP[sys_num].ICS_HT_RP.ULSL.CarrierAGCGA  = data->Data[2];
        } else if(data->Data[1] == 0x89) { /* //设置下行AGC门限值*/
            MOD_RP[sys_num].ICS_HT_RP.DLSL.CarrierAGCGA  = data->Data[2];
        } else if(data->Data[1] == 0x88) { /* //设置上行AGC开关*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.ULSL.AGCSW, &data->Data[2], 3);
        } else if(data->Data[1] == 0x87) { /* //设置下行AGC开关*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.DLSL.AGCSW, &data->Data[2], 3);
        } else if(data->Data[1] == 0x63) { /* //设置上行静噪开关,高低门限*/
            MOD_RP[sys_num].ICS_HT_RP.ULSL.slSW      = data->Data[2];
            MOD_RP[sys_num].ICS_HT_RP.ULSL.slHIGHGA  = data->Data[3];
            MOD_RP[sys_num].ICS_HT_RP.ULSL.slLOWGA   = data->Data[4];
        } else if(data->Data[1] == 0x68) { /* //设置下行静噪开关,高低门限*/
            MOD_RP[sys_num].ICS_HT_RP.DLSL.slSW      = data->Data[2];
            MOD_RP[sys_num].ICS_HT_RP.DLSL.slHIGHGA  = data->Data[3];
            MOD_RP[sys_num].ICS_HT_RP.DLSL.slLOWGA   = data->Data[4];
        } else if(data->Data[1] == 0x66) { /* //设置上行ICS开关,ICS整机增益*/
            MOD_RP[sys_num].ICS_HT_RP.ULSL.ICSSW      = data->Data[2];
            MOD_RP[sys_num].ICS_HT_RP.ULSL.ICSGain    = data->Data[3];
        } else if(data->Data[1] == 0x6b) { /* //设置下行ICS开关,ICS整机增益*/
            MOD_RP[sys_num].ICS_HT_RP.DLSL.ICSSW      = data->Data[2];
            MOD_RP[sys_num].ICS_HT_RP.DLSL.ICSGain    = data->Data[3];
        } else if(data->Data[1] == 0xe0) { /* 设置上行信道ATT*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.ch_ul_att, &data->Data[2], data->Data[2] * 2 + 1);
        } else if(data->Data[1] == 0xe1) { /* 设置下行信道ATT*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.ch_dl_att, &data->Data[2], data->Data[2] * 2 + 1);
        } else if(data->Data[1] == 0xe2) { /* 设置上行信道AGC*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.ch_ul_agc, &data->Data[2], data->Data[2] * 2 + 1);
        } else if(data->Data[1] == 0xe3) { /* 设置下行信道AGC*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.ch_dl_agc, &data->Data[2], data->Data[2] * 2 + 1);
        } else if(data->Data[1] == 0xe4) { /* 阻塞最大补偿值*/
            MOD_RP[sys_num].ICS_HT_RP.block_compensate = data->Data[2];
        }
        break;
    case DIG_PICO:
        if(data->Data[1] == 0x02) { /*下行信道*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.ULFREQ, &data->Data[2], data->Data[2] * 4 + 1);
            MOD_RP[sys_num].ICS_HT_RP.WK_CH.CHN = MOD_RP[sys_num].ICS_HT_RP.ULFREQ.CHN;
            for( i = 0; i < data->Data[2]; i++) {
                if(SYS_Var[sys_num].RF_SYS.MIMO_Enable == 0x01)
                    MOD_RP[sys_num].ICS_HT_RP.WK_CH.CH[i] = GET_CHN(MOD_RP[sys_num].ICS_HT_RP.ULFREQ.Arr[i].b, DL, SYS_Var[sys_num].RF_SYS.MIMO_Replace);
                else
                    MOD_RP[sys_num].ICS_HT_RP.WK_CH.CH[i] = GET_CHN(MOD_RP[sys_num].ICS_HT_RP.ULFREQ.Arr[i].b, DL, sys_num);
            }
        }

        else if(data->Data[1] == 0x06)/*信道开关*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.CHSW, &data->Data[2], 4);
        else if(data->Data[1] == 0x30) { /*上行静噪开关等信息*/
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[2], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.ULSL.AGCDepth = Temp16 / 16;
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[4], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.ULSL.INOverGa = Temp16 / 16;
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[6], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.ULSL.ManualAtt = Temp16 / 16;
            MOD_RP[sys_num].ICS_HT_RP.ULSL.ICSSW = data->Data[8];
            MOD_RP[sys_num].ICS_HT_RP.ULSL.slHIGHGA = data->Data[9];
            MOD_RP[sys_num].ICS_HT_RP.ULSL.slLOWGA = data->Data[10];
            MOD_RP[sys_num].ICS_HT_RP.ULSL.slSW = data->Data[11];
        } else if(data->Data[1] == 0x31) { /*下行静噪开关等信息*/
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[2], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.DLSL.AGCDepth = Temp16 / 16;
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[4], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.DLSL.INOverGa = Temp16 / 16;
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[6], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.DLSL.ManualAtt = Temp16 / 16;
            MOD_RP[sys_num].ICS_HT_RP.DLSL.ICSSW = data->Data[8];
            MOD_RP[sys_num].ICS_HT_RP.DLSL.slHIGHGA = data->Data[9];
            MOD_RP[sys_num].ICS_HT_RP.DLSL.slLOWGA = data->Data[10];
            MOD_RP[sys_num].ICS_HT_RP.DLSL.slSW = data->Data[11];
        } else if(data->Data[1] == 0x32) /*模块类型*/
            MOD_RP[sys_num].ICS_HT_RP.QueryRP.ICSType = data->Data[2];

        else if(data->Data[1] == 0x07) { /*上行AGC门限*/
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[2], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.ULSL.AGCDepth = Temp16 / 16;
        } else if(data->Data[1] == 0x08) { /*下行AGC门限*/
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[2], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.DLSL.AGCDepth = Temp16 / 16;
        } else if(data->Data[1] == 0x09) { /*上行输入过载门限*/
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[2], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.ULSL.INOverGa = Temp16 / 16;
        } else if(data->Data[1] == 0x0a) { /*下行输入过载门限*/
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[2], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.DLSL.INOverGa = Temp16 / 16;
        } else if(data->Data[1] == 0x0b) { /*上行ATT*/
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[2], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.ULSL.ManualAtt = Temp16 / 16;
        } else if(data->Data[1] == 0x0c) { /*下行ATT*/
            memcpy((u8 *)&Temp16, (u8 *)&data->Data[2], 2);
            ByteSwap((u8 *)&Temp16);
            MOD_RP[sys_num].ICS_HT_RP.DLSL.ManualAtt = Temp16 / 16;
        } else if(data->Data[1] == 0x0d) { /*上行ICS开关*/
            MOD_RP[sys_num].ICS_HT_RP.ULSL.ICSSW = data->Data[2];
        } else if(data->Data[1] == 0x0e) { /*下行ICS开关*/
            MOD_RP[sys_num].ICS_HT_RP.DLSL.ICSSW = data->Data[2];
        } else if(data->Data[1] == 0x0f) { /*上行静噪高低门限*/
            MOD_RP[sys_num].ICS_HT_RP.ULSL.slHIGHGA = data->Data[2];
            MOD_RP[sys_num].ICS_HT_RP.ULSL.slLOWGA = data->Data[3];
        } else if(data->Data[1] == 0x10) { /*下行静噪高低门限*/
            MOD_RP[sys_num].ICS_HT_RP.DLSL.slHIGHGA = data->Data[2];
            MOD_RP[sys_num].ICS_HT_RP.DLSL.slLOWGA = data->Data[3];
        } else if(data->Data[1] == 0x11) { /*上行静噪开关*/
            MOD_RP[sys_num].ICS_HT_RP.ULSL.slSW = data->Data[2];
        } else if(data->Data[1] == 0x12) { /*下行静噪开关*/
            MOD_RP[sys_num].ICS_HT_RP.DLSL.slSW = data->Data[2];
        } else if(data->Data[1] == 0x15) { /*下行每信道输入功率*/
            for( i = 0; i < data->Data[2]; i++) {
                memcpy((u8 *)&Temp16, (u8 *)&data->Data[3 + i * 2], 2);
                ByteSwap((u8 *)&Temp16);
                Temp16 = Temp16 / 16;
                MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLIP[i] = Temp16;
            }
        } else if(data->Data[1] == 0x16) { /*下行每信道输出功率*/
            for( i = 0; i < data->Data[2]; i++) {
                memcpy((u8 *)&Temp16, (u8 *)&data->Data[3 + i * 2], 2);
                ByteSwap((u8 *)&Temp16);
                Temp16 = Temp16 / 16;
                MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOP[i] = Temp16;
            }
        } else if(data->Data[1] == 0xe0) { /* 设置上行信道ATT*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.ch_ul_att, &data->Data[2], data->Data[2] * 2 + 1);
        } else if(data->Data[1] == 0xe1) { /* 设置下行信道ATT*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.ch_dl_att, &data->Data[2], data->Data[2] * 2 + 1);
        } else if(data->Data[1] == 0xe2) { /* 设置上行信道AGC*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.ch_ul_agc, &data->Data[2], data->Data[2] * 2 + 1);
        } else if(data->Data[1] == 0xe3) { /* 设置下行信道AGC*/
            memcpy((u8 *)&MOD_RP[sys_num].ICS_HT_RP.ch_dl_agc, &data->Data[2], data->Data[2] * 2 + 1);
        } else if(data->Data[1] == 0xe4) { /* 阻塞最大补偿值*/
            MOD_RP[sys_num].ICS_HT_RP.block_compensate = data->Data[2];
        }


        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*	函 数 名: void SETODU_RP(Rs485Comm *data)
*	功能说明: 设置查询ODU相关信息
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SETODU_RP(Rs485Comm *data)
{
    u8 sys_num;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/

    switch(data->Func) {
    case ODU_FUNC:
        if(data->Data[0] == 0x09) { //设置微波发送 接收 链路增益衰减
            SYS_Var[sys_num].RF_SYS.GA_SW.UL_LNA_ATT = data->Data[1] & 0x7F;
            SYS_Var[sys_num].RF_SYS.GA_SW.DL_LNA_ATT = data->Data[2] & 0x7F;
        }
        if(data->Data[0] == 0x05) { //设置微波发送 接收 信道频点
            ChangeFreqSucceed = 1;
        }
        break;
    default:
        break;
    }
}

/*
*********************************************************************************************************
*	函 数 名: void HEGETTOTALATT_RP(Rs485Comm *data)
*	功能说明: 当自动AGC由下行低噪实现时,查询整机ATT值,以便衰减除下行低噪外其它模块
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void HEGETTOTALATT_RP(Rs485Comm *data)
{
    u8 sys_num;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/

    switch(data->Func) {
    case LNA_FUNC:   /*低噪模块*/
        if(data->Addr & 0x08) { /*上行*/
        } else {
            if(MOD_RP[sys_num].AUTO_AGC.Total_Att != data->Data[0]) {
                MOD_RP[sys_num].AUTO_AGC.Total_Att = data->Data[0];
                MOD_RP[sys_num].AUTO_AGC.StartAgc = 0x01;
            }
        }
        break;
    default:
        break;
    }
}
#if BANDSW_ENABLE
/*
*********************************************************************************************************
*	函 数 名: void SET_BANDSW_RP(Rs485Comm *data)
*	功能说明: 设置信道开关命令/命令回应的命令数据,这是京奥公司扩展的.
单独信道开关使能
0x3d ：
      byte1：0x00- 查询 ；0x01-设置.
      byte2:开关 低四位有效 从低到高分别代表1 - 4 信道
注意:2选频或可变选带 模块地址0x00对应1信道 0x01对应2信道 0x02对应3信道 0x03对应4信道
     4选频或双可变选带中 模块地址0x00的1-2频点对应1信道，3-4频点对应2信道
     模块地址0x01的1-2频点对应3信道，3-4频点对应4信道
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SET_BANDSW_RP(Rs485Comm *data)
{
    u8 sys_sel, submod;
    //sys_num = (data->Addr & 0x07)/SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
    sys_sel = (data->Addr & 0xf0) / SEL_ADDR_SKIP - SYS_ADDR_BASE ; /*选频选带模块的地址区分与其它模块的地址区分不一致*/
    submod  = data->Addr & 0x07;
    switch(data->Func) {
    case BAND1_FUNC:    /*1选频模块*/
    case BAND2_FUNC:    /*2选频模块*/
    case BAND4_FUNC:		/*4选频模块*/
        if(data->Addr & 0x08) { /*上行*/
            if((SYS_Var[sys_sel].RF_SYS.MOD_ONLINE.FREQ4.SubAddr.UL_EN[submod] == 0x01)) {	/* 4选频模块1占用1 2 两个信道开关*/
                SYS_Var[sys_sel].RF_SYS.GA_SW.UL_BAND_SW[submod]	= data->Data[1] & 0x01;
                SYS_Var[sys_sel].RF_SYS.GA_SW.UL_BAND_SW[submod + 1]	= data->Data[1] & 0x02;
            } else if((SYS_Var[sys_sel].RF_SYS.MOD_ONLINE.FREQ4.SubAddr.UL_EN[submod] == 0x01)) { /* 4选频模块2占用3 4 两个信道开关*/
                SYS_Var[sys_sel].RF_SYS.GA_SW.UL_BAND_SW[submod + 1]	= data->Data[1] & 0x04;
                SYS_Var[sys_sel].RF_SYS.GA_SW.UL_BAND_SW[submod + 2]	= data->Data[1] & 0x08;
            } else {
                SYS_Var[sys_sel].RF_SYS.GA_SW.UL_BAND_SW[submod]	= data->Data[1];
            }
            if(data->Data[0] == 0x01)
                SPI_Flash_Write(SYS_Var[sys_sel].Base_Addr, (u8 *)&SYS_Var[sys_sel], sizeof(SYS_Var[sys_sel]));
        } else {
            if((SYS_Var[sys_sel].RF_SYS.MOD_ONLINE.FREQ4.SubAddr.DL_EN[submod] == 0x01)) {	/* 4选频模块1占用1 2 两个信道开关*/
                SYS_Var[sys_sel].RF_SYS.GA_SW.DL_BAND_SW[submod]	= data->Data[1] & 0x01;
                SYS_Var[sys_sel].RF_SYS.GA_SW.DL_BAND_SW[submod + 1]	= data->Data[1] & 0x02;
            } else if((SYS_Var[sys_sel].RF_SYS.MOD_ONLINE.FREQ4.SubAddr.DL_EN[submod] == 0x01)) { /* 4选频模块2占用3 4 两个信道开关*/
                SYS_Var[sys_sel].RF_SYS.GA_SW.DL_BAND_SW[submod + 1]	= data->Data[1] & 0x04;
                SYS_Var[sys_sel].RF_SYS.GA_SW.DL_BAND_SW[submod + 2]	= data->Data[1] & 0x08;
            } else {
                SYS_Var[sys_sel].RF_SYS.GA_SW.DL_BAND_SW[submod]	= data->Data[1];
            }
            if(data->Data[0] == 0x01)
                SPI_Flash_Write(SYS_Var[sys_sel].Base_Addr, (u8 *)&SYS_Var[sys_sel], sizeof(SYS_Var[sys_sel]));
        }
        break;
    default:
        break;
    }
}
#endif

#if READ_SN_ENABLE
/*
*********************************************************************************************************
*	函 数 名: void SET_SN_RP(Rs485Comm *data)
*	功能说明: 设置读取模块系列号命令/命令回应的命令数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SET_SN_RP(Rs485Comm *data)
{
    u8 sys_num, sys_sel/*,submod*/;
    u8 i, j;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ; /*除选频选带模块外的模块地址*/
    sys_sel = (data->Addr & 0xf0) / SEL_ADDR_SKIP - SYS_ADDR_BASE ; /*选频选带模块的地址区分与其它模块的地址区分不一致*/
    //submod  = data->Addr & 0x07;                                  /*选频选带模块的子地址*/

    switch(data->Func) {
    case BAND1_FUNC:
    case BAND2_FUNC:
    case BAND4_FUNC:
        for(i = 0; i < MAX_MOD_NUM; i++) {
            //如果监控板中已经存有模块系列就只更新,如果不存在,直接存入数组
            if((MOD_RP[sys_sel].RF_MOD_SN.MOD_SN[i].Func == data->Func) && (MOD_RP[sys_sel].RF_MOD_SN.MOD_SN[i].Addr == data->Addr))
                break;
        }
        if( i == MAX_MOD_NUM) {
            j = MOD_RP[sys_sel].RF_MOD_SN.Mod_Num;
            MOD_RP[sys_sel].RF_MOD_SN.Mod_Num += 1;
        } else
            j = i;
        MOD_RP[sys_sel].RF_MOD_SN.MOD_SN[j].Func = data->Func;
        MOD_RP[sys_sel].RF_MOD_SN.MOD_SN[j].Addr = data->Addr;
        memset(MOD_RP[sys_sel].RF_MOD_SN.MOD_SN[j].SN, 0x00, sizeof(MOD_RP[sys_sel].RF_MOD_SN.MOD_SN[j].SN));
        memcpy(MOD_RP[sys_sel].RF_MOD_SN.MOD_SN[j].SN, &data->Data[1], sizeof(MOD_RP[sys_sel].RF_MOD_SN.MOD_SN[j].SN));
        break;
    case PA_FUNC:
    case LNA_FUNC:
    case ICS_FUNC:
    case DIG_PICO:
    case XFP_FUNC:
    case PUSH_FUNC:
        for(i = 0; i < MAX_MOD_NUM; i++) {
            //如果监控板中已经存有模块系列就只更新,如果不存在,直接存入数组
            if((MOD_RP[sys_num].RF_MOD_SN.MOD_SN[i].Func == data->Func) && (MOD_RP[sys_num].RF_MOD_SN.MOD_SN[i].Addr == data->Addr))
                break;
        }
        if( i == MAX_MOD_NUM) {
            j = MOD_RP[sys_num].RF_MOD_SN.Mod_Num;
            MOD_RP[sys_num].RF_MOD_SN.Mod_Num += 1;
        } else
            j = i;
        MOD_RP[sys_num].RF_MOD_SN.MOD_SN[j].Func = data->Func;
        MOD_RP[sys_num].RF_MOD_SN.MOD_SN[j].Addr = data->Addr;
        memset(MOD_RP[sys_num].RF_MOD_SN.MOD_SN[j].SN, 0x00, sizeof(MOD_RP[sys_num].RF_MOD_SN.MOD_SN[j].SN));
        memcpy(MOD_RP[sys_num].RF_MOD_SN.MOD_SN[j].SN, &data->Data[1], sizeof(MOD_RP[sys_num].RF_MOD_SN.MOD_SN[j].SN));
        break;
    default:
        break;

    }

}
#endif

/*
*********************************************************************************************************
*	函 数 名: void SETSKIP_RP(Rs485Comm *data)
*	功能说明: 设置修正步进命令/命令回应的命令数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SETSKIP_RP(Rs485Comm *data)
{
    u8 sys_sel, submod;
    sys_sel = (data->Addr & 0xf0) / SEL_ADDR_SKIP - SYS_ADDR_BASE ; /*选频选带模块的地址区分与其它模块的地址区分不一致*/
    submod  = data->Addr & 0x07;                                  /*选频选带模块的子地址*/
    switch(data->Func) {
    case BAND1_FUNC:        /*1选频模块*/
    case BAND2_FUNC:        /*2选频模块*/
    case BAND4_FUNC:		/*4选频模块*/
        if(data->Addr & 0x08) { /*上行*/
            if(data->Func == BAND2_FUNC)
                submod = submod * 2;
            else if(data->Func == BAND4_FUNC)
                submod = submod * 4;
            MOD_RP[sys_sel].SKIP_RP.UL_SKIP[submod + data->Data[1] - 1] = data->Data[2];
            //memcpy((u8*)&MOD_RP[sys_sel].SKIP_RP.UL_SKIP[submod + data->Data[1] - 1],&data->Data[2],1);
        } else {
            if(data->Func == BAND2_FUNC)
                submod = submod * 2;
            else if(data->Func == BAND4_FUNC)
                submod = submod * 4;
            MOD_RP[sys_sel].SKIP_RP.DL_SKIP[submod + data->Data[1] - 1] = data->Data[2];
            //memcpy((u8*)&MOD_RP[sys_sel].SKIP_RP.DL_SKIP[submod + data->Data[1] - 1],&data->Data[2],1);
        }
        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*	函 数 名: u8 Mod_RP_Handle(u8 *SendBuf,u8 *RecvBuf)
*	功能说明: 底层模块回应数据的处理
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
u8 Mod_RP_Handle(u8 *SendBuf, u8 *RecvBuf, u8 RecvLen)
{
    u8 i, *begin;
    Rs485Comm *Sstr, *Rstr;

    Sstr = (Rs485Comm *)&SendBuf[1];
    for( i = 0; i < RecvLen; i++ ) {
        if(RecvBuf[i] == SOFD)
            break;
    }
    if( i == RecvLen)//找不到协议起始标志
        return 0;
    Rstr = (Rs485Comm *)&RecvBuf[i + 1];
    begin = &RecvBuf[i + 1];

    for( i = RecvLen; i > 0; i--) {
        if(RecvBuf[i] == EOM)
            break;
    }
    if(i < 8)    //RS485最小协议包长度不小于8个字节或者未找到协议结束标志
        return 0;

    if(begin[Rstr->Clen + 7] != EOM)
        return 0;
    if(Rstr->Ack != 0x00)	//应答标志错
        return 0;

    if((Rstr->Func != Sstr->Func) || (Rstr->Addr != Sstr->Addr) || (Rstr->Cmd != Sstr->Cmd))	//模块功能,模块地址,命令编号不一致
        return 0;

    switch(Rstr->Cmd) {
    case QUERY:          /*模块状态查询*/
        Query_RP(Rstr);
        break;
    case SETATT:         /*设置ATT*/
        SETATT_RP(Rstr);
        break;
    case SETFREQ:         /*设置频率*/
        SETFREQ_RP(Rstr);
        break;
    case SETSW:         /*设置功放及信道开关*/
        SETSW_RP(Rstr);
        break;
    case SETGA:        /*设置功率门限*/
    case GETOVERPT:    /*获取功率门限*/
        SETGA_RP(Rstr);
        break;
    case SETAGCGA:     /*设置AGC门限*/
        SETAGCGA_RP(Rstr);
        break;
    case GETUNDERPT:     /*查询欠功率门限值*/
    case SETUNDERPT:     /*设置过功率门限值*/
        UNDERPT_RP(Rstr);
        break;
    case SETAGCSW:       /*设置查询自动AGC开关*/
        SETAGCSW_RP(Rstr);
        break;
    case GETIP:          /*获取输入功率*/
        GETIP_RP(Rstr);
        break;
    case GETOP:          /*获取输出功率*/
        GETOP_RP(Rstr);
        break;
    case HEGETTOTALATT:  /*获取整机总ATT值*/
        HEGETTOTALATT_RP(Rstr);
        break;
    case SETICS:
        SETICS_RP(Rstr);       /*与ICS相关设置*/
        break;
#if BANDSW_ENABLE
    case 0x3d:
        SET_BANDSW_RP(Rstr);/*设置单独子带开关*/
        break;
#endif

    case SETSN:
        SET_SN_RP(Rstr);      /*读取,设置模块系列号*/
        break;

    case 0x91:
        SETODU_RP(Rstr);/*与ODU相关设置*/
        break;
    default:
        break;
    }
    //old protocol transform new protocol
    protocol_old2new(Rstr);

    return 1;
}

/*
*********************************************************************************************************
*	函 数 名: void  ClearModStatus(void *pdata)
*	功能说明: 如果查询不到模块,清除模块状态到默认值
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void  ClearModStatus(CheckComm *data)
{
    u8 sys_num;
    sys_num = (data->Addr & 0x07) / SYS_ADDR_SKIP - SYS_ADDR_BASE ;

    if(SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ODU_Online == 0x01)
        return;
    /*功放模块*/
    if((data->Func == PA_FUNC) && ((data->Addr & 0x08) == UL) && (data->Cmd == QUERY) && (!Search_Module_Online(PUSH_FUNC, DL + sys_num))) {
        SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_SW  = OFF;
        SYS_Var[sys_num].RF_SYS.GA_SW.UL_PA1_ATT = 0;
        MOD_RP[sys_num].UL_PA1.tp = 0x00;
        MOD_RP[sys_num].PWR_RP.UL_PA1_PO = -126;
        MOD_RP[sys_num].PWR_RP.UL_PA1_PI = -126;
        if((ICS_HT_FUNC == DIG_PICO) && (MOD_RP[sys_num].PWR_RP.DL_PA1_PI == -126))
            MOD_RP[sys_num].ICS_HT_RP.PICO_ENABLE = 1;
    }
    if((data->Func == PA_FUNC) && ((data->Addr & 0x08) == DL) && (data->Cmd == QUERY) && (!Search_Module_Online(PUSH_FUNC, DL + sys_num))) {
        SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_SW  = OFF;
        SYS_Var[sys_num].RF_SYS.GA_SW.DL_PA1_ATT = 0;
        MOD_RP[sys_num].DL_PA1.tp = 0x00;
        MOD_RP[sys_num].PWR_RP.DL_PA1_PO = -126;
        MOD_RP[sys_num].PWR_RP.DL_PA1_PI = -126;
        if((ICS_HT_FUNC == DIG_PICO) && (MOD_RP[sys_num].PWR_RP.UL_PA1_PI == -126))
            MOD_RP[sys_num].ICS_HT_RP.PICO_ENABLE = 1;
    }
    /*底噪模块*/
    if((data->Func == LNA_FUNC) && ((data->Addr & 0x08) == UL) && (data->Cmd == QUERY) && (!Search_Module_Online(PUSH_FUNC, DL + sys_num))) {
        SYS_Var[sys_num].RF_SYS.GA_SW.UL_LNA_ATT = 0;
        MOD_RP[sys_num].PWR_RP.UL_LNA_PO = -126;
        MOD_RP[sys_num].PWR_RP.UL_LNA_PI = -126;
    }
    if((data->Func == LNA_FUNC) && ((data->Addr & 0x08) == DL) && (data->Cmd == QUERY) && (!Search_Module_Online(PUSH_FUNC, DL + sys_num))) {
        SYS_Var[sys_num].RF_SYS.GA_SW.DL_LNA_ATT = 0;
        MOD_RP[sys_num].PWR_RP.DL_LNA_PO = -126;
        MOD_RP[sys_num].PWR_RP.DL_LNA_PI = -126;
    }
    /*ICS模块*/
    if((data->Func == ICS_HT_FUNC) && ((data->Addr & 0x08) == DL) && (data->Cmd == QUERY)) {
        MOD_RP[sys_num].ICS_HT_RP.ULSL.ManualAtt = 0;
        MOD_RP[sys_num].ICS_HT_RP.DLSL.ManualAtt = 0;
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULInputP = -126;
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLInputP = -126;
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.ULOutputP = -126;
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOutputP = -126;
        memset((u8 *)&MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOP, 0x00, sizeof(MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLOP));
        memset((u8 *)&MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLIP, 0x00, sizeof(MOD_RP[sys_num].ICS_HT_RP.QueryRP.DLIP));
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.ERL[0] = 0x00;
        MOD_RP[sys_num].ICS_HT_RP.QueryRP.ERL[1] = 0x00;
    }
}

u8 get_rs485_mod_init_state()
{
    return DelayFlag;
}

/*
*********************************************************************************************************
*	函 数 名: void  RS485Task(void *arg)
*	功能说明: RS485主任务,用于轮询,设置底层各模块的信息
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void *rs485_thread(void *arg)
{
    u8 salan_check_flag = 0;
    u16 RcvUpdateIndex = 0;
    u8 i;
    DelayFlag = 0;
    for( i = 0; i < SYS_NUM; i++) {
        SYS_Var[i].RF_SYS.MOD_ONLINE.ICS.Num = 6;
    }
    if(rs485_tty_open()) {
        RLDEBUG("rs485 open tty faild\r\n");
        return ;
    }

    //初始化旧地址转BAND
    oldsys_band_table_init();

    sem_init(&(SetSem.SetSem), 0, 0);

    timedelay(0, 1, 0, 0); //差不多50秒后再开始查询模块状态

    Only_Check_Module_Online();

    Check_Module_Online();
    DelayFlag = 1;
    while(1) {
        /*在远程升级时不进行485轮询*/
        // if(RcvUpdateIndex != RecvPackIndex) {
        //     RcvUpdateIndex = RecvPackIndex;
        //     timedelay(0, 30, 0, 0);
        //     continue;
        // }

        Auto_AGC_Set();

        if(SetAttArr.SetNum) { /*自动AGC时设置ATT*/
            memset(RSSendBuf, 0x00, sizeof(RSSendBuf));
            RSSendLen = FillSetData(RSSendBuf, (_SET_STRUCT *)&SetAttArr.Arr[SetAttArr.SetNumAdd]);
            RS485_SEND(RSSendBuf, RSSendLen);
            //memset(RSRecvBuf,0x00,sizeof(RSRecvBuf));
            RSRecvLen = RS485_RECV(RSRecvBuf, sizeof(RSRecvBuf), SETTIME);
            if(RSRecvLen != 0x00) {
                Mod_RP_Handle(RSSendBuf, RSRecvBuf, RSRecvLen);
            }
            /*ＷＣＤＭＡ使用别公司的下行功放上行底噪模块，在结尾会多一个0xff影响下一个模块的通信，在这里先查询一次*/
            if((SetAttArr.Arr[SetAttArr.SetNumAdd].Func == PA_FUNC) && (SetAttArr.Arr[SetAttArr.SetNumAdd].Addr == 0x01)
                    || (SetAttArr.Arr[SetAttArr.SetNumAdd].Func == LNA_FUNC) && (SetAttArr.Arr[SetAttArr.SetNumAdd].Addr == 0x09)) {
                RSSendLen = FillPollData(RSSendBuf, (CheckComm *)&InvalidCmd, 0x01);
                RS485_SEND(RSSendBuf, RSSendLen);
                //memset(RSRecvBuf,0x00,sizeof(RSRecvBuf));
                RSRecvLen = RS485_RECV(RSRecvBuf, sizeof(RSRecvBuf), GETTIME);
            }
            SetAttArr.SetNumAdd ++;
            //OS_ENTER_CRITICAL();
            if(SetAttArr.SetNumAdd >= SetAttArr.SetNum ) {
                SetAttArr.SetNumAdd = 0x00;
                SetAttArr.SetNum    = 0x00;
            }
            //OS_EXIT_CRITICAL();

        }
        if(SetSem.SetSemFlag) { /*通过信号量设置模块*/
            memset(RSSendBuf, 0x00, sizeof(RSSendBuf));
            RSSendLen = FillSetData(RSSendBuf, &SetSem.SET_STRUCT[SetSem.SetSemFlag - 1]);

            //salan add at 20151031
            if((PA_FUNC == SetSem.SET_STRUCT[SetSem.SetSemFlag - 1].Func) && (SETATT == SetSem.SET_STRUCT[SetSem.SetSemFlag - 1].Cmd)) {
                salan_check_flag = 1;
            }
            //the end of add

            RS485_SEND(RSSendBuf, RSSendLen);
            //memset(RSRecvBuf,0x00,sizeof(RSRecvBuf));
            RSRecvLen = RS485_RECV(RSRecvBuf, sizeof(RSRecvBuf), SETTIME);

            //salan add at 20151031
            if(salan_check_flag) {
                SetSem.SET_STRUCT[SetSem.SetSemFlag - 1].Cmd = QUERY;
                SetSem.SET_STRUCT[SetSem.SetSemFlag - 1].Clen = 0;

                memset(RSSendBuf, 0x00, sizeof(RSSendBuf));
                RSSendLen = FillSetData(RSSendBuf, &SetSem.SET_STRUCT[SetSem.SetSemFlag - 1]);
                RS485_SEND(RSSendBuf, RSSendLen);
                //memset(RSRecvBuf,0x00,sizeof(RSRecvBuf));
                RSRecvLen = RS485_RECV(RSRecvBuf, sizeof(RSRecvBuf), SETTIME);
                salan_check_flag = 0;
            }
            //the end of add

            if(RSRecvLen != 0x00) {
                if(RSRecvBuf[0] != 0x7e) {
                    memcpy(MPA_RcvBuf, (u8 *)&RSRecvBuf[1], RSRecvLen - 1);
                    MPA_RcvLen = RSRecvLen - 1;
                } else {
                    memcpy(MPA_RcvBuf, RSRecvBuf, RSRecvLen);
                    MPA_RcvLen = RSRecvLen;
                }
                Mod_RP_Handle(RSSendBuf, RSRecvBuf, RSRecvLen);
            }
            SetSem.SetSemFlag = SetSem.SetSemFlag - 1;
            sem_post(&(SetSem.SetSem));
            /*ＷＣＤＭＡ使用别公司的下行功放上行底噪模块，在结尾会多一个0xff影响下一个模块的通信，在这里先查询一次*/
            if((SetSem.SET_STRUCT[SetSem.SetSemFlag].Func == PA_FUNC) && (SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr == 0x01)
                    || (SetSem.SET_STRUCT[SetSem.SetSemFlag].Func == LNA_FUNC) && (SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr == 0x09)) {
                RSSendLen = FillPollData(RSSendBuf, (CheckComm *)&InvalidCmd, 0x01);
                RS485_SEND(RSSendBuf, RSSendLen);
                //memset(RSRecvBuf,0x00,sizeof(RSRecvBuf));
                RSRecvLen = RS485_RECV(RSRecvBuf, sizeof(RSRecvBuf), GETTIME);
            }
        }//end if(SetSem.SetSemFlag)/*通过信号量设置模块*/
        else if(SetModArr.SetNum) { /*通过设置数组设置模块*/
            memset(RSSendBuf, 0x00, sizeof(RSSendBuf));
            RSSendLen = FillSetData(RSSendBuf, &SetModArr.Arr[SetModArr.SetNumAdd]);
            RS485_SEND(RSSendBuf, RSSendLen);
            //memset(RSRecvBuf,0x00,sizeof(RSRecvBuf));
            RSRecvLen = RS485_RECV(RSRecvBuf, sizeof(RSRecvBuf), SETTIME);
            if(RSRecvLen != 0x00) {
                Mod_RP_Handle(RSSendBuf, RSRecvBuf, RSRecvLen);
            }
            /*ＷＣＤＭＡ使用别公司的下行功放上行底噪模块，在结尾会多一个0xff影响下一个模块的通信，在这里先查询一次*/
            if((SetModArr.Arr[SetModArr.SetNumAdd].Func == PA_FUNC) && (SetModArr.Arr[SetModArr.SetNumAdd].Addr == 0x01)
                    || (SetModArr.Arr[SetModArr.SetNumAdd].Func == LNA_FUNC) && (SetModArr.Arr[SetModArr.SetNumAdd].Addr == 0x09)) {
                RSSendLen = FillPollData(RSSendBuf, (CheckComm *)&InvalidCmd, 0x01);
                RS485_SEND(RSSendBuf, RSSendLen);
                //memset(RSRecvBuf,0x00,sizeof(RSRecvBuf));
                RSRecvLen = RS485_RECV(RSRecvBuf, sizeof(RSRecvBuf), GETTIME);
            }
            SetModArr.SetNumAdd ++;
            //OS_ENTER_CRITICAL();
            if(SetModArr.SetNumAdd >= SetModArr.SetNum ) {
                SetModArr.SetNumAdd = 0x00;
                SetModArr.SetNum    = 0x00;
            }
            //OS_EXIT_CRITICAL();
        }//end else if(SetModArr.SetNum)//通过设置数组设置模块
        else if(CheckCommArr.PollAdd < CheckCommArr.CheckNum ) { /*没有设置事件就轮询模块状态*/
            memset(RSSendBuf, 0x00, sizeof(RSSendBuf));
            if( (RSSendLen = FillPollData(RSSendBuf, &CheckCommArr.Arr[CheckCommArr.PollAdd], 0x00)) == -1) {
                if(CheckCommArr.Arr[CheckCommArr.PollAdd].Online >= OFFLINENUM)
                    ClearModStatus(&CheckCommArr.Arr[CheckCommArr.PollAdd]);
                CheckCommArr.PollAdd++;
                timedelay(0, 0, 100, 0);
            } else {
                RS485_SEND(RSSendBuf, RSSendLen);
                //memset(RSRecvBuf,0x00,sizeof(RSRecvBuf));
                RSRecvLen = RS485_RECV(RSRecvBuf, sizeof(RSRecvBuf), GETTIME );
                if(RSRecvLen >= 0x07) {
                    CheckCommArr.Arr[CheckCommArr.PollAdd].Online = 0;
                    Mod_RP_Handle(RSSendBuf, RSRecvBuf, RSRecvLen);
                } else {
                    CheckCommArr.Arr[CheckCommArr.PollAdd].Online += 1;    //如果查询无返回值,模块不在线次数自加1
                }
                /*ＷＣＤＭＡ使用别公司的功放模块，在结尾会多一个0xff影响下一个模块的通信，在这里先查询一次*/
                if((CheckCommArr.Arr[CheckCommArr.PollAdd].Func == PA_FUNC) && (CheckCommArr.Arr[CheckCommArr.PollAdd].Addr == 0x01)
                        || (CheckCommArr.Arr[CheckCommArr.PollAdd].Func == LNA_FUNC) && (CheckCommArr.Arr[CheckCommArr.PollAdd].Addr == 0x09)) {
                    RSSendLen = FillPollData(RSSendBuf, (CheckComm *)&InvalidCmd, 0x01);
                    RS485_SEND(RSSendBuf, RSSendLen);
                    //memset(RSRecvBuf,0x00,sizeof(RSRecvBuf));
                    RSRecvLen = RS485_RECV(RSRecvBuf, sizeof(RSRecvBuf), GETTIME);
                }

                CheckCommArr.PollAdd++;
            }
            if(CheckCommArr.PollAdd >= CheckCommArr.CheckNum) {
                if(ICS_HT_FUNC == DIG_PICO)
                    GET_DIG_PICO_POWER();

                CheckCommArr.PollAdd = 0x00;
                CheckCommArr.GlobalPollNum ++;
                // if(CheckCommArr.GlobalPollNum == 5 * POLLSKIP) { //大论询5次后加入自动AGC需查询的参数
                //     Auto_AGC_Check();
                // }
                if(CheckCommArr.GlobalPollNum >= 0xfffffff0) {
                    CheckCommArr.GlobalPollNum = 10 * POLLSKIP;
                }
                timedelay(0, 0, 100, 0);
            }
            Try_Check_Time += 1;
            if(Try_Check_Time >= 432000) { /*差不多每隔一天就清除不在线次数,重新轮询*/
                u8 i;
                Try_Check_Time = 0;
                for(i = 0 ; i < CheckCommArr.CheckNum; i++) {
                    CheckCommArr.Arr[i].Online = 0x00;
                }
            }
        }//end if(PollAdd < CheckCommArr.CheckNum)
        else {
            timedelay(0, 0, 100, 0);
        }
    }//end while(1)
}

