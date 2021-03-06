/*
name:porting.h
author:luke
date:20170407
ver:1.0
description:
	porting api
*/
#ifndef PORTING_H_
#define PORTING_H_

/*****************************include files***********************/
#include "helios.h"
#include "para_def.h"


/*****************************para define*************************/
#define MONITOR_TTY_NAME 			"/dev/ttyGS3"
#define TTY_BAUDRATE				B115200
#define DIG_TTY_NAME 				"/dev/ttyS0"

#define TTY_485NAME 				"/dev/ttymxc1"
#define TTY_485BAUDRATE				B9600
/*****************************data struct define******************/

/*****************************funs********************************/
//file opreate
s32 unit_file_init(void);
s32 unit_file_read(unit_para *up);
s32 unit_file_write(unit_para *up);
s32 band_file_init(void);
s32 band_file_read(band_para *bp, u8 mod_index);
s32 band_file_write(band_para *bp, u8 mod_index);
s32 pcb_file_init(void);
s32 pcb_file_read(pcb_share_para *pp);
s32 pcb_file_write(pcb_share_para *pp);
//time delay
void timedelay(u8 m, u8 s, u16 ms, u16 us);
//frame tty
s8 frame_tty_open(void);
s8 frame_tty_close(void);
s8 frame_tty_send(s8 *src, u16 len);
s16 frame_tty_recv(s8 *src, u16 len);
//dig tty
s8 dig_band0_tty_open(void);
s8 dig_band0_tty_close(void);
s8 dig_band0_tty_send(s8 *src, u16 len);
s8 dig_band0_tty_recv(void);
//debug
void relay_printf(char* fmt, ...);

s8 rs485_tty_open(void);
s8 rs485_tty_close(void);
u8 RS485_SEND(u8 * src ,u32 len);
u16 RS485_RECV(u8 *src,u16 len,u16 timeout);
s8 led_open();
s8 led_run_control(int val);
s8 led_alarm_control(int val);
#endif //PORTING_H_