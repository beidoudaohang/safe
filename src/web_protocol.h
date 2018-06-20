#ifndef WEB_PROTOCOL_H_
#define WEB_PROTOCOL_H_
#include "module_adr_def.h"
#include "protocol_def.h"
#include "helios.h"


#define WEB_PROTOCOL_HEAD_LEN 8


#pragma pack(1)

typedef struct 
{
	u8 cmd;
    md_adr_info md_adr;
    u8 authority;
	u8 ack;
    u16 len;
    u8 para[PARA_BUF_SIZE];
}web_protocol_t;

typedef struct 
{
    long type;
    web_protocol_t data;
}web_msg_t;
#pragma pack()




s8 web_recv_deal(s8 *str, para_stream *ps);
s16 web_pack(const para_stream *ps, web_msg_t *pf);

void *local_web_thread(void *arg);
#endif //WEB_PROTOCOL_H_