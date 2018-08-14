#ifndef __SNMP_SEND_H__
#define __SNMP_SEND_H__
#include <pthread.h>
#include <semaphore.h>
extern pthread_t monitor_tid;
extern pthread_attr_t monitor_attr;

#define SNMP_VERSION_V2 2
#define SNMP_VERSION_V3 3

#define SNMP_AUTH_MD5 0
#define SNMP_AUTH_SHA 1

#define SNMP_PRIV_DES 0
#define SNMP_PRIV_AES_128 1
#define SNMP_PRIV_AES_192 2
#define SNMP_PRIV_AES_256 3

struct AlarmItem
{
	int id;
	char status;
	unsigned char snmp_ver;
	char snmp_ip[21];
	unsigned short int snmp_port;
	char snmp_community[21];
	char snmp_username[21];
	unsigned char snmp_auth;
	char snmp_authpass[21];
	unsigned char snmp_priv;
	char snmp_privpass[21];
	char snmp_engine_id[26];
};
struct snmp_msg_st
{
	long int msg_type;
	struct AlarmItem AlarmItemData;
};

void snmp_msg_init(void);
void snmp_msg_send(int id, char status);
#endif //__SNMP_SEND_H__