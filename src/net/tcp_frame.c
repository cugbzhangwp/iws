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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <libgen.h>
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
#include <oled.h>
#include <printcolor.h>
#include <tcp_frame.h>
#include <common_tcp.h>
#include <runconf.h>
//#include <common_tcp.h>
#define MAXEVENTS 1024
#define BUF_SIZE 100
#define tcp_frame_locat() printf(RED"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__)

extern void get_hight_Pri(int cpux);
extern int set_tcp_opt(int client_sockfd,int server_index);
//set this descriptor non blocking
static int set_non_block(int sfd){
    int flags, s;

    flags = fcntl(sfd, F_GETFL, 0); 
    if (flags == -1) {
        perror ("fcntl");
        return -1; 
    }   

    flags |= O_NONBLOCK;
    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1) {
        perror ("fcntl");
        return -1; 
    }   

    return 0;
}



int set_host_opt(int client_sockfd)
{
    const int on =1;
    int keepAlive = 1; // 开启keepalive属性  
    int keepIdle = 3; // 如该连接在60秒内没有任何数据往来,则进行探测   
    int keepInterval = 5; // 探测时发包的时间间隔为5 秒  
    int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.  
    int ret;
    struct timeval timeout={3,0};//3s
    ret=setsockopt(client_sockfd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
    ret=setsockopt(client_sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
    if(ret!=0)
    {
        perror("err:");
        exit(0);
    }
    //重启后立马可以使用端口
    setsockopt(client_sockfd, SOL_SOCKET, SO_REUSEADDR, &on,sizeof (on)); 
   // 重启后立马可以使用端口
    //判断对方否否发生意外断开
    setsockopt(client_sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));  
    setsockopt(client_sockfd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));  
    setsockopt(client_sockfd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));  
    setsockopt(client_sockfd, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
    //判断对方否否发生意外断开*/
    return 0;
}













int set_client_sev_opt(int client_sockfd)
{
    const int on =1;
    int keepAlive = 1; // 开启keepalive属性  
    int keepIdle = 3; // 如该连接在60秒内没有任何数据往来,则进行探测   
    int keepInterval = 5; // 探测时发包的时间间隔为5 秒  
    int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.  
    int ret;
    struct timeval timeout={3,0};//3s
    ret=setsockopt(client_sockfd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
    ret=setsockopt(client_sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
    if(ret!=0)
    {
        perror("err:");
        exit(0);
    }
    //重启后立马可以使用端口
    setsockopt(client_sockfd, SOL_SOCKET, SO_REUSEADDR, &on,sizeof (on)); 
   // 重启后立马可以使用端口
    //判断对方否否发生意外断开
    setsockopt(client_sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));  
    setsockopt(client_sockfd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));  
    setsockopt(client_sockfd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));  
    setsockopt(client_sockfd, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
    //判断对方否否发生意外断开*/
    return 0;
}










