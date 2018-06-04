/*
name:protocol_task.c
author:salan
date:20170322
ver:1.0
description:
	monitor task
*/

/*****************************include files***********************/
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include "helios.h"
#include "protocol_def.h"
#include "protocol_api.h"
#include "para_table.h"
#include "protocol_task.h"
#include "porting.h"
#include "protocol_sem.h"
#include "log.h"
#include "minilzo.h"
/*************************data struct define***********************/

/*************************para define****************************/
frame frame_send;
frame frame_local_recv;
frame frame_remote_recv;
para_stream para_stream_t;

//协议处理进程
pthread_t protocol_ts_id;
pthread_attr_t protocol_ts_attr;
//数据发送进程，
pthread_t frame_send_ts_id;
pthread_attr_t frame_send_ts_attr;
//协议包相关信息
frame_info frame_info_t;
volatile FRAME_SOURCE frame_source;
protocol_sem protocol_sem_t;
/****************************funs*******************************/
/*协议处理进程，处理远控与上位机所发的请求*/
void *protocol_task(void* arg)
{
	s32 err;
	s32 cnt;

	arg = arg;

	//lzo init
	if (LZO_E_OK != lzo_init()) {
		RLDEBUG("protocol_task:lzo_init() false \r\n");
	}

	while (1) {
		//RLDEBUG("protocol_task:sem_wait() begin \r\n");

		#if TEST_DATA
		err = sem_wait(&(protocol_sem_t.protocol_recv_sem));
		if (err) {
			RLDEBUG("protocol_task:sem_wait() false \r\n");
			timedelay(0, 0, 100, 0);
			continue;
		} else {
			if ((0 == (frame_info_t.local_recv_len)) && (0 == (frame_info_t.remote_recv_len)))
			{
				RLDEBUG("protocol_task:(0 == (frame_info_t.local_recv_len)) && (0 == (frame_info_t.remote_recv_len)) \r\n");
				memset(&frame_local_recv, 0, sizeof(frame));
				memset(&frame_remote_recv, 0, sizeof(frame));
				memset(&frame_info_t, 0, sizeof(frame_info));
				continue;
			}
		}
		#endif
		
		// memcpy(frame_local_recv, "");
		// frame_info_t.local_recv_len = 

		//switch frame source
		memset(&para_stream_t, 0, sizeof(para_stream));
		if (frame_info_t.local_recv_len) {
			frame_source = FRAME_SOURCE_LOCAL;
		} else if (frame_info_t.remote_recv_len) {
			frame_source = FRAME_SOURCE_REMOTE;
		}
		//check para_stream_t is busy?
		err = 10;
		while (err > 0) {
			if (PARA_STREAM_BUSY == para_stream_t.flag) {
				timedelay(0, 0, 50, 0);
			} else
				break;

			err--;
		}
		if (err > 0) {
			if (FRAME_SOURCE_LOCAL == frame_source) {

				//RLDEBUG("protocol_task:frame_local:frame_recv_deal() -->recv data len=%d \r\n", frame_info_t.local_recv_len);

				err = frame_recv_deal((s8*)&frame_local_recv, frame_info_t.local_recv_len, frame_source, &para_stream_t);
			} else {
				err = frame_recv_deal((s8*)&frame_remote_recv, frame_info_t.remote_recv_len, frame_source, &para_stream_t);
			}
		} else {
			RLDEBUG("protocol_task:para stream busy\r\n");
			continue;
		}
		//data processing err
		if (err < 0) {
			RLDEBUG("protocol_task:recv data processing err\r\n");
			continue;
		}

		/*检查是否有数据包未发送完成？*/
		cnt = 0;
		err = 10;
		while (err > 0) {
			sem_getvalue(&(protocol_sem_t.protocol_send_sem), &cnt);
			if (cnt > 0) {
				timedelay(0, 0, 100, 0);
			} else {
				break;
			}
			err--;
		}
		if (err > 0) {
			memset(&frame_send, 0, sizeof(frame));
			cnt = (s32)frame_pack((const para_stream*)&para_stream_t, (frame*)&frame_send);
		} else {
			RLDEBUG("protocol_task:send buf full\r\n");
			continue;
		}
		/*post send data*/
		if (cnt > 0) {
			frame_info_t.send_len = cnt;
			err = sem_post(&(protocol_sem_t.protocol_send_sem));
			if (err) {
				RLDEBUG("protocol_task:post send sem err\r\n");
			}
		}
	}
}

void *frame_send_task(void* arg)
{
	s32 err;

	arg = arg;

	while (1) {
		err = sem_wait(&(protocol_sem_t.protocol_send_sem));
		if (err) {
			continue;
		}
		if (frame_info_t.send_len) {
			if (FRAME_SOURCE_LOCAL == frame_source) {
				err = frame_tty_send((s8*)&frame_send, frame_info_t.send_len);
				if (err < 0) {
					RLDEBUG("frame_send_task:local tty send false\r\n");
				}
			} else {

			}
		}

		frame_info_t.send_len = 0;
	}
}
