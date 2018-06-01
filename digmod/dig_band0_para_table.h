/*
name:dig_para_table.h
author:luke
date:20170324
ver:1.0
description:
	dig parameter dat deal
*/
#ifndef DIG_PARA_TABLE_H_
#define DIG_PARA_TABLE_H_

/*****************************include files***********************/
#include "helios.h"
#include "para_def.h"
#include "para_dynamic_def.h"



/*****************************para define*************************/

/*****************************data struct define******************/

/*****************************funs********************************/
s8 fill_one_adr(u8 adr, void *dest);
s8 analyze_one_adr(u8 adr, s8 *src);
u16 get_dig_set_table_size(void);
u16 get_dig_read_table_size(void);
s8 get_dig_read_table_next_adrs(u16 begin, u16 *newindex, s8 *dest);

#endif //DIG_PARA_TABLE_H_