int set_client_client_opt(int client_sockfd,int server_index)
{
    const int on =1;
    int keepAlive = 1; // 开启keepalive属性  
    int keepIdle = 3; // 如该连接在60秒内没有任何数据往来,则进行探测   
    int keepInterval = 5; // 探测时发包的时间间隔为5 秒  
    int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.  
    int ret;
    struct timeval timeout={8,0};//3s
        //exit(0);
    int bNodelay=1;
    ret=setsockopt(client_sockfd,IPPROTO_TCP,TCP_NODELAY,(char *)&bNodelay,sizeof(bNodelay)); 
    printf("server_index=%d,TCP_NODELAY ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    ret=setsockopt(client_sockfd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
    //ret=setsockopt(client_sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
    printf("server_index=%d,SO_SNDTIMEO ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    //重启后立马可以使用端口
    ret=setsockopt(client_sockfd, SOL_SOCKET, SO_REUSEADDR, &on,sizeof (on)); 
    printf("server_index=%d,SO_REUSEADDR ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    //重启后立马可以使用端口
    //判断对方否否发生意外断开
    ret=setsockopt(client_sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));  
    printf("server_index=%d,SO_KEEPALIVE ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    ret=setsockopt(client_sockfd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));  
    printf("server_index=%d,TCP_KEEPIDLE ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    ret=setsockopt(client_sockfd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));  
    printf("server_index=%d,TCP_KEEPINTVL ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    ret=setsockopt(client_sockfd, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
    printf("server_index=%d,TCP_KEEPCNT ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    //判断对方否否发生意外断开*/
    return 0;
}










//
static int create_and_bind (char *port){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;
    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;     //IPv4 IPv6通用
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     //监听套接字；
    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    //根据暗示得到所有可用的addrinfo
    s = getaddrinfo (NULL, port, &hints, &result);
    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    if (s != 0){
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
        return -1;
    }
    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    //use the first addr to create socket
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;
        set_client_sev_opt(sfd);
        // const int on =1;
        // //重启后立马可以使用端口
        // setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on,sizeof (on));//it must be use before bind();
        s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
        if (s == 0) {
            //bind success
            break;
        }

        close (sfd);//if bind failed we close this socket
    }
    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

  if (rp == NULL) {
        fprintf (stderr, "Could not bind\n");
        return -1;
  }

  freeaddrinfo (result);
    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

  return sfd;
}

// add EPOLLIN event of fd to the epollfd, and epoll uses EPOLLLT in default
void addfd(int epollfd, int fd, int enable_et){
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLOUT;
    if(enable_et)
        event.events |= EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    //set_non_block(fd);
}




// add EPOLLIN event of fd to the epollfd, and epoll uses EPOLLLT in default
void addfd_et(int epollfd, int fd, int enable_et){
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLOUT;
    if(enable_et)
        event.events |= EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    set_non_block(fd);
}




int writenbytes(char * ch,int sockfd,int needread)
{   
    int readLen,nextreadcount;
    nextreadcount=readLen=0;
    while(nextreadcount!=needread){
        readLen=write(sockfd, ch+nextreadcount, needread-nextreadcount);
        if(readLen<=0){
            return -1;
        }
        nextreadcount+=readLen;
    }
    return nextreadcount;
}

int readnbytes(char * ch,int sockfd,int needread)
{   
    int readLen,nextreadcount;
    nextreadcount=readLen=0;
    while(nextreadcount!=needread){
        //printf(BLUE"inside %s，readlen=%d,needread=%d\n"NONE, __FUNCTION__,readLen,needread);
        readLen=read(sockfd, ch+nextreadcount, needread-nextreadcount);
        if(readLen<=0){
            return -1;
        }
        nextreadcount+=readLen;
    }
    return nextreadcount;
}

int essid_scan(char *buf)
{
FILE *fp;
//char buf[2000] = {0};
if((fp = popen("iwlist wlan0 scan", "r")) == NULL) {
perror("Fail to popen\n");
exit(1);
}

fread(buf, 1, 20480, fp);

//while(fgets(buf, 2000, fp) != NULL) {
//printf("%s", buf);
//}
pclose(fp);
return 0;
}




int pak_check(int size,char * buf)

{
    return 0;
}




//the level-trigger logic
void lt(struct epoll_event *events, int number, int epollfd, int listenfd ,short port,int (*FnName)(int,int, short,char *,void * ,void *),void * para,int connect_index){
    char buf[BUF_SIZE];
    char bufsend[20480];
    int framestart=0xaabbccdd;
    int framelen=200*sizeof(DATA_FRAME_GEO_S);
    APP_S * p_app;
    p_app=(APP_S *)para;
    char distr[80]={" hello epoll world"};
    int i,ifor,i_connect;
    int pak_len;
    int ret;
    //int connect_index;
    
    for(i = 0; i < number; i++){
        int sockfd = events[i].data.fd;
        //new client connection
        if(sockfd == listenfd){

            tcp_frame_locat();
            struct sockaddr_in client_addr;
            socklen_t client_addrlen = sizeof(client_addr);
            int connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addrlen);
            if(p_app->app_server.connect[connect_index].is_connect==1)
            {
                tcp_frame_locat();
                printf("connect_index=%d\n",connect_index);
                close(connfd);
                return 0;
            }
            struct timeval timeout={8,0};//3s
            ret=setsockopt(connfd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
            //ret=setsockopt(client_sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
            printf("server_index=%d,SO_SNDTIMEO ret=%d\n",connfd,ret);
            if(ret!=0)
            {
                printf("connfd=%d\n",connfd);
                perror("err:");
                //exit(0);
            }
            //set_client_client_opt(connfd,connfd);
            tcp_frame_locat();
            // for(ifor=0;ifor<10;ifor++)
            // {
            //    if(0==p_app->app_server.connect[ifor].is_request){
            //         p_app->app_server.connect[ifor].is_request=1;
            //         connect_index=ifor;
            //         //i_connect=ifor;
            //         printf("i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",connect_index,app->app_server.connect[ifor].is_connect);
            //         break;
            //    }
            // }
            //set_host_opt(listenfd);
            //set_host_opt(connfd);
            // set_tcp_opt(listenfd,100);
            // set_tcp_opt(connfd,101);
            printf(RED"\ninside %s line %d connect_index=%d\n"NONE, __FUNCTION__,__LINE__,connect_index);







                // for(ifor=0;ifor<10;ifor++)
                // {
                //    if(0==p_app->app_server.connect[ifor].is_connect){
                //         p_app->app_server.connect[ifor].is_connect=1;
                //         p_app->app_server.connect[ifor].sendbuf=p_app->buffer.databuf[LOOPREAD];
                //         p_app->app_server.connect[ifor].sig_send=0;
                //         i_connect=ifor;
                //         printf("client i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",i_connect,app->app_server.connect[ifor].is_connect);
                //         break;
                //    }
                // }






            p_app->app_server.connect[connect_index].is_connect=1;
            p_app->app_server.connect[connect_index].sig_send=0;
            p_app->app_server.connect[connect_index].sendbuf=p_app->buffer.databuf[LOOPREAD];
            //system("mplayer --no-video /home/pi/shero.mp3 &");
            //p_app->app_sig.sig_data_send=-1;
            //p_app->app_sig.sig_is_data_connect=1;
            //p_app->buffer.databuf[LOOPSEND]=p_app->buffer.databuf[LOOPREAD];
            if(connfd < 0){
                perror("accept failed..");
                //return -1;
                //exit(-1);
            }
            //register this event
            addfd(epollfd, connfd, 0);
        }else if(events[i].events & EPOLLIN){//当有数据可读时执行
            //if there exists data unreaded , this code will invoke
            memset(buf, 0, sizeof(buf)); 
            printf("sockefd %d read event trigger once\n", sockfd);
            //int ret = recv(sockfd, buf, BUF_SIZE, 0);
            ret = readfromsocket(buf,8,sockfd);
            pak_len=*(int *)(buf+4);
            if(pak_len>0){
                ret += readfromsocket(buf+ret,pak_len,sockfd);
            }
            printf(RED"inside %s line %d ret=%d pak_len=%d\n"NONE, __FUNCTION__,__LINE__,ret,pak_len);

            if((ret <1 )){
                printf("some read error or client closed.\n");
                p_app->app_server.connect[connect_index].is_connect=0;
                //p_app->app_server.already_server.connect[connect_index].sig_send=-1;
                //p_app->app_server.already_server.connect[connect_index].sendbuf=p_app->buffer.databuf[LOOPREAD];
                close(sockfd); // will unregister
                continue;
            }
            //snprintf(distr,80,"hello client %d this is epoll's world",sockfd);
            //disp_on_oled(1,buf);
            if(!pak_check(ret,buf)){//run call back
                FnName(ret, listenfd,port ,buf,(void * )buf,para);
            }
            //system("hostapd /etc/rtl_hostapd.conf -B");
            //system("./abollo-s &");
            //essid_scan(bufsend);

            //sleep(6);
            //printf("essid info=%s\n",bufsend);
            ret = send(sockfd, "OK", 2, 0);
            //ret=readfromsocket("buf",3,sockfd);
            printf("send %d bytes from client: %s\n", ret, buf);
        }
        else if(events[i].events & EPOLLOUT){//当有数据可读时执行
            int realwrite;
            while(p_app->app_server.connect[connect_index].sig_send<=0)
                {
                    usleep(250*1000);
                    //
                    //wait_go_signal(1);
                }
            #define reversebit(x,y)  x^=(1<<y)
            #define NET_LED 5
            if(p_app->app_sig.iswork){
                p_app->app_sig.iswork=0;
                reversebit(p_app->pca9539[1],NET_LED);
            }
            framelen=p_app->app_para.fs*sizeof(DATA_FRAME_GEO_S);
            realwrite=writenbytes(&framestart,sockfd,4);
            if (realwrite < 0) {
                printf("realwrite=%d\n",realwrite);
                if (errno == EWOULDBLOCK) {
                    printf("errno=%d\n",errno);
                    //exit(0);
                }
                printf("out errno=%d\n",errno);
                //exit(0);
            }
            realwrite=writenbytes(&framelen,sockfd,4);
            if (realwrite < 0) {
                printf("realwrite=%d\n",realwrite);
                if (errno == EWOULDBLOCK) {
                    printf("errno=%d\n",errno);
                    //exit(0);
                }
                printf("out errno=%d\n",errno);
                //exit(0);
            }

            //printf(BLUE"p_app->app_server.connect[connect_index].sendbuf->index=%d\n"NONE,p_app->app_server.connect[connect_index].sendbuf->loopbufindex);
            //printf(RED"inside function：%s,line : %d,!p_app->ping_pang=%d,p_app->AdcNow[7]=%d\n"NONE,__FUNCTION__,__LINE__,!p_app->ping_pang,p_app->AdcNow[7]);
            realwrite=writenbytes(p_app->app_server.connect[connect_index].sendbuf->data_start,sockfd,p_app->app_para.fs*sizeof(DATA_FRAME_GEO_S));
            if (realwrite < 0) {
                printf("realwrite=%d\n",realwrite);
                if (errno == EWOULDBLOCK) {
                    printf("errno=%d\n",errno);
                    //exit(0);
                }
                printf("out errno=%d\n",errno);
                //exit(0);
            }

            //printf(BLUE"p_app->app_para.fs=%d,p_app->app_server.connect[connect_index].sendbuf->index=%d\n"NONE,p_app->app_para.fs,p_app->app_server.connect[connect_index].sendbuf->loopbufindex);


            //write(sockfd,p_app->app_server.connect[connect_index].sendbuf->data_start,200*sizeof(DATA_FRAME_GEO_S));
            p_app->app_server.connect[connect_index].sig_send--;
            //printf(GREEN"function：%s,line:%d,app.app_sig.sig_data_send=%d\n"NONE,__FUNCTION__,__LINE__,p_app->app_sig.sig_data_send);
            p_app->app_server.connect[connect_index].sendbuf=p_app->app_server.connect[connect_index].sendbuf->loopbufnext;

        }
        else{
            // what we are not interested
            printf(GREEN"something else happened\n"NONE);
        }
    }
}




//the edge-trigger logic
void et(struct epoll_event *events, int number, int epollfd, int listenfd,short port,int (*FnName)(int,int,short,char *,void * ,void *),void * para){
    char buf[BUF_SIZE];
    int i;
    for(i = 0; i < number; i++){
        int sockfd = events[i].data.fd;
        //new client connection
        if(sockfd == listenfd){
            struct sockaddr_in client_addr;
            socklen_t client_addrlen = sizeof(client_addr);
            int connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addrlen);
            if(connfd < 0){
                perror("accept failed..");
                exit(-1);
            }
            //register this event and set edge trigger mode
            addfd(epollfd, connfd, 1);
        }else if(events[i].events & EPOLLIN){
            //in this mode , we should read all the data out once
            printf("sockefd %d read event trigger once\n", sockfd);
            while(1){
                memset(buf, 0, sizeof(buf));
                //int ret = recv(sockfd, buf, BUF_SIZE, 0);
                int ret=readfromsocket(buf,1,sockfd);
                if(ret < 0){
                    //for the nonblocking IO,the following errors indicate the read complete
                    if(errno == EAGAIN || errno == EWOULDBLOCK){
                        printf("read later\n");
                        break;
                    }
                    printf("some other error\n");
                    close(sockfd); // will unregister
                    break;
                }else if(ret == 0){
                    printf("client closed.\n");
                    close(sockfd);
                    break;
                }else{
                    printf("get %d bytes from client: %s\n", ret, buf);
                    if(!pak_check(ret,buf)){//run call back
                        FnName(ret, sockfd,port ,buf,(void * )buf,para);
                        write(sockfd,"OK1", 3);
                    }
                }
            }
        }else{
            // what we are not interested
            printf("something else happened\n");
        }
    }
}


//the edge-trigger logic
void et_1(struct epoll_event *events, int number, int epollfd, int listenfd,short port,int (*FnName)(int,int,short,char *,void * ,void *),void * para){
    char buf[BUF_SIZE];
    int i;
    for(i = 0; i < number; i++){
        int sockfd = events[i].data.fd;
        //new client connection
        if(sockfd == listenfd){
            struct sockaddr_in client_addr;
            socklen_t client_addrlen = sizeof(client_addr);
            int connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addrlen);
            if(connfd < 0){
                perror("accept failed..");
                exit(-1);
            }
            //register this event and set edge trigger mode
            addfd(epollfd, connfd, 1);
        }else if(events[i].events & EPOLLIN){
            //in this mode , we should read all the data out once
            printf("sockefd %d read event trigger once\n", sockfd);
            while(1){
                memset(buf, 0, sizeof(buf));
                //int ret = recv(sockfd, buf, BUF_SIZE, 0);
                int ret=readfromsocket(buf,1,sockfd);
                if(ret < 0){
                    //for the nonblocking IO,the following errors indicate the read complete
                    if(errno == EAGAIN || errno == EWOULDBLOCK){
                        printf("read later\n");
                        break;
                    }
                    printf("some other error\n");
                    close(sockfd); // will unregister
                    break;
                }else if(ret == 0){
                    printf("client closed.\n");
                    close(sockfd);
                    break;
                }else{
                    printf("get %d bytes from client: %s\n", ret, buf);
                    if(!pak_check(ret,buf)){//run call back
                        FnName(ret, sockfd,port ,buf,(void * )buf,para);
                        write(sockfd,"OK1", 3);
                    }
                }
            }
        }else{
            // what we are not interested
            printf("something else happened\n");
        }
    }
}



int start_a_tcp_ser (unsigned short port,int (*FnName)(int,int,short,char *,void *,APP_S * app),void * para)//data_server
{
    printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
    APP_S * app;
    app=(APP_S *)para;
    int connect_index=app->app_server.already_server;
    int ifor;
    for(ifor=0;ifor<10;ifor++)
    {
       if(0==app->app_server.connect[ifor].is_request){
            app->app_server.connect[ifor].is_request=1;
            connect_index=ifor;
            //i_connect=ifor;
            printf("i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",connect_index,app->app_server.connect[ifor].is_connect);
            break;
       }
    }
    app->app_server.already_server++;
    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event events[MAXEVENTS];
    // unsigned short tcp_port;
    // sscanf(port,"d",&tcp_port);
    char tcp_port[10];
    printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    get_hight_Pri(1);
    snprintf(tcp_port,10,"%d",port);
    printf(GREEN"inside %s line %d tcp_port=%s\n"NONE, __FUNCTION__,__LINE__,tcp_port);

    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    sfd = create_and_bind (tcp_port);
 
    if (sfd == -1)
        abort ();

    //listen for connection coming
    s = listen (sfd, SOMAXCONN);
    if (s == -1) {
        perror ("listen");
        abort ();
    }

    //create a epoll object
    efd = epoll_create1 (0);
    if (efd == -1) {
        perror ("epoll_create");
        abort ();
    }
    //printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    //add the listen socket to the event poll
    addfd(efd, sfd, 0);

    while(1){
        int ret = epoll_wait(efd, events, MAXEVENTS, 10);
        if(ret < 0){
            //printf("epoll wait failture..\n");
            break;
        }
        //et(events, ret, efd, sfd);
        //printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
        lt(events, ret, efd, sfd,port,FnName,para,connect_index);
    }
    //printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    app->app_server.connect[connect_index].is_request=0;
    close (sfd);

    return EXIT_SUCCESS;
}













//the level-trigger logic
void lt_bandpass(struct epoll_event *events, int number, int epollfd, int listenfd ,short port,int (*FnName)(int,int, short,char *,void * ,void *),void * para,int connect_index){
    char buf[BUF_SIZE];
    char bufsend[20480];
    int framestart=0xaabbccdd;
    int framelen=200*sizeof(DATA_FRAME_GEO_S);
    APP_S * p_app;
    p_app=(APP_S *)para;
    char distr[80]={" hello epoll world"};
    int i;
    int pak_len;
    int ret;
    for(i = 0; i < number; i++){
        int sockfd = events[i].data.fd;
        //new client connection
        if(sockfd == listenfd){
            struct sockaddr_in client_addr;
            socklen_t client_addrlen = sizeof(client_addr);
            int connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addrlen);
            set_host_opt(listenfd);
            set_host_opt(connfd);
            printf(RED"\ninside %s line %d connect_index=%d\n"NONE, __FUNCTION__,__LINE__,connect_index);

            p_app->app_server.connect[connect_index].is_connect=1;
            p_app->app_server.connect[connect_index].sig_send=0;
            p_app->app_server.connect[connect_index].sendbuf=p_app->buffer.databuf[LOOPREAD];
            //p_app->app_sig.sig_data_send=-1;
            //p_app->app_sig.sig_is_data_connect=1;
            //p_app->buffer.databuf[LOOPSEND]=p_app->buffer.databuf[LOOPREAD];
            if(connfd < 0){
                perror("accept failed..");
                exit(-1);
            }
            //register this event
            addfd(epollfd, connfd, 0);
        }else if(events[i].events & EPOLLIN){//当有数据可读时执行
            //if there exists data unreaded , this code will invoke
            memset(buf, 0, sizeof(buf)); 
            printf("sockefd %d read event trigger once\n", sockfd);
            //int ret = recv(sockfd, buf, BUF_SIZE, 0);
            ret = readfromsocket(buf,8,sockfd);
            pak_len=*(int *)(buf+4);
            if(pak_len>0){
                ret += readfromsocket(buf+ret,pak_len,sockfd);
            }
            printf(RED"inside %s line %d ret=%d pak_len=%d\n"NONE, __FUNCTION__,__LINE__,ret,pak_len);

            if((ret <1 )){
                printf("some read error or client closed.\n");
                p_app->app_server.connect[connect_index].is_connect=0;
                //p_app->app_server.already_server.connect[connect_index].sig_send=-1;
                //p_app->app_server.already_server.connect[connect_index].sendbuf=p_app->buffer.databuf[LOOPREAD];
                close(sockfd); // will unregister
                continue;
            }
            //snprintf(distr,80,"hello client %d this is epoll's world",sockfd);
            //disp_on_oled(1,buf);
            if(!pak_check(ret,buf)){//run call back
                FnName(ret, listenfd,port ,buf,(void * )buf,para);
            }
            //system("hostapd /etc/rtl_hostapd.conf -B");
            //system("./abollo-s &");
            //essid_scan(bufsend);

            //sleep(6);
            //printf("essid info=%s\n",bufsend);
            ret = send(sockfd, "OK", 2, 0);
            //ret=readfromsocket("buf",3,sockfd);
            printf("send %d bytes from client: %s\n", ret, buf);
        }
        else if(events[i].events & EPOLLOUT){//当有数据可读时执行
            while(p_app->app_server.connect[connect_index].sig_send<=0)
                {
                    usleep(250*1000);
                    //
                    //wait_go_signal(1);
                }
            #define reversebit(x,y)  x^=(1<<y)
            #define NET_LED 5
            if(p_app->app_sig.iswork){
                p_app->app_sig.iswork=0;
                reversebit(p_app->pca9539[1],NET_LED);
            }
            framelen=p_app->app_para.fs*sizeof(DATA_FRAME_GEO_S);
            writenbytes(&framestart,sockfd,4);
            writenbytes(&framelen,sockfd,4);
            printf(BLUE"p_app->app_server.connect[connect_index].sendbuf->index=%d\n"NONE,p_app->app_server.connect[connect_index].sendbuf->loopbufindex);
            //printf(RED"inside function：%s,line : %d,!p_app->ping_pang=%d,p_app->AdcNow[7]=%d\n"NONE,__FUNCTION__,__LINE__,!p_app->ping_pang,p_app->AdcNow[7]);
            writenbytes(p_app->app_server.connect[connect_index].sendbuf->data_start,sockfd,p_app->app_para.fs*sizeof(DATA_FRAME_GEO_S));

            //write(sockfd,p_app->app_server.connect[connect_index].sendbuf->data_start,200*sizeof(DATA_FRAME_GEO_S));
            p_app->app_server.connect[connect_index].sig_send--;
            //printf(GREEN"function：%s,line:%d,app.app_sig.sig_data_send=%d\n"NONE,__FUNCTION__,__LINE__,p_app->app_sig.sig_data_send);
            p_app->app_server.connect[connect_index].sendbuf=p_app->app_server.connect[connect_index].sendbuf->loopbufnext;
        }
        else{
            // what we are not interested
            printf("something else happened\n");
        }
    }
}



int start_a_tcp_ser_bandpass (unsigned short port,int (*FnName)(int,int,short,char *,void *,APP_S * app),void * para)//data_server
{
    printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
    APP_S * app;
    app=(APP_S *)para;
    int connect_index=app->app_server.already_server;
    app->app_server.already_server++;
    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event events[MAXEVENTS];
    // unsigned short tcp_port;
    // sscanf(port,"d",&tcp_port);
    char tcp_port[10];
    printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    get_hight_Pri(1);
    snprintf(tcp_port,10,"%d",port);
    printf(GREEN"inside %s line %d tcp_port=%s\n"NONE, __FUNCTION__,__LINE__,tcp_port);

    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    sfd = create_and_bind (tcp_port);
    if (sfd == -1)
        abort ();

    //listen for connection coming
    s = listen (sfd, SOMAXCONN);
    if (s == -1) {
        perror ("listen");
        abort ();
    }

    //create a epoll object
    efd = epoll_create1 (0);
    if (efd == -1) {
        perror ("epoll_create");
        abort ();
    }
    //printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    //add the listen socket to the event poll
    addfd(efd, sfd, 0);

    while(1){
        int ret = epoll_wait(efd, events, MAXEVENTS, -1);
        if(ret < 0){
            //printf("epoll wait failture..\n");
            break;
        }
        //et(events, ret, efd, sfd);
        //printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
        lt_bandpass(events, ret, efd, sfd,port,FnName,para,connect_index);
    }
    //printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);


    close (sfd);

    return EXIT_SUCCESS;
}



















int start_a_tcp_ser_et (unsigned short port,int (*FnName)(int,int,short,char *,void *,APP_S * app),void * para)
{
    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event events[MAXEVENTS];
    char tcp_port[10];
    printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    snprintf(tcp_port,10,"%d",port);
    printf(GREEN"inside %s line %d tcp_port=%s\n"NONE, __FUNCTION__,__LINE__,tcp_port);

    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    sfd = create_and_bind (tcp_port);
    if (sfd == -1)
        abort ();

    //listen for connection coming
    s = listen (sfd, SOMAXCONN);
    if (s == -1) {
        perror ("listen");
        abort ();
    }

    //create a epoll object
    efd = epoll_create1 (0);
    if (efd == -1) {
        perror ("epoll_create");
        abort ();
    }
    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    //add the listen socket to the event poll
    addfd(efd, sfd, 1);

    while(1){
        int ret = epoll_wait(efd, events, MAXEVENTS, -1);
        if(ret < 0){
            printf("epoll wait failture..\n");
            break;
        }
        et(events, ret, efd, sfd,port,FnName,para);
        printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
        //lt(events, ret, efd, sfd,FnName,para);
    }
    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);


    close (sfd);

    return EXIT_SUCCESS;
}





//the level-trigger logic
int lt_data(struct epoll_event *events, int number, int epollfd, int listenfd ,short port,int (*FnName)(int,int, short,char *,void * ,void *),void * para){
    char buf[BUF_SIZE];
    char bufsend[20480];
    APP_S * app;
    app=(APP_S *)para;
    char distr[80]={" hello epoll world"};
    int i;
    int pak_len;
    int ret;
    for(i = 0; i < number; i++){
        int sockfd = events[i].data.fd;
        //new client connection
        if(sockfd == listenfd){
            struct sockaddr_in client_addr;
            socklen_t client_addrlen = sizeof(client_addr);
            int connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addrlen);
            if(connfd < 0){
                perror("accept failed..");
                exit(-1);
            }
            //register this event
            addfd(epollfd, connfd, 0);
        }else if(events[i].events & EPOLLIN){//当有数据可读时执行
            //if there exists data unreaded , this code will invoke
            memset(buf, 0, sizeof(buf)); 
            printf("sockefd %d read event trigger once\n", sockfd);
            //int ret = recv(sockfd, buf, BUF_SIZE, 0);
            ret = readfromsocket(buf,8,sockfd);
            pak_len=*(int *)(buf+4);
            if(pak_len>0){
                ret += readfromsocket(buf+ret,pak_len,sockfd);
            }
            printf(RED"inside %s line %d ret=%d pak_len=%d\n"NONE, __FUNCTION__,__LINE__,ret,pak_len);

            if((ret <1 )){
                printf("some read error or client closed.\n");
                close(sockfd); // will unregister
                continue;
            }
            //snprintf(distr,80,"hello client %d this is epoll's world",sockfd);
            //disp_on_oled(1,buf);
            if(!pak_check(ret,buf)){//run call back
                FnName(ret, listenfd,port ,buf,(void * )buf,para);
            }
            //system("hostapd /etc/rtl_hostapd.conf -B");
            //system("./abollo-s &");
            //essid_scan(bufsend);

            //sleep(6);
            //printf("essid info=%s\n",bufsend);
            ret = send(sockfd, "OK", 2, 0);
            //ret=readfromsocket("buf",3,sockfd);
            printf("send %d bytes from client: %s\n", ret, buf);
        }
        else if(events[i].events & EPOLLOUT){//当有数据可读时执行
            //printf("something can write\n");
            while(app->app_sig.sig_data_send<=0)
                {
                    app->buffer.databuf[LOOPSEND]=app->buffer.databuf[LOOPREAD];
                    wait_go_signal(1);
                }
            //p_app->AdcNow[7]--;
            //printf(RED"inside function：%s,line : %d,!p_app->ping_pang=%d,p_app->AdcNow[7]=%d\n"NONE,__FUNCTION__,__LINE__,!p_app->ping_pang,p_app->AdcNow[7]);
            writenbytes(0xAABBCCDD,sockfd,4);
            writenbytes(200*sizeof(DATA_FRAME_GEO_S),sockfd,4);

            writenbytes(app->buffer.databuf[LOOPSEND]->data_start,sockfd,200*sizeof(DATA_FRAME_GEO_S));
            //write(sockfd,app->buffer.databuf[LOOPSEND]->data_start,200*sizeof(DATA_FRAME_GEO_S));
            app->buffer.databuf[LOOPSEND]=app->buffer.databuf[LOOPSEND]->loopbufnext;
            //ret = send(sockfd, &p_app->AdcNow[7], 4, 0);
            //sleep(1);
        }
        else{
            // what we are not interested
            printf("something else happened\n");
        }
    }
}



