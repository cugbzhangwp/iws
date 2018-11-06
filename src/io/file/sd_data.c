#include <sd_data.h>
#include <app_main.h>
#include <time.h>    // for gettimeofday()
#include <sys/time.h>    // for gettimeofday()
#include <printcolor.h>
#include <iws.h>
/** 
 * @file statfs.c
 * @brief get disk infomation
 * @author T-bagwell@ChinaUnix.net
 * @date 2009-12-12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/vfs.h>
#include <pca9539_relay.h>
#include <ltc2943.h>
#include <app_main.h>
#include <cstp.h>
char sendbuf_tmp[256];
extern APP_S app;


    iws_head myiws_head={//文件头结构初始化(部分固定内容)
                            .Flag=OTS_FLAG,
                            .iOTSVer=0x01,   // OTS 文件格式版本。当前为 0x1，如读取程序发现版本高于自己所支持的，应给与提示
                            //.Proj="default",// 工程名，可选留空
                            .iFileSize=0, // 全部文件大小，单位 Byte；
                            .iHeadSize=0, // 文件头大小，单位 Byte ；
                            .iDevID=OTS_DEV_OSO_N,  // 设备类型ID； OSO-N 为 OTS_DEV_OSO_N ;将来可扩展其他设备类型
                            .DevSN="abl001", // 设备序列号；字符串
                            .iFs=0,      // 采样率
                            .nChan=0,     // 通道个数
                            .iChOrder=0,
                            .iChanMask=0, // 通道掩码。比如开启了1,2,3,5通道，则为 0x00000017
                            .fTime=0,    // 总采样时间长度，单位 mSec         -
/*

                            .iGain=0,     // 增益倍数，如无增益，设置为 0x1      -
                            .iADShift=0, // AD 偏置，如无偏置，设置为 0            -
                            .iADMax=0,   // AD 最大值，即正满幅AD值，比如 24 位AD为 2^23      -
                            .iADMin=0,   // AD 最小值，即负满幅AD值      -
                            .iAD_para=0,  // AD设置，复制 para_AD（可选）        -
                            //.iGian_ch,
                            //.ch_paras.
                            .iAD_unused=0,// AD设置扩展，未使用     -
                            .iADC=0x7fffffff,     // AD转换系数，OSO-N 为 0x5D1420;       -
                            .iADCUnit=0,  // 转换单位，OSO-N 为 OTS_ADCUNIT_Volt      -
                            .iRefVolot=2.5, // 参考电压（可选）     -*/
                            .iDataType=0, // 数据类型，参考宏定义;OSO-N 为 OTS_DATATYPE_Mixed
                            .fDelay=0,    // 数据延迟，单位 mSec
                            .iTimeStamp=0,// 第一个采样的 Unix 时间戳
                            .nFileOrder=0,
                            //.unuse[3];
                            .nNextHeadSize=0,    // 保留扩展    
                            };
int writedata(char * buffer_write,int needwritebyte,FILE * fp)
{

	int realwrite,nwrite,writetimes,ret;
	//unsigned int * pdata;
	//unsigned int data;
	//pdata=(int * )buffer_write+8;
	//data=*pdata&0xff;
	//led_blink_ctrl=!led_blink_ctrl;
	//ret=led_blink1();
	realwrite=nwrite=writetimes=0;
	do{//写文件部分
	//ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);
	nwrite=fwrite(buffer_write+realwrite, 1,needwritebyte-realwrite,fp);
	writetimes++;
	realwrite+=nwrite;
	//ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
	}while(realwrite!=needwritebyte);
	return realwrite;
}


#define FILE_NEWFILE 0
#define FILE_OK 1
#define FILE_CLOSED 2
#define FILE_ERR 3
#define FILE_DISCARD 4
#define FILE_RESET 5
#define FILE_SAVE 0 //control

int close_data_file(APP_S * app,int fid)
{
	app->app_file[fid].status=FILE_NEWFILE;
	app->app_file[fid].write_count=0;
	app->app_file[fid].fid=fclose(app->app_file[fid].fid);
}

#define ISADOK 2





int get_time_str(long tv,char * strTime)
{
 
        //char strTime[32];
        struct tm *ptm = gmtime(&tv);  //将秒转换成struct tm的形式
        strftime(strTime, 32, "%F-%T", ptm);
        printf("%s ", strTime); //输出精确到秒
        return 0;
}



