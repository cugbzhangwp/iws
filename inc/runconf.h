#ifndef _RUNCONF_H
#define _RUNCONF_H
#include <spi_rw.h>
#include <constmacro.h>
#include <pthread.h>
#define SPI_DEV_NUM 5
#define THREADNUM 20
#define IWSPARAFILE "iwspara.para"
#define IWSPARAPATH "/home/pi/Geo_Pi"
#define IWSNTPPATH "/etc"



#define IWSPARAFILE_ORIG "iwspara.para"
#define IWSPARAPATH_ORIG "/home/pi"



#define IWS_SVR_PARAFILE "iws_svr_para.para"



#define IWS_SVR_PARAFILE_ORIG "iws_svr_para.para"










//char * strsource={"a","b"};
typedef struct TREAD_SWITCH{//线程开关
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
	int usrwifi_loop;
	int netvkey_loop;
	int unuse[17];
}__attribute__ ((packed,aligned(1)))TREAD_SWITCH_S,*TREAD_SWITCH_T;


typedef struct CMD_LINE_OPT{//命令行参数
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
	char unuse[472];
}__attribute__ ((packed,aligned(1)))CMD_LINE_OPT_S;



typedef struct APP_COUNT{//命令行参数
	int second_count;
	struct timespec date_time;//数据采集时间
	int si_count;//计算状态信息总计数
	int si_count_second;//计算状态信息秒计数
	int si_count_times;//计算状态信息秒计数
	int sample_times_orig;//ad原始读取次数
	int sample_times_orig_valid;//ad原始读取次数
	int sample_times_ch;//平均到每个通道ad原始读取次数
	int resample_times;//经过重采样后采样次数
	int last_trig;//最后一次触发计数
	int trig_start;//触发启动计数
	int trig_count;//
	int trig_count2;
	int before_trig;
	int end_trig;
	int unuse[512-456-8];
}__attribute__ ((packed,aligned(1)))APP_COUNT_S;

typedef struct AD_REG{//命令行参数
	unsigned char reg[20];
}__attribute__ ((packed,aligned(1)))AD_REG_S;


typedef struct APP_PARA{//程序参数
	AD_REG_S ad_reg;
	AD_REG_S ad_reg_default;
	int ad_ch_select;
	unsigned int fs;//解析出的采样率
	unsigned int gain;//解析出的放大系数
	char unuse[512-52];
}__attribute__ ((packed,aligned(1)))APP_PARA_S;







typedef struct APP_DEVICE{//命令行参数
	char name[20]; //"oled"
	char device[80];// "/dev/zwp_spi1.1"
	int dev_id;// is or is not need a thread to run
	int dev_use_count;
}__attribute__ ((packed,aligned(1)))APP_DEVICE_S;







typedef struct APP_DEVICE_CONF{//LET APP_DEVICE_S->dev_conf TO IT ITEM
	SPICONF_S spi_conf[SPI_DEV_NUM];
}__attribute__ ((packed,aligned(1)))APP_DEVICE_CONF_S;

typedef struct TEST1{//命令行参数
	char name[20]; //"oled"
	char device[80];// "/dev/zwp_spi1.1"
	int dev_id;// is or is not need a thread to run
}__attribute__ ((packed,aligned(1)))TEST1_S;


typedef struct TEST{//命令行参数
	char name[20]; //"oled"
	char device[80];// "/dev/zwp_spi1.1"
	int dev_id;// is or is not need a thread to run
	TEST1_S test1;
}__attribute__ ((packed,aligned(1)))TEST_S;



typedef struct THREAD_COTR{//命令行参数
	struct THREAD_COTR * this;
	char name[48]; //线程名字
	int tid;
	pthread_t app_thread_id;
	int (* thread_func)(void *);//线程函数
	void * para;//参数
	int run_status;//当前运行状态;
	int conf_status;//配置开关
	int dependent;//运行所依赖的线程
	int index;//线程私有索引
	pthread_mutex_t * mut[2];
	pthread_cond_t  * cond[2];
	int control[20];
}__attribute__ ((packed,aligned(1)))THREAD_COTR_S;



//pthread_t app_thread[THREADNUM];

typedef struct P_COTR{//进程控制
	int pid;//进程id
	int a;//
}__attribute__ ((packed,aligned(1)))P_COTR_S;


typedef struct FILE_CENTER{
 pid_t pid;
 pid_t ppid;
 char path[100];
 char filename[100];
 char suffix[8];
 FILE * fid;
 int write_count;
 int read_count;
 int tmp_count;
 int file_size;
 int control;
 int status;
}__attribute__ ((packed,aligned(1)))FILE_CENTER_S;



typedef struct FID_CENTER{//文件中心
	int pid;
	char path[100];
	char filename[100];
	int fid;
	int status;
}__attribute__ ((packed,aligned(1)))FID_CENTER_S;




typedef struct FID_UDP{//文件中心
	int pid;
	char path[100];
	char filename[100];
	int fid;
	int status;
}__attribute__ ((packed,aligned(1)))FID_UDP_S;



typedef struct FID_TCP{//文件中心
	int pid;
	char path[100];
	char filename[100];
	short port;
	int fid;
	int status;
}__attribute__ ((packed,aligned(1)))FID_TCP_S;






typedef struct SYNC_CTRL{//同步中心
	#define MUTNUM 4
	#define CONDNUM 4
	pthread_mutex_t mut[MUTNUM];
	pthread_cond_t cond[CONDNUM];
}__attribute__ ((packed,aligned(4)))SYNC_CTRL_S;


#endif



/*

SN=SN:20170314
HW="==HW D2.1=="
SWITCH_LED=YES
SWITCH_OLED=YES
PRIORITY=-10
MAIN_LOOP=ON
GPS_LOOP=ON
GPIO_LOOP=OFF
AD_LOOP=ON
TCP_SERVER_LOOP=ON
UDP_BEAT_LOOP=ON
UDP_DATA_LOOP=OFF
TCP_DATA_LOOP=ON
FILE_LOOP=ON
FILE_NAME_LOOP=ON
FILE_USYN_CLOSE_LOOP=ON
OLED_DISPLAY_LOOP=ON
KEYBOARD_LOOP=ON
USRWIFI_LOOP=OFF

PATH=/usr/sbin/OL_PATH:$PATH

export PATH SN HW SWITCH_LED SWITCH_OLED PRIORITY
export MAIN_LOOP
export GPS_LOOP
export GPIO_LOOP
export AD_LOOP
export TCP_SERVER_LOOP
export UDP_BEAT_LOOP
export UDP_DATA_LOOP
export TCP_DATA_LOOP
export FILE_LOOP
export FILE_NAME_LOOP
export FILE_USYN_CLOSE_LOOP
export OLED_DISPLAY_LOOP
export KEYBOARD_LOOP
export USRWIFI_LOOP

*/











