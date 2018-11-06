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
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <termios.h>

#include <zwpdebug.h>
#include <printcolor.h>
#include <app_main.h>
#include <ublox.h>


extern int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);

char rbuf[512];



int open_hi219(int k)
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



int read_hi219()//初始化gps相关配置
{
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
 
	int ifor=0,jfor=0;
	const int rr=28;
	char rbuf[rr];
	int retv=0;
	int istimevalid=0;
	//sleep(2);
	tcflush(app.app_fid[HI219_FID].fid, TCIOFLUSH);//清空读写缓冲
	while(1)
	{
		retv=0;
		//tcflush(app.app_fid[GPS_FID].fid, TCIOFLUSH);//清空读写缓冲
		//write(app.app_fid[GPS_FID].fid,ubx_poll_s->poll,ubx_poll_s->poll_len);
		while(retv!=10){
			//
			retv+=read(app.app_fid[HI219_FID].fid,rbuf+retv,10-retv);
			//printf(BLUE"RETV=%d\n"NONE,retv);
		}
		//printf(BLUE"RETV=%d\n"NONE,retv);
		for(ifor=0;ifor<retv;ifor++)
		{
			printf(RED"%02X "NONE,rbuf[ifor]);
		}
		//sendnbytes(p_oso->fids[FID_GPS_MQ],rbuf,retv);

		printf("\n");
	}
	return 0;
}


int trg_read_hi219()//初始化gps相关配置
{
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
 
	int ifor=0,jfor=0;
	const int rr=28;
	char rbuf[rr];
	int retv=0;
	int istimevalid=0;
	//sleep(2);
	tcflush(app.app_fid[HI219_FID].fid, TCIOFLUSH);//清空读写缓冲
	while(1)
	{
		retv=0;
		//tcflush(app.app_fid[GPS_FID].fid, TCIOFLUSH);//清空读写缓冲
		//write(app.app_fid[GPS_FID].fid,ubx_poll_s->poll,ubx_poll_s->poll_len);
		while(retv!=10){
			//
			retv+=read(app.app_fid[HI219_FID].fid,rbuf+retv,10-retv);
			//printf(BLUE"RETV=%d\n"NONE,retv);
		}
		//printf(BLUE"RETV=%d\n"NONE,retv);
		for(ifor=0;ifor<retv;ifor++)
		{
			printf(RED"%02X "NONE,rbuf[ifor]);
		}
		//sendnbytes(p_oso->fids[FID_GPS_MQ],rbuf,retv);

		printf("\n");
	}
	return 0;
}





int make_at_cmd(char * at_cmd,char * cmd,char * para)//生产at命令串
{
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
 
	int ifor=0;
	//sleep(2);
	snprintf(at_cmd,40,"AT+%s=%s\r\n",cmd,para);
	return strlen(at_cmd);
}

//AT+SETPTL=A0,B1,D0,D1

int cfg_hi219(char * at_cmd,int length)//将at命令配置给设备
{
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
 
	int ifor=0;
	//sleep(2);
	tcflush(app.app_fid[HI219_FID].fid, TCIOFLUSH);//清空读写缓冲
	int retv=0;
	//tcflush(app.app_fid[GPS_FID].fid, TCIOFLUSH);//清空读写缓冲
	//write(app.app_fid[GPS_FID].fid,ubx_poll_s->poll,ubx_poll_s->poll_len);
	while(retv!=length){
		//
		retv+=write(app.app_fid[HI219_FID].fid,at_cmd+retv,length-retv);
		//printf(BLUE"RETV=%d\n"NONE,retv);
	}
	//printf(BLUE"RETV=%d\n"NONE,retv);
	for(ifor=0;ifor<retv;ifor++)
	{
		printf(GREEN"%02X "NONE,at_cmd[ifor]);
	}
	//sendnbytes(p_oso->fids[FID_GPS_MQ],rbuf,retv);
	printf("\n");
	return 0;
}




