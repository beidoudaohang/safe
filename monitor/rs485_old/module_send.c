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
#include <math.h>

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

#define MAKEWORD(a, b)      ((u16)(((u8)(((u64)(a)) & 0xff)) | ((u16)((u8)(((u64)(b)) & 0xff))) << 8))
#define LOBYTE(w)           ((u8)(((u64)(w)) & 0xff))
#define HIBYTE(w)           ((u8)((((u64)(w)) >> 8) & 0xff))

void int22hex(int val,u8 b, u8 *des)
{
	int ntemp1,ntemp2;
	u8 bsigned;

	if(des == NULL) return;

	if (val < 0 )
		bsigned = TRUE;
	else
		bsigned = FALSE;
	ntemp1 = abs(val);

	des[0] = ntemp1 % 16;
	ntemp1 = ntemp1 /16;
	ntemp2 = ntemp1 % 16;
	des[0] += ntemp2 * 16;

	ntemp1 = ntemp1 /16;
	des[1] = ntemp1 % 16;
	ntemp1 = ntemp1 /16;
	ntemp2 = ntemp1 % 16;
	des[1] += ntemp2 * 16;

	u16 temp;

	if (bsigned)
	{
		temp = MAKEWORD(des[0],des[1]);
		
		temp = ~temp + 1;
		if (!b)
			temp = temp & 0x0FFF;
		
		des[0] = LOBYTE(temp);
		des[1] = HIBYTE(temp);
	}
}

void rs485_workfreq_send(u8 sys_num, void * src)
{
	u8 DLModAddr, i;
	ch_link *pch;
	FREQ CHFreq;
	s32 err;
	u16 index;

	if(sys_num == 0xff) return;

	pch = (ch_link*)src;
	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;

	index = oldaddr_find_mod(ICS_HT_FUNC, DLModAddr);

	if(index == 0xff ) return;

	memset((u8 *)&SetSem.SET_STRUCT[SetSem.SetSemFlag], 0x00, sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Func = ICS_HT_FUNC;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr = DLModAddr;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd = SETICS;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen = 0x03 + SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num * 4;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0] = 0x01;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1] = 0x02; //工作信道号
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2] = SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;

	if((MOD_RP[sys_num].ICS_HT_RP.QueryRP.ICSType & 0xf0) == 0x10){//可变选带
		for (i = 0; i < (SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num>>1); i++) {
			
			CHFreq.a = pch[i].workfreq - exmod_para_a[index].ch_info_t.bandwidth[i]/2;
			memcpy(&SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + i*2 * 4 ], (u8 *)&CHFreq.b, 4);
			CHFreq.a = pch[i].workfreq + exmod_para_a[index].ch_info_t.bandwidth[i]/2;
			memcpy(&SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + (i*2+1) * 4], (u8 *)&CHFreq.b, 4);
		}
	}else{
		for (i = 0; i < SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num; i++){
			// if (Sel_SYS->RF_SYS.MIMO_Enable == 0x01)
			//     CHFreq.a = GET_FREQ((u16)*begin, DL, Sel_SYS->RF_SYS.MIMO_Replace);
			// else
			//     CHFreq.a = GET_FREQ((u16)*begin, DL, Sel_SYS->Multiple);
			CHFreq.a = pch[i].workfreq;
			memcpy(&SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + i * 4], (u8 *)&CHFreq.b, 4);
		}
	}

	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

void rs485_ch_agc_send(u8 sys_num, void * src,  u8 udl)
{
	u8 DLModAddr, i;
	ch_link *pch;
	s32 err;
	u16 index;

	if(sys_num == 0xff) return;

	pch = (ch_link*)src;
	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;

	index = oldaddr_find_mod(ICS_HT_FUNC, DLModAddr);

	if(index == 0xff ) return;

	memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Func	  = ICS_HT_FUNC; 
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr	  = DLModAddr;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd	  = SETICS;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen	  = 0x03 + SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = 0x01;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1]  = udl == UL ? 0xe2 : 0xe3;  
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2]  = SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;
	
	if((MOD_RP[sys_num].ICS_HT_RP.QueryRP.ICSType & 0xf0) == 0x10){//可变选带
		for (i = 0; i < (SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num>>1); i++)
		{
			SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + i*2+1] = pch[i].agc_th;
		}
	}else{
		for (i = 0; i < SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num; i++)
		{
			SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + i] = pch[i].agc_th;
		}
	}

	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

