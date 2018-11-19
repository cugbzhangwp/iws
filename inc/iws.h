#ifndef IWS_H
#define IWS_H
#include <filehead.h>
#include <alg.h>
/*
typedef unsigned int s32;
typedef unsigned short s16;
typedef unsigned char u8;
//typedef struct FILE;
typedef struct beat{
int pak_type;
unsigned long pk_count;
unsigned long ip;
char str[4];
}__attribute__ ((packed,aligned(1)))beat_s,*beat_t;

*/
#define DATA_SERVER_MAIN 0
#define DATA_SERVER_OPTION 1
#define EW_SERVER_MAIN 2
#define EW_SERVER_OPTION 3
#define OP_SERVER_MAIN 4
#define OP_SERVER_OPTION 5


#define SERVER_PROTOCOL_CSTP 0
#define SERVER_PROTOCOL_ELSE 1


#define SERVER_PROTOCOL_CSTP_VERSION_10 0
#define SERVER_PROTOCOL_CSTP_VERSION_20 1




#define INDEX_FILE_NUM 10240
#define IWS_UP_WAVEDATA_EXT_NUM 10000














typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct FILE;
typedef struct  FIR_LIST;




typedef struct IWS_INSTALL_INFO{//设备注册包
	int a;
	int b;
	int c;
	int unuse[13];
}__attribute__ ((packed,aligned(1)))IWS_INSTALL_INFO;



typedef struct IWS_PARA{////
	int slt_length;
	int llt_length;
	int threshold;
	int trg_ch;
	int i_l[3];
	int firlen;//做重采样时滤波器长度
	int location;
	int first_time;
	int band_passstruct_count;
	int filter_chose;//滤波器选择
	int threshold_a[3];//***
	int trig_flag;//***
	IWS_INSTALL_INFO iws_install_info;
	//float result;
	//
}__attribute__ ((packed,aligned(1)))IWS_PARA;



typedef struct IWS_REGISTER{//设备注册包
	char type_lable[2];
	int package_number;
	char sta_id[8];
	char key[32];
	short control_authority;
	int longitude;
	int latitude;
	short altitude;
	int sensitivity;
	short init_tran_mode;
	char unuse[192];
}__attribute__ ((packed,aligned(1)))IWS_REGISTER;






typedef struct IWS_REGISTER_RE{//注册相应包
	char type_lable[2];
	int package_number;
	char sta_id[8];
	short auth_state;
	int last_package_number;
	char unuse[44];
}__attribute__ ((packed,aligned(1)))IWS_REGISTER_RE;


typedef struct IWS_TRANS_CTRL{//传输模式控制包
	char type_lable[2];
	int package_number;
	char sta_id[8];
	short ctrl_id;
	short trans_mode;
	char unuse[46];
}__attribute__ ((packed,aligned(1)))IWS_TRANS_CTRL;

typedef struct IWS_TIMEDATA_REQUEST{//时段请求包
	char type_lable[2];
	int package_number;
	char sta_id[8];
	short ctrl_id;
	unsigned int time_mark;
	int time_length;
	char unuse[40];
}__attribute__ ((packed,aligned(1)))IWS_TIMEDATA_REQUEST;



typedef struct IWS_TRIGGER_THRESHOLD_CTRL{//触发阈值设置
	char type_lable[2];
	int package_number;
	char sta_id[8];
	short ctrl_id;
	short trigger_threshold;
	char unuse[46];
}__attribute__ ((packed,aligned(1)))IWS_TRIGGER_THRESHOLD_CTRL;




typedef struct IWS_CC_RE{//控制命令应答包
	char type_lable[2];
	int package_number;
	char sta_id[8];
	short ctrl_id;
	short re_status;
	char status_desp[32];
	char unuse[206];
}__attribute__ ((packed,aligned(1)))IWS_CC_RE;


typedef struct IWS_UP_SI{//狀態信息包
	char type_lable[2];//SI
	int package_number;
	char sta_id[8];
	int ab_time;
	int time;
	int ud[10];
	int ew[10];
	int ns[10];
	char unuse[114];
}__attribute__ ((packed,aligned(1)))IWS_UP_SI;



