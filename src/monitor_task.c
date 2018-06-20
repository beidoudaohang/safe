/*
 * File      : monitor_task.c
 * 
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-30      sujj      first version
*/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#include "helios.h"
#include "log.h"
#include "web_protocol.h"

pthread_t monitor_tid;
pthread_attr_t monitor_attr;
pthread_t local_web_tid;
pthread_attr_t local_web_attr;

void *monitor_thread(void *arg)
{
    s32 err;

    // RLDEBUG("start monitor thread...\r\n"); 

    //modem Remote controls

    //local web server
    pthread_attr_init(&local_web_attr);
    pthread_attr_setdetachstate(&local_web_attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&local_web_tid, &local_web_attr, local_web_thread, NULL);
    if (err < 0) {
        RLDEBUG("creat local web thread false!\r\n");
    }

    while(1){
        //monitor 
        sleep(1);
    }

}