void rs485_ch_att_send(u8 sys_num, void * src,  u8 udl)
{
	u8 DLModAddr, i;
	ch_link *pch;
	s32 err;
	u16 index;

	if(sys_num == 0xff) return;

	pch = (ch_link*)src;
	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;

	index = oldaddr_find_mod(ICS_HT_FUNC, DLModAddr);

	if(index == 0xff ) return;

	memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Func	  = ICS_HT_FUNC; 
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr	  = DLModAddr;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd	  = SETICS;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen	  = 0x03 + SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = 0x01;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1]  = udl == UL ? 0xe0 : 0xe1;  
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2]  = SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;
	
	if((MOD_RP[sys_num].ICS_HT_RP.QueryRP.ICSType & 0xf0) == 0x10){//可变选带
		for (i = 0; i < (SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num>>1); i++)
		{
			SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + i*2+1] = pch[i].att;
		}
	}else{
		for (i = 0; i < SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num; i++)
		{
			SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + i] = pch[i].att;
		}
	}
	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

void rs485_ch_sw_send(u8 sys_num, void * src)
{
	u8 DLModAddr, i, tmp;
	ch_link *pch;
	s32 err;
	u8 sw[3] = {0};
	u16 index;

	if(sys_num == 0xff) return;

	pch = (ch_link*)src;
	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;

	index = oldaddr_find_mod(ICS_HT_FUNC, DLModAddr);

	if(index == 0xff ) return;

	if((MOD_RP[sys_num].ICS_HT_RP.QueryRP.ICSType & 0xf0) == 0x10){//可变选带
		for (i = 0; i < (SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num>>1); i++){
			tmp = (i<<1) + 1;
			if (tmp < 8){
				if (pch[i].sw)
					sw[2] |= (0x01 << tmp);
			}else if ((tmp >= 8) && (tmp < 16)){
				if (pch[i].sw)
					sw[1] |= (0x01 << (tmp - 8));
			}else{
				if (pch[i].sw)
					sw[0] |= (0x01 << (tmp - 16));
			}
		}
	}else{
		for (i = 0; i < SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num; i++){
			if (i < 8){
				if (pch[i].sw)
					sw[2] |= (0x01 << i);
			}else if ((i >= 8) && (i < 16)){
				if (pch[i].sw)
					sw[1] |= (0x01 << (i - 8));
			}else{
				if (pch[i].sw)
					sw[0] |= (0x01 << (i - 16));
			}
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
	int tmp;
	f32 pinop;
	u8 arr[2];

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
	
	pinop = pch->pin_op_th * 16;
	tmp = pinop;
	int22hex(tmp, FALSE, arr);
	//ByteSwap((u8 *)&tmp);
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2] = arr[1];
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3] = arr[0];

	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

void rs485_center_freq_send(u8 sys_num, void * src,  u8 udl)
{
	u8 DLModAddr, i;
	s32 err;

	if(sys_num == 0xff) return;

	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;

	
	memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Func     = ICS_HT_FUNC;   
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr     = DLModAddr;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd      = SETFREQ;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen     = 0x06;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = 0x01;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1]  = udl == UL ? 0x01 : 0x02;  
	memcpy((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2], (u8*)src, 4);
	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

void rs485_ch_bandwidth_send(u8 sys_num, void * src)
{
	u8 DLModAddr, i;
	s32 err;
	f32 *pch;
	u16 index;
	FREQ CHFreq;

	if(sys_num == 0xff) return;

	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;
	pch = (f32*)src;

	index = oldaddr_find_mod(ICS_HT_FUNC, DLModAddr);
	
	memset((u8 *)&SetSem.SET_STRUCT[SetSem.SetSemFlag], 0x00, sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Func = ICS_HT_FUNC;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr = DLModAddr;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd = SETICS;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen = 0x03 + SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num * 4;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0] = 0x01;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1] = 0x02; //工作信道号
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2] = SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num;

	if((MOD_RP[sys_num].ICS_HT_RP.QueryRP.ICSType & 0xf0) == 0x10){//可变选带
		for (i = 0; i < (SYS_Var[sys_num].RF_SYS.MOD_ONLINE.ICS.Num>>1); i++) {
			
			CHFreq.a = exmod_para_a[index].ch_info_t.dl[i].workfreq - pch[i]/2;
			memcpy(&SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + i*2 * 4 ], (u8 *)&CHFreq.b, 4);
			CHFreq.a = exmod_para_a[index].ch_info_t.dl[i].workfreq + pch[i]/2;
			memcpy(&SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3 + (i*2+1) * 4], (u8 *)&CHFreq.b, 4);
		}
	}
	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

