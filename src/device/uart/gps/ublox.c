/*******************************************************
*ilename:receive.c
* Description:Receive data from Serial_Port
* Date:
*******************************************************/
/*********************头文件定义***********************/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h> /* These are the usual header files */
#include <string.h>
#include <unistd.h> /* for close() */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <termios.h>
#include <zwpdebug.h>
#include <printcolor.h>
#include <app_main.h>
#include <ublox.h>
#include <wiringPi.h>
#include <pca9539_relay.h>
extern APP_S app;

#define max_buffer_size 100
static int s_gpsOK_1 = 0, s_gpsOK_2 = 0;
char gps_buff[512];
struct sockaddr_in host_gps_addr; /* cilent's address information */
static int gps_checksum( void );
static void gps_proc_frame( void );
static void gps_gpgga( void );
static void gps_gprmc( void );
void writetofile( void );
static void proc_gps( char *buff, int length );
void proc_args( int argc, char **argv );
void proc_error( void );
void main_therad_alarm( int sig );
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
int poll_msg(unsigned char * nav_poll,int N,int rr,int ( * fun_callback)(char *,int));
FILE *fi;
int gps_debug = (0);
int timesync;  
int real;      
GPGGA abl_gps_gga;//abollo GPS GPGGA
DATE_TIME mydate_now;




int printgps_info();


struct gps_data_t gps_data;
struct gps_data_t gps_bak;	
struct tm tm_time;			



#define PPS 24

//void ( * call_back_func )(void);
void pps_handle()
{
	printf(GREEN"YES GET PPS\n"NONE);
	return 0;
}








int gpsd_test()
{
//sudo gpsd -G -D 5 -N -n /dev/ttyUSB1


}



int register_pps_interupt()
{	
    pinMode(PPS,INPUT);
    pullUpDnControl(PPS,PUD_UP);
    if(wiringPiISR(PPS,INT_EDGE_FALLING,pps_handle) < 0)
    {
            printf("Unable to setup ISR \n");
    }
    // pinMode(isr1,INPUT);
    // pullUpDnControl(isr1,PUD_UP);
    // if(wiringPiISR(isr1,INT_EDGE_RISING,call_back_func) < 0)
    // {
    //         printf("Unable to setup ISR \n");
    // }
    printf("Interrupt test program\n");
 //    while(1){
 //    	//printf("count=%d\n",icount++);
 //    	//bsp_DelayUS(1000000);
 //    	sleep(9999);
	// }
    // while(1)
    // {
    //         if(flag)
    //         {
    //                 while(digitalRead(button) ==0);
    //                 printf("button press rdy_count=%d\n",rdy_count);
    //                 flag = 0;
    //         }
    // }
}

typedef struct gps_ubx_write{
	unsigned int start_flag;
	int pak_type;
	int ubx_conf_len;
	char reserve[20];
	unsigned char ubx[96];
}__attribute__ ((packed,aligned(1)))gps_ubx_write_s;


int check_sum_ubx(unsigned char * ubx,int len)
{
	;
}

int conf_ubx_to_gps(unsigned char * ubx,int len)
{
	;
}


inline int udp_recvnbytes(int sockfd,char * ch,int needread,struct sockaddr * cilent)
{	
	int readLen,nextreadcount;
	nextreadcount=readLen=0;
	socklen_t sin_size;
	//sin_size=sizeof(struct sockaddr_in);
	while(nextreadcount!=needread){
		readLen=recvfrom(sockfd, ch+nextreadcount, needread-nextreadcount,0,cilent,&sin_size);
		printf(BLUE"inside %s，readlen=%d,needread=%d\n"NONE, __FUNCTION__,readLen,needread);

		if(readLen<=0){
			return -1;
		}
		nextreadcount+=readLen;
	}
	return nextreadcount;
}


int udp_sendnbytes(int sockfd,char * ch,int needread,struct sockaddr_in * cilent)
{	
	int readLen,nextreadcount;
	struct sockaddr_in test;
	socklen_t addr_len;
	//len = sizeof(clent_addr);
	addr_len = sizeof(struct sockaddr_in);

	nextreadcount=readLen=0;
	while(nextreadcount!=needread){
		//printf(BLUE"inside %s，readlen=%d,needread=%d\n"NONE, __FUNCTION__,readLen,needread);
		readLen=sendto(sockfd,ch+nextreadcount,needread-nextreadcount,0,cilent,addr_len);
		if(readLen<=0){
			return -1;
		}
		nextreadcount+=readLen;
	}
	return nextreadcount;
}






















void handle_udp_gps(int fd)
{
	//int sendnbytes(int sockfd,char * ch,int needread)
	app.app_udp_socket[UDP_GPS].fid=fd;
	#define BUFF_LEN 1024
    char buf[BUFF_LEN];  //接收缓冲区，1024字节
    socklen_t len;
    int count;
    struct sockaddr_in clent_addr;  //clent_addr用于记录发送方的地址信息
    int ifor;
    while(app.thread_switch.main_loop)
    {
        //memset(buf, 0, BUFF_LEN);
        len = sizeof(clent_addr);
        count = recvfrom(app.app_udp_socket[UDP_GPS].fid, buf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
        if(count == -1)
        {
            printf("recieve data fail!\n");
            continue;
        }
		
		printf("count=%d\n",count);
		//sendto(app.app_udp_socket[UDP_GPS].fid,"START NMEA MSG\n",strlen("START NMEA MSG\n"),0,(struct sockaddr*)&clent_addr, len);
		gps_ubx_write_s * ubx=(gps_ubx_write_s *)buf;

		if(ubx->start_flag==0xaabbccdd){

			if(!check_sum_ubx(&ubx->ubx,ubx->ubx_conf_len)){
				if(!ubx->ubx_conf_len){
					conf_ubx_to_gps(&ubx->ubx,ubx->ubx_conf_len);
				}
				memcpy(&host_gps_addr,&clent_addr,sizeof(clent_addr));
			}
			//sendto(myoso->fids[FID_GPS_MQ],"err config",strlen("err config"),0,(struct sockaddr *)&cilent,sin_size);
			memcpy(&host_gps_addr,&clent_addr,sizeof(clent_addr));
		}
		else{
			printf(RED"change host\n"NONE);
			memcpy(&host_gps_addr,&clent_addr,sizeof(clent_addr));
			app.app_sign.has_gps_mq_ready=1;	
			sendto(app.app_udp_socket[UDP_GPS].fid,"will send ubx&nmea message to you",strlen("will send ubx&nmea message to you"),0,(struct sockaddr *)&clent_addr,len);
		}


    }
}


void handle_gpsd_gps_nmea(int fd)
{
	//int sendnbytes(int sockfd,char * ch,int needread)
	app.app_udp_socket[GPSD_GPS].fid=fd;
	#define BUFF_LEN 1024
    char buf[BUFF_LEN];  //接收缓冲区，1024字节
    socklen_t len;
    int count;
    struct sockaddr_in clent_addr;  //clent_addr用于记录发送方的地址信息
    int ifor;
    while(app.thread_switch.main_loop)
    {
        //memset(buf, 0, BUFF_LEN);
        len = sizeof(clent_addr);
        count = recvfrom(app.app_udp_socket[GPSD_GPS].fid, buf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
        if(count == -1)
        {
            printf("recieve data fail!\n");
            continue;
        }
		
		// printf("count=%d\n",count);
		// printf("%s\n",buf);
		proc_gps(buf, count);
		//sendto(app.app_udp_socket[UDP_GPS].fid,"START NMEA MSG\n",strlen("START NMEA MSG\n"),0,(struct sockaddr*)&clent_addr, len);
		gps_ubx_write_s * ubx=(gps_ubx_write_s *)buf;

		// if(ubx->start_flag==0xaabbccdd){

		// 	if(!check_sum_ubx(&ubx->ubx,ubx->ubx_conf_len)){
		// 		if(!ubx->ubx_conf_len){
		// 			conf_ubx_to_gps(&ubx->ubx,ubx->ubx_conf_len);
		// 		}
		// 		memcpy(&host_gps_addr,&clent_addr,sizeof(clent_addr));
		// 	}
		// 	//sendto(myoso->fids[FID_GPS_MQ],"err config",strlen("err config"),0,(struct sockaddr *)&cilent,sin_size);
		// 	memcpy(&host_gps_addr,&clent_addr,sizeof(clent_addr));
		// }
		// else{
		// 	printf(RED"change host\n"NONE);
		// 	memcpy(&host_gps_addr,&clent_addr,sizeof(clent_addr));
		// 	app.app_sign.has_gps_mq_ready=1;	
		// 	sendto(app.app_udp_socket[GPSD_GPS].fid,"will send ubx&nmea message to you",strlen("will send ubx&nmea message to you"),0,(struct sockaddr *)&clent_addr,len);
		// }


    }
}







void sig_term( int sig )
{
	sig = sig;
	fclose(fi);
	close(app.app_fid[GPS_FID].fid);
	exit(0);
}

int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio, oldtio;
	if  (tcgetattr(fd, &oldtio)  !=  0) 
	{ 
		perror("SetupSerial 1");
		return -1;
	}
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag |= CLOCAL | CREAD; 
	newtio.c_cflag &= ~CSIZE; 

	switch( nBits )
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	switch( nEvent )
	{
	case 'O':                    
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E':                     
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':                    
		newtio.c_cflag &= ~PARENB;
		break;
	}

	switch( nSpeed )
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}
	if( nStop == 1 )
	{
		newtio.c_cflag &=  ~CSTOPB;
	}
	else if ( nStop == 2 )
	{
		newtio.c_cflag |=  CSTOPB;
	}

	newtio.c_cflag &= ~CRTSCTS;	
	newtio.c_iflag = 0;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME]  = 20;
	newtio.c_cc[VMIN] = 0;

	tcflush(fd, TCIFLUSH);
	if(tcsetattr(fd, TCSANOW, &newtio) != 0)
	{
		//perror("com set error");
		write(2, "Searial Set Error\n", 19);
		return -1;
	}
	//printf("set done!\n");
	write(2, "Set Done\n", 10);
	return 0;
}











