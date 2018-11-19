#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>   
//#include <net/if.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <asm/types.h>  
#include <linux/netlink.h>  
#include <linux/rtnetlink.h>
#include <stdlib.h>  
#include <stdio.h>  
#include <sys/ioctl.h>  
#include <linux/if.h>  
#include <string.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <math.h>
#include <time.h>


#include <dirent.h>
#include <printcolor.h>
#include <runconf.h>
#include <sysdebug.h>
#include <oled.h>
#include <devices.h>
#include <constmacro.h>
#include <bcm2835.h>
#include <app_main.h>
#include <cJSON.h>
#include <ad1256.h>
#include <sd_data.h>
#include <ltc2943.h>
#include <stdbool.h>
#include <i2crw.h>
#include <device_mq.h>
#include <ublox.h>
#include <pca9539_relay.h>
#include <alg.h>
#include <readconf.h>

extern int  main_ad(APP_S * app);
extern int oled_main(APP_S * app);
extern int gps_main(APP_S * app);
extern int hi219_main(APP_S * app);
extern int openltc2943(APP_S * app);
extern int udp_atcmd_main(APP_S * app);
extern int beep_init();
extern int open_pcf8574(APP_S * app);//复位设备
extern int open_pca9539(APP_S * app);//复位设备
extern int oledmain_wiring(APP_S * app);
extern int iws_client_frame(APP_S * app);
extern int iws_client(APP_S * app);
extern int iws_client1(APP_S * app);
extern int fs_monitor(APP_S * app);
extern int init_steim2buf(APP_S * app);
extern int init_status_out_buf(APP_S * app);
extern int make_data_main(APP_S * app);
extern int sd_steid2_main(APP_S * app);
extern int async_alg_main(APP_S *app);//异步算法处理过程
extern int async_alg_main_v20(APP_S *app);
extern void test_main();
extern int button_push();


extern struct gps_data_t gps_data;
char eth_ip[100];
char wlan_ip[100];
char eth_ip_now[100];
char wlan_ip_now[100];


int setbit(int num, int bit){
	return num|=(1<<bit);
}

int clearbit(int num, int bit){
	return num&=~(1<<bit);
}

int testbit(int num, int bit){
	return num&=(1<<bit);
}



char setbitchar(char num, int bit){
	return num|=(1<<bit);
}

char clearbitchar(char num, int bit){
	return num&=~(1<<bit);
}

char testbitchar(char num, int bit){
	return num&=(1<<bit);
}

APP_DEVICE_CONF_S dev_config=
{
	.spi_conf[0].mode=3,//spi模式
	.spi_conf[0].lsb=0x08,//spi模式
	.spi_conf[0].bits = 8,//传输数据位数
	.spi_conf[0].speed = 1000000,//时钟频率
	.spi_conf[0].delay=10,//发送延迟
	.spi_conf[1].mode=3,//spi模式
	.spi_conf[1].lsb=0x08,//spi模式
	.spi_conf[1].bits = 8,//传输数据位数
	.spi_conf[1].speed = 1000000,//时钟频率
	.spi_conf[1].delay=10,//发送延迟
};











TEST_S test=
{
	.name="a",
	.device="b",
	.dev_id=5,
	.test1.name="aa",
	.test1.device="bb",
	.test1.dev_id=6,


};







void init_base()
{	
	;
}
int maintail()
{
	;
}
int printinfo()
{


}





/*
typedef struct APP{//命令行参数
	TREAD_SWITCH_S thread_switch;
	CMD_LINE_OPT_S cmd_opt;
	APP_COUNT_S app_count;
	APP_PARA_S app_para;
}__attribute__ ((packed,aligned(1)))APP_S;
*/
int	thread_switch_init(TREAD_SWITCH_T thread_switch)//初始化线程开关
{
	thread_switch->main_loop=1;
	thread_switch->gps_loop=1;
	thread_switch->gpio_loop=1;
	thread_switch->ad_loop=1;
	thread_switch->tcp_server_loop=1;
	thread_switch->udp_beat_loop=1;
	thread_switch->udp_data_loop=1;
	thread_switch->file_loop=1;
	thread_switch->tcp_data_loop=1;
	thread_switch->file_name_loop=1;
	thread_switch->file_usyn_close_loop=1;
	thread_switch->oled_display_loop=1;
	thread_switch->keyboard_loop=1;
	thread_switch->usrwifi_loop=1;
	thread_switch->netvkey_loop=1;
}

int cmd_opt_init(CMD_LINE_OPT_S * cmd_opt)//初始化命令行参数
{
	cmd_opt->mode=0;
	cmd_opt->isdebug=0;
	cmd_opt->is_main_debug=0;
	cmd_opt->is_gps_debug=0;
	cmd_opt->is_ad_debug=0;
	cmd_opt->is_fileprocess_debug=0;
	cmd_opt->is_tcpserver_debug=0;
	cmd_opt->is_tcpdata_debug=0;
	cmd_opt->is_ad1_debug=0;
	cmd_opt->is_ad2_debug=0;
}
int app_count_init(APP_COUNT_S * app_count)//初始化程序计数器
{
	;	
}
/*
enum
{
	//Register address, followed by reset the default values 
	REG_STATUS = 0,	// x1H
	REG_MUX    = 1, // 01H
	REG_ADCON  = 2, // 20H
	REG_DRATE  = 3, // F0H
	REG_IO     = 4, // E0H
	REG_OFC0   = 5, // xxH
	REG_OFC1   = 6, // xxH
	REG_OFC2   = 7, // xxH
	REG_FSC0   = 8, // xxH
	REG_FSC1   = 9, // xxH
	REG_FSC2   = 10, // xxH
	CTRL_OPEN_CHNUM =11, //4
	CTRL_CH_MUX0 =12,//0x01
	CTRL_CH_MUX1 =13,//0x23
	CTRL_CH_MUX2 =14,//0x45
	CTRL_CH_MUX3 =15,//0x67
	CTRL_CH_MUX4 =16,//0x01
	CTRL_CH_MUX5 =17,//0x01
	CTRL_CH_MUX6 =18,//0x01
	CTRL_CH_MUX7 =19,//0x01
};

*/
int ad_para_init(AD_REG_S * ad_reg)//初始化硬件参数
{
	;
}

int ad_default_para_init(AD_REG_S * ad_reg_default)//初始化硬件参数
{
	ad_reg_default->reg[REG_STATUS]=0x01;
	ad_reg_default->reg[REG_MUX]=0x01;
	ad_reg_default->reg[REG_ADCON]=0x00;
	ad_reg_default->reg[REG_DRATE]=0xB0;
	ad_reg_default->reg[REG_IO]=0xE0;
	ad_reg_default->reg[REG_OFC0]=0x00;
	ad_reg_default->reg[REG_OFC1]=0x00;
	ad_reg_default->reg[REG_OFC2]=0x00;
	ad_reg_default->reg[REG_FSC0]=0x00;
	ad_reg_default->reg[REG_FSC1]=0x00;
	ad_reg_default->reg[REG_FSC2]=0x00;
	ad_reg_default->reg[CTRL_OPEN_CHNUM]=0x04;
	ad_reg_default->reg[CTRL_CH_MUX0]=0x01;
	ad_reg_default->reg[CTRL_CH_MUX1]=0x23;
	ad_reg_default->reg[CTRL_CH_MUX2]=0x45;
	ad_reg_default->reg[CTRL_CH_MUX3]=0x67;
	// ad_reg_default->reg[CTRL_CH_MUX0]=0x00;
	// ad_reg_default->reg[CTRL_CH_MUX1]=0x00;
	// ad_reg_default->reg[CTRL_CH_MUX2]=0x00;
	// ad_reg_default->reg[CTRL_CH_MUX3]=0x00;
	ad_reg_default->reg[CTRL_CH_MUX4]=0x01;
	ad_reg_default->reg[CTRL_CH_MUX5]=0x01;
	ad_reg_default->reg[CTRL_CH_MUX6]=0x01;
	ad_reg_default->reg[CTRL_CH_MUX7]=0x01;
	return 0;
}




int read_para_file_fromsd(APP_PARA_S * app_para)
{
	if(0==read_file2buf(app_para,sizeof(APP_PARA_S),IWSPARAFILE,IWSPARAPATH,"r")){//尝试读默认设置
		printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
		return 0;
	}
	else{
		if(0==read_file2buf(app_para,sizeof(APP_PARA_S),IWSPARAFILE_ORIG,IWSPARAPATH_ORIG,"r")){
			printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
			if(save_bufinfile(app_para,sizeof(APP_PARA_S),IWSPARAFILE,IWSPARAPATH,"w+")==0){//将出厂设置设置成默认配置
				printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
				//atcmd_com_OK_rt();
			}
			return 0;
		}
		else{//出厂设置也读不到的情况下
			printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
			ad_default_para_init(&app_para->ad_reg_default);//程序生成一套设置
			printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
			//int read_file2buf(char * buf,int len,const char * filename,const char * filepath,const char * mode)
			memcpy(&app_para->ad_reg,&app_para->ad_reg_default,sizeof(AD_REG_S));
			if(save_bufinfile(app_para,sizeof(APP_PARA_S),IWSPARAFILE,IWSPARAPATH,"w+")==0){//保存成默认设置
				printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
				//atcmd_com_OK_rt();
			}
			if(save_bufinfile(app_para,sizeof(APP_PARA_S),IWSPARAFILE_ORIG,IWSPARAPATH_ORIG,"w+")==0){//保存成出厂设置
				//atcmd_com_OK_rt();
				printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
			}
		}
		return 0;
	}

}