int hi219_main(APP_S * app)
{
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
	fd_set   t_set1;//\\判断gps是否有报文输出
    struct timeval  tv;
	int readtimes,ishi219ok=0;
	int ifor=0;
	struct termios opt;//串口参数设置结构体
	/*******************************************************************/
	app->app_fid[HI219_FID].fid=open_hi219(0);//读取gps串口
	/*打开串口 1*/
	/*******************************************************************/
	//tcgetattr(app->app_fid[HI219_FID].fid,&opt);
	//cfmakeraw(&opt);
	/*****************************************************************/
	//cfsetispeed(&opt,B4800); /*波特率设置为 9600bps*/
	//cfsetospeed(&opt,B4800);
	/*****************************************************************/
	//tcsetattr(app->app_fid[HI219_FID].fid,TCSANOW,&opt);
	set_opt(app->app_fid[HI219_FID].fid, 115200, 8, 'N', 1);
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
	FD_SET(app->app_fid[HI219_FID].fid, &t_set1);
	tv.tv_sec= 2;
	tv.tv_usec= 1000;
	int i_state;
	//printf("--------------->GPS_1");
	i_state= select(app->app_fid[HI219_FID].fid+1, &t_set1, NULL, NULL, &tv);
	//printf("--------------->GPS_2\n");
	if (i_state < 0) {
			
		printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
		return -1;
	};
						 
	if (i_state > 0){
	ishi219ok=1;	

	}
	else{
	ishi219ok=0;	
	}
	printf(YELLOW"file=%s,funciton=%s,line=%d\n"NONE, __FILE__,__FUNCTION__,__LINE__);
 	
	char at_cmd[32];
	read_hi219();
	//int cmdlength=make_at_cmd(at_cmd,"SETPTL","A0");
 	//cfg_hi219(at_cmd,cmdlength);
	//cfg_gps();
	//printf(YELLOW"THE i_state=%d isgpsok=%d\r\n"NONE,i_state,isgpsok);
	if(ishi219ok)//如果gps正常,则开始处理程序
		start_process_nmew(1);

	printf(YELLOW"THE END OF GPS\r\n"NONE);
	//myoso->fids[FID_GPS]=close(myoso->fids[FID_GPS]);
	printf(YELLOW"GPS EXIT\r\n"NONE);
	//myoso->status_bit.gps_status=0;
	printf(GREEN"YOU GUESS WHAT ? IT WORKS ,WE JUST FINISH MY JOB--GPS!\n"NONE);
	return 0;
}






