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
#include <ifaddrs.h>
#include "common.h"
#include "para.h"
#include "oso_command.h"
#include "hardwarestate.h"
#include "printcolor.h"
#include "filehead.h"
#include <abollo_para.h>
#include "ledcmd.h"
#define BS 192000
#define FILESIZE 40960*3
#define LOOPBUFNUM 64
#define LOOPBUFSIZE 262144
#define LOOPBUFTOTALSIZE LOOPBUFSIZE*LOOPBUFNUM//4000HZ*4BYTE*16CH*loopnum
#define MAXDATASIZE 1000 /* Max number of bytes of data */
#define DEFAULTPARAFILE "/lib/.para.abp"
//#define MAXDATASIZE 1024 /* Max number of bytes of data */


#define THREADNUM 20//总的线程数量

#define CONDNUM 10// 条件变量数量


#define LOGSTRSIZE 200


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

#define COND_CLIENT_FILE 0
#define CONDGPIO 1
#define CONDREAD 2
#define CONDALL 3
#define CONDMAIN 4
#define COND_TCP_DATA 5



#define MUT_FILE_NAME 6
#define COND_FILE_NAME 6

#define MUT_FILE_CLOSE 7
#define COND_FILE_CLOSE 7

#define MUT_WRITE_SD 8
#define COND_WRITE_SD 8




#define FID_GPIOCTL0 0
#define FID_ZWP_SSC0 1
#define FID_GPS 2
#define FID_CLIENT_DATA 3
#define FID_TCP_SER 4
#define FID_UDP_SER 5
#define FID_TCP_CLI 6
#define FID_TCP_CLI1 7
#define FID_SPI0 8
#define FID_KEYBOARD_KEYS0 9





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
#define PTH_WRITE_SD 11
#define PTH_DISPLAY 12
#define PTH_KEYBOARD 13
#define PTH_USRWIFI 14
#define PTH_NETVKEY 15

#define SECONDDATA 4

#define GPSLOCKLABEL 62//gps锁定位是否置位决定此值
#define GPSUNLOCKLABEL 60

#define ONEDAYSEC 86400
#define ONEHOURSEC 3600
#define ONEMINSEC 60
#define DATA1000 1000



#define SSCUNKNOWERR (1<<0)
#define GPSUNKNOWERR (1<<1)
#define SDUNKNOWERR (1<<2)
#define FPGAUNKNOWERR (1<<3)
#define BATTUNKNOWERR (1<<4)
#define WEBUNKNOWERR (1<<5)




#define LINE1START 0
#define LINE1END 19
#define LINE2START 20
#define LINE2END 39
#define LINE3START 40
#define LINE3END 59
#define LINE4START 60
#define LINE4END 79





#define CH1MASK 0x0100
#define CH2MASK 0x0200
#define CH3MASK 0x0400
#define CH4MASK 0x0800
#define CH5MASK 0x1000
#define CH6MASK 0x2000
#define CH7MASK 0x4000
#define CH8MASK 0x8000
#define CH9MASK 0x0001
#define CH10MASK 0x0002
#define CH11MASK 0x0004
#define CH12MASK 0x0008
#define CH13MASK 0x0010
#define CH14MASK 0x0020
#define CH15MASK 0x0040
#define CH16MASK 0x0080




//菜单层级定义===>
#define MAINSCREEN 0
#define MAINMENU 1
#define INFOMENU 5
#define SENDSETTING 2//M序列参数设置
#define SYNCSETTING 3//M序列参数设置
#define FLASHSETTING 4//M序列参数设置
#define ABOUTSETTING 5//M序列参数设置
#define RXSETTING 6//M序列参数设置
#define FPGASETTING 29//FPGA TRANSIMITTER参数设置
#define SENDSETTING_BASE_SPACE SENDSETTING*5
#define SYNCSETTING_BASE_SPACE SYNCSETTING*5
#define FLASHSETTING_BASE_SPACE FLASHSETTING*5
#define ABOUTSETTING_BASE_SPACE ABOUTSETTING*5
#define RXSETTING_BASE_SPACE RXSETTING*5

