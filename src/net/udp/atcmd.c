#include <stdio.h>  
#include <stdlib.h>  
#include <sys/wait.h>  
#include <sys/types.h>  

#include <string.h>
#include <unistd.h> /* for close() */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <atcmd.h>
#include <printcolor.h>
#include <app_main.h>
#include <pca9539_relay.h>
#include <ltc2943.h>
#include <iws.h>
#include <cstp.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define SERVER_PORT 8887
#define BUFF_LEN 4096
extern APP_S app;

unsigned char recvbuf[BUFF_LEN]; /* buffer for message */
//char paramsg[BEAT_PAK_SIZE]; /* buffer for message */
unsigned char sendbuf[BUFF_LEN];
int sendlen=0;



// int split_str(char * str)
// {
// 	char str[] = "我,是,中国,程序员";
// 	char *ptr;
// 	char *p;
// 	printf("开始前:  str=%s\n", str);
// 	printf("开始分割:\n");
// 	ptr = strtok(str, ",");
// 	while (ptr != NULL) {
// 		printf("ptr=%s\n", ptr);
// 		ptr = strtok(NULL, ",");
// 	}
// 	return 0;
// }


/*            int i; 
            unsigned int j; 
            char input[ ]=”10 0x1b aaaaaaaa bbbbbbbb”; 
            char s[5]; 
            sscanf(input,”%d %x %5[a-z] %*s %f”,&i,&j,s,s); 
            printf(“%d %d %s ”,i,j,s); 
            */



int atcmd_setad_rt()
{
	sendbuf[0]='O';
	sendbuf[1]='K';
	sendbuf[2]=0x0d;
	sendbuf[3]=0x0a;
	sendlen=4;
	return 0;
}
int atcmd_com_OK_rt()
{
	sendbuf[0]='O';
	sendbuf[1]='K';
	sendbuf[2]=0x0d;
	sendbuf[3]=0x0a;
	sendlen=4;
	return 0;
}


int atcmd_com_ER_rt()
{
	sendbuf[0]='E';
	sendbuf[1]='R';
	sendbuf[2]=0x0d;
	sendbuf[3]=0x0a;
	sendlen=4;
	return 0;
}




int atcmd_setad(char * buf,int length)
{
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
	int ifor;
	//printf("atcmd_setad\n");
	for(ifor=0;ifor<sizeof(AD_REG_S);ifor++){
		app.app_para.ad_reg.reg[ifor]=buf[ifor];
		//app.ad1256.run_ctrl.has_reset=0;//通知ad线程重新配置
		printf("%0x ",buf[ifor]);
	}
	int fs,firlen;
	switch(app.app_para.ad_reg.reg[3])
	{
		case 0xB0:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		fs=200;
		firlen=67;
		break;
		case 0xa1:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		fs=100;
		firlen=83;
		break;
		case 0x92:
		firlen=165;
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		fs=50;
		break;
		default:
		printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		app.app_para.ad_reg.reg[3]=0xB0;
		fs=200;
		break;

	}
	app.app_para.fs=fs;
	app.app_para.ad_reg.reg[3]=0xB0;
	app.app_sig.sig_setad_request=1;
	
	atcmd_setad_rt();
	printf("\n");
	return 0;
}
//save_bufinfile(char * buf,int len,char * filename,char * filepath)
int atcmd_saveadp(char * buf,int length)
{
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
	char filepath[]="/home/pi/Geo_Pi";
	char filename[]="iwspara.para";
	if(save_bufinfile(&app.app_para,sizeof(APP_PARA_S),IWSPARAFILE,IWSPARAPATH,"w+")==0){
		atcmd_com_OK_rt();
	}
	else{
		atcmd_com_ER_rt();
	}
	return 0;
}



int atcmd_saveserver(char * buf,int length)
{
	//AT+SAVESVR\r\n
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
	char filepath[]="/home/pi/Geo_Pi";
	char filename[]="iwspara.para";
	if(save_bufinfile(&app.iws_server,6*sizeof(IWS_SERVER),IWS_SVR_PARAFILE,IWSPARAPATH,"w+")==0){
		atcmd_com_OK_rt();
	}
	else{
		atcmd_com_ER_rt();
	}
	return 0;
}










int append_AP(FILE * fp)
{
	//fputs();
}


int atcmd_GETAPFILE(char * buf,int length)
{
	if(try_system("cp /etc/wpa_supplicant/wpa_supplicant.conf /home/pi/userdata/write/")==0){
		atcmd_com_OK_rt();
		return 0;
	}
	else{
		atcmd_com_ER_rt();
		return -1;
	}

}





