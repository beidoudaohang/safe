#ifndef __CH_AGC_DEAL_H__
#define __CH_AGC_DEAL_H__

#include "helios.h"
#include "module_adr_def.h"

s8 ch_to_total_agc(md_adr_info *md, u8 ul_dl);
u8 ch_agc_check(s8 * agc, md_adr_info *md, u8 ul_dl);
u8 ch_agc_check_sw(u8 * sw, md_adr_info *md, u8 ul_dl);

#endif //__CH_AGC_DEAL_H__