#ifndef ALG_H
#define ALG_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/vfs.h>
//#include <iws.h>
// typedef struct sFIR_LIST{//设备注册包
// 	struct sFIR_LIST * next;//下个时刻的指针
// 	struct sFIR_LIST * last;//上个时刻的指针
// 	struct sFIR_LIST * triged;//上个时刻的指针
// 	//struct gong *;//上个时刻的指针
// 	int count; //位置标记
// 	long utc_second; //utc时间秒
// 	long utc_nanosecond; //utc时间纳秒
// 	float data[5]; //原始数据
// 	float band_pass_data[5];//做完带通的结果
// 	float data_a[5];//做完带通的结果
// 	float data_v[5];//做完带通的结果
// 	float data_d[5];//做完带通的结果
// 	int sample_rata;   //采样率 50,100,200  
// }__attribute__ ((packed,aligned(1)))FIR_LIST;



// typedef struct FIR_LIST1{//设备注册包
// 	 struct FIR_LIST1 *next;//下个时刻的指针
// 	 int count; //位置标记
// 	 long utc_second; //utc时间秒
// 	 long utc_nanosecond; //utc时间纳秒
// 	 float data[5]; //原始数据
// 	 int sample_rata;   //采样率 50,100,200  
// 	 struct FIR_LIST1 *last;//上个时刻的指针
// }__attribute__ ((packed,aligned(1)))FIR_LIST1;

