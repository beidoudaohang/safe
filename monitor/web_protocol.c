/*
 * File      : web_protocol.c
 * 
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-06-15      sujj      first version
*/

#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/un.h> 
#include <sys/socket.h>
#include <sys/select.h>
#include <signal.h>

#include "helios.h"
#include "log.h"
#include "web_protocol.h"
#include "para_table.h"
#include "usr_def.h"
#include "data_task.h"
#include "protocol_api.h"
#include "porting.h"
#include "common_api.h"

/* #define WEBMSGPATHNAME "/tmp/webmsg"  
#define ID 27  
#define MODE IPC_CREAT|0666
#define SENDMSG 1  
#define RECVMSG 2   */
#define WEBCLIENTCNT 10
#define MAXDATASIZE 1024
#define WEB_SOCK_FILE "/tmp/web_socket"

int web_clientfd[WEBCLIENTCNT] = {0};
extern para_stream para_stream_t;


static s8 web_md_adr_check(web_protocol_t *pw)
{
	u8 cnt;
	s8 flag = -1;

	if (NULL == pw)
		return -1;
#if MONITOR_MODULE_ENABLE
	if (((pw->md_adr.mod_type) == (unit_para_t.md_adr_t.mod_type)) && ((pw->md_adr.mod_band) == (unit_para_t.md_adr_t.mod_band)) && ((pw->md_adr.mod_adr_t.dat) == (unit_para_t.md_adr_t.mod_adr_t.dat)))
		flag = 0;

	if(pw->md_adr.mod_type > 0){
		cnt = (pw->md_adr.mod_type-1)<<4;
	}else{
		return -1;
	}
	for (; cnt < MONITOR_MOD_NUM; cnt++) {
		if (((pw->md_adr.mod_type) == (exmod_para_a[cnt].md_adr_t.mod_type)) 
			&& ((pw->md_adr.mod_band) == (exmod_para_a[cnt].md_adr_t.mod_band)) 
			&& ((pw->md_adr.mod_adr_t.dat) == (exmod_para_a[cnt].md_adr_t.mod_adr_t.dat))) {
					break;
		}
	}
	if (cnt < MONITOR_MOD_NUM) {
		flag = 0;
	}
#endif
#if OTHER_MODULE_ENABLE
	for (cnt = 0; cnt < MOD_NUM_IN_ONE_PCB; cnt++) {
		if (((pw->md_adr.mod_type) == (band_para_a[cnt].md_adr_t.mod_type)) && ((pw->md_adr.mod_band) == (band_para_a[cnt].md_adr_t.mod_band)) && ((pw->md_adr.mod_adr_t.dat) == (band_para_a[cnt].md_adr_t.mod_adr_t.dat)))
			break;
	}

	if (cnt < MOD_NUM_IN_ONE_PCB) {
		flag = 0;
	}
#endif

	return flag;
}

/*
fun：用户登录
para：
	pw：登录信息
return：
	0---sus
	-1：用户不存在
	-2：密码错误
	-3: err
 */
static s32 web_usr_login(web_protocol_t *pw)
{
	s32 cnt = 0;
	s8 dy_pwd[12] = {0};
	s8* sn = NULL;
	s32 err;

	if (NULL == pw) {
		return -1;
	}

	err = usr_name_to_auth((s8*)(pw->para + 1));
	if (err < 0) {
		return -1;
	}
	if (!(pw->para[0])) {
		//固定密码校验
		cnt = err - 1;
		if ((!strncmp((const char*)(pw->para + 1), (const char*)(pcb_share.usr.usr[cnt].name), USR_NAME_SIZE)) && \
				(!strncmp((const char*)((pw->para + 1) + USR_NAME_SIZE), (const char*)(pcb_share.usr.usr[cnt].pass), USR_PASS_SIZE))) {
			RLDEBUG("usr_login:name & pwd match! \r\n");
		} else {
			//动态密码校验
			//find mod
#if MONITOR_MODULE_ENABLE
			if (((pw->md_adr.mod_type) == (unit_para_t.md_adr_t.mod_type)) && ((pw->md_adr.mod_band) == (unit_para_t.md_adr_t.mod_band)) && ((pw->md_adr.mod_adr_t.dat) == (unit_para_t.md_adr_t.mod_adr_t.dat)))
				sn = (s8*) & (unit_para_t.u_mfrs.sn);
#endif
			if (NULL == sn) {
				cnt = md_adr_to_index(&(pw->md_adr));
				if (cnt < 0) {
					return -3;
				}
				sn = (s8*) & (band_para_a[cnt].md_mfrs.sn);
			}

			if (NULL == sn) {
				return -3;
			}

			err = usr_dynamic_pwd_creat((const s8*)&err, (const s8*)sn, dy_pwd);
			if (err < 0) {
				return -3;
			}
			if (strncmp((const char*)dy_pwd, (const char*)((pw->para + 1) + USR_NAME_SIZE), 8)) {
				usr_auth.timeout = 0;
				usr_auth.authorize = USR_UNLOGIN;
				return -2;
			}
		}
	} else {
		return -1;
	}

	usr_auth.timeout = USR_AUTHORIZE_TIME;
	usr_auth.authorize = cnt + 1;

	RLDEBUG("usr_login: usr name:%s,usr authorize:%d \r\n", (pw->para + 1), usr_auth.authorize);

	return 0;
}

