#include <sys/types.h>   
#include <sys/socket.h>   
#include <sys/sysinfo.h>  
#include <sys/ioctl.h>
#include <stdint.h>
#include <sys/vfs.h> 
#include <netinet/in.h>   
#include <arpa/inet.h>
#include <netinet/tcp.h>     
#include <unistd.h>   
#include <stdlib.h>   
#include <stdio.h>   
#include <fcntl.h>
#include <poll.h>
#include <linux/types.h>
#include <printcolor.h>
#define PORT 6557   


int tcp_ui()  
{ 
	fd_set   t_set1,t_set_server;
	struct timeval  tv;
	int i_state,*pak_type,*p_read;
	//重启后立马可以使用端口
	int server_sockfd = socket(AF_INET, SOCK_STREAM, 0),ifor,ret;
	const int on =1;
	setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &on,sizeof (on)); 
	//重启后立马可以使用端口

	//判断对方否否发生意外断开
	int keepAlive = 1; // 开启keepalive属性  
	int keepIdle = 3; // 如该连接在60秒内没有任何数据往来,则进行探测   
	int keepInterval = 3; // 探测时发包的时间间隔为5 秒  
	int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.  

	setsockopt(server_sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));  
	setsockopt(server_sockfd, SOL_TCP, TCP_KEEPIDLE, (void*)&keepIdle, sizeof(keepIdle));  
	setsockopt(server_sockfd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));  
	setsockopt(server_sockfd, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
	//判断对方否否发生意外断开

	int pthread_kill_err;
	
	struct sockaddr_in server_addr;  
	server_addr.sin_family = AF_INET;  
	server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	server_addr.sin_port = htons(PORT);  
	if(bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))==-1){
	perror("Bind error.");
	exit(1);
	};  
	/* listen */  
	listen(server_sockfd, 5);  
	char readfromsocke[1024]; //网络数据第一手缓存
	int client_sockfd[5],on_connect=1,i_connect=0;
	struct sockaddr_in client_addr;  
	socklen_t len = sizeof(client_addr);  
	while(1)  
	{
		client_sockfd[i_connect] = accept(server_sockfd, (struct sockaddr *)&client_addr, &len); 
		on_connect=1; 
		while(on_connect)
		{
			FD_ZERO(&t_set1);
			FD_SET(client_sockfd[i_connect], &t_set1);
			tv.tv_sec= 10;
			tv.tv_usec= 1000;
			i_state= 0;
			i_state= select(client_sockfd[i_connect] +1, &t_set1, NULL, NULL, &tv);
			if (i_state < 0) {
				printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
				return -1;
			};

			if (i_state > 0){
				//sleep(1);
				//printf("i_Nread_time=%d\n",i_read_time);
				ssize_t readLen=read(client_sockfd[i_connect],readfromsocke, 4);
				printf(BLUE"readLen=%d\n"NONE, readLen);
				if(readLen<=0){
					on_connect=0;
					continue;
				}
				pak_type=(int *)readfromsocke;
				p_read=pak_type;
				//printf("get char from client: %s real read %ld byte\n", ch,readLen);  
				printf("get NUM from client: %d real read %ld byte\n", *pak_type,readLen);  
				printf("pak_type: %d *(pak_type+1) %d \n", *pak_type,*(pak_type+1));  
				switch(*pak_type) 
					{
					case 0:
					{
					printf(RED"1!\n"NONE);
					break;
					}
					default:
					printf(RED"2\n"NONE);
					break;
					}
			}
			else{
				printf(RED"no data recive!\n"NONE);
			}

			
		}
		close(client_sockfd[i_connect]);
	}
	printf(GREEN"YOU GUESS WHAT ? IT WORKS ,WE JUST FINISH MY JOB--TCP SERVER!\n"NONE);
	printf("EXIT TCP SERVER\n");
    return 0;  
} 
