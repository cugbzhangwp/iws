#include <sd_data.h>
#include <app_main.h>
#include <time.h>    // for gettimeofday()
#include <sys/time.h>    // for gettimeofday()
#include <printcolor.h>
#include <sys/wait.h>
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






















int write_log_file()
{
	//static int is
}

int save_bufinfile(char * buf,int len,const char * filename,const char * filepath,const char * mode)
{
	int is_dirok=0;
	char tmp[256];
	printf("filepath=%s\n",filepath);
	if(is_dir_exist(filepath)==0){
		is_dirok=1;
	}
	else{
		snprintf(tmp,256,"mkdir -p %s",filepath);
		if(try_system(tmp)==0){
			is_dirok=1;
		}
	}
	if(is_dirok==1)
	{
		FILE * fp;
		snprintf(tmp,256,"%s/%s",filepath,filename);
		if (fp=fopen(tmp,mode)){//打开日志文件;
			int realwrite=mywrite(buf,len,fp);
			if(len==realwrite){
				;
				fclose(fp);
				return 0;
			}
			else{
				write_log_file();
				return 1;
			}
		}
	}
}

/*
network={
	ssid="iws"
	psk="Zwp123456"
	key_mgmt=WPA-PSK
}

*/

int read_file2buf(char * buf,int len,const char * filename,const char * filepath,const char * mode)
{
	int is_dirok=0;
	char tmp[256];
	snprintf(tmp,256,"%s/%s",filepath,filename);
	printf("filepath=%s\n",filepath);
	printf("tmp=%s\n",tmp);

	if(is_file_exist(tmp)==0){
		FILE *fp;
		if (fp=fopen(tmp,mode)){//读取配置文件
			printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
			int realwrite=myread(buf,len,fp);
			if(realwrite==len){
				printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
				return 0;
			}
			else{
				printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
				return -1;
			}
		}
	}
	else{
		return -1;
	}

}







/*
int opengpsfile(oso * myoso)
{
	char file_url[80];
	memset(file_url,0,80);
	strcat(file_url,myoso->projpath_now);
	strcat(file_url,"NMEA.gps");
	if (myoso->fp_gps=fopen(file_url,"a+")){//打开日志文件;
		printf(GREEN"GPS FILE IS%s myoso->fp_gps=%d\r\n"NONE,file_url,myoso->fp_gps);
		//exit(0);
		}
		else
		{
		;
		}
		if(myoso->fp_gps!=NULL){
			printf(GREEN"GPS FILE IS %lld \r\n"NONE,myoso->fp_gps);
			//fwrite("LOG FILE IS", sizeof("LOG FILE IS"),1,myoso->fp_proj_filenum);
			//fprintf(myoso->fp_proj_filenum, "%s\n","LOG FILE IS");
	myoso->change_bit.isgpsfileopen=1;
//setbuf(myoso->fp_gps,NULL);
		}
}





int closegpsfile(oso * myoso)
{
myoso->change_bit.isgpsfileopen=0;
fsync_close(myoso->fp_gps);

}




int openlogfile(oso * myoso)
{
	char file_url[80];
	memset(file_url,0,80);
	strcat(file_url,myoso->projpath_now);
	strcat(file_url,"LOG.log");
	if (myoso->fp_log=fopen(file_url,"a+")){//打开日志文件;
		printf(GREEN"LOG FILE IS    %s myoso->fp_log=%d\n"NONE,file_url,myoso->fp_log);
		}
		else
		{
		;
		}
		if(myoso->fp_log!=NULL){
			printf(GREEN"LOG FILE IS %lld \r\n"NONE,myoso->fp_log);
			fwrite(">>OPEN LOG FILE\n", sizeof(">>OPEN LOG FILE\n"),1,myoso->fp_log);
			//fprintf(myoso->fp_proj_filenum, "%s\n","LOG FILE IS");
myoso->change_bit.islogfileopen=1;
//setbuf(myoso->fp_log,NULL);
		}
}


int closelogfile(oso * myoso)
{
myoso->change_bit.islogfileopen=0;
fsync_close(myoso->fp_log);
}


*/








