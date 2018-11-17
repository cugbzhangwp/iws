#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <libgen.h>
#include <sys/types.h>   
#include <sys/socket.h>   
#include <sys/sysinfo.h>  
//#include <linux/tcp.h>
#include <sys/vfs.h> 
#include <netinet/in.h>   
#include <arpa/inet.h>
#include <netinet/tcp.h>  
#include <printcolor.h>
#include <cstp.h>

#include <iws.h>
#include <app_main.h>
#include <ublox.h>



#include <stdio.h>//printf
#include <string.h> //strcmp
#include <sys/inotify.h>//inotify_init inotify_add_watch....
#include <sys/select.h>//select timeval
#include <unistd.h>//close


#define CSTP_DEBUG_COLOR BROWN



#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#define ERR_EXIT(msg,flag)  {perror(msg);goto flag;}

extern int add_file_list(char * fileurl);
extern int del_file_list(char * fileurl);
extern int printf_file_list();
extern int init_psa_pga();

int is_sd_mounted=0;


extern APP_S app;





int htonl_re(IWS_REGISTER *pak)
{
    pak->package_number=htonl(pak->package_number);
    pak->control_authority=htons(pak->control_authority);
    pak->longitude=htonl(pak->longitude);
    pak->latitude=htonl(pak->latitude);
    pak->altitude=htons(pak->altitude);
    pak->sensitivity=htonl(pak->sensitivity);
    pak->init_tran_mode=htons(pak->init_tran_mode);
    return 0;
}





int htonl_cc_re(IWS_CC_RE *pak)
{
    pak->ctrl_id=htons(pak->ctrl_id);
    pak->package_number=htonl(pak->package_number);
    pak->re_status=htons(pak->re_status);
    return 0;
}



int htonl_si(IWS_UP_SI *pak)
{
    pak->package_number=htonl(pak->package_number);
    pak->ab_time=htonl(pak->ab_time);
    pak->time=htonl(pak->time);
    int ifor;
    for(ifor=0;ifor<10;ifor++)
    {
        pak->ud[ifor]=htonl(pak->ud[ifor]);
        pak->ew[ifor]=htonl(pak->ew[ifor]);
        pak->ns[ifor]=htonl(pak->ns[ifor]);    
    }

    return 0;
}



int htonl_ti(IWS_UP_TI *pak)
{
    pak->package_number=htonl(pak->package_number);
    pak->imt_s=htonl(pak->imt_s);
    pak->imt_ms=htons(pak->imt_ms);
    pak->rt_s=htons(pak->rt_s);
    pak->sta_lta=htonl(pak->sta_lta);
     pak->imd=htons(pak->imd);
    pak->ud_pga=htonl(pak->ud_pga);
    pak->ud_pgv=htonl(pak->ud_pgv);
    pak->ud_pgd=htonl(pak->ud_pgd);
    pak->ew_pga=htonl(pak->ew_pga);
    pak->ew_pgv=htonl(pak->ew_pgv);
     pak->ew_pgd=htonl(pak->ew_pgd);
    pak->ns_pga=htonl(pak->ns_pga);
    pak->ns_pgd=htonl(pak->ns_pgd);
    pak->ns_pgv=htonl(pak->ns_pgv);
    pak->cdv=htons(pak->cdv);
    pak->ud_psa03=htonl(pak->ud_psa03);
     pak->ud_psa10=htonl(pak->ud_psa10);
    pak->ud_psa30=htonl(pak->ud_psa30);
    pak->ew_psa03=htonl(pak->ew_psa03);
    pak->ew_psa10=htonl(pak->ew_psa10);
    pak->ew_psa30=htonl(pak->ew_psa30);
    pak->ns_psa03=htonl(pak->ns_psa03);
     pak->ns_psa10=htonl(pak->ns_psa10);
     pak->ns_psa30=htonl(pak->ns_psa30);
    return 0;
}


int htonl_wave(IWS_UP_WAVEDATA *pak)
{
    //pak->package_number=htonl(pak->package_number);
    //pak->imt_s=htonl(pak->imt_s);
    pak->datacount=htons(pak->datacount);
     //pak->fs_factor=htons(pak->fs_factor);
    pak->srm=htons(pak->srm);
    pak->tcv=htonl(pak->tcv);
     pak->dso=htons(pak->dso);
    pak->fdbo=htons(pak->fdbo);
    pak->sbt1000=htons(pak->sbt1000);
    pak->nbbn=htons(pak->nbbn);
    pak->sbt1002=htons(pak->sbt1002);
    pak->sf=htonl(pak->sf);
    return 0;
}

int htonl_wave_head(IWS_UP_WAVEDATA *pak)
{
    pak->package_number=htonl(pak->package_number);
    //pak->imt_s=htonl(pak->imt_s);
    pak->datacount=htons(pak->datacount);
    pak->fs_factor=htons(pak->fs_factor);
    return 0;
}





int fs_monitor(APP_S * app)
{
    for(;;){
        filenevt(app,"/home/pi/userdata/data/steim2"); //监测文件夹变化 
    
    }
    
}

int filenevt(APP_S * app,char * path) //监测文件夹变化
{
    int length, i = 0;
    int fd;
    int wd;
    char buffer[BUF_LEN];
    char url[200];

    if((fd = inotify_init()) < 0)
        ERR_EXIT("inotify_init",inotify_init_err);

    if( (wd = inotify_add_watch( fd, path,   IN_CREATE | IN_DELETE ) ) < 0)
        ERR_EXIT("inofity_add_watch", inotify_add_watch_err);
    
    fd_set rfd;
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 10000;//10millsecond
    while(1)
    {
        int retval;
        FD_ZERO(&rfd);
        FD_SET(fd, &rfd);
        retval = select(fd + 1, &rfd, NULL, NULL, &tv);
        if(retval == 0) continue;
        else if(retval == -1)
            ERR_EXIT("select",select_err);

        // retval > 0
        length = read( fd, buffer, BUF_LEN );  
        if(length < 0)
            ERR_EXIT("read",read_err);

        //length >= 0
        int i = 0;
        while ( i < length ) 
        {
            struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
            if ( event->len ) 
            {
                if ( event->mask & IN_CREATE ) 
                {
                    if ( event->mask & IN_ISDIR ) {
                        printf( "The directory %s was created.\n", event->name );       
                    }
                    else{
                        snprintf(url,200,"%s/%s",path,event->name);
                        add_file_list(url);
                        printf_file_list();
                        printf( "The file %s was created.\n", event->name );
                    }
                    if(strcmp(event->name,"kill") == 0){
                        ERR_EXIT("success exit",success_exit);
                    }

                }
                else if ( event->mask & IN_DELETE ) 
                {
                    if ( event->mask & IN_ISDIR ) {
                        printf( "The directory %s was deleted.\n", event->name );       
                    }
                    else{
                        del_file_list(url);
                        printf_file_list();
                        printf( "The file %s was deleted.\n", event->name );
                    }
                }
                else if ( event->mask & IN_MODIFY ) 
                {
                    if ( event->mask & IN_ISDIR ){
                        printf( "The directory %s was modified.\n", event->name );
                    }
                    else{
                        printf( "The file %s was modified.\n", event->name );
                    }
                }
            }else
            {
                //TODO
                //when only a file(not directory) is specified by add watch function, event->len's value may be zero, we can handle it here
            }
            i += EVENT_SIZE + event->len;
        }
    }
success_exit:
    ( void ) inotify_rm_watch( fd, wd );
    ( void ) close( fd );
    return 0;

read_err:
select_err:
inotify_add_watch_err:
    ( void ) inotify_rm_watch( fd, wd );
inotify_init_err:
    ( void ) close( fd );

    return -1;
}













extern struct gps_data_t gps_data;
extern APP_S app;
#define BUFSIZE 1024
IWS_CSTP iws_cstp_globe;


unsigned char iws_client_buf[2048];


extern int writenbytes(char * ch,int sockfd,int needread);
extern int readnbytes(char * ch,int sockfd,int needread);


char * host_id="YJ.IWS00";
char * serv_psd="01234567 ";
char * serv_psd1="76543210";
int init_iws_reg_pak(IWS_REGISTER * iws_reg_pak,int server_index)
{
    iws_reg_pak->type_lable[0]='R';
    iws_reg_pak->type_lable[1]='E';
    iws_reg_pak->package_number=0;
    //app.iws_server[index].sid
    int ifor;
    for(ifor=0;ifor<8;ifor++)
    {
         iws_reg_pak->sta_id[ifor]=app.iws_server[server_index].sid[ifor];   
    }
    for(ifor=0;ifor<32;ifor++)
    {
         iws_reg_pak->key[ifor]=app.iws_server[server_index].server_key[ifor];   
    }
    // for(ifor=7;ifor<15;ifor++)
    // {
    //      iws_reg_pak->key[ifor]=serv_psd[ifor];//app.iws_server[server_index].server_key[ifor];   
    // }
    // for(ifor=16;ifor<24;ifor++)
    // {
    //      iws_reg_pak->key[ifor]=serv_psd[ifor];//app.iws_server[server_index].server_key[ifor];   
    // }
    // for(ifor=24;ifor<32;ifor++)
    // {
    //      iws_reg_pak->key[ifor]=serv_psd[ifor];//app.iws_server[server_index].server_key[ifor];   
    // }
    // for(ifor=24;ifor<32;ifor++)
    // {
    //      iws_reg_pak->key[ifor]=serv_psd1[ifor];//app.iws_server[server_index].server_key[ifor];   
    // }
    // snprintf(iws_reg_pak->sta_id,8,"%s","jd.00001");
    // snprintf(iws_reg_pak->key,32,"%s","xxxxxxxxxxxxxx");
    iws_reg_pak->control_authority=0;//app.iws_server[server_index].status.iscan_control;
    iws_reg_pak->init_tran_mode=app.iws_server[server_index].mode;
    //app->iws_server[server_index].mode=1;
    iws_reg_pak->longitude=3992*1000;//gps_data.ns_d*1000000+gps_data.ns_m10000;
    iws_reg_pak->latitude=11646*1000;//gps_data.ew_d*1000000+gps_data.ew_m10000;
    iws_reg_pak->altitude=40;gps_data.height_m;
    iws_reg_pak->sensitivity=5000000;//171192421;
    htonl_re(iws_reg_pak);
/*
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
    short init_trigger_threshold;
    char unuse[192];
*/
}






int pak_iws_reg_pak(IWS_REGISTER * iws_reg_pak)//
{
    iws_reg_pak->type_lable[0]='R';
    iws_reg_pak->type_lable[1]='E';
    iws_reg_pak->package_number=0;
    snprintf(iws_reg_pak->sta_id,8,"%s","jd.00001");
    snprintf(iws_reg_pak->key,32,"%s","xxxxxxxxxxxxxx");
    iws_reg_pak->control_authority=0;
    iws_reg_pak->init_tran_mode=1;

/*
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
    short init_trigger_threshold;
    char unuse[192];
*/
}


/*
    IWS_REGISTER iws_reg_pak;//注册信息包
    IWS_REGISTER_RE iws_reg_re_pak;//注册信息相应包
    IWS_TRANS_CTRL iws_control_pak;//传输模式控制包
    IWS_TIMEDATA_REQUEST iws_timedata_request_pak;//时段请求数据包
    IWS_TRIGGER_THRESHOLD_CTRL iws_trigger_threshold_pak;//触发阈值设置包
    IWS_CC_RE iws_cc_re_pak;//控制命令应答包
    IWS_UP_SI iws_up_si_pak;//状态信息包
    IWS_UP_TI iws_up_ti_pak;//触发信息包
*/
int init_iws_up_ti_pak(IWS_UP_TI * iws_up_ti_pak,int server_index)
{
    iws_up_ti_pak->type_lable[0]='T';
    iws_up_ti_pak->type_lable[1]='I';
    int ifor;
    for(ifor=0;ifor<8;ifor++)
    {
         iws_up_ti_pak->sta_id[ifor]=app.iws_server[server_index].sid[ifor];   
    }
/*
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
    short init_trigger_threshold;
    char unuse[192];
*/
}

int init_iws_up_si_pak(IWS_UP_SI * iws_up_si_pak,int server_index)
{
    iws_up_si_pak->type_lable[0]='S';
    iws_up_si_pak->type_lable[1]='I';
    iws_up_si_pak->package_number=0;
    int ifor;
    for(ifor=0;ifor<8;ifor++)
    {
         iws_up_si_pak->sta_id[ifor]=app.iws_server[server_index].sid[ifor];   
    }
/*
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
    short init_trigger_threshold;
    char unuse[192];
*/
}
int init_iws_cc_re_pak(IWS_CC_RE * iws_cc_re_pak,int server_index)
{
    iws_cc_re_pak->type_lable[0]='C';
    iws_cc_re_pak->type_lable[1]='R';
    iws_cc_re_pak->package_number=0;
    int ifor;
    for(ifor=0;ifor<8;ifor++)
    {
         iws_cc_re_pak->sta_id[ifor]=app.iws_server[server_index].sid[ifor];   
    }
    // for(ifor=0;ifor<32;ifor++)
    // {
    //      iws_cc_re_pak->key[ifor]=app.iws_server[server_index].server_key[ifor];   
    // }
    //snprintf(iws_cc_re_pak->key,32,"%s","xxxxxxxxxxxxxx");
    //iws_reg_pak->control_authority=0;
    //iws_reg_pak->init_tran_mode=1;
/*
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
    short init_trigger_threshold;
    char unuse[192];
*/
}

int init_iws_trigger_threshold_pak(IWS_TRIGGER_THRESHOLD_CTRL * iws_trigger_threshold_pak,int server_index)
{
    iws_trigger_threshold_pak->type_lable[0]='C';
    iws_trigger_threshold_pak->type_lable[1]='C';
    iws_trigger_threshold_pak->package_number=0;
    iws_trigger_threshold_pak->ctrl_id=3;
    //app.iws_server[index].sid
    int ifor;
    for(ifor=0;ifor<8;ifor++)
    {
         iws_trigger_threshold_pak->sta_id[ifor]=app.iws_server[server_index].sid[ifor];   
    }
    return 0;
/*
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
    short init_trigger_threshold;
    char unuse[192];
*/
}
int init_iws_timedata_request_pak(IWS_TIMEDATA_REQUEST * iws_timedata_request_pak,int server_index)
{
    iws_timedata_request_pak->type_lable[0]='C';
    iws_timedata_request_pak->type_lable[1]='C';
    iws_timedata_request_pak->package_number=0;
    iws_timedata_request_pak->ctrl_id=2;
    //app.iws_server[index].sid
    int ifor;
    for(ifor=0;ifor<8;ifor++)
    {
         iws_timedata_request_pak->sta_id[ifor]=app.iws_server[server_index].sid[ifor];   
    }
    return 0;
/*
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
    short init_trigger_threshold;
    char unuse[192];
*/
}


int init_iws_control_pak(IWS_TRANS_CTRL * iws_control_pak,int server_index)
{

    iws_control_pak->type_lable[0]='C';
    iws_control_pak->type_lable[1]='C';
    iws_control_pak->package_number=0;
    iws_control_pak->ctrl_id=1;
    //app.iws_server[index].sid
    int ifor;
    for(ifor=0;ifor<8;ifor++)
    {
         iws_control_pak->sta_id[ifor]=app.iws_server[server_index].sid[ifor];   
    }
    return 0;
}

int init_iws_reg_re_pak(IWS_REGISTER_RE * iws_reg_re_pak,int server_index)//注册信息响应包
{
    iws_reg_re_pak->type_lable[0]='R';
    iws_reg_re_pak->type_lable[1]='R';
    iws_reg_re_pak->package_number=0;
    int ifor;
    for(ifor=0;ifor<8;ifor++)
    {
         iws_reg_re_pak->sta_id[ifor]=app.iws_server[server_index].sid[ifor];   
    }
    // for(ifor=0;ifor<32;ifor++)
    // {
    //      iws_reg_re_pak->key[ifor]=app.iws_server[server_index].server_key[ifor];   
    // }
    iws_reg_re_pak->last_package_number=0;
/*
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
    short init_trigger_threshold;
    char unuse[192];
*/
}


int init_iws_wave_pak(IWS_UP_WAVEDATA * iws_up_wavedata,int server_index)//注册信息响应包
{
/*
    char wcts[2];//wc ,wt,ws
    int package_number;
    char qi;//quality index.D
    char reserve;
    char sta_idx[5];
    char pos_idx[2];
    char ch_idx[3];
    char net_idx[2];
    char data_start_utc[10];
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
*/
        iws_up_wavedata->wcts[0]='W';
        iws_up_wavedata->wcts[1]='C';
        iws_up_wavedata->package_number=0;//*
        iws_up_wavedata->sf=5000000;//171192421;
        iws_up_wavedata->qi='D';
        iws_up_wavedata->reserve=' ';
        iws_up_wavedata->sta_idx[0]='A';
        iws_up_wavedata->sta_idx[1]='0';
        iws_up_wavedata->sta_idx[2]='0';
        iws_up_wavedata->sta_idx[3]='0';
        iws_up_wavedata->sta_idx[4]='1';
         iws_up_wavedata->pos_idx[0]='4';
        iws_up_wavedata->pos_idx[1]='0';
          iws_up_wavedata->ch_idx[0]='E';
        iws_up_wavedata->ch_idx[1]='I';
        iws_up_wavedata->ch_idx[2]='\0';//*
        iws_up_wavedata->net_idx[0]=app.iws_server[server_index].sid[0];
        iws_up_wavedata->net_idx[1]=app.iws_server[server_index].sid[1];
        iws_up_wavedata->srm=1;
        iws_up_wavedata->as=0;//(0<<0)|(1<<1);
        iws_up_wavedata->iotm='\0';
        iws_up_wavedata->dqm='\0';
        iws_up_wavedata->nbs=2;
        iws_up_wavedata->tcv=0;
        iws_up_wavedata->dso=64;
        iws_up_wavedata->fdbo=48;
        iws_up_wavedata->sbt1000=1000;
        iws_up_wavedata->nbbn=56;
        iws_up_wavedata->cf=11;
        iws_up_wavedata->order=1;
        iws_up_wavedata->drl=8;
        iws_up_wavedata->sbt1002=1002;
        iws_up_wavedata->dimension=3;
        int ifor;
        for(ifor=0;ifor<5;ifor++)
        {
             iws_up_wavedata->sta_idx[ifor]=app.iws_server[server_index].sid[ifor+3];   
        }
        htonl_wave(iws_up_wavedata);
        return 0;
    // char type_lable[2];
    // int package_number;
    // char sta_id[8];
    // char key[32];
    // short control_authority;
    // int longitude;
    // int latitude;
    // short altitude;
    // int sensitivity;
    // short init_tran_mode;
    // short init_trigger_threshold;
    // char unuse[192];
}

