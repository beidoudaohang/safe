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


// #define AD9370_RST_REG 0x70
// #define CHN1_ATT_REG  0xc1
// #define CHN1_ATT_LE_REG 0xc2
// #define CHN2_ATT_REG 0xc3
// #define CHN2_ATT_LE_REG 0xc4

typedef enum {
	AD9370_A = 0,
	AD9370_B
} AD9370_CH;

int ad9370_dev_open(void);
int ad9370_dev_close(void);
int ad9370_dev_reset(int index);
int ad9370_dev_reg_set(AD9370_CH ch, int reg, char dat);
int ad9370_dev_reg_read(AD9370_CH ch, int reg, char *dat);

u8 ad9370_config();
void bspAD9370StateCheck(void);

#endif /* HEADLESS_H_ */