static s16 web_get_para_num(const web_protocol_t *pw)
{
	s16 len, cnt, nums;
	para *_para;

	if (NULL == pw)
		return -1;

	len = pw->len;
	cnt = 0;
	nums = 0;
	while (cnt < len) {
		_para = (para*)((pw->para) + cnt);
		nums++;
		cnt += ((_para->para_len) + sizeof(para));
	}

	return nums;
}

static s16 web_set_para_deal(const web_protocol_t *pw, para_stream *ps)
{
	u16 cnt;
	u16 para_lens, para_nums;
	para *_para;
	s32 err;

	if (NULL == pw)
		return -1;
	if (NULL == ps)
		return -1;

	//RLDEBUG("set_para_deal processing\r\n");

	para_lens = pw->len;
	RLDEBUG("set_para_deal para len is:%d\r\n", para_lens);
	para_nums = web_get_para_num(pw);
	RLDEBUG("set_para_deal para numbers is:%d\r\n", para_nums);

	_para = (para*)(pw->para);
	para_lens = 0;
	for (cnt = 0; cnt < para_nums; cnt++) {
		RLDEBUG("set_para_deal para adr is:%x\r\n", (_para->para_adr));
		err = one_para_adr_set_processing((s8*)(_para), ps);
		if (err < 0) {
			RLDEBUG("set_para_deal:one_para_adr_set_processing() false \r\n");
		}
		para_lens += sizeof(para) + (_para->para_len);
		_para = (para*)((pw->para) + para_lens);
	}

	return 0;
}

static s8 web_read_para_deal(const web_protocol_t *pw, para_stream *ps)
{
	u16 cnt;
	u16 para_nums;
	u16 adr;

	if (NULL == pw)
		return -1;
	if (NULL == ps)
		return -1;

	//RLDEBUG("read_para_deal\r\n");

	para_nums = pw->len;
	para_nums >>= 1;

	//RLDEBUG("read_para_deal para numbers:%d\r\n", para_nums);

	for (cnt = 0; cnt < para_nums; cnt++) {
		memcpy((void*)&adr, (pw->para + cnt * 2), sizeof(u16));
		//RLDEBUG("read_para_deal para adr:%d\r\n", adr);
		one_para_adr_read_processing(adr, ps);
	}
	return 0;
}

