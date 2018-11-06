#include <app_main.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <sys/wait.h>  
#include <sys/types.h>  

#include <string.h>
#include <unistd.h> /* for close() */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <atcmd.h>
#include <printcolor.h>
#include <app_main.h>
#include <pca9539_relay.h>
#include <ltc2943.h>

int udp_frame(APP_S * app,short port,int (*FnName)())
{
    int server_fd, ret;
    struct sockaddr_in ser_addr; 

    server_fd = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
    if(server_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    ser_addr.sin_port = htons(port);  //端口号，需要网络序转换

    // const int on =1;
    // //重启后立马可以使用端口
    // setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on,sizeof (on));//it must be use before bind();



    ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if(ret < 0)
    {
        printf(" udp_frame socket bind fail!\n");
        //return -1;
    }

    //handle_udp_msg(server_fd);   //处理接收到的数据
    (*FnName)(server_fd);
    close(server_fd);
    return 0;
}
//int (*lt_frame)(struct epoll_event *, int, int, int,short,int (*FnName)(int,int, short,char *,void * ,void *),void *)