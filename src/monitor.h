/*
 * File      : monitor.h
 * 
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-30      sujj      first version
*/
#ifndef __MONITOR_H__
#define __MONITOR_H__
#include <pthread.h>

extern pthread_t monitor_tid;
extern pthread_attr_t monitor_attr;

void *monitor_thread(void *arg);


#endif //__MONITOR_H__