int tcp_server_frame (unsigned short port,APP_S * app,int (*lt_frame)(struct epoll_event *, int, int, int,short,int (*FnName)(int,int, short,char *,void * ,void *),void *),int (*FnName)(int,int,short,char *,void *,APP_S * app),void * para)
{
    printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event events[MAXEVENTS];
    // unsigned short tcp_port;
    // sscanf(port,"d",&tcp_port);
    char tcp_port[10];
    printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    snprintf(tcp_port,10,"%d",port);
    printf(GREEN"inside %s line %d tcp_port=%s\n"NONE, __FUNCTION__,__LINE__,tcp_port);

    printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    sfd = create_and_bind (tcp_port);
    if (sfd == -1)
        abort ();

    //listen for connection coming
    s = listen (sfd, SOMAXCONN);

    if (s == -1) {
        perror ("listen");
        abort ();
    }

    //create a epoll object
    efd = epoll_create1 (0);
    if (efd == -1) {
        perror ("epoll_create");
        abort ();
    }
    //printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

    //add the listen socket to the event poll
    addfd(efd, sfd, 0);

    while(1){
        int ret = epoll_wait(efd, events, MAXEVENTS, -1);
        if(ret < 0){
            //printf("epoll wait failture..\n");
            break;
        }
        //et(events, ret, efd, sfd);
        //printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
        lt_frame(events, ret, efd, sfd,port,FnName,para);
    }
    //printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);


    close (sfd);

    return EXIT_SUCCESS;
}