//save_bufinfile(char * buf,int len,char * filename,char * filepath)
int atcmd_addAP(char * buf,int length)
{
	atcmd_com_ER_rt();
	return 0;
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
	FILE * fp;
	char tmpstr[128];
	char fileurl[]="/etc/wpa_supplicant/wpa_supplicant.conf";
/*
network={
	ssid="iws"
	psk="Zwp123456"
	key_mgmt=WPA-PSK
}

*/	
	char ssid[24];
	char passwd[24];
	sscanf(buf,"%[^,],%s",ssid,passwd);
	printf("ssid=%s,passwd=%s",ssid,passwd);
	//exit(0);
	int is_havebefore=0;
	if (fp=fopen(fileurl,"w+")){//打开日志文件;
			//fseek(fp,-(para.nChan+2)*4,1);
		//rewind(fp);
		fseek(fp,0, SEEK_SET);
		while(!feof(fp))
		{
			//char * strstr(char * str1, char * str2)
			fgets(tmpstr, 100, fp);
			printf("tmpstr=%s\n",tmpstr);
			if(NULL!=strstr(tmpstr,"network=")){//未找到
				fputs(ssid,fp);
				fputs(passwd,fp);
				fputs("key_mgmt=WPA-PSK",fp);
				fputs("}",fp);
				fputs("network={",fp);
				fclose(fp);
				is_havebefore=1;
				break;
			}
			else{

//printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
			}
		}
		if(is_havebefore==0)//如果未找到之前信息
		{
			fputs("network={",fp);
			fputs(ssid,fp);
			fputs(passwd,fp);
			fputs("key_mgmt=WPA-PSK",fp);
			fputs("}",fp);
			fclose(fp);
		}
		atcmd_com_OK_rt();
	}
	return 0;
}



//save_bufinfile(char * buf,int len,char * filename,char * filepath)
int atcmd_PUTAP(char * buf,int length)
{	if(is_file_exist("/home/pi/userdata/write/wpa_supplicant.conf")==0){
		if(try_system("cp /home/pi/userdata/write/wpa_supplicant.conf /etc/wpa_supplicant/")==0){
			atcmd_com_OK_rt();
			return 0;
		}
		else{
			atcmd_com_ER_rt();
			return -1;
		}
	}
	else{
		atcmd_com_ER_rt();
			return -1;
	}
}






//save_bufinfile(char * buf,int len,char * filename,char * filepath)

int atcmd_setpca_rt()
{
	sendbuf[0]='O';
	sendbuf[1]='K';
	sendbuf[2]=0x0d;
	sendbuf[3]=0x0a;
	sendlen=4;
	return 0;
}


int printatpara(char * buf,int length)
{
	int ifor;
	for(ifor=0;ifor<length;ifor++)
	{
		printf("%X",buf[ifor]);
	}
	printf("\n");
	return 0;
}


int atcmd_getpca(char * buf,int length)
{
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
	//sendbuf[0]=app.pca9539[0];
	//sendbuf[1]=app.pca9539[1];
	printf("app.pca9539[0]=%02X,app.pca9539[1]=%02X\n",app.pca9539[0],app.pca9539[1]);
	memcpy(sendbuf,&app.pca9539,2);
	printf("sendbuf[0]=%02X,sendbuf[1]=%02X\n",sendbuf[0],sendbuf[1]);

	sendbuf[2]=0x0d;
	sendbuf[3]=0x0a;
	sendlen=4;
	return 0;

}


int do_rm()
{
    pid_t status;
    status = system("rm /home/pi/userdata/*");

    if (-1 == status)
    {
        printf("system error!");
        return -1;
    }
    else
    {
        printf("exit status value = [0x%x]\n", status);

        if (WIFEXITED(status))
        {
            if (0 == WEXITSTATUS(status))
            {
			    printf("run shell script successfully.\n");
			    return 0;
            }
            else
            {
                printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
                return -2;
            }
        }
        else
        {
            printf("exit status = [%d]\n", WEXITSTATUS(status));
            return -3;
        }
    }
    return -1;

}


int atcmd_cleansd(char * buf,int length)
{
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
	if(do_rm()==0){
		sendbuf[0]='O';
		sendbuf[1]='K';
		sendbuf[2]=0x0d;
		sendbuf[3]=0x0a;
		sendlen=4;	
	}
	else{
		sendbuf[0]='E';
		sendbuf[1]='R';
		sendbuf[2]=0x0d;
		sendbuf[3]=0x0a;
		sendlen=4;	
	}

	return 0;

}

//getvoltage1(ftemp);
int atcmd_btinfo(char * buf,int length)
{
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
	float ftemp;
	printf("current=%f\n",-1000*1000*getcurrent());
	snprintf(sendbuf,512,"V=%f,I=%f,T=%f",getvoltage1(&ftemp),400*1000*getcurrent(),gettemperature());
	sendlen=strlen(sendbuf);
	sendbuf[sendlen++]=0x0d;
	sendbuf[sendlen++]=0x0a;
	return 0;

}