s8 web_recv_deal(s8 *str, para_stream *ps)
{
	u16 cnt;
	web_protocol_t *pw = NULL;


	if ((NULL == str) || (NULL == ps))
		return -1;



	pw = (web_protocol_t*)str;

	/*远控读取模块地址信息的指令，不做地址判断*/
	if (FRAME_CMD_READ_MOD_INFO != (pw->cmd))
		if (web_md_adr_check(pw)) {
			RLDEBUG("frame module adr err\r\n");
			goto WEB_RCV_ERR;
		}

	ps->cmd = pw->cmd;
	ps->device_id = unit_para_t.unit_sundry.device_id;
	ps->flag = PARA_STREAM_BUSY;
	memcpy((void*) & (ps->md_adr), (void*) & (pw->md_adr), sizeof(md_adr_info));
	ps->remote_code = 0;
	ps->source = FRAME_SOURCE_LOCAL;
	ps->authority = pw->authority;
	ps->next = (s8*)(ps->data);

	switch (pw->cmd) {
	case FRAME_CMD_READ_MOD_INFO: {
		read_md_info_deal(ps);
		goto WEB_RCV_DONE;
		break;
	}
	case FRAME_CMD_LOGIN: {
		if (0x00 == pw->para[0]) {	//login
			if (web_usr_login(pw)) {
				ps->ack = FRAME_ACK_USR_PASS_ERR;
			}
		} else if (0x01 == (pw->para[0])) {	//pwd changed

		} else if (0x02 == (pw->para[0])) {	//pwd reset

		}
		
		goto WEB_RCV_DONE;
		break;
	}
	};

	//permission check
#ifdef RL_PERMISSION_CHECK
	if (FRAME_SOURCE_LOCAL == ps->source) {
		if (USR_UNLOGIN == usr_auth.authorize) {
			ps->ack = FRAME_ACK_USR_UNLOGIN;
			RLDEBUG("user unlogin\r\n");
			goto WEB_RCV_DONE;
		}
	}
#endif

	switch (pw->cmd) {
	case FRAME_CMD_ALARM: {

		break;
	}
	case FRAME_CMD_HEARTBEAT: {
		//para_stream_t.data[0]=unit_dynamic_para_t.gps_t.gps_status;
		//para_stream_t.data[]
		break;
	}
	case FRAME_CMD_READ_PARA: {
		web_read_para_deal(pw, ps);
		break;
	}
	case FRAME_CMD_READ_MOD_INFO: {
		read_md_info_deal(ps);
		break;
	}
	case FRAME_CMD_SET_PARA: {
		web_set_para_deal(pw, ps);

		sem_post(&(data_write_sem));
		break;
	}
	case FRAME_CMD_LOGIN: {
		if (web_usr_login(pw)) {
			ps->ack = FRAME_ACK_USR_PASS_ERR;
		}
		break;
	}
	case FRAME_CMD_UPDATE: {

		break;
	}

	default: {
		RLDEBUG("can't find cmd\r\n");
		ps->ack = FRAME_ACK_CMD;
	}
	};
WEB_RCV_DONE:
	return 0;
WEB_RCV_ERR:
	return -1;
}


s16 web_pack(const para_stream *ps, web_protocol_t *pw)
{
	s32 cnt;

	if (NULL == ps)
		return -1;
	if (NULL == pw) {
		return -1;
	}

	if ((ps->paralen) > 1024)
		return -1;

	//cnt = one_frame_pack(para, pw);
	memcpy((void*)(pw->para), (void*)(ps->data), (ps->paralen));

	RLDEBUG("ps->paralen=%d\n",ps->paralen);

	pw->len = ps->paralen;
	pw->cmd = ps->cmd;
	memcpy((void*) &(pw->md_adr), (void*) &(ps->md_adr), sizeof(md_adr_info));
	pw->authority = ps->authority;
	pw->ack = ps->ack;

	cnt = pw->len + WEB_PROTOCOL_HEAD_LEN;
	if (cnt < 0)
		return -1;

	/**/
	return cnt;
}

s16 web_protocol_to_new_protocol(web_protocol_t *pw, frame *pf)
{
	s32 len;

	if (NULL == pf)
		return -1;
	if (NULL == pw)
		return -1;

	memcpy((void*)(pf->cmd.data), (void*)(pw->para), (pw->len));

	pf->cmd.total = 1;
	pf->cmd.index = 1;
	pf->cmd.ver = PROTOCOL_VER;
	pf->cmd.cmd = pw->cmd;
	memcpy((void*) & (pf->cmd.md_adr), (void*) & (pw->md_adr), sizeof(md_adr_info));
	pf->cmd.authority = pw->authority;
	pf->cmd.ack = pw->ack;

	memcpy((pf->header.start), FRAME_START, sizeof(FRAME_START));
	pf->header.remote_code = 0;
	pf->header.crc = crc((const s8*)(pf->header.data), ((pw->len) + FRAME_CMD_LEN));
	pf->header.device_id = unit_para_t.unit_sundry.device_id;
	pf->header.len = 5 + (pw->len) + FRAME_CMD_LEN;

	#if 1
	//compress
	if(protocol_encrypt_lzo(pf) == -1){
		return -1;
	}
	#else
	RLDEBUG("pf->header.len = %d\r\n", pf->header.len);
	memcpy((pf->arr + pf->header.len + FRAME_START_LEN + 2), FRAME_END, sizeof(FRAME_END));
	#endif

	return ((pf->header.len) + FRAME_START_LEN + 2 + FRAME_END_LEN );
}

