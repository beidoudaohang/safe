/**************************
* Created by:     XMAN
* Created date:   2016-06-06
* Version:        V1.0
* Descriptions:   9370 config
**************************/
/**
 * \file 9370.h
 *
 * \brief Contains definitions for 9370.c
*/


#ifndef _AD9370_H_
#define _AD9370_H_

#include <pthread.h>
#include <unistd.h>
// #define AD9370_RST_REG 0x70
// #define CHN1_ATT_REG  0xc1
// #define CHN1_ATT_LE_REG 0xc2
// #define CHN2_ATT_REG 0xc3
// #define CHN2_ATT_LE_REG 0xc4

typedef enum {
	AD9370_A = 0, //上行
	AD9370_B	 //下行
} AD9370_CH;

typedef enum {
	AD9370_SET_CENTER_FREQ = 1, 
	AD9370_SET_BANDWIDTH,
	AD9370_SET_CMD_NUMBER	 
} AD9370_TASK_SET;

typedef struct
{
	pthread_t ad9370_tid;
	pthread_attr_t ad9370_attr;
	u8 ad9370_cmd_queue[AD9370_SET_CMD_NUMBER];
	u8 total;
} ad9370_task_st;

extern pthread_t ad9370_tid;
extern pthread_attr_t ad9370_attr;

int ad9370_dev_open(void);
int ad9370_dev_close(void);
int ad9370_dev_reset(int index);
int ad9370_dev_reg_set(AD9370_CH ch, int reg, char dat);
int ad9370_dev_reg_read(AD9370_CH ch, int reg, char *dat);

u8 ad9370_config();
u8 ad9370_cmd_add(u8 cmd);
// void bspAD9370StateCheck(void);

#endif /* HEADLESS_H_ */