void rs485_blocking_compensation_send(u8 sys_num, void * src)
{
	u8 DLModAddr, i;
	s32 err;
  
	if(sys_num == 0xff) return;

	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;
	
	memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Func     = ICS_HT_FUNC;   
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr     = DLModAddr;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd      = SETICS;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen     = 0x03;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = 0x01;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1]  = 0xe4;  
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2]  = *(u8*)src;  
	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

void rs485_max_agc_th_send(u8 sys_num, void * src, u8 udl)
{
	u8 DLModAddr, i;
	s32 err;
	int tmp;
	f32 max_agc_th;
	u8 arr[2];

	if(sys_num == 0xff) return;

	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;
	
	memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Func     = ICS_HT_FUNC;   
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr     = DLModAddr;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd      = SETICS;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen     = 0x04;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = 0x01;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1]  = udl == UL ? 0x07 : 0x08;  

	max_agc_th = *(s8*)src * 16;
	tmp = max_agc_th;
	int22hex(tmp, FALSE, arr);
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2] = arr[1];
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3] = arr[0];

	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

void rs485_att_th_send(u8 sys_num, void * src, u8 udl)
{
	u8 DLModAddr, i;
	s32 err;
	int tmp;
	f32 att_th;
	u8 arr[2];

	if(sys_num == 0xff) return;

	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;
	
	memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Func     = ICS_HT_FUNC;   
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr     = DLModAddr;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd      = SETICS;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen     = 0x04;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = 0x01;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1]  = udl == UL ? 0x0b : 0x0c;  

	att_th = *(s8*)src * 16;
	tmp = att_th;
	int22hex(tmp, FALSE, arr);
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2] = arr[1];
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3] = arr[0];

	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

void rs485_pa_att_th_send(u8 sys_num, void * src, u8 udl)
{
	u8 ULModAddr, DLModAddr, i;
	s32 err;
	
	if(sys_num == 0xff) return;

	ULModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + UL;
	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;
	
	memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Func     = PA_FUNC;   //功放
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr     = udl == UL ? ULModAddr : DLModAddr;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd      = SETATT;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen     = 0x01;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = *(s8*)src;

	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

void rs485_ch_mute_sw_send(u8 sys_num, void * src)
{
	u8  DLModAddr, i;
	s32 err;
	ch_link *pch;
	if(sys_num == 0xff) return;

	pch = (ch_link*)src;
	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;
	
	memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Func     = ICS_HT_FUNC;   
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr     = DLModAddr;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd      = SETICS;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen     = 0x03;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = 0x01;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1]  = 0x11;  
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2]  =  pch[0].mute_sw;  

	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

void rs485_ch_mute_th_send(u8 sys_num, void * src)
{
	u8  DLModAddr, i;
	s32 err;
	u16 index;

	if(sys_num == 0xff) return;

	DLModAddr = SYS_ADDR_BASE + sys_num * SYS_ADDR_SKIP + DL;

	index = oldaddr_find_mod(ICS_HT_FUNC, DLModAddr);
	
	memset((u8*)&SetSem.SET_STRUCT[SetSem.SetSemFlag],0x00,sizeof(SetSem.SET_STRUCT[SetSem.SetSemFlag]));
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Func     = ICS_HT_FUNC;   
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Addr     = DLModAddr;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Cmd      = SETICS;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].Clen     = 0x04;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[0]  = 0x01;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[1]  = 0x0f;  
	//高门限不能小于低门限
	if(exmod_para_a[index].ch_info_t.ul[0].mute_th_h < exmod_para_a[index].ch_info_t.ul[0].mute_th_l){
		exmod_para_a[index].ch_info_t.ul[0].mute_th_h = exmod_para_a[index].ch_info_t.ul[0].mute_th_l;
	}
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[2]  =  exmod_para_a[index].ch_info_t.ul[0].mute_th_h;
	SetSem.SET_STRUCT[SetSem.SetSemFlag].data[3]  =  exmod_para_a[index].ch_info_t.ul[0].mute_th_l;

	SetSem.SetSemFlag += 1;
	err = sem_wait(&SetSem.SetSem);
}