int init_cstp_globe(IWS_CSTP *iws_cstp_globe,int server_index)
{
    init_iws_reg_pak(&(iws_cstp_globe->iws_reg_pak),server_index);
    init_iws_cc_re_pak(&(iws_cstp_globe->iws_cc_re_pak),server_index);
    //init_iws_cc_re_pak(IWS_CC_RE * iws_cc_re_pak,int server_index);
//     init_iws_reg_re_pak(&iws_cstp_globe->iws_reg_re_pak,server_index);//注册信息响应包
//     init_iws_control_pak(&iws_cstp_globe->iws_control_pak,server_index);
//     init_iws_timedata_request_pak(&iws_cstp_globe->iws_timedata_request_pak,server_index);
//     init_iws_trigger_threshold_pak(&iws_cstp_globe->iws_trigger_threshold_pak,server_index);
// init_iws_up_si_pak(&iws_cstp_globe->iws_up_si_pak,server_index);
// init_iws_up_ti_pak(&iws_cstp_globe->iws_up_ti_pak,server_index);
// init_iws_wave_pak(&iws_cstp_globe->iws_reg_re_pak,server_index);//注册信息响应包
/*
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
    short init_trigger_threshold;
    char unuse[192];
*/
}









int init_server(IWS_SERVER *iws_server)
{
/*
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
    unsigned char unuse[2048-80-118-80-sizeof(IWS_STEIM2)];
}__attribute__ ((packed,aligned(1)))IWS_SERVER;
*/


    iws_server->steim2_buf=NULL;
    iws_server->status_send_buf=NULL;//服务器目前发送状态信息包指针
    iws_server->sig_status=0;//服务器目前发送状态信息包指针
    iws_server->sig_trig=0;//服务器目前发送状态信息包指针
    iws_server->sig_status=0;//服务器目前发送状态信息包指针
    iws_server->sig_trig_wave=0;//服务器目前发送状态信息包指针
    iws_server->sig=0;//服务器目前发送状态信息包指针
    return 0;
}







int init_cstp(IWS_CSTP *iws_cstp_globe,int server_index)
{
    int ifor;
    //iws_cstp_globe.pak_num=0;
    init_iws_reg_pak(&iws_cstp_globe->iws_reg_pak,server_index);
    init_iws_cc_re_pak(&iws_cstp_globe->iws_cc_re_pak,server_index);    
    init_iws_reg_re_pak(&iws_cstp_globe->iws_reg_re_pak,server_index);//注册信息响应包
    init_iws_control_pak(&iws_cstp_globe->iws_control_pak,server_index);
    init_iws_timedata_request_pak(&iws_cstp_globe->iws_timedata_request_pak,server_index);
    init_iws_trigger_threshold_pak(&iws_cstp_globe->iws_trigger_threshold_pak,server_index);
    init_iws_up_si_pak(&iws_cstp_globe->iws_up_si_pak,server_index);
    init_iws_up_ti_pak(&iws_cstp_globe->iws_up_ti,server_index);
    init_iws_wave_pak(&iws_cstp_globe->iws_up_wavedata,server_index);//注册信息响应包
    return 0;
/*
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
    short init_trigger_threshold;
    char unuse[192];
*/
}







int wait_for_device_ok(APP_S * app)
{
    int gps_ok=0;
    while(gps_ok==0)
    {
        if(gps_data.status=='A'){
            gps_ok=1;
        }
        sleep(1);
        //检查各设备状态
    }
    //exit(0);
    return 0;
}

#define CSTP_UP_PAKLEN 256
#define CSTP_DOWN_PAKLEN 64
int check_reg_re(char * buf,int server_index)
{
    /*
    char type_lable[2];
    int package_number;
    char sta_id[8];
    short auth_state;
    int last_package_number;
    char unuse[44];
    */
    IWS_REGISTER_RE * reg_re=(IWS_REGISTER_RE * )buf;
    if(reg_re->type_lable[0]=='R'&&reg_re->type_lable[1]=='R'){
        ;
    }
    else{
        return -1;
    }

    if(reg_re->auth_state==0){
        app.iws_cstp[server_index].pak_num=ntohl(reg_re->last_package_number);//返回上次包序号
        return 0;
    }
    else{
        printf(CYAN"reg_re->auth_state=%d\n"NONE,reg_re->auth_state);
        return -2;
    }
}
int register_device_iws(APP_S * app,IWS_REGISTER * iws_reg_pak,int sockfd)//等待设备状态就绪，从而开始设备注册
{

    char readbuf[1024];
    int success_len,sendlen=0,recvlen=0;
    int ifor;
    int ret;
    for(ifor=0;ifor<5;ifor++)
    {
        printf(PURPLE"try to connect server sizeof(IWS_REGISTER)=%d\n"NONE,sizeof(IWS_REGISTER));
        sendlen = write(sockfd, iws_reg_pak, sizeof(IWS_REGISTER));
        if(sendlen==CSTP_UP_PAKLEN){
            recvlen = read(sockfd, readbuf, CSTP_DOWN_PAKLEN);
            //ret=check_reg_re(readbuf);
            printf("check_reg_re ret=%d\n",ret);
            if(ret==0){
                //&(iws_cstp_globe->iws_reg_pak)
                return 0;
            }
            else{
                sleep(10);
                ;//循环
            }
        }
        
    }
}


//register_device_iws



