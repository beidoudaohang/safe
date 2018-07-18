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

#include "para_table.h"
#include "helios.h"
#include "log.h"
#include "web_protocol.h"
#include "porting.h"
#include "Module_Comm.h"

pthread_t monitor_tid;
pthread_attr_t monitor_attr;
pthread_t local_web_tid;
pthread_attr_t local_web_attr;
pthread_t rs485_tid;
pthread_attr_t rs485_attr;

/* #define RS485_SRC "\x7E\x13\x02\x11\x00\x00\xB3\x73\x7F"

void *rs485_thread(void *arg)
{
    u8 str[100];
    u16 len;

    if(rs485_tty_open()) {
        RLDEBUG("rs485 open tty faild\r\n");
        return ;
    }

    while(1){

        RS485_SEND(RS485_SRC, sizeof(RS485_SRC));

        len = RS485_RECV(str, sizeof(str), 300);
     
        if(len > 0){
            fflush(stdin);
        }

        timedelay(0, 1, 0, 0);
    }
} */
//extern band_para exmod_para_a[MONITOR_MOD_NUM];
void debug_freq()
{
    u8 i=0;
    u8 index=0;
    for(i=0; i<MONITOR_MOD_NUM; ++i){
        if(exmod_para_a[i].md_adr_t.mod_type == MOD_TYPE_DIG){
            index = i;
            break;
        }
    }
    
    if(i >= MONITOR_MOD_NUM) return;

    RLDEBUG("band:%d\r\n", exmod_para_a[index].md_adr_t.mod_band);

    RLDEBUG("ul freq\t\t\tdl freq\tul att\tdl att\r\n");
    for(i=0; i< FREQ_CHANNEL_NUMS_MAX; ++i){
        if(exmod_dynamic_para_a[index].ch_rf_t.feature[i].FEATURE.enable)
        RLDEBUG("%f\t\t%f\t%d\t%d\r\n", exmod_para_a[index].ch_info_t.ul[i].workfreq, 
            exmod_para_a[index].ch_info_t.dl[i].workfreq, 
            exmod_para_a[index].ch_info_t.ul[i].att,
            exmod_para_a[index].ch_info_t.dl[i].att);
    }

}

void debug_info()
{
    u8 i=0;
    u8 index=0;
    for(i=0; i<MONITOR_MOD_NUM; ++i){
        if(exmod_para_a[i].md_adr_t.mod_type == MOD_TYPE_PA){
            index = i;
            break;
        }
    }
    
    if(i >= MONITOR_MOD_NUM) return;

    RLDEBUG("band:%d\r\n", exmod_para_a[index].md_adr_t.mod_band);

    RLDEBUG("PA TEMP: %d\r\n", exmod_dynamic_para_a[index].md_dynamic_sundry.temperature);
    RLDEBUG("PA ATT: %d\r\n", exmod_para_a[index].md_basic.att);
}

void *monitor_thread(void *arg)
{
    s32 err;
    u16 i;
    // RLDEBUG("start monitor thread...\r\n"); 

    //modem Remote controls

    //local web server
    pthread_attr_init(&local_web_attr);
    pthread_attr_setdetachstate(&local_web_attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&local_web_tid, &local_web_attr, local_web_thread, NULL);
    if (err < 0) {
        RLDEBUG("creat local web thread false!\r\n");
    }

    pthread_attr_init(&rs485_attr);
    pthread_attr_setdetachstate(&rs485_attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&rs485_tid, &rs485_attr, rs485_thread, NULL);
    if (err < 0) {
        RLDEBUG("creat rs485 thread false!\r\n");
    }

    while(1){
        //monitor 
        sleep(15);
        if(get_rs485_mod_init_state()){
            debug_freq();
            debug_info();
        }
    }

}
