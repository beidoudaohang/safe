/*
name:protocol_sem.h
author:luke
date:20170612
ver:1.0
description:
	module parameter dat struct
*/
#ifndef PROTOCOL_SEM_H_
#define PROTOCOL_SEM_H_

/*****************************include files***********************/
#include "helios.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
/*****************************para define*************************/
#define PROTOCOL_RECV_SEM "protocol_recv_sem"
#define PROTOCOL_SEND_SEM "protocol_send_sem"
/*****************************data struct define******************/
typedef struct
{
	pthread_mutex_t protocol_recv_mutex;
	pthread_mutexattr_t protocol_recv_mtxattr;

	sem_t protocol_recv_sem;
	sem_t protocol_send_sem;
} protocol_sem;

/*****************************funs********************************/
extern protocol_sem protocol_sem_t;


#endif //PROTOCOL_SEM_H_
#