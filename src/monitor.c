/*
 * File      : monitor.c
 * 
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-30      sujj      first version
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/un.h> 
#include "helios.h"
#include "log.h"

#define WEBCLIENTCNT 10
#define MAXDATASIZE 1024
#define WEB_SOCK_FILE "../web_socket"

pthread_t monitor_tid;
pthread_attr_t monitor_attr;
pthread_t local_web_tid;
pthread_attr_t local_web_attr;

int web_clientfd[WEBCLIENTCNT] = {0};

int socket_init()
{
    int sockfd; 
    struct sockaddr_un sun;

    if((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1){
        LOG_DEBUG("socket create error");
        exit(1);
    }

    //LOG_DEBUG("socket success!,sockfd = %d\n", sockfd);

    memset(&sun, 0, sizeof(sun));
    sun.sun_family = AF_LOCAL;
    strcpy (sun.sun_path, WEB_SOCK_FILE);    

    if(bind(sockfd, (struct sockaddr *)&sun, sizeof(sun)) == -1){
        LOG_DEBUG("bind error ");
        exit(1);
    }

    //LOG_DEBUG("bind success!\n");
    if(listen(sockfd, WEBCLIENTCNT) == -1){
        LOG_DEBUG("listen error");
        exit(1);
    }
    LOG_DEBUG("listening....\n");

    return sockfd;
}

void client_deal(fd_set rset, fd_set mainset, u16 ready)
{
    socklen_t recvbytes, sendbytes;
    u16 i;
    int client_fd;
    char buf[MAXDATASIZE];

    for(i=0; i<WEBCLIENTCNT; ++i){
        client_fd = web_clientfd[i];
        if(FD_ISSET(client_fd, &rset)){
            if((recvbytes = recv(client_fd, buf, MAXDATASIZE, 0)) == -1){
                LOG_DEBUG("recv error");
                continue;
            }

            if(recvbytes == 0){
                close(client_fd); 
                FD_CLR(client_fd, &mainset);
                web_clientfd[i] = 0;
                LOG_DEBUG("client:%d off!\n", client_fd);
            }else{
                buf[recvbytes] = 0;

                LOG_DEBUG("client:%d input\n%s\n", client_fd, buf);

                if((sendbytes = send(client_fd, (void *)buf, strlen(buf), 0)) == -1){
                    LOG_DEBUG("send error ");
                    continue;
                }
            }

            if(--ready == 0) break;
        }
    }
}

void *local_web_thread(void *arg)
{
    struct sockaddr_un cun;
    socklen_t client_size;
    int sockfd, client_fd, maxfd;
    fd_set rset, mainset;
    u16 ready, i;

    LOG_DEBUG("start local web thread...\r\n"); 

    unlink(WEB_SOCK_FILE);  

    sockfd = socket_init();

    client_size = sizeof(struct sockaddr);
    FD_ZERO(&mainset);
    FD_SET(sockfd, &mainset);
    maxfd = sockfd;
    while(1){
        rset = mainset;
        ready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if(ready == -1){
            LOG_DEBUG("select error");
            continue;
        }

        if(FD_ISSET(sockfd, &rset)){
            if((client_fd = accept(sockfd, (struct sockaddr *)&cun, &client_size)) != -1){
                LOG_DEBUG("client:%d enter!\n", client_fd);

                for(i=0; i<WEBCLIENTCNT; ++i){
                    if(web_clientfd[i] <= 0) {
                        web_clientfd[i] = client_fd;
                        break;
                    }
                }

                if(i < WEBCLIENTCNT){
                    FD_SET(client_fd, &mainset);

                    if(client_fd > maxfd){
                        maxfd = client_fd;
                    }
                }


            }
            if(--ready == 0) continue;
        }

        client_deal(rset, mainset, ready);
    }

    close(sockfd);
    unlink (WEB_SOCK_FILE);  
    LOG_DEBUG("close web server\n");

}

void *monitor_thread(void *arg)
{
    s32 err;

    LOG_DEBUG("start monitor thread...\r\n"); 

    //modem Remote controls

    //snmp

    //local web server
    pthread_attr_init(&local_web_attr);
    pthread_attr_setdetachstate(&local_web_attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&local_web_tid, &local_web_attr, local_web_thread, NULL);
    if (err < 0) {
        LOG_DEBUG("creat local web thread false!\r\n");
    }

    while(1){
        //monitor 
        sleep(1);
    }

}