typedef struct BAND_PASS_RESULT{//设备注册包
	//struct FIR_LIST *next;//下个时刻的指针
	//int count; //位置标记
	//long utc_second; //utc时间秒
	//long utc_nanosecond; //utc时间纳秒

	//long utc_second_instant_PGA; //utc时间秒              最终PGA
	//long utc_nanosecond_instant_PGA; //utc时间纳秒        最终PGA
	//long utc_second_instant_PGV; //utc时间秒              最终PGV
	//long utc_nanosecond_instant_PGV; //utc时间纳秒        最终PGV
	//long utc_second_instant_PGD; //utc时间秒              最终PGD
	//long utc_nanosecond_instant_PGD; //utc时间纳秒        最终PGD

	long utc_second_final_PGA; //utc时间秒                最终PGA
	long utc_nanosecond_final_PGA; //utc时间纳秒          最终PGA
	long utc_second_final_PGV; //utc时间秒                最终PGV
	long utc_nanosecond_final_PGV; //utc时间纳秒          最终PGV
	long utc_second_final_PGD; //utc时间秒                最终PGD
	long utc_nanosecond_final_PGD; //utc时间纳秒          最终PGD


	//float data[5]; //原始数据
	//float dataV[5];//速度数据
	//float dataD[5];//位移数据


	//float band_pass_data1[5];//中间带通数据
	//float band_pass_dataD1[5];//中间带通数据
	//float band_pass_dataV1[5];//中间带通数据

	//float band_pass_dataV[5];//带通后速度数据
	//float band_pass_dataD[5];//带通后位移数据
	//float band_pass_data[5];//带通后加速度数据


	//float instant_PGA;//瞬时PGA
	//float instant_PGV;//瞬时PGV
	//float instant_PGD;//瞬时PGD

	//float final_PGA;//最终的PGA
	//float final_PGV;//最终的PGV
	//float final_PGD;//最终的PGD


	float final_PGA;//最终的PGA
	float final_PGV;//最终的PGV
	float final_PGD;//最终的PGD


	float final_PGA_component[3];//最终的PGA
	float final_PGV_component[3];//最终的PGV
	float final_PGD_component[3];//最终的PGD
	//int sample_rata;   //采样率 50,100,200  
	//struct FIR_LIST *last;//上个时刻的指针
	////};
	}__attribute__ ((packed,aligned(1)))BAND_PASS_RESULT;

 typedef struct FIR_LIST{//设备注册包
	 struct FIR_LIST *next;//下个时刻的指针
	 int count; //位置标记

	 long utc_second; //utc时间秒
	 long utc_nanosecond; //utc时间纳秒

	 long utc_second_instant_PGA; //utc时间秒              瞬时PGA
	 long utc_nanosecond_instant_PGA; //utc时间纳秒        瞬时PGA
	 long utc_second_instant_PGV; //utc时间秒              瞬时PGV
	 long utc_nanosecond_instant_PGV; //utc时间纳秒        瞬时PGV
	 long utc_second_instant_PGD; //utc时间秒              瞬时PGD
	 long utc_nanosecond_instant_PGD; //utc时间纳秒        瞬时PGD

	 //long utc_second_final_PGA; //utc时间秒                最终PGA
	 //long utc_nanosecond_final_PGA; //utc时间纳秒          最终PGA
	 //long utc_second_final_PGV; //utc时间秒                最终PGV
	 //long utc_nanosecond_final_PGV; //utc时间纳秒          最终PGV
	 //long utc_second_final_PGD; //utc时间秒                最终PGD
	 //long utc_nanosecond_final_PGD; //utc时间纳秒          最终PGD


	 float data[5]; //原始数据
	 float dataV[5];//速度数据
	 float dataD[5];//位移数据
	 //float band_pass_data1[5];//中间带通数据
	 //float band_pass_dataD1[5];//中间带通数据
	 //float band_pass_dataV1[5];//中间带通数据
	 float band_pass_dataV[5];//带通后速度数据
	 float band_pass_dataD[5];//带通后位移数据
	 float band_pass_data[5];//带通后加速度数据
	 float instant_PGA;//瞬时PGA
	 float instant_PGV;//瞬时PGV
	 float instant_PGD;//瞬时PGD
	 //float final_PGA;//最终的PGA
	 //float final_PGV;//最终的PGV
	 //float final_PGD;//最终的PGD
	 int sample_rata;   //采样率 50,100,200  
	 struct FIR_LIST *last;//上个时刻的指针
}__attribute__ ((packed,aligned(1)))FIR_LIST;















 typedef struct FIR_LIST2{//设备注册包
	 struct FIR_LIST *next;//下个时刻的指针
	 int count; //位置标记
	 long utc_second; //utc时间秒
	 long utc_nanosecond; //utc时间纳秒
	 float data[5]; //原始数据
	 float band_pass_data[5];//做完带通的结果
	 float dataV[5];//速度
	 float dataD[5];//位移
	 int sample_rata;   //采样率 50,100,200  
	 struct FIR_LIST *last;//上个时刻的指针
}__attribute__ ((packed,aligned(1)))FIR_LIST2;

 typedef struct FIR_LIST1{//设备注册包
	 struct FIR_LIST1 *next;//下个时刻的指针
	 int count; //位置标记
	 long utc_second; //utc时间秒
	 long utc_nanosecond; //utc时间纳秒
	 float data[5]; //原始数据
	 int sample_rata;   //采样率 50,100,200  
	 struct FIR_LIST1 *last;//上个时刻的指针
}__attribute__ ((packed,aligned(1)))FIR_LIST1;


#define IWS_CH_NUM 3
typedef struct IWS_STEIM2{//
	struct IWS_STEIM2 * next;
	struct IWS_STEIM2 * prev;
	// long utc_time[IWS_CH_NUM][2];            // 输出的utc时间 每个通道第一个为秒 第二个为纳秒
	// FIR_LIST *tishen;               // 中间的变量
	// int sample_Num[IWS_CH_NUM]={0,0,0};      // 输出的采样点
	// int flag_30_32=5;
	// int count[IWS_CH_NUM]={0,0,0};           // 中间系数
	// int mincount[IWS_CH_NUM]={10,10,10};     // 中间系数
	// int datad[IWS_CH_NUM][10];               // 查分数组
	// int semidata[IWS_CH_NUM][48];            // 中间数据
	// int semicount[IWS_CH_NUM]={0,0,0};       // W的个数
	// int out[IWS_CH_NUM][48];                 // 输出的值
	long utc_time[IWS_CH_NUM][2];            //* 输出的utc时间 每个通道第一个为秒 第二个为纳秒
	FIR_LIST *tishen;               // 中间的变量
	int sample_Num[IWS_CH_NUM];      // *输出的采样点
	int sample_Num_tmp[IWS_CH_NUM];      // *输出的采样点
	int flag_30_32;
	int count[IWS_CH_NUM];           // 中间系数
	int mincount[IWS_CH_NUM];     // 中间系数
	int datad[IWS_CH_NUM][10];               // 查分数组
	int semidata[IWS_CH_NUM][48];            // 中间数据
	int semicount[IWS_CH_NUM];       // W的个数
	int out[IWS_CH_NUM][48];                 //* 输出的值
	int ch_num;//*
	double pi;
	int steim2out_ret;
	float max[3];
	float min[3];
	float ffvalue[3];
	float fisrt_value_flag[3];
	int change_c;
}__attribute__ ((packed,aligned(1)))IWS_STEIM2;









