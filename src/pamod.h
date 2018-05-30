/*
 * File      : pamod.h
 * 
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-30      sujj      first version
*/
#ifndef __PAMOD_H__
#define __PAMOD_H__
#include <pthread.h>

extern pthread_t pamod_tid;
extern pthread_attr_t pamod_attr;

void *pamod_thread(void *arg);

#endif //__PAMOD_H__