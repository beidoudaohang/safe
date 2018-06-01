/*
 * File      : digmod_task.c
 * 
 *dig module
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-30      sujj      first version
*/
#include <pthread.h>
#include <unistd.h>
#include "helios.h"
#include "log.h"

pthread_t digmod_tid;
pthread_attr_t digmod_attr;

s32 fpga_load()
{
    //TODO: FPGA Load
    return 0;
}

s32 fpga_config()
{
    //TODO: FPGA Config
    return 0;
}

s32 fpga_monitor()
{
    //TODO: FPGA Monitor
    return 0;
}

s32 ad9370_config()
{
    //TODO: ad9370 Config
    return 0;
}

s32 dig_temp_monitor()
{
    //TODO: temp monitor

    //get temp val

    //cheak temp 


    return 0;
}

s32 dig_alarm_deal()
{
    //TODO: alarm deal
    return 0;
}

void *digmod_thread(void *arg)
{
    RLDEBUG("start digmod thread...\r\n"); 

    while(fpga_load()){
        RLDEBUG("fpga load fail!");
    }

    while(fpga_config()){
        RLDEBUG("fpga config fail!");
    }

    while(fpga_config()){
        RLDEBUG("fpga config fail!");
    }

    while(fpga_monitor()){
        RLDEBUG("fpga monitor fail!");
    }

    ad9370_config();

    while(1){
        dig_temp_monitor();

        dig_alarm_deal();

        sleep(1);
    }

}