int save_file(APP_S * app)
{
	char file_url[200];
	snprintf(app->app_file[APP_FILE_DATA].path,200,"%s","/home/pi/userdata/data");
	snprintf(app->app_file[APP_FILE_DATA].suffix,200,"%s","iws");
    while(app->thread_switch.main_loop)
    {
        if(app->app_sig.sig_setad_request){
            app->app_sig.sig_setad_request=0;//
            app->app_sig.sig_setad_ready=1;
            //app.app_sig.sig_data_save=0;
        }
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
        while((app->app_sig.sig_data_save<=0)||(app->app_sig.sig_setad_ready!=ISADOK))
        {
            usleep(250*1000);//250ms
           // printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
                //
                //wait_go_signal(1);
        }
        //printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
        if(app->app_file[APP_FILE_DATA].control==FILE_SAVE){
        	//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
        	if(app->app_file[APP_FILE_DATA].status==FILE_NEWFILE){
        		//snprintf(app->app_file[APP_FILE_DATA].filename,100,"%d.%d",app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_sec,app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_nsec);
				//writedata(app->buffer.databuf[LOOPSAVE]->data_start,200*sizeof(DATA_FRAME_GEO_S),fp);
                get_time_str(app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_sec,app->app_file[APP_FILE_DATA].filename);
                
                //snprintf(app->app_file[APP_FILE_DATA].filename,100,"%d.%d",app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_sec,app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_nsec);
                //printf("%s/%s.%s.%s",app->app_file[APP_FILE_DATA].path,app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_sec,app->app_file[APP_FILE_DATA].filename,app->app_file[APP_FILE_DATA].suffix);//0614正常使用模式

				snprintf(file_url,200,"%s/iws_data_%d.%s.%s",app->app_file[APP_FILE_DATA].path,app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_sec,app->app_file[APP_FILE_DATA].filename,app->app_file[APP_FILE_DATA].suffix);//0614正常使用模式
                app->app_file[APP_FILE_DATA].fid=fopen(file_url,"wb+");
                while(app->app_file[APP_FILE_DATA].fid==0)
                {
                    app->app_file[APP_FILE_DATA].fid=fopen(file_url,"wb+");
                }
				printf("fid=%d,file_url=%s\n",app->app_file[APP_FILE_DATA].fid,file_url);
				//exit(0);
                writedata(&myiws_head,sizeof(iws_head),app->app_file[APP_FILE_DATA].fid);
				app->app_file[APP_FILE_DATA].status=FILE_OK;
            }
            if(app->app_file[APP_FILE_DATA].status==FILE_OK){
            	//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
                #define reversebit(x,y)  x^=(1<<y)
                #define SD_LED 6
                reversebit(app->pca9539[1],SD_LED);
                //printf("current=%f\n",-1000*1000*getcurrent());
                set_pca9539(app->pca9539);
				app->app_file[APP_FILE_DATA].write_count+=writedata(app->buffer.databuf[LOOPSAVE]->data_start,app->app_para.fs*sizeof(DATA_FRAME_GEO_S),app->app_file[APP_FILE_DATA].fid);
            }
            if((app->app_file[APP_FILE_DATA].write_count)==(10*600*5*app->app_para.fs*sizeof(DATA_FRAME_GEO_S)))
            {
            	printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
            	close_data_file(app,APP_FILE_DATA);
                //exit(0);
            }

            //p_app->AdcNow[7]--;
            //printf(RED"inside function：%s,line : %d,!p_app->ping_pang=%d,p_app->AdcNow[7]=%d\n"NONE,__FUNCTION__,__LINE__,!p_app->ping_pang,p_app->AdcNow[7]);
            //
        }
        else{
        	printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
        	if(app->app_file[APP_FILE_DATA].status==FILE_OK){
        		close_data_file(app,APP_FILE_DATA);
        	}
        }
        //fwrite(app.buffer.databuf[LOOPSAVE]->data_start, 1,200*sizeof(DATA_FRAME_GEO_S),fp);
        app->app_sig.sig_data_save--;
        //printf(GREEN"function：%s,line:%d,app.app_sig.sig_data_send=%d\n"NONE,__FUNCTION__,__LINE__,p_app->app_sig.sig_data_send);
        app->buffer.databuf[LOOPSAVE]=app->buffer.databuf[LOOPSAVE]->loopbufnext;
    }
}


int get_diskinfo(char *argv,char * sendbuf);

unsigned int  sd_data_main(APP_S * app)
{	
    get_diskinfo("/home/pi/userdata",sendbuf_tmp);
	while(app->thread_switch.main_loop)
	{
		save_file(app);
	}

}



















int write_wave_index=0;


