#ifndef _CSTP_H
#define _CSTP_H
//#include <iws.h>
#include <filehead.h>
#include <iws.h>
extern int steim2_mk_wave_pak(IWS_UP_WAVEDATA * iws_up_wavedata,IWS_STEIM2_OUT * steim2_buf,int pak_num);
extern int init_iws_wave_pak(IWS_UP_WAVEDATA * iws_up_wavedata,int server_index);//注册信息响应包
extern int update_point(int mode,int server_index);//改变模式时重新配置发送指针
extern int init_cstp(IWS_CSTP *iws_cstp_globe,int server_index);
#endif