int GpsThread()
{
	//fflush(fi);
	const int rr=100;
	char rbuf[rr];
	int retv=0;
	int ifor;
	int bc;
/*
	char nav_timeutc[]={0xB5,0x62,0x01,0x21,0x00,0x00,0x22,0x67};//获取时间是否有效
	//fwrite(nav_timeutc, 8,1,fi);
	write(fd,nav_timeutc,sizeof(nav_timeutc));


	while(retv!=rr)
		{
		retv+=read(fd,rbuf,rr-retv);
		printf(BLUE"RETV=%d\n"NONE,retv);
		}

	for(ifor=0;ifor<rr;ifor++)
		{
		printf("%02X ",rbuf[ifor]);
		}
	printf("\n");

*/
	fi = fdopen(app.app_fid[GPS_FID].fid, "r");//将设备文件转化成流文件
	//opentowrite();//打开要保存的文件
	//tcflush(fd, TCIOFLUSH);
	printf("inside %s\n", __FUNCTION__);
	//while (app.thread_switch.main_loop && app.thread_switch.gps_loop)//总循环打开，gps循环打开

	while (1)//总循环打开，gps循环打开
	{

		//pthread_mutex_lock(&mut_cp_gps_info);
		//isneedupdate_gps_info=1;
		//printf(BLUE"inside %s %s\n", __FUNCTION__,gps_buff);

		memset(&gps_buff, 0, sizeof(gps_buff));
		fgets(gps_buff, sizeof(gps_buff), fi);
		printf("%s",gps_buff);
		//printf(RED"%d\n"NONE,pthread_mutex_unlock(&mut_cp_gps_info));


		proc_gps(gps_buff, strlen(gps_buff));
		bc=(bc==0)?1:0;
	/*
		if(bc==0){
		ioctl(fdio,GPIO_IOC_N_GPIO_UP,AT91_PIN_PD19);
		}
		else{
		ioctl(fdio,GPIO_IOC_N_GPIO_DOWN,AT91_PIN_PD19);
		}

	*/

		//printf(RED"%s"NONE,gps_buff);

		//printf(YELLOW"inside %s line%s\n"NONE, __FUNCTION__,__LINE__);
		if(gps_buff[0]!='$'){//UBX协议返回内容
			printf(RED"!!!!!!!!!!!!!!!!!!!!!>%s strlen=%d<!!!!!!!!!!!!!!!!!!!!\n"NONE,gps_buff,strlen(gps_buff));
			for(ifor=0;ifor<28;ifor++)
				{
					printf("%02X ",gps_buff[ifor]);
				}
				printf("\n");
		}
		//printf(YELLOW"inside %s\n"NONE, __FUNCTION__);
		if (s_gpsOK_1 & s_gpsOK_2){
			s_gpsOK_1 = 0;
			s_gpsOK_2 = 0;
		}
		
	}
	return 0;
}






char sbuff[256];
int s_index=-1;
//
//
static void proc_gps( char *buff, int length )
{
	int i,ifor;
	//printf(YELLOW"inside %s buff=%s\n"NONE, __FUNCTION__,buff);
	//printf(YELLOW"inside %s sbuff=%s\n"NONE, __FUNCTION__,sbuff);
	// if(strstr(sbuff,"UBX")!=NULL)
	//printf(YELLOW"inside %s sbuff=%s\n"NONE, __FUNCTION__,sbuff);
	for(i=0; i<length; i++)
	{

		if (buff[i] == '$')  
		{

			s_index = 1;
			sbuff[0] = buff[i];
			sbuff[s_index] = 0;
			//printf(YELLOW"inside %s sbuff[%d]=%c\n"NONE, __FUNCTION__,i,sbuff[s_index]);
			continue;
		}
		if (s_index==-1)  
		{
			continue;
		}
		if (buff[i]==13)
		{
			if (gps_checksum()==0)
			{
				gps_proc_frame();
			}
			s_index = -1;

			continue;
		}
		sbuff[s_index] = buff[i];
		//printf(YELLOW"inside %s sbuff[%d]=%c\n"NONE, __FUNCTION__,i,sbuff[s_index]);
		s_index ++;
		sbuff[s_index] = 0;
	}
/*
for(ifor=0;ifor<256;ifor++)
{
	printf(YELLOW"ifor=%d-%c "NONE,ifor,sbuff[ifor]);


}
	printf(YELLOW"\n"NONE);*/
	//printf(YELLOW"inside %s sbuff=%s\n"NONE, __FUNCTION__,sbuff);
}

//
//
static int gps_checksum( void )
{
	return 0;
	int i;
	char *ptr1, *ptr2, cs, css[4];
	ptr1 = &sbuff[1];
	ptr2 = strchr(sbuff, '*');
	cs = 0;
	for(i=0; i< (ptr2-ptr1); i++)
	{
		cs ^= ptr1[i];
	}
	sprintf(css, "%02X", cs);
	ptr2 ++;
	return (strncmp(css, ptr2, 2)!=0);
}

//
//
char *cmd_list[64];  
int cmd_count;
FILE *fi_gps_gga_rmc;

void writetofile( void )
{
	FILE *fi;
	fi = fopen("/usr/gps_data", "rb+");
	if (fi==NULL)
	{
		fi = fopen("/usr/gps_data", "wb+");
	}
	write(fileno(fi), &gps_data, sizeof(gps_data));
	fclose(fi);
}

int opentowrite()
{
	fi_gps_gga_rmc = fopen("/mnt/sd/RMCGGA.DAT", "rb");
	if (fi_gps_gga_rmc==NULL)
	{
		fi_gps_gga_rmc = fopen("/mnt/sd/RMCGGA.DAT", "wb");
	}
	setbuf(fi_gps_gga_rmc,NULL);
	//write(fileno(fi_gps_gga_rmc), &gps_data, sizeof(gps_data));	
}

int closetowrite(FILE *fi)
{
	fclose(fi);
}