#define SENDSETTING_ORDER SENDSETTING_BASE_SPACE+SENDSETTING+0//阶数设置
#define SENDSETTING_FREQ SENDSETTING_BASE_SPACE+SENDSETTING+1//频率设置
#define SENDSETTING_OTHER SENDSETTING_BASE_SPACE+SENDSETTING+2
#define SENDSETTING_OTHER2 SENDSETTING_BASE_SPACE+SENDSETTING+3
#define SENDSETTING_PLAN_A_B_C SENDSETTING_BASE_SPACE+SENDSETTING+4

#define SYNCSETTING_AUTO SYNCSETTING_BASE_SPACE+SYNCSETTING+1
#define SYNCSETTING_MANU SYNCSETTING_BASE_SPACE+SYNCSETTING+0

#define FLASHSETTING_RM FLASHSETTING_BASE_SPACE+0
#define FLASHSETTING_FORMAT FLASHSETTING_BASE_SPACE+1
#define FLASHSETTING_DISCONNECT FLASHSETTING_BASE_SPACE+2

#define RXSETTING_CHSWITCH 30+0;
#define RXSETTING_CHGAIN 30+1;


//菜单层级定义<===

#define DISP_SCREEN_POWEROFF 41
#define DISP_SCREEN_OPERATE 42

#define DEBUGRUN 1



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
	s32 pakcet_num;	//当前数据包的包号(一个缓冲区里所有数据分成若干包发送)
	s16 pakcet_len;	//当前数据包的长度，总长度
	s16 valid_len;  //此包中数据段的有效长度
	char filename[32];//文件名
	s16 frame;      //数据帧(一个缓冲区的数据相当于一帧)
	int fs; //预留
	int ch_num;
	unsigned int read_seq;
	unsigned int sendbufused;
	char reserve[6];
	s32 crc_val;		//预留用作CRC校验
	char gps[100];//GPS包
	char unuse[300];
	//u8 packet_buff[BS];//ad数据
}__attribute__ ((packed,aligned(1)))NET_packet;

typedef struct switch_bit{///控制各个线程开关
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
char oled_display_loop:1;
char keyboard_loop:1;
char usrwifi_loop:1;
char netvkey_loop:1;
char  :1;
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
char oled_display_loop:1;
char keyboard_loop:1;
char usrwifi_loop:1;
char :2;
}switch_bit_ispause;

typedef struct control_exit{///控制各个线程循环zhan ting
int main_loop;
int gps_loop;
int gpio_loop;
int ad_loop;
int tcp_server_loop;
int udp_beat_loop;
int udp_data_loop;
int file_loop;
int tcp_data_loop;
int file_name_loop;
int file_usyn_close_loop;
int oled_display_loop;
int keyboard_loop;
}control_exit;









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
char m_sync_status:1;
char m_sync_status_man:1;
char oled_display_status:1;
char keyboard_status:1;
char usrwifi_status:1;
char netvkey_status:1;
char :7;
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
char is_sync:1;
char :6;
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
char isgpsfileopen:1;
char islogfileopen:1;
char :1;
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
typedef struct CMD_ECHO{
int cmdecho_num;
char unuse[6];
}__attribute__ ((packed,aligned(1)))CMD_ECHO;
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




typedef struct GPGGA_data{
char nmea[7]; 
float utcdatetime;//YMDHMS
float Latitude;
char NS[2]; //N/S
float Longitude; //dddmm.mmmm
char EW[2]; //E/W
int FS;
int NoSV;
float HDOP;
float msl;
char uMsl;
float Altref;
char uSep;
int DiffAge;
int DiffStation;
char cs;
}__attribute__ ((packed,aligned(1)))GPGGA;


struct MyTm
{
	int tm_sec;     
	int tm_min;     
	int tm_hour;    
	int tm_mday;    
	int tm_mon;     
	int tm_year;    
	int tm_zone;    
};

struct gps_data_t
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
unsigned short cres;//ce dian zu
unsigned short bediscardsd;
char **argv;
unsigned short shortunuse[36];

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




