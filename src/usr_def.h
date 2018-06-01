/*
name:usr_def.h
author:luke
date:20170323
ver:1.0
description:
	usr dat struct
*/
#ifndef USR_DEF_H_
#define USR_DEF_H_

/*****************************include files***********************/
#include "helios.h"
#include "protocol_def.h"

/*****************************para define*************************/
#define USR_NAME_SIZE 20
#define USR_PASS_SIZE 20
#define USR_GROUP_SIZE 4

#define USR_AUTHORIZE_TIME		(0xffff)

/*user authority define*/
typedef enum
{
	USR_UNLOGIN = 0,
	USR_TERMINAL = 1,		/*终端用户权限*/
	USR_OPERATOR = 2,		/*运营商用户权限*/
	USR_AGENT = 3,		/*代理商用户权限*/
	USR_MANUFACTURER = 4,	/*工厂用户权限*/
	USR_LOCAL = 0XFF		/*本控用户权限*/
} USR_AUTHORITY;

typedef struct
{
	u8 name[USR_NAME_SIZE];
	u8 pass[USR_PASS_SIZE];
} usr_item;

typedef struct {
	usr_item usr[USR_GROUP_SIZE];
} usr_group;

/*usr authority*/
typedef struct {
	u32 timeout;
	USR_AUTHORITY authorize;
} usr_authorize;

/*****************************data struct define******************/

/*****************************funs********************************/
s8 autorize_update(frame *pf);
s32 usr_login(frame *pf);
void usr_auth_lapse(void);
s32 itimer_init(void);

#endif //USR_DEF_H_