static void gps_proc_frame( void )
{

	if(app.app_sign.has_gps_mq_ready)
	{
		udp_sendnbytes(app.app_udp_socket[UDP_GPS].fid,sbuff,strlen(sbuff)+1,&host_gps_addr);
		//			sendto(app.app_udp_socket[UDP_GPS].fid,"will send ubx&nmea message to you",strlen("will send ubx&nmea message to you"),0,(struct sockaddr *)&cilent,len);
		//sendnbytes(p_oso->fids[FID_GPS_MQ],sbuff,strlen(sbuff)+1);
	}
	char *ptr1, *ptr2;
	struct timeval aa, bb;
	int i=0;
	int ifor;
	ptr1 = sbuff;

	for(;;)
	{
		cmd_list[i] = ptr1;
		ptr2 = strchr(ptr1, ',');
		if (ptr2==NULL)break;
		*ptr2 = 0;
		ptr1 = ptr2;
		ptr1 ++;
		i ++;
	}

// for(ifor=0;ifor<256;ifor++)
// {
// 	printf(YELLOW"ifor=%d-%c "NONE,ifor,sbuff[ifor]);


// }
// 	printf(YELLOW"\n"NONE);
			
	ptr1 = strchr(ptr1, '*');
	*ptr1 = 0;
	if (i==0)return;    
	cmd_count = i + 1;

	gps_gpgga();
	gps_gprmc();
	//printf("%d %d %d %d %d %d %d\n", gps_data.tm_time.tm_zone,gps_data.tm_time.tm_year, gps_data.tm_time.tm_mon, gps_data.tm_time.tm_mday, \
		gps_data.tm_time.tm_hour, gps_data.tm_time.tm_min, gps_data.tm_time.tm_sec);
	//printgps_info();
	//write(fileno(fi_gps_gga_rmc), &gps_data, sizeof(gps_data));//写入文件
}
//




static void gps_gpgga( void )
{
	//printf("Inside %s.\n", __FUNCTION__);
	if(strcmp(cmd_list[0], "$GPGGA")!=0)
		return;

	s_gpsOK_1 = 1;

	if (isdigit(cmd_list[7][0]))
	{
		sscanf( cmd_list[7], "%02ld", &gps_data.count);  
		
	}
	#define reversebit(x,y)  x^=(1<<y)
    #define WORK_LED 7
    if(app.app_sig.iswork){
    	app.app_sig.iswork=0;
    	reversebit(app.pca9539[1],WORK_LED);
    	//printf(RED"WORK_LED reversebit\n"NONE);
    }

	sscanf(cmd_list[9], "%ld.%ld", &gps_data.height_d, &gps_data.height_m);
}


long long seclast;
long long secnow=0;

long long rtt_mktime(DATE_TIME time)//时分秒转unix秒
{
    long long res;

    // 1..12 -> 11,12,1..10, Puts Feb last since it has leap day
    if (time.month <= 2)
    {
        time.month += 10;
        time.year -= 1;
    }
    else
    {
        time.month -= 2;
    }

    /*
    // Calculate how much days from 1970 to today
    res = 59;                            //31@0001-January and 28@0001-February
    res += (time.year - 1) * 365;        //at least 365 days one year
    res += time.year / 4 - time.year / 100 + time.year / 400;    //leap years from 0001
    res += 367 * time.month / 12 - 30;    //days from March(0.5/7<=slope<0.5/5)
    res += time.day - 1;                //days
    res -= 719162;                        //days from 0001-1-1 to 1970-1-1
    // Calculate how much seconds
    res = ( (res * 24 + time.hour) * 60 + time.minute) * 60 + time.second;
    */
    ///*
    res = (long long)(time.year/4 - time.year/100 + time.year/400) +
            367*time.month/12 + time.day +
            time.year*365 - 719499;
    res = ((res*24 + time.hour        // now have hours
            )*60 + time.minute            // now have minutes
           )*60 + time.second;            // finally seconds
    //*/

    return res;
}




long long t2t()//
{
		//printgps_info();
		seclast=secnow;
		mydate_now.year=(unsigned short)gps_data.tm_time.tm_year;
		mydate_now.month=(unsigned char)gps_data.tm_time.tm_mon;
		mydate_now.day=(unsigned char)gps_data.tm_time.tm_mday;
		mydate_now.hour=(unsigned char)gps_data.tm_time.tm_hour;
		mydate_now.minute=(unsigned char)gps_data.tm_time.tm_min;
		mydate_now.second=(unsigned char)gps_data.tm_time.tm_sec;
		secnow=rtt_mktime(mydate_now);
		/*if((secnow-1)==seclast)//正常
			;
		else{
			printf(RED"HAPPEN SOMETHING UNKONW"NONE);
			}*/
			
		return secnow;

}


int gobankhometime=0;//如果在持续刷新的屏幕中1分钟没有按键则返回主界面
int has_set_time=0;
static void gps_gprmc( void )
{
	//printf(YELLOW"inside %s\n"NONE, __FUNCTION__);
	//getethstatus();检测网卡是否插入
	char tmpstr[100];
	time_t tmg;
	struct timeval tv;  
	struct timezone tz; 
	int year;  
	int tmp;
	if(strcmp(cmd_list[0], "$GPRMC")!=0)
		return;
	s_gpsOK_2 = 1;
	app.app_count.second_count++;
	memset(&gps_data.tm_time, 0, sizeof(gps_data.tm_time));
	if (isdigit(cmd_list[1][0]))
	{
		sscanf(cmd_list[1], "%02d%02d%02d",&gps_data.tm_time.tm_hour, &gps_data.tm_time.tm_min, &gps_data.tm_time.tm_sec);
		sscanf(cmd_list[1], "%02d%02d%02d",&tm_time.tm_hour, &tm_time.tm_min, &tm_time.tm_sec);
	}
	
	if (isdigit(cmd_list[7][0]))
	{
		sscanf(cmd_list[9], "%02d%02d%02d",&gps_data.tm_time.tm_mday, &gps_data.tm_time.tm_mon, &year);
		sscanf(cmd_list[9], "%02d%02d%02d",&tm_time.tm_mday, &tm_time.tm_mon, &year);
	}
	year += 2000;  
	gps_data.tm_time.tm_year = year;



	tm_time.tm_year = year-1900;	
	tm_time.tm_mon -= 1;			
	tmg = mktime(&tm_time);			
	gettimeofday(&tv, &tz);		
	if (((unsigned long)tv.tv_sec&0xffffffc0) != ((unsigned long)tmg&0xffffffc0))
	{
		if (gps_debug!=0)
		{
			printf("Sync local time...");
		}
		tv.tv_sec = tmg;
		tv.tv_usec = 0;
		tz.tz_dsttime = 0;
		tz.tz_minuteswest = -(8*60);
		if(app.iws_server_share.status.is_clock_sync_gps==0&&(gps_data.status=='A')&&app.iws_server_share.status.is_clock_sync_ntp==0){
			app.iws_server_share.status.is_clock_sync_gps=1;
			settimeofday(&tv, &tz);
			system("rpi_gpio_ntp -N 1 -g 24 &");
		}		
		
		if (gps_debug!=0)
		{
			printf("Done.\n");
		}
	}
	if (timesync != 0)
	{
		sig_term(0);
	}
	if (isdigit(cmd_list[3][0]))
		sscanf(cmd_list[3], "%02ld%02ld.%04d", &gps_data.ns_d, &gps_data.ns_m10000, &tmp);
	gps_data.ns_m10000 *= 10000;
	gps_data.ns_m10000 += tmp;
	if (isdigit(cmd_list[5][0]))	
	sscanf(cmd_list[5], "%03ld%02ld.%04d", &gps_data.ew_d, &gps_data.ew_m10000, &tmp);
	gps_data.ew_m10000 *= 10000;
	gps_data.ew_m10000 += tmp;
	gps_data.ns = cmd_list[4][0];
	gps_data.ew = cmd_list[6][0];
	gps_data.status = cmd_list[2][0]; // A: located; V: Not located.
	char gpslabel;
	char endstr='\0';
	if(gps_data.status=='A'){
		gpslabel='A';
		/*if(p_oso->change_bit.isgpsfileopen==1)给NMEW文件中只写时间信息
		{
			//printf(GREEN"%d\n"NONE,strlen(hd));
			fwrite(cmd_list[1],9,1,p_oso->fp_gps);
			fwrite(&endstr,1,1,p_oso->fp_gps);
		}*/
	}
	else{
		gpslabel='V';
	}
	//printf("%s\n",sbuff);
		//sprintf(tmpstr, "%lld\n",t2t());
		//fwrite(tmpstr,strlen(tmpstr),1,fi_gps_gga_rmc);
		//fprintf(fi_gps_gga_rmc,"%s,",sbuff);
	//fprintf(fi_gps_gga_rmc,"%lld\n",t2t());
	//printgps_info();
	//printf(GREEN"mainscreen_index=%d nowindex=%d\n"NONE,mainscreen_index,nowindex);
}

