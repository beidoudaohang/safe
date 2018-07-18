/*
 * File      : module_send.h
 * 
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-06-28      sujj      first version
*/
#ifndef __MODULE_SEND_H__
#define __MODULE_SEND_H__

void rs485_workfreq_send(u8 sys_num, void * src);
void rs485_ch_agc_send(u8 sys_num, void * src,  u8 udl);
void rs485_ch_att_send(u8 sys_num, void * src,  u8 udl);
void rs485_ch_sw_send(u8 sys_num, void * src);
void rs485_ch_pin_op_th_send(u8 sys_num, void * src,  u8 udl);
#endif //__MODULE_SEND_H__