int print_iwspara()
{
	printf(GREEN"slt_length=%d\n"NONE,app.iws_para.slt_length);
	printf(GREEN"llt_length=%d\n"NONE,app.iws_para.llt_length);
	printf(GREEN"threshold=%d\n"NONE,app.iws_para.threshold);
	printf(GREEN"trg_ch=%d\n"NONE,app.iws_para.trg_ch);
	printf(GREEN"x=%d\n"NONE,app.iws_para.i_l[0]);
	printf(GREEN"y=%d\n"NONE,app.iws_para.i_l[1]);
	printf(GREEN"z=%d\n"NONE,app.iws_para.i_l[2]);
	return 0;
}
int iws_para_check()
{
	app.iws_para.trg_ch=app.iws_para.trg_ch-1;
	if(app.iws_para.trg_ch>4)
	{
		app.iws_para.trg_ch=4;
	}
	if(app.iws_para.trg_ch<0)
	{
		app.iws_para.trg_ch=0;
	}

	return 0;
}
int atcmd_iws_setpara(char * buf,int length)
{
	print_iwspara();
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
	unsigned int ch[16];
	int ifor;
	printatpara(buf,length);
	sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",\
		&app.iws_para.slt_length,&app.iws_para.llt_length,&app.iws_para.threshold,&app.iws_para.trg_ch,\
		&app.iws_para.i_l[0],&app.iws_para.i_l[1],&app.iws_para.i_l[2],&app.iws_para.filter_chose,\
		&app.iws_para.threshold_a[0],&app.iws_para.threshold_a[1],&app.iws_para.threshold_a[2],\
		&app.iws_para.trig_flag);
	iws_para_check();
	atcmd_com_OK_rt();
	printf("\n");
	return 0;
}





int atcmd_iws_getpara(char * buf,int length)
{
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
	memcpy(sendbuf,&app.iws_para,sizeof(IWS_PARA));
	sendlen=sizeof(IWS_PARA);
	sendbuf[sendlen++]=0x0d;
	sendbuf[sendlen++]=0x0a;
	return 0;
}
























int atcmd_setpca(char * buf,int length)
{
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
	unsigned int ch[16];
	int ifor;
	printatpara(buf,length);
	sscanf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",\
		&ch[0],&ch[1],&ch[2],&ch[3],&ch[4],&ch[5],&ch[6],&ch[7],&ch[8],\
		&ch[9],&ch[10],&ch[11],&ch[12],&ch[13],&ch[14],&ch[15]);
	//#define BEEPBIT 4//1不响p03
	//#define SW0BIT 5//输入范围切换p02
	#define CH1_SA1 7
	#define CH1_SA0 6
	#define CH2_SA1 5
	#define CH2_SA0 4
	#define CH3_SA1 3
	#define CH3_SA0 2
	#define CH4_SA1 1
	#define CH4_SA0 0

	#define LED1BIT 8//1不亮
	#define LED2BIT 9
	#define LED3BIT 10
	#define STBIT 11//1使能
	#define RELAYCH3BIT 12//0吸合
	#define RELAYCH2BIT 13
	#define RELAYCH1BIT 14
	#define RELAYCH0BIT 15


	//setbitchar(char num, int bit)
//AT+SETPCA=1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
//AT+SETPCA=0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	// for(ifor=0;ifor<8;ifor++)
	// {
	// app.pca9539[0]=0xFF&ch[ifor];
	// app.pca9539[1]=0xFF&(ch[8]<<7)|(ch[9]<<6)|(ch[10]<<5)|(ch[11]<<4)|(ch[12]<<3)|(ch[13]<<2)|(ch[14]<<1)|(ch[15]<<0);
	// printf("app.pca9539[0]=%X,app.pca9539[1]=%X\n",app.pca9539[0],app.pca9539[1]);	
	// }
	app.pca9539[0]=(ch[0]<<7)|(ch[1]<<6)|(ch[2]<<5)|(ch[3]<<4)|(ch[4]<<3)|(ch[5]<<2)|(ch[6]<<1)|(ch[7]<<0);
	app.pca9539[1]=(ch[8]<<7)|(ch[9]<<6)|(ch[10]<<5)|(ch[11]<<4)|(ch[12]<<3)|(ch[13]<<2)|(ch[14]<<1)|(ch[15]<<0);
	printf("app.pca9539[0]=%X,app.pca9539[1]=%X\n",app.pca9539[0],app.pca9539[1]);
	//set_pca9539_value(app.pca9539);
	set_pca9539(app.pca9539);
	//printf("atcmd_setad\n");
	for(ifor=0;ifor<16;ifor++){
		//app.app_para.ad_reg_default.reg[ifor]=buf[ifor];
		//app.ad1256.run_ctrl.has_reset=0;//通知ad线程重新配置
		printf("ch%d=%d,",ifor,ch[ifor]);
	}
	atcmd_setpca_rt();
	printf("\n");
	return 0;
}






int print_ser_info(IWS_SERVER * server)
{
	printf(RED"index=%d\n"NONE,server->index);
	printf(RED"ipv4_addr=%s\n"NONE,server->ipv4_addr);
	printf(RED"port=%d\n"NONE,server->port);
	return 0;
}



