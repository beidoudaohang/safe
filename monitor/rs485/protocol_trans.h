#ifndef __PROTOCOL_TRANS_H__
#define __PROTOCOL_TRANS_H__

#include "Module_Struct.h"
#include "helios.h"


typedef struct {
    u8 band;
    f32 ul_low_freq;
    f32 ul_high_freq;
    f32 dl_low_freq;
    f32 dl_high_freq;
} band_table;

void protocol_old2new(Rs485Comm *Rstr);

#endif