typedef struct count2{
	unsigned int copycount;//统计写入大缓冲区的次数,写入文件后置零
	unsigned int writeframe;//copycount置位前的值
}__attribute__ ((packed,aligned(1)))counter2;




typedef struct timecounter{
	unsigned int count_one_read;
	unsigned int readfrom_ssc;//8 byte
	unsigned int gps_sec;
	unsigned int write_sd;
	unsigned int sendto_pc;
	unsigned int file_process;
	unsigned int unuse[3];
}__attribute__ ((packed,aligned(1)))timecounter;



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



typedef struct pth_statue{//各个线程现在状态指示
int status[14];
}pth_status;

typedef struct SCREEN{
int nowscreennum;//当前显示序号
int defaultscreennum;//默认显示序号

}SCREEN;

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


typedef struct mytime{
struct tm *timenow;
time_t timep;
char timestr[30]; 
}__attribute__ ((packed,aligned(1)))OSOTIME;




typedef struct ledswitch_bit{
char led0:1;
char led1:1;
char led2:1;
char led3:1;
char :4;
}ledswitch_bit;





typedef struct ledctrl{
short bcbc[10];//用于乒乓操作控制
ledswitch_bit ledswitch;
}__attribute__ ((packed,aligned(1)))LEDCTRL;

typedef struct PFUNC{
	int i;//(* fucp[THREADNUM])(oso * );
}__attribute__ ((packed,aligned(1)))PFUNC;

typedef struct PARA_INFO{
	int ch_isopen;//open or close;
	int ch_fs;
	int gain;
	int mode;
	int r[4];
}__attribute__ ((packed,aligned(1)))PARA_INFO;

typedef struct OSO_SINGLE{
	int sig_data_ready;//data ready from AD
	int sig_data_save;//data ready from file_name
	int sig_data_send;//data ready from file_process
}__attribute__ ((packed,aligned(1)))OSO_SINGLE;



typedef struct LOOPBUFFER{
struct LOOPBUFFER *	loopbufnext;
int realdatasize;
void * nextdatastart;
}LOOPBUFFER;


typedef struct TCP_DATA_STATUS{//各个线程的运行控制量
int isconnect;
char unuse[64];
}__attribute__ ((packed,aligned(1)))TCP_DATA_STATUS;


typedef struct PTHREAD_STATUS{//各个线程的运行控制量
TCP_DATA_STATUS tcp_data_status;
char unuse[960];
}__attribute__ ((packed,aligned(1)))PTHREAD_STATUS;

#define FILE_ABL 0//写的abl文件流
#define FILE_ABL_CLOSE 1//关闭的abl文件流
#define FILE_ABL_GPS 2//关闭的abl文件流
#define FILE_ABL_LOG 3//关闭的abl文件流


#define CMD_PAK_SIZE sizeof(cmd_s)
#define PARA_PAK_SIZE sizeof(para_s)
#define INFO_PAK_SIZE sizeof(oso_n_info_s)
#define ABL_TX_CMD_SIZE sizeof(ABL_TX_CMD)


typedef struct LOCK_UNLOCK{
	int pre_sync;
	int sync_ctrl;
	pthread_mutex_t mut;
	pthread_cond_t cond;
	int type;
}__attribute__ ((packed,aligned(1)))LOCK_UNLOCK,*LOCK_UNLOCK_P;


typedef struct S_CMD_LINE_OPT{
	int mode; 
	int isdebug;

	int is_main_debug;
	int is_gps_debug;
	int is_ad_debug;
	int is_fileprocess_debug;
	int is_tcpserver_debug;
	int is_tcpdata_debug;
	int is_ad1_debug;
	int is_ad2_debug;

	char unuse[504];
}__attribute__ ((packed,aligned(1)))CMD_LINE_OPT;