int select_register_device_iws(APP_S * app,IWS_REGISTER * iws_reg_pak,int sockfd,int server_index)//等待设备状态就绪，从而开始设备注册
{
printf(CYAN"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    char readbuf[1024];
    int success_len,sendlen=0,recvlen=0,realwrite=0;
    int ifor;
    int ret;
    int try_times=0;
    int has_read=1;
    char *p_re=(char * )iws_reg_pak;
    while(try_times<50){
        int rw_select_ret=rw_select(sockfd);
        printf(PURPLE"inside function %s line:%d,rw_select_ret=%d\n"NONE,__FUNCTION__,__LINE__,rw_select_ret);

        switch(rw_select_ret)
        {
            case 1://read
            printf(CYAN"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
                recvlen = readnbytes(readbuf,sockfd,  CSTP_DOWN_PAKLEN);
             printf(CYAN"recvlen:%d\n"NONE,recvlen);
               for(ifor=0;ifor<recvlen;ifor++)
                {
                    printf(CYAN"%02X-%c "NONE,readbuf[ifor],readbuf[ifor]);
                }
                printf("\n");
                if(recvlen==CSTP_DOWN_PAKLEN){
                    ret=check_reg_re(readbuf,server_index);
                    printf(LIGHT_GREEN"check_reg_re ret=%d\n"NONE,ret);
                    if(ret==0){
                    //&(iws_cstp_globe->iws_reg_pak)
                        return 0;
                    }
                    else{
                        sleep(1);
                        has_read=1;
                        ;//循环
                    }
                }
                if(recvlen<=0){
                    return -1;
                }



            break;
            case 2://write
            //printf(CYAN"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
                if(has_read==1){
                    has_read=0;//TODO must be 0
                    try_times++;
                    
                    int ifor;
                    for(ifor=6;ifor<32;ifor++)
                    {
                        iws_reg_pak->key[ifor]='\0';
                    }
                    realwrite=writenbytes(iws_reg_pak,sockfd,sizeof(IWS_REGISTER));
                    for(ifor=0;ifor<256;ifor++)
                    {
                        printf(YELLOW"%d:%02X-%c "NONE,ifor,p_re[ifor],p_re[ifor]);
                    }
/*
        char *p=&app->iws_cstp[connect_index].iws_up_wavedata;
        for(ifor=0;ifor<realread;ifor++)
        {
            printf(LIGHT_RED"%d:%02X-%c "NONE,ifor,p[ifor],p[ifor]);
        }
        printf("\n");

*/
                    printf("\n");
                    for(ifor=0;ifor<256;ifor++)
                    {
                        printf(GREEN"%02X "NONE,p_re[ifor]);
                    }
/*
        char *p=&app->iws_cstp[connect_index].iws_up_wavedata;
        for(ifor=0;ifor<realread;ifor++)
        {
            printf(LIGHT_RED"%d:%02X-%c "NONE,ifor,p[ifor],p[ifor]);
        }
        printf("\n");

*/
                    printf("\n");

                    printf(CYAN"inside function %s line:%d realwrite=%d\n"NONE,__FUNCTION__,__LINE__,realwrite);
                    sleep(1);
                }
                else{
                    sleep(1);
                    ;
                }
                if(realwrite<0){
                    return realwrite;
                }
            break;
            case -1:
                return -1;
            break;
            default:
                return -2;
            break;
                
        }
    }




}



int rw_select_server(int fid,int server_index)
{
    fd_set t_set1,t_set_server,t_set_read,t_set_write;
    struct timeval  tv;
    FD_ZERO(&t_set_write);
    FD_ZERO(&t_set_read);
    FD_SET(fid, &t_set_write);
    FD_SET(fid, &t_set_read);
    tv.tv_sec= 10;
    tv.tv_usec= 1000;
    int i_state= 0;
    //printf("--------------->3\ttcp_ser wait for select\n");
    i_state= select(fid+1,  &t_set_read, &t_set_write,NULL, &tv);//*/

    if(FD_ISSET(fid,&t_set_read)){//判断是否可读
        //printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        //exit(0);
        return 1;
    }
    if (i_state < 0) {
        printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
        return i_state;
    };
    if (i_state == 0){
        printf(RED"server_index=%d ,inside function %s line:%d\n"NONE,server_index,__FUNCTION__,__LINE__);
        return 0;
    }
    if(FD_ISSET(fid,&t_set_write)){//判断是否可写
        return 2;
    }


}


int rw_select(int fid)
{
    fd_set t_set1,t_set_server,t_set_read,t_set_write;
    struct timeval  tv;
    FD_ZERO(&t_set_write);
    FD_ZERO(&t_set_read);
    FD_SET(fid, &t_set_write);
    FD_SET(fid, &t_set_read);
    tv.tv_sec= 10;
    tv.tv_usec= 1000;
    int i_state= 0;
    //printf("--------------->3\ttcp_ser wait for select\n");
    i_state= select(fid+1,  &t_set_read, &t_set_write,NULL, &tv);//*/

    if(FD_ISSET(fid,&t_set_read)){//判断是否可读
        //printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        //exit(0);
        return 1;
    }
    if (i_state < 0) {
        printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
        return -1;
    };
    if (i_state == 0){
        printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    }
    if(FD_ISSET(fid,&t_set_write)){//判断是否可写
        return 2;
    }


}




int iws_read_pre_check(const char * buf,int len)
{
    int ifor;
    if(buf[0]=='C'&&buf[1]=='C'){
        return 0;
    }
    if(buf[0]=='R'&&buf[1]=='R'){
        return 1;
    }
    else{
        return -1;
    }
}















int iws_read_check_CC_pkid(int pkid,int server_index)
{
    app.iws_cstp[server_index].server_pak_num=ntohl(pkid);//获取服务器发下的控制命令包序号
    return 0;
}




int iws_read_check_RR_pkid(int pkid,int server_index)
{
    app.iws_cstp[server_index].pak_num=0;//ntohl(pkid);
    printf("pkid=%d,app.iws_cstp[server_index].pak_num=%d\n",pkid,app.iws_cstp[server_index].pak_num);
    //exit(0);
    return 0;
}






//char * host_id="abcdefgh";

int iws_read_check_CC_staid(const char * staid,int server_index)
{
    return 0;//TODO
    int ifor;
#define STAID_LEN 8
    for(ifor=0;ifor<STAID_LEN;ifor++)
    {
        if(app.iws_server[server_index].sid[ifor]!=staid[ifor]){
            break;
        }
    }
    if(ifor<7)
    {
        return -1;
    }
    else{
        return 0;
    }
}










int iws_read_check_RR_staid(const char * staid,int server_index)
{
    return 0;//TODO
    int ifor;
#define STAID_LEN 8
    for(ifor=0;ifor<STAID_LEN;ifor++)
    {
        if(app.iws_server[server_index].sid[ifor]!=staid[ifor]){
            break;
        }
    }
    if(ifor<7)
    {
        return -1;
    }
    else{
        return 0;
    }
}

//Task distribution

int iws_read_check_CC_TD_frame(const char * buf,int len,int server_index)
{
    int ifor;
    int ret;
    short cmd;
    cmd=ntohs(*(short *)(buf+14));
    int realwrite=0;
        printf(PURPLE"inside function %s line:%d,cmd=%d\n"NONE,__FUNCTION__,__LINE__,cmd);

    switch(cmd)
    {
#define TRANS_MODE 1
#define TIME_REQ 2
#define TRIGGER_THRESHOLD 3
// #define TRANS_MODE 4
// #define TRANS_MODE 5

        case TRANS_MODE:
        //IWS_CC_RE iws_cc_re_pak;//控制命令应答包
        memcpy(&(app.iws_cstp[server_index].iws_control_pak),buf,len);
        //realwrite=writenbytes(&iws_cstp_globe.iws_cc_re_pak,fid,IWS_CC_RE);
        break;
        case TIME_REQ:
        memcpy(&(app.iws_cstp[server_index].iws_timedata_request_pak),buf,len);
        //realwrite=writenbytes(&iws_cstp_globe.iws_cc_re_pak,fid,IWS_CC_RE);
        break;
        case TRIGGER_THRESHOLD:
        memcpy(&(app.iws_cstp[server_index].iws_trigger_threshold_pak),buf,len);
        app.iws_para.threshold=ntohs(app.iws_cstp[server_index].iws_trigger_threshold_pak.trigger_threshold)/100;
        printf("app.iws_para.threshold=%d\n",app.iws_para.threshold);
        //exit(0);
        //realwrite=writenbytes(&iws_cstp_globe.iws_cc_re_pak,fid,IWS_CC_RE);
        break;
        default:
        //memcpy(&iws_cstp_globe.iws_trigger_threshold_pak,buf,len);
        //realwrite=writenbytes(&iws_cstp_globe.iws_cc_re_pak,fid,IWS_CC_RE);
        break;

    }
    return cmd;

}






int iws_read_check_RR_frame(const char * buf,int len,int server_index)
{
    int ifor;
    int ret;
    short cmd;
    cmd=*(short *)(buf+14);
    int realwrite=0;
        printf(PURPLE"inside function %s line:%d,cmd=%d\n"NONE,__FUNCTION__,__LINE__,cmd);

    switch(cmd)
    {
#define REG_OK 0
#define REG_FAIL 1


        case REG_OK:
            //app.iws_server[server_index].
        break;
        case REG_FAIL:
        break;
        default:
        break;

    }
    return cmd;

}



int iws_read_check_CC(const char * buf,int len,int server_index)
{
    int ifor;
    int ret;
    printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    ret=iws_read_check_CC_pkid(*(int *)(buf+2),server_index);
    if(ret==0){
        printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        ret=iws_read_check_CC_staid(buf+6,server_index);
        if(ret==0){
                printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
            //memcpy(
            return iws_read_check_CC_TD_frame( buf,len,server_index);
        }
        return -1;
    }
    return -1;
}


int iws_read_check_RR(const char * buf,int len,int server_index)
{
    int ifor;
    int ret;
        printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

    ret=iws_read_check_RR_pkid(*(int *)(buf+2),server_index);
    //exit(0);
    if(ret==0){
            printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

        ret=iws_read_check_RR_staid(buf+6,server_index);
        if(ret==0){
                printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

            //memcpy(
            return iws_read_check_RR_frame( buf,len,server_index);
        }
        return -1;
    }
    return -1;
}







int mk_cannot_ctrl_pk(const char * buf,IWS_CC_RE * iws_cc_re_pak,int pak_len)
{
    printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    printf("get it\n");
    return 0;

}





//extern int osonewthread(pthread_t * newthread,int  (* threadfun)(void *),void * paras);


int get_time_data_ram(void *index)
{
    IWS_RQ_TIME *iws_rq_time =(IWS_RQ_TIME *)index;
    int ifor;
    FIR_LIST * list_data_start,*list_now;
    list_data_start=list_now=app.listnow;

    for(ifor=0;ifor<iws_rq_time->t_t;ifor++)
    {
        list_data_start=list_data_start->last;

    }

        printf(RED"YES = %d\n"NONE,iws_rq_time->timestart);
        printf(RED"app.app_count.second_count = %d\n"NONE,app.app_count.second_count);
        printf(RED"iws_rq_time->server_index = %d\n"NONE,iws_rq_time->server_index);
        printf(RED"iws_rq_time->t_t = %d\n"NONE,iws_rq_time->t_t);
        printf(RED"iws_rq_time->len = %llu\n"NONE,iws_rq_time->len);
       printf(RED"iws_rq_time->date_time.tv_sec = %d\n"NONE,iws_rq_time->date_time.tv_sec);
       printf(RED"list_now->count = %d\n"NONE,list_now->count);
       printf(RED"list_now->utc_second = %d\n"NONE,list_now->utc_second);
       printf(RED"list_data_start->count = %d\n"NONE,list_data_start->count);
       printf(RED"list_data_start->utc_second = %d\n"NONE,list_data_start->utc_second);

    return 0;
}

int check_pak_wave(IWS_UP_WAVEDATA *iws_up_wavedata)
{
    return 0;
}

int check_pak_wave_ext(IWS_UP_WAVEDATA_EXT * iws_up_wavedata_ext,IWS_RQ_TIME *iws_rq_time)
{
    //if()
    int is_start_valid,is_end_valid;
    if((iws_up_wavedata_ext->wcts[0]=='W')){
        if(iws_up_wavedata_ext->iws_ext_head.utc_time[0]>=iws_rq_time->timestart){
            is_start_valid=1;
        }
        else{
            is_start_valid=0;
        }
        if(iws_up_wavedata_ext->iws_ext_head.utc_time[0]<=iws_rq_time->timeend){
            is_end_valid=1;

        }
        else{
            is_end_valid=0;
        }
        //printf(GREEN"iws_up_wavedata_ext->iws_ext_head.utc_time[0]=%d\n,\
            iws_rq_time->timestart=%d\n,\
            iws_rq_time->timeend=%d\n"NONE,\
            iws_up_wavedata_ext->iws_ext_head.utc_time[0],\
            iws_rq_time->timestart,\
            iws_rq_time->timeend);
        if(is_start_valid&&is_end_valid){
            return 0;
        }
        else{
            return -1;
        }
        
    }
    else{
        return -100;//data error
    }
    
    
}


int sig_is_rq_data_ready=0;

IWS_UP_WAVEDATA_EXT * iws_up_wavedata_ext_globe;


int get_time_data_file(void * index)
{
    IWS_RQ_TIME *iws_rq_time =(IWS_RQ_TIME *)index;
    FILE * rq_fp;
    APP_S * app=(APP_S *)iws_rq_time->para;
    IWS_UP_WAVEDATA_EXT * iws_up_wavedata_ext=malloc(sizeof(IWS_UP_WAVEDATA_EXT));
    iws_up_wavedata_ext_globe=iws_up_wavedata_ext;
    int readcount=0,sendcount=0;
    int check_ret;
    int check_file_ret;
    do{
        rq_fp=fopen(app->iws_file_list[iws_rq_time->file_index].name,"rb");
        int readlen,realwrite;
        if(rq_fp==NULL){
            perror("rp_fp open error");
        }
        else{
            while(feof(rq_fp)==0){
                if(app->iws_server[iws_rq_time->server_index].sig_is_rq_data_ready==0){
                    readlen=myread((unsigned char *)iws_up_wavedata_ext,sizeof(IWS_UP_WAVEDATA_EXT),rq_fp);
                    readcount+=readlen;
                    //printf(BLUE"readcount=%d\n"NONE,readcount);
                    iws_up_wavedata_ext->wcts[1]='S';
                    if(readlen==sizeof(IWS_UP_WAVEDATA_EXT)){
                        check_ret=check_pak_wave_ext(iws_up_wavedata_ext,iws_rq_time);
                        if(check_ret==0){
                            app->iws_server[iws_rq_time->server_index].sig_is_rq_data_ready=1;
                            //realwrite=writenbytes((unsigned char *)&iws_up_wavedata_ext->wcts[0],\
                                app->iws_server[iws_rq_time->server_index].fid,\
                                sizeof(IWS_UP_WAVEDATA)); 
                            sendcount+=realwrite;
                            //printf(GREEN"sendcount=%d\n"NONE,sendcount);
                            
                        }
                        else if(check_ret==-1){
                            continue;
                        }
                        else if(check_ret==-100){
                            //iws_rq_time->isused=0;
                            //fclose(rq_fp);
                            break;
                        }


                    }
                    else{

                        //fclose(rq_fp);
                        break;
                        printf(RED"readlen=%d\n"NONE,readlen);
                        printf(RED"readcount=%d\n"NONE,readcount);
                        printf(GREEN"sendcount=%d\n"NONE,sendcount);
                    }
                }
                usleep(5);

            }
            fclose(rq_fp);
        }
        iws_rq_time->file_index++;
        if(app->iws_file_list[iws_rq_time->file_index].tv_sec<iws_rq_time->timeend\
            &&(app->iws_file_list[iws_rq_time->file_index].isused==1)\
            &&(app->iws_file_list[iws_rq_time->file_index].tv_sec!=0)){
            check_file_ret=1;
        }
        else{
            check_file_ret=0;
        }
        printf(GREEN"app->iws_file_list[iws_rq_time->file_index].name=%s\n,\
            app->iws_file_list[iws_rq_time->file_index].tv_sec=%d\n,\
            iws_rq_time->timeend=%d\n,\
            check_file_ret=%d\n"NONE,\
            app->iws_file_list[iws_rq_time->file_index].name,\
            app->iws_file_list[iws_rq_time->file_index].tv_sec,\
            iws_rq_time->timeend,\
            check_file_ret);
    }while(check_file_ret);
    iws_rq_time->isused=0;
    return 0;

    for(;;)
    {
        sleep(1);
        printf(GREEN"YES = %d\n"NONE,iws_rq_time->timestart);
        printf(GREEN"iws_rq_time->server_index = %d\n"NONE,iws_rq_time->server_index);
        printf(GREEN"iws_rq_time->len = %d\n"NONE,iws_rq_time->len);
        printf(GREEN"iws_rq_time->date_time.tv_sec = %d\n"NONE,iws_rq_time->date_time.tv_sec);

 
    }
    return 0;
}



int check_timerq(IWS_RQ_TIME *iws_rq_time)
{
    #define DATARAMLEN 10800
    unsigned int timed=iws_rq_time->date_time.tv_sec-iws_rq_time->timestart;
    printf(BLUE"timed=%d,\
        iws_rq_time->date_time.tv_sec=%d,\
        iws_rq_time->timestart=%d\n"NONE,\
        timed,iws_rq_time->date_time.tv_sec\
        ,iws_rq_time->timestart);
    if(timed<0)
        return -1;
    if(timed<DATARAMLEN){//内存
        return timed;
    }
    else{//文件
        return 0;
    }
}

int cstp_newthread(pthread_t * newthread,int  (* threadfun)(void *),void * paras)
{
    printf(GREEN"*paras=%d\n"NONE,index);
    int a=0;
    if ( pthread_create(newthread, NULL, threadfun,paras) ){
        printf("error creating gpsthread.");
        return -1;
    }
    printf(PURPLE"UDP DATA THREAD DONE\n"NONE); 
    return 0;
}



int file_Traversal(APP_S * app,int index)
{
    int ifor;
    int d_time;
    for(ifor=app->globe_steim2_file_index;ifor>=0;ifor--)
    {
        //printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

        if(app->iws_file_list[ifor].isused==1){
            //printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

            d_time=app->iws_file_list[ifor].tv_sec-app->iws_rq_time[index].timestart;
            printf(BLUE"ifor=%d,file:%s,function：%s,line:%d\n"NONE,ifor,__FILE__,__FUNCTION__,__LINE__);
            printf(RED"now check=%s,d_time=%d\n"NONE,\
                app->iws_file_list[ifor].name,\
                d_time);
            if(d_time<0){
                app->iws_rq_time[index].file_index=ifor;
                break;
            }
        }
        
        
    }
    if(ifor==0){
        return -1;
    }
    else{
        return ifor;
    }
      

}





int timedata_rq_process(const char * buf,int len,int server_index,APP_S * app)//(iws_client_buf,realread,server_index,app);
{
    pthread_t  newthread;
    int paras=server_index;
    int ifor;
    IWS_TIMEDATA_REQUEST *timerq=(IWS_TIMEDATA_REQUEST *)buf;
    printf(RED"timerq->type_lable[0]=%c,timerq->type_lable[1]=%c,timerq->time_mark=%d\n"NONE,\
        timerq->type_lable[0],timerq->type_lable[1],timerq->time_mark);
    int can_use_index;
    int is_ok=0;
    for(ifor=0;ifor<4;ifor++)
    {
        if(app->iws_rq_time[ifor].isused==0)
        {
            can_use_index=ifor;
            app->iws_rq_time[ifor].isused=1;
            app->iws_rq_time[ifor].para=app;
            app->iws_rq_time[ifor].server_index=server_index;
            app->iws_rq_time[ifor].timestart=htonl(timerq->time_mark);

            app->iws_rq_time[ifor].len=htonl(timerq->time_length);
            app->iws_rq_time[ifor].timeend=app->iws_rq_time[ifor].timestart+app->iws_rq_time[ifor].len;
            app->iws_rq_time[ifor].t_t=app->app_count.date_time.tv_sec-timerq->time_mark;
            app->iws_rq_time[ifor].date_time.tv_sec=app->app_count.date_time.tv_sec;
            is_ok=1;
            break;
        }
    }
    if(is_ok==0){//检查是否有可用资源
        return -1;
    }
    int ck_datalocation;
    ck_datalocation=check_timerq(&app->iws_rq_time[can_use_index]);//检查时间
    ck_datalocation=file_Traversal(app,can_use_index);
    printf(BLUE"ck_datalocation=%d\n"NONE,ck_datalocation);
    if(ck_datalocation==-1){
        return -2;
    }
    

    if(app->globe_steim2_file_index==ck_datalocation){
        printf(BLUE"DATA is in now write file;ck_datalocation=%d\n"NONE,ck_datalocation);
        return -2; 
    }
    else{
        printf(RED"app->iws_file_list[ifor].name=%s\n",app->iws_file_list[ck_datalocation].name);
        int ii=cstp_newthread(&newthread,get_time_data_file,(void *)&app->iws_rq_time[can_use_index]);
        printf(PURPLE"timerq:%d\n"NONE,timerq->time_mark);
        if(ii){
            return -3;
        }
        
    }

    return 0;

    if(ck_datalocation==0){//需要到文件中读取
        int ii=cstp_newthread(&newthread,get_time_data_file,(void *)&app->iws_rq_time[can_use_index]);
        printf(PURPLE"timerq:%d\n"NONE,timerq->time_mark);
        if(ii){
            return -3;
        }
        return 0;
    }
    else{//在内存中读取
        int ii=cstp_newthread(&newthread,get_time_data_ram,(void *)&app->iws_rq_time[can_use_index]);
        if(ii){
            return -3;
        }
        return 0;
    }
}










int debug_read(char * buf,int len,int type,void * para)
{

    int ifor;
    for(ifor=0;ifor<len;ifor++)
    {
        printf(GREEN"%d-[0x%02X] "NONE,ifor,buf[ifor],buf[ifor]);
    }
    printf("\n");
    return 0;

}






int debug_buf(char * p,int len,char * disp)
{
    //char *p=(char *)&app->iws_cstp[connect_index].iws_up_wavedata;
    printf(RED"%s\n"NONE,disp);
    int ifor;
    for(ifor=0;ifor<len;ifor++)
    {
        printf(LIGHT_RED"%d:%02X-%c "NONE,ifor,p[ifor],p[ifor]);
    }
     printf("\n");
     return 0;
}








#define WAVEMODE 1
#define TRIG_WAVE 2
#define TRIG_NOWAVE 3
update_point(int mode,int server_index)//改变模式时重新配置发送指针
{
    if((app.iws_server[server_index].protocol==SERVER_PROTOCOL_CSTP)&&(app.iws_server[server_index].version==SERVER_PROTOCOL_CSTP_VERSION_10)){
        switch(mode)
        {
            case WAVEMODE:
                
                //app.iws_server[server_index].sig_trig_wave=0;
                app.is_trig_start=0;
                app.iws_server[server_index].is_first_send=0;
                app.iws_server[server_index].is_trig_start=0;
                app.iws_server[server_index].is_trig_init=0;
                //app.iws_server[server_index].sig_trig_wave=0;
                app.iws_server[server_index].steim2_buf=app.buffer.steim2_out_buf[STEIM2_NOW];//回复连续波形模式，更新波形组包指针
            break;
            case TRIG_WAVE:
                app.is_trig_start=0;
                app.iws_server[server_index].is_trig_start=0;
                app.iws_server[server_index].is_trig_init=0;
                //app.is_trig_start=0;//TODO目前这个处理，可能造成切换瞬间错过触发
                //app.is_trig_start=0;
                //app.iws_server[server_index].sig_trig_wave=0;
                app.iws_server[server_index].sig=0;//清空连续波形时未发送缓存
                printf(RED"connect app.buffer.steim2_out_buf[STEIM2_NOW]->utc_time[0]=%d\n"NONE,app.buffer.steim2_out_buf[STEIM2_NOW]->utc_time[0]);
                app.iws_server[server_index].status_send_buf=app.buffer.status_out_buf[STATUS_NOW];//状态信息包指针更新
            break;
            case TRIG_NOWAVE:
                app.is_trig_start=0;//TODO目前这个处理，可能造成切换瞬间错过触发
                app.iws_server[server_index].is_trig_start=0;
                //app.is_trig_start=0;
                //app.iws_server[server_index].sig_trig_wave=0;
                app.iws_server[server_index].sig=0;//清空连续波形时未发送缓存
                printf(RED"connect app.buffer.steim2_out_buf[STEIM2_NOW]->utc_time[0]=%d\n"NONE,app.buffer.steim2_out_buf[STEIM2_NOW]->utc_time[0]);
                app.iws_server[server_index].status_send_buf=app.buffer.status_out_buf[STATUS_NOW];//连接成功,初始化发送之争
            break;
            default:
            break;
        }

    }
    else{
        switch(mode)
        {
            case WAVEMODE:
                
                //app.iws_server[server_index].sig_trig_wave=0;
                app.is_trig_start=0;
                init_psa_pga();

                app.iws_server[server_index].is_trig_start=0;
                app.iws_server[server_index].is_trig_init=0;
                //app.iws_server[server_index].sig_trig_wave=0;
                app.iws_server[server_index].iws_pak_buf.wave_buf_index.now=\
                app.app_sig.sig_wave_data_write_buf_total;
                break;
            case TRIG_WAVE:
                app.is_trig_start=0;
                init_psa_pga();

                //app.is_trig_start=0;//TODO目前这个处理，可能造成切换瞬间错过触发
                //app.is_trig_start=0;
                //app.iws_server[server_index].sig_trig_wave=0;
                app.iws_server[server_index].sig=0;//清空连续波形时未发送缓存
                printf(RED"connect app.buffer.steim2_out_buf[STEIM2_NOW]->utc_time[0]=%d\n"NONE,app.buffer.steim2_out_buf[STEIM2_NOW]->utc_time[0]);
                app.iws_server[server_index].status_send_buf=app.buffer.status_out_buf[STATUS_NOW];//状态信息包指针更新
            break;
            case TRIG_NOWAVE:
                init_psa_pga();

                app.is_trig_start=0;//TODO目前这个处理，可能造成切换瞬间错过触发
                app.iws_server[server_index].is_trig_start=0;
                //app.is_trig_start=0;
                //app.iws_server[server_index].sig_trig_wave=0;
                app.iws_server[server_index].sig=0;//清空连续波形时未发送缓存
                printf(RED"connect app.buffer.steim2_out_buf[STEIM2_NOW]->utc_time[0]=%d\n"NONE,app.buffer.steim2_out_buf[STEIM2_NOW]->utc_time[0]);
                app.iws_server[server_index].status_send_buf=app.buffer.status_out_buf[STATUS_NOW];//连接成功,初始化发送之争
            break;
            default:
            break;
        }


    }
    return 0;
}


int iws_read_process(int fid,int server_index,APP_S * app)
{
    printf(LIGHT_GRAY"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    char buf[1024];
    int realread=0,realwrite=0;
    int ret;
    //realread=read(fid,buf,1024);
    //iws_client_buf
    //#define CSTP_DOWN_PAKLEN
    realread+=readnbytes(iws_client_buf,fid,CSTP_DOWN_PAKLEN);
    int ifor;
    if(realread<1){
        return realread;
    }
        
    for(ifor=0;ifor<realread;ifor++){
        printf("%X ",iws_client_buf[ifor]);
    }
    printf("\n");
    printf(LIGHT_GRAY"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    ret=iws_read_pre_check(iws_client_buf,CSTP_DOWN_PAKLEN);//
    switch(ret)
    {
        case 0://控制信息包
            if(0/*app->iws_server[server_index].status.iscan_control!=1*/){//无控制权
                printf(LIGHT_GRAY"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

                mk_cannot_ctrl_pk(iws_client_buf,&(app->iws_cstp[server_index].iws_cc_re_pak),CSTP_DOWN_PAKLEN);
                realwrite=writenbytes(&(app->iws_cstp[server_index].iws_cc_re_pak),fid,sizeof(IWS_CC_RE));
                for(ifor=0;ifor<realread;ifor++){
                    printf("%0X-%c ",iws_client_buf[ifor],iws_client_buf[ifor]);
                }
                printf("\n");
                //exit(0);
                return realwrite;//
            }
            printf(LIGHT_GRAY"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
            switch (iws_read_check_CC(iws_client_buf,CSTP_DOWN_PAKLEN,server_index))
            {
                case TRANS_MODE:
                printf(LIGHT_GRAY"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

                //IWS_CC_RE iws_cc_re_pak;//控制命令应答包
                //memcpy(&iws_cstp_globe.iws_control_pak,buf,len);
                if(0==ntohs(app->iws_cstp[server_index].iws_control_pak.trans_mode))
                {
                    break;
                }
                app->iws_server[server_index].last_mode=app->iws_server[server_index].mode;
                app->iws_server[server_index].mode=ntohs(app->iws_cstp[server_index].iws_control_pak.trans_mode);
                if(app->iws_server[server_index].mode!=app->iws_server[server_index].last_mode){
                    update_point(app->iws_server[server_index].mode,server_index);
                }
                app->iws_cstp[server_index].iws_cc_re_pak.package_number=htonl(app->iws_cstp[server_index].server_pak_num);
                realwrite=writenbytes(&(app->iws_cstp[server_index].iws_cc_re_pak),fid,sizeof(IWS_CC_RE));
                //exit(0);
                break;
                case TIME_REQ:
                printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
                timedata_rq_process(iws_client_buf,realread,server_index,app);
                //memcpy(&iws_cstp_globe.iws_timedata_request_pak,buf,len);
                app->iws_cstp[server_index].iws_cc_re_pak.package_number=app->iws_cstp[server_index].server_pak_num;
                realwrite=writenbytes(&(app->iws_cstp[server_index].iws_cc_re_pak),fid,sizeof(IWS_CC_RE));
                break;
                case TRIGGER_THRESHOLD:
                    printf(LIGHT_GRAY"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

                //memcpy(&iws_cstp_globe.iws_trigger_threshold_pak,buf,len);
                app->iws_cstp[server_index].iws_cc_re_pak.package_number=app->iws_cstp[server_index].server_pak_num;
                realwrite=writenbytes(&(app->iws_cstp[server_index].iws_cc_re_pak),fid,sizeof(IWS_CC_RE));
                break;
                default:
                //memcpy(&iws_cstp_globe.iws_trigger_threshold_pak,buf,len);
                app->iws_cstp[server_index].iws_cc_re_pak.package_number=app->iws_cstp[server_index].server_pak_num;
                realwrite=writenbytes(&(app->iws_cstp[server_index].iws_cc_re_pak),fid,sizeof(IWS_CC_RE));
                break;
            }
        break;
        case 1://注册信息相应包
            iws_read_check_RR(iws_client_buf,CSTP_DOWN_PAKLEN,server_index);
        break;
        case -1://无法识别的包
            for(ifor=0;ifor<realread-1;ifor++)//查找接收是否错位
            {
                if((iws_client_buf[ifor]=='C'&&iws_client_buf[ifor+1]=='C')||(iws_client_buf[ifor]=='R'&&iws_client_buf[ifor+1]=='R')){
                    realread+=readnbytes(iws_client_buf+CSTP_DOWN_PAKLEN,fid,ifor);
                    break;
                }
            }
        break;
    }
    // else{
    //     realread+=readnbytes(iws_client_buf+CSTP_DOWN_PAKLEN,fid,ret);
    //     return realread;//读包错位错误,已经纠正
    // }
    printf("\n");
    return realread;
}


IWS_UP_WAVEDATA test_wave_data[6];
//IWS_UP_TI ti_pak[6];

int test_wave_data_init()
{
    /*
typedef struct IWS_UP_WAVEDATA{//触发信息包
    char wcts[2];//wc ,wt,ws
    int package_number;
    char qi;//quality index.D
    char reserve;
    char sta_idx[5];
    char pos_idx[2];
    char ch_idx[3];
    char net_idx[2];
    char data_start_utc[10];
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
    */
    int ifor;
    for(ifor=0;ifor<6;ifor++)
    {
        test_wave_data[ifor].wcts[0]='w';
        test_wave_data[ifor].wcts[1]='c';
        test_wave_data[ifor].package_number=0;
        test_wave_data[ifor].sta_idx[0]='j';
        test_wave_data[ifor].sta_idx[1]='d';
        test_wave_data[ifor].sta_idx[2]='.';
        test_wave_data[ifor].sta_idx[3]='0';
        test_wave_data[ifor].sta_idx[4]='0';
        test_wave_data[ifor].sta_idx[5]='0';
        test_wave_data[ifor].sta_idx[6]='0';
        test_wave_data[ifor].sta_idx[7]='1';
        test_wave_data[ifor].sf=171;
        test_wave_data[ifor].qi='D';


        //ti_pak[ifor].

    }

}




int htonl_wave_data(int * data)
{
    int ifor;
    for(ifor=0;ifor<48;ifor++){
        data[ifor]=htonl(data[ifor]);
    }
    return 0;
}


int htonl_wave_utc(iws_utc_10 *data_start_utc)
{
    data_start_utc->year=htons(data_start_utc->year);
    data_start_utc->yday=htons(data_start_utc->yday);
    data_start_utc->u_sec=htons(data_start_utc->u_sec);
    //u_sec
    return 0;
}



int tim_flag=0;

int mk_wave_pak(int connect_index)
{
        if(tim_flag<3){//发送首个波形包打开始标志
            tim_flag++;
            app.iws_cstp[connect_index].iws_up_wavedata.iotm=8;
            app.iws_cstp[connect_index].iws_up_wavedata.iotm=0x28;
        }
        else{
            tim_flag=3;
            app.iws_cstp[connect_index].iws_up_wavedata.iotm=0;
            app.iws_cstp[connect_index].iws_up_wavedata.iotm=0x20;
        }
        //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        //printf(GREEN"app->iws_server[connect_index].steim2_buf=%d\n"NONE,app.iws_server[connect_index].steim2_buf);
        memcpy(&app.iws_cstp[connect_index].iws_up_wavedata.data[0],&app.iws_server[connect_index].steim2_buf->out[0],192);
        app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.u_sec=(unsigned int)(app.iws_server[connect_index].steim2_buf->utc_time[1]/100000);


        struct tm  *mytm2= localtime(&app.iws_server[connect_index].steim2_buf->utc_time[0]);
        app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.year=mytm2->tm_year+1900;
        app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.yday=mytm2->tm_yday+1;
        app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.hour=mytm2->tm_hour;
        app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.min=mytm2->tm_min;
        app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.sec=mytm2->tm_sec;
// printf("my_iws_utc.year=%d\n",app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.year);
// printf("my_iws_utc.yday=%d\n",app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.yday);
// printf("my_iws_utc.hour=%d\n",app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.hour);
// printf("my_iws_utc.min=%d\n",app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.min);
// printf("my_iws_utc.sec=%d\n",app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.sec);

        app.iws_cstp[connect_index].iws_up_wavedata.datacount=app.iws_server[connect_index].steim2_buf->sample_Num-1;
        app.iws_cstp[connect_index].pak_num=app.iws_cstp[connect_index].pak_num+1;
        app.iws_cstp[connect_index].iws_up_wavedata.package_number=app.iws_cstp[connect_index].pak_num;//包序号维护
        printf(BLUE"app.iws_cstp[connect_index].iws_up_wavedata.package_number=%d\n"NONE,app.iws_cstp[connect_index].iws_up_wavedata.package_number);
        
        app.iws_cstp[connect_index].iws_up_wavedata.fs_factor=app.app_para.fs;
        //printf(LIGHT_CYAN"app->iws_server[connect_index].steim2_buf->utc_time[0]=%d,utc_time[1]=%d,u_sec=%d,ch_num=%d,pak_num=%d\n"NONE,app.iws_server[connect_index].steim2_buf->utc_time[0],app.iws_server[connect_index].steim2_buf->utc_time[1],\
        app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.u_sec,app.iws_server[connect_index].steim2_buf->ch_num,app.iws_cstp[connect_index].iws_up_wavedata.package_number=app.iws_cstp[connect_index].pak_num);

        switch(app.iws_server[connect_index].steim2_buf->ch_num){//打通道标记
            case 0:
            app.iws_cstp[connect_index].iws_up_wavedata.ch_idx[2]='N';
            //app.iws_cstp[connect_index].iws_up_wavedata.iotm=0x20;
            break;
            case 1:
            app.iws_cstp[connect_index].iws_up_wavedata.ch_idx[2]='E';
            //app.iws_cstp[connect_index].iws_up_wavedata.iotm=0;
            break;
            case 2:
            app.iws_cstp[connect_index].iws_up_wavedata.ch_idx[2]='Z';
            //app.iws_cstp[connect_index].iws_up_wavedata.iotm=0;
            break;
        }
        //app.iws_cstp[connect_index].iws_up_wavedata.ch_idx[2]=0x30+app.iws_server[connect_index].steim2_buf->ch_num;
        //printf(RED"steim2_buf=%d\n"NONE,app.iws_server[connect_index].steim2_buf);
        htonl_wave_head(&app.iws_cstp[connect_index].iws_up_wavedata);//头部大小端转换
        htonl_wave_data((int *)&app.iws_cstp[connect_index].iws_up_wavedata.data[0]);//数据大小端转换
        htonl_wave_utc(&app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc);//时标大小端转换
    //test_wave_data[connect_index].datacount=app.iws_server[connect_index].steim2_buf->sample_Num;
        //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

    return 0;
}

int debut_print_iws_utc(iws_utc_10 * iws_up_wavedata)
{
    printf("iws_utc.year=%d\n",iws_up_wavedata->year);
    printf("iws_utc.yday=%d\n",iws_up_wavedata->yday);
    printf("iws_utc.hour=%d\n",iws_up_wavedata->hour);
    printf("iws_utc.min=%d\n",iws_up_wavedata->min);
    printf("iws_utc.sec=%d\n\n\n",iws_up_wavedata->sec);
    return 0;
}

int debug_print_wavedata_time(IWS_UP_WAVEDATA * iws_up_wavedata)
{
    struct timespec date_time;//当前时间
    printf(RED"IN pak time:\n"NONE);
    debut_print_iws_utc(&iws_up_wavedata->data_start_utc);
    clock_gettime(CLOCK_REALTIME, &date_time);
    iws_utc_10 iws_utc_now;
    struct tm  *mytm2= localtime(&(date_time.tv_sec));
    iws_utc_now.year=mytm2->tm_year+1900;
    iws_utc_now.yday=mytm2->tm_yday+1;
    iws_utc_now.hour=mytm2->tm_hour;
    iws_utc_now.min=mytm2->tm_min;
    iws_utc_now.sec=mytm2->tm_sec;
    printf(GREEN"NOW time:\n"NONE);
    printf(GREEN"NOW SEND time date_time.tv_sec:%d\n"NONE,date_time.tv_nsec);
    debut_print_iws_utc(&iws_utc_now);
    return 0;
}



////app.iws_up_ti[app->app_sig.sig_trig_ti_write_buf]

int steim2_mk_ext_wave_pak(IWS_UP_WAVEDATA_EXT * iws_up_wavedata_ext,IWS_STEIM2_OUT * steim2_buf,int pak_num)
{

    steim2_mk_wave_pak(&(iws_up_wavedata_ext->wcts[0]),steim2_buf,pak_num);
    iws_up_wavedata_ext->iws_ext_head.utc_time[0]=steim2_buf->utc_time[0];
    iws_up_wavedata_ext->iws_ext_head.utc_time[1]=steim2_buf->utc_time[1];
    //printf(BLUE"iws_up_wavedata_ext->wcts[0]=%c\n"NONE,iws_up_wavedata_ext->wcts[0]);
    return 0;
}


int copy_steim2_data_2_send_buf(IWS_UP_WAVEDATA * iws_up_wavedata,IWS_UP_WAVEDATA_EXT *iws_up_wavedata_ext)
{
    memcpy(&iws_up_wavedata->wcts[0],&iws_up_wavedata_ext->wcts[0],sizeof(IWS_UP_WAVEDATA));
    iws_up_wavedata->wcts[1]='C';
    //printf(BLUE"iws_up_wavedata_ext->ch_idx[2]=%c\n"NONE,iws_up_wavedata_ext->ch_idx[2]);

    //printf(RED"iws_up_wavedata->>wcts[0]=%c\n"NONE,iws_up_wavedata->wcts[0]);
    return 0;
}







////app.iws_up_ti[app->app_sig.sig_trig_ti_write_buf]

int steim2_mk_wave_pak(IWS_UP_WAVEDATA * iws_up_wavedata,IWS_STEIM2_OUT * steim2_buf,int pak_num)
{
        // if(tim_flag<3){//发送首个波形包打开始标志
        //     tim_flag++;
        //     app.iws_cstp[connect_index].iws_up_wavedata.iotm=8;
        //     app.iws_cstp[connect_index].iws_up_wavedata.iotm=0x28;
        // }
        // else{
        //     tim_flag=3;
        //     app.iws_cstp[connect_index].iws_up_wavedata.iotm=0;
        //     app.iws_cstp[connect_index].iws_up_wavedata.iotm=0x20;
        // }
        //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        //printf(GREEN"app->iws_server[connect_index].steim2_buf=%d\n"NONE,app.iws_server[connect_index].steim2_buf);
        
        memcpy(&iws_up_wavedata->data[0],&steim2_buf->out[0],192);
        iws_up_wavedata->wcts[1]='C';
        iws_up_wavedata->data_start_utc.u_sec=(unsigned int)(steim2_buf->utc_time[1]/100000);
        if(app.iws_server_share.status.is_clock_sync_ntp==1){
            iws_up_wavedata->iotm=0x20;
        }
        else{
            iws_up_wavedata->iotm=0x00;
        }
        

        struct tm  *mytm2= localtime(&(steim2_buf->utc_time[0]));
        iws_up_wavedata->data_start_utc.year=mytm2->tm_year+1900;
        iws_up_wavedata->data_start_utc.yday=mytm2->tm_yday+1;
        iws_up_wavedata->data_start_utc.hour=mytm2->tm_hour;
        iws_up_wavedata->data_start_utc.min=mytm2->tm_min;
        iws_up_wavedata->data_start_utc.sec=mytm2->tm_sec;
        //debug_print_wavedata_time(iws_up_wavedata);
        iws_up_wavedata->datacount=steim2_buf->sample_Num;
        //app.iws_cstp[connect_index].pak_num=app.iws_cstp[connect_index].pak_num+1;
        iws_up_wavedata->package_number=pak_num;//包序号维护
        //printf(BLUE"app.iws_cstp[connect_index].iws_up_wavedata.package_number=%d\n"NONE,app.iws_cstp[connect_index].iws_up_wavedata.package_number);
        
        iws_up_wavedata->fs_factor=app.app_para.fs;
        //printf(LIGHT_CYAN"app->iws_server[connect_index].steim2_buf->utc_time[0]=%d,utc_time[1]=%d,u_sec=%d,ch_num=%d,pak_num=%d\n"NONE,app.iws_server[connect_index].steim2_buf->utc_time[0],app.iws_server[connect_index].steim2_buf->utc_time[1],\
        app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.u_sec,app.iws_server[connect_index].steim2_buf->ch_num,app.iws_cstp[connect_index].iws_up_wavedata.package_number=app.iws_cstp[connect_index].pak_num);

        switch(steim2_buf->ch_num){//打通道标记
            case 0:
            iws_up_wavedata->ch_idx[2]='N';
            //app.iws_cstp[connect_index].iws_up_wavedata.iotm=0x20;
            break;
            case 1:
            iws_up_wavedata->ch_idx[2]='E';
            //app.iws_cstp[connect_index].iws_up_wavedata.iotm=0;
            break;
            case 2:
            iws_up_wavedata->ch_idx[2]='Z';
            //app.iws_cstp[connect_index].iws_up_wavedata.iotm=0;
            break;
        }
        //app.iws_cstp[connect_index].iws_up_wavedata.ch_idx[2]=0x30+app.iws_server[connect_index].steim2_buf->ch_num;
        //printf(RED"steim2_buf=%d\n"NONE,app.iws_server[connect_index].steim2_buf);
        htonl_wave_head(iws_up_wavedata);//头部大小端转换
        htonl_wave_data((int *)&(iws_up_wavedata->data[0]));//数据大小端转换
        htonl_wave_utc(&(iws_up_wavedata->data_start_utc));//时标大小端转换
    //test_wave_data[connect_index].datacount=app.iws_server[connect_index].steim2_buf->sample_Num;
        //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

    return 0;
}





















int steim2_mk_trig_ti_pak(IWS_UP_TI * iws_up_ti,int pak_num)
{

    //iws_up_ti[app->app_sig.sig_trig_ti_write_buf]
    memcpy(iws_up_ti,&app.iws_up_ti[app.app_sig.sig_trig_ti_send_buf%1000],256);
    //iws_up_ti->wcts[1]='S';

    iws_up_ti->package_number=pak_num;
    //iws_up_ti->wcts[1]='T';
    app.app_sig.sig_trig_ti_send_buf++;
    app.app_count.before_trig=app.app_count.resample_times-app.app_count.last_trig;
    app.app_count.end_trig=app.app_count.resample_times-app.app_count.trig_start;
    //printf("aa=%d,bb=%d,app.app_count.last_trig=%d,app.app_count.trig_start=%d\n",aa,bb,app.app_count.last_trig,app.app_count.trig_start);
    // if((app.app_count.before_trig>(30*app.app_para.fs))\
    //     &&(app.app_count.end_trig>(60*app.app_para.fs))\
    //     ){
    //     //app.is_trig_start_ti=0;//结束一次触发
    //     //init_psa_pga();
    // }
    //printf("aa=%d,bb=%d,app.app_count.last_trig=%d,app.app_count.trig_start=%d\n",aa,bb,app.app_count.last_trig,app.app_count.trig_start);
    htonl_ti(iws_up_ti);//统一做大小端转换
    return 0;
}





int steim2_evt_trig_pak(IWS_UP_WAVEDATA * iws_up_wavedata,int connect_index, int pak_num)
{
//app->iws_server[connect_index].sig_trig_wave_data_send_buf

    memcpy(iws_up_wavedata,&(app.iws_wave_pak_ext[app.app_sig.sig_wave_data_send_buf%1000].wcts[0]),256);
    iws_up_wavedata->package_number=htonl(pak_num);
    //app.iws_server[connect_index].sig_trig_wave_data_send_buf++;
    app.app_sig.sig_wave_data_send_buf++;

    //sig_trig_wave_data_send_buf
    // int aa=app.app_count.resample_times-app.app_count.last_trig;
    // int bb=app.app_count.resample_times-app.app_count.trig_start;
   // //printf("aa=%d,bb=%d,app.app_count.last_trig=%d,app.app_count.trig_start=%d\n",aa,bb,app.app_count.last_trig,app.app_count.trig_start);
   //  if((aa>(30*app.app_para.fs))\
   //      &&(bb>(60*app.app_para.fs))\
   //      &&('Z'==iws_up_wavedata->ch_idx[2])
   //      ){
   //      app.iws_server[connect_index].is_trig_start=0;//结束一次触发
   //  }
    return 0;
}

















int steim2_mk_trig_pak(IWS_UP_WAVEDATA * iws_up_wavedata,int connect_index, int pak_num)
{
//app->iws_server[connect_index].sig_trig_wave_data_send_buf

    memcpy(iws_up_wavedata,&(app.iws_wave_pak_ext[app.iws_server[connect_index].sig_trig_wave_data_send_buf%IWS_UP_WAVEDATA_EXT_NUM].wcts[0]),256);
    iws_up_wavedata->package_number=htonl(pak_num);
    app.iws_server[connect_index].sig_trig_wave_data_send_buf++;



    int aa=app.app_count.resample_times-app.app_count.last_trig;
    int bb=app.app_count.resample_times-app.app_count.trig_start;
   //printf("aa=%d,bb=%d,app.app_count.last_trig=%d,app.app_count.trig_start=%d\n",aa,bb,app.app_count.last_trig,app.app_count.trig_start);
    if((aa>(30*app.app_para.fs))\
        &&(bb>(60*app.app_para.fs))\
        &&('Z'==iws_up_wavedata->ch_idx[2])
        ){
        app.iws_server[connect_index].is_trig_start=0;//结束一次触发
        //init_psa_pga();
    }




    //if(app->is_trig_start==0){//开启触发处理过程
    //app->app_sig.sig_wave_data_send_buf=(app->app_sig.sig_wave_data_write_buf+860)%1000;
    
        // if(tim_flag<3){//发送首个波形包打开始标志
        //     tim_flag++;
        //     app.iws_cstp[connect_index].iws_up_wavedata.iotm=8;
        //     app.iws_cstp[connect_index].iws_up_wavedata.iotm=0x28;
        // }
        // else{
        //     tim_flag=3;
        //     app.iws_cstp[connect_index].iws_up_wavedata.iotm=0;
        //     app.iws_cstp[connect_index].iws_up_wavedata.iotm=0x20;
        // }
        //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        //printf(GREEN"app->iws_server[connect_index].steim2_buf=%d\n"NONE,app.iws_server[connect_index].steim2_buf);
//         memcpy(&iws_up_wavedata->data[0],&steim2_buf->out[0],192);
//         iws_up_wavedata->data_start_utc.u_sec=(unsigned int)(steim2_buf->utc_time[1]/100000);


//         struct tm  *mytm2= localtime(&(steim2_buf->utc_time[0]));
//         iws_up_wavedata->data_start_utc.year=mytm2->tm_year+1900;
//         iws_up_wavedata->data_start_utc.yday=mytm2->tm_yday+1;
//         iws_up_wavedata->data_start_utc.hour=mytm2->tm_hour;
//         iws_up_wavedata->data_start_utc.min=mytm2->tm_min;
//         iws_up_wavedata->data_start_utc.sec=mytm2->tm_sec;
// // printf("my_iws_utc.year=%d\n",app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.year);
// // printf("my_iws_utc.yday=%d\n",app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.yday);
// // printf("my_iws_utc.hour=%d\n",app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.hour);
// // printf("my_iws_utc.min=%d\n",app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.min);
// // printf("my_iws_utc.sec=%d\n",app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.sec);

//         iws_up_wavedata->datacount=steim2_buf->sample_Num-1;
//         //app.iws_cstp[connect_index].pak_num=app.iws_cstp[connect_index].pak_num+1;
//         iws_up_wavedata->package_number=++pak_num;//包序号维护
//         //printf(BLUE"app.iws_cstp[connect_index].iws_up_wavedata.package_number=%d\n"NONE,app.iws_cstp[connect_index].iws_up_wavedata.package_number);
        
//         iws_up_wavedata->fs_factor=app.app_para.fs;
//         //printf(LIGHT_CYAN"app->iws_server[connect_index].steim2_buf->utc_time[0]=%d,utc_time[1]=%d,u_sec=%d,ch_num=%d,pak_num=%d\n"NONE,app.iws_server[connect_index].steim2_buf->utc_time[0],app.iws_server[connect_index].steim2_buf->utc_time[1],\
//         app.iws_cstp[connect_index].iws_up_wavedata.data_start_utc.u_sec,app.iws_server[connect_index].steim2_buf->ch_num,app.iws_cstp[connect_index].iws_up_wavedata.package_number=app.iws_cstp[connect_index].pak_num);

//         switch(steim2_buf->ch_num){//打通道标记
//             case 0:
//             iws_up_wavedata->ch_idx[2]='N';
//             //app.iws_cstp[connect_index].iws_up_wavedata.iotm=0x20;
//             break;
//             case 1:
//             iws_up_wavedata->ch_idx[2]='E';
//             //app.iws_cstp[connect_index].iws_up_wavedata.iotm=0;
//             break;
//             case 2:
//             iws_up_wavedata->ch_idx[2]='Z';
//             //app.iws_cstp[connect_index].iws_up_wavedata.iotm=0;
//             break;
//         }
//         //app.iws_cstp[connect_index].iws_up_wavedata.ch_idx[2]=0x30+app.iws_server[connect_index].steim2_buf->ch_num;
//         //printf(RED"steim2_buf=%d\n"NONE,app.iws_server[connect_index].steim2_buf);
//         htonl_wave_head(iws_up_wavedata);//头部大小端转换
//         htonl_wave_data((int *)&(iws_up_wavedata->data[0]));//数据大小端转换
//         htonl_wave_utc(&(iws_up_wavedata->data_start_utc));//时标大小端转换
//     //test_wave_data[connect_index].datacount=app.iws_server[connect_index].steim2_buf->sample_Num;
//         //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

    return 0;
}




















int mk_si_pak(int connect_index)//状态信息组包
{
        printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

    //app.iws_server[connect_index].
        int jfor;
        for(jfor=0;jfor<10;jfor++)
        {
            app.iws_cstp[connect_index].iws_up_si_pak.ud[jfor]=app.iws_server[connect_index].status_send_buf->ud[jfor];
            app.iws_cstp[connect_index].iws_up_si_pak.ew[jfor]=app.iws_server[connect_index].status_send_buf->ew[jfor];
            app.iws_cstp[connect_index].iws_up_si_pak.ns[jfor]=app.iws_server[connect_index].status_send_buf->ns[jfor];

        }
        //memcpy(app.iws_cstp[connect_index].iws_up_si_pak,&app.iws_server[connect_index].steim2_buf->out[0],192);
        //memcpy(&app.iws_cstp[connect_index].iws_up_wavedata.data[0],&app.iws_server[connect_index].steim2_buf->out[0],192);
        //steim2_out_buf
        //app.iws_cstp[connect_index].iws_up_wavedata.datacount=app.iws_server[connect_index].steim2_buf->sample_Num;
        app.iws_cstp[connect_index].iws_up_si_pak.package_number=++app.iws_cstp[connect_index].pak_num;
        app.iws_cstp[connect_index].iws_up_si_pak.ab_time=app.iws_server[connect_index].status_send_buf->utc_time[0];
        app.iws_cstp[connect_index].iws_up_si_pak.time=10;//app.iws_server[connect_index].status_send_buf->utc_time[0];
        printf(PURPLE"mk_si_pak app.iws_server[connect_index].status_send_buf=%d\n"NONE,app.iws_server[connect_index].status_send_buf);
        printf("app.iws_server[connect_index].status_send_buf->utc_time[0]=%d\n",app.iws_server[connect_index].status_send_buf->utc_time[0]);
        htonl_si(&app.iws_cstp[connect_index].iws_up_si_pak);
        //app->iws_server[connect_index].status_send_buf
        //htonl_wave1(&app.iws_cstp[connect_index].iws_up_wavedata);
    return 0;
}








int is_first_send=0;

int get_start_buf(int connect_index)
{

    if((app.iws_server[connect_index].protocol==SERVER_PROTOCOL_CSTP)&&(app.iws_server[connect_index].version==SERVER_PROTOCOL_CSTP_VERSION_10)){
        int ifor;
        pthread_mutex_lock(&app.app_mut[9]);
            app.iws_server[connect_index].steim2_buf=app.buffer.steim2_out_buf[STEIM2_NOW];//连接成功,初始化发送之争
            //printf(BLUE"connect_index=%d,app->iws_server[connect_index].steim2_buf.index=%d,sig=%d\n"NONE,connect_index,app->iws_server[connect_index].steim2_buf->index,app->iws_server[connect_index].sig);
        pthread_mutex_unlock(&app.app_mut[9]);
        for(ifor=0;ifor<app.iws_server[connect_index].sig;ifor++)
        {
            app.iws_server[connect_index].steim2_buf=app.iws_server[connect_index].steim2_buf->prev;//连接成功,初始化发送之争
        }
        printf(BLUE"connect_index=%d,\
            app->iws_server[connect_index].steim2_buf.index=%d,\
            sig=%d,\
            app->iws_server[connect_index].is_first_send=%d\
            \n"NONE,\
            connect_index,\
            app.iws_server[connect_index].steim2_buf->index,\
            app.iws_server[connect_index].sig,\
            app.iws_server[connect_index].is_first_send
            );
        return 0;
    }
    else{
        app.iws_server[connect_index].iws_pak_buf.wave_buf_index.now=\
        app.app_sig.sig_wave_data_write_buf_total;
    }


}





int mk_trig_ti_check(IWS_UP_TI * iws_up_ti,int server_index)
{
        int ifor;
        for(ifor=0;ifor<8;ifor++)
        {
             iws_up_ti->sta_id[ifor]=app.iws_server[server_index].sid[ifor];   
        }
        return 0;
}


int mk_wave_data_check(IWS_UP_WAVEDATA * iws_up_wavedata,int server_index)
{
    iws_up_wavedata->wcts[1]='T';
    iws_up_wavedata->net_idx[0]=app.iws_server[server_index].sid[0];
    iws_up_wavedata->net_idx[1]=app.iws_server[server_index].sid[1];
    int ifor;
    for(ifor=0;ifor<5;ifor++)
    {
         iws_up_wavedata->sta_idx[ifor]=app.iws_server[server_index].sid[ifor+3];   
    }
    return 0;
}


int init_trig(APP_S * app,int connect_index)
{

    return 0;
}



int check_event_continue(APP_S * app,int connect_index)
{
    int is_data_intime=app->iws_server[connect_index].trig_endtime-app->iws_wave_pak_ext[app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now%IWS_UP_WAVEDATA_EXT_NUM].iws_ext_head.utc_time[0];
    printf(LIGHT_RED"is_data_intime=%d\n"NONE,is_data_intime);
    if(is_data_intime>=0){
        return 1;
    }
    else{
        return 0;
    }
}



int end_event(APP_S * app,int connect_index)
{
    app->iws_server[connect_index].is_trig_init=0;
}
int check_is_trig_buf_ready(APP_S * app,int connect_index)
{
    int ifor;
    int d_t;
    if(app->iws_server[connect_index].is_trig_init==0){
        for(ifor=app->app_sig.sig_wave_data_write_buf_total;ifor>0;ifor--)
        {
            d_t=app->iws_wave_pak_ext[ifor%IWS_UP_WAVEDATA_EXT_NUM].iws_ext_head.utc_time[0]-\
                app->iws_server[connect_index].trig_starttime;
            if(d_t==-1){

                app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now=ifor;
                app->iws_server[connect_index].is_trig_init=1;
                printf(RED"app->iws_server[connect_index].trig_starttime=%d\n\
                    ifor=%d\n\
                    "NONE,\
                    app->iws_server[connect_index].trig_starttime,\
                    ifor);
                printf(BLUE"app->app_sig.sig_wave_data_write_buf_total=%d\n,\
                    app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index=%d\n,\
                    totol.iws_ext_head.utc_time[0]=%d\n,\
                    tirg.iws_ext_head.utc_time[0]=%d\n"NONE,\
                    app->app_sig.sig_wave_data_write_buf_total,\
                    app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now,\
                    app->iws_wave_pak_ext[(app->app_sig.sig_wave_data_write_buf_total-1)%IWS_UP_WAVEDATA_EXT_NUM].iws_ext_head.utc_time[0],\
                    app->iws_wave_pak_ext[app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now%IWS_UP_WAVEDATA_EXT_NUM].iws_ext_head.utc_time[0]\
                    );
                //exit(0);
                break;
            }
        }
        //return 0;
    }
    int is_buf_ready=(app->app_sig.sig_wave_data_write_buf_total-\
        app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now);
    if(is_buf_ready>1){
        if(0==app->iws_wave_pak_ext[app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now%IWS_UP_WAVEDATA_EXT_NUM].iws_ext_head.utc_time[0]){
            return 0;
        }


        printf(YELLOW"app->iws_wave_pak_ext[(app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now%IWS_UP_WAVEDATA_EXT_NUM].ch_idx[2]=%c\n"NONE,app->iws_wave_pak_ext[(app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now%IWS_UP_WAVEDATA_EXT_NUM)].ch_idx[2]);
        printf(GREEN"app->app_sig.sig_wave_data_write_buf_total=%d\n\
            app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index=%d\n\
            totol.iws_ext_head.utc_time[0]=%d\n\
            tirg.iws_ext_head.utc_time[0]=%d\n"NONE,\
            app->app_sig.sig_wave_data_write_buf_total,\
            app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now,\
            app->iws_wave_pak_ext[(app->app_sig.sig_wave_data_write_buf_total-1)%IWS_UP_WAVEDATA_EXT_NUM].iws_ext_head.utc_time[0],\
            app->iws_wave_pak_ext[app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now%IWS_UP_WAVEDATA_EXT_NUM].iws_ext_head.utc_time[0]\
            );
        return 1;
    }
    //int is_can_send_mode2=\
        (app->is_trig_start==1)&&\
        (app->iws_server[connect_index].mode==2);
    return 0;
}

int start_record_event()
{
    char url[256];
    if(app.evt_record.fp!=NULL)
    {
        app.evt_record.fp=fclose(app.evt_record.fp);
        app.evt_record.is_file_open=0;
    }
    snprintf(url,256,"%s/evt_%d.%d.evt",app.evt_record.path,app.evt_record.utc_time[0],app.evt_record.utc_time[1]);
    app.evt_record.fp=fopen(url,"w+");
    printf("url=%s,app.evt_record.fp=%d\n",url,app.evt_record.fp);
    //exit(0);
    if(app.evt_record.fp!=NULL){
            printf("url=%s,app.evt_record.fp=%d\n",url,app.evt_record.fp);
        app.evt_record.is_file_open=1;
        return 0;
    }
        printf("url=%s,app.evt_record.fp=%d\n",url,app.evt_record.fp);
    return 0;
}



int iws_write_process_v20(int fid,int connect_index,APP_S * app)//写处理框架
{
    //return 2;
    //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    //printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    char buf[1024];
    int realread=0,realsend=0;
    int ifor;
    int is_data_ready=(app->app_sig.sig_wave_data_write_buf_total-\
        app->iws_server[connect_index].iws_pak_buf.wave_buf_index.now);
    while((is_data_ready>0)&&(app->iws_server[connect_index].mode==1))
    {

       
        //printf("app->iws_server[connect_index].sig=%d,app->iws_server[connect_index].mode=%d\n",app->iws_server[connect_index].sig,app->iws_server[connect_index].mode);       
        //mk_wave_pak(connect_index);
        // if(app->iws_server[connect_index].is_first_send==0)
        // {
        //     app->iws_server[connect_index].is_first_send=1;

        //     pthread_mutex_lock(&app->app_mut[9]);
        //         app->iws_server[connect_index].steim2_buf=app->buffer.steim2_out_buf[STEIM2_NOW];//连接成功,初始化发送之争
        //         //printf(BLUE"connect_index=%d,app->iws_server[connect_index].steim2_buf.index=%d,sig=%d\n"NONE,connect_index,app->iws_server[connect_index].steim2_buf->index,app->iws_server[connect_index].sig);

        //     pthread_mutex_unlock(&app->app_mut[9]);
        //     for(ifor=0;ifor<app->iws_server[connect_index].sig;ifor++)
        //     {
        //         app->iws_server[connect_index].steim2_buf=app->iws_server[connect_index].steim2_buf->prev;//连接成功,初始化发送之争
        //     }
            
        //     printf(BLUE"connect_index=%d,app->iws_server[connect_index].steim2_buf.index=%d,sig=%d\n"NONE,connect_index,app->iws_server[connect_index].steim2_buf->index,app->iws_server[connect_index].sig);

        // }
        if(0==app->iws_server[connect_index].is_first_send)//连接后首次发送数据则更新发送指针
        {
            app->iws_server[connect_index].is_first_send=1;
            get_start_buf(connect_index);
        }
        int buf_index=app->iws_server[connect_index].iws_pak_buf.wave_buf_index.now%IWS_UP_WAVEDATA_EXT_NUM;
        copy_steim2_data_2_send_buf(&app->iws_cstp[connect_index].iws_up_wavedata,&app->iws_wave_pak_ext[buf_index]);
        app->iws_server[connect_index].iws_pak_buf.wave_buf_index.now++;
        //steim2_mk_wave_pak(&app->iws_cstp[connect_index].iws_up_wavedata,app->iws_server[connect_index].steim2_buf,++(app->iws_cstp[connect_index].pak_num));
        // printf(RED"connect_index=%d,app.buffer.steim2_out_buf[STEIM2_NOW].index=%d\n"NONE,connect_index,app->buffer.steim2_out_buf[STEIM2_NOW]->index);
        // printf(GREEN"connect_index=%d,app->iws_server[connect_index].steim2_buf.index=%d\n"NONE,connect_index,app->iws_server[connect_index].steim2_buf->index);
        //printf(LIGHT_BLUE"sig=%d,mode=%d\n"NONE,app->iws_server[connect_index].sig,app->iws_server[connect_index].mode);
        //printf(PURPLE"\n\
            buf_index=%d\n,\
            app->iws_cstp[connect_index].iws_up_wavedata.wcts[0]=%c\n,\
            app->app_sig.sig_wave_data_write_buf=%d\n,\
            is_data_ready=%d\n,\
            wave_buf_index.now=%d\n,\
            app->app_sig.sig_wave_data_write_buf_total=%d\n"NONE,\
            buf_index,\
            app->iws_cstp[connect_index].iws_up_wavedata.wcts[0],\
            app->app_sig.sig_wave_data_write_buf,\
            is_data_ready,\
            app->iws_server[connect_index].iws_pak_buf.wave_buf_index.now,\
            app->app_sig.sig_wave_data_write_buf_total);
        struct timespec date_time;//数据采集时间
// printf("\nmy_iws_utc.year=%d\n",app->iws_cstp[connect_index].iws_up_wavedata.data_start_utc.year);
// printf("my_iws_utc.yday=%d\n",app->iws_cstp[connect_index].iws_up_wavedata.data_start_utc.yday);
// printf("my_iws_utc.hour=%d\n",app->iws_cstp[connect_index].iws_up_wavedata.data_start_utc.hour);
// printf("my_iws_utc.min=%d\n",app->iws_cstp[connect_index].iws_up_wavedata.data_start_utc.min);
// printf("my_iws_utc.sec=%d\n",app->iws_cstp[connect_index].iws_up_wavedata.data_start_utc.sec);

        realsend=writenbytes(&app->iws_cstp[connect_index].iws_up_wavedata,fid,256);
        clock_gettime(CLOCK_REALTIME, &date_time);
        //debug_buf((char *)&app->iws_cstp[connect_index].iws_up_wavedata,realsend,"send to server data:");
        //printf(GREEN"realsend=%d,date_time.tv_sec=%d,date_time.tv_nsec=%d\n"NONE,realsend,date_time.tv_sec,date_time.tv_nsec);
        
        // if(app->iws_server[connect_index].steim2_buf->ch_num==0){
        //     int realwrite=mywrite(&app->iws_cstp[connect_index].iws_up_wavedata.data[0],192,fp_zzzzzzzzz);
        // }

        // printf(LIGHT_RED"app->iws_server[connect_index].steim2_buf=%d\n"NONE,app->iws_server[connect_index].steim2_buf);
        // printf(LIGHT_RED"srm=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata.srm);
        // printf(LIGHT_RED"sf=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata.sf);
        // printf(LIGHT_RED"package_number=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata.package_number);
        // printf(LIGHT_RED"datacount=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata.datacount);
        // printf(LIGHT_RED"fs_factor=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata->fs_factor);
        // printf(LIGHT_RED"fs_factor=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata->fs_factor);
        // printf(LIGHT_RED"fs_factor=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata->fs_factor);
        // printf(LIGHT_RED"fs_factor=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata->fs_factor);
        app->iws_server[connect_index].steim2_buf=app->iws_server[connect_index].steim2_buf->next;
        //exit(0);
        //return realsend; 
        is_data_ready=(app->app_sig.sig_wave_data_write_buf_total-\
            app->iws_server[connect_index].iws_pak_buf.wave_buf_index.now);
    }
    //app.is_trig_start=1
    if(app->iws_server[connect_index].sig_is_rq_data_ready==1){
        writenbytes((unsigned char *)&iws_up_wavedata_ext_globe->wcts[0],\
                    fid,\
                    sizeof(IWS_UP_WAVEDATA));
        app->iws_server[connect_index].sig_is_rq_data_ready=0;
    }
    
    if((app->iws_server[connect_index].mode==2)&&(app->iws_server[connect_index].is_trig_start==1)){
        if((check_is_trig_buf_ready(app,connect_index)))
        {
            if(check_event_continue(app,connect_index))
            {
                int buf_index=app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now%IWS_UP_WAVEDATA_EXT_NUM;
                copy_steim2_data_2_send_buf(&app->iws_cstp[connect_index].iws_up_wavedata,&app->iws_wave_pak_ext[buf_index]);
                mk_wave_data_check(&app->iws_cstp[connect_index].iws_up_wavedata,connect_index);
                //printf(RED"buf_index=%d\napp->iws_cstp[connect_index].iws_up_wavedata=%c\n"NONE,buf_index,app->iws_cstp[connect_index].iws_up_wavedata.ch_idx[2]);
                realsend+=writenbytes(&app->iws_cstp[connect_index].iws_up_wavedata,fid,sizeof(IWS_UP_WAVEDATA));
                app->iws_server[connect_index].iws_pak_buf.trig_wave_buf_index.now++;//printf("");
            }
            else{
                app->iws_server[connect_index].is_trig_start=0;
                end_event(app,connect_index);
            }
        }

    }




/*
    if((app->iws_server[connect_index].mode==2)&&(app->iws_server[connect_index].sig_trig_wave>0))//触发传波形
    {
        //printf(GREEN"app->iws_server[connect_index].sig_trig_wave=%d\n"NONE,app->iws_server[connect_index].sig_trig_wave);
        steim2_mk_trig_pak(&app->iws_cstp[connect_index].iws_up_wavedata,connect_index,++(app->iws_cstp[connect_index].pak_num));
        mk_wave_data_check(&app->iws_cstp[connect_index].iws_up_wavedata,connect_index);
        pthread_mutex_lock(&app->app_mut[connect_index]);
            app->iws_server[connect_index].sig_trig_wave--;
            if(app->iws_server[connect_index].is_trig_start==0){
                app->iws_server[connect_index].sig_trig_wave=0;
            }
            //app->iws_server[connect_index].sig--;
        pthread_mutex_unlock(&app->app_mut[connect_index]);
        realsend+=writenbytes(&app->iws_cstp[connect_index].iws_up_wavedata,fid,256);
        //debug_buf((char *)&app->iws_cstp[connect_index].iws_up_wavedata,realsend,"send to server data:");
        //printf(GREEN"realsend=%d,app->iws_server[connect_index].sig_trig_wave=%d\n"NONE,realsend,app->iws_server[connect_index].sig_trig_wave);

        //app->iws_server[connect_index].sig_trig_wave--;
    }
*/
    if((app->iws_server[connect_index].mode>1)&&(app->iws_server[connect_index].sig_trig_ti>0))//触发传波形
    {
        app->iws_server[connect_index].sig_trig_ti--;
        steim2_mk_trig_ti_pak(&app->iws_cstp[connect_index].iws_up_ti,++(app->iws_cstp[connect_index].pak_num));
        mk_trig_ti_check(&app->iws_cstp[connect_index].iws_up_ti,connect_index);
        realsend+=writenbytes(&app->iws_cstp[connect_index].iws_up_ti,fid,256);
        //debug_buf((char *)&app->iws_cstp[connect_index].iws_up_ti,256,"ti:");
    }
    if((app->iws_server[connect_index].sig_status>0)&&(app->iws_server[connect_index].mode>1))//
    {

        app->iws_server[connect_index].sig_status--;
        mk_si_pak(connect_index);
        realread=writenbytes(&app->iws_cstp[connect_index].iws_up_si_pak,fid,256);
        app->iws_server[connect_index].status_send_buf=app->iws_server[connect_index].status_send_buf->next;
    }
    if((app->iws_server[connect_index].mode==3))
    {
    }
    //realread=writenbytes(app->app_server.connect[connect_index].sendbuf->data_start,fid,app->app_para.fs*sizeof(DATA_FRAME_GEO_S));
    //write(sockfd,p_app->app_server.connect[connect_index].sendbuf->data_start,200*sizeof(DATA_FRAME_GEO_S));
    //app->app_server.connect[connect_index].sig_send--;
    //printf(GREEN"function：%s,line:%d,app.app_sig.sig_data_send=%d\n"NONE,__FUNCTION__,__LINE__,p_app->app_sig.sig_data_send);
    //app->app_server.connect[connect_index].sendbuf=app->app_server.connect[connect_index].sendbuf->loopbufnext;
    return realread; 
}











int iws_write_process_v10(int fid,int connect_index,APP_S * app)//写处理框架
{
    //return 2;
    //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    //printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    char buf[1024];
    int realread=0,realsend=0;
    int ifor;
    if((app->iws_server[connect_index].sig>0)&&(app->iws_server[connect_index].mode==1))
    {
        //printf("app->iws_server[connect_index].sig=%d,app->iws_server[connect_index].mode=%d\n",app->iws_server[connect_index].sig,app->iws_server[connect_index].mode);       
        //mk_wave_pak(connect_index);
        // if(app->iws_server[connect_index].is_first_send==0)
        // {
        //     app->iws_server[connect_index].is_first_send=1;

        //     pthread_mutex_lock(&app->app_mut[9]);
        //         app->iws_server[connect_index].steim2_buf=app->buffer.steim2_out_buf[STEIM2_NOW];//连接成功,初始化发送之争
        //         //printf(BLUE"connect_index=%d,app->iws_server[connect_index].steim2_buf.index=%d,sig=%d\n"NONE,connect_index,app->iws_server[connect_index].steim2_buf->index,app->iws_server[connect_index].sig);

        //     pthread_mutex_unlock(&app->app_mut[9]);
        //     for(ifor=0;ifor<app->iws_server[connect_index].sig;ifor++)
        //     {
        //         app->iws_server[connect_index].steim2_buf=app->iws_server[connect_index].steim2_buf->prev;//连接成功,初始化发送之争
        //     }
            
        //     printf(BLUE"connect_index=%d,app->iws_server[connect_index].steim2_buf.index=%d,sig=%d\n"NONE,connect_index,app->iws_server[connect_index].steim2_buf->index,app->iws_server[connect_index].sig);

        // }
        if(0==app->iws_server[connect_index].is_first_send)//连接后首次发送数据则更新发送指针
        {
            app->iws_server[connect_index].is_first_send=1;
            get_start_buf(connect_index);
        }
        steim2_mk_wave_pak(&app->iws_cstp[connect_index].iws_up_wavedata,app->iws_server[connect_index].steim2_buf,++(app->iws_cstp[connect_index].pak_num));
        // printf(RED"connect_index=%d,app.buffer.steim2_out_buf[STEIM2_NOW].index=%d\n"NONE,connect_index,app->buffer.steim2_out_buf[STEIM2_NOW]->index);
        // printf(GREEN"connect_index=%d,app->iws_server[connect_index].steim2_buf.index=%d\n"NONE,connect_index,app->iws_server[connect_index].steim2_buf->index);
        pthread_mutex_lock(&app->app_mut[connect_index]);
            app->iws_server[connect_index].sig--;
        pthread_mutex_unlock(&app->app_mut[connect_index]);
        //app->iws_server[connect_index].sig--;
        //printf(LIGHT_BLUE"sig=%d,mode=%d\n"NONE,app->iws_server[connect_index].sig,app->iws_server[connect_index].mode);

        struct timespec date_time;//数据采集时间
// printf("\nmy_iws_utc.year=%d\n",app->iws_cstp[connect_index].iws_up_wavedata.data_start_utc.year);
// printf("my_iws_utc.yday=%d\n",app->iws_cstp[connect_index].iws_up_wavedata.data_start_utc.yday);
// printf("my_iws_utc.hour=%d\n",app->iws_cstp[connect_index].iws_up_wavedata.data_start_utc.hour);
// printf("my_iws_utc.min=%d\n",app->iws_cstp[connect_index].iws_up_wavedata.data_start_utc.min);
// printf("my_iws_utc.sec=%d\n",app->iws_cstp[connect_index].iws_up_wavedata.data_start_utc.sec);

        realsend=writenbytes(&app->iws_cstp[connect_index].iws_up_wavedata,fid,256);
        clock_gettime(CLOCK_REALTIME, &date_time);
        //debug_buf((char *)&app->iws_cstp[connect_index].iws_up_wavedata,realsend,"send to server data:");
        //printf(GREEN"realsend=%d,date_time.tv_sec=%d,date_time.tv_nsec=%d\n"NONE,realsend,date_time.tv_sec,date_time.tv_nsec);
        
        // if(app->iws_server[connect_index].steim2_buf->ch_num==0){
        //     int realwrite=mywrite(&app->iws_cstp[connect_index].iws_up_wavedata.data[0],192,fp_zzzzzzzzz);
        // }

        // printf(LIGHT_RED"app->iws_server[connect_index].steim2_buf=%d\n"NONE,app->iws_server[connect_index].steim2_buf);
        // printf(LIGHT_RED"srm=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata.srm);
        // printf(LIGHT_RED"sf=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata.sf);
        // printf(LIGHT_RED"package_number=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata.package_number);
        // printf(LIGHT_RED"datacount=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata.datacount);
        // printf(LIGHT_RED"fs_factor=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata->fs_factor);
        // printf(LIGHT_RED"fs_factor=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata->fs_factor);
        // printf(LIGHT_RED"fs_factor=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata->fs_factor);
        // printf(LIGHT_RED"fs_factor=%d\n"NONE,app->iws_cstp[connect_index].iws_up_wavedata->fs_factor);
        printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        app->iws_server[connect_index].steim2_buf=app->iws_server[connect_index].steim2_buf->next;
        return realsend; 
    }
    //app.is_trig_start=1

    if((app->iws_server[connect_index].mode==2)&&(app->iws_server[connect_index].sig_trig_wave>0))//触发传波形
    {
        //printf(GREEN"app->iws_server[connect_index].sig_trig_wave=%d\n"NONE,app->iws_server[connect_index].sig_trig_wave);
        steim2_mk_trig_pak(&app->iws_cstp[connect_index].iws_up_wavedata,connect_index,++(app->iws_cstp[connect_index].pak_num));
        mk_wave_data_check(&app->iws_cstp[connect_index].iws_up_wavedata,connect_index);
        pthread_mutex_lock(&app->app_mut[connect_index]);
            app->iws_server[connect_index].sig_trig_wave--;
            if(app->iws_server[connect_index].is_trig_start==0){
                app->iws_server[connect_index].sig_trig_wave=0;
            }
            //app->iws_server[connect_index].sig--;
        pthread_mutex_unlock(&app->app_mut[connect_index]);
        realsend+=writenbytes(&app->iws_cstp[connect_index].iws_up_wavedata,fid,256);
        //debug_buf((char *)&app->iws_cstp[connect_index].iws_up_wavedata,realsend,"send to server data:");
        //printf(GREEN"realsend=%d,app->iws_server[connect_index].sig_trig_wave=%d\n"NONE,realsend,app->iws_server[connect_index].sig_trig_wave);

        //app->iws_server[connect_index].sig_trig_wave--;
    }
    if((app->iws_server[connect_index].mode>1)&&(app->iws_server[connect_index].sig_trig_ti>0))//触发传波形
    {
        app->iws_server[connect_index].sig_trig_ti--;
        steim2_mk_trig_ti_pak(&app->iws_cstp[connect_index].iws_up_ti,++(app->iws_cstp[connect_index].pak_num));
        mk_trig_ti_check(&app->iws_cstp[connect_index].iws_up_ti,connect_index);
        realsend+=writenbytes(&app->iws_cstp[connect_index].iws_up_ti,fid,256);
        //debug_buf((char *)&app->iws_cstp[connect_index].iws_up_ti,256,"ti:");
    }
    if((app->iws_server[connect_index].sig_status>0)&&(app->iws_server[connect_index].mode>1))//
    {

        app->iws_server[connect_index].sig_status--;
        mk_si_pak(connect_index);
        realread=writenbytes(&app->iws_cstp[connect_index].iws_up_si_pak,fid,256);
        app->iws_server[connect_index].status_send_buf=app->iws_server[connect_index].status_send_buf->next;
    }
    if((app->iws_server[connect_index].mode==3))
    {
    }
    //realread=writenbytes(app->app_server.connect[connect_index].sendbuf->data_start,fid,app->app_para.fs*sizeof(DATA_FRAME_GEO_S));
    //write(sockfd,p_app->app_server.connect[connect_index].sendbuf->data_start,200*sizeof(DATA_FRAME_GEO_S));
    //app->app_server.connect[connect_index].sig_send--;
    //printf(GREEN"function：%s,line:%d,app.app_sig.sig_data_send=%d\n"NONE,__FUNCTION__,__LINE__,p_app->app_sig.sig_data_send);
    //app->app_server.connect[connect_index].sendbuf=app->app_server.connect[connect_index].sendbuf->loopbufnext;
    return realread; 
}

int iws_write_process(int fid,int connect_index,APP_S * app)//写处理框架
{
    if((app->iws_server[connect_index].protocol==SERVER_PROTOCOL_CSTP)&&(app->iws_server[connect_index].version==SERVER_PROTOCOL_CSTP_VERSION_10)){
        iws_write_process_v10(fid,connect_index,app);
    }
    else{
        iws_write_process_v20(fid,connect_index,app);
    }
    return 0;
}



int register_iws_to_server(APP_S * app,IWS_REGISTER * iws_reg_pak,int sockfd)//等待设备状态就绪，从而开始设备注册
{

    char readbuf[1024];
    int success_len,sendlen=0,recvlen=0;
    int ifor;
    int ret;
    for(ifor=0;ifor<5;ifor++)
    {
        printf(PURPLE"try to connect server sizeof(IWS_REGISTER)=%d\n"NONE,sizeof(IWS_REGISTER));
        sendlen = write(sockfd, iws_reg_pak, sizeof(IWS_REGISTER));
        if(sendlen==CSTP_UP_PAKLEN){
            recvlen = read(sockfd, readbuf, CSTP_DOWN_PAKLEN);
            //ret=check_reg_re(readbuf);
            printf("check_reg_re ret=%d\n",ret);
            if(ret==0){
                //&(iws_cstp_globe->iws_reg_pak)
                return 0;
            }
            else{
                sleep(10);
                ;//循环
            }
        }
        
    }
}



int set_tcp_opt(int client_sockfd,int server_index)
{
    const int on =1;
    int keepAlive = 1; // 开启keepalive属性  
    int keepIdle = 3; // 如该连接在60秒内没有任何数据往来,则进行探测   
    int keepInterval = 5; // 探测时发包的时间间隔为5 秒  
    int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.  
    int ret;
    struct timeval timeout={8,0};//3s
        //exit(0);
    int bNodelay=1;
    ret=setsockopt(client_sockfd,IPPROTO_TCP,TCP_NODELAY,(char *)&bNodelay,sizeof(bNodelay)); 
    printf("server_index=%d,TCP_NODELAY ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    ret=setsockopt(client_sockfd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
    //ret=setsockopt(client_sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
    printf("server_index=%d,SO_SNDTIMEO ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    //重启后立马可以使用端口
    ret=setsockopt(client_sockfd, SOL_SOCKET, SO_REUSEADDR, &on,sizeof (on)); 
    printf("server_index=%d,SO_REUSEADDR ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    //重启后立马可以使用端口
    //判断对方否否发生意外断开
    ret=setsockopt(client_sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));  
    printf("server_index=%d,SO_KEEPALIVE ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    ret=setsockopt(client_sockfd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));  
    printf("server_index=%d,TCP_KEEPIDLE ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    ret=setsockopt(client_sockfd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));  
    printf("server_index=%d,TCP_KEEPINTVL ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    ret=setsockopt(client_sockfd, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
    printf("server_index=%d,TCP_KEEPCNT ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",client_sockfd);
        perror("err:");
        //exit(0);
    }
    //判断对方否否发生意外断开*/
    return 0;
}








int set_socket_non_block(int sfd){
    int flags, s;

    flags = fcntl(sfd, F_GETFL, 0); 
    if (flags == -1) {
        perror ("fcntl");
        return -1; 
    }   

    flags |= O_NONBLOCK;
    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1) {
        perror ("fcntl");
        return -1; 
    }   

    return 0;
}









int iws_client1(APP_S * app)
{
    //test_wave_data_init();
    sleep(5);
    IWS_CSTP iws_cstp;
    int server_index=app->server_index;
    app->server_index++;
    printf("in iws_client1 ping_pang=%d\n",app->ping_pang);
    int ifor,i_connect;
    // if(((app->iws_server[server_index].mode)>3)||((app->iws_server[server_index].mode)<1))
    // {
    //     app->iws_server[server_index].mode=app->iws_server[server_index].last_mode=1;
    // }
    // else{
    //     app->iws_server[server_index].last_mode=app->iws_server[server_index].mode;
    // }
    //app->iws_server[server_index].mode=1;
    
    
    struct sockaddr_in servaddr;
    char buf[BUFSIZE];
    int sockfd, n;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //inet_pton(AF_INET, "222.222.119.11", &servaddr.sin_addr);
    inet_pton(AF_INET, app->iws_server[server_index].ipv4_addr, &servaddr.sin_addr);
    servaddr.sin_port = htons(app->iws_server[server_index].port);

    /*连接服务端*/
    //printf("try to connect\n");
   // printf("ip=%s,port=%d\n",app->iws_server[server_index].ipv4_addr,app->iws_server[server_index].port);
    //exit(0);
    //wait_for_device_ok(app);//等待设备状态就绪，从而开始设备注册
    int reg_status=-1;
/*
    pthread_t  newthread;
    int ii=osonewthread(&newthread,app_server_start_ctrl,(void * )app);
*/
    int on_connect=0;
    //int i_connect;//标记连接索引,以方便ad给信号
    int timeout;
    app->iws_server[server_index].version=1;
    while(1){
        app->iws_server[server_index].status.isconnect=0;
        reg_status=-1;//重连时复位注册状态
        on_connect=0;
        int ret;
        int realread=0,realwrite=0,rw_select_ret=0;
        timeout=app->app_count.second_count;
        
        
        app->iws_server[server_index].fid=sockfd;
        // servaddr.sin_family = AF_INET;
        // inet_pton(AF_INET, "192.168.0.112", &servaddr.sin_addr);
        // servaddr.sin_port = htons(1973);
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        inet_pton(AF_INET, app->iws_server[server_index].ipv4_addr, &servaddr.sin_addr);
        servaddr.sin_port = htons(app->iws_server[server_index].port);
        //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        // printf(RED"try to conenct server %d,sockfd=%d\n"NONE,server_index,sockfd);
        // printf(RED"server ip :%s\n"NONE,app->iws_server[server_index].ipv4_addr);
        // printf(RED"server port :%d\n"NONE,app->iws_server[server_index].port);
        init_cstp(&app->iws_cstp[server_index],server_index);
        init_server(&app->iws_server[server_index]);
        app->iws_server[server_index].is_first_send=0;
        app->iws_server[server_index].sig=0;


    int keepAlive = 1; // 开启keepalive属性  
    int keepIdle = 3; // 如该连接在60秒内没有任何数据往来,则进行探测   
    int keepInterval = 5; // 探测时发包的时间间隔为5 秒  
    int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发. 
     ret=setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));  
    //printf("server_index=%d,SO_KEEPALIVE ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",sockfd);
        perror("err:");
        //exit(0);
    }
    ret=setsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));  
    //printf("server_index=%d,TCP_KEEPIDLE ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",sockfd);
        perror("err:");
        //exit(0);
    }
    ret=setsockopt(sockfd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));  
    //printf("server_index=%d,TCP_KEEPINTVL ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",sockfd);
        perror("err:");
        //exit(0);
    }
    ret=setsockopt(sockfd, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
   // printf("server_index=%d,TCP_KEEPCNT ret=%d\n",server_index,ret);
    if(ret!=0)
    {
        printf("client_sockfd=%d\n",sockfd);
        perror("err:");
        //exit(0);
    }
    //判断对方否否发生意外断开*/


        ret=connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        set_socket_non_block(sockfd);
        //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        if(ret==0){
            set_tcp_opt(sockfd,server_index);
            reg_status=select_register_device_iws(app,&(app->iws_cstp[server_index].iws_reg_pak),sockfd,server_index);//等待设备状态就绪，从而开始设备注册
            // reg_status=register_device_iws(app,&(iws_cstp_globe.iws_reg_pak),sockfd);//等待设备状态就绪，从而开始设备注册
            if(reg_status==0){
            printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
                // for(ifor=0;ifor<10;ifor++)
                // {
                //    if(0==app->app_server.connect[ifor].is_connect){
                //         app->app_server.connect[ifor].is_connect=1;
                //         app->app_server.connect[ifor].sendbuf=app->buffer.databuf[LOOPREAD];
                //         i_connect=ifor;
                //         printf("client i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",i_connect,app->app_server.connect[ifor].is_connect);
                //         break;
                //    }
                // }
            //pthread_mutex_lock(&app->app_mut[9]);
                //exit(0);
                app->iws_server[server_index].steim2_buf=app->buffer.steim2_out_buf[STEIM2_NOW];//连接成功,初始化发送之争
                printf(RED"connect app.buffer.steim2_out_buf[STEIM2_NOW]->utc_time[0]=%d\n"NONE,app->buffer.steim2_out_buf[STEIM2_NOW]->utc_time[0]);
                app->iws_server[server_index].status_send_buf=app->buffer.status_out_buf[STATUS_NOW];//连接成功,初始化发送之争
                printf(RED"steim2_buf=%d\n"NONE,app->iws_server[server_index].steim2_buf);
                //exit(0);
                on_connect=1;
                app->iws_server[server_index].status.isconnect=1;
            //pthread_mutex_unlock(&app->app_mut[9]);
            }
            else{
                close(sockfd);
                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                app->iws_server[server_index].status.isconnect=0;
                //app->app_server.connect[i_connect].is_connect=0;
                on_connect=0; 
            }
        }
        else{
            app->iws_server[server_index].status.isconnect=0;//
            on_connect=0;
        }
        //reg_status=register_device_iws(app,&(iws_cstp_globe.iws_reg_pak),sockfd);//等待设备状态就绪，从而开始设备注册
        while(on_connect==1){
            
            
            //printf("reg_status=%d\n",reg_status);
            //exit(0);
            if(reg_status==0){

                rw_select_ret=rw_select_server(sockfd,server_index);
                //printf(RED"inside function %s line:%d rw_select_ret=%d\n"NONE,__FUNCTION__,__LINE__,rw_select_ret);
                switch(rw_select_ret)
                {
                    case 1:
                        realread=iws_read_process(sockfd,server_index,app);
                        printf(GREEN"realread=%d\n"NONE,realread);
                        if(realread<=0){
                            //printf(BLUE"sockfd=%d closed\n"NONE,sockfd);
                            close(sockfd);
                            
                            sockfd = socket(AF_INET, SOCK_STREAM, 0);
                            //app->app_server.connect[i_connect].is_connect=0;
                            printf(BLUE"sockfd=%d closed\n"NONE,sockfd);
                            on_connect=0; 
                        }

                    break;
                    case 2:
                        //printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
                        //app->app_server.connect[i_connect].sig_send=123;
                        //printf(RED"sig_send=%d\n"NONE,app->app_server.connect[ifor].sig_send);
                        // if(reg_status!=0||isoutoftimes){
                        //     reg_status=register_iws_to_server(app,&(iws_cstp_globe.iws_reg_pak),sockfd);//等待设备状态就绪，从而开始设备注册
                        // }
                        if(reg_status==0){//注册成功的情况下
                                //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

                            //while((app->iws_server[server_index].sig)<1||(app->iws_server[server_index].sig_status)<1)
                            //{
                                   // printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

                                //printf(RED"sig_send=%d\n"NONE,app->app_server.connect[ifor].sig_send);
                                //app->app_server.connect[i_connect].is_connect=1;
                                //app.app_server.connect[ifor].sig_send++
                                usleep(5);
                                //
                                //wait_go_signal(1);
                                //printf(RED"sig_send=%d\n"NONE,app->app_server.connect[i_connect].sig_send);
                                //printf(GREEN"i_connect=%d,app.app_server.connect[i_connect].is_connect=%d\n"NONE,i_connect,app->app_server.connect[i_connect].is_connect);

                                //printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
                            //}
                                //printf(PURPLE"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);

                            realwrite=iws_write_process(sockfd,server_index,app);
                                                    //realread=iws_read_process(sockfd);

                            if(realwrite<0){
                                app->iws_server[server_index].status.isconnect=0;
                                close(sockfd);
                                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                                //app->app_server.connect[i_connect].is_connect=0;
                                on_connect=0; 
                            }   
                        }

                    break;
                    case -1:
                        app->iws_server[server_index].status.isconnect=0;
                        close(sockfd);
                        sockfd = socket(AF_INET, SOCK_STREAM, 0);
                        //app->app_server.connect[i_connect].is_connect=0;
                        on_connect=0;
                    break;

                    case 0://TIMEOUT
                        app->iws_server[server_index].status.isconnect=0;
                        close(sockfd);
                        sockfd = socket(AF_INET, SOCK_STREAM, 0);
                        on_connect=0;
                        printf(LIGHT_CYAN"server_index %d timeout\n"NONE,server_index);
                    break;
                    default:
                    break;
                        
                }
            }
            else{
                close(sockfd);
                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                on_connect=0;
            }
        }
        sleep(5);
        // if((app->app_count.second_count-timeout)<5){
        //    sleep(10-(app->app_count.second_count-timeout));
        // }
        // else{
        // }
    }
    return 0;
}















