/*
name:protocol_api.h
author:luke
date:20170324
ver:1.0
description:
	module parameter dat struct
*/
#ifndef PROTOCOL_API_H_
#define PROTOCOL_API_H_

/*****************************include files***********************/
#include "helios.h"
#include "protocol_def.h"

/*****************************para define*************************/


/*****************************data struct define******************/

/*****************************funs********************************/
s8 frame_recv_deal(s8 *str, u16 len, FRAME_SOURCE source, para_stream *ps);
s16 frame_pack(const para_stream *para, frame *pf);
s32 heartbeat(para_stream *ps, frame *pf);
#endif //PROTOCOL_API_H_