typedef struct IWS_CILENT_PSV{//狀態信息包
	char head[8];
	int ud_pga;//UD向PGA
	int ud_pgv;//单位为mm/s,频带范围0.1-10Hz
	int ud_pgd;//
	int ew_pga;
	int ew_pgv;
	int ew_pgd;
	int ns_pga;
	int ns_pgv;
	int ns_pgd;
}__attribute__ ((packed,aligned(1)))IWS_CILENT_PSV;














typedef struct IWS_EXT_HEAD{//狀態信息包
	unsigned long utc_time[2];
	int package_number;
}__attribute__ ((packed,aligned(1)))IWS_EXT_HEAD;

typedef struct IWS_UP_SI_EXT{//狀態信息包
	IWS_EXT_HEAD iws_ext_head;
	char type_lable[2];//SI
	int package_number;
	char sta_id[8];
	int ab_time;
	int time;
	int ud[10];
	int ew[10];
	int ns[10];
	char unuse[114];
}__attribute__ ((packed,aligned(1)))IWS_UP_SI_EXT;




typedef struct IWS_PAK_BUF_INDEX{//狀態信息包
	unsigned int now;
	unsigned int number;
}__attribute__ ((packed,aligned(1)))IWS_PAK_BUF_INDEX;

typedef struct IWS_PAK_BUF{//狀態信息包
	unsigned int globe_pk_num;
	IWS_PAK_BUF_INDEX si_buf_index;
	IWS_PAK_BUF_INDEX ti_buf_index;
	IWS_PAK_BUF_INDEX wave_buf_index;
	IWS_PAK_BUF_INDEX trig_wave_buf_index;
}__attribute__ ((packed,aligned(1)))IWS_PAK_BUF;






typedef struct IWS_UP_TI{//触发信息包
	char type_lable[2];//SI
	int package_number;
	int imt_s;//Initial movement time
	short imt_ms;//Initial movement time
	short rt_s;//Relative time
	char sta_id[8];
	int sta_lta;//STA/LTA
	short imd;//Initial movement derect
	int ud_pga;//UD向PGA
/*
int
4B
单位为mm/s/s,频带范围0.1-10Hz
*/

	int ud_pgv;//单位为mm/s,频带范围0.1-10Hz
	int ud_pgd;//
	int ew_pga;
	int ew_pgv;
	int ew_pgd;
	int ns_pga;
	int ns_pgv;
	int ns_pgd;
	short cdv;//Column degree value
	int ud_psa03;
	int ud_psa10;
	int ud_psa30;
	int ew_psa03;
	int ew_psa10;
	int ew_psa30;
	int ns_psa03;
	int ns_psa10;
	int ns_psa30;
	int ol_data;//TODO

	int final_PGA;
	int final_PGV;
	int final_PGD;

	int ud_psv03;
	int ud_psv10;
	int ud_psv30;
	int ew_psv03;
	int ew_psv10;
	int ew_psv30;
	int ns_psv03;
	int ns_psv10;
	int ns_psv30;



	char unuse[102];
}__attribute__ ((packed,aligned(1)))IWS_UP_TI;













typedef struct IWS_UP_YJ{//触发信息包
	char type_lable[2];//SI
	int package_number;
	int imt_s;//Initial movement time
	short imt_ms;//Initial movement time
	short rt_s;//Relative time
	char sta_id[8];
	int sta_lta;//STA/LTA
	short imd;//Initial movement derect
	int ud_pga;//UD向PGA
/*
int
4B
单位为mm/s/s,频带范围0.1-10Hz
*/
	int ud_pgv;//单位为mm/s,频带范围0.1-10Hz
	int ud_pgd;//
	int ew_pga;
	int ew_pgv;
	int ew_pgd;
	int ns_pga;
	int ns_pgv;
	int ns_pgd;
	short cdv;//Column degree value
	int ud_psa03;
	int ud_psa10;
	int ud_psa30;
	int ew_psa03;
	int ew_psa10;
	int ew_psa30;
	int ns_psa03;
	int ns_psa10;
	int ns_psa30;
	int ol_data;//TODO
	char unuse[150];
}__attribute__ ((packed,aligned(1)))IWS_UP_YJ;















