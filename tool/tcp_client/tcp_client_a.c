

    #include <sys/types.h>   
    #include <sys/socket.h>   
    #include <netinet/in.h>   
    #include <arpa/inet.h>   
    #include <unistd.h>   
    #include <stdlib.h>   
    #include <stdio.h>   
      
    #define PORT 6558   
      
    int main(int argc,char ** argv)  
    {  
      /* create a socket */  
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);  
      FILE * fp;
      struct sockaddr_in address;  
      address.sin_family = AF_INET;  
      address.sin_addr.s_addr = inet_addr(argv[1]);  
	short port;
	char ch[64000];  
	//fp=fopen("/mnt/sd/recvfromtcp.txt","wb");
	fp=fopen("recvfromtcp.txt","wb");
	int i_read_time,from_stdin[2],readLen,realsend;
				if(argc>2)
				{
					sscanf(argv[2],"%hd",&port);
					address.sin_port = htons(port);  
					      /* connect to the server */  
					      int result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));  
					      if(result == -1)  
					      {  
						perror("connect failed: ");  
						exit(1);  
					      }  
		
					      /* exchange data */  
						while(1)
						{
						readLen=read(sockfd, ch, 64000);  
						//realsend=write(sockfd, ch, 470);  
						//ch[readLen]='\0';
						fwrite(ch,readLen,1,fp);
					      	printf("get %d byte from server: %hd\n",readLen, ch+2);  
						}
				}
				else{
					      address.sin_port = htons(PORT);  
		
					      /* connect to the server */  
					      int result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));  
					      if(result == -1)  
					      {  
						perror("connect failed: ");  
						exit(1);  
					      }  
		
					      /* exchange data */  

						for(i_read_time=0;i_read_time<10;i_read_time++)
						{
						printf("enter pak type:");
						scanf("%s",ch);
						//from_stdin=*ch;
						sscanf(ch,"%d",&from_stdin[0]);
						printf("enter data:");
						scanf("%s",ch);
						//from_stdin=*ch;
						sscanf(ch,"%d",&from_stdin[1]);
						printf("the num read from u are:%d\n",*from_stdin);
					     printf("char length: %lu\n", strlen(ch));  
					      write(sockfd, &from_stdin, 8);  
						printf("the ch is %s\n",ch);
					      readLen=read(sockfd, &ch, 512);  
						ch[readLen]='\0';
					      printf("get char from server: %s\n", ch);  
						switch(from_stdin[0])
							{
							case 0:
								switch(from_stdin[1])
								{
									case 0:
									break;
									case 512:
									printf("uptime=%lu\n",*(int *)(ch+16));
									break;
									default:
									break;
								}
							break;
							case 256:
							break;
							default:
							break;
							}
						}
					      /* close the socket */  
			}
      close(sockfd);  
	fclose(fp);
      return 0;  
    }  