#define BUFSIZE 666
#define SERV_PORT 161
#define OPEN_MAX 1024


/*
bogon:pi zhangwp$ nmap -sT 222.222.119.11

Starting Nmap 7.31 ( https://nmap.org ) at 2018-06-01 23:12 CST
Nmap scan report for 222.222.119.11
Host is up (0.018s latency).
Not shown: 984 closed ports
PORT     STATE    SERVICE
21/tcp   open     ftp
80/tcp   open     http
85/tcp   open     mit-ml-dev
135/tcp  filtered msrpc
139/tcp  filtered netbios-ssn
161/tcp  open     snmp
445/tcp  filtered microsoft-ds
593/tcp  filtered http-rpc-epmap
1000/tcp open     cadlock
2601/tcp filtered zebra
2602/tcp filtered ripd
2604/tcp filtered ospfd
3306/tcp filtered mysql
4444/tcp filtered krb524
6666/tcp open     irc
6667/tcp filtered irc
*/
int iws_client_frame(APP_S * app)
{
    struct sockaddr_in servaddr;
    char buf[BUFSIZE];
    int sockfd, n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //inet_pton(AF_INET, "222.222.119.11", &servaddr.sin_addr);
    inet_pton(AF_INET, "192.168.12.24", &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);

    /*连接服务端*/
    printf("try to connect\n");
    int ret=connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("end ret=%d\n",ret);
    while(fgets(buf, BUFSIZE, stdin) != NULL){
        /*通过sockfd给服务端发送数据*/
        write(sockfd, buf, strlen(buf));
        n = read(sockfd, buf, BUFSIZE);
        if(n == 0)
            printf("the other side has been closed.\n");
        else/*打印输出服务端传过来的数据*/
            write(STDOUT_FILENO, buf, n);
    }

    close(sockfd);
    return 0;
}















