#include <math.h>
#include <string.h>

#include "ch_agc_deal.h"
#include "helios.h"
#include "para_table.h"
#include "log.h"
#include "module_adr_def.h"
#include "Module_Struct.h"

//信道AGC计算总AGC
s8 ch_to_total_agc(md_adr_info *md, u8 ul_dl)
{   
	u8 i, is_exmod;
	u32 agc_power_sum=0;
	s8 agc_total=0;
    s16 index;
    
    if(md == NULL) return INVALID_VALUE;

    index = find_para_adr_mod( md, &is_exmod);

    if(index == -1) return INVALID_VALUE;

	for(i=0; i<FREQ_CHANNEL_NUMS_MAX; i++){
        if(exmod_para_a[index].ch_info_t.dl[0].sw == 0) continue;
        
        if(ul_dl == UL)
            agc_power_sum += (u32)pow(10, (double)exmod_para_a[index].ch_info_t.ul[i].agc_th/10);
        else
            agc_power_sum += (u32)pow(10, (double)exmod_para_a[index].ch_info_t.dl[i].agc_th/10);  
	}

	agc_total = (s8)(10 * log10((double)agc_power_sum)+0.005); 

    return agc_total;
}

//检查子带AGC是否超过总AGC门限,超过返回1,没超过返回0
u8 ch_agc_check(s8 * agc, md_adr_info *md, u8 ul_dl)
{
	u8 i, is_exmod;
	u32 agc_power_sum=0;
	s8 agc_total=0;
    s16 index;

	if(agc == NULL || md == NULL) return 1;

    index = find_para_adr_mod( md, &is_exmod);

    if(index == -1) return INVALID_VALUE;

	for(i=0; i<FREQ_CHANNEL_NUMS_MAX; i++){
        if(exmod_para_a[index].ch_info_t.dl[i].sw == 1){
            agc_power_sum += (u32)pow(10, (double)agc[i]/10);
        }
	}

	agc_total = (s8)(10 * log10((double)agc_power_sum)+0.005);
	
	if(ul_dl == UL){
		if(exmod_para_a[index].md_augment.max_agc_th.max_agc_th_ul < agc_total){
			return 1;
		}
	}else{
		if(exmod_para_a[index].md_augment.max_agc_th.max_agc_th_dl < agc_total){
			return 1;
		}
	}

	return 0;
}



//检查子带AGC是否超过总AGC门限,超过返回1,没超过返回0
u8 ch_agc_check_sw(u8 * sw, md_adr_info *md, u8 ul_dl)
{
	u8 i, is_exmod;
	u32 agc_power_sum=0;
	s8 agc_total=0;
    s16 index;

	if(sw == NULL || md == NULL ) return 1;

    index = find_para_adr_mod( md, &is_exmod);

    if(index == -1) return INVALID_VALUE;


	for(i=0; i<FREQ_CHANNEL_NUMS_MAX; i++){
        if((sw[i]&0x01) && ul_dl == UL){
            agc_power_sum += (u32)pow(10, (double)unit_para_t.band_whole.band_restrict_ul[unit_dynamic_para_t.band_current].ch_agc_th[i]/10);
        }else if((sw[i]&0x01) && ul_dl == DL){
            agc_power_sum += (u32)pow(10, (double)unit_para_t.band_whole.band_restrict_dl[unit_dynamic_para_t.band_current].ch_agc_th[i]/10);
        }
	}

	agc_total = (s8)(10 * log10((double)agc_power_sum)+0.005);
	
	if(ul_dl == UL){
		if(exmod_para_a[index].md_augment.max_agc_th.max_agc_th_ul < agc_total){
			RLDEBUG("exmod_para_a[index].md_augment.max_agc_th.max_agc_th_ul:%d, agc_total:%d\n",exmod_para_a[index].md_augment.max_agc_th.max_agc_th_ul, agc_total);
			return 1;
		}
	}else{
		if(exmod_para_a[index].md_augment.max_agc_th.max_agc_th_dl < agc_total){
			RLDEBUG("exmod_para_a[index].md_augment.max_agc_th.max_agc_th_dl:%d, agc_total:%d\n",exmod_para_a[index].md_augment.max_agc_th.max_agc_th_dl, agc_total);
			return 1;
		}
	}

	return 0;
}