int printgps_info()
{
printf(GREEN"Latitude d=%ld\n"NONE,gps_data.ns_d);
printf(GREEN"Latitude ff.=%ld\n"NONE,gps_data.ns_m10000);
printf(GREEN"Longitude d=%ld\n"NONE,gps_data.ew_d);
printf(GREEN"Longitude ff.=%ld\n"NONE,gps_data.ew_m10000);
printf(GREEN"NS=%c\n"NONE,gps_data.ns);
printf(GREEN"EW=%c\n"NONE,gps_data.ew);
printf(GREEN"status=%c\n"NONE,gps_data.status);
printf(GREEN"speed100=%ld\n"NONE,gps_data.speed100);
printf(GREEN"count=%ld\n"NONE,gps_data.count);
}



/*struct gps_data_t
{
	struct MyTm tm_time;    
	unsigned long ns_d;     
	unsigned long ns_m10000;
	unsigned long ew_d;     
	unsigned long ew_m10000;
	char ns;                
	char ew;                
	char status;            
	char card;              
	unsigned long speed100; 
	unsigned long cog100;   
	unsigned long count;    
	unsigned long height_d; 
	unsigned long height_m; 
};
*/


#define NNNN1 (22080000)
#define NNNN2 (22260000)

#define EEEE1 (113549000)
#define EEEE2 (114255000)

char test_buff[64];

void proc_args( int argc, char **argv )
{
	int i;
	if (argc<2)return;							
	for( i=1; i<argc; i++ )
	{
		if (strcmp(argv[i], "-timesync")==0)	
		{
			timesync = 1;
			continue;
		}
		if (strcmp(argv[i], "-d")==0)			
		{
			gps_debug = 1;
		}
		if (strcmp(argv[i], "-r")==0)			
		{
			real = 1;
		}
	}
}







struct gps_data_t * PraseGPS(void)
{
	tcflush(app.app_fid[GPS_FID].fid, TCIOFLUSH);
	for (;;)
	{
		memset(&gps_buff, 0, sizeof(gps_buff));
		//memset(&gps_data, 0, sizeof(gps_data));
		fgets(gps_buff, sizeof(gps_buff), fi);
		//printf("After fgets.\n");
		proc_gps(gps_buff, strlen(gps_buff));
		if (s_gpsOK_1 & s_gpsOK_2)
		{
			break;
		}
	}

	s_gpsOK_1 = 0;
	s_gpsOK_2 = 0;
	printf("%d %d %d %d %d %d\n", gps_data.tm_time.tm_year, gps_data.tm_time.tm_mon, gps_data.tm_time.tm_mday, \
		gps_data.tm_time.tm_hour, gps_data.tm_time.tm_min, gps_data.tm_time.tm_sec);

	return &gps_data;
}





/**
 * open port
 * @param  fd
 * @param  comport 想要打开的串口号
 * @return  返回-1为打开失败
 */
int open_port(int fd,int comport) 
{ 
	char *dev[]={"/dev/ttyUSB0","/dev/ttyS1","/dev/ttyS2"};

	if (comport==1)//串口1 
	{
		fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd)
		{ 
			perror("Can't Open Serial Port"); 
			return(-1); 
		} 
     } 
     else if(comport==2)//串口2 
     {     
		fd = open( "/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY); //没有设置<span style="font-family: Arial, Helvetica, sans-serif;">O_NONBLOCK非阻塞模式，也可以设置为非阻塞模式，两个模式在下一篇博客中具体说明</span>

		if (-1 == fd)
		{ 
			perror("Can't Open Serial Port"); 
			return(-1); 
		} 
     } 
     else if (comport==3)//串口3 
     { 
		fd = open( "/dev/ttyS2", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd)
		{ 
			perror("Can't Open Serial Port"); 
			return(-1); 
		} 
     } 
     /*恢复串口为阻塞状态*/ 
     if(fcntl(fd, F_SETFL, 0)<0) 
     		printf("fcntl failed!\n"); 
     else 
		printf("fcntl=%d\n",fcntl(fd, F_SETFL,0)); 
     /*测试是否为终端设备*/ 
     if(isatty(STDIN_FILENO)==0) 
		printf("standard input is not a terminal device\n"); 
     else 
		printf("isatty success!\n"); 
     printf("fd-open=%d\n",fd); 
     return fd; 
}

int open_dev(char * device)
{
	int fid;
	fid = open(device,O_RDWR|O_NOCTTY); /*读写方式打开串口*/
	if(fid == -1){ /*打开失败*/
		perror("open /dev/ttyS2");
		return -1;
	}
	else
		return fid;
}


int open_serial(int k)
{
	int fid;
	if(k==0){
		fid = open("/dev/ttyUSB1",O_RDWR|O_NOCTTY); /*读写方式打开串口*/
	}
	else
	{
		fid = open("/dev/ttyS1",O_RDWR|O_NOCTTY);
		//perror("open /dev/ttyS2");
	}
	if(fid == -1){ /*打开失败*/
		perror("open /dev/ttyS2");
		return -1;
	}
	else
		return fid;
}
/********************************************************************/

int check_sum(char * Buffer,int N)
{
char CK_A = 0;
char CK_B = 0;
		//printf(YELLOW"before check N=%d\n"NONE,N);
		int jfor;
		// for(jfor=0;jfor<N+2;jfor++)
		// 	{
		// 	printf(YELLOW"%02X "NONE,Buffer[jfor]);
		// 	}
		// 	printf("\n");


int I=0;
for(I=0;I<N;I++)
{
CK_A = CK_A + Buffer[I];
CK_B = CK_B + CK_A;
}
Buffer[N]=CK_A;
Buffer[N+1]=CK_B;
		// printf(YELLOW"after check N=%d\n"NONE,N);
		// for(jfor=0;jfor<N+2;jfor++)
		// 	{
		// 	printf(YELLOW"%02X "NONE,Buffer[jfor]);
		// 	}
		// 	printf("\n");
}



char hd[max_buffer_size],*rbuf; /*定义接收缓冲区*/



int start_process_nmew(int method)
{
	switch(method)
	{
	case 0:
	my_method();
	break;
	case 1://
	GpsThread();
	break;
	default:
	break;


	}
	return 0;
}