int gpsd_client_frame()
{
    struct sockaddr_in servaddr;
    char buf[BUFSIZE];
    int sockfd, n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //inet_pton(AF_INET, "222.222.119.11", &servaddr.sin_addr);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(2947);

    /*连接服务端*/
    printf("try to connect\n");
    int ret=connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(ret<0)
        exit(ret);
    printf("end ret=%d\n",ret);
    int ifor;
    while(1){
        /*通过sockfd给服务端发送数据*/
        //write(sockfd, buf, strlen(buf));
        n = read(sockfd, buf, BUFSIZE);
        printf("read %d bytes\n",n);
        for(ifor=0;ifor<n;ifor++){
            printf("%X",buf[ifor]);
        }
        printf("\n");
    }

    close(sockfd);
    return 0;
}



























int iws_client( APP_S * app)
{
    //IWS_CSTP iws_cstp;
    printf("in iws_client ping_pang=%d\n",app->ping_pang);
    int ifor,i_connect;
            // for(ifor=0;ifor<10;ifor++)
            // {
            //    //if(0==app->app_server.connect[ifor].is_connect){
            //         app.app_server.connect[ifor].is_connect=1;
            //         //app->app_server.connect[ifor].sendbuf=app->buffer.databuf[LOOPREAD];
            //         i_connect=ifor;
            //         printf("i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",i_connect,app.app_server.connect[ifor].is_connect);
            //         //break;
            //    //}
            // }
    while(1){
            for(ifor=0;ifor<10;ifor++)
            {
               //if(0==app->app_server.connect[ifor].is_connect){
                    //app.app_server.connect[ifor].is_connect=1;
                    //app->app_server.connect[ifor].sendbuf=app->buffer.databuf[LOOPREAD];
                    i_connect=ifor;
                    printf("i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",i_connect,app->app_server.connect[ifor].is_connect);
                    //break;
               //}
            }
            exit(0);
    }
}








