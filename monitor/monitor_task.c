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
#include "porting.h"

pthread_t monitor_tid;
pthread_attr_t monitor_attr;
pthread_t local_web_tid;
pthread_attr_t local_web_attr;
pthread_t rs485_tid;
pthread_attr_t rs485_attr;

#define RS485_SRC "test rs485\r\n"

void *rs485_thread(void *arg)
{
    u8 str[100];
    u16 len;

    if(rs485_tty_open()) {
        RLDEBUG("rs485 open tty faild\r\n");
        return ;
    }

    while(1){
        // RLDEBUG(RS485_SRC);
        // RS485_SEND(RS485_SRC, strlen(RS485_SRC));
        // timedelay(0, 5, 0, 0);
        len = RS485_RECV_TEST(str, sizeof(str));
        if(len > 0){
            RLDEBUG(str);
            fflush(stdin);
        }
    }
}


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

    pthread_attr_init(&rs485_attr);
    pthread_attr_setdetachstate(&rs485_attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&rs485_tid, &rs485_attr, rs485_thread, NULL);
    if (err < 0) {
        RLDEBUG("creat rs485 thread false!\r\n");
    }

    while(1){
        //monitor 
        sleep(1);
    }

}
