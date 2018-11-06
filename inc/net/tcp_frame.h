#ifndef _TCP_FRAME_H
#define _TCP_FRAME_H
#include <runconf.h>
#include <app_main.h>
#include <sys/epoll.h>
//extern int start_a_tcp_ser (char * port,int (*FnName)(int, char *,void *,APP_S * app),void * para);
extern int start_a_tcp_ser (unsigned short port,int (*FnName)(int,int,short,char *,void *,APP_S * app),void * para);
extern int start_a_tcp_ser_et (unsigned short port,int (*FnName)(int,int,short,char *,void *,APP_S * app),void * para);
extern int lt_data(struct epoll_event *events, int number, int epollfd, int listenfd ,short port,int (*FnName)(int,int, short,char *,void * ,void *),void * para);
		//
#endif