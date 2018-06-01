/*
 * File      : digmod_task.h
 * 
 *dig module
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-30      sujj      first version
*/
#ifndef __DIGMOD_H__
#define __DIGMOD_H__
#include <pthread.h>

extern pthread_t digmod_tid;
extern pthread_attr_t digmod_attr;

void *digmod_thread(void *arg);

#endif //__DIGMOD_H__