#ifndef COM_H
#define COM_H
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <sys/vfs.h>
//#include <linux/jiffies.h>
#include <errno.h>
#include "para.h"
#include "oso_command.h"
#include "inc/hardwarestate.h"
#include "printcolor.h"
#include "filehead.h"
#include "abollo_para.h"
#define BS 192000
#define FILESIZE 40960*3
#define MAXDATASIZE 1000 /* Max number of bytes of data */
#define DEFAULTPARAFILE "/lib/.para.osp"
//#define MAXDATASIZE 1024 /* Max number of bytes of data */

#define BOOL int
#define TRUE 1
#define FALSE 0
#define DEBUGRUN 0
#define OSO_SOFT_VERSION "v1.0.0.0"
#define MUT_CLIENT_FILE 0
#define MUTGPIO 1
#define MUTREAD 2
#define MUTALL 3
#define MUTMAIN 4
#define MUT_TCP_DATA 5
#define MUT_FILE_NAME 6
#define COND_CLIENT_FILE 0
#define CONDGPIO 1
#define CONDREAD 2
#define CONDALL 3
#define CONDMAIN 4
#define COND_TCP_DATA 5
#define COND_FILE_NAME 6

#define MUT_FILE_CLOSE 7
#define COND_FILE_CLOSE 7






#define FID_GPIOCTL0 0
#define FID_ZWP_SSC0 1
#define FID_GPS 2
#define FID_CLIENT_DATA 3
#define FID_TCP_SER 4
#define FID_UDP_SER 5
#define FID_TCP_CLI 6
#define FID_TCP_CLI1 7
#define FID_SPI0 8





#define FILE_THREAD_SWITCH 1





#define PTH_MAIN 0
#define PTH_GPS 1
#define PTH_GPIO 2
#define PTH_AD 3
#define PTH_TCP_SER 4
#define PTH_UDP_BEAT 5
#define PTH_UDP_DATA 6
#define PTH_FILE 7
#define PTH_TCP_DATA 8
#define PTH_FILE_NAME 9
#define PTH_FILE_USY_CLOSE 10


#define SECONDDATA 4



#define ONEDAYSEC 86400
#define ONEHOURSEC 3600
#define ONEMINSEC 60
#define DATA1000 1000



#define DEBUGPRINT(sentence) if(DEBUGRUN) sentence		
#define tellsize(v) printf("sizeof v =%d\n",sizeof(v));
//设置x的第y位为1
#define Setbit(x,y) (x)|=(1<<(y-1))
//得到x的第y位的值
#define Getbit(Number,pos) ((Number)>>(pos-1)&1)
//打印x的值
#define print(x) printf("%d\n",x)
//将整数(4个字节)循环右移动k位
#define Rot(a,k) ((a)<<(k)|(a)>>(32-k))
//判断a是否为2的幂次数
#define POW2(a) ((((a)&(a-1))==0)&&(a!=0))
#define OPPX(x) (~(x)+1)


typedef unsigned int s32;
typedef unsigned short s16;
typedef unsigned char u8;

typedef struct beat{
int pak_type;
unsigned long pk_count;
unsigned long ip;
char str[4];
}__attribute__ ((packed,aligned(1)))beat_s,*beat_t;
/*
typedef	struct  NET_packet_t{
	s16 id_net;		//IP地址的最后一个字节
	s32 pakcet_num;	//当前数据包的包号
	s16 pakcet_len;	//当前数据包的长度，总长度
	s16 valid_len;  //有效长度
	char filename[32];//文件名
	s16 frame;      //数据帧
	char reserve[22]; //预留
	s32 crc_val;		//预留用作CRC校验
	char gps[400];//GPS包
	//u8 packet_buff[BS];//ad数据
}__attribute__ ((packed,aligned(1)))NET_packet;*/
typedef	struct  NET_packet_t{
	s16 id_net;		//IP地址的最后一个字节
	s32 pakcet_num;	//当前数据包的包号
	s16 pakcet_len;	//当前数据包的长度，总长度
	s16 valid_len;  //有效长度
	char filename[32];//文件名
	s16 frame;      //数据帧
	int fs; //预留
	int ch_num;
	char reserve[14];
	s32 crc_val;		//预留用作CRC校验
	char gps[400];//GPS包
	//u8 packet_buff[BS];//ad数据
}__attribute__ ((packed,aligned(1)))NET_packet;