int my_method()
{
	int flag_close, retv,i,ncount=0;
	int ifor=0;
	int strle,p_next=0;
	char * p_nmea;
	int padd=0,ret;
	char delims[] = ",";
	char *result = NULL;
	//char * p_info_start=(char *)&p_oso->info.gps_info;
	char gpslabel;//GPS有效标志
	GPSRMC_str gpsrmc;
//原始处理程序开始-------------------------->
	while (app.thread_switch.main_loop && app.thread_switch.gps_loop)
	{
	/*
	pthread_mutex_lock(&myoso->mut[MUTALL]);
	while(ispause)
		{
		printf(YELLOW"GPS PAUSED\n"NONE);
		pthread_cond_wait(&myoso->cond[CONDALL],&myoso->mut[MUTALL]);
		if(ctltotal==0){
			pthread_mutex_unlock(&myoso->mut[MUTALL]);
			printf(YELLOW"GPS EXIT\n"NONE);	
			close(fd);
			return 0;
			}
		printf(YELLOW"GPS RESUME\n"NONE);	
		}
	pthread_mutex_unlock(&myoso->mut[MUTALL]);*/








	rbuf=hd;
	//printf("startgettime in GPS=%d\r\n",startgettime);
	/*while (!startgettime)//作为读取gps信息的时间同步
	{
	printf("waiting^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^%d\r\n",startgettime);
	usleep(100000);
	}*/
	retv=read(app.app_fid[GPS_FID].fid,rbuf,1);
	/*接收数据*/
	if(retv==-1)
	{
	perror("read"); /*读状态标志判断*/
	printf("read GPS package err!\r\n");

	}

	/*************************开始接收数据******************************/
	while(*rbuf!='\n')
	/*判断数据是否接收完毕*/
	{
	ncount+=1;
	rbuf++;
	retv=read(app.app_fid[GPS_FID].fid,rbuf,1);
	//printf("in while!\r\n");
	if(retv==-1)
		{
		printf("read GPS package err!\r\n");
		error("read");
		}
	}
	rbuf++;
	hd[ncount+1]='\0';
	char endstr='\0';


	printf(BLUE"%s"NONE,hd);

	/*******************************************************************/
	//printf("The data received is:\n"); /*输出接收到的数据*/
	/*for(i=0;i<ncount;i++)
	{
	printf("%c",hd[i]);
	}*/
	//printf("%s",hd);
	//printf("\n");

	//printf(GREEN""NONE);
	//DEBUG(""); 
	

	//strcpy(gps_info_p,hd);
	//memcpy(,ch,myoso->oso_size.para_s);
	//strlen(hd);
	//myoso->myoso->info.gps_info
	p_nmea=hd;
	strle=strlen(p_nmea);
	if(strstr(hd,"$GPRMC")!=NULL){

/*/写gpsNMEW文件
	if(myoso->change_bit.isgpsfileopen==1)
	{
	//printf(GREEN"%d\n"NONE,strlen(hd));
	fwrite(hd, strlen(hd),1,myoso->fp_gps);
	}
//写gpsNMEW文件*/
			if(strstr(hd,",A,")!=NULL){
				gpslabel='A';
				}
			else{
				gpslabel='V';
				}
		//pthread_mutex_lock(&mtx);
		//pthread_cond_signal(&cond); 
		//pthread_mutex_unlock(&mtx);
		//ret = sscanf(hd,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[*],%[^,],%[*],%[*],%[^,]",gpsrmc.nmea,gpsrmc.utcdatetime,gpsrmc.Status,gpsrmc.Latitude,gpsrmc.NS,gpsrmc.Longitude,gpsrmc.EW,gpsrmc.Speed,gpsrmc.Course,gpsrmc.utcymdtime,gpsrmc.mag1,gpsrmc.mag2,gpsrmc.mode);
/*		
		printf(GREEN"gpsrmc.nmea: %s\n"NONE,gpsrmc.nmea);
		printf(GREEN"gpsrmc.utcdatetime :%s\n"NONE,gpsrmc.utcdatetime);
		printf(GREEN"gpsrmc.Status:%s\n"NONE,gpsrmc.Status);
		printf(GREEN"gpsrmc.Latitude:%s\n"NONE,gpsrmc.Latitude);
		printf(GREEN"gpsrmc.NS:%s\n"NONE,gpsrmc.NS);
		printf(GREEN"gpsrmc.Longitude:%s\n"NONE,gpsrmc.Longitude);
		printf(GREEN"gpsrmc.EW:%s\n"NONE,gpsrmc.EW);
		printf(GREEN"gpsrmc.Speed:%s\n"NONE,gpsrmc.Speed);
		printf(GREEN"gpsrmc.Course:%s\n"NONE,gpsrmc.Course);
		printf(GREEN"gpsrmc.utcymdtime:%s\n"NONE,gpsrmc.utcymdtime);
		printf(GREEN"gpsrmc.mag1:%s\n"NONE,gpsrmc.mag1);
		printf(GREEN"gpsrmc.mag2:%s\n"NONE,gpsrmc.mag2);
		printf(GREEN"gpsrmc.mode:%s\n"NONE,gpsrmc.mode);*/
		/*for(ifor=0;ifor<16;ifor++)
		{
		padd=strstr(&hd[padd],",");
		switch(ifor)
			{
			case 0:
			//$GPRMC,092327.00,A,4011.78992,N,11617.29274,E,0.016,,070116,,,A*78
			break;
			}
		}*/
/*
	result = strtok(hd, delims);//以给定的分隔符分割gps报文，delims为分隔符
	timestr=0;
		while( result != NULL ) {
			timestr++;
			//printf("timestr=%d\r\n",timestr);
			if (timestr==2)
				{
				//printf("in timestr\r\n");
				//pthread_cond_broadcast(&cond);
				startgetdata=1;
				strcpy(&myoso->filename, result);
				//pthread_mutex_unlock(&mtx);
			       //printf( "result is \"%s\"\n", result );
				//printf( YELLOW"--time: %s\n"NONE, result);
				}
			else if(timestr==10)
				{
				strcpy(&myoso->folder, result);
				//printf( YELLOW"--date: %s\n"NONE, result);
					//if(mkdir(result,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH)<0)//创建新目录
					//{
					 // printf("mkdir failed\n");
					 // //return 2;
					//}
				}
		       result = strtok( NULL, delims);
		   }*/
		//memset(p_info_start,0,390);
		//printf(YELLOW"%s"NONE,hd);
		p_next=0;
		}//if find GRRMC
		//printf(GREEN"strle=%d,p_next=%d\n"NONE,strle,p_next);
		if((p_next+strle+1)<390)
		{
			//memcpy(p_info_start+p_next,p_nmea,strle);
			p_next+=strle;
			for(ifor=0;ifor<390;ifor++)
			{
				;//printf("%c",myoso->info.gps_info[ifor]);
			}
		}
	//printf(GREEN"%s"NONE,myoso->info.gps_info);
	//DEBUGb(""); 
	//pthread_cond_signal(&cond);
	ncount=0;//重置控制变量
	//startgettime=0;
	//printf(YELLOW"THE END OF GPS CTLTOTAL\r\n"NONE);
	}
//原始处理程序结束<--------------------------
}
typedef struct ubx_poll{
	unsigned char poll[8];
	int poll_len;
	int poll_recv;
	int is_valid;

}__attribute__ ((packed,aligned(1)))ubx_poll; 

ubx_poll nav_poll_fix_s={{0xB5,0x62,0x01,0x03,0x00,0x00,0x22,0x67},{8},{24},{0}};
ubx_poll nav_poll_dop_s={{0xB5,0x62,0x01,0x04,0x00,0x00,0x22,0x67},{8},{26},{0}};
ubx_poll nav_poll_timeutc_s={{0xB5,0x62,0x01,0x21,0x00,0x00,0x22,0x67},{8},{28},{0}};

char gprmc[60]={"serch for signal"};
char gprmc_all[60]={"serch for signal"};

char gprmc1[60];
char gprmc2[60]={"utc not valid"};

int nav_poll_dop_callback(char * rbuf,int nbytes)
{
	int ifor;
	// for(ifor=0;ifor<nbytes;ifor++)//打印输出,可以不要
	// {
	// printf(BLUE"%02X "NONE,rbuf[ifor]);
	// }
	// printf("\n");
	return 1;
}

int nav_poll_timeutc_callback(char * rbuf,int nbytes)
{
	int ifor;
	// for(ifor=0;ifor<nbytes;ifor++)//打印输出,可以不要
	// {
	// 	printf(GREEN"%02X "NONE,rbuf[ifor]);
	// }
	//printf("\n");
	if(rbuf[25]==0x07){//rbuf[25]此项为0x07代表UTC时间有效
		//snprintf(gprmc2,40,"%s","utc valid");
		// if((mainscreen_index==5)&(nowindex==0)){
		// 	update_gps_fix_info2ledbuff(gprmc2,40);
		// }
	
		return  1;//valid
	}
	else{
		// snprintf(gprmc2,40,"%s","utc not valid");
		// if((mainscreen_index==5)&(nowindex==0)){
		// 	update_gps_fix_info2ledbuff(gprmc2,40);
		// }
		return 0;
	}
		
}


int callback_test(char * rbuf,int nbytes)
{
	printf(YELLOW"inside %s line=%d nbytes=%d\n"NONE, __FUNCTION__,__LINE__,nbytes);

	int ifor;
	for(ifor=0;ifor<nbytes;ifor++)//打印输出,可以不要
	{
	printf(BLUE"%02X "NONE,rbuf[ifor]);
	}
	printf("\n");

}



