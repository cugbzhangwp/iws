#ifndef _OSO_PROTOCOL_H
#define _OSO_PROTOCOL_H
#include <sys/sysinfo.h>
typedef struct OSO_CMD{
int pak_type;
int cmd;
int t[10];
}__attribute__ ((packed,aligned(1)))OSO_CMD;




typedef struct BEAT_PAK{
int pak_type;
unsigned long pk_count;
unsigned long ip;
char str[4];
}__attribute__ ((packed,aligned(1)))BEAT_PAK;


struct CH_PARA{

unsigned short para_ad_1282;
unsigned short para_ad_hpf;

};



typedef struct OSO_PARA{
int pak_type;
int para_isautostart;
unsigned long para_ad;// 5373 use
unsigned short para_ch_spi;//ch mask
unsigned short para_ad_spi;//mode fs etc...
struct CH_PARA ch_para[10];//gain &hpf filter
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

}__attribute__ ((packed,aligned(1)))OSO_PARA;


typedef struct COUNTER{
	int count_one_read;
	unsigned long long file_write_count;//8 byte
	unsigned long long file_write_count_all;
	unsigned long long data_read_count;
	unsigned long long data_read_count_all;
	unsigned long long data_send_count;
	unsigned long long data_send_count_all;
}__attribute__ ((packed,aligned(1)))COUNTER;





typedef struct SYS_INFO{
int pak_type;
short run_status;
unsigned long  long free_sd;//sd card free space
unsigned long  long free_mem;//free memery //but now i just use it as total size of sdCard
COUNTER count;//52
int bat;//energy
struct sysinfo sys_info;
char gps_info[400];
}__attribute__ ((packed,aligned(1)))SYS_INFO; 




typedef struct ABL_TX_PARA{ // 阿波罗发射配置 
int pak_type;
unsigned char order; // 阶数 
 unsigned char freq; // 频率 
unsigned short m_code_len; // m序列长度（供截断）
 int m_init_phz; // m序列初相 
int m_polynomial; // m序列多项式
 unsigned short cycle; // 周期
 int m_sync_Sec; //m序列同步秒 
short m_sync_mSec; // m序列同步毫秒
 short m_sync_uSec; // m序列同步微秒
 unsigned short m_sync_Flag; // m序列同步标记
 unsigned short pwm_freq; // m序列同步标记
 unsigned short pwm_ratio; // m序列同步标记
 unsigned short pwm_mode; // m序列同步标记
 unsigned short r_mode; // 
}__attribute__ ((packed,aligned(1)))ABL_TX_PARA;





typedef struct PROG_STATIC_INFO_BLOCK{ // 

char compile_time[20]; //程序编译时间
}__attribute__ ((packed,aligned(1)))PROG_STATIC_INFO_BLOCK;



typedef struct PROG_RUN_INFO_BLOCK{ // 

unsigned int gpssecond;//gps有效后经历的秒

}__attribute__ ((packed,aligned(1)))PROG_RUN_INFO_BLOCK;




typedef struct PROG_COTR_BLOCK{ // 

unsigned int  time_cell;//控制固定任务时间粒度
unsigned int  iscacucurrent;//控制是否计算电流新值--暂不用(预留)

}__attribute__ ((packed,aligned(1)))PROG_COTR_BLOCK;





typedef struct NEW_PROTOCOL_BLOCK{ // 
int pak_type;//
int cmd;//
int nextblocksize;//
int t[9];
}__attribute__ ((packed,aligned(1)))NEW_PROTOCOL_BLOCK;














#endif