int iws_client2( APP_S * app)
{
    //IWS_CSTP iws_cstp;
    printf("in iws_client1 ping_pang=%d IWS_SERVER=%d\n",app->ping_pang,sizeof(IWS_SERVER));
    int ifor,i_connect;
            // for(ifor=0;ifor<10;ifor++)
            // {
            //    //if(0==app->app_server.connect[ifor].is_connect){
            //         app.app_server.connect[ifor].is_connect=1;
            //         //app->app_server.connect[ifor].sendbuf=app->buffer.databuf[LOOPREAD];
            //         i_connect=ifor;
            //         printf("i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",i_connect,app.app_server.connect[ifor].is_connect);
            //         //break;
            //    //}
            // }
    while(1){
            for(ifor=0;ifor<10;ifor++)
            {
               //if(0==app->app_server.connect[ifor].is_connect){
                    //app.app_server.connect[ifor].is_connect=1;
                    //app->app_server.connect[ifor].sendbuf=app->buffer.databuf[LOOPREAD];
                    i_connect=ifor;
                    printf("i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",i_connect,app->app_server.connect[ifor].is_connect);
                    //break;
               //}
            }
            return 0;
    }


            //exit(0);
//     init_cstp_globe(&iws_cstp_globe);
//     struct sockaddr_in servaddr;
//     char buf[BUFSIZE];
//     int sockfd, n;

//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     bzero(&servaddr, sizeof(servaddr));
//     servaddr.sin_family = AF_INET;
//     //inet_pton(AF_INET, "222.222.119.11", &servaddr.sin_addr);
//     inet_pton(AF_INET, "192.168.12.5", &servaddr.sin_addr);
//     servaddr.sin_port = htons(8888);

//     /*连接服务端*/
//     printf("try to connect\n");
//     //wait_for_device_ok(app);//等待设备状态就绪，从而开始设备注册
//     int reg_status;
// /*
//     pthread_t  newthread;
//     int ii=osonewthread(&newthread,app_server_start_ctrl,(void * )app);
// */
//     int on_connect=0;
//     int realread,realwrite,rw_select_ret;
//     //int i_connect;//标记连接索引,以方便ad给信号
//     while(1){
//         int ret=connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
//         if(ret==0){
//             on_connect=1;
//             for(ifor=0;ifor<10;ifor++)
//             {
//                if(0==app->app_server.connect[ifor].is_connect){
//                     app->app_server.connect[ifor].is_connect=1;
//                     app->app_server.connect[ifor].sendbuf=app->buffer.databuf[LOOPREAD];
//                     i_connect=ifor;
//                     printf("i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",i_connect,app->app_server.connect[ifor].is_connect);
//                     break;
//                }
//             }
//         }
//         else{
//             on_connect=-1;
//         }
//         while(on_connect){
            
//             reg_status=0;//register_device(app,&(iws_cstp_globe.iws_reg_pak),sockfd);//等待设备状态就绪，从而开始设备注册
//             if(reg_status==0){
//                 rw_select_ret=rw_select(sockfd);
//                 printf(RED"inside function %s line:%d rw_select_ret=%d\n"NONE,__FUNCTION__,__LINE__,rw_select_ret);
//                 switch(rw_select_ret)
//                 {
//                     case 1:
//                         realread=iws_read_process(sockfd);
//                         if(realread==-1){
//                             close(sockfd);
//                             on_connect=0; 
//                         }

//                     break;
//                     case 2:
//                         printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
//                         //app->app_server.connect[i_connect].sig_send=123;
//                         printf(RED"sig_send=%d\n"NONE,app->app_server.connect[ifor].sig_send);

//                         while(app->app_server.connect[i_connect].sig_send<=0)
//                         {
//                             //printf(RED"sig_send=%d\n"NONE,app->app_server.connect[ifor].sig_send);
//                             app->app_server.connect[i_connect].is_connect=1;
//                             //app.app_server.connect[ifor].sig_send++
//                             usleep(250*1000);
//                             //
//                             //wait_go_signal(1);
                            
//                             printf(GREEN"i_connect=%d,app.app_server.connect[i_connect].is_connect=%d\n"NONE,i_connect,app->app_server.connect[i_connect].is_connect);

//                             printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
//                         }
//                         realwrite=iws_write_process(sockfd,i_connect,app);
//                         if(realwrite==-1){
//                             close(sockfd);
//                             on_connect=0; 
//                         }
//                     break;
//                     case -1:
//                         close(sockfd);
//                         on_connect=0;
//                     break;
//                     default:
//                     break;
                        
//                 }
//                 //realsend=write(client_sockfd[i_connect],sendbuf+sendcount,sizeof(NET_packet)+DATATOSENDSIZE-sendcount);
//                 //n = read(sockfd, buf, BUFSIZE);
//             }
//             else{
//                 close(sockfd);
//                 on_connect=0;
//             }
//         }


//         printf("end ret=%d\n",ret);
//     }
//     // while(fgets(buf, BUFSIZE, stdin) != NULL){
//     //     /*通过sockfd给服务端发送数据*/
//     //     write(sockfd, buf, strlen(buf));
//     //     n = read(sockfd, buf, BUFSIZE);
//     //     if(n == 0)
//     //         printf("the other side has been closed.\n");
//     //     else/*打印输出服务端传过来的数据*/
//     //         write(STDOUT_FILENO, buf, n);
//     // }
//     return 0;
}