#define CH_NUM_PSA 3
#define DIRECT_NUM_PSA 3
typedef struct IWS_TRIG_PSAGET{//
	double dx[CH_NUM_PSA][DIRECT_NUM_PSA];//***0
	double x2[CH_NUM_PSA][DIRECT_NUM_PSA];//***0
	double y0[CH_NUM_PSA][DIRECT_NUM_PSA];//***0
	double y1[CH_NUM_PSA][DIRECT_NUM_PSA];//***0
	int first;//***1
	float zuni;//*** 0.5
	int sample_rata;
	float result[CH_NUM_PSA][DIRECT_NUM_PSA];//***0
	float period[3];//{0.3,1,3}
	long utc_time_imt[2];
	long trig_start[2];
}__attribute__ ((packed,aligned(4)))IWS_TRIG_PSAGET;


typedef struct IWS_TRIG_PGAGET{//
	int final_liedu_pga;
	int final_liedu_pgv;
	int PGV_type;//2
	BAND_PASS_RESULT band_pass_data;
	float multiple_PGA;//=3.73;
	float increment_PGA;//=-1.23;
	float multiple_PGV;//=3.61;
	float increment_PGV;//=2.72;
	int trig;//=1;
	int countt;

}__attribute__ ((packed,aligned(4)))IWS_TRIG_PGAGET;






















#define IWS_CH_NUM 3
typedef struct IWS_STATUS{//
	int timestart;
	int timelen;
	int ud[10];
	int ew[10];
	int ns[10];
}__attribute__ ((packed,aligned(1)))IWS_STATUS;










//计算烈度的函数
//ipga通过瞬时最大加速度计算烈度的函数
//ipgv通过瞬时最大速度计算烈度值的函数





//  typedef struct FIR_LIST{//设备注册包
// 	struct FIR_LIST *next;//下个时刻的指针
// 	int count; //位置标记
// 	float data;
// 	float band_pass_data;//做完带通的结果
// 	int sample_rata;   //采样率 50,100,200  
// 	struct FIR_LIST *last;//上个时刻的指针
// }__attribute__ ((packed,aligned(1)))FIR_LIST;

//计算单自由度拟加速度反应
int psa(float *data,float * result,int num,float period ,int sample_rata);
//这个速度慢
//data 各分量加速度测量数组 
//num 测量数据长度
//result 结果
//sample_rata 采样率
//period 需要计算的周期


int psa_2(float *data,int data_num,float sample_rata,float zuni,float *period,int period_num,float *result);
//计算PSA  使用psa2 速度更快
//data 各分量加速度测量数组 
//data_num 测量数据长度
//sample_rata 采样率
//zuni 阻尼
//period 需要计算的周期数组 因为 0.3 1 3（秒）
//result 对应的结果数组






//typedef struct FIR_LIST{//设备注册包
//	struct FIR_LIST *next;//下个时刻的指针
//	int count; //位置标记
//	float data;
//	int sample_rata;   //采样率 50,100,200  
//	struct FIR_LIST *last;//上个时刻的指针
//	}__attribute__ ((packed,aligned(1)))FIR_LIST;