int nav_poll_fix_callback(char * rbuf,int nbytes)
{

	int ifor;
	// for(ifor=0;ifor<nbytes;ifor++)//打印输出,可以不要
	// {
	// 	printf(RED"%02X "NONE,rbuf[ifor]);
	// }
	//printf("\n");


	if(rbuf[0]==0xB5){
		switch(rbuf[10])
		{
			case 0x00:
			snprintf(gprmc,60,"%s","no fix");
			break;
			case 0x01:
			snprintf(gprmc,60,"%s","dead reckoning only");
			break;
			case 0x02:
			snprintf(gprmc,60,"%s","2D-fix");
			//sscanf( "3", "%02ld", &gps_data.count); 
			break;
			case 0x03:
			snprintf(gprmc,60,"%s","3D-fix");
			//sscanf( "4", "%02ld", &gps_data.count); 
			break;
			case 0x04:
			snprintf(gprmc,60,"%s","GPS + dead reckoning combined");
			break;
			case 0x05:
			snprintf(gprmc,60,"%s","Time only fix");
			break;
			default:
			break;
		}
		// if((mainscreen_index==5)&(nowindex==0)){
		// 	update_gps_fix_info2ledbuff(gprmc,20);
		// }
	
	}
}





int callback_test_frame(int (*fun_callback)(char *,int))
{
	unsigned char nav_poll_fix[]={0xB5,0x62,0x01,0x03,0x00,0x00,0x22,0x67};//获取时间是否有效命令
			printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);

fun_callback(nav_poll_fix,sizeof(nav_poll_fix));

			printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);

}




int ubx_poll_frame(ubx_poll * ubx_poll_s,int (*fun_callback)(char *,int))
{

	check_sum(ubx_poll_s->poll+2,ubx_poll_s->poll_len-4);
	int ifor;
	
	int retv=0;
	char rbuf[ubx_poll_s->poll_recv];
	while(rbuf[0]!=0xB5)
	{
		retv=0;
		tcflush(app.app_fid[GPS_FID].fid, TCIOFLUSH);//清空读写缓冲
		write(app.app_fid[GPS_FID].fid,ubx_poll_s->poll,ubx_poll_s->poll_len);
		while(retv!=ubx_poll_s->poll_recv)//循环读取28字节返回内容
		{
			retv+=read(app.app_fid[GPS_FID].fid,rbuf+retv,ubx_poll_s->poll_recv-retv);
			//printf(BLUE"RETV=%d\n"NONE,retv);
		}
		for(ifor=0;ifor<retv;ifor++)
		{
			printf(RED"%02X "NONE,rbuf[ifor]);
		}
		if(app.app_sign.has_gps_mq_ready)
		{
			udp_sendnbytes(app.app_udp_socket[UDP_GPS].fid,rbuf,retv,&host_gps_addr);
			//			sendto(app.app_udp_socket[UDP_GPS].fid,"will send ubx&nmea message to you",strlen("will send ubx&nmea message to you"),0,(struct sockaddr *)&cilent,len);

		}
		//sendnbytes(p_oso->fids[FID_GPS_MQ],rbuf,retv);

		printf("\n");
	}
	ubx_poll_s->is_valid=fun_callback(rbuf,ubx_poll_s->poll_recv);
	return ubx_poll_s->is_valid;
}


int get_fix_status()
{
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
	unsigned char nav_poll_fix[]={0xB5,0x62,0x01,0x03,0x00,0x00,0x22,0x67};//获取位置是否锁定
	unsigned char nav_poll_dop[]={0xB5,0x62,0x01,0x04,0x00,0x00,0x22,0x67};//获取时间是否有效命令
	unsigned char nav_timeutc[]={0xB5,0x62,0x01,0x21,0x00,0x00,0x22,0x67};//获取时间是否有效命令
	int ret=0;
	while(!ret)
	{
		// write(fd,nav_poll_fix,sizeof(nav_poll_fix));
		// //printf(YELLOW"--------------------CFG_GPS------------------\n"NONE);
		// while(retv!=5)
		// 	{
		// 	retv+=read(fd,rbuf,5-retv);
		// 	printf(BLUE"RETV=%d\n"NONE,retv);
		// 	}

		// for(ifor=0;ifor<10;ifor++)
		// 	{
		// 	printf("%02X ",rbuf[ifor]);
		// 	}
		// //printf("\n");
		// int retv=0,rr=24;
		// char rbuf[rr];
		// while(retv!=rr)//循环读取28字节返回内容
		// {
		// 	retv+=read(fd,rbuf,rr-retv);
		// 	//printf(BLUE"RETV=%d\n"NONE,retv);
		// }
/*
- 0x00 = no fix
- 0x01 = dead reckoning only
- 0x02 = 2D-fix
- 0x03 = 3D-fix
- 0x04 = GPS + dead reckoning combined - 0x05 = Time only fix
*/
		// if((mainscreen_index==5)&(nowindex==0)){
		// 	update_gps_fix_info2ledbuff(gprmc);
		// 	isneedupdate=1;
		// }
		// for(ifor=0;ifor<rr;ifor++)//打印输出,可以不要
		// {
		// printf("%02X ",rbuf[ifor]);
		// }
		// printf("\n");
		// printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);

		// int fix = poll_msg(nav_poll_fix,sizeof(nav_poll_fix),24,nav_poll_fix);
		// printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);

		// int pod = poll_msg(nav_poll_dop,sizeof(nav_poll_dop),26,nav_poll_dop_callback);
		// printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);

		// int nav_timeutc = poll_msg(nav_timeutc,sizeof(nav_timeutc),28,nav_timeutc_callback);
		// printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);
		//int nav_timeutc = poll_msg(&nav_timeutc,sizeof(nav_timeutc),28,callback_test);
		// printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);
		//tcflush(app.app_fid[GPS_FID].fid, TCIOFLUSH);//清空读写缓冲
		ret=ubx_poll_frame(&nav_poll_fix_s,nav_poll_fix_callback);
		//ret=ubx_poll_frame(&nav_poll_dop_s,nav_poll_dop_callback);
		ret=ubx_poll_frame(&nav_poll_timeutc_s,nav_poll_timeutc_callback);
		// if((mainscreen_index==5)&(nowindex==0)){
		// 	isneedupdate=1;
		// }
        if(app.app_sig.iswork){
        	app.app_sig.iswork=0;
        	reversebit(app.pca9539[1],WORK_LED);
        }
        
                //printf("current=%f\n",-1000*1000*getcurrent());
        //set_pca9539(app.pca9539);
		sleep(3);
		//sectimer_taskmanage(0,p_oso);
		//callback_test_frame(callback_test);
	}
	return 0;
}



int poll_msg(unsigned char * nav_poll,int N,int rr,int (*fun_callback)(char *,int))
{
	printf(YELLOW"inside %s line=%d nav_poll=%d N=%d,rr=%d\n"NONE, __FUNCTION__,__LINE__,nav_poll,N,rr);

	check_sum(nav_poll+2,N-4);
	int ifor;
			printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);

	write(app.app_fid[GPS_FID].fid,nav_poll,N);
	int retv=0;
	char rbuf[rr];
			printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);

	while((rbuf[0]!=0xB5)&&(rbuf[1]!=0x62))
	{
			printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);

		while(retv!=rr)//循环读取28字节返回内容
		{
			retv+=read(app.app_fid[GPS_FID].fid,rbuf,rr-retv);
			//printf(BLUE"RETV=%d\n"NONE,retv);
		}
	}
			printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);

	int ret=fun_callback(rbuf,rr);
			printf(YELLOW"inside %s line=%d\n"NONE, __FUNCTION__,__LINE__);

	return 0;
}