/*
        iws_up_wavedata->data_start_utc.u_sec=(unsigned int)(steim2_buf->utc_time[1]/100000);


        struct tm  *mytm2= localtime(&(steim2_buf->utc_time[0]));
        iws_up_wavedata->data_start_utc.year=mytm2->tm_year+1900;
        iws_up_wavedata->data_start_utc.yday=mytm2->tm_yday+1;
        iws_up_wavedata->data_start_utc.hour=mytm2->tm_hour;
        iws_up_wavedata->data_start_utc.min=mytm2->tm_min;
        iws_up_wavedata->data_start_utc.sec=mytm2->tm_sec;
*/

// int get_time_str(long tv,char * strTime)
// {
 
//         //char strTime[32];
//         struct tm *ptm = gmtime(&tv);  //将秒转换成struct tm的形式
//         strftime(strTime, 32, "%F-%T", ptm);
//         printf("%s ", strTime); //输出精确到秒
//         return 0;
// }





int save_steim2_file(APP_S * app)
{
    char file_url[200];
    snprintf(app->app_file[STEIM2_WAVE_DATA].path,200,"%s","/home/pi/userdata/data");
    snprintf(app->app_file[STEIM2_WAVE_DATA].suffix,200,"%s","iws");
    while(app->thread_switch.main_loop)
    {
        if(app->app_sig.sig_setad_request){
            app->app_sig.sig_setad_request=0;//
            app->app_sig.sig_setad_ready=1;
            //app.app_sig.sig_data_save=0;
        }
        //printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
        while((app->app_sig.sig_wave_data_ready<=0))
        {
            usleep(200*1000);//250ms
           // printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
                //
                //wait_go_signal(1);
        }
        //printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
        if(app->app_file[STEIM2_WAVE_DATA].control==FILE_SAVE){
            //printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
            if(app->app_file[STEIM2_WAVE_DATA].status==FILE_NEWFILE){
                snprintf(app->app_file[APP_FILE_DATA].filename,100,"%d.%d",app->iws_wave_pak_ext[write_wave_index%1000].utc_time[0],app->iws_wave_pak_ext[write_wave_index%1000].utc_time[1]);
                //writedata(app->buffer.databuf[LOOPSAVE]->data_start,200*sizeof(DATA_FRAME_GEO_S),fp);
                //get_time_str(app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_sec,app->app_file[APP_FILE_DATA].filename);
                
                //snprintf(app->app_file[APP_FILE_DATA].filename,100,"%d.%d",app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_sec,app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_nsec);
                //printf("%s/%s.%s.%s",app->app_file[APP_FILE_DATA].path,app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_sec,app->app_file[APP_FILE_DATA].filename,app->app_file[APP_FILE_DATA].suffix);//0614正常使用模式

                snprintf(file_url,200,"%s/%s.%s.%s",app->app_file[STEIM2_WAVE_DATA].path,app->app_file[APP_FILE_DATA].filename,"steim2",app->app_file[STEIM2_WAVE_DATA].suffix);//0614正常使用模式
                app->app_file[STEIM2_WAVE_DATA].fid=fopen(file_url,"wb+");
                while(app->app_file[STEIM2_WAVE_DATA].fid==0)
                {
                    app->app_file[STEIM2_WAVE_DATA].fid=fopen(file_url,"wb+");
                }
                printf("fid=%d,file_url=%s\n",app->app_file[STEIM2_WAVE_DATA].fid,file_url);
                //exit(0);
                //writedata(&myiws_head,sizeof(iws_head),app->app_file[STEIM2_WAVE_DATA].fid);
                app->app_file[STEIM2_WAVE_DATA].status=FILE_OK;
            }
            if(app->app_file[STEIM2_WAVE_DATA].status==FILE_OK){
                app->app_file[STEIM2_WAVE_DATA].write_count+=writedata(&(app->iws_wave_pak_ext[write_wave_index%1000].wcts[0]),256,app->app_file[STEIM2_WAVE_DATA].fid);

                //printf(GREEN"sizeof(IWS_UP_WAVEDATA):%d,write_count=%d\n"NONE,sizeof(IWS_UP_WAVEDATA),app->app_file[STEIM2_WAVE_DATA].write_count);
                //exit(0);
                write_wave_index++;
            }
            if((app->app_file[STEIM2_WAVE_DATA].write_count)==(256*10000))
            {
                printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
                close_data_file(app,STEIM2_WAVE_DATA);
            }

            //p_app->AdcNow[7]--;
            //printf(RED"inside function：%s,line : %d,!p_app->ping_pang=%d,p_app->AdcNow[7]=%d\n"NONE,__FUNCTION__,__LINE__,!p_app->ping_pang,p_app->AdcNow[7]);
            //
        }
        else{
            //printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
            if(app->app_file[STEIM2_WAVE_DATA].status==FILE_OK){
                close_data_file(app,STEIM2_WAVE_DATA);
            }
        }
        //fwrite(app.buffer.databuf[LOOPSAVE]->data_start, 1,200*sizeof(DATA_FRAME_GEO_S),fp);
        pthread_mutex_lock(&app->app_mut[mutex_lock_sd_save]);
            app->app_sig.sig_wave_data_ready--;//通知写sd卡过程可以读取
        pthread_mutex_unlock(&app->app_mut[mutex_lock_sd_save]);
        //printf(GREEN"function：%s,line:%d,app.app_sig.sig_data_send=%d\n"NONE,__FUNCTION__,__LINE__,p_app->app_sig.sig_data_send);
        //app->buffer.databuf[LOOPSAVE]=app->buffer.databuf[LOOPSAVE]->loopbufnext;
    }
}