int atcmd_iws_setserver(char * buf,int length)
{
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
/*
#define DATA_SERVER_MAIN 0
#define DATA_SERVER_OPTION 1
#define EW_SERVER_MAIN 2
#define EW_SERVER_OPTION 3
#define OP_SERVER_MAIN 4
#define OP_SERVER_OPTION 5
example:
AT+SETSVR=0,192.168.10.100,5589\r\n
*/
	//printatpara(buf,length);
	int type;
	char addr[20];
	unsigned short port;
	int tmp,can_ctrl,mode;
	printf(YELLOW"buf=%s\n"NONE,buf);
	//sscanf(buf,"%d,%[^,],%d,%d",&type,addr,&tmp,&can_ctrl);
	sscanf(buf,"%d,%[^,],%d,%d,%d",&type,addr,&tmp,&can_ctrl,&mode);
	
	app.iws_server[type].index=type;
	snprintf(app.iws_server[type].ipv4_addr,20,"%s",addr);
    app.iws_server[type].last_mode=app.iws_server[type].mode;
    app.iws_server[type].mode=mode;
    if(app.iws_server[type].mode!=app.iws_server[type].last_mode){
        update_point(app.iws_server[type].mode,type);
    }
	int ifor;
	if(can_ctrl==1){
		printf(YELLOW"addr=%s,can_ctrl=%d\n"NONE,addr,can_ctrl);

		
		for(ifor=0;ifor<6;ifor++){
			//app.iws_server[ifor].mode=mode;


			if(type==ifor){
				app.iws_server[ifor].status.iscan_control=can_ctrl;
			}
			else{
				app.iws_server[ifor].status.iscan_control=0;
			}

		}
	}
	else{
		printf(RED"addr=%s,can_ctrl=%d\n"NONE,addr,can_ctrl);

		app.iws_server[type].status.iscan_control=can_ctrl;
	}
	

	app.iws_server[type].port=tmp;
	print_ser_info(&app.iws_server[type]);
	atcmd_com_OK_rt();
	return 0;
}










int atcmd_iws_getserver(char * buf,int length)
{
	//41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a
/*
#define DATA_SERVER_MAIN 0
#define DATA_SERVER_OPTION 1
#define EW_SERVER_MAIN 2
#define EW_SERVER_OPTION 3
#define OP_SERVER_MAIN 4
#define OP_SERVER_OPTION 5
example:
AT+SETSVR=0,192.168.10.100,5589\r\n
*/

/*
int atcmd_com_OK_rt()
{
	sendbuf[0]='O';
	sendbuf[1]='K';
	sendbuf[2]=0x0d;
	sendbuf[3]=0x0a;
	sendlen=4;
	return 0;
}
*/
	int type;
	sscanf(buf,"%d",&type);
	snprintf(sendbuf,512,"%d,%s,%u,%d,%d",app.iws_server[type].index,app.iws_server[type].ipv4_addr,app.iws_server[type].port,app.iws_server[type].status.iscan_control,app.iws_server[type].mode);
	printf("============================%s\n",sendbuf);
	sendlen=strlen(sendbuf);
	printf("type=%d sendlen=%d sendbuf=%s\n",type,sendlen,sendbuf);
	//sendlen=strlen(sendbuf);
	sendbuf[sendlen++]=0x0d;
	sendbuf[sendlen++]=0x0a;
	return 0;
}
















int atcmd_getad(char * buf,int length)
{
	unsigned char tmp=app.app_para.ad_reg.reg[3];
	switch(app.app_para.fs)
	{
		case 200:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		app.app_para.ad_reg.reg[3]=0xB0;
		break;
		case 100:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		app.app_para.ad_reg.reg[3]=0xa1;
		break;
		case 50:
		app.app_para.ad_reg.reg[3]=0x92;
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		break;
		default:
		printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		app.app_para.ad_reg.reg[3]=0xB0;
		break;
	}



	memcpy(sendbuf,&app.app_para.ad_reg,sizeof(AD_REG_S));
	app.app_para.ad_reg.reg[3]=tmp;
	sendlen=sizeof(AD_REG_S);
	sendbuf[sendlen++]=0x0d;
	sendbuf[sendlen++]=0x0a;
	return 0;
}








int atcmd_setSD(char * buf,int length)
{
	int i;
	sscanf(buf,"%d",&i);
	app.app_file[APP_FILE_DATA].control=i;//==1时不存文件
	atcmd_com_OK_rt();
	return 0;
}



int atcmd_setntp(char * buf,int length)
{
	int i;
	char ntp[2048];
	sprintf(ntp,"%s",buf);
	printf(BLUE"ntp=%s\n"NONE,ntp);
	if(save_bufinfile(buf,length,"ntp.conf",IWSNTPPATH,"w+")==0){//保存成默认设置
		printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
		system("sudo /etc/init.d/ntpd restart &");
			//atcmd_com_OK_rt();
	}
	atcmd_com_OK_rt();
	return 0;
}