// /*
// Copyright (c) 2009-2018 Roger Light <roger@atchoo.org>

// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v1.0
// and Eclipse Distribution License v1.0 which accompany this distribution.
 
// The Eclipse Public License is available at
//    http://www.eclipse.org/legal/epl-v10.html
// and the Eclipse Distribution License is available at
//   http://www.eclipse.org/org/documents/edl-v10.php.
 
// Contributors:
//    Roger Light - initial implementation and documentation.
// */

// /* For nanosleep */
// /*
// #define _POSIX_C_SOURCE 200809L

// #include <errno.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include <mosquitto.h>
// #include <client_shared.h>

// #define STATUS_CONNECTING 0
// #define STATUS_CONNACK_RECVD 1
// #define STATUS_WAITING 2
// #define STATUS_DISCONNECTING 3

// /* Global variables for use in callbacks. See sub_client.c for an example of
//  * using a struct to hold variables for use in callbacks. */
// static char *topic = NULL;
// static char *message = NULL;
// static long msglen = 0;
// static int qos = 0;
// static int retain = 0;
// static int mode = MSGMODE_NONE;
// static int status = STATUS_CONNECTING;
// static int mid_sent = 0;
// static int last_mid = -1;
// static int last_mid_sent = -1;
// static bool connected = true;
// static char *username = NULL;
// static char *password = NULL;
// static bool disconnect_sent = false;
// static bool quiet = false;

