#ifndef WEB_PROTOCOL_H_
#define WEB_PROTOCOL_H_
#include "module_adr_def.h"
#include "protocol_def.h"
#include "helios.h"
#pragma pack(1)
typedef struct {
	u8 data[PARA_BUF_SIZE];		/*para data*/
	u16 paralen;				/*para data len*/
	//u16 device_id;				/*device id*/
	u8 cmd;						/*frame command*/
	u8 authority;					/*operate authority*/
	u16 total;					/*total frames*/
	u16 index;					/*current frames*/
	md_adr_info md_adr;			/*the module address which the frame want to operate*/
	u8 flag;
	s8 *next;
} web_para_stream;

typedef struct 
{
    md_adr_info md_adr;
    u8 authority;
    u16 len;
    char para[PARA_BUF_SIZE];
}web_protocol_t;

typedef struct 
{
    long cmd;
    web_protocol_t data;
}web_msg_t;
#pragma pack()
#endif //WEB_PROTOCOL_H_