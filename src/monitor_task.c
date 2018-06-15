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
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "helios.h"
#include "log.h"
#include "web_protocol.h"

pthread_t monitor_tid;
pthread_attr_t monitor_attr;
pthread_t local_web_tid;
pthread_attr_t local_web_attr;


void *local_web_thread(void *arg)
{
    int msgid;
    web_msg_t web_msg;
    //long msg_to_receive = 0;

    /*创建消息队列*/
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        RLDEBUG("msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    /*循环从消息队列中接收消息*/
    while (1)
    {
        /*读取消息*/
        if (msgrcv(msgid, (void *)&web_msg, sizeof(web_protocol_t), 0, 0) == -1)
        {
            RLDEBUG("msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }

        // printf("cmd:%d, len=%d, You wrote: %s", web_msg.msg_cmd, web_msg.prodata.len, web_msg.prodata.data);
    }

    /*从系统内核中移走消息队列*/
    // if (msgctl(msgid, IPC_RMID, 0) == -1)
    // {
    //     fprintf(stderr, "msgctl(IPC_RMID) failed\n");
    //     exit(EXIT_FAILURE);
    // }
    RLDEBUG("close web server\n");

}

void *monitor_thread(void *arg)
{
    s32 err;

    // RLDEBUG("start monitor thread...\r\n"); 

    //modem Remote controls

    //snmp

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