// void my_connect_callback(struct mosquitto *mosq, void *obj, int result)
// {
//     int rc = MOSQ_ERR_SUCCESS;

//     if(!result){
//         switch(mode){
//             case MSGMODE_CMD:
//             case MSGMODE_FILE:
//             case MSGMODE_STDIN_FILE:
//                 rc = mosquitto_publish(mosq, &mid_sent, topic, msglen, message, qos, retain);
//                 break;
//             case MSGMODE_NULL:
//                 rc = mosquitto_publish(mosq, &mid_sent, topic, 0, NULL, qos, retain);
//                 break;
//             case MSGMODE_STDIN_LINE:
//                 status = STATUS_CONNACK_RECVD;
//                 break;
//         }
//         if(rc){
//             if(!quiet){
//                 switch(rc){
//                     case MOSQ_ERR_INVAL:
//                         fprintf(stderr, "Error: Invalid input. Does your topic contain '+' or '#'?\n");
//                         break;
//                     case MOSQ_ERR_NOMEM:
//                         fprintf(stderr, "Error: Out of memory when trying to publish message.\n");
//                         break;
//                     case MOSQ_ERR_NO_CONN:
//                         fprintf(stderr, "Error: Client not connected when trying to publish.\n");
//                         break;
//                     case MOSQ_ERR_PROTOCOL:
//                         fprintf(stderr, "Error: Protocol error when communicating with broker.\n");
//                         break;
//                     case MOSQ_ERR_PAYLOAD_SIZE:
//                         fprintf(stderr, "Error: Message payload is too large.\n");
//                         break;
//                 }
//             }
//             mosquitto_disconnect(mosq);
//         }
//     }else{
//         if(result && !quiet){
//             fprintf(stderr, "%s\n", mosquitto_connack_string(result));
//         }
//     }
// }