int app_para_init(APP_PARA_S * app_para)//初始化硬件参数
{
	if(read_para_file_fromsd(app_para)==0){
		printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
		return 0;
	}
	else{
		printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
		ad_default_para_init(&app_para->ad_reg_default);
		//int read_file2buf(char * buf,int len,const char * filename,const char * filepath,const char * mode)
		memcpy(&app_para->ad_reg,&app_para->ad_reg_default,sizeof(AD_REG_S));
	}
	printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
	ad_para_init(&app_para->ad_reg);
	return 0;
}

/*
int app_devices_init(APP_DEVICE_S * app_devices)//初始化所用到的硬件
{

	app_devices[SSC0_AD1282_ID].name="ads1282";
	app_devices[SSC0_AD1282_ID].device="/dev/zwp_ssc0";
	app_devices[SSC0_AD1282_ID].dev_id=SSC0_AD1282_ID;
	app_devices[SSC0_AD1282_ID].dev_use_count=0;

	app_devices[SSC1_AD1282_ID].name="ads1282";
	app_devices[SSC1_AD1282_ID].device="/dev/zwp_ssc0";
	app_devices[SSC1_AD1282_ID].dev_id=SSC0_AD1282_ID;
	app_devices[SSC1_AD1282_ID].dev_use_count=0;

	app_devices[SPI_FPGA_ID].name="fpga";
	app_devices[SPI_FPGA_ID].device="/dev/zwp_spi0.0";
	app_devices[SPI_FPGA_ID].dev_id=SPI_FPGA_ID;
	app_devices[SPI_FPGA_ID].dev_use_count=0;
	
	app_devices[SPI_OLED_ID].name="oled";
	app_devices[SPI_OLED_ID].device="/dev/zwp_spi0.0";
	app_devices[SPI_OLED_ID].dev_id=SPI_OLED_ID;
	app_devices[SPI_OLED_ID].dev_use_count=0;
	
	app_devices[IIC_2943_ID].name="ltc2943";
	app_devices[IIC_2943_ID].device="/dev/i2c-0";
	app_devices[IIC_2943_ID].dev_id=IIC_2943_ID;
	app_devices[IIC_2943_ID].dev_use_count=0;
	
	app_devices[UART_GPS_ID].name="gps";
	app_devices[UART_GPS_ID].device="/dev/ttyS1";
	app_devices[UART_GPS_ID].dev_id=UART_GPS_ID;
	app_devices[UART_GPS_ID].dev_use_count=0;

}


*/
int app_sys_var_init(SYNC_CTRL_S * app_sync)
{
	int ifor;
	for(ifor=0;ifor<SYNCNUM;ifor++)
	{
		pthread_mutex_init(&(app_sync->mut[ifor]),NULL);
		pthread_cond_init(&(app_sync->cond[ifor]),NULL);
	}
	return 0;
}















#define IWS_CONF_FILE "/etc/iws/iws.conf"

int init_read_conf_file()
{
	//read_conf_value(const char *key,char *value,const char *file);
	char port[8];
	char udpport[8];
	char buf[64];
	read_conf_value("BASE_PORT",port,IWS_CONF_FILE);
	sscanf(port,"%d",&app.app_server.base_port);
	read_conf_value("UDP_PORT",udpport,IWS_CONF_FILE);
	sscanf(udpport,"%d",&app.udp_port);
	printf("app.app_server.base_port=%d\n",app.app_server.base_port);
	read_conf_value("IS_NTP_SYNC",buf,IWS_CONF_FILE);
	sscanf(buf,"%d",&app.iws_server_share.status.is_clock_sync_ntp);
	printf("app.iws_server_share.status.is_clock_sync_ntp=%d\n",app.iws_server_share.status.is_clock_sync_ntp);
	// read_conf_value("BASE_PORT",port,IWS_CONF_FILE);
	// sscanf(port,"%d",&app.app_server.base_port);
	// printf("app.app_server.base_port=%d\n",app.app_server.base_port);
	// read_conf_value("BASE_PORT",port,IWS_CONF_FILE);
	// sscanf(port,"%d",&app.app_server.base_port);
	// printf("app.app_server.base_port=%d\n",app.app_server.base_port);
	return 0;
}


int init_app_struct()
{
	memset(&app,0,sizeof(APP_S));
	app.self=&app;
}


int init_app_files(FILE_CENTER_S * app_files)
{
	snprintf(app_files[APP_LOG_FILE].path,100,"%s","/home/pi/userdata/log");
    snprintf(app_files[APP_LOG_FILE].suffix,8,"%s","log");
	app_files->pid=getpid();
	app_files->ppid=getppid();
	return 0;
}


int init_conf()
{
	thread_switch_init(&app.thread_switch);
	cmd_opt_init(&app.cmd_opt);
	app_count_init(&app.app_count);


	init_read_conf_file();
	app_para_init(&app.app_para);
	app_sys_var_init(&app.app_sync);
	init_mut_cond(&app);
	app.xishu_globe=50;//1711.92421;
	return 0;
}






int debug_test()
{

	printf(GREEN"test.name=%s\n"NONE,test.name);
	printf(GREEN"test.device=%s\n"NONE,test.device);
	printf(GREEN"test.dev_id=%d\n"NONE,test.dev_id);
	printf(GREEN"test.test1.name=%s\n"NONE,test.test1.name);
	printf(GREEN"test.test1.device=%s\n"NONE,test.test1.device);
	printf(GREEN"test.test1.dev_id=%d\n"NONE,test.test1.dev_id);
	//exit(0);

}

int start_devices()
{
	app_display_init("/dev/zwp_spi1.1",NULL);
}



int osonewthread(pthread_t * newthread,int  (* threadfun)(void *),void * paras)
{
	printf(GREEN"*paras=%d\n"NONE,*(int *)paras);
	if ( pthread_create(newthread, NULL, threadfun,paras) ){
		printf("error creating gpsthread.");
		return -1;
	}
	printf(PURPLE"UDP DATA THREAD DONE\n"NONE);	
	return 0;
}


/*
struct OMA_PAK
{
	char start_label[4];//$OMA
	int length;//type-checksum
	int type;//for pak filter
	int flag;//
	char data[length-10];//length-type-flag-checksum
	char checksum[2];//
	char end_label[4];//$AMO

};
*/

int json_conf_callback(int size,char * buf,void *buff,APP_S * app)
{   

    //    char * json_str="{\
    \"filename\":\"/etc/abl.conf\",\
    \"type\":\"read\",\
    \"configs\":[{\"key\":\"a\",\"value\":\"2\"},{\"key\":\"b\",\"value\":\"3\"}]\
}";

    int ifor;
    if(!check_pak_conf(size,buf)){
        cJSON * root = NULL;
        root=cJSON_Parse(buf);
        if(root==NULL)
            return -1;
        cJSON *filename = cJSON_GetObjectItemCaseSensitive(root, "filename");
        cJSON *type = cJSON_GetObjectItemCaseSensitive(root, "type");
        cJSON *configs = cJSON_GetObjectItemCaseSensitive(root, "configs");
        int confignum = cJSON_GetArraySize(configs);
        for(ifor=0;ifor<confignum;ifor++)
        {
            cJSON * config=cJSON_GetArrayItem(configs,ifor);
            cJSON * key = cJSON_GetObjectItemCaseSensitive(config, "key");
            cJSON * value = cJSON_GetObjectItemCaseSensitive(config, "value");
            printf("key=%s value=%s\n",key->valuestring,value->valuestring);
            write_conf_value(key->valuestring,value->valuestring,filename->valuestring);
        }
        cJSON_Delete(root);
    }
}










