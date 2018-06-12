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

#define AD9370_RST_REG 0x70
#define CHN1_ATT_REG  0xc1
#define CHN1_ATT_LE_REG 0xc2
#define CHN2_ATT_REG 0xc3
#define CHN2_ATT_LE_REG 0xc4

extern u8 ad9370_config();
extern void bspAD9370StateCheck(void);

#endif /* HEADLESS_H_ */