typedef struct switch_bit{///控制各个线程循环
char main_loop:1;
char gps_loop:1;
char gpio_loop:1;
char ad_loop:1;
char tcp_server_loop:1;
char udp_beat_loop:1;
char udp_data_loop:1;
char file_loop:1;
char tcp_data_loop:1;
char file_name_loop:1;
char file_usyn_close_loop:1;
char  :5;
}switch_bit;


typedef struct switch_bit_ispause{///控制各个线程循环zhan ting
char main_loop:1;
char gps_loop:1;
char gpio_loop:1;
char ad_loop:1;
char tcp_server_loop:1;
char udp_beat_loop:1;
char udp_data_loop:1;
char file_loop:1;
char tcp_data_loop:1;
char file_name_loop:1;
char file_usyn_close_loop:1;
char  :5;
}switch_bit_ispause;











typedef struct status_bit{//各个线程现在状态指示
char main_status:1;
char gps_status:1;
char gpio_status:1;
char ad_status:1;
char tcp_server_status:1;
char udp_beat_status:1;
char udp_data_status:1;
char file_status:1;
char tcp_data_status:1;
char file_name_status:1;
char file_usyn_close_status:1;
char :5;
}status_bit;


typedef struct signal_bit{
char main_signal:1;
char gps_signal:1;
char gpio_signal:1;
char ad_signal:1;//is there ad data no be write or send
char tcp_server_signal:1;
char udp_beat_signal:1;
char udp_data_signal:1;
char file_signal:1;//is there data
char tcp_data_signal:1;
char :7;
}signal_bit;


typedef struct tcp_data_bit{
char main_signal:1;
char gps_signal:1;
char gpio_signal:1;
char ad_signal:1;//is there ad data no be write or send
char tcp_server_signal:1;
char udp_beat_signal:1;
char udp_data_signal:1;
char file_signal:1;//is there data
}tcp_data_bit;

typedef struct gpstime_fpga{
unsigned int gps_lable:5;
unsigned int year:7;
unsigned int mouth:4;
unsigned int day:5;
unsigned int hour:5;
unsigned int min:6;
unsigned int gps_lable2:6;
unsigned int sec:6;
unsigned int msec:10;
unsigned int usec:10;
}__attribute__ ((packed,aligned(1)))gpstime_f;

typedef struct change_bit{
char isnewfile:1;
char isprojupdate:1;
char isgps:1;
char isfirstin:1;
char isparachange:1;
char isnospace:1;
char :3;
}change_bit;


/*
typedef struct gpstime_fpga{
unsigned int usec:10;
unsigned int msec:10;
unsigned int sec:6;
unsigned int gps_lable2:6;
unsigned int min:6;
unsigned int hour:5;
unsigned int day:5;
unsigned int mouth:4;
unsigned int year:7;
unsigned int gps_lable:5;
}__attribute__ ((packed,aligned(1)))gpstime_f;
*/

typedef struct GPSRMC_str{
char nmea[7]; 
char utcdatetime[10];//YMDHMS
char Status[2]; //A/V
char Latitude[10]; //ddmm.mmmm
char NS[2]; //N/S
char Longitude[10]; //dddmm.mmmm
char EW[2]; //E/W
char Speed[5]; //速率000.0~999.9节
char Course[5]; //航向000.0~359.9度
char utcymdtime[6];
char mag1[10];
char mag2[2];
char mode[2];
}__attribute__ ((packed,aligned(1)))GPSRMC_str;

typedef struct GPSRMC_data{
char nmea[7]; 
float utcdatetime;//YMDHMS
char Status[2]; //A/V
float Latitude; //ddmm.mmmm
char NS[2]; //N/S
float Longitude; //dddmm.mmmm
char EW[2]; //E/W
float Speed; //速率000.0~999.9节
float Course; //航向000.0~359.9度
int utcymdtime;
float mag1;
float mag2;
char mode[2];
}__attribute__ ((packed,aligned(1)))GPSRMC_data;





typedef struct oso_n_cmd{
int pak_type;
int cmd;
int t[10];
}__attribute__ ((packed,aligned(1)))cmd_s,*cmd_t;

struct ch_para{

unsigned short para_ad_1282;
unsigned short para_ad_hpf;

};

/*
typedef struct oso_n_para{
int pak_type;
int para_isautostart;
unsigned long para_ad;
unsigned long para_ad_spi;
unsigned short para_ad_1282;
unsigned int ch_para[];
unsigned long forex[2];
unsigned long para_ch;
unsigned long para_ip_server;
int para_file_size;
int beat_bt;
char **argv;
}__attribute__ ((packed,aligned(1)))para_s,*para_t;

*/

