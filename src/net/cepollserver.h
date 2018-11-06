#ifndef  C_EPOLL_SERVER_H  
#define  C_EPOLL_SERVER_H  
#include <unistd.h>
#include <sys/epoll.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <fcntl.h>  
#include <arpa/inet.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <iostream>  
#include <pthread.h>
#include <memory.h>
#include <string.h>
  
#define _MAX_SOCKFD_COUNT 65535  
  
class CEpollServer  
{  
        public:  
                CEpollServer();  
                ~CEpollServer();  
  
                bool InitServer(const char* chIp, int iPort);  
                void Listen();  
                static void ListenThread( void* lpVoid );  
                void Run();  
  
        private:  
                int        m_iEpollFd;  
                int        m_isock;  
                pthread_t       m_ListenThreadId;// 监听线程句柄  
  
};  
  
#endif  