int cfg_gps()//初始化gps相关配置
{
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
 
	int ifor=0,jfor=0;
	const int rr=28;
	char rbuf[rr];
	int retv=0;
	int istimevalid=0;
	/*char  cfg_tp[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x01,\//0x01表示配置tp2
	0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7D,\//0x00,0x00,0x7D,0x00表示输出脉冲的频率8192000
	0x00,0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x80,0x00,0x00,\
	0x00,0x80,0x00,0x00,0x00,0x00,0x6F,0x00,0x00,0x00,0xC2,0x4E};
	*/
	/*	4.096M
	char cfg_tp[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x01,0x01,0x00,0x00,0x32,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x80,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0xEF,0x00,0x00,0x00,0xB9,0xB2};
	*/
	// 8M时钟
	//char cfg_tp[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x01,0x01,0x00,0x00,0x32,0x00,0x00,0x00,0x00,0x12,0x7A,0x00,0x00,0x12,0x7A,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0xEF,0x00,0x00,0x00,0x92,0x42};
	//配置tp1char cfg_tp[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7D,0x00,0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x6F,0x00,0x00,0x00,0xC1,0x2E};
	//char  cfg_tp[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x00,\
	0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7D,\
	0x00,0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x80,0x00,\
	0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x6F,\
	0x00,0x00,0x00,\
	0xC1,0x2E};
	char  cfg_tp2[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x01,\
	0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7D,\
	0x00,0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x80,0x00,\
	0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x6F,\
	0x00,0x00,0x00,\
	0xC2,0x4E};
	//sleep(2);
	//tcflush(app.app_fid[GPS_FID].fid, TCIOFLUSH);//清空读写缓冲
	write(app.app_fid[GPS_FID].fid,cfg_tp2,sizeof(cfg_tp2));//配置tp2时钟
	char cfg_msg[]={0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x00,0x00,0xFA,0x0F};//打开/关闭报文,{固定,固定,06=cfg(class),01=msg(id),03=3byte(length),00=3byte(length),F0=MSG,00=MSG,00=rate,CK_A,CK_B}
	char nmea_msg_close[]={0x0A,0x09,0x00,0x01,0x40,0x06,0x07,0x04,0x05,0x02,0x03,0x08,0x41};//对照下方注释进行关闭开启字段,此表中项作为cfg_msg[7]中的值

	char nmea_msg[]={0x00,0x04};//对照下方注释进行关闭开启字段,此表中项作为cfg_msg[7]中的值
/*,
53 DTM 0xF0 0x0A Datum Reference
54 GBS 0xF0 0x09 GNSS Satellite Fault Detection
55 GGA 0xF0 0x00 Global positioning system fix data
56 GLL 0xF0 0x01 Latitude and longitude, with time of position fix and status
57 GPQ 0xF0 0x40 Poll message
58 GRS 0xF0 0x06 GNSS Range Residuals
59 GSA 0xF0 0x02 GNSS DOP and Active Satellites
60 GST 0xF0 0x07 GNSS Pseudo Range Error Statistics
61 GSV 0xF0 0x03 GNSS Satellites in View
62 RMC 0xF0 0x04 Recommended Minimum data
63 TXT 0xF0 0x41 Text Transmission
64 VTG 0xF0 0x05 Course over ground and Ground speed
65 ZDA 0xF0 0x08 Time and Date
//ublox模块默认开启项如下:
!!!!!!!!!!!!!!!!!!!!!>$GPRMC,,V,,,,,,,,,,N*53
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPVTG,,,,,,,,,N*30
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGGA,,,,,,0,00,99.99,,,,,,*48
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGSV,1,1,03,02,,,10,13,,,13,15,,,19*75
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGLL,,,,,,V,N*64
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPZDA,,,,,00,00*48
*/

	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
 	//sleep(2);
	//tcflush(app.app_fid[GPS_FID].fid, TCIOFLUSH);//清空读写缓冲
	for(ifor=0;ifor<sizeof(nmea_msg_close);ifor++)//关闭nmea_msg中所列出的报文
	{
		retv=0;
		cfg_msg[7]=nmea_msg_close[ifor];
		check_sum(cfg_msg+2,sizeof(cfg_msg)-4);
		/*for(jfor=0;jfor<sizeof(cfg_msg);jfor++)
			{
			printf(BLUE"%02X "NONE,cfg_msg[jfor]);
			}
			printf("\n");*/
		write(app.app_fid[GPS_FID].fid,cfg_msg,sizeof(cfg_msg));//配置tp2时钟
/*
		while(retv!=100)
			{
			retv+=read(fd,rbuf,100-retv);
			printf(RED"RETV=%d\n"NONE,retv);
			}
		for(jfor=0;jfor<100;jfor++)
			{
			printf(RED"%02X "RED,rbuf[jfor]);
			}
			printf("\n");
*/



	}


/*
53 DTM 0xF0 0x0A Datum Reference
54 GBS 0xF0 0x09 GNSS Satellite Fault Detection
55 GGA 0xF0 0x00 Global positioning system fix data
56 GLL 0xF0 0x01 Latitude and longitude, with time of position fix and status
57 GPQ 0xF0 0x40 Poll message
58 GRS 0xF0 0x06 GNSS Range Residuals
59 GSA 0xF0 0x02 GNSS DOP and Active Satellites
60 GST 0xF0 0x07 GNSS Pseudo Range Error Statistics
61 GSV 0xF0 0x03 GNSS Satellites in View
62 RMC 0xF0 0x04 Recommended Minimum data
63 TXT 0xF0 0x41 Text Transmission
64 VTG 0xF0 0x05 Course over ground and Ground speed
65 ZDA 0xF0 0x08 Time and Date

!!!!!!!!!!!!!!!!!!!!!>$GPRMC,,V,,,,,,,,,,N*53
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPVTG,,,,,,,,,N*30
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGGA,,,,,,0,00,99.99,,,,,,*48
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGSV,1,1,03,02,,,10,13,,,13,15,,,19*75
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGLL,,,,,,V,N*64
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPZDA,,,,,00,00*48
*/

	//char nav_timeutc={B5 62 01 21 00 00 22 67};//获取时间是否有效
	//char cfg_tp_rx[512];
	//check_sum(cfg_tp+2,2);
	//write(fd,cfg_tp,sizeof(cfg_tp));//配置tp1时钟
	int kfor=0;
	sleep(2);
	tcflush(app.app_fid[GPS_FID].fid, TCIOFLUSH);//清空读写缓冲
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
 
	get_fix_status();
	for(ifor=0;ifor<sizeof(nmea_msg);ifor++)//配置报文需要输出内容
		{
		retv=0;
		cfg_msg[7]=nmea_msg[ifor];
		cfg_msg[8]=0x01;
		check_sum(cfg_msg+2,sizeof(cfg_msg)-4);
		/*for(jfor=0;jfor<sizeof(cfg_msg);jfor++)
			{
			printf(BLUE"%02X "NONE,cfg_msg[jfor]);
			}
			printf("\n");*/
		write(app.app_fid[GPS_FID].fid,cfg_msg,sizeof(cfg_msg));//配置tp2时钟
/*
		while(retv!=100)
			{
			retv+=read(fd,rbuf,100-retv);
			printf(RED"RETV=%d\n"NONE,retv);
			}
		for(jfor=0;jfor<100;jfor++)
			{
			printf(RED"%02X "RED,rbuf[jfor]);
			}
			printf("\n");
*/
	}
	exit(0);
}



int cfg_nmea_gps()//初始化gps相关配置
{
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
 
	int ifor=0,jfor=0;
	const int rr=28;
	char rbuf[rr];
	int retv=0;
	int istimevalid=0;
	/*char  cfg_tp[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x01,\//0x01表示配置tp2
	0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7D,\//0x00,0x00,0x7D,0x00表示输出脉冲的频率8192000
	0x00,0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x80,0x00,0x00,\
	0x00,0x80,0x00,0x00,0x00,0x00,0x6F,0x00,0x00,0x00,0xC2,0x4E};
	*/
	/*	4.096M
	char cfg_tp[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x01,0x01,0x00,0x00,0x32,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x80,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0xEF,0x00,0x00,0x00,0xB9,0xB2};
	*/
	// 8M时钟
	//char cfg_tp[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x01,0x01,0x00,0x00,0x32,0x00,0x00,0x00,0x00,0x12,0x7A,0x00,0x00,0x12,0x7A,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0xEF,0x00,0x00,0x00,0x92,0x42};
	//配置tp1char cfg_tp[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7D,0x00,0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x6F,0x00,0x00,0x00,0xC1,0x2E};
	//char  cfg_tp[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x00,\
	0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7D,\
	0x00,0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x80,0x00,\
	0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x6F,\
	0x00,0x00,0x00,\
	0xC1,0x2E};
	char  cfg_tp2[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x01,\
	0x01,/**/0x00,0x00,0x00,0x00,0x00,0x00,/**/0x00,0x00,0x7D,0x00,/*freqPeriod*/\
	0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x80,0x00,\
	0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x6F,\
	0x00,0x00,0x00,\
	0xC2,0x4E};
	char  cfg_tp2_iws[]={0xB5,0x62,0x06,0x31,0x20,0x00,0x01,\
	0x01,/**/0x00,0x00,0x00,0x00,0x00,0x00,/**/0x00,0x30,0x75,0x00,/*freqPeriod*/\
	0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x80,0x00,\
	0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x6F,\
	0x00,0x00,0x00,\
	0xC2,0x4E};
	check_sum(cfg_tp2_iws+2,sizeof(cfg_tp2_iws)-4);
	//write(app.app_fid[GPS_FID].fid,cfg_tp2,sizeof(cfg_tp2));//配置tp2时钟
	write(app.app_fid[GPS_FID].fid,cfg_tp2_iws,sizeof(cfg_tp2_iws));//配置tp2时钟
	char cfg_msg[]={0xB5,0x62,0x06,0x01,0x03,0x00,0xF0,0x00,0x00,0xFA,0x0F};//打开/关闭报文,{固定,固定,06=cfg(class),01=msg(id),03=3byte(length),00=3byte(length),F0=MSG,00=MSG,00=rate,CK_A,CK_B}
	char nmea_msg_close[]={0x0A,0x09,0x00,0x01,0x40,0x06,0x07,0x04,0x05,0x02,0x03,0x08,0x41};//对照下方注释进行关闭开启字段,此表中项作为cfg_msg[7]中的值

	char nmea_msg[]={0x00,0x04};//对照下方注释进行关闭开启字段,此表中项作为cfg_msg[7]中的值