int  sd_steid2_main(APP_S * app)
{   
    while(app->thread_switch.main_loop)
    {
        save_steim2_file(app);
    }

}







int p_num=0;


IWS_UP_WAVEDATA evt_wave_pak;


int test_write_evt(IWS_UP_WAVEDATA *evt_wave_pak)
{
    if(app.evt_record.is_file_open==1&&app.evt_record.fp!=NULL)
    {
        return writedata(evt_wave_pak,sizeof(IWS_UP_WAVEDATA),app.evt_record.fp);

    }

}


extern int steim2_evt_trig_pak(IWS_UP_WAVEDATA * iws_up_wavedata,int connect_index, int pak_num);
int  make_data_main(APP_S * app)
{   
    int ifor;
    while(app->thread_switch.main_loop)
    {
        while(app->app_sig.sig_can_steim<=0)
        {
            usleep(50*1000);//250ms
           // printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
                //
                //wait_go_signal(1);
        }
        app->app_sig.sig_wave_data_write_buf=(app->app_sig.sig_wave_data_write_buf_total)%1000;
        steim2_mk_ext_wave_pak(&(app->iws_wave_pak_ext[app->app_sig.sig_wave_data_write_buf]),app->buffer.steim2_out_buf[STEIM2_MAKE_PAK],app->app_sig.sig_wave_data_write_buf_total);
        app->app_sig.sig_wave_data_write_buf_total++;
        //int steim2_mk_wave_pak(IWS_UP_WAVEDATA * iws_up_wavedata,IWS_STEIM2_OUT * steim2_buf,int pak_num)
        //app->iws_wave_pak[0];
        //app->app_sig.sig_wave_data_ready--;
        for(ifor=0;ifor<6;ifor++)
        {
            if((app->iws_server[ifor].is_trig_start==1)&&(app->iws_server[ifor].status.isconnect==1)&&(app->iws_server[ifor].mode==2)){
                //app->buffer.steim2_out_buf[STEIM2_NOW];
                pthread_mutex_lock(&app->app_mut[ifor]);
                    app->iws_server[ifor].sig_trig_wave++;//通知触发发送波形可以读取
                    //printf(BLUE"app->iws_server[ifor].sig_trig_wave=%d\n"NONE,app->iws_server[ifor].sig_trig_wave);
                    //app->iws_server[ifor].is_trig_start=1;
            //app->iws_server[connect_index].sig--;
                pthread_mutex_unlock(&app->app_mut[ifor]);
                //app->iws_server[ifor].sig_trig_wave++;
            }
        }
        if(app->is_trig_start==1){
            app->sig_trig_wave++;
        }
        



        while((app->sig_trig_wave>0))//触发传波形
        {
            steim2_evt_trig_pak(&evt_wave_pak,0,0);
            test_write_evt(&evt_wave_pak);
            app->sig_trig_wave--;
            //printf(GREEN"app->iws_server[connect_index].sig_trig_wave=%d\n"NONE,app->iws_server[connect_index].sig_trig_wave);
            //steim2_mk_trig_pak(&app->iws_cstp[connect_index].iws_up_wavedata,connect_index,++(app->iws_cstp[connect_index].pak_num));

        }







        //mutex_lock_sd_save
        pthread_mutex_lock(&app->app_mut[mutex_lock_sd_save]);
            app->app_sig.sig_wave_data_ready++;//通知写sd卡过程可以读取
        pthread_mutex_unlock(&app->app_mut[mutex_lock_sd_save]);

        pthread_mutex_lock(&app->app_mut[MUTEX_LOCK_IS_CAN_STEIM]);
            app->app_sig.sig_can_steim--;
    //app->iws_server[connect_index].sig--;
        pthread_mutex_unlock(&app->app_mut[MUTEX_LOCK_IS_CAN_STEIM]);
        
        //printf(GREEN"app->app_sig.sig_can_steim=%d\n"NONE,app->app_sig.sig_can_steim);
        app->buffer.steim2_out_buf[STEIM2_MAKE_PAK]=app->buffer.steim2_out_buf[STEIM2_MAKE_PAK]->next;
        //save_steim2_file(app);
    }

}































