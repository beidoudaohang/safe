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
#include "digmod.h"
#include "monitor.h"
#include "pamod.h"

void *monitor_thread(void *arg);
void *pamod_thread(void *arg);
void *web_thread(void *arg);


int main(int argc, char const *argv[])
{
    s32 err;

    LOG_DEBUG("start system...\r\n"); 

    pthread_attr_init(&digmod_attr);
    pthread_attr_setdetachstate(&digmod_attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&digmod_tid, &digmod_attr, digmod_thread, NULL);
    if (err < 0) {
        LOG_DEBUG("creat digmod thread false!\r\n");
    }

    pthread_attr_init(&monitor_attr);
    pthread_attr_setdetachstate(&monitor_attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&monitor_tid, &monitor_attr, monitor_thread, NULL);
    if (err < 0) {
        LOG_DEBUG("creat monitor thread false!\r\n");
    }

    pthread_attr_init(&pamod_attr);
    pthread_attr_setdetachstate(&pamod_attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&pamod_tid, &pamod_attr, pamod_thread, NULL);
    if (err < 0) {
        LOG_DEBUG("creat pamod thread false!\r\n");
    }

    while(1){
        sleep(1);
    }


    return 0;
    
}

