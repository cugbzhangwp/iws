#ifndef BAND_PASSSTRUCT_H
#define BAND_PASSSTRUCT_H
#include "fir_list.h"



int band_passstruct(FIR_LIST *i_fir_list,double &dataOut);

//dataIn 数据的起始地址
//style 重抽样类型 ：style=81 50采样率带通 ，style=191 200采样率带通 ，style=301 200采样率带通
//dataNum 数据的个数
//dataout 重抽样后的数据



//返回1 代表成功
//返回0 代表style填的不对
//返回-1 代表数据长度不够

#endif