static int get_disk_infomation(const char *path, struct disk_infomation *di);
static char *convert_size(float size, char *dest);
static unsigned long kscale(unsigned long b, unsigned long bs) ;









int get_diskinfo(char *argv,char * sendbuf)
{
                    printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

    struct disk_infomation *di=&app.di;;//malloc(sizeof(struct disk_infomation));
    char *path =(char *)malloc(20);
    strncpy(path, argv, strlen(argv));
    get_disk_infomation(path, di);
                printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

    // printf("DiskType:            %s\n", di->disk_format);
    // printf("DiskTotalCapacity    %s\n", di->disk_total_capacity);
    // printf("DiskFreeCapacity    %s\n", di->disk_free_capacity);
    // printf("DiskPermissions        %s\n", di->disk_permissions);
                                printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

    sprintf(sendbuf,"DT=%s,DTC=%s,DFC=%s,DP=%s,%d",di->disk_format,di->disk_total_capacity\
        ,di->disk_free_capacity,di->disk_permissions,app.app_file[APP_FILE_DATA].control);
                    printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

    //free(di);
    free(path);
    return 0;
}



int iws_client_main(APP_S * app)
{
    printf("in iws_client_main ping_pang=%d\n",app->ping_pang);
    exit(0);
    return 0;
}



int print_diskinfo(char * argv)
{
    struct disk_infomation *di=malloc(sizeof(struct disk_infomation));
    char *path =(char *)malloc(20);
    strncpy(path, argv, strlen(argv));
                    printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

    get_disk_infomation(path, di);
                printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

    printf("DiskType:            %s\n", di->disk_format);
    printf("DiskTotalCapacity    %s\n", di->disk_total_capacity);
    printf("DiskFreeCapacity    %s\n", di->disk_free_capacity);
    printf("DiskPermissions        %s\n", di->disk_permissions);

    free(di);
    free(path);
    return 0;
}

/** 
 * @brief block to byte 
 * @param b     block
 * @param bs     KByte
 * @return         Kbyte
 */
static unsigned long kscale(unsigned long b, unsigned long bs) 
{
    return (b * (unsigned long long) bs + 1024/2) / 1024;
}

/** 
 * @brief get disk informations 
 * @param path     the disk mount point
 * @param di     the structure of the disk information
 * @return         success: 1, faild: 0
 */
static int get_disk_infomation(const char *path,
                        struct disk_infomation *di)
{
    struct statfs buf;
    int i=0;
    float disk_total_size = 0;
    float disk_free_size = 0;

    printf("path=%s\n",path);
    i=statfs(path, &buf);
    if(i < 0){
        printf("get disk infomation faild\n");
        return 0;
    }
    switch(buf.f_type)
    {
        memset(di->disk_format, 0, 8);
        case 0x4d44:
        sprintf(di->disk_format, "FAT");
        break;

        case 0x5346544e:
        case 0X65735546: 
        sprintf(di->disk_format, "NTFS");
        break;

        case 0xEF53:
        case 0xEF51:
        sprintf(di->disk_format, "EXT2");
        break;

        default:
        sprintf(di->disk_format, "unknown");
        break;
    }


    memset(di->disk_total_capacity, 0, 10);
    memset(di->disk_free_capacity, 0, 10);

    disk_total_size = 
            (float)(kscale(buf.f_blocks, buf.f_bsize));
    disk_free_size = 
            (float)(kscale(buf.f_bfree, buf.f_bsize));

    convert_size(disk_total_size, di->disk_total_capacity);
    convert_size(disk_free_size, di->disk_free_capacity);

    memset(di->disk_permissions, 0, 3);
    sprintf(di->disk_permissions, "rw\n");
    return 1;
}


/** 
 * @name convert_size
 * @brief convert size to GB MB KB
 *         size:    the size
 *         dest:    the return size like string
 * @author JerryLeo
 * @date 2009-12-12
 */
static char *convert_size(float size, char *dest)
{
    if((((size/1024.0)/1024.0)) >= 1.0){ 
        sprintf(dest, "%0.2fGB", ((size/1024.0)/1024.0));
    } 
    else if(((size/1024.0)) >= 1.0){ 
        sprintf(dest, "%0.2fMB", (size/1024.0));
    } 
    else{
        sprintf(dest, "%0.2fKB", size);
    } 
    return dest;
}

