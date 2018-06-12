/*
 * File      : pamod_task.c
 * 
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-30      sujj      first version
*/
#include <pthread.h>
#include <unistd.h>
#include "helios.h"
#include "log.h"

pthread_t pamod_tid;
pthread_attr_t pamod_attr;

s32 ul_pa_temp_monitor()
{
    //TODO: temp monitor

    //get temp val

    //cheak temp 


    return 0;
}

s32 dl_pa_temp_monitor()
{
    //TODO: temp monitor

    //get temp val

    //cheak temp 


    return 0;
}

s32 ul_pa_alarm_deal()
{
    //TODO: alarm deal
    return 0;
}

s32 dl_pa_alarm_deal()
{
    //TODO: alarm deal
    return 0;
}

void *pamod_thread(void *arg)
{
    // RLDEBUG("start pamod thread...\r\n"); 


    while(1){
        ul_pa_temp_monitor();
        dl_pa_temp_monitor();
        ul_pa_alarm_deal();
        dl_pa_alarm_deal();
        //pa sw control
        sleep(1);
    }

}