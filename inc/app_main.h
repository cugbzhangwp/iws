#ifndef _MAIN_H
#define _MAIN_H
#include <runconf.h>
#include <sys/time.h>    // for gettimeofday()
#include <time.h>    // for gettimeofday()
#include <ad1256.h>
#include <iws.h>
#include <alg.h>
#include <ublox.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern int setbit(int num, int bit);
extern int clearbit(int num, int bit);
extern int testbit(int num, int bit);
extern char setbitchar(char num, int bit);
extern char clearbitchar(char num, int bit);
extern char testbitchar(char num, int bit);



#define LOCAT() ;

#define LED_LS3 5
#define LED_LS2 6
#define LED_LS1 7
#define ST 4
#define RELAY_CH0 0
#define RELAY_CH1 1
#define RELAY_CH2 2
#define RELAY_CH3 3

#define SW0 2
#define BEEP_EN 3




#define mutex_lock_sd_save 11
#define MUTEX_LOCK_IS_CAN_STEIM 12
#define MUTEX_LOCK_IS_CAN_ALG 13



//struct timeval start, end;
#define RST  RPI_GPIO_P1_12     //P1
#define RST_1()     bcm2835_gpio_write(RST,HIGH)
#define RST_0()     bcm2835_gpio_write(RST,LOW)

#define BEEP RPI_V2_GPIO_P1_37
#define BEEP_ON() 	bcm2835_gpio_write(BEEP,LOW)
#define BEEP_OFF() 	bcm2835_gpio_write(BEEP,HIGH)


#define GPS_FID 5
#define HI219_FID 6
#define LTC2943_FID 7


#define UDP_ATCMD 0
#define UDP_GPS 1
#define GPSD_GPS 2

#define APP_FILE_DATA 0//FILE_CENTER_S app_file[5];
#define STEIM2_WAVE_DATA 1//FILE_CENTER_S app_file[5];
#define APP_LOG_FILE 2//FILE_CENTER_S app_file[5];




typedef struct DATA_FRAME_GEO{//
	struct timespec date_time;//数据采集时间
	unsigned int datacount;//计数
	int ch;//通道标志
	int ad_data;//ad数据
}__attribute__ ((packed,aligned(1)))DATA_FRAME_GEO_S;



typedef struct LOOPBUF{
	struct LOOPBUF *	loopbufnext;
	struct LOOPBUF *	loopbuflast;
	int loopbufindex;
	int loopbufnum;
	int realdatasize;
	int data_offset;
	int data_valid_len;
	int is_this_buf_valid;
	void * nextdatastart;
	DATA_FRAME_GEO_S * data_start;
}__attribute__ ((packed,aligned(1)))PI_LOOPBUFFER;

typedef struct TIME_GEO{//
	struct timeval start, end;
	struct timespec first_data_time,end_date_time;
}__attribute__ ((packed,aligned(1)))TIME_GEO_S;

//LOOPBUFFER *	loopbuflist,*loopread,*loopwrite,* loopbuflisthead,*loopnow,*loopsend;
typedef struct APP_BUF{//
	#define PBUFFERNUM 10
		#define LOOPBUFLIST 0
		#define LOOPREAD 1
		#define LOOPWRITE 2
		#define LOOPBUFLISTHEAD 3
		#define LOOPNOW 4
		#define LOOPSEND 5
		#define LOOPSEND2 6
		#define LOOPSAVE 7
	PI_LOOPBUFFER * databuf[PBUFFERNUM];
	void * databuf_base;
	void * steim2_base;
	
		#define STEIM2_HEAD 0
		#define STEIM2_NOW 1
		#define STEIM2_MAKE_PAK 2
		#define LOOPBUFLISTHEAD 3
		#define LOOPNOW 4
		#define LOOPSEND 5
		#define LOOPSEND2 6
		#define LOOPSAVE 7
	IWS_STEIM2_OUT * steim2_out_buf[10];//压缩算法返回数据存入此链表
	void * status_out_base;
		#define STATUS_HEAD 0
		#define STATUS_NOW 1
		#define LOOPWRITE 2
		#define LOOPBUFLISTHEAD 3
		#define LOOPNOW 4
		#define LOOPSEND 5
		#define LOOPSEND2 6
		#define LOOPSAVE 7
	IWS_STATUS_OUT * status_out_buf[10];
	int buf_index[10];
	char unuse[512-44-44-44-40];
}__attribute__ ((packed,aligned(1)))APP_BUF_S;

typedef struct APP_SIGNAL{//
	int sig_data_send;
	int sig_data_save;
	int sig_is_data_connect;
	int sig_setad_request;
	int sig_setad_ready;
	int iswork;
	int sig_issend;
	int sig_steim2_save;
	int sig_wave_data_ready;
	int sig_can_steim;
	int sig_can_alg;
	int sig_wave_data_write_buf;//目前正在写入的数组项,仪器启动后单调增大
	int sig_wave_data_write_buf_total;//目前正在写入的数组项
	int sig_wave_data_send_buf;//计算出的需要发送的数组项
	int sig_trig_wave;
	int sig_trig_ti_write_buf_total;//触发后写指针,仪器启动后单调增大
	int sig_trig_ti_write_buf;//触发后写指针
	int sig_trig_ti_send_buf;//触发后读指针；
	int sig_trig_yj_write_buf;
	int sig_trig_yj_write_buf_total;
	int sig_trig_yj_send_buf;
}__attribute__ ((packed,aligned(1)))APP_SIGNAL_S;