// void my_disconnect_callback(struct mosquitto *mosq, void *obj, int rc)
// {
//     connected = false;
// }

// void my_publish_callback(struct mosquitto *mosq, void *obj, int mid)
// {
//     last_mid_sent = mid;
//     if(mode == MSGMODE_STDIN_LINE){
//         if(mid == last_mid){
//             mosquitto_disconnect(mosq);
//             disconnect_sent = true;
//         }
//     }else if(disconnect_sent == false){
//         mosquitto_disconnect(mosq);
//         disconnect_sent = true;
//     }
// }

// void my_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str)
// {
//     printf("%s\n", str);
// }

// int load_stdin(void)
// {
//     long pos = 0, rlen;
//     char buf[1024];
//     char *aux_message = NULL;

//     mode = MSGMODE_STDIN_FILE;

//     while(!feof(stdin)){
//         rlen = fread(buf, 1, 1024, stdin);
//         aux_message = realloc(message, pos+rlen);
//         if(!aux_message){
//             if(!quiet) fprintf(stderr, "Error: Out of memory.\n");
//             free(message);
//             return 1;
//         } else
//         {
//             message = aux_message;
//         }
//         memcpy(&(message[pos]), buf, rlen);
//         pos += rlen;
//     }
//     msglen = pos;

//     if(!msglen){
//         if(!quiet) fprintf(stderr, "Error: Zero length input.\n");
//         return 1;
//     }