int band_passstruct_20180715(FIR_LIST *i_fir_list);
int band_passstruct(FIR_LIST *i_fir_list,int PGV_type,int *count,int timespan);
extern int band_passstruct_new(FIR_LIST *i_fir_list);
extern int band_passstruct2(FIR_LIST *i_fir_list,int band_pass_flag,int x_y_z);
extern int PGA_V_G(FIR_LIST *i_fir_list,BAND_PASS_RESULT *band_pass_data,int *count,int timespan,int *trig,int PGV_type);
extern int PGA_V_G2(FIR_LIST *i_fir_list,BAND_PASS_RESULT *band_pass_data,int *count,int timespan,int *trig,int PGV_type,int band_pass_flag);
//返回2代表可以取PGA PGV PGD 等值
//PGV_type 1：合并后再带通  2：合并前带通
//*count ：用于控制分段计算pgv，pgv等参数 初始化为1
//timespan : 测算PGA，PGV，PGD的时间间隔（秒）


//dataIn 数据的起始地址
//style 重抽样类型 ：style=81 50采样率带通 ，style=191 200采样率带通 ，style=301 200采样率带通
//dataNum 数据的个数
//dataout 重抽样后的数据



//返回1 代表成功
//返回0 代表style填的不对
//返回-1 代表数据长度不够

int resample(int *dataIn,int style,int dataNum,int * dataOut);



//dataIn 数据的起始地址
//style 重抽样类型 ：style=81 250到50 ，style=61 250到100 ，style=41 250到200
//dataNum 数据的个数
//dataout 重抽样后的数据
//返回1 代表成功
//返回0 代表style填的不对
//返回-1 代表数据长度不够


int slt_lltstru(FIR_LIST *data,int slt_length,int llt_length,float *result,float threshold,int channel);
int slt_lltstru_2(FIR_LIST *data,int slt_length,int llt_length,float *result,float threshold,int channel);
//计算slt/llt 触发函数
//如果返回2代表满足触发条件
//slt_length :短时窗的长度
//llt_length :  长时窗的长度
//threshold : 触发阈值
//result : 返回的slt/llt的值
//data: 输入的结构体指针
//channel: 计算烈度所用的通道
int psa_struct_2(FIR_LIST *i_list,float sample_rata,float zuni,float *period,float *result,double *dx,double *x2,double *y0,double *y1,int *first);
int resample2(FIR_LIST1 *i_fir_list,int* location,FIR_LIST *i_result_list,int* first_time,int vector_type);
//第一次调用的时候将location 和first_time 所指的值赋为1
//以后每次只要调用就行
//返回2代表抽取了一个新数要填到后面的链表里
//vector_type 2 代表第5通道由xy矢量合成        3 代表有三个方向一起合成
void resample2_test_main();
int writesteim2(int count,int *data,int flag_32_30,int *semidata,int *sample_Num);                 //count 要填充的个数
int writeCk(int *semidata,int semidata_num,int count,int flag_32_30);
int steim2(FIR_LIST *i_fir_list,int *count,int *mincount,int *data ,int *semidata,int *semi_count,int *sample_Num,int *flag,int channel_num);
int steim2out(FIR_LIST *i_fir_list,int *count,int *mincount,int *data ,int *semidata,int *semi_count,int *out,int *sample_num,int channel_num,int *flag);
int iws_do_steim_callback(FIR_LIST *list,IWS_STEIM2 * steim2_init_para);
extern int iws_do_steim_pre(FIR_LIST *list,IWS_STEIM2 * steim2_init_para);
extern int psa_struct_2_main();
extern int iws_do_trig_pre(BAND_PASS_RESULT *band_pass_data,float * result,int final_liedu_pga,long * utc_time_imt,float sta_lta,int rela_t);
extern int get_imt(FIR_LIST *list,long * imt,long * trig_start);
extern int first_direction(FIR_LIST *i_fir_list,float threshold,int fir_length);
extern float New_Intensity_calculation(float pga,float pgv,int style);
#endif