typedef struct APP_CONNECT{//
	unsigned short connect_port;
	int is_connect;
	int sig_send;
	int i_connect;
	int is_request;
	PI_LOOPBUFFER * sendbuf;
}__attribute__ ((packed,aligned(1)))APP_CONNECT_S;

typedef struct APP_SERVER_MANAGE{//
	int max_server_num;
	int already_server;
	int base_port;
	APP_CONNECT_S connect[10];
}__attribute__ ((packed,aligned(1)))APP_SERVER_MANAGE_S;

typedef struct APP_SIGN{//
	int has_bcm2835_init;
	int has_wiring_init;
	int has_gps_mq_ready;
}__attribute__ ((packed,aligned(1)))APP_SIGN_S;

typedef struct APP_GLOBE_CONTROL{//
	int is_loop;
	int has_wiring_init;
}__attribute__ ((packed,aligned(1)))APP_GLOBE_CONTROL_S;

typedef struct APP_MUT{//
	pthread_mutex_t mut[CONDNUM];
	int i;
	//pthread_cond_t cond[CONDNUM];
}__attribute__ ((packed,aligned(1)))APP_MUT_S;

typedef struct APP_COND{//
	pthread_cond_t cond[CONDNUM];
	int i;
	//
}__attribute__ ((packed,aligned(1)))APP_COND_S;






typedef struct EVT_RECORD{//
	FILE * fp;
	long utc_time[2];
	int is_file_open;
	char path[200];
	//
}__attribute__ ((packed,aligned(1)))EVT_RECORD;










struct disk_infomation{
    char disk_format[8];
    char disk_total_capacity[11];
    char disk_free_capacity[11];
    char disk_permissions[3];
};



typedef struct APP{//命令行参数
	struct APP * self;
	APP_MUT_S app_mut[20];//定义后可能存在不确定问题 bus err-对齐引起的问题
	APP_COND_S app_cond[20];//定义后可能存在不确定问题
	SYNC_CTRL_S app_sync;
	TREAD_SWITCH_S thread_switch;
	unsigned char pca9539[2];
	CMD_LINE_OPT_S cmd_opt;
	APP_COUNT_S app_count;
	APP_PARA_S app_para;
	// APP_MUT_S app_mut[10];//定义后可能存在不确定问题 bus err
	// APP_COND_S app_cond[10];//定义后可能存在不确定问题
	THREAD_COTR_S app_thread[30];
	FILE_CENTER_S app_file[5];
	FID_CENTER_S app_fid[10];
	FID_UDP_S app_udp_socket[10];
	FID_TCP_S app_tcp_socket[10];
	TIME_GEO_S time;
	AD_S ad1256;
	APP_BUF_S buffer;
	APP_SIGNAL_S app_sig;
	APP_SERVER_MANAGE_S app_server;
	APP_SIGN_S app_sign;
	APP_GLOBE_CONTROL_S app_globe_control;
	IWS_PARA iws_para;
	IWS_SERVER iws_server[6];
	IWS_CSTP iws_cstp[6];
	IWS_FILE_LIST iws_file_list[INDEX_FILE_NUM];
	IWS_RQ_TIME iws_rq_time[4];
	FIR_LIST * listnow;
	FIR_LIST * listbefore30sec;
	FIR_LIST * fir_list;
	IWS_STEIM2 steim2_para;//调用压缩程序时传入的参数
	IWS_STEIM2 steim2_rq_para[4];
	IWS_STATUS_OUT iws_status;
	int is_trig;
	int is_trig_start;
	int is_trig_start_ti;
	int is_trig_over;
	int sig_trig_wave;
	char ** argv;
	int argc;
	//APP_DEVICE_S app_devices[10];//oso need about 20 devices
	int AdcNow[8];
	int buf[2][5000];
	int server_index;
	int ping_pang;
	unsigned short udp_port;
	IWS_UP_WAVEDATA_EXT iws_wave_pak_ext[IWS_UP_WAVEDATA_EXT_NUM];
	IWS_UP_WAVEDATA iws_wave_pak[1000];
	IWS_UP_TI iws_up_ti[1000];
	IWS_TRIG_PSAGET iws_trig_psaget;
	IWS_TRIG_PGAGET iws_trig_pgaget;
	struct gps_data_t *gps_data;
	struct disk_infomation di;
	double xishu_globe;//1711.92421
	EVT_RECORD evt_record;
	IWS_SERVER_SHARE iws_server_share;
	unsigned int globe_steim2_file_index;
	IWS_PAK_BUF iws_pak_buf;
	IWS_UP_SI_EXT iws_up_si_ext[1000];
	IWS_LOG_FRAME iws_log_frame;
	IWS_UP_YJ iws_up_yj[1000];
	IWS_INSTALL_INFO iws_install_info;
	char compiler_time[32];
	struct sockaddr_in clent_addr;
	IWS_CILENT_PSV iws_cilnet_psv;
}__attribute__ ((packed,aligned(1)))APP_S;
//#include <ad1256.h>
APP_S app;
extern APP_S app;
#endif