typedef struct OSO_NT{
	char u[512];
}__attribute__ ((packed,aligned(1)))OSO_NT_S;
typedef struct oso{
	struct oso * self;//
	para_s para;
	para_s para_next;
	oso_size oso_size;
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
	LEDCTRL ledctrl;
	OSOTIME osotime;
	ctrl ctrl;
	switch_bit switch_bit;
	status_bit status_bit;
	signal_bit signal_bit;
	switch_bit_ispause switch_bit_ispause;
	FILE * fp_para;
	FILE * fp;
	FILE * fp_close;
	FILE * fp_write;
	FILE * fp_data;
	FILE * fp_data_close;
	FILE * fp_proj_filenum;
	FILE * fp_log;
	FILE * fp_gps;
	FILE * p_file[8];
	void * buffer_read;
	void * buffer_write;
	void * buffer_send;
	void * buffer1;
	void * buffer2;
	unsigned char * bigbuf_read;//数据从buffer1暂存如bigbuf
	unsigned char * bigbuf_write;
	unsigned char * bigbuf_send;
	unsigned char * bigbuf1;//数据从buffer1暂存如bigbuf
	unsigned char * bigbuf2;
	oso_head * oso_n_head;
	char bc;
	int buffer_size;
	void * cpbuf;
	NET_packet *sendbuf;
	oso_head * oso_head;
	int which;
	int isnewfile;
	int closefile;
	change_bit change_bit;
	int is_file_open;
	int is_save_file;
	int miss_write; 
	counter count;
	counter2 count2;
	timecounter timer;//记录运行过程中的相对时间
	int fids[16];
	pthread_t oso_thread[THREADNUM];
	unsigned long tids[THREADNUM];
	unsigned int timecount[THREADNUM];
	pth_status pth_status;
	pthread_mutex_t mut[CONDNUM];
	pthread_cond_t cond[CONDNUM];
	double m_time_fpga;
	ABL_TX_CMD para_m;
	ABL_TX_CMD para_m_up;//从上位机配置的m序列参数
	ABL_TX_CMD_M2 para_m2;
	int isbreakdown;
	control_exit control;
	OSO_SINGLE oso_sig;
	PTHREAD_STATUS thread_status;
	CMD_LINE_OPT opts;
	unsigned short chmask[16];
	//OSO_NT_S oso_nt;
}__attribute__ ((packed,aligned(1)))oso;

typedef struct pointcenter{//
oso * myoso;
NET_packet *sendbuf;
}__attribute__ ((packed,aligned(1)))POINTCENTER;


extern int fsync_close(FILE * fp);
extern unsigned int  file_process_udd(oso * myoso);
extern int check_sum(char * Buffer,int N);
extern int file_size(char* filename);
extern long long rtt_mktime(DATE_TIME time);
extern int  init_led(oso * myoso);//使led灯进入故障指示模式
extern int send_para_spi0_multi1282(oso * myoso);
extern int set_abollo_send(oso * myoso);
extern int m_sync_manu_cancel(oso * myoso) ;
extern int m_sync(oso * myoso);
extern int m_sync_manu(oso * myoso);
extern int  breakdown_led(oso * myoso);//使led灯进入故障指示模式
extern int update_m_parafile_A(ABL_TX_CMD * m_para);
extern int display(oso * myoso);
extern int keyboard(oso *);
extern int usrwifi(oso *);
extern int netvkey(oso *);
extern int getethstatus();
extern int print_m_para(oso * myoso);
extern int update_parafile(oso *);
extern int update_m_parafile(oso * myoso);
extern int check_update_from_udisk();
unsigned int  file_usynclose(oso *);
extern void printbin(void *p,int i);
extern int thread_update(oso * myoso);
extern int tcp_send_data_A(oso * myoso);
extern int is_dir_exist(const char * pathname);
extern int makeproj(oso * );
extern int spi_ad_cfg(int ctrlnum);
extern int data_ser_epoll();
extern int ispause,fdio;
extern char displaybuffer[][80];
extern int isneedupdate;
extern SCREEN myoled;
extern unsigned int mainscreen_index;
extern int nowindex;
extern int ctltotal;
extern int user_is_save_file;
extern int is_auto_sync;//开关是否自动同步,定义在keyboard中
extern unsigned int transplan_index;
extern LOOPBUFFER *	loopbuflist,*loopread,*loopwrite,* loopbuflisthead,*loopnow,*loopsend;
#endif