typedef struct oso_n_para{
int pak_type;
int para_isautostart;
unsigned long para_ad;// 5373 use
unsigned short para_ch_spi;//ch mask
unsigned short para_ad_spi;//mode fs etc...
struct ch_para ch_para[10];//gain &hpf filter
unsigned long filename_num; //file num use
unsigned long forex;//
unsigned long para_ch;//ch num
char projname[32];
unsigned long para_ip_server;
int para_file_size;
int beat_bt;
char **argv;
}__attribute__ ((packed,aligned(1)))para_s,*para_t;










typedef struct ctrl{
	int count,startsend,startgettime,startgetdata,fdio,mainctl,ctltotal,io_cmd,io_arg,io_do,read_send;
}ctrl;

typedef struct oso_n_statfs{
	struct statfs diskInfo;
	char tgt[50];
}oso_n_statfs;

typedef struct count{
	int count_one_read;
	unsigned long long file_write_count;//8 byte
	unsigned long long file_write_count_all;
	unsigned long long data_read_count;
	unsigned long long data_read_count_all;
	unsigned long long data_send_count;
	unsigned long long data_send_count_all;
}__attribute__ ((packed,aligned(1)))counter;



typedef struct oso_n_info{
int pak_type;
short run_status;
unsigned long  long free_sd;//sd card free space
unsigned long  long free_mem;//free memery //but now i just use it as total size of sdCard
counter count;//52
int bat;//energy
struct sysinfo sys_info;
char gps_info[400];
}__attribute__ ((packed,aligned(1)))oso_n_info_s,*oso_n_info_p; 



typedef struct filehead{
para_s * para;
char * filename_write;
counter * count;
oso_n_info_s * info;
}__attribute__ ((packed,aligned(1)))filehead;



typedef struct oso_size{
unsigned int self;
unsigned int oso;
unsigned int filehead;
unsigned int oso_n_info_s;
unsigned int counter;
unsigned int para_s;
unsigned int NET_packet;
unsigned int cmd_s;
unsigned int beat_s;
}__attribute__ ((packed,aligned(1)))oso_size;



typedef struct oso_hw_state{
short ssc;
short spi;
short gps;
short gpio;
short sd;
short rj45;
char r[20];
}__attribute__ ((packed,aligned(1)))oso_hw_state;


typedef        struct _DATE_TIME                    //年月日时分秒毫秒
{
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    unsigned short msel;                        //millisecond
    unsigned short usel;                        //millisecond
    unsigned short nsel;                        //millisecond
} DATE_TIME;



typedef struct oso{
	struct oso * self;
	para_s para;
	para_s para_next;
	oso_size oso_size;
	ABL_TX_CMD para_m;
	int is_save_file;
	oso_n_statfs sd_info;
	oso_n_info_s info;
	DATE_TIME mydate;
	beat_s * mybeat;
	oso_hw_state hw_state;
	char parafile[60];
	char projname[32];
	char projpath[50];
	char projpath_now[50];
	char filename[20];//get from gps ,will change anytime
	char filename_write[100];//get from filename ,will change when need
	char folder[10];
	unsigned short fpgainfo[10];
	ctrl ctrl;
	switch_bit switch_bit;
	status_bit status_bit;
	signal_bit signal_bit;
	switch_bit_ispause switch_bit_ispause;
	FILE *fp_para;
	FILE *fp;
	FILE *fp_data;
	FILE *fp_data_close;
	FILE *fp_proj_filenum;
	FILE *fp_close;
	void * buffer_read;
	void * buffer_write;
	void * buffer1;
	void * buffer2;
	oso_head * oso_n_head;
	char bc;
	int buffer_size;
	void * cpbuf;
	NET_packet *sendbuf;
	oso_head * oso_head;
	int which;
	int isnewfile;
	change_bit change_bit;
	int is_file_open;
	int miss_write;
	counter count;
	int fids[15];
	pthread_t oso_thread[11];
	pthread_mutex_t mut[8];
	pthread_cond_t cond[8];
	
}__attribute__ ((packed,aligned(1)))oso;





extern int update_parafile(oso *);
unsigned int  file_usynclose(oso *);
extern void printbin(void *p,int i);
extern int thread_update(oso * myoso);
extern int tcp_send_data_A(oso * myoso);
extern int is_dir_exist(const char * pathname);
extern int makeproj(oso * );
extern int spi_ad_cfg(int ctrlnum);
extern int ispause,fdio;
#endif
