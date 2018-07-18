/*
 * File      : module_send.c
 * 
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-06-28      sujj      first version
*/
#include <string.h>

#include "helios.h"
#include "log.h"
#include "module_send.h"
#include "Module_Comm.h"
#include "Module_Struct.h"
#include "para_table.h"

extern _SET_MOD_SEM  SetSem;
extern _SYS_Var  SYS_Var[SYS_NUM];
extern MODULE_RP MOD_RP[SYS_NUM];
extern u8  ICS_HT_FUNC;

void rs485_workfreq_send(u8 sys_num, void * src)
{
    u8 DLModAddr, i;
    ch_link *pch;
    FREQ CHFreq;
    s32 err;

    if(sys_num == 0xff) return;

    pch = (ch_link*)src;
    DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;

    memset((u8 *)&SetSem.SET_STRUCT[SetSem.SetSemFlag], 0x00, sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Func = ICS_HT_FUNC;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr = DLModAddr;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd = SETICS;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen = 0x03 + SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num * 4;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0] = 0x01;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1] = 0x02; //工作信道号
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2] = SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;
    for (i = 0; i < SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num; i++)
    {
        // if (Sel_SYS->RF_SYS.MIMO_Enable == 0x01)
        //     CHFreq.a = GET_FREQ((u16)*begin, DL, Sel_SYS->RF_SYS.MIMO_Replace);
        // else
        //     CHFreq.a = GET_FREQ((u16)*begin, DL, Sel_SYS->Multiple);
        CHFreq.a = pch[i].workfreq;
        memcpy(&SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + i * 4], (u8 *)&CHFreq.b, 4);
    }
    SetSem.SetSemFlag += 1;
    err = sem_wait(&SetSem.SetSem);
}

void rs485_ch_agc_send(u8 sys_num, void * src,  u8 udl)
{
    u8 DLModAddr, i;
    ch_link *pch;
    s32 err;

    if(sys_num == 0xff) return;

    pch = (ch_link*)src;
    DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;

    memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Func	  = ICS_HT_FUNC; 
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr	  = DLModAddr;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd	  = SETICS;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen	  = 0x03 + SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = 0x01;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1]  = udl == UL ? 0xe2 : 0xe3;  
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2]  = SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;
    
    for (i = 0; i < SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num; i++)
    {
        SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + i] = pch[i].agc_th;
    }
    SetSem.SetSemFlag += 1;
    err = sem_wait(&SetSem.SetSem);
}

void rs485_ch_att_send(u8 sys_num, void * src,  u8 udl)
{
    u8 DLModAddr, i;
    ch_link *pch;
    s32 err;

    if(sys_num == 0xff) return;

    pch = (ch_link*)src;
    DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;

    memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Func	  = ICS_HT_FUNC; 
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr	  = DLModAddr;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd	  = SETICS;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen	  = 0x03 + SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = 0x01;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1]  = udl == UL ? 0xe0 : 0xe1;  
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2]  = SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;
    
    for (i = 0; i < SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num; i++)
    {
        SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + i] = pch[i].att;
    }
    SetSem.SetSemFlag += 1;
    err = sem_wait(&SetSem.SetSem);
}

void rs485_ch_sw_send(u8 sys_num, void * src)
{
    u8 DLModAddr, i;
    ch_link *pch;
    s32 err;
    u8 sw[3] = {0};

    if(sys_num == 0xff) return;

    pch = (ch_link*)src;
    DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;

    for (i = 0; i < SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num; i++)
    {

        if (i < 8)
        {
            if (pch[i].sw)
                sw[2] |= (0x01 << i);
        }
        else if ((i >= 8) && (i < 16))
        {
            if (pch[i].sw)
                sw[1] |= (0x01 << (i - 8));
        }
        else
        {
            if (pch[i].sw)
                sw[0] |= (0x01 << (i - 16));
        }
    }

    memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Func  = ICS_HT_FUNC;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr  = DLModAddr;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd   = SETICS;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen  = 0x06;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0] = 0x01;
    if(ICS_HT_FUNC == ICS_FUNC)	 
        SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1] = 0x01;
    else
        SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1] = 0x06;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2] = SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3] = sw[0];
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[4] = sw[1];
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[5] = sw[2];

    SetSem.SetSemFlag += 1;
    err = sem_wait(&SetSem.SetSem);
}

void rs485_ch_pin_op_th_send(u8 sys_num, void * src,  u8 udl)
{
    u8 DLModAddr, i;
    ch_link *pch;
    s32 err;
    u16 tmp;

    if(sys_num == 0xff) return;

    pch = (ch_link*)src;
    DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;

    memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Func	  = ICS_HT_FUNC; 
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr	  = DLModAddr;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd	  = SETICS;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen	  = 0x04;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = 0x01;
    SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1]  = udl == UL ? 0x09 : 0x0a;  
    
    tmp = pch->pin_op_th * 16;
    ByteSwap((u8 *)&tmp);
    memcpy(&SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2], &tmp, sizeof(u16));

    SetSem.SetSemFlag += 1;
    err = sem_wait(&SetSem.SetSem);
}