int change_ip_conf()
{
	;
}
// int change_ip_conffile(char * buf,int len,const char * filename,const char * filepath,const char * mode)
// {
// 	int is_dirok=0;
// 	char tmp[256];
// 	printf("filepath=%s\n",filepath);
// 	if(is_dir_exist(filepath)==0){
// 		is_dirok=1;
// 	}
// 	else{
// 		snprintf(tmp,256,"mkdir -p %s",filepath);
// 		if(try_system(tmp)==0){
// 			is_dirok=1;
// 		}
// 	}
// 	if(is_dirok==1)
// 	{
// 		FILE * fp;
// 		snprintf(tmp,256,"%s/%s",filepath,filename);
// 		if (fp=fopen(tmp,mode)){//打开日志文件;
// 			int realwrite=mywrite(buf,len,fp);
// 			if(len==realwrite){
// 				;
// 				fclose(fp);
// 				return 0;
// 			}
// 			else{
// 				write_log_file();
// 				return 1;
// 			}
// 		}
// 	}
// }

int atcmd_setip(char * buf,int length)
{
	int i;
	char str[2048];
	buf[length-2]='\0';
	//sprintf(ntp,"%s",buf);
	char ip[20],mask[20],gateway[20],dns[20],dns1[20];
	int ret=sscanf(buf,"%[^,],%[^,],%[^,],%s",ip,gateway,dns,dns1);
	int ifor;
	printf("buf=%s\n",buf);
/*
IPADDR=192.168.0.210/24
GATEWAY=192.168.0.1
DNS=192.168.0.1
*/
	snprintf(str,256,"IPADDR=%s\nGATEWAY=%s\nDNS=%s\n",ip,gateway,dns);
	int strl=strlen(str);
	if(save_bufinfile(str,strl,"staticip.conf",IWSPARAPATH_ORIG,"w+")==0){//保存成默认设置
		printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
			//atcmd_com_OK_rt();
	}
	printf("ret=%d %s,%s,%s,%s\n",ret,ip,gateway,dns,dns1);
	// if(save_bufinfile(buf,length,"iws_ntp.conf",IWSPARAPATH,"w+")==0){//保存成默认设置
	// 	printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
	// 		//atcmd_com_OK_rt();
	// }
	if(try_system("sudo /home/pi/ip.sh &")==0){
		atcmd_com_OK_rt();
	}
	else{
		atcmd_com_ER_rt();
	}
	//atcmd_com_OK_rt();
	return 0;
}



int atcmd_setsid(char * buf,int length)
{
	int index;
	char sid[16];
	char key[64];
	buf[length-2]='\0';
	//sprintf(ntp,"%s",buf);
	//	snprintf(sendbuf,512,"%d,%s,%s",app.iws_server[type].index,app.iws_server[type].sid,app.iws_server[type].server_key);

	int ret=sscanf(buf,"%d,%[^,],%s",&index,sid,key);
	int ifor;
	snprintf(app.iws_server[index].sid,16,"%s",sid);
	snprintf(app.iws_server[index].server_key,36,"%s",key);
	printf("index=%d,sid=%s,key=%s\n",app.iws_server[index].index,app.iws_server[index].sid,app.iws_server[index].server_key);

/*
IPADDR=192.168.0.210/24
GATEWAY=192.168.0.1
DNS=192.168.0.1
*/
	init_cstp(&app.iws_cstp[index],index);
//&app->iws_cstp[connect_index].iws_up_wavedata
	atcmd_com_OK_rt();
	//atcmd_com_OK_rt();
	return 0;
}


int atcmd_getsid(char * buf,int length)
{
	int type;
	sscanf(buf,"%d",&type);
	//snprintf(app.iws_server[type].sid,16,"%s","jd.00001");
	//snprintf(app.iws_server[type].server_key,36,"12345678901234567890123456789012");

	snprintf(sendbuf,512,"%d,%s,%s",app.iws_server[type].index,app.iws_server[type].sid,app.iws_server[type].server_key);
	printf("============================%s\n",sendbuf);
	sendlen=strlen(sendbuf);
	printf("type=%d sendlen=%d sendbuf=%s\n",type,sendlen,sendbuf);
	//sendlen=strlen(sendbuf);
	sendbuf[sendlen++]=0x0d;
	sendbuf[sendlen++]=0x0a;
	return 0;
}



extern int print_diskinfo(char * argv);
extern int get_diskinfo(char *argv,char * sendbuf);

int atcmd_getSD(char * buf,int length)
{

			//printf(BLUE"inside %s，readlen=%d,needread=%d\n"NONE, __FUNCTION__,readLen,needread);
    printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	print_diskinfo("/home/pi/userdata");
    printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

	get_diskinfo("/home/pi/userdata",sendbuf);
	sendlen=strlen(sendbuf);
	sendbuf[sendlen++]=0x0d;
	sendbuf[sendlen++]=0x0a;
	return 0;
}











