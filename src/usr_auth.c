/*
name:usr_auth.c
author:luke
date:20170517
ver:1.0
description:
	main file
*/

/*****************************include files***********************/
#include <string.h>

//os path include file
#include <sys/time.h>
#include <signal.h>
#include "log.h"
#include "helios.h"
#include "usr_def.h"
#include "para_table.h"
#include "porting.h"
#include "protocol_def.h"
#include "md5.h"
#include "common_api.h"

/*****************************para define*************************/
//u8 PRO_DEBUG_ENABLE=1;
//u8 DEVICE_DEBUG_ENABLE=1;
/*****************************data struct define******************/

/*****************************functions***************************/
/*
fun：查找用户名对应的权限
para：
	name：用户名
return：
	>0---用户名对应的权限
	-1---err
*/
s32 usr_name_to_auth(s8* name)
{
	s32 err;

	if (NULL == name) {
		return -1;
	}

	for (err = 0; err < USR_GROUP_SIZE; err++) {
		if (!strncmp((char*)name, (char*)(pcb_share.usr.usr[err].name), USR_NAME_SIZE)) {
			break;
		}
	}
	if (err >= USR_GROUP_SIZE) {
		return -1;
	}

	return (err + 1);
}

s8 autorize_update(frame *pf)
{
	if (NULL == pf)
		return -1;

	if (USR_LOCAL == pf->cmd.authority) {
		if (USR_UNLOGIN != usr_auth.authorize) {
			usr_auth.timeout = USR_AUTHORIZE_TIME;
		} else {
			return -1;
		}
	}

	return 0;
}
/*
fun：产生动态密码
para：
	auth：用户权限
	sn：产品序列号
	dest：存储动态密码
return：
	-1:err
	0:sus
 */
s32 usr_dynamic_pwd_creat(const s8* auth, const s8* sn, s8* dest)
{
	s8 buf[24];
	s32 err;

	if (NULL == auth)
		return -1;
	if (NULL == sn)
		return -1;
	if (NULL == dest)
		return -1;

	memset(buf, 0, sizeof(buf));
	memcpy(buf, auth, 1);
	memcpy(buf + 1, sn, 15);

	err = md5_make(buf, 16, dest);
	if (err < 0) {
		return -1;
	}

	return 0;
}

/*
fun：重置密码
para：
	name：用户名
	old_pwd：旧密码
	new_pwd：新密码
	auth：用户权限
return：
	-1：用户名或者密码为空
	-2：用户不存在
	-3：用户未登录
	-4：旧密码错误
	-5:err
	0:sus
*/
s32 usr_pw_changed(const s8* name, const s8* old_pwd, const s8* new_pwd, u8 auth)
{
	s32 err;

	if ((NULL == name) || (NULL == old_pwd) || (NULL == new_pwd)) {
		return -1;
	}

	err = usr_name_to_auth((s8*) name);
	if (err < 0) {
		return -2;
	}

	if (0XFF == auth) {	//local
		//check old pwd
		if (usr_auth.authorize >= (u32)err) {
			if (!strncmp((const char*)(pcb_share.usr.usr[err - 1].pass), (const char*)old_pwd, USR_PASS_SIZE)) {
				memcpy(pcb_share.usr.usr[err - 1].pass, new_pwd, USR_PASS_SIZE);
			} else {
				return -4;
			}
		} else {
			return -5;
		}
	} else if (auth >= (u8)err) {	//remote
		if (!strncmp((const char*)(pcb_share.usr.usr[err - 1].pass), (const char*)old_pwd, USR_PASS_SIZE)) {
			memcpy(pcb_share.usr.usr[err - 1].pass, new_pwd, USR_PASS_SIZE);
		} else {
			return -4;
		}
	} else {
		return -5;
	}

	return 0;
}

/*
fun：用户登录
para：
	pf：登录信息
return：
	0---sus
	-1：用户不存在
	-2：密码错误
	-3: err
 */
s32 usr_login(frame *pf)
{
	s32 cnt = 0;
	s8 dy_pwd[12] = {0};
	s8* sn = NULL;
	s32 err;

	if (NULL == pf) {
		return -1;
	}

	err = usr_name_to_auth((s8*)(pf->cmd.data + 1));
	if (err < 0) {
		return -1;
	}
	if (!(pf->cmd.data[0])) {
		//固定密码校验
		cnt = err - 1;
		if ((!strncmp((const char*)(pf->cmd.data + 1), (const char*)(pcb_share.usr.usr[cnt].name), USR_NAME_SIZE)) && \
		        (!strncmp((const char*)((pf->cmd.data + 1) + USR_NAME_SIZE), (const char*)(pcb_share.usr.usr[cnt].pass), USR_PASS_SIZE))) {
			RLDEBUG("usr_login:name & pwd match! \r\n");
		} else {
			//动态密码校验
			//find mod
#if MONITOR_MODULE_ENABLE
			if (((pf->cmd.md_adr.mod_type) == (unit_para_t.md_adr_t.mod_type)) && ((pf->cmd.md_adr.mod_band) == (unit_para_t.md_adr_t.mod_band)) && ((pf->cmd.md_adr.mod_adr_t.dat) == (unit_para_t.md_adr_t.mod_adr_t.dat)))
				sn = (s8*) & (unit_para_t.u_mfrs.sn);
#endif
			if (NULL == sn) {
				cnt = md_adr_to_index(&(pf->cmd.md_adr));
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
			if (strncmp((const char*)dy_pwd, (const char*)((pf->cmd.data + 1) + USR_NAME_SIZE), 8)) {
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

	RLDEBUG("usr_login: usr name:%s,usr authorize:%d \r\n", (pf->cmd.data + 1), usr_auth.authorize);

	return 0;
}
/*
fun：授权计时
para：
return：
 */
void usr_auth_lapse(void)
{
	if (USR_UNLOGIN != usr_auth.authorize) {
		if (usr_auth.timeout > 0) {
			usr_auth.timeout--;
		} else {
			usr_auth.authorize = USR_UNLOGIN;
			usr_auth.timeout = 0;
		}
	}
}
#if 0
/*
fun：定时器句柄
para：
	sig：信号
return：
 */
extern s32 alarm_led_update(void);
void it_handler(s32 sig)
{
	sig = sig;

	//RLDEBUG("it_handler call \r\n");
	//usr auther
	usr_auth_lapse();

	//led update
	alarm_led_update();
}
/*
fun：定时器初始化
para：

return：
	-1：err
	0:sus
*/
s32 itimer_init(void)
{
	s32 err;
	struct itimerval it_val;
	static struct sigaction sig_it_alarm;

	memset(&it_val, 0, sizeof(it_val));
	it_val.it_value.tv_sec = 1;
	it_val.it_value.tv_usec = 0;
	it_val.it_interval.tv_sec = 1;
	it_val.it_interval.tv_usec = 0;

	memset(&sig_it_alarm, 0, sizeof(sig_it_alarm));
	sigemptyset(&sig_it_alarm.sa_mask);
	sig_it_alarm.sa_flags = 0;
	sig_it_alarm.sa_handler = it_handler;

	err = sigaction(SIGALRM, &sig_it_alarm, NULL);
	if (err < 0) {
		RLDEBUG("itimer_init:sigaction() false\r\n");
		return -1;
	}

	err = setitimer(ITIMER_REAL, &it_val, NULL);
	if (err < 0) {
		RLDEBUG("itimer_init:setitimer() false\r\n");
		return -1;
	}

	return 0;
}
#endif