/*
00 00 00 5A A5 23 00 83 12 90 
00 A0 99 03 BF 00 A8 FE B0 E6 
FF 0E 00 01 00 C0 45 FF 83 FE 
45 01 D0 F5 E5 01 3B 6F FA F0 
00 00 00 00 5A A5 23 00 5B 1D 
90 00 A0 96 03 BF 00 A8 FE B0 
E5 FF 0E 00 00 00 C0 44 FF 83 
FE 44 01 D0 F5 E5 00 3B 6F FA 
F0 00 00 00 00 5A A5 23 00 D3 
36 90 00 A0 97 03 BE 00 A7 FE 
B0 E5 FF 0E 00 00 00 C0 43 FF 
81 FE 44 01 D0 F5 E5 00 3B 6F 
FA F0 00 00 00 00 5A A5 23 00 
43 7D 90 00 A0 97 03 BE 00 A7 
FE B0 E6 FF 0D 00 00 00 C0 48 
FF 83 FE 43 01 D0 F5 E5 01 3B 
6F FA F0 00 00 00 00 5A A5 23 
00 EB 82 90 00 A0 98 03 BF 00 
A7 FE B0 E6 FF 0E 00 01 00 C0 
48 FF 84 FE 42 01 D0 F5 E5 00 
3B 6F FA F0 00 00 00 00 5A A5 
23 00 E5 55 90 00 A0 98 03 BF 
00 A8 FE B0 E5 FF 0E 00 00 00 
C0 46 FF 84 FE 40 01 D0 F5 E5 
00 3B 6F FA F0 00 00 00 00 5A 
A5 23 00 B1 F5 90 00 A0 98 03 
C0 00 A6 FE B0 E5 FF 0E 00 01 
00 C0 45 FF 81 FE 3F 01 D0 F5 
E5 01 3B 6F FA F0 00 00 00 00 
5A A5 23 00 7E 51 90 00 A0 98 
03 C0 00 A7 FE B0 E5 FF 0E 00 
00 00 C0 44 FF 84 FE 43 01 D0 
F5 E5 00 3B 6F FA F0 00 00 00 
00 5A A5 23 00 4E B3 90 00 A0 
97 03 BC 00 A6 FE B0 E5 FF 0E 
00 00 00 C0 45 FF 84 FE 43 01 
D0 F5 E5 01 3B 6F FA F0 00 00 
00 00 5A A5 23 00 5A 78 90 00 
A0 96 03 BF 00 A8 FE B0 E6 FF 
0E 00 00 00 C0 46 FF 85 FE 43 
01 D0 F5 E5 01 3B 6F FA F0 00 
00 00 00 5A A5 23 00 D9 38 90 
00 A0 97 03 BE 00 A8 FE B0 E4 
FF 0E 00 00 00 C0 44 FF 81 FE 
43 01 D0 F5 E5 00 3B 6F FA F0 
00 00 00 00 5A A5 23 00 62 EE 
90 00 A0 97 03 C0 00 A7 FE B0 
E5 FF 0F 00 01 00 C0 43 FF 80 
FE 45 01 D0 F5 E5 00 3B 6F FA 
F0 00 00 00 00 5A A5 23 00 A0 
A5 90 00 A0 96 03 C0 00 A8 FE 
B0 E5 FF 0D 00 01 00 C0 44 FF 
83 FE 43 01 D0 F5 E5 00 3B 6F 
FA F0 00 00 00 00 5A A5 23 00 
44 B7 90 00 A0 98 03 BE 00 A8 
FE B0 E6 FF 0E 00 00 00 C0 47 
FF 81 FE 44 01 D0 F5 E5 00 3B 
6F FA F0 00 00 00 00 5A A5 23 
00 EE 7A 90 00 A0 97 03 BF 00 
A7 FE B0 E4 FF 0E 00 00 00 C0 
46 FF 81 FE 44 01 D0 F5 E5 00 
3B 6F FA F0 00 00 00 00 5A A5 
23 00 55 F2 90 00 A0 98 03 BF 
00 A5 FE B0 E5 FF 0E 00 00 00 
C0 45 FF 85 FE 43 01 D0 F5 E5 
00 3B 6F FA F0 00 00 00 00 5A 
A5 23 00 1D FF 90 00 A0 98 03 
BD 00 A8 FE B0 E5 FF 0E 00 00 
00 C0 45 FF 81 FE 41 01 D0 F5 
E5 01 3B 6F FA F0 00 00 00 00 
5A A5 23 00 31 32 90 00 A0 97 
03 C1 00 A8 FE B0 E6 FF 0E 00 
00 00 C0 46 FF 84 FE 43 01 D0 
F5 E5 01 3B 6F FA F0 00 00 00 
00 5A A5 23 00 CE 09 90 00 A0 
98 03 C0 00 A7 FE B0 E4 FF 0E 
00 00 00 C0 45 FF 80 FE 46 01 
D0 F5 E5 01 3B 6F FA F0 00 00 
00 00 5A A5 23 00 D8 73 90 00 
A0 97 03 BF 00 A9 FE B0 E6 FF 
0E 00 01 00 C0 47 FF 80 FE 42 
01 D0 F5 E5 01 3B 6F FA F0 00 
00 00 00 5A A5 23 00 CE 69 90 
00 A0 97 03 BE 00 A7 FE B0 E4 
FF 0E 00 00 00 C0 4A FF 85 FE 
43 01 D0 F5 E5 01 3B 6F FA F0 
00 00 00 00 5A A5 23 00 F9 48 
90 00 A0 96 03 BE 00 A6 FE B0 
E5 FF 0E 00 00 00 C0 46 FF 82 
FE 44 01 D0 F5 E5 01 3B 6F FA 
F0 00 00 00 00 5A A5 23 00 40 
E7 90 00 A0 97 03 BF 00 A7 FE 
B0 E6 FF 0E 00 01 00 C0 47 FF 
80 FE 43 01 D0 F5 E5 01 3B 6F 
FA F0 00 00 00 00 5A A5 23 00 
DE 73 90 00 A0 98 03 C2 00 A8 
FE B0 E4 FF 0E 00 00 00 C0 45 
FF 81 FE 40 01 D0 F5 E5 01 3B 
6F FA F0 00 00 00 00 5A A5 23 
00 30 FD 90 00 A0 95 03 BD 00 
A6 FE B0 E4 FF 0E 00 00 00 C0 
43 FF 82 FE 44 01 D0 F5 E5 01 
3B 6F FA F0 00 00 00 00 5A A5 
23 00 5A 5F 90 00 A0 97 03 C0 
00 A6 FE B0 E4 FF 0E 00 00 00 
C0 47 FF 80 FE 44 01 D0 F5 E5 
01 3B 6F FA F0 00 00 00 00 5A 
A5 23 00 0F F0 90 00 A0 96 03 
BE 00 A9 FE B0 E4 FF 0F 00 01 
00 C0 46 FF 7E FE 46 01 D0 F5 
E5 01 3B 6F FA F0 00 00 00 00 
5A A5 23 00 FF 76 90 00 A0 96 
03 BE 00 A9 FE B0 E5 FF 0E 00 
01 00 C0 44 FF 83 FE 44 01 D0 
F6 E5 01 3B 6F FA F0 00 00 00 
00 5A A5 23 00 DE 92 90 00 A0 
96 03 BE 00 A7 FE B0 E6 FF 0E 
00 00 00 C0 46 FF 80 FE 43 01 
D0 F5 E5 01 3B 6F FA F0 00 00 
00 00 5A A5 23 00 D3 C7 90 00 
A0 97 03 BF 00 A8 FE B0 E5 FF 
0E 00 00 00 C0 47 FF 84 FE 44 
01 D0 F5 E5 01 3B 6F FA F0 00 
00 00 00 5A A5 23 00 7B BE 90 
00 A0 97 03 C0 00 A8 FE B0 E5 
FF 0E 00 00 00 C0 45 FF 85 FE 
44 01 D0 F6 E5 00 3B 6F FA F0 
00 00 00 00 5A A5 23 00 A4 22 
90 00 A0 98 03 BF 00 A6 FE B0 
E5 FF 0E 00 00 00 C0 47 FF 7F 
FE 47 01 D0 F5 E5 00 3B 6F FA 
F0 00 00 00 00 5A A5 23 00 42 
F6 90 00 A0 95 03 BF 00 A7 FE 
B0 E4 FF 0E 00 01 00 C0 46 FF 
82 FE 45 01 D0 F6 E5 00 3B 6F 
FA F0 00 00 00 00 5A A5 23 00 
3A 94 90 00 A0 98 03 BF 00 A6 
FE B0 E4 FF 0E 00 00 00 C0 43 
FF 83 FE 41 01 D0 F6 E5 00 3B 
6F FA F0 00 00 00 00 5A A5 23 
00 FF 9C 90 00 A0 98 03 BF 00 
A8 FE B0 E5 FF 0D 00 00 00 C0 
45 FF 87 FE 42 01 D0 F6 E5 00 
3B 6F FA F0 00 00 00 00 5A A5 
23 00 73 78 90 00 A0 96 03 C0 
00 A6 FE B0 E6 FF 0E 00 00 00 
C0 45 FF 84 FE 3F 01 D0 F6 E5 
00 3B 6F FA F0 00 00 00 00 5A 
A5 23 00 50 E8 90 00 A0 95 03 
BF 00 A7 FE B0 E6 FF 0E 00 00 
00 C0 47 FF 84 FE 42 01 D0 F6 
E5 00 3B 6F FA F0 00 00 00 00 
5A A5 23 00 D2 74 90 00 A0 99 
03 BD 00 A7 FE B0 E5 FF 0E 00 
01 00 C0 48 FF 84 FE 44 01 D0 
F6 E5 00 3B 6F FA F0 00 00 00 
00 5A A5 23 00 F1 F0 90 00 A0 
97 03 BF 00 A7 FE B0 E4 FF 0E 
00 00 00 C0 48 FF 82 FE 44 01 
D0 F6 E5 01 3B 6F FA F0 00 00 
00 00 5A A5 23 00 B0 AC 90 00 
A0 97 03 BF 00 A7 FE B0 E5 FF 
0E 00 01 00 C0 43 FF 84 FE 40 
01 D0 F6 E5 01 3B 6F FA F0 00 
00 00 00 5A A5 23 00 62 97 90 
00 A0 95 03 BF 00 A6 FE B0 E4 
FF 0E 00 00 00 C0 46 FF 82 FE 
44 01 D0 F6 E5 01 3B 6F FA F0 
00 00 00 00 5A A5 23 00 24 AD 
90 00 A0 97 03 BE 00 A8 FE B0 
E4 FF 0E 00 01 00 C0 46 FF 84 
FE 45 01 D0 F6 E5 01 3B 6F FA 
F0 00 00 00 00 5A A5 23 00 39 
7D 90 00 A0 97 03 BE 00 A7 FE 
B0 E4 FF 0E 00 01 00 C0 45 FF 
83 FE 43 01 D0 F5 E5 01 3B 6F 
FA F0 00 00 00 00 5A A5 23 00 
A9 8C 90 00 A0 98 03 C0 00 A7 
FE B0 E6 FF 0E 00 00 00 C0 44 
FF 80 FE 43 01 D0 F6 E5 01 3B 
6F FA F0 00 00 00 00 5A A5 23 
00 16 2D 90 00 A0 97 03 C0 00 
A8 FE B0 E5 FF 0E 00 01 00 C0 
43 FF 83 FE 41 01 D0 F6 E5 00 
3B 6F FA F0 00 00 00 00 5A A5 
23 00 6D 5A 90 00 A0 97 03 BF 
00 A8 FE B0 E6 FF 0E 00 00 00 
C0 47 FF 82 FE 44 01 D0 F6 E5 
00 3B 6F FA F0 00 00 00 00 5A 
A5 23 00 5A 92 90 00 A0 95 03 
BF 00 A6 FE B0 E4 FF 0E 00 01 
00 C0 47 FF 84 FE 42 01 D0 F6 
E5 00 3B 6F FA F0 00 00 00 00 
5A A5 23 00 63 F8 90 00 A0 97 
03 BF 00 A6 FE B0 E4 FF 0E 00 
01 00 C0 45 FF 81 FE 42 01 D0 
F6 E5 00 3B 6F FA F0 00 00 00 
00 5A A5 23 00 14 6E 90 00 A0 
97 03 BE 00 A8 FE B0 E4 FF 0E 
00 01 00 C0 47 FF 85 FE 42 01 
D0 F6 E5 00 3B 6F FA F0 00 00 
00 00 5A A5 23 00 34 FA 90 00 
A0 98 03 BE 00 A8 FE B0 E5 FF 
0E 00 00 00 C0 46 FF 81 FE 42 
01 D0 F6 E5 00 3B 6F FA F0 00 
00 00 00 5A A5 23 00 08 62 90 
00 A0 98 03 C0 00 A8 FE B0 E5 
FF 0E 00 00 00 C0 43 FF 82 FE 
42 01 D0 F6 E5 FF 3A 6F FA F0 
00 00 00 00 5A A5 23 00 7E DD 
90 00 A0 98 03 BD 00 A6 FE B0 
E5 FF 0E 00 00 00 C0 47 FF 82 
FE 42 01 D0 F6 E5 FF 3A 6F FA 
F0 00 00 00 00 5A A5 23 00 8B 
8A 90 00 A0 96 03 BD 00 A7 FE 
B0 E5 FF 0E 00 01 00 C0 46 FF 
82 FE 43 01 D0 F6 E5 FF 3A 6F 
FA F0 00 00 00 00 5A A5 23 00 
59 8F 90 00 A0 96 03 BE 00 A6 
FE B0 E5 FF 0D 00 01 00 C0 45 
FF 83 FE 41 01 D0 F6 E5 00 3B 
6F FA F0 00 00 00 00 5A A5 23 
00 D0 04 90 00 A0 96 03 BE 00 
A7 FE B0 E5 FF 0E 00 01 00 C0 
45 FF 83 FE 41 01 D0 F6 E5 00 
3B 6F FA F0 00 00 00 00 5A A5 
23 00 3F 70 90 00 A0 96 03 C1 
00 A8 FE B0 E5 FF 0E 00 01 00 
C0 44 FF 83 FE 45 01 D0 F6 E5 
00 3B 6F FA F0 00 00 00 00 5A 
A5 23 00 F1 23 90 00 A0 99 03 
BF 00 A6 FE B0 E4 FF 0E 00 00 
00 C0 45 FF 82 FE 42 01 D0 F6 
E5 FF 3A 6F FA F0 00 00 00 00 
5A A5 23 00 63 FF 90 00 A0 96 
03 BE 00 A7 FE B0 E5 FF 0E 00 
00 00 C0 47 FF 80 FE 45 01 D0 
F6 E5 FF 3A 6F FA F0 00 00 00 
00 5A A5 23 00 14 27 90 00 A0 
96 03 BE 00 A8 FE B0 E5 FF 0E 
00 01 00 C0 43 FF 80 FE 41 01 
D0 F6 E5 00 3B 6F FA F0 00 00


*/