typedef struct AT_CMD_CALL{//
	char atcmd[16];//at命令字符串如：SETAD
	int (* func)(char * ,int length);//回调函数，用于处理at命令，只需提供函数名，具体由下位机实现
	int status;//是否有效
	char unuse[8];//保留
}__attribute__ ((packed,aligned(1)))AT_CMD_CALL;

AT_CMD_CALL iws_atcmd[]={
	{"SETAD",atcmd_setad},//设置ad参数寄存器值
	{"GETAD",atcmd_getad},//获取ad寄存器值
	{"SETSD",atcmd_setSD},//AT+SETPCA\r\n 设置是否存sd卡
	{"SETPCA",atcmd_setpca},//AT+SETPCA=0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,位定义参考
	{"GETPCA",atcmd_getpca},//AT+GETPCA\r\n 获取pca寄存器值
	{"CLEANSD",atcmd_cleansd},//AT+CLEANSD\r\n 清空sd卡数据区
	{"BTINFO",atcmd_btinfo},//AT+GETAPFILE\r\n 获取电池信息
	{"SAVEADP",atcmd_saveadp},//AT+SAVEADP\r\n 将内存的配置文件存入文件
	//{"SETAP",atcmd_addAP},//cleansd//atcmd_saveadp
	//atcmd_addAP
	{"GETAPFILE",atcmd_GETAPFILE},//AT+GETAPFILE\r\n 将wpa_supplicant.conf文件拷贝到ftp跟目录
	{"PUTAP",atcmd_PUTAP},//AT+PUTAP\r\n 将ftp://write/wpa_supplicant.conf文件存为配置文件
	{"GETSD",atcmd_getSD},//atcmd_getSD
	{"SETIWS",atcmd_iws_setpara},//atcmd_iws_para/
	{"GETIWS",atcmd_iws_getpara},//atcmd_iws_para/
	{"SETSVR",atcmd_iws_setserver},//atcmd_iws_para/
	{"GETSVR",atcmd_iws_getserver},//atcmd_iws_para/
	{"SAVESVR",atcmd_saveserver},////atcmd_saveserver
	{"SETNTP",atcmd_setntp},////atcmd_saveserver
	{"SETIP",atcmd_setip},////atcmd_saveserver
	{"SETSID",atcmd_setsid},////atcmd_saveserver
	{"GETSID",atcmd_getsid},////atcmd_saveserver

};
struct sockaddr_in host_gps_addr; /* cilent's address information */
int get_net_addr(char *ipaddr,char * net_addr)
{
	int ifor,lastdot=0,dotcount=0;
	for(ifor=0;ifor<20;ifor++)
	{	
		net_addr[ifor]=ipaddr[ifor];
		if(ipaddr[ifor]=='.'){
			lastdot=ifor;
			dotcount++;
			if(dotcount==3)
				break;
		}
	}
	net_addr[ifor+1]=0;
	//printf("%s\n",net_addr);
	return 0;
}
inline int recvnbytes(int sockfd,char * ch,int needread,struct sockaddr * cilent,socklen_t * sin_size)
{	
	int readLen,nextreadcount;
	nextreadcount=readLen=0;
	while(nextreadcount!=needread){
		readLen=recvfrom(sockfd, ch+nextreadcount, needread-nextreadcount,0,cilent,sin_size);
		//printf(BLUE"inside %s，readlen=%d,needread=%d\n"NONE, __FUNCTION__,readLen,needread);

		if(readLen<=0){
			return -1;
		}
		nextreadcount+=readLen;
	}
	return nextreadcount;
}


int sendnbytes(int sockfd,char * ch,int needread)
{	
	int readLen,nextreadcount,sin_size_gps;
	sin_size_gps=sizeof(struct sockaddr_in);

	nextreadcount=readLen=0;
	while(nextreadcount!=needread){
		//printf(BLUE"inside %s，readlen=%d,needread=%d\n"NONE, __FUNCTION__,readLen,needread);
		readLen=sendto(sockfd,ch+nextreadcount,needread-nextreadcount,0,(struct sockaddr *)&host_gps_addr,sin_size_gps);
		if(readLen<=0){
			return -1;
		}
		nextreadcount+=readLen;
	}
	return nextreadcount;
}



