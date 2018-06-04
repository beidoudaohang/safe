/*
 * File      : main.c
 * 
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-30      sujj      first version
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "log.h"
#include "digmod_task.h"
#include "monitor_task.h"
#include "pamod_task.h"



int main(int argc, char const *argv[])
{
    s32 err;

    RLDEBUG("start system...\r\n"); 

    pthread_attr_init(&digmod_attr);
    pthread_attr_setdetachstate(&digmod_attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&digmod_tid, &digmod_attr, digmod_thread, NULL);
    if (err < 0) {
        RLDEBUG("creat digmod thread false!\r\n");
    }

    pthread_attr_init(&monitor_attr);
    pthread_attr_setdetachstate(&monitor_attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&monitor_tid, &monitor_attr, monitor_thread, NULL);
    if (err < 0) {
        RLDEBUG("creat monitor thread false!\r\n");
    }

    pthread_attr_init(&pamod_attr);
    pthread_attr_setdetachstate(&pamod_attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&pamod_tid, &pamod_attr, pamod_thread, NULL);
    if (err < 0) {
        RLDEBUG("creat pamod thread false!\r\n");
    }

    while(1){
        sleep(1);
    }


    return 0;
    
}