void frame_to_web_protocol(frame *pf, web_protocol_t *pw)
{
	pw->cmd = pf->cmd.cmd;
	pw->authority = pf->cmd.authority;
	pw->ack = pf->cmd.ack;
	memcpy(&pw->md_adr, &pf->cmd.md_adr, sizeof(md_adr_info));
	pw->len = pf->header.len - 5 - FRAME_CMD_LEN;
	memcpy(pw->para, pf->cmd.data, pw->len);
}

s16 new_protocol_to_web_protocol(s8 *str, u16 len, web_protocol_t *pw)
{
	frame *pf = NULL;
	s8 *begin = NULL;

	if(protocol_decode_unlzo(str, len) == -1){
		return -1;
	}


	if (frame_protocol_check(str, len, &begin)) {
		RLDEBUG("frame protocol err\r\n");
		return -1;
	}
	pf = (frame*)(begin);

	frame_to_web_protocol(pf, pw);

	return 0;
}


int socket_init()
{
	int sockfd; 
	struct sockaddr_un sun;

	if((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1){
		RLDEBUG("socket create error");
		exit(1);
	}

	//RLDEBUG("socket success!,sockfd = %d\n", sockfd);

	memset(&sun, 0, sizeof(sun));
	sun.sun_family = AF_LOCAL;
	strcpy (sun.sun_path, WEB_SOCK_FILE);    

	if(bind(sockfd, (struct sockaddr *)&sun, sizeof(sun)) == -1){
		RLDEBUG("bind error ");
		exit(1);
	}

	//RLDEBUG("bind success!\n");
	if(listen(sockfd, WEBCLIENTCNT) == -1){
		RLDEBUG("listen error");
		exit(1);
	}
	RLDEBUG("listening....\n");

	return sockfd;
}

void client_deal(fd_set rset, fd_set mainset, u16 ready)
{
	socklen_t recvbytes, sendbytes;
	u16 i;
	s32 err, cnt;
	int client_fd;
	web_msg_t web_msg_recv, web_msg_send;

	for(i=0; i<WEBCLIENTCNT; ++i){
		client_fd = web_clientfd[i];
		if(FD_ISSET(client_fd, &rset)){
			if((recvbytes = recv(client_fd, (void*)&web_msg_recv.data, sizeof(web_protocol_t), 0)) == -1){
				RLDEBUG("recv error");
				continue;
			}

			if(recvbytes == 0){
				close(client_fd); 
				FD_CLR(client_fd, &mainset);
				web_clientfd[i] = 0;
				RLDEBUG("client:%d off!\n", client_fd);
			}else{
				RLDEBUG("web recv data:");
				hexdata_debug((s8*)&web_msg_recv.data, recvbytes/* web_msg_recv.data.len + WEB_PROTOCOL_HEAD_LEN */);

				if(!get_rs485_mod_init_state()){
					continue;
				}

				if(web_msg_recv.data.md_adr.mod_type == MOD_TYPE_RELAY){
					//RELAY模块是新协议，需要把web数据包打包成新协议发送，再把收到的数据包重新打包成web数据包返回给CGI程序

				}else{
					//check para_stream_t is busy?
					err = 10;
					while (err > 0) {
						if (PARA_STREAM_BUSY == para_stream_t.flag) {
							timedelay(0, 0, 50, 0);
						} else
							break;

						err--;
					}
					memset(&para_stream_t, 0, sizeof(para_stream));
					if(err){
						err = web_recv_deal((s8*)&web_msg_recv.data, &para_stream_t);
					}else{
						RLDEBUG("local_web_thread:para stream busy\r\n");
						continue;
					}
					
					if (err < 0) {
						RLDEBUG("local_web_thread:recv data processing err\r\n");
						continue;
					}
				}

				memset(&web_msg_send, 0, sizeof(web_msg_send));
				// web_msg_send.type = SENDMSG;
				cnt = (s32)web_pack((const para_stream*)&para_stream_t, (web_protocol_t*)&web_msg_send.data);
				para_stream_t.flag = PARA_STREAM_NORMAL;

				RLDEBUG("web send data:");
				hexdata_debug((s8*)&web_msg_send.data, cnt);
				
				if((sendbytes = send(client_fd, (void *)&web_msg_send.data, cnt, 0)) == -1){
					RLDEBUG("send error \n");
					continue;
				}
				
			}

			if(--ready == 0) break;
		}
	}
}

void *local_web_thread(void *arg)
{
	struct sockaddr_un cun;
	socklen_t client_size;
	int sockfd, client_fd, maxfd;
	fd_set rset, mainset;
	u16 ready, i;

	// RLDEBUG("start local web thread...\r\n"); 

	unlink(WEB_SOCK_FILE);  
	signal(SIGPIPE, SIG_IGN);

	sockfd = socket_init();

	client_size = sizeof(struct sockaddr);
	FD_ZERO(&mainset);
	FD_SET(sockfd, &mainset);
	maxfd = sockfd;
	while(1){
		rset = mainset;
		ready = select(maxfd+1, &rset, NULL, NULL, NULL);
		if(ready == -1){
			RLDEBUG("select error \n");
			continue;
		}

		if(FD_ISSET(sockfd, &rset)){
			if((client_fd = accept(sockfd, (struct sockaddr *)&cun, &client_size)) != -1){
				RLDEBUG("client:%d enter!\n", client_fd);

				for(i=0; i<WEBCLIENTCNT; ++i){
					if(web_clientfd[i] <= 0) {
						web_clientfd[i] = client_fd;
						break;
					}
				}

				if(i < WEBCLIENTCNT){
					FD_SET(client_fd, &mainset);

					if(client_fd > maxfd){
						maxfd = client_fd;
					}
				}


			}
			if(--ready == 0) continue;
		}

		client_deal(rset, mainset, ready);
	}

	close(sockfd);
	unlink (WEB_SOCK_FILE);  
	RLDEBUG("close web server\n");

}
#if 0
void *local_web_thread(void *arg)
{
	s32 err, cnt;
	int msgid;
	ssize_t recv_len;
	web_msg_t web_msg_recv, web_msg_send;
	
	key_t key;
	key = ftok(WEBMSGPATHNAME, ID);  
	/*创建消息队列*/
	msgid = msgget(key, MODE);
	// msgid = msgget((key_t)1234, MODE);
	if (msgid == -1)
	{
		RLDEBUG("msgget failed with error\n");
		exit(EXIT_FAILURE);
	}

	/*循环从消息队列中接收消息*/
	while (1)
	{

		/*读取消息*/
		if (recv_len = msgrcv(msgid, (void *)&web_msg_recv, sizeof(web_protocol_t), RECVMSG, 0) == -1)
		{
			RLDEBUG("msgrcv failed with error\n");
			continue;
			// exit(EXIT_FAILURE);
		}

		// RLDEBUG("return:%d, len:%d\r\n", recv_len, web_msg_recv.data.len);
		RLDEBUG("web recv data:");
		hexdata_debug((s8*)&web_msg_recv.data, web_msg_recv.data.len + WEB_PROTOCOL_HEAD_LEN);

		if(!get_rs485_mod_init_state()){
			continue;
		}

		if(web_msg_recv.data.md_adr.mod_type == MOD_TYPE_RELAY){
			//RELAY模块是新协议，需要把web数据包打包成新协议发送，再把收到的数据包重新打包成web数据包返回给CGI程序

		}else{
			//check para_stream_t is busy?
			err = 10;
			while (err > 0) {
				if (PARA_STREAM_BUSY == para_stream_t.flag) {
					timedelay(0, 0, 50, 0);
				} else
					break;

				err--;
			}
			memset(&para_stream_t, 0, sizeof(para_stream));
			if(err){
				err = web_recv_deal((s8*)&web_msg_recv.data, &para_stream_t);
			}else{
				RLDEBUG("local_web_thread:para stream busy\r\n");
				continue;
			}
			

			if (err < 0) {
				RLDEBUG("local_web_thread:recv data processing err\r\n");
				continue;
			}
		}



		memset(&web_msg_send, 0, sizeof(web_msg_send));
		web_msg_send.type = SENDMSG;
		cnt = (s32)web_pack((const para_stream*)&para_stream_t, (web_protocol_t*)&web_msg_send.data);
		para_stream_t.flag = PARA_STREAM_NORMAL;

		RLDEBUG("web send data:");
		hexdata_debug((s8*)&web_msg_send.data, cnt);

		if (msgsnd(msgid, (void *)&web_msg_send, cnt, 0) == -1)
		{
			RLDEBUG("web msgsed failed\n");
			continue;
			// exit(EXIT_FAILURE);
		}
	}

	/*从系统内核中移走消息队列*/
	// if (msgctl(msgid, IPC_RMID, 0) == -1)
	// {
	//     fprintf(stderr, "msgctl(IPC_RMID) failed\n");
	//     exit(EXIT_FAILURE);
	// }
	RLDEBUG("close web server\n");

}
#endif