int atcmd_cmd_check(char * buf,int length)
{
	int cmdstart,cmdend,ifor,jfor;
	int iscmdstartvalid=0,iscmdendvalid=0;
	int iscmddefine;
	//printf(GREEN"function：%s,line:%d,buf=%s\n"NONE,__FUNCTION__,__LINE__,buf);
	for(ifor=0;ifor<15;ifor++)
	{
		if(buf[ifor]=='+'){
			cmdstart=ifor;
			iscmdstartvalid=1;
			//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
			break;
		}
	}
	cmdend=0;
	for(ifor=cmdstart;ifor<15;ifor++)
	{
		//printf("=buf[ifor]=%c\n",buf[ifor]);
		if((buf[ifor]=='=')||((buf[ifor]==0x0d)&&(buf[ifor+1]==0x0a))){
			//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
			cmdend=ifor;
			iscmdendvalid=1;
			break;
		}
	}
	if(cmdstart>=cmdend){
		return -2;
	}

	//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	//printf(RED"cmdstart：%d,cmdend:%d\n"NONE,cmdstart,cmdend);
	//printf(RED"iscmdstartvalid%d,iscmdendvalid:%d\n"NONE,iscmdstartvalid,iscmdendvalid);

	if(iscmdstartvalid&iscmdendvalid)
	{
		//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		for(ifor=0;ifor<ARRAY_SIZE(iws_atcmd);ifor++)
		{
			//printf("ifor=%d\n",ifor);
			iscmddefine=0;
			for(jfor=0;jfor<(cmdend-cmdstart-1);jfor++)
			{
				if(buf[jfor+cmdstart+1]==iws_atcmd[ifor].atcmd[jfor]){
					//printf("start cmd process\n");
					//printf(RED"ifor=%d,jfor=%d,function：%s,line:%d,buf[jfor+cmdstart+1]=%c\n"NONE,ifor,jfor,__FUNCTION__,__LINE__,buf[jfor+cmdstart+1]);
					iscmddefine=1;
				}
				else{
					iscmddefine=0;
					break;
				}
			}
			//printf("iscmddefine=%d\n",iscmddefine);
			if(iscmddefine){
				//buf[length-3]='\0';
				iws_atcmd[ifor].func(buf+cmdend+1,length-cmdend-1);
				return 0;
				break;
			}
		}
		//printf();
	}
	return 0;
}

int atcmd_pre_check(char * buf,int length)
{
	printf("atcmd_pre_check length=%d\n",length);
	if(buf[0]=='A' && buf[1]=='T' && buf[2]=='+' && buf[length-2]==0x0d && buf[length-1]==0x0a){
		printf("ok\n");
		if(0==atcmd_cmd_check(buf,length)){
			return 0;
		}
		
	}
	else{
		int ifor;
		for(ifor=0;ifor<length;ifor++){
			printf("%x ",buf[ifor]);
		}
		printf("%x%x%x%x%x\n",buf[0],buf[1],buf[2],buf[length-2],buf[length-1]);
		printf("err atcmd\n");
	}
	printf("end\n");
	return -1;
}

