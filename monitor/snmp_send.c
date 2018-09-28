#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <stdlib.h>

#include "snmp_send.h"
#include "helios.h"
#include "log.h"
#include "porting.h"
#include "mib.h"
#include "para_table.h"

#define SNMPMSGPATHNAME "/tmp/webmsg"  
#define ID 27 
#define SNMPOPENMODE IPC_CREAT|0666
#define SNMPMSG 3  
#define SNMP_APP_NAME "snmp_app"

struct snmp_msg_st snmp_msg_t;
int snmp_msg_id = 0;

void snmp_msg_init(void)
{
	key_t key;

	key = ftok(SNMPMSGPATHNAME, ID);  
	/*创建消息队列*/
	snmp_msg_id = msgget(key, SNMPOPENMODE);
	if (snmp_msg_id == -1)
	{
		RLDEBUG("msgget failed with error\n");
		exit(1);
	}
}

//检查snmp程序是否在线
u8 snmp_app_check(char * app_name)
{
	int ret = -1;
	char cmd[50];
	FILE *fstream=0;    
	char buff[5] = {0};  
 	
 	sprintf(cmd, "ps | grep -v \"grep\" | grep \"%s\"", app_name);
	
	if(0 == (fstream = popen(cmd, "r")))    
	{   
		fprintf(stderr,"execute command failed: %s", strerror(errno));    
		return -1;    
	}   

	while(NULL!=fgets(buff, sizeof(buff), fstream)) {
		if (atoi(buff) > 0) {
			ret = 1;
			break;
		}
	}
	pclose(fstream);
	
	return ret;
}

void snmp_msg_send(int id, char status)
{
	if(unit_para_t.unit_augment.snmp_t.snmp_sw != 1) return;

	// RLDEBUG("SNMP SW ON\n");
	
	if(snmp_app_check(SNMP_APP_NAME) != -1){
		RLDEBUG("SEND SNMP MSG DATA\n");
		snmp_msg_t.AlarmItemData.id = id;
		snmp_msg_t.AlarmItemData.status = status;
		snmp_msg_t.AlarmItemData.snmp_ver = unit_para_t.unit_augment.snmp_t.snmp_ver;
		memcpy(snmp_msg_t.AlarmItemData.snmp_ip, unit_para_t.unit_augment.snmp_t.snmp_ip, sizeof(snmp_msg_t.AlarmItemData.snmp_ip));
		snmp_msg_t.AlarmItemData.snmp_port = unit_para_t.unit_augment.snmp_t.snmp_port;
		memcpy(snmp_msg_t.AlarmItemData.snmp_community, unit_para_t.unit_augment.snmp_t.snmp_community, sizeof(snmp_msg_t.AlarmItemData.snmp_community));
		memcpy(snmp_msg_t.AlarmItemData.snmp_username, unit_para_t.unit_augment.snmp_t.snmp_username, sizeof(snmp_msg_t.AlarmItemData.snmp_username));
		snmp_msg_t.AlarmItemData.snmp_auth = unit_para_t.unit_augment.snmp_t.snmp_auth;
		memcpy(snmp_msg_t.AlarmItemData.snmp_authpass, unit_para_t.unit_augment.snmp_t.snmp_authpass, sizeof(snmp_msg_t.AlarmItemData.snmp_authpass));
		snmp_msg_t.AlarmItemData.snmp_priv = unit_para_t.unit_augment.snmp_t.snmp_priv;
		memcpy(snmp_msg_t.AlarmItemData.snmp_privpass, unit_para_t.unit_augment.snmp_t.snmp_privpass, sizeof(snmp_msg_t.AlarmItemData.snmp_privpass));
		memcpy(snmp_msg_t.AlarmItemData.snmp_engine_id, unit_para_t.unit_augment.snmp_t.snmp_engine_id, sizeof(snmp_msg_t.AlarmItemData.snmp_engine_id));

		snmp_msg_t.msg_type = SNMPMSG;
		if (msgsnd(snmp_msg_id, (void *)&snmp_msg_t, sizeof(struct AlarmItem), 0) == -1)
		{
			RLDEBUG("snmp msgsed failed\n");
		}
	}else{
		RLDEBUG("NO SNMP APP\n");
	}
}