//     return 0;
// }

// int load_file(const char *filename)
// {
//     long pos, rlen;
//     FILE *fptr = NULL;

//     fptr = fopen(filename, "rb");
//     if(!fptr){
//         if(!quiet) fprintf(stderr, "Error: Unable to open file \"%s\".\n", filename);
//         return 1;
//     }
//     mode = MSGMODE_FILE;
//     fseek(fptr, 0, SEEK_END);
//     msglen = ftell(fptr);
//     if(msglen > 268435455){
//         fclose(fptr);
//         if(!quiet) fprintf(stderr, "Error: File \"%s\" is too large (>268,435,455 bytes).\n", filename);
//         return 1;
//     }else if(msglen == 0){
//         fclose(fptr);
//         if(!quiet) fprintf(stderr, "Error: File \"%s\" is empty.\n", filename);
//         return 1;
//     }else if(msglen < 0){
//         fclose(fptr);
//         if(!quiet) fprintf(stderr, "Error: Unable to determine size of file \"%s\".\n", filename);
//         return 1;
//     }
//     fseek(fptr, 0, SEEK_SET);
//     message = malloc(msglen);
//     if(!message){
//         fclose(fptr);
//         if(!quiet) fprintf(stderr, "Error: Out of memory.\n");
//         return 1;
//     }
//     pos = 0;
//     while(pos < msglen){
//         rlen = fread(&(message[pos]), sizeof(char), msglen-pos, fptr);
//         pos += rlen;
//     }
//     fclose(fptr);
//     return 0;
// }


// #define VERSION 0
// void print_usage(void)
// {
//     int major, minor, revision;

//     mosquitto_lib_version(&major, &minor, &revision);
//     printf("mosquitto_pub is a simple mqtt client that will publish a message on a single topic and exit.\n");
//     printf("mosquitto_pub version %s running on libmosquitto %d.%d.%d.\n\n", VERSION, major, minor, revision);
//     printf("Usage: mosquitto_pub {[-h host] [-p port] [-u username [-P password]] -t topic | -L URL}\n");
//     printf("                     {-f file | -l | -n | -m message}\n");
//     printf("                     [-c] [-k keepalive] [-q qos] [-r]\n");
// #ifdef WITH_SRV
//     printf("                     [-A bind_address] [-S]\n");
// #else
//     printf("                     [-A bind_address]\n");
// #endif
//     printf("                     [-i id] [-I id_prefix]\n");
//     printf("                     [-d] [--quiet]\n");
//     printf("                     [-M max_inflight]\n");
//     printf("                     [-u username [-P password]]\n");
//     printf("                     [--will-topic [--will-payload payload] [--will-qos qos] [--will-retain]]\n");
// #ifdef WITH_TLS
//     printf("                     [{--cafile file | --capath dir} [--cert file] [--key file]\n");
//     printf("                      [--ciphers ciphers] [--insecure]]\n");
// #ifdef WITH_TLS_PSK
//     printf("                     [--psk hex-key --psk-identity identity [--ciphers ciphers]]\n");
// #endif
// #endif
// #ifdef WITH_SOCKS
//     printf("                     [--proxy socks-url]\n");
// #endif
//     printf("       mosquitto_pub --help\n\n");
//     printf(" -A : bind the outgoing socket to this host/ip address. Use to control which interface\n");
//     printf("      the client communicates over.\n");
//     printf(" -d : enable debug messages.\n");
//     printf(" -f : send the contents of a file as the message.\n");
//     printf(" -h : mqtt host to connect to. Defaults to localhost.\n");
//     printf(" -i : id to use for this client. Defaults to mosquitto_pub_ appended with the process id.\n");
//     printf(" -I : define the client id as id_prefix appended with the process id. Useful for when the\n");
//     printf("      broker is using the clientid_prefixes option.\n");
//     printf(" -k : keep alive in seconds for this client. Defaults to 60.\n");
//     printf(" -L : specify user, password, hostname, port and topic as a URL in the form:\n");
//     printf("      mqtt(s)://[username[:password]@]host[:port]/topic\n");
//     printf(" -l : read messages from stdin, sending a separate message for each line.\n");
//     printf(" -m : message payload to send.\n");
//     printf(" -M : the maximum inflight messages for QoS 1/2..\n");
//     printf(" -n : send a null (zero length) message.\n");
//     printf(" -p : network port to connect to. Defaults to 1883 for plain MQTT and 8883 for MQTT over TLS.\n");
//     printf(" -P : provide a password\n");
//     printf(" -q : quality of service level to use for all messages. Defaults to 0.\n");
//     printf(" -r : message should be retained.\n");
//     printf(" -s : read message from stdin, sending the entire input as a message.\n");
// #ifdef WITH_SRV
//     printf(" -S : use SRV lookups to determine which host to connect to.\n");
// #endif
//     printf(" -t : mqtt topic to publish to.\n");
//     printf(" -u : provide a username\n");
//     printf(" -V : specify the version of the MQTT protocol to use when connecting.\n");
//     printf("      Can be mqttv31 or mqttv311. Defaults to mqttv311.\n");
//     printf(" --help : display this message.\n");
//     printf(" --quiet : don't print error messages.\n");
//     printf(" --will-payload : payload for the client Will, which is sent by the broker in case of\n");
//     printf("                  unexpected disconnection. If not given and will-topic is set, a zero\n");
//     printf("                  length message will be sent.\n");
//     printf(" --will-qos : QoS level for the client Will.\n");
//     printf(" --will-retain : if given, make the client Will retained.\n");
//     printf(" --will-topic : the topic on which to publish the client Will.\n");
// #ifdef WITH_TLS
//     printf(" --cafile : path to a file containing trusted CA certificates to enable encrypted\n");
//     printf("            communication.\n");
//     printf(" --capath : path to a directory containing trusted CA certificates to enable encrypted\n");
//     printf("            communication.\n");
//     printf(" --cert : client certificate for authentication, if required by server.\n");
//     printf(" --key : client private key for authentication, if required by server.\n");
//     printf(" --ciphers : openssl compatible list of TLS ciphers to support.\n");
//     printf(" --tls-version : TLS protocol version, can be one of tlsv1.2 tlsv1.1 or tlsv1.\n");
//     printf("                 Defaults to tlsv1.2 if available.\n");
//     printf(" --insecure : do not check that the server certificate hostname matches the remote\n");
//     printf("              hostname. Using this option means that you cannot be sure that the\n");
//     printf("              remote host is the server you wish to connect to and so is insecure.\n");
//     printf("              Do not use this option in a production environment.\n");
// #  ifdef WITH_TLS_PSK
//     printf(" --psk : pre-shared-key in hexadecimal (no leading 0x) to enable TLS-PSK mode.\n");
//     printf(" --psk-identity : client identity string for TLS-PSK mode.\n");
// #  endif
// #endif
// #ifdef WITH_SOCKS
//     printf(" --proxy : SOCKS5 proxy URL of the form:\n");
//     printf("           socks5h://[username[:password]@]hostname[:port]\n");
//     printf("           Only \"none\" and \"username\" authentication is supported.\n");
// #endif
//     printf("\nSee http://mosquitto.org/ for more information.\n\n");
// }

// int main_mqtt(int argc, char *argv[])
// {
//     struct mosq_config cfg;
//     struct mosquitto *mosq = NULL;
//     int rc;
//     int rc2;
//     char *buf, *buf2;
//     int buf_len = 1024;
//     int buf_len_actual;
//     int read_len;
//     int pos;

//     buf = malloc(buf_len);
//     if(!buf){
//         fprintf(stderr, "Error: Out of memory.\n");
//         return 1;
//     }

//     memset(&cfg, 0, sizeof(struct mosq_config));
//     rc = client_config_load(&cfg, CLIENT_PUB, argc, argv);
//     if(rc){
//         client_config_cleanup(&cfg);
//         if(rc == 2){
//             /* --help */
//             print_usage();
//         }else{
//             fprintf(stderr, "\nUse 'mosquitto_pub --help' to see usage.\n");
//         }
//         free(buf);
//         return 1;
//     }

//     topic = cfg.topic;
//     message = cfg.message;
//     msglen = cfg.msglen;
//     qos = cfg.qos;
//     retain = cfg.retain;
//     mode = cfg.pub_mode;
//     username = cfg.username;
//     password = cfg.password;
//     quiet = cfg.quiet;

//     if(cfg.pub_mode == MSGMODE_STDIN_FILE){
//         if(load_stdin()){
//             fprintf(stderr, "Error loading input from stdin.\n");
//             free(buf);
//             return 1;
//         }
//     }else if(cfg.file_input){
//         if(load_file(cfg.file_input)){
//             fprintf(stderr, "Error loading input file \"%s\".\n", cfg.file_input);
//             free(buf);
//             return 1;
//         }
//     }

//     if(!topic || mode == MSGMODE_NONE){
//         fprintf(stderr, "Error: Both topic and message must be supplied.\n");
//         print_usage();
//         free(buf);
//         return 1;
//     }


//     mosquitto_lib_init();

//     if(client_id_generate(&cfg, "mosqpub")){
//         free(buf);
//         return 1;
//     }

//     mosq = mosquitto_new(cfg.id, true, NULL);
//     if(!mosq){
//         switch(errno){
//             case ENOMEM:
//                 if(!quiet) fprintf(stderr, "Error: Out of memory.\n");
//                 break;
//             case EINVAL:
//                 if(!quiet) fprintf(stderr, "Error: Invalid id.\n");
//                 break;
//         }
//         mosquitto_lib_cleanup();
//         free(buf);
//         return 1;
//     }
//     if(cfg.debug){
//         mosquitto_log_callback_set(mosq, my_log_callback);
//     }
//     mosquitto_connect_callback_set(mosq, my_connect_callback);
//     mosquitto_disconnect_callback_set(mosq, my_disconnect_callback);
//     mosquitto_publish_callback_set(mosq, my_publish_callback);

//     if(client_opts_set(mosq, &cfg)){
//         free(buf);
//         return 1;
//     }
//     rc = client_connect(mosq, &cfg);
//     if(rc) return rc;

//     if(mode == MSGMODE_STDIN_LINE){
//         mosquitto_loop_start(mosq);
//     }

//     do{
//         if(mode == MSGMODE_STDIN_LINE){
//             if(status == STATUS_CONNACK_RECVD){
//                 pos = 0;
//                 read_len = buf_len;
//                 while(fgets(&buf[pos], read_len, stdin)){
//                     buf_len_actual = strlen(buf);
//                     if(buf[buf_len_actual-1] == '\n'){
//                         buf[buf_len_actual-1] = '\0';
//                         rc2 = mosquitto_publish(mosq, &mid_sent, topic, buf_len_actual-1, buf, qos, retain);
//                         if(rc2){
//                             if(!quiet) fprintf(stderr, "Error: Publish returned %d, disconnecting.\n", rc2);
//                             mosquitto_disconnect(mosq);
//                         }
//                         break;
//                     }else{
//                         buf_len += 1024;
//                         pos += 1023;
//                         read_len = 1024;
//                         buf2 = realloc(buf, buf_len);
//                         if(!buf2){
//                             free(buf);
//                             fprintf(stderr, "Error: Out of memory.\n");
//                             return 1;
//                         }
//                         buf = buf2;
//                     }
//                 }
//                 if(feof(stdin)){
//                     if(last_mid == -1){
//                         /* Empty file */
//                         mosquitto_disconnect(mosq);
//                         disconnect_sent = true;
//                         status = STATUS_DISCONNECTING;
//                     }else{
//                         last_mid = mid_sent;
//                         status = STATUS_WAITING;
//                     }
//                 }
//             }else if(status == STATUS_WAITING){
//                 if(last_mid_sent == last_mid && disconnect_sent == false){
//                     mosquitto_disconnect(mosq);
//                     disconnect_sent = true;
//                 }
// #ifdef WIN32
//                 Sleep(100);
// #else
//                 struct timespec ts;
//                 ts.tv_sec = 0;
//                 ts.tv_nsec = 100000000;
//                 nanosleep(&ts, NULL);
// #endif
//             }
//             rc = MOSQ_ERR_SUCCESS;
//         }else{
//             rc = mosquitto_loop(mosq, -1, 1);
//         }
//     }while(rc == MOSQ_ERR_SUCCESS && connected);

//     if(mode == MSGMODE_STDIN_LINE){
//         mosquitto_loop_stop(mosq, false);
//     }

//     if(message && mode == MSGMODE_FILE){
//         free(message);
//     }
//     mosquitto_destroy(mosq);
//     mosquitto_lib_cleanup();

//     if(rc){
//         fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
//     }
//     return rc;
// }
// */