typedef struct iws_utc_10{
	short year;
	short yday;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	unsigned char unuse;
	short u_sec;

}__attribute__ ((packed,aligned(1)))iws_utc_10;


typedef struct IWS_UP_WAVEDATA{//触发信息包
	char wcts[2];//wc ,wt,ws
	int package_number;
	char qi;//quality index.D
	char reserve;
	char sta_idx[5];
	char pos_idx[2];
	char ch_idx[3];
	char net_idx[2];
	//char data_start_utc[10];
	iws_utc_10 data_start_utc;
	unsigned short datacount;
	short fs_factor;
	short srm;//Sampling rate multiplier
	char as;//Activity sign
	char iotm;//Input-output time mark
	char dqm;//Data quality mark
	char nbs;//Number of back subblocks
	int tcv;//time correct value
	short dso;//data start offset
	short fdbo;//first data block offset
	short sbt1000;//subblock type
	short nbbn;//next block byte num
	char cf;//coding format;
	char order;//Word order
	char drl;//data record len
	char reserve1;
	short sbt1002;//subblock type
	char dimension;//dimension量纲
	char reserve2;
	int sf;//Sensitivity factor
	char data[192];
}__attribute__ ((packed,aligned(1)))IWS_UP_WAVEDATA;








typedef struct IWS_UP_WAVEDATA_EXT{//触发信息包
	IWS_EXT_HEAD iws_ext_head;
	char wcts[2];//wc ,wt,ws
	int package_number;
	char qi;//quality index.D
	char reserve;
	char sta_idx[5];
	char pos_idx[2];
	char ch_idx[3];
	char net_idx[2];
	//char data_start_utc[10];
	iws_utc_10 data_start_utc;
	unsigned short datacount;
	short fs_factor;
	short srm;//Sampling rate multiplier
	char as;//Activity sign
	char iotm;//Input-output time mark
	char dqm;//Data quality mark
	char nbs;//Number of back subblocks
	int tcv;//time correct value
	short dso;//data start offset
	short fdbo;//first data block offset
	short sbt1000;//subblock type
	short nbbn;//next block byte num
	char cf;//coding format;
	char order;//Word order
	char drl;//data record len
	char reserve1;
	short sbt1002;//subblock type
	char dimension;//dimension量纲
	char reserve2;
	int sf;//Sensitivity factor
	char data[192];
}__attribute__ ((packed,aligned(1)))IWS_UP_WAVEDATA_EXT;











typedef struct IWS_CSTP{//狀態信息包
	IWS_REGISTER iws_reg_pak;//注册信息包
	IWS_REGISTER_RE iws_reg_re_pak;//注册信息相应包
	IWS_TRANS_CTRL iws_control_pak;//传输模式控制包
	//IWS_TRANS_CTRL iws_control_pak;//传输模式控制包
	IWS_TIMEDATA_REQUEST iws_timedata_request_pak;//时段请求数据包
	IWS_TRIGGER_THRESHOLD_CTRL iws_trigger_threshold_pak;//触发阈值设置包
	IWS_CC_RE iws_cc_re_pak;//控制命令应答包
	IWS_UP_SI iws_up_si_pak;//状态信息包
	IWS_UP_TI iws_up_ti;//触发信息包
	IWS_UP_WAVEDATA iws_up_wavedata;
	int pak_num;//host
	int server_pak_num;//server
	int ctrl_id;
	int cmd;
}__attribute__ ((packed,aligned(1)))IWS_CSTP;



typedef struct IWS_SERVER_STATUS{//
	int isconnect;//是否已经连接
	int iscan_control;//是否可控制
	int reg_status;
	int is_on;//是否打开
	unsigned char unuse[64-16];
}__attribute__ ((packed,aligned(1)))IWS_SERVER_STATUS;



typedef struct IWS_STATUS_OUT{//
	struct IWS_STATUS_OUT * next;
	struct IWS_STATUS_OUT * prev;
	int index;
	long utc_time[2];            //* 输出的utc时间 每个通道第一个为秒 第二个为纳秒
	int starttime;
	// int ud_max;
	// int ud_min;
	// int ew_max;
	// int ew_min;
	// int ns_max;
	// int ns_min;
	float max[3];
	float min[3];
	int ud[10];
	int ew[10];
	int ns[10];                 //* 输出的值
}__attribute__ ((packed,aligned(1)))IWS_STATUS_OUT;