//
int udp_atcmd_main1(APP_S * app)
{	
	char net_addr[20];
	int i_state;
	fd_set   t_set1;
    struct timeval  tv;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in cilent; /* cilent's address information */
	struct sockaddr_in host_socket_addr; /* cilent's address information */
	socklen_t sin_size;
	int sockid;
	if ((sockid = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		/* handle exception */
		perror("Creating socket failed.");
		exit(1);
	}
	bzero(&cilent,sizeof(cilent));
	cilent.sin_family=AF_INET;
	cilent.sin_port=htons(PORT_HOST);
	cilent.sin_addr.s_addr = inet_addr(UPSERVERADDR);
	int struct_len = sizeof(cilent);
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT_HOST);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	sin_size=sizeof(struct sockaddr_in);
	if (bind(sockid, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
		 //handle exception 
		perror("Bind error.");
		exit(1);
	}
	int num ;
	while(app->thread_switch.main_loop)
 	{
			//len = recvfrom(sockfd, recvline, 512, 0, (struct sockaddr *) &ca,  (socklen_t *)&struct_len); 
		num= recvfrom(sockid,recvmsg,512,0,(struct sockaddr_in *)&cilent,&struct_len);
		int ifor;
		printf("num=%d\n",num);
		for(ifor=0;ifor<num;ifor++){
			printf("%x ",recvbuf[ifor]);
		}
		printf("\n");
	}
	while(app->thread_switch.main_loop)
 	{
		FD_ZERO(&t_set1);
		FD_SET(app->app_udp_socket[UDP_ATCMD].fid, &t_set1);
		tv.tv_sec= 3;//myoso->para.beat_bt;
		tv.tv_usec= 0;//200000;
		i_state= 0;
		i_state= select(app->app_udp_socket[UDP_ATCMD].fid +1, &t_set1, NULL, NULL, &tv);
		if (i_state < 0) {
			close(app->app_udp_socket[UDP_ATCMD].fid);
			printf("xintiao 在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
			printf(LIGHT_CYAN"in main while sockfd_server=%d\n"NONE,app->app_udp_socket[UDP_ATCMD].fid);
			//continue;
			return -1;
		};
		if (i_state > 0){//有呼吸回应
			//len = recvfrom(sockfd, recvline, 512, 0, (struct sockaddr *) &ca,  (socklen_t *)&struct_len); 
			int num = recvfrom(app->app_udp_socket[UDP_ATCMD].fid,recvmsg,512,0,(struct sockaddr_in *)&cilent,&struct_len);
			int ifor;
			printf("num=%d\n",num);
			for(ifor=0;ifor<num;ifor++){
				printf("%x ",recvbuf[ifor]);
			}
			printf("\n");
			if(atcmd_pre_check(recvmsg,num)==0){
				//sendnbytes(app->app_udp_socket[UDP_ATCMD].fid,"OK",2);
				//return_callback(return_callback_buf,return_callback_len);
				sendto(app->app_udp_socket[UDP_ATCMD].fid,sendbuf,sendlen,0,(struct sockaddr *)&cilent,&sin_size);
			}
			else{
				sendto(app->app_udp_socket[UDP_ATCMD].fid,"ERR ATCMD\n",sizeof("ERR ATCMD\n"),0,(struct sockaddr *)&cilent,&sin_size);
			}
			

		}
		else{//如果没收到呼吸回应
			//sendto(sockfd_server,sendmsg,BEAT_PAK_SIZE,0,(struct sockaddr *)&host_socket_addr,sin_size);
		}
	}
	app->app_udp_socket[UDP_ATCMD].fid=close(app->app_udp_socket[UDP_ATCMD].fid); /* close listenfd */
}

/*
	ad_reg_default->reg[REG_STATUS]=0x01;
	ad_reg_default->reg[REG_MUX]=0x01;
	ad_reg_default->reg[REG_ADCON]=0x20;
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
	ad_reg_default->reg[CTRL_CH_MUX4]=0x01;
	ad_reg_default->reg[CTRL_CH_MUX5]=0x01;
	ad_reg_default->reg[CTRL_CH_MUX6]=0x01;
	ad_reg_default->reg[CTRL_CH_MUX7]=0x01;

 41 54 2b 53 45 54 41 44 3d 01 01 20 B0 E0 00 00 00 00 00 00 04 01 23 45 67 01 01 01 01 0d 0a //AT+SETAD=[AD_REG_S]
*/







// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <string.h>



void handle_udp_msg(int fd)
{	app.app_udp_socket[UDP_ATCMD].fid=fd;
    char buf[BUFF_LEN];  //接收缓冲区，1024字节
    socklen_t len;
    int count;
    struct sockaddr_in clent_addr;  //clent_addr用于记录发送方的地址信息
    int ifor;
    while(app.thread_switch.main_loop)
    {
        //memset(buf, 0, BUFF_LEN);
        len = sizeof(clent_addr);
        count = recvfrom(app.app_udp_socket[UDP_ATCMD].fid, recvbuf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
        if(count == -1)
        {
            printf("recieve data fail!\n");
            return;
        }
		
		printf("count=%d\n",count);
		for(ifor=0;ifor<count;ifor++){
			printf("%x ",recvbuf[ifor]);
		}
		printf("\n");
		if(atcmd_pre_check(recvbuf,count)==0){
			//sendnbytes(app->app_udp_socket[UDP_ATCMD].fid,"OK",2);
			//return_callback(return_callback_buf,return_callback_len);
			sendto(app.app_udp_socket[UDP_ATCMD].fid,sendbuf,sendlen,0,(struct sockaddr*)&clent_addr, len);
			for(ifor=0;ifor<sendlen;ifor++){
				printf("%02X ",sendbuf[ifor]);
			}
			printf("\n");
			
		}
		else{
			sendto(app.app_udp_socket[UDP_ATCMD].fid,"ERR ATCMD\n",sizeof("ERR ATCMD\n"),0,(struct sockaddr*)&clent_addr, len);
		}
        // printf("client:%s\n",recvbuf);  //打印client发过来的信息
        // memset(recvbuf, 0, BUFF_LEN);
        // sprintf(recvbuf, "I have recieved %d bytes data!\n", count);  //回复client
        // printf("server:%s\n",recvbuf);  //打印自己发送的信息给
        // sendto(app.app_udp_socket[UDP_ATCMD].fid, recvbuf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr, len);  //发送信息给client，注意使用了clent_addr结构体指针

    }
}


/*
    server:
            socket-->bind-->recvfrom-->sendto-->close
*/

int udp_atcmd_main(APP_S * app)
{
    int server_fd, ret;
    struct sockaddr_in ser_addr; 

    server_fd = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
    if(server_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    ser_addr.sin_port = htons(SERVER_PORT);  //端口号，需要网络序转换
	int flag=1,len=sizeof(int); 
	if( setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &flag, len) == -1) 
	{ 
		perror("setsockopt"); 
		exit(1); 
	} 

    ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if(ret < 0)
    {
        printf("atcmd socket bind fail!\n");
        //return -1;
    }
    handle_udp_msg(server_fd);   //处理接收到的数据

    close(server_fd);
    return 0;
}






