/*,
53 DTM 0xF0 0x0A Datum Reference
54 GBS 0xF0 0x09 GNSS Satellite Fault Detection
55 GGA 0xF0 0x00 Global positioning system fix data
56 GLL 0xF0 0x01 Latitude and longitude, with time of position fix and status
57 GPQ 0xF0 0x40 Poll message
58 GRS 0xF0 0x06 GNSS Range Residuals
59 GSA 0xF0 0x02 GNSS DOP and Active Satellites
60 GST 0xF0 0x07 GNSS Pseudo Range Error Statistics
61 GSV 0xF0 0x03 GNSS Satellites in View
62 RMC 0xF0 0x04 Recommended Minimum data
63 TXT 0xF0 0x41 Text Transmission
64 VTG 0xF0 0x05 Course over ground and Ground speed
65 ZDA 0xF0 0x08 Time and Date
//ublox模块默认开启项如下:
!!!!!!!!!!!!!!!!!!!!!>$GPRMC,,V,,,,,,,,,,N*53
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPVTG,,,,,,,,,N*30
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGGA,,,,,,0,00,99.99,,,,,,*48
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*30
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGSV,1,1,03,02,,,10,13,,,13,15,,,19*75
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPGLL,,,,,,V,N*64
<!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!>$GPZDA,,,,,00,00*48
*/

	tcflush(app.app_fid[GPS_FID].fid, TCIOFLUSH);//清空读写缓冲
	for(ifor=0;ifor<sizeof(nmea_msg_close);ifor++)//关闭nmea_msg中所列出的报文
	{
		retv=0;
		cfg_msg[7]=nmea_msg_close[ifor];
		check_sum(cfg_msg+2,sizeof(cfg_msg)-4);
		/*for(jfor=0;jfor<sizeof(cfg_msg);jfor++)
			{
			printf(BLUE"%02X "NONE,cfg_msg[jfor]);
			}
			printf("\n");*/
		write(app.app_fid[GPS_FID].fid,cfg_msg,sizeof(cfg_msg));//配置tp2时钟
/*
		while(retv!=100)
			{
			retv+=read(fd,rbuf,100-retv);
			printf(RED"RETV=%d\n"NONE,retv);
			}
		for(jfor=0;jfor<100;jfor++)
			{
			printf(RED"%02X "RED,rbuf[jfor]);
			}
			printf("\n");
*/



	}









	for(ifor=0;ifor<sizeof(nmea_msg_close);ifor++)//配置报文需要输出内容
	{
		retv=0;
		cfg_msg[7]=nmea_msg_close[ifor];
		cfg_msg[8]=0x01;
		check_sum(cfg_msg+2,sizeof(cfg_msg)-4);
		for(jfor=0;jfor<sizeof(cfg_msg);jfor++)
			{
			printf(BLUE"%02X "NONE,cfg_msg[jfor]);
			}
			printf("\n");
		write(app.app_fid[GPS_FID].fid,cfg_msg,sizeof(cfg_msg));//配置tp2时钟
/*
		while(retv!=100)
			{
			retv+=read(fd,rbuf,100-retv);
			printf(RED"RETV=%d\n"NONE,retv);
			}
		for(jfor=0;jfor<100;jfor++)
			{
			printf(RED"%02X "RED,rbuf[jfor]);
			}
			printf("\n");
*/
	}
}












int gps_main(APP_S * app)
{
	app->gps_data=&gps_data;
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
	//register_pps_interupt();
	//while(1);
	fd_set   t_set1;//\\判断gps是否有报文输出
    struct timeval  tv;
	int readtimes,isgpsok=0;
	int ifor=0;
	char test[100];
	struct termios opt;//串口参数设置结构体
	/*******************************************************************/
	if(app->argc>1){
		app->app_fid[GPS_FID].fid=open_dev(app->argv[1]);//读取gps串口
	}
	else{
		app->app_fid[GPS_FID].fid=open_serial(0);//读取gps串口
	}
	/*打开串口 1*/
	/*******************************************************************/
	tcgetattr(app->app_fid[GPS_FID].fid,&opt);
	cfmakeraw(&opt);
	/*****************************************************************/
	cfsetispeed(&opt,B9600); /*波特率设置为 9600bps*/
	cfsetospeed(&opt,B9600);
	/*****************************************************************/
	tcsetattr(app->app_fid[GPS_FID].fid,TCSANOW,&opt);
	 /*数据保存*/
	//printf("ready for receiving data...\n");
	//int flags = fcntl(fd, F_GETFL, 0);
	//fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	//fcntl(client_sockfd[i_connect], F_SETFL, flags & ~O_NONBLOCK);
	int iprint=0;
	/*while(1)
	{
	char cfg_tp[]={0xB5,0x62,0x06,0x07,0x00,0x00};
	char cfg_tp_rx[512];
	check_sum(cfg_tp+2,2);
	write(fd,cfg_tp,sizeof(cfg_tp));
	retv=read(fd,cfg_tp_rx,10);
	printf("WRITE TO GPS TO TTYS1_TX\n");
	for(iprint=0;iprint<10;iprint++)
	{
		printf("READ FROM GPS TO TTYS1_RX %d=%x\n",iprint,cfg_tp_rx[iprint]);
	}
	usleep(10000000);
	}*/
	FD_ZERO(&t_set1);
	FD_SET(app->app_fid[GPS_FID].fid, &t_set1);
	tv.tv_sec= 2;
	tv.tv_usec= 1000;
	int i_state;
	//printf("--------------->GPS_1");
	i_state= select(app->app_fid[GPS_FID].fid+1, &t_set1, NULL, NULL, &tv);
	//printf("--------------->GPS_2\n");
	if (i_state < 0) {
			
		printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
		return -1;
	};
						 
	if (i_state > 0){
		int retv=read(app->app_fid[GPS_FID].fid,test,100);
		for(ifor=0;ifor<100;ifor++){
			printf("%X",test[ifor]);
		}
		printf("\n");
		isgpsok=1;	

	}
	else{
	isgpsok=0;	
	}
	isgpsok=1;	
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
 	

	
	//cfg_nmea_gps();
	//printf(YELLOW"THE i_state=%d isgpsok=%d\r\n"NONE,i_state,isgpsok);
	if(isgpsok){//如果gps正常,则开始处理程序
		//cfg_gps();
		cfg_nmea_gps();
		start_process_nmew(1);
	}
 	exit(0);
	printf(YELLOW"THE END OF GPS\r\n"NONE);
	//myoso->fids[FID_GPS]=close(myoso->fids[FID_GPS]);
	printf(YELLOW"GPS EXIT\r\n"NONE);
	//myoso->status_bit.gps_status=0;
	printf(GREEN"YOU GUESS WHAT ? IT WORKS ,WE JUST FINISH MY JOB--GPS!\n"NONE);
	return 0;
}