typedef struct IWS_SERVER_SHARE_STATUS{//
	int is_clock_sync_ntp;
	int is_clock_sync_gps;
}__attribute__ ((packed,aligned(1)))IWS_SERVER_SHARE_STATUS;

typedef struct IWS_SERVER_SHARE{//
	IWS_SERVER_SHARE_STATUS status;
	char unuse[512-sizeof(IWS_SERVER_SHARE_STATUS)];
}__attribute__ ((packed,aligned(1)))IWS_SERVER_SHARE;






















typedef struct IWS_STEIM2_OUT{//
	struct IWS_STEIM2_OUT * next;
	struct IWS_STEIM2_OUT * prev;
	int index;
	long utc_time[2];            //* 输出的utc时间 每个通道第一个为秒 第二个为纳秒
	int sample_Num;      // *输出的采样点
	int ch_num;
	int out[48];                 //* 输出的值
}__attribute__ ((packed,aligned(1)))IWS_STEIM2_OUT;













typedef struct IWS_RQ_TIME{//服务器信息
	int isused;
	unsigned int timestart;
	unsigned int timeend;
	unsigned int len;
	int server_index;
	int file_index;
	void * para;
	int t_t;
	struct timespec date_time;//数据采集时间
}__attribute__ ((packed,aligned(1)))IWS_RQ_TIME;



typedef struct IWS_SERVER{//服务器信息
	int index;
	//int mode;
	char ipv4_addr[24];//
	char ipv6_addr[64];//
	char sid[16];
	char server_key[36];
	char des[24];//描述
	unsigned short port;
	IWS_SERVER_STATUS status;
	int fid;
	int mode;//仪器当前传输模式
	int last_mode;//仪器上一个传输模式
	IWS_STEIM2_OUT * steim2_buf;//服务器目前的发送指针
	IWS_STATUS_OUT * status_send_buf;//服务器目前发送状态信息包指针
	FIR_LIST * fir_list_trig_start;//触发后服务器数据开始指针
	FIR_LIST * fir_list_async_trig_start;//触发后服务器数据开始指针
	int sig;//数据就绪标志
	int sig_status;//状态信息信号
	int sig_trig;//触发信息信号
	int sig_trig_wave;//触发后发送数据索引指针
	IWS_STEIM2 steim2_para_init_tirg;//触发模式下调用压缩程序时传入的参数和输出结果
	int sig_trig_ti;
	int sig_trig_ti1;
	int sig_trig_wave_data_send_buf;
	int is_trig_start;
	int is_first_send;
	IWS_PARA iws_para;
	int protocol;
	int version;
	IWS_PAK_BUF iws_pak_buf;
	IWS_RQ_TIME iws_rq_time;
	int sig_is_rq_data_ready;
	int trig_starttime;
	int trig_endtime;
	int is_trig_data_ready;
	int is_trig_init;
	char ch_label[3][4];
	int is_trig_ti_init;
	int sig_trig_yj;
	//unsigned char unuse[2048-132-118-92-sizeof(IWS_RQ_TIME)-sizeof(IWS_PAK_BUF)-sizeof(IWS_STEIM2)-sizeof(IWS_PARA)];//62
	unsigned char unuse[58];
}__attribute__ ((packed,aligned(1)))IWS_SERVER;






typedef struct IWS_FILE_LIST{//设备注册包
	int isused;
	unsigned int tv_sec;
	unsigned int tv_nsec;
	unsigned int pak_num;
	unsigned int file_index;
	char name[256];
}__attribute__ ((packed,aligned(1)))IWS_FILE_LIST;

typedef struct IWS_LOG{//设备注册包
	int is_log_ready;
	char logstr[512];
	int len;
}__attribute__ ((packed,aligned(1)))IWS_LOG;

typedef struct IWS_LOG_FRAME{//设备注册包
	FILE * fp;
	char file[64];
	char path[256];
	int file_index;
	int is_file_open;
	int count;
	IWS_LOG iws_log[100];
}__attribute__ ((packed,aligned(1)))IWS_LOG_FRAME;

#endif