int at_process_frame(int size,char * buf,void *buff,APP_S * app)
{
	printf(YELLOW"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
}


int shell_process_frame(int size,char * buf,void *buff,APP_S * app)
{
	printf(BLUE"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
}

int oma_process_frame(int size,char * buf,void *buff,APP_S * app)
{
	printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
}



int json_process_frame(int size,char * buf,void *buff,APP_S * app)
{
	json_conf_callback(size,buf,buff,app);
	printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
}
int conf_process_frame(int size,char * buf,void *buff,APP_S * app)
{
	printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
}





struct OMA_PAK
{
	char start_label[4];//$OMA
	int length;//type-checksum
	int type;//for pak filter
	int flag;//
};
//!shell+df -h+
//!at+start+


#define PAK_TYPE_AT 0
#define PAK_TYPE_SHELL 1
#define PAK_TYPE_OMA 2
#define PAK_TYPE_JSON_CONF 3
#define PAK_TYPE_CONF 4













int tcp_process_frame(int size,int sockfd,short port,char * buf,void *buff,APP_S * app)
{
	printf(YELLOW"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
	int ifor;
	struct OMA_PAK * pak_head;
	pak_head=(struct OMA_PAK *)buf;

	switch(pak_head->type)
	{
		case PAK_TYPE_AT:
		at_process_frame(size,buf,buff,app);
		break;
		case PAK_TYPE_SHELL:
		shell_process_frame(size,buf,buff,app);
		break;
		case PAK_TYPE_OMA:
		oma_process_frame(size,buf,buff,app);
		break;
		case PAK_TYPE_JSON_CONF:
		json_process_frame(size,buf,buff,app);
		break;
		case PAK_TYPE_CONF:
		conf_process_frame(size,buf,buff,app);
		break;
		default:
		break;
	};

}

short base_port=7000;


int app_server_start_ctrl(APP_S * app)
{
	printf(YELLOW"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

	start_a_tcp_ser(app->app_server.base_port,tcp_process_frame,app);

	printf(YELLOW"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
	return 0;
}



int tcp_process_client_manager_self(APP_S * app)
{
	pthread_t  newthread;

	int ii=osonewthread(&newthread,app_server_start_ctrl,(void * )app);
	//int ret = send(sockfd, "OK", 2, 0);
	//exit(0);
	sleep(1);
	base_port++;
	return 0;
}






int mq_gps_main(APP_S * app)
{
	udp_frame(app,9999,handle_udp_gps);

}





int gpsd_gps_main(APP_S * app)
{
	udp_frame(app,4000,handle_gpsd_gps_nmea);

}









//be43cb901d9cf96ec4b0a41338e9f4ad43ed1572  1


int tcp_process_client_manager(int size,int sockfd,short port,char * buf,void *buff,APP_S * app)
{
	printf(YELLOW"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
	pthread_t  newthread;
	int ii=osonewthread(&newthread,app_server_start_ctrl,(void * )app);
	//int ret = send(sockfd, "OK", 2, 0);
	sleep(1);
	base_port++;
	write(sockfd,&(base_port), 2);
	printf(YELLOW"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
	return 0;
}

























int app_server_start_oled(APP_S * app)
{
	start_a_tcp_ser(6558,tcp_process_frame,app);
}


int app_server_start()
{
	pthread_t  newthread;
	int ii=osonewthread(&newthread,app_server_start_oled,(void * )&app);
	int * retStr = NULL; 
	printf(GREEN"&retStr=%u\n"NONE, &retStr);
	printf(GREEN"retStr=%u\n"NONE, retStr); 
    pthread_join(newthread, (void**)&retStr);
}


int app_add_newthread(pthread_t * newthread,int  (* threadfun)(void *),void * paras)
{
	if ( pthread_create(newthread, NULL, threadfun, paras) ){
		printf("error creating gpsthread.");
		return -1;
	}
	printf(PURPLE"UDP DATA THREAD DONE\n"NONE);	
	return 0;
}




extern int json_test(void);




int app_server_start_test(APP_S * app)
{
	start_a_tcp_ser_et(6559,tcp_process_client_manager,app);
}





// int l_data;
// #define  BIT_MASK(nr)      ( 1UL <<  ( (nr) % BITS_PER_LONG) )
// void SetBit(int pos)//将某一位置位的操作
// {
// 	if((pos >= 0) && (pos <= 31))
// 	{
// 		l_data |= BIT_MASK(pos);//BIT_MASK是 掩码。就是除了这一位其余位均为0

// 	}
// }
// void ClrBit(int pos)//将某一位清0的操作
// {
// 	if((pos >= 0) && (pos <= 31))
// 	{
// 		l_data &= ~BIT_MASK(pos);
// 	}
// }






int getip_main(APP_S * app)
{	while(1){
		sleep(1);
		getlocalips(eth_ip_now,wlan_ip_now);
		//printf(BLUE"eth=%s,wlan=%s\n"NONE,eth_ip_now,wlan_ip_now);
	}
	return 0;
}




void log_file_scan(int index,char * fileurl)
{
	// char index_str[16];
	// sscanf(fileurl,"/home/pi/userdata/log/%[0-9].",index_str);
	// sscanf(index_str,"%d",&app.iws_file_list[ifor].file_index);

}


int init_log_env(IWS_LOG_FRAME * iws_log_frame)
{	
    snprintf(iws_log_frame->path,256,"%s","/home/pi/userdata/log");
    iws_log_frame->file_index=0;
    struct dirent **namelist;
    char file[128];
    char file_url[256];
    char date_time_str[64];
	struct timespec date_time;//数据采集时间
	

	  int ic,n = scandir(iws_log_frame->path,&namelist,0,alphasort);

	  if(n < 0)
	  { 
	      printf("scandir return \n");
	  }
	  else
	  {
	  	sscanf(namelist[n-1]->d_name,"%d",&iws_log_frame->file_index);
	  	
	      int index=0;
	      while(index < n)
	      {
	          //printf("d_ino：%ld  d_off:%ld d_name: %s\n", namelist[index]->d_ino,namelist[index]->d_off,namelist[index]->d_name);
	          //snprintf(file_url,1000,"%s/%s",basePath,namelist[index]->d_name);
	          //(*callbackfun)(ic,file_url);
	          
	          ic++;
	          free(namelist[index]);
	          index++;
	      }
	      free(namelist);
	  }
	  iws_log_frame->file_index=iws_log_frame->file_index+1;
	  clock_gettime(CLOCK_REALTIME, &date_time);
	  get_time_str1(date_time.tv_sec,date_time_str);
		snprintf(file_url,200,"%s/%04d_%s.log",iws_log_frame->path,iws_log_frame->file_index,date_time_str);//0614正常使用模式
		printf("log file is %s\n",file_url);
	//exit(0);
	iws_log_frame->fp=fopen(file_url,"w+");
	setbuf(iws_log_frame->fp,NULL);
	printf("log file is %s\n",file_url);
	if(iws_log_frame->fp!=NULL){
		printf("log file is %s\n",file_url);
		iws_log_frame->is_file_open=1;
		return 0;
	}
	else{
		iws_log_frame->is_file_open=0;
		return -1;
	}
    //myscandir(iws_log_frame->path,0,log_file_scan);
    return 0;
}















int open_new_log_file(IWS_LOG_FRAME * iws_log_frame)
{	
    char file[128];
    char file_url[256];
    char date_time_str[64];
	struct timespec date_time;//数据采集时间
	clock_gettime(CLOCK_REALTIME, &date_time);
	get_time_str1(date_time.tv_sec,date_time_str);
	snprintf(file_url,200,"%s/%04d_%s.log",iws_log_frame->path,iws_log_frame->file_index,date_time_str);//0614正常使用模式
	iws_log_frame->fp=fopen(file_url,"w+");
	setbuf(iws_log_frame->fp,NULL);
	if(iws_log_frame->fp!=NULL){
		printf("log file is %s\n",file_url);
		iws_log_frame->is_file_open=1;
		return 0;
	}
	else{
		iws_log_frame->is_file_open=0;
		return -1;
	}
    //myscandir(iws_log_frame->path,0,log_file_scan);
    return 0;
}



int mk_log_time_mark(IWS_LOG_FRAME * iws_log_frame)
{
	struct timespec date_time;//数据采集时间
	char date_time_str[64];
	int realwrite;
	clock_gettime(CLOCK_REALTIME, &date_time);
	get_time_str1(date_time.tv_sec,date_time_str);
	fprintf(iws_log_frame->fp, "%s\n", date_time_str);
	//realwrite=mywrite(date_time_str,strlen(date_time_str),iws_log_frame->fp);
	return 0;

}


int write_log_to_file(IWS_LOG_FRAME * iws_log_frame)
{
	int ifor;
	int realwrite;
	int is_time_mark=0;
	for(ifor=0;ifor<100;ifor++)
	  {
	  	//printf(RED"try write log ifor=%d,iws_log_frame->iws_log[ifor].is_log_ready=%d\n"NONE,ifor,iws_log_frame->iws_log[ifor].is_log_ready);
	  	if(iws_log_frame->iws_log[ifor].is_log_ready==1){
	  		if(is_time_mark==0){
	  			mk_log_time_mark(iws_log_frame);
	  			is_time_mark=1;
	  		}
			realwrite=mywrite(iws_log_frame->iws_log[ifor].logstr,iws_log_frame->iws_log[ifor].len,iws_log_frame->fp);
	  		iws_log_frame->iws_log[ifor].is_log_ready=0;
	  		//printf("HAVE write to log file realwrite=%d\n",realwrite);
	  		iws_log_frame->count++;
	  	}
	  }
	  if(is_time_mark==1){
	  	fprintf(iws_log_frame->fp, "%s\n\n", "END THIS LOG");
	  }
	  
	  return 0;
}

int log_main(APP_S * app)
{
	init_log_env(&app->iws_log_frame);

	for(;;)
	{	if(app->iws_log_frame.is_file_open==1){
			write_log_to_file(&app->iws_log_frame);
		}

		if(app->iws_log_frame.count>1000){
			app->iws_log_frame.count=0;
			app->iws_log_frame.file_index=app->iws_log_frame.file_index+1;
			fclose(app->iws_log_frame.fp);
			open_new_log_file(&app->iws_log_frame);
		}
		sleep(1);
    //return 0;
		//printf("%s\n",getenv("PATH"));
		//printf("ntp status=%s\n",getenv("TRY_RESTARTTIMES"));
		//system("env > env.txt");
		//exit(0);
	}
}

int ntp_info_parse(char *buf)
{
;
}


int ntp_check_main(APP_S * app)
{
   FILE   *stream; 
   FILE   *wstream;
   char   buf[1024]; 
     
    //wstream = fopen( "test_popen.txt", "w+"); //新建一个可写的文件
    for(;;){
       memset( buf, '\0', sizeof(buf) );//初始化buf,以免后面写如乱码到文件中
    	stream = popen( "ntpq -p", "r" ); //将“ls －l”命令的输出 通过管道读取（“r”参数）到FILE* stream

	    fread( buf, sizeof(char), sizeof(buf), stream); //将刚刚FILE* stream的数据流读取到buf中
	    //fwrite( buf, 1, sizeof(buf), wstream );//将buf中的数据写到FILE    *wstream对应的流中，也是写到文件中
	   	//printf("buf=%s\n",buf);
	   	ntp_info_parse(buf);
	   	pclose( stream );
	   	sleep(10);
    }
}

int app_thread_init(THREAD_COTR_S * app_thread)
{
	printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

/*
typedef struct THREAD_COTR{//命令行参数
	char name[20]; //线程名字
	pthread_t app_thread;
	int (* thread_func)(void *);//线程函数
	void * para;//参数
	int run_status;//当前运行状态;
	int conf_status;//配置开关
	int dependent;//运行所依赖的线程
	int control[20];
}__attribute__ ((packed,aligned(1)))THREAD_COTR_S;
*/	
	strcpy(app_thread[0].name,"oled_tcp_server");
	app_thread[0].thread_func=app_server_start_oled;
	app_thread[0].para=&app;
	app_thread[0].run_status=0;
	app_thread[0].conf_status=0;
	app_thread[0].dependent=0;



	strcpy(app_thread[1].name,"ads1256_server");
	app_thread[1].thread_func=main_ad;
	app_thread[1].para=&app;
	app_thread[1].run_status=0;
	app_thread[1].conf_status=1;
	app_thread[1].dependent=0;

	strcpy(app_thread[2].name,"tcp_server_test");
	app_thread[2].thread_func=app_server_start_test;
	app_thread[2].para=&app;
	app_thread[2].run_status=0;
	app_thread[2].conf_status=0;
	app_thread[2].dependent=0;




	//tcp_process_client_manager_self
	strcpy(app_thread[3].name,"base data server");
	app_thread[3].thread_func=tcp_process_client_manager_self;
	app_thread[3].para=&app;
	app_thread[3].run_status=0;
	app_thread[3].conf_status=1;
	app_thread[3].dependent=0;	



	//tcp_process_client_manager_self
	strcpy(app_thread[4].name,"oled test server");
	app_thread[4].thread_func=oledmain_wiring;
	app_thread[4].para=&app;
	app_thread[4].run_status=0;
	app_thread[4].conf_status=1;
	app_thread[4].dependent=0;
	//tcp_process_client_manager_self
	strcpy(app_thread[5].name,"gps-ublox server");
	app_thread[5].thread_func=gps_main;
	app_thread[5].para=&app;
	app_thread[5].run_status=0;
	app_thread[5].conf_status=0;
	app_thread[5].dependent=0;


	strcpy(app_thread[6].name,"hi219 server");
	app_thread[6].thread_func=hi219_main;
	app_thread[6].para=&app;
	app_thread[6].run_status=0;
	app_thread[6].conf_status=0;
	app_thread[6].dependent=0;

	strcpy(app_thread[7].name,"save data server");
	app_thread[7].thread_func=sd_data_main;
	app_thread[7].para=&app;
	app_thread[7].run_status=0;
	app_thread[7].conf_status=1;
	app_thread[7].dependent=0;
	strcpy(app_thread[8].name,"udp atcmd server");
	app_thread[8].thread_func=udp_atcmd_main;
	app_thread[8].para=&app;
	app_thread[8].run_status=0;
	app_thread[8].conf_status=1;
	app_thread[8].dependent=0;

	strcpy(app_thread[9].name,"pca9539 server");
	app_thread[9].thread_func=bcm_open_pca9539;
	app_thread[9].para=&app;
	app_thread[9].run_status=0;
	app_thread[9].conf_status=1;
	app_thread[9].dependent=0;



	strcpy(app_thread[10].name,"mq_gps_main server");
	app_thread[10].thread_func=mq_gps_main;
	app_thread[10].para=&app;
	app_thread[10].run_status=0;
	app_thread[10].conf_status=0;
	app_thread[10].dependent=0;



	strcpy(app_thread[11].name,"getip_main server");
	app_thread[11].thread_func=getip_main;
	app_thread[11].para=&app;
	app_thread[11].run_status=0;
	app_thread[11].conf_status=1;
	app_thread[11].dependent=0;
	//getip_main



	strcpy(app_thread[12].name,"gpsd_gps_main server");
	app_thread[12].thread_func=gpsd_gps_main;
	app_thread[12].para=&app;
	app_thread[12].run_status=0;
	app_thread[12].conf_status=1;
	app_thread[12].dependent=0;
	//gpsd_gps_main

//int iws_client(APP_S * app)


	strcpy(app_thread[13].name,"cstp0 server");
	app_thread[13].thread_func=iws_client1;
	app_thread[13].para=&app;
	app_thread[13].run_status=0;
	app_thread[13].conf_status=1;
	app_thread[13].dependent=0;
	app_thread[13].index=0;
	strcpy(app_thread[14].name,"cstp1 server");
	app_thread[14].thread_func=iws_client1;
	app_thread[14].para=&app;
	app_thread[14].run_status=0;
	app_thread[14].conf_status=1;
	app_thread[14].dependent=0;
	app_thread[14].index=0;
	strcpy(app_thread[13].name,"cstp2 server");
	app_thread[15].thread_func=iws_client1;
	app_thread[15].para=&app;
	app_thread[15].run_status=0;
	app_thread[15].conf_status=1;
	app_thread[15].dependent=0;
	app_thread[15].index=0;
	strcpy(app_thread[16].name,"cstp3 server");
	app_thread[16].thread_func=iws_client1;
	app_thread[16].para=&app;
	app_thread[16].run_status=0;
	app_thread[16].conf_status=1;
	app_thread[16].dependent=0;
	app_thread[16].index=0;
	strcpy(app_thread[17].name,"cstp4 server");
	app_thread[17].thread_func=iws_client1;
	app_thread[17].para=&app;
	app_thread[17].run_status=0;
	app_thread[17].conf_status=1;
	app_thread[17].dependent=0;
	app_thread[17].index=0;
	strcpy(app_thread[18].name,"cstp5 server");
	app_thread[18].thread_func=iws_client1;
	app_thread[18].para=&app;
	app_thread[18].run_status=0;
	app_thread[18].conf_status=1;
	app_thread[18].dependent=0;
	app_thread[18].index=0;

	strcpy(app_thread[19].name,"fs_monitor server");
	app_thread[19].thread_func=fs_monitor;
	app_thread[19].para=&app;
	app_thread[19].run_status=0;
	app_thread[19].conf_status=1;
	app_thread[19].dependent=0;
	app_thread[19].index=0;
//int  make_data_main(APP_S * app)



	strcpy(app_thread[20].name,"make data server");
	app_thread[20].thread_func=make_data_main;
	app_thread[20].para=&app;
	app_thread[20].run_status=0;
	app_thread[20].conf_status=1;
	app_thread[20].dependent=0;
	app_thread[20].index=0;




	strcpy(app_thread[21].name,"sd_steid2_main data server");
	app_thread[21].thread_func=sd_steid2_main;
	app_thread[21].para=&app;
	app_thread[21].run_status=0;
	app_thread[21].conf_status=1;
	app_thread[21].dependent=0;
	app_thread[21].index=0;


	strcpy(app_thread[22].name,"async_alg_main data server");
	app_thread[22].thread_func=async_alg_main_v20;
	app_thread[22].para=&app;
	app_thread[22].run_status=0;
	app_thread[22].conf_status=1;
	app_thread[22].dependent=0;
	app_thread[22].index=0;


	strcpy(app_thread[23].name,"log server");
	app_thread[23].thread_func=log_main;
	app_thread[23].para=&app;
	app_thread[23].run_status=0;
	app_thread[23].conf_status=1;
	app_thread[23].dependent=0;
	app_thread[23].index=0;

	strcpy(app_thread[24].name,"ntp_check server");
	app_thread[24].thread_func=ntp_check_main;
	app_thread[24].para=&app;
	app_thread[24].run_status=0;
	app_thread[24].conf_status=1;
	app_thread[24].dependent=0;
	app_thread[24].index=0;

//int async_alg_main(APP_S *app)//异步算法处理过程
	return 0;
//int  sd_steid2_main(APP_S * app)
	//sd_data_main
}



int app_thread_start(THREAD_COTR_S * app_thread)
{
	//printf(YELLOW"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
	printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
/*
typedef struct THREAD_COTR{//命令行参数
	char name[20]; //线程名字
	pthread_t app_thread;
	int (* thread_func)(void *);//线程函数
	void * para;//参数
	int run_status;//当前运行状态;
	int conf_status;//配置开关
	int dependent;//运行所依赖的线程
	int control[20];
}__attribute__ ((packed,aligned(1)))THREAD_COTR_S;
*/	int ifor;
	for(ifor=0;ifor<25;ifor++)
	{
		if(app_thread[ifor].run_status==0&&app_thread[ifor].conf_status==1){
			printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
			if ( pthread_create(&app_thread[ifor].app_thread_id, NULL, app_thread[ifor].thread_func, app_thread[ifor].para) ){
				printf("error creating %s thread\n",app_thread[ifor].name);
			return -1;
			}
			printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
			app_thread[ifor].run_status=1;
			app_thread[ifor].tid=pthread_self();
			printf("creating %s thread ok\n",app_thread[ifor].name);
		}
	}
	return 0;

}

int app_thread_join(THREAD_COTR_S * app_thread)
{
	printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

/*
typedef struct THREAD_COTR{//命令行参数
	char name[20]; //线程名字
	pthread_t app_thread;
	int (* thread_func)(void *);//线程函数
	void * para;//参数
	int run_status;//当前运行状态;
	int conf_status;//配置开关
	int dependent;//运行所依赖的线程
	int control[20];
}__attribute__ ((packed,aligned(1)))THREAD_COTR_S;
*/	int ifor;
	for(ifor=0;ifor<23;ifor++)
	{
		int * retStr = NULL; 
		printf(GREEN"&retStr=%u\n"NONE, &retStr);
		printf(GREEN"retStr=%u\n"NONE, retStr); 
	    pthread_join(app_thread[ifor].app_thread_id, (void**)&retStr);
		printf("%s thread end\n",app_thread[ifor].name);
	
	}
	return 0;

}



	//pthread_mutex_init(&(myoso->mut[MUT_FILE_CLOSE]),NULL);
	//pthread_cond_init(&(myoso->cond[COND_FILE_CLOSE]),NULL);

int send_go_signal(int to_who)
{
		pthread_mutex_lock(&app.app_sync.mut[to_who]);
		pthread_cond_broadcast(&app.app_sync.cond[to_who]);
		pthread_mutex_unlock(&app.app_sync.mut[to_who]);
		return 0;

}




int wait_go_signal(int wait_who)
{
	printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
	pthread_mutex_lock(&(app.app_sync.mut[wait_who]));
	printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
	pthread_cond_wait(&(app.app_sync.cond[wait_who]), &(app.app_sync.mut[wait_who]));
	pthread_mutex_unlock(&(app.app_sync.mut[wait_who]));
	return 0;
}



int reset_ad()
{
    bcm2835_gpio_fsel(RST, BCM2835_GPIO_FSEL_OUTP);//
	RST_1();
	bsp_DelayUS(1000);
	RST_0();
	bsp_DelayUS(100);
	RST_1();
	return 0;
}

int reset_everything()
{
	//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
    bcm2835_gpio_fsel(RST, BCM2835_GPIO_FSEL_OUTP);//
	//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
	//bsp_DelayUS(1000);
	bcm2835_delay(100);
	RST_1();
	bcm2835_delay(100);
	//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
	//bsp_DelayUS(1000);
	//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
	RST_0();
	//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
	//bsp_DelayUS(1000);
	bcm2835_delay(100);
	RST_1();
		//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

	return 0;
}


int beep_init()
{

    bcm2835_gpio_fsel(BEEP, BCM2835_GPIO_FSEL_OUTP);//
	BEEP_ON();
		printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

	bsp_DelayUS(1000*1000);
		printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

	BEEP_OFF();
		printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

	bsp_DelayUS(1000*1000);
		printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

	return 0;

}


int wiring_bcm2835_init()
{
	if(app.app_sign.has_bcm2835_init==0){
		if(wiringPiSetupGpio() < 0){
			printf("wiringPiSetup error\n");
			exit(1);
		}
		printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
		if (!bcm2835_init()){
			printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

			app.app_sign.has_bcm2835_init=0;
			printf("bcm2835_init error\n");
			exit(1);
		}
		else{
			printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
			//beep_init();
			printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

			app.app_sign.has_bcm2835_init=1;
		}
		printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

		//reset_ad();
		//reset_pca9539();
		//reset_everything();
	}
	else{
		//app.app_sign.has_bcm2835_init=1;
	}
	printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

/*
	pthread_mutex_init(&(myoso->mut[MUT_CLIENT_FILE]),NULL);
	pthread_mutex_init(&(myoso->mut[MUT_TCP_DATA]),NULL);
	pthread_mutex_init(&(myoso->mut[MUTMAIN]),NULL);
	pthread_mutex_init(&(myoso->mut[MUTALL]),NULL);
	pthread_mutex_init(&(myoso->mut[MUTGPIO]),NULL);
	pthread_mutex_init(&(myoso->mut[MUTREAD]),NULL);
	pthread_cond_init(&(myoso->cond[CONDREAD]),NULL);
	pthread_cond_init(&(myoso->cond[CONDGPIO]),NULL);
	pthread_cond_init(&(myoso->cond[COND_CLIENT_FILE]),NULL);
	pthread_cond_init(&(myoso->cond[COND_TCP_DATA]),NULL);
	pthread_cond_init(&(myoso->cond[CONDALL]),NULL);
	pthread_cond_init(&(myoso->cond[CONDMAIN]),NULL);

*/


int init_mut_cond(APP_S * app)
{
	int ifor;
	for(ifor=0;ifor<20;ifor++)
	{
		pthread_mutex_init(&(app->app_mut[ifor].mut),NULL);
		app->app_mut[ifor].i=ifor;
		pthread_cond_init(&(app->app_cond[ifor].cond),NULL);
		app->app_cond[ifor].i=ifor;
	}
}

// #define  RELAY  RPI_GPIO_P1_16     //P4
// #define  SENSOR  RPI_GPIO_P1_22     //P4
// #define RELAY_EN()     bcm2835_gpio_write(RELAY,HIGH)
// #define RELAY_UNEN()     bcm2835_gpio_write(RELAY,LOW)
// #define SENSOR_EN()     bcm2835_gpio_write(SENSOR,HIGH)
// #define SENSOR_UNEN()     bcm2835_gpio_write(SENSOR,LOW)
//#define BEEP RPI_V2_GPIO_P1_37

int beep_pin_init()
{
    if(app.app_sign.has_bcm2835_init==0){
        wiring_bcm2835_init();
    }
    bcm2835_gpio_fsel(BEEP, BCM2835_GPIO_FSEL_OUTP);//
    return 0;
}









int init_device_pin()
{
	oled_pin_init();//初始化oled管脚
	pca9539_pin_init();
	beep_pin_init();
}

int getlocalips(char * eth0_ip,char * wlan0_ip) {//获取本地网卡信息
    struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct!=NULL) {
         if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            //printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer); 

			if(strstr(ifAddrStruct->ifa_name,"eth0")!=NULL){
			//memcpy(eth0_ip,addressBuffer,strlen(addressBuffer));
				if(strstr(addressBuffer,".")!=NULL){
					snprintf(eth0_ip,100,"eth0:%s",addressBuffer);
				}
			 } 
			else if((ifAddrStruct->ifa_name,"wlan0")!=NULL){
				if(strstr(addressBuffer,"192")!=NULL){
					snprintf(wlan0_ip,100,"wlan0:%s",addressBuffer);
				}
			//memcpy(wlan0_ip,addressBuffer,strlen(addressBuffer));
			}
		}
        else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // // is a valid IP6 Address
            // tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            // char addressBuffer[INET6_ADDRSTRLEN];
            // inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            // printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer); 
        } 
        ifAddrStruct=ifAddrStruct->ifa_next;
    }
    return 0;
}

// int check_eth0()  
// {  
//     int fd, retval;  
//     char buf[BUFLEN] = {0};  
//     int len = BUFLEN;  
//     struct sockaddr_nl addr;  
//     struct nlmsghdr *nh;  
//     struct ifinfomsg *ifinfo;  
//     struct rtattr *attr;  
  
//     fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);  
//     setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &len, sizeof(len));  
//     memset(&addr, 0, sizeof(addr));  
//     addr.nl_family = AF_NETLINK;  
//     addr.nl_groups = RTNLGRP_LINK;  
//     bind(fd, (struct sockaddr*)&addr, sizeof(addr));  
//     while ((retval = read(fd, buf, BUFLEN)) > 0)  
//     {  
// 	printf("ii\n");
//         for (nh = (struct nlmsghdr *)buf; NLMSG_OK(nh, retval); nh = NLMSG_NEXT(nh, retval))  
//         {  
// 	printf("22\n");
//             if (nh->nlmsg_type == NLMSG_DONE)  
//                 break;  
//             else if (nh->nlmsg_type == NLMSG_ERROR)  
//                 return;  
//             else if (nh->nlmsg_type != RTM_NEWLINK)  
//                 continue;  
//             ifinfo = NLMSG_DATA(nh);  
//             sprintf(eth_status_str,"%s",
//                     (ifinfo->ifi_flags & IFF_LOWER_UP) ? "O" : "X" );
// 		update_net_info2ledbuff();
// 		isneedupdate=1;
//             attr = (struct rtattr*)(((char*)nh) + NLMSG_SPACE(sizeof(*ifinfo)));  
//             len = nh->nlmsg_len - NLMSG_SPACE(sizeof(*ifinfo));  
//             for (; RTA_OK(attr, len); attr = RTA_NEXT(attr, len))  
//             {  
//                 if (attr->rta_type == IFLA_IFNAME)  
//                 {  
// 	printf("33\n");
//                     printf(" %s", (char*)RTA_DATA(attr));  
//                     break;  
//                 }  
//             }  
// //                printf("\n");  
//         }  
//     }  
//   close(fd);
//     return 0;  
// }  



extern int gpsd_client_frame();


// struct dhcp_conf{

// }

int get_dhcpcd_conf()
{
	;
	//sed -i 's/被替换的内容/要替换成的内容/' file
}






int psa_test()
	{
	float data[100000];
	double pi=3.14159265358979323846264338327950288;
	int i;
	float result;
	float result1[1];
	float period[1];
	period[0]=1;
	result1[0]=0;
	result=0;
	for(i=0;i<100000;i++)
		{
		data[i]=2*sin(2*pi*i/100)+sin(2*pi*i/1000);
		}


	//int psa(float *data,float &result,int num,float period ,int sample_rata);
	//这个速度慢 
	//data 各分量加速度测量数组 
	//num 测量数据长度
	//result 结果
	//sample_rata 采样率
	//period 需要计算的周期
	psa(&data[0],&result,1000,1, 100);



	//int psa_2(float *data,int data_num,float sample_rata,float zuni,float *period,int period_num,float *result);
	//计算PSA  使用psa2 速度更快
	//data 各分量加速度测量数组 
	//data_num 测量数据长度
	//sample_rata 采样率
	//zuni 阻尼
	//period 需要计算的周期数组 因为 0.3 1 3（秒）
	//result 对应的结果数组
	psa_2(&data[0],100000,100,0.05,&period[0],1,&result1[0]);



	printf(" psa	=   %16.9f",result);
	printf(" psa2	=  %16.9f",result1[0]);
	getchar();
	return 1;
}













void band_passstruct_main()
	{
	int j,i;
	int instant_liedu_pga;
	int instant_liedu_pgv;
	int PGV_type=2;

	float multiple_PGA=3.73;
	float increment_PGA=-1.23;
	float multiple_PGV=3.61;
	float increment_PGV=2.72;

	int countt;
	float data[2000];
	float dataV[2000];
	float dataD[2000];
	double dataOut;
	double pi=3.14159265358979323;
	countt=1;
	int sample_rate=50; //  改变采样率
		for(i=0;i<2000;i++)
			{
			//data[i]=/*pow(-1.0,double(i))**/i;
			data[i]=100*sin(2*pi/50*i);
			}
	FIR_LIST ssdata[1000];





	for(i=0;i<1500;i++)
		{
		if(i==0)
			dataV[i]=data[i+0]/sample_rate;
		else
			dataV[i]=dataV[i-1]+(data[i+40]+data[i+40-1])/sample_rate/2;
		}

	for(i=0;i<1000;i++)
		{
		if(i==0)
			dataD[i]=dataV[i]/sample_rate/2;
		else
			dataD[i]=(dataV[i]+dataV[i-1])/sample_rate/2;
		}

	for(j=0;j<4;j++)
		{
	ssdata[0].dataV[j]=dataV[0];
	ssdata[0].next=&ssdata[1];
	ssdata[0].sample_rata=sample_rate;
	int i;
	for(i=1;i<999;i++)
		{
		ssdata[i].dataV[j]=dataV[i];
		ssdata[i].last=&ssdata[i-1];
		ssdata[i].next=&ssdata[i+1];
		ssdata[i].sample_rata=sample_rate;
		}

	ssdata[999].dataV[j]=dataV[999];
	ssdata[999].last=&ssdata[998];
	ssdata[999].sample_rata=sample_rate;

		}








	for(j=0;j<5;j++)
		{
	
		ssdata[0].data[j]=data[0];
		ssdata[0].next=&ssdata[1];
		ssdata[0].sample_rata=sample_rate;
		int i;
		 for(i=1;i<999;i++)
			{
			ssdata[i].data[j]=data[i];
			ssdata[i].last=&ssdata[i-1];
			ssdata[i].next=&ssdata[i+1];
			ssdata[i].sample_rata=sample_rate;
			}

		 ssdata[999].data[j]=data[999];
		 ssdata[999].last=&ssdata[998];
		 ssdata[999].sample_rata=sample_rate;
		}
	 if(sample_rate==50)
		 {

		 int i;
		for(i=200;i<900;i++)
			{

			if(band_passstruct(&ssdata[i],PGV_type,&countt,1)==2)

			//cout<<dataOut<<"    "<<data[80]<<endl;
				{

				instant_liedu_pga=	(int)(ipga(ssdata[i].band_pass_data[4],multiple_PGA,increment_PGA)+0.5);
				instant_liedu_pgv=   (int)(ipgv(ssdata[i].band_pass_dataV[4],multiple_PGV,increment_PGV)+0.5);
				
				printf("%12.8f  %12.8f %12.8f %12.8f %12.8f \n\n\n",ssdata[i].dataV[0],ssdata[i].band_pass_dataV[1],ssdata[i].data[4],ssdata[i].instant_PGA,data[i-0] );
				printf("instant_liedu_pga= %d          instant_liedu_pgv=%d      \n\n\n\n\n",instant_liedu_pga,instant_liedu_pgv);
				
				}
			
	       else
			   {
			   printf("%12.8f  %12.8f %12.8f %12.8f %12.8f  \n",ssdata[i].dataV[0],ssdata[i].band_pass_dataV[1],ssdata[i].band_pass_dataV[4],ssdata[i].instant_PGA,data[i-0] );
		       }
			}
	     }

	 if(sample_rate==100)
		 {

		 	int i;
		 for(i=200;i<400;i++)
			 {

			 band_passstruct(&ssdata[i],PGV_type,&countt,1);
			 //cout<<dataOut<<"    "<<data[80]<<endl;
			 printf("%16.8f  %16.8f %16.8f %16.8f \n",ssdata[i].band_pass_data[0],ssdata[i].band_pass_data[1],ssdata[i].band_pass_data[2],data[i-95] );
			 }
		 }

	 if(sample_rate==200)
		 {

		 	int i;
		 for(i=400;i<600;i++)
			 {

			 band_passstruct(&ssdata[i],PGV_type,&countt,1);
			 //cout<<dataOut<<"    "<<data[80]<<endl;
			 printf("%16.8f  %16.8f %16.8f %16.8f \n",ssdata[i].band_pass_data[0],ssdata[i].band_pass_data[1],ssdata[i].band_pass_data[2],data[i-150] );
			 }
		 }





	//band_pass(data,191,200,dataOut);
	////cout<<dataOut<<"    "<<data[60]<<endl;
	//printf("%d   %d \n",dataOut,data[190-95] );
	//band_pass(data,301,400,dataOut);
	////cout<<dataOut<<"    "<<data[40]<<endl;
	//printf("%d   %d \n",dataOut,data[300-150] );
	//cout<<"press enter kye to continue......"<<endl;
	//cin.get();
	getchar();
	}



// float ipga(float pga)
// {
// 	printf("pga=%f\n",pga);
// 	float tmp=(float)pga;
// 		printf("tmp=%f\n",tmp);
// 	printf("log10f(pga)=%f\n",log10f(pga));
// 	return(3.73*log10f(pga)-1.23);
// }
// float ipgv(float pgv)
// {
// 	return(3.61*log10f(pgv)+2.72);
// }




float floattest_ipga(float test)
{
	printf("pga=%f\n",test);
	printf("log10f=%f\n",log10f(100.0));
	float tmp=(float)(3.73*log10f(test)-1.23);
	printf("ipga=%f\n",tmp);
	return tmp;
}
float floattest_ipgv(float test)
{
	printf("pgv=%f\n",test);
	printf("log10f=%f\n",log10f(100.0));
	float tmp=(float)(3.61*log10f(test)+2.72);
		printf("ipgv=%f\n",tmp);
	return tmp;
}


int stru_test()
	{
	int i,slt_length,llt_length,j;
	float result;
	float * p_result=&result;
	float data[5][1000];
	double pi=3.14159265358979;
	float threshold=5;
	slt_length=6;
	llt_length=100;
	for(j=0;j<5;j++)
		{

		for(i=0;i<1000;i++)
			{
			data[j][i]=0.1*sin(2*pi/15*i);

			}
		for(i=500+10*j;i<1000;i++)
			{
			data[j][i]=data[j][i]+40*sin(2*pi/15*i);

			}
		}
	int ifor,jfor;
	for(ifor=0;ifor<1;ifor++)
	{
		for(jfor=0;jfor<1000;jfor++)
		{
			//printf("data=%f\n",data[ifor][jfor]);
		}
		//printf("\n");
	}
	
	FIR_LIST ssdata[1000];
	for(j=3;j<4;j++)
		{

		ssdata[0].band_pass_data[0]=data[j][0];
		ssdata[0].next=&ssdata[1];
		//ssdata[0].sample_rata=sample_rate;
		int i;
		for(i=1;i<999;i++)
			{
			ssdata[i].band_pass_data[0]=data[j][i];
			ssdata[i].last=&ssdata[i-1];
			ssdata[i].next=&ssdata[i+1];
			//ssdata[i].sample_rata=sample_rate;
			}

		ssdata[999].band_pass_data[0]=data[j][999];
		ssdata[999].last=&ssdata[998];
		//ssdata[999].sample_rata=sample_rate;
		}
		result =5;
	for(i=300;i<600;i++)
		{
		if(slt_lltstru(&ssdata[i],slt_length,llt_length,p_result,threshold,0)==2)
		printf("%d     \n\n\n\n",i );
		printf("%d  %f \n",i,result);
				/*printf("%d  %f \n",i,ssdata[i].band_pass_data );*/
		//floattest(i);
		}

	float pga=100.0;
	float pgv=100.0;
	// printf("pga = %f  ipga =%f \n",pga,ipga(pga));
 //    printf("pgv = %f  ipgv =%f \n",pgv,ipgv(pgv));
 //    printf("log10f=%f\n",log10f(100.0));
	//floattest_ipga();
	printf("pga = %f  ipga =%f \n",pga,floattest_ipga(pga));
	printf("pgv = %f  ipgv =%f \n",pgv,floattest_ipgv(pgv));
	getchar();

	return 1;

}


int init_iws_trg_paras()
{
	app.iws_para.slt_length=6;
	app.iws_para.llt_length=100;
	app.iws_para.threshold=15;
	app.iws_para.trg_ch=4;
	app.iws_para.i_l[0]=1;
	app.iws_para.i_l[0]=1;
	app.iws_para.i_l[0]=1;
	app.iws_para.iws_install_info.a=3;
	app.iws_para.iws_install_info.b=270;
	app.iws_para.iws_install_info.c=0;
	return 0;
}

int read_para_file_fromsd_comm(void * app_para,int len,char * parafile,char * parafile_orig)
{
	if(0==read_file2buf(app_para,len,parafile,IWSPARAPATH,"r")){//尝试读默认设置
		printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
		return 0;
	}
	else{
		if(0==read_file2buf(app_para,len,parafile_orig,IWSPARAPATH_ORIG,"r")){
			// printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
			// if(save_bufinfile(app_para,sizeof(APP_PARA_S),parafile,IWSPARAPATH,"w+")==0){//将出厂设置设置成默认配置
			// 	printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
			// 	//atcmd_com_OK_rt();
			// }
			return 1;
		}
		else{//出厂设置也读不到的情况下
			return 2;
		}
		return -1;
	}

}




int init_iws_server_paras_parafile(IWS_SERVER * app_para)//初始化硬件参数
{
	int ret;
	ret=read_para_file_fromsd_comm(app_para,6*sizeof(IWS_SERVER),IWS_SVR_PARAFILE,IWS_SVR_PARAFILE_ORIG);
	printf(GREEN"inside %s line %d,ret=%d\n"NONE, __FUNCTION__,__LINE__,ret);
	if(ret==0){
		printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
		return 0;
	}
	else if(ret==1){
		return 1;
	}
	else if(ret==2){
		return 2;
	}
	else{
		return -1;
	}

}








int init_iws_server_paras_default()
{
	int ifor;
	for(ifor=0;ifor<6;ifor++)
	{
		snprintf(app.iws_server[ifor].ipv4_addr,20,"%s","222.222.119.11");
		app.iws_server[ifor].port=1973;
		app.iws_server[ifor].index=ifor;	
	}
	return 0;
}


int init_iws_server_paras(IWS_SERVER * app_para)
{
	int ret;
	printf("sizeof(IWS_SERVER)=%d\n",sizeof(IWS_SERVER));
	ret=init_iws_server_paras_parafile(app_para);
	if(ret==0){
		return 0;
	}
	else if(ret==1){//有出厂设置
		//init_iws_server_paras_default();
		if(save_bufinfile(app_para,6*sizeof(IWS_SERVER),IWS_SVR_PARAFILE,IWSPARAPATH,"w+")==0){//保存成默认设置
				printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
				//atcmd_com_OK_rt();
		}
		return 0;
	}
	else if(ret==2){//没有出厂设置
		init_iws_server_paras_default();
		if(save_bufinfile(app_para,6*sizeof(IWS_SERVER),IWS_SVR_PARAFILE,IWSPARAPATH,"w+")==0){//保存成默认设置
				printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
				//atcmd_com_OK_rt();
		}
		if(save_bufinfile(app_para,6*sizeof(IWS_SERVER),IWS_SVR_PARAFILE_ORIG,IWSPARAPATH_ORIG,"w+")==0){//保存成默认设置
				printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
				//atcmd_com_OK_rt();
		}
		return 0;
	}	
}

int init_iws_tirg_paras_default()
{
	app.iws_para.slt_length=6;
	app.iws_para.llt_length=100;
	app.iws_para.threshold=15;
	app.iws_para.trg_ch=4;
	app.iws_para.i_l[0]=1;
	app.iws_para.i_l[0]=1;
	app.iws_para.i_l[0]=1;
	return 0;
}



int init_iws_trig_paras_parafile(IWS_PARA * iws_tirg_para)//初始化硬件参数
{
	int ret;
	ret=read_para_file_fromsd_comm(iws_tirg_para,sizeof(IWS_PARA),IWS_TRIG_PARAFILE,IWS_TRIG_PARAFILE);
	printf(GREEN"inside %s line %d,ret=%d\n"NONE, __FUNCTION__,__LINE__,ret);
	if(ret==0){
		printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
		return 0;
	}
	else if(ret==1){
		return 1;
	}
	else if(ret==2){
		return 2;
	}
	else{
		return -1;
	}

}





















int init_iws_trig_paras(IWS_PARA * iws_tirg_para)
{
	int ret;
	printf("sizeof(IWS_PARA)=%d\n",sizeof(IWS_PARA));
	ret=init_iws_trig_paras_parafile(iws_tirg_para);
	if(ret==0){
		return 0;
	}
	else if(ret==1){//有出厂设置
		//init_iws_server_paras_default();
		if(save_bufinfile(iws_tirg_para,sizeof(IWS_PARA),IWS_TRIG_PARAFILE,IWSPARAPATH,"w+")==0){//保存成默认设置
				printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
				//atcmd_com_OK_rt();
		}
		return 0;
	}
	else if(ret==2){//没有出厂设置
		init_iws_tirg_paras_default();
		if(save_bufinfile(iws_tirg_para,sizeof(IWS_PARA),IWS_TRIG_PARAFILE,IWSPARAPATH,"w+")==0){//保存成默认设置
				printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
				//atcmd_com_OK_rt();
		}
		if(save_bufinfile(iws_tirg_para,sizeof(IWS_PARA),IWS_TRIG_PARAFILE,IWSPARAPATH_ORIG,"w+")==0){//保存成默认设置
				printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
				//atcmd_com_OK_rt();
		}
		return 0;
	}	
}










int init_iws_steim2_para(IWS_STEIM2 *steim2_para)
{
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
	steim2_para->mincount[0]=steim2_para->mincount[2]=steim2_para->mincount[1]=10;
	steim2_para->semicount[0]=steim2_para->semicount[2]=steim2_para->semicount[1]=0;
	//steim2_para->mincount[0]=steim2_para->mincount[2]=steim2_para->mincount[1]=10;
	//steim2_para->semicount[0]=steim2_para->semicount[2]=steim2_para->semicount[1]=10;
	steim2_para->pi=3.14159265358979323;
	steim2_para->max[0]=steim2_para->max[1]=steim2_para->max[2]=-5;
	steim2_para->min[0]=steim2_para->min[1]=steim2_para->min[2]=5;
	//steim2_para->change_c[0]=steim2_para->change_c[1]=steim2_para->change_c[2]=1;

}


int init_evt_record(APP_S * app)
{
	snprintf(app->evt_record.path,100,"%s","/home/pi/userdata/data/event");
}
int init_cilent_psv(APP_S * app)
{
	snprintf(app->iws_cilnet_psv.head,8,"%s","SI_HEAD");

}

int init_iws_paras()
{
	//init_iws_trig_paras_parafile(IWS_PARA * iws_tirg_para)//初始化硬件参数
	//init_iws_trg_paras();
	init_iws_trig_paras(&app.iws_para);
	init_iws_server_paras(&app.iws_server);
	init_iws_steim2_para(&app.steim2_para);
	init_steim2buf(&app);
	init_status_out_buf(&app);
	init_evt_record(&app);
	init_cilent_psv(&app);
	return 0;
}

int init_paras()
{
	app.gps_data=&gps_data;
	init_iws_paras();
	GetCompileTime(app.compiler_time);
	printf("GetCompileTime(app.compiler_time);=%s\n",app.compiler_time);
	return 0;
}



int add_file_list(char * fileurl)
{
	int ifor;
	char str[16];
	char nsec_str[16];
	char index_str[16];
		//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

	// for(ifor=0;ifor<10;ifor++)
	// {
	// 	//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

	// 	str[ifor]=fileurl[ifor];
	// }
	// //printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

	// str[10]='\0';
	sscanf(fileurl,"/home/pi/userdata/data/steim2/%[0-9].%[0-9].%[0-9]",index_str,str,nsec_str);
	

			//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

	for(ifor=0;ifor<10240;ifor++)
	{
			//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

		if(app.iws_file_list[ifor].isused==0){
			app.iws_file_list[ifor].isused=1;
					//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

			snprintf(app.iws_file_list[ifor].name,1020,"%s",fileurl);
			//sscanf(fileurl,"%[1-9]",str);
			//printf()
			///home/pi/userdata/data/iws_data_1531749114
		//printf(BLUE"file:%s,function：%s,line:%d,str=%s\n"NONE,__FILE__,__FUNCTION__,__LINE__,str);
			sscanf(str,"%d",&app.iws_file_list[ifor].tv_sec);
			//app->globe_steim2_file_index++
			sscanf(index_str,"%d",&app.iws_file_list[ifor].file_index);
			app.globe_steim2_file_index=app.iws_file_list[ifor].file_index+1;
			printf("str---=%s,nsec_str=%s,index_str=%s\n",str,nsec_str,index_str);
					//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
			printf("ifor=%d,%s,tv=%d\n",ifor,fileurl,app.iws_file_list[ifor].tv_sec);
			break;
		}
		
		
	}
	return 0;

}

int del_file_list(char * fileurl)
{
	int ifor;
	for(ifor=0;ifor<1024;ifor++)
	{
		if(strstr(fileurl,app.iws_file_list[ifor].name)!=NULL){
			app.iws_file_list[ifor].isused=0;
			printf("ifor=%d,%s\n",ifor,fileurl);
			break;
		}
	}
	return 0;

}





int printf_file_list()
{
	int ifor;
	for(ifor=0;ifor<1024;ifor++)
	{
		if(app.iws_file_list[ifor].isused==1){
			printf(PURPLE"ifor=%d,%s\n"NONE,ifor,app.iws_file_list[ifor].name);
		}
	}
	return 0;

}

// typedef struct iws_utc_10{
// 	short year;
// 	short yday;
// 	unsigned char hour;
// 	unsigned char min;
// 	unsigned char sec;
// 	unsigned char unuse;
// 	short u_sec;

// }__attribute__ ((packed,aligned(1)))iws_utc_10;

int main(int argc,char ** argv)
{	


	int tmp_len=2048-132-118-92-sizeof(IWS_RQ_TIME)-sizeof(IWS_PAK_BUF)-sizeof(IWS_STEIM2)-sizeof(IWS_PARA);
	printf("tmp_len=%d,sizeof(IWS_SERVER)=%d,sizeof(IWS_RQ_TIME)=%d\n",tmp_len,sizeof(IWS_SERVER),sizeof(IWS_RQ_TIME));
	printf("sizeof(IWS_SERVER)=%d\n",sizeof(IWS_SERVER));
	printf("sizeof(IWS_STEIM2)=%d\n",sizeof(IWS_STEIM2));
	printf("sizeof(IWS_CSTP)=%d\n",sizeof(IWS_CSTP));
	printf("pow(100.0,0.5)=%f\n",pow(100.0,0.5));
	printf("powf(100.0,0.5)=%f\n",powf(100.0,0.5));
	// printf("powl(100.0,0.5)=%d\n",pow(100.0,0.5));
	// printf("pow(100.0,0.5)=%d\n",pow(100.0,0.5));
	//exit(0);
	//psa_struct_2_main();
	//test_main();
	//exit(0);
	//exit(0);//IWS_STEIM2//IWS_CSTP

// struct timespec date_time;
// clock_gettime(CLOCK_REALTIME,&date_time);
// struct tm  *mytm= localtime(&date_time.tv_sec);
// struct tm  *mytm2= gmtime(&date_time.tv_sec);
// printf("date_time.tv_sec=%d\n",date_time.tv_sec);
// printf("tm.year=%d\n",mytm->tm_year);
// printf("tm.mon=%d\n",mytm->tm_mon);
// printf("tm.mmon=%d\n",mytm->tm_mday);
// printf("tm.tm_hour=%d\n",mytm->tm_hour);
// printf("tm.tm_min=%d\n",mytm->tm_min);
// printf("tm.tm_sec=%d\n",mytm->tm_sec);

// printf("date_time.tv_sec=%d\n",date_time.tv_sec);
// printf("tm.year=%d\n",mytm2->tm_year);
// printf("tm.mon=%d\n",mytm2->tm_mon);
// printf("tm.mmon=%d\n",mytm2->tm_mday);
// printf("tm.tm_hour=%d\n",mytm2->tm_hour);
// printf("tm.tm_min=%d\n",mytm2->tm_min);
// printf("tm.tm_sec=%d\n",mytm2->tm_sec);
// //printf("tm.mon=%d\n",mytm2->tm_mon);

// iws_utc_10 my_iws_utc;
// my_iws_utc.year=mytm2->tm_year;
// my_iws_utc.yday=mytm2->tm_yday;
// my_iws_utc.hour=mytm2->tm_hour;
// my_iws_utc.min=mytm2->tm_min;
// my_iws_utc.sec=mytm2->tm_sec;




// printf("date_time.tv_sec=%d\n",date_time.tv_sec);
// printf("my_iws_utc.year=%d\n",my_iws_utc.year);
// printf("my_iws_utc.yday=%d\n",my_iws_utc.yday);
// printf("my_iws_utc.hour=%d\n",my_iws_utc.hour);
// printf("my_iws_utc.min=%d\n",my_iws_utc.min);
// printf("my_iws_utc.sec=%d\n",my_iws_utc.sec);

// exit(0);

	printf("IWS_SERVER=%d\n",sizeof(IWS_SERVER));
	//exit(0);

	// char * a="1,222.222.119.11,1973";
	// int type;
	// char addr[20];
	// unsigned short port=0;
	// int aa;
	// printf(YELLOW"a=%s\n"NONE,a);

	// int ret=sscanf(a,"%d,%[^,],%d",&type,addr,&aa);
	// printf(YELLOW"a=%s,ret=%d\n"NONE,a,ret);
	// printf(YELLOW"a=%s,type=%d,addr=%s,port=%d\n"NONE,a,type,addr,aa);
	// exit(0);
/*
PCFADDR 0x38
LTC2943ADDR 0x64
RELAY_ADDR 0x76
*/
	
	//resample2_test_main();//通过200hz做低通抽取
	// exit(0);
	//psa_test();
	//band_passtest();
	// stru_test();
	// exit(0);
	//check_eth0();
	//gps_main(&app);
	//gps_main(&app);
	//printf("ieth_ip:%s,wlan_ip:%s\n",eth_ip,wlan_ip);
	//exit(0);
	//open_pca9539();
	//sleep(3);
	//iws_client_frame();
	//printf("end main\n");
	//exit(0);
	//init_mut_cond(&app);

	//openltc2943(&app);
	//
	
	// printf_file_list();
	// exit(0);
	//open_pcf8574(&app);//复位设备
	//openltc2943(&app);
	//open_pca9539(&app);
	//exit(0);
	//filenevt("/home/pi/userdata/data"); //监测文件夹变化
	//steim2_main();
	//exit(0);
	init_app_struct();//
	init_conf();
	listfiles("/home/pi/userdata/data/steim2",1);
	//exit(0);
	//fs_monitor(&app);
	//iws_client_frame(&app);
	//gps_main(&app);
	init_paras();//初始化参数

	app.argv=argv;
	app.argc=argc;
	//log_main(&app);
	printf("argc=%d,argv[1]=%s\n",app.argc,app.argv[1]);
	//exit(0);
	//gps_main(&app);
	//iws_client1(&app);
	//iws_client(&app);
	int ifor;
	for(ifor=0;ifor<IWS_UP_WAVEDATA_EXT_NUM;ifor++)
	{
		//init_iws_wave_pak(&app.iws_wave_pak[ifor],0);//注册信息响应包
		init_iws_wave_pak(&(app.iws_wave_pak_ext[ifor].wcts[0]),0);//注册信息响应包
	}
	for(ifor=0;ifor<1000;ifor++)
	{
		init_iws_wave_pak(&app.iws_wave_pak[ifor],0);//注册信息响应包
		//init_iws_wave_pak(&(app.iws_wave_pak_ext[ifor].wcts[0]),0);//注册信息响应包
	}
	for(ifor=0;ifor<1000;ifor++)
	{
		init_iws_up_ti_pak(&app.iws_up_ti[ifor],0);//注册信息响应包
	}

	get_dhcpcd_conf();
	//gpsd_client_frame();
	snprintf(wlan_ip,100,"NO IP");
	snprintf(eth_ip,100,"NO IP");
	snprintf(wlan_ip_now,100,"NO IP");
	snprintf(eth_ip_now,100,"NO IP");
	getlocalips(eth_ip,wlan_ip);

	wiring_bcm2835_init();//初始化库
	bcm2835_I2C_Init();//初始化i2c参数
	init_device_pin();//初始化各个设备的管教
	BEEP_OFF();
	button_push();
	//oledmain_wiring(&app);
	open_pcf8574(&app);//复位设备
	//exit(0);
	//printf("start\n");
	//sleep(3);
	//oledmain_wiring(&app);
	//exit(0);
	openltc2943(&app);//电池信息
	//reset_devices();
	// set_addr(0x3c);
	//sleep(3);
	//oled_main(&app);
	//exit(0);
	//main_ad(&app);
	//app.argv=argv;
	//app.argc=argc;
	//gps_main(&app);
	printf("start\n");
	//sleep(3);
	//bcm_open_pca9539(&app);//设置pca9539的值
	// oled_main(&app);
	// exit(0);
		//exit(0);
	open_pca9539(&app);
	//main_ad(&app);
	//openltc2943(&app);//电池信息
	//exit(0);
	//open_pca9539(&app);//继电器，蜂鸣器,前端通路选择
	//exit(0);
	//exit(0);
	printf("sizeof=%d\n",sizeof(struct timespec));
	//exit(0);
	signal(SIGPIPE,SIG_IGN);
	//test_and_exit(0);
	debug_test();
	
	json_test();

	//app_devices_init(&app.app_devices);//
	//start_devices();
	//udp_atcmd_main(&app);
    int i_connect;
    printf("in main ping_pang=%d\n",app.ping_pang);
     ///iws_client1(&app);
    //iws_client(&app);
	app_thread_init((THREAD_COTR_S *)&app.app_thread);
	while(1){
		printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
		app_thread_start(&app.app_thread);
		printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
		//sleep(100000);
		wait_go_signal(0);
		printf(GREEN"GET RESTART THREAD SIGNAL\n"NONE);
	}
	app_thread_join(&app.app_thread);
	//app_server_start();
	//tcp_oled_server("6557");
	//tcp_ui();
	return 0;
}



























/*
pthread_mutex_lock(&myoso->mut[MUTALL]);

pthread_cond_wait(&myoso->cond[CONDALL], &myoso->mut[MUTALL]);

pthread_mutex_unlock(&myoso->mut[MUTALL]);

pthread_mutex_lock(&mut_gpio);
pthread_cond_signal(&cond_gpio); 
pthread_mutex_unlock(&mut_gpio);
pthread_mutex_unlock(&myoso->mut[MUTALL]);

pthread_mutex_lock(&(myoso->mut[MUT_FILE_NAME]));
pthread_cond_broadcast(&(myoso->cond[COND_FILE_NAME]), &(myoso->mut[MUT_FILE_NAME]));
pthread_mutex_unlock(&(myoso->mut[MUT_FILE_NAME]));
*/

