#ifndef _ALGV20_H
#define _ALGV20_H
#include <alg.h>
int resample11_11(FIR_LIST1 *i_fir_list,int* location,FIR_LIST *i_result_list,int *first_time,int vector_type);
int band_passstruct11_11(FIR_LIST *i_fir_list,int band_pass_flag,int num,float *mean);     //   这里需要再加一个参数标记事件后的数据点数，用以去除直流偏置从事件的前十秒开始？或者是3秒   第一次触发要调用一次用来计算3秒的值
int calculate_mean(FIR_LIST* fir_list,float *mean);
int PGA_V_G_11_11(FIR_LIST *i_fir_list,BAND_PASS_RESULT *band_pass_data,int *count,int timespan,int *trig,int PGV_type,int band_pass_flag);

#endif