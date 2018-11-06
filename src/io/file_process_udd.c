#include <file_process.h>
#include <current.h>
#include <commonfun.h>
//#include "filehead.h"
//#define DEBUG
//#define FFT
/*
typedef struct CURRENT
{
int ifor;
unsigned int * pcurrent;
int ad_current;
float current;
float currentp;
float currentn;
unsigned long long sumcurrentn;
unsigned long long sumcurrentp;
int nnum;
int pnum;
int iscurrentvalidn;
int iscurrentvalidp;
}CURRENT;
*/
extern CURRENT tx_current;
extern int current_algorithm;
extern int iscurrent_needupdate;
extern int current_reference_ch;
extern int is_need_update_sdinfo;
int is_have_space=1;//计算是否有足够的空间存储
int is_file_open=0;
char needmvfile[80];
char file_url[80];
char now_file_url[80];
char datacacu[12800];
int ADdata[8000];//提取出的纯Ad值，用去参与其他计算-暂时保留
pthread_t pt_cacu_current;
//批量删除文件=======================>
/*
void error_quit(const char *msg)
{
    printf("Error:%s:%s\n",msg,strerror(errno));
}

void path_change(char *path)
{
    printf("Leave %s Successed . . .\n",getcwd(NULL,0));

    if(chdir(path)==-1)
        error_quit("Chdir");

    printf("Entry %s Successed . . .\n",getcwd(NULL,0));
}

void delfile(char *dirname)
{
    DIR *dirp;
    struct dirent *dir;
    struct stat buf;
    char *p=getcwd(NULL,0);

    if((dirp=opendir(dirname))==NULL)
        error_quit("Opendir");
    path_change(dirname);

    while(dir=readdir(dirp))
    {
        if((strcmp(dir->d_name,".")==0) || (strcmp(dir->d_name,".."))==0)
            continue;
        if(stat(dir->d_name,&buf)==-1)
            error_quit("Get stat");
        if(S_ISDIR(buf.st_mode))
        {
            delfile(dir->d_name);
            continue;
        }
        if(remove(dir->d_name)==-1)
            error_quit(dir->d_name);
        printf("Del %s Successed . . .\n",dir->d_name);

    }

    closedir(dirp);

    path_change(p);
}

*/
//批量删除文件<=======================
/*
#define PI 3.1415926
#define SAMPLENUMBER 512

void InitForFFT();
void MakeWave();
void FFT();

int INPUT[SAMPLENUMBER],DATA[SAMPLENUMBER];
float fWaveR[SAMPLENUMBER],fWaveI[SAMPLENUMBER],w[SAMPLENUMBER];
float sin_tab[SAMPLENUMBER],cos_tab[SAMPLENUMBER];

fft512()
{
int i;
InitForFFT();
MakeWave();
for ( i=0;i
{
fWaveR[i]=INPUT[i];
fWaveI[i]=0.0f;
w[i]=0.0f;
}
FFT(fWaveR,fWaveI);
for ( i=0;i
{
DATA[i]=w[i];
}
while ( 1 ); // break point
}

void FFT(float dataR[SAMPLENUMBER],float dataI[SAMPLENUMBER])
{
int x0,x1,x2,x3,x4,x5,x6,x7,x8,xx;
int i,j,k,b,p,L;
float TR,TI,temp;
for ( i=0;i
{
x0=x1=x2=x3=x4=x5=x6=x7=x8=0;
x0=i&0x01; x1=(i/2)&0x01; x2=(i/4)&0x01; x3=(i/8)&0x01;x4=(i/16)&0x01; x5=(i/32)&0x01; x6=(i/64)&0x01;x7=(i/128)&0x01;x8=(i/256)&0x01;
xx=x0*256+x1*128+x2*64+x3*32+x4*16+x5*8+x6*4+x7*2+x8;
dataI[xx]=dataR[i];
}
for ( i=0;i
{
dataR[i]=dataI[i]; dataI[i]=0; 
}

for ( L=1;L<=9;L++ )
{
b=1; i=L-1;
while ( i>0 ) 
{
b=b*2; i--;
}
for ( j=0;j<=b-1;j++ )
{
p=1; i=9-L;
while ( i>0 )
{
p=p*2; i--;
}
p=p*j;
for ( k=j;k<512;k=k+2*b )
{
TR=dataR[k]; TI=dataI[k]; temp=dataR[k+b];
dataR[k]=dataR[k]+dataR[k+b]*cos_tab[p]+dataI[k+b]*sin_tab[p];
dataI[k]=dataI[k]-dataR[k+b]*sin_tab[p]+dataI[k+b]*cos_tab[p];
dataR[k+b]=TR-dataR[k+b]*cos_tab[p]-dataI[k+b]*sin_tab[p];
dataI[k+b]=TI+temp*sin_tab[p]-dataI[k+b]*cos_tab[p];
}
}
}
for ( i=0;i
{ 
w[i]=sqrt(dataR[i]*dataR[i]+dataI[i]*dataI[i]);
}
}


void InitForFFT()
{
int i;
for ( i=0;i
{
sin_tab[i]=sin(PI*2*i/SAMPLENUMBER);
cos_tab[i]=cos(PI*2*i/SAMPLENUMBER);
}
}

void MakeWave()
{
int i;
for ( i=0;i
{
INPUT[i]=sin(PI*2*i/SAMPLENUMBER*30)*1024;
}
}


*/
//fft=============================================fft//

struct compx
{
float real;
float imag;
};

struct compx EE(struct compx b1,struct compx b2)
{
    struct compx b3 ;
    b3.real=b1.real*b2.real-b1.imag*b2.imag ;
    b3.imag=b1.real*b2.imag+b1.imag*b2.real ;
    return(b3);
}

void myFFT(struct compx * xin,int N)
{
    int f,m,nv2,nm1,i,k,j=1,l ;
    //int f,m,nv2,nm1,i,k,j=N/2,l;
    struct compx v,w,t ;
    nv2=N/2 ;
    f=N ;
    for(m=1;(f=f/2)!=1;m++)
    {
        ;
    }
    nm1=N-1 ;
   
    //变址运算
    for(i=1;i<=nm1;i++)
    {
        if(i<j)
        {
            t=xin[j];
            xin[j]=xin[i];
            xin[i]=t ;
        }
        k=nv2 ;
        while(k<j)
        {
            j=j-k ;
            k=k/2 ;
        }
        j=j+k ;
    }
   
    {
        int le,lei,ip ;
        float pi ;
        for(l=1;l<=m;l++)
        {
            le=pow(2,l);
            // 这里用的是L而不是1  !!!!
            lei=le/2 ;
            pi=3.14159 ;
            v.real=1.0 ;
            v.imag=0.0 ;
            w.real=cos(pi/lei);
            w.imag=-sin(pi/lei);
            for(j=1;j<=lei;j++)
            {
                //double p=pow(2,m-l)*j;
                  //double ps=2*pi/N*p;
                //  w.real=cos(ps);
                 // w.imag=-sin(ps);
                for(i=j;i<=N;i=i+le)
                {
                    //w.real=cos(ps);
                     // w.imag=-sin(ps);
                    ip=i+lei ;
                    t=EE(xin[ip],v);
                    xin[ip].real=xin[i].real-t.real ;
                    xin[ip].imag=xin[i].imag-t.imag ;
                    xin[i].real=xin[i].real+t.real ;
                    xin[i].imag=xin[i].imag+t.imag ;
                }
                v=EE(v,w);
            }
        }
    }
    return ;
}


/*****************main programe********************/





float result[257];
struct compx s[257];
int Num=256 ;
const float pp=3.14159 ;

fft256()
{
    int i=1 ;
/*
    for(;i<0x101;i++)
    {
        s[i].real=data;
        s[i].imag=0 ;
    }*/
   
    myFFT(s,Num);
   
    for(i=1;i<0x101;i++)
    {
        result[i]=sqrt(pow(s[i].real,2)+pow(s[i].imag,2));
    }

}

//fft=============================================fft//


static inline unsigned long long get_space_info(oso * myoso)
{
	//oso_n_statfs mystatfs;
	statfs(myoso->sd_info.tgt,&(myoso->sd_info.diskInfo));
	unsigned long long blocksize = myoso->sd_info.diskInfo.f_bsize;// 每个block里面包含的字节数
	unsigned long long totalsize = blocksize * myoso->sd_info.diskInfo.f_blocks;//总的字节数
	//unsigned long long freeDisk = myoso->sd_info.diskInfo.f_bfree * blocksize; //剩余空间的大小  
	myoso->info.free_sd =myoso->sd_info.diskInfo.f_bfree * blocksize; //剩余空间的大小  
	myoso->info.free_mem = blocksize * myoso->sd_info.diskInfo.f_blocks; //剩余空间的大小  
	unsigned long long availableDisk =myoso->sd_info.diskInfo.f_bavail * blocksize;   //可用空间大小
	//printf(RED"YOU GOT %lld BYTE FREE SD SPACE TO WRITE\n"NONE,myoso->info.free_sd );
	//printf(CYAN"YOU GOT %lldM BYTE FREE SD SPACE TO WRITE\n"NONE,totalsize/1024/1024);
	//printf(CYAN"YOU GOT %lldG BYTE FREE SD SPACE TO WRITE\n"NONE,totalsize/1024/1024/1024);
	//if(totalsize==0){
	//printf("NO SD CARD WE WILL JUST SEND TO RJ-45\r\n");	
	//}
return myoso->info.free_sd;
}








inline int writelog_filewrite(oso * myoso ,const char * str)
{
	//printf("in write log\n");
	char connectinfo[LOGSTRSIZE];
	//printf("in write log\n");
	memset(connectinfo,0,LOGSTRSIZE);
	//printf("in write log\n");
	if(myoso->change_bit.islogfileopen==1)
	{
	//printf("in write log1\n");
	sprintf(connectinfo,"[%lu]:[%lu]:[filewrite]:[%s]\n",pthread_self(),myoso->osotime.timep,str);
	//printf("in write log2\n");
	fwrite(connectinfo, strlen(connectinfo),1,myoso->fp_log);
//printf("in write log3\n");
	}
}

static inline myremount(char * filename,char * mode,FILE * fp)
{
	do{
		system("mount -o remount,rw /mnt/sd");
		fp=fopen(filename,mode);
		}while(fp==NULL);

}









inline int writedata(char * buffer_write,int needwritebyte,FILE * fp)
{

	int realwrite,nwrite,writetimes;
	realwrite=nwrite=writetimes=0;
	do{//写文件部分
	ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);
	nwrite=fwrite(buffer_write+realwrite, 1,needwritebyte-realwrite,fp);
	writetimes++;
	realwrite+=nwrite;
	ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
	}while(realwrite!=needwritebyte);
	return realwrite;
}







#define DEBUG_1





unsigned int  file_process_udd(oso * myoso)
{	
	writelog_filewrite(myoso ,"FILE SAVE THREAD GOING");
	int ch_data_size;//data size except gps ch;
	double ch_data_sum[10]={0};//max 10 ch;
	myoso->status_bit.file_status=1;
	filehead filehead;
	//ioctl(fdio,103,0);
	filehead.para=&myoso->para;
	filehead.filename_write=&myoso->filename_write;
	filehead.count=&myoso->count;
	filehead.info=&myoso->info;
	int start_ch=0;
	int firstisrun=1;
	int nwrite;
	int ifft;
	int ifor_write;//写缓冲循环
	int ifor_ch;
	char  filepath[50];
	FILE * fp;
	FILE * fp_fft;

#ifdef DEBUG
	FILE *fp_data;
	FILE * fp_ch[16];
#endif
#ifdef DEBUG_1
	FILE *fp_data_1;
#endif
	//char *filepath="/mnt/nfs/data/";
	/*
	if(is_dir_exist("/mnt/nfs/data/")==0){
	//printf(PURPLE"is_dir_exist 0\n"NONE);
		strcpy(filepath, "/mnt/nfs/data/");
	printf(RED"filepath=1%s\n"NONE,filepath);
	}
	else {
	printf(PURPLE"is_dir_exist 1\n"NONE);
	strcpy(filepath, "/mnt/sd/");
	printf(RED"filepath=2%s\n"NONE,filepath);
	}*/
	strcpy(filepath, myoso->projpath_now);
	char file_time_name[40];
	char file_name_temp[40];

	int realwrite;
	int writetimes=0;
	memset(file_url,0,40);
	strcpy(file_time_name, myoso->filename_write);
	printf(PURPLE"IN FILE THREAD\n"NONE);
	myoso->ledctrl.ledswitch.led1=1;
	unsigned long long file_size=(unsigned long long)(myoso->buffer_size*myoso->para.para_file_size);//=(unsigned long)(FILESIZE*(1<<myoso->para.para_file_size));
	is_have_space=((get_space_info(myoso)-1000000)>file_size)?1:0;//更新存储空间信息状态;

	//printf(BLUE"file_size=%lld,get_space_info(myoso)=%lld\n"NONE,file_size,get_space_info(myoso));


	char tmpfilepath[]="/OL_ACQ/";


	while(ctltotal&&!(myoso->control.file_loop))
	{
		//file_size=(unsigned long)(FILESIZE*(1<<myoso->para.para_file_size));
		//printf(PURPLE"========BLOCK IN FILE PROCESSsig_data_save=======%d\n"NONE,myoso->oso_sig.sig_data_save);

		pthread_mutex_lock(&myoso->mut[MUTALL]);
		while(ispause&&(myoso->signal_bit.file_signal==0))
			{
			printf(GREEN"FILE PROCESS AD PAUSED\n"NONE);
			if((myoso->change_bit.isfirstin==0)||(myoso->change_bit.isnospace==1)){
				myoso->change_bit.isfirstin=1;
			}
			else{
				printf(RED"BEFORE FILE CLOSE\n"NONE);
				pthread_mutex_lock(&(myoso->mut[MUT_FILE_CLOSE]));
				if(myoso->fp!=NULL){
					fclose(myoso->fp);
					myoso->fp=NULL;
				}
#ifdef DEBUG
				if(myoso->fp_data!=NULL){
					fclose(myoso->fp_data);
					myoso->fp_data=NULL;
				}
#endif

				pthread_mutex_unlock(&(myoso->mut[MUT_FILE_CLOSE]));
				printf(RED"AFTER FILE CLOSE\n"NONE);
			}
			pthread_cond_wait(&myoso->cond[CONDALL], &myoso->mut[MUTALL]);
			if(ctltotal==0){
				pthread_mutex_unlock(&myoso->mut[MUTALL]);
				//close(fd);
				printf(GREEN"FILE PROCESS EXIT\n"NONE);
				return 0;
				}
			if(1==is_file_open)
				{
				myoso->count.file_write_count=0;
				strcpy(file_name_temp,file_time_name);
				}
			printf(GREEN"FILE PROCESS RESUME\n"NONE);
			}
		pthread_mutex_unlock(&myoso->mut[MUTALL]);




		pthread_mutex_lock(&(myoso->mut[MUT_FILE_NAME]));
		while(myoso->oso_sig.sig_data_save<=0)
			{
			ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
			pthread_cond_wait(&(myoso->cond[COND_FILE_NAME]), &(myoso->mut[MUT_FILE_NAME]));
			file_size=(unsigned long long)(myoso->buffer_size*myoso->para.para_file_size);
						//ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
			//printf(PURPLE"========AFTER FILESIZE=======%llu\n"NONE,file_size);
			//printf(GREEN"file_size= %llu byte \r\n"NONE,file_size);
			}
		myoso->oso_sig.sig_data_save--;
		if(myoso->opts.isdebug)
			printf(PURPLE"========BLOCK IN FILE PROCESSsig_data_save=======%d\n"NONE,myoso->oso_sig.sig_data_save);
		strcpy(file_time_name, myoso->filename_write);
		pthread_mutex_unlock(&(myoso->mut[MUT_FILE_NAME]));

		//printf(GREEN"========sig_data_save=======%d\n"NONE,myoso->oso_sig.sig_data_save);
/*
		pthread_mutex_lock(&(myoso->mut[MUT_CLIENT_FILE]));
		while(myoso->signal_bit.file_signal==0)
			{
			printf(PURPLE"========BLOCK IN FILE PROCESS=======\n"NONE);
			pthread_cond_wait(&(myoso->cond[COND_CLIENT_FILE]), &(myoso->mut[MUT_CLIENT_FILE]));
			file_size=(unsigned long long)(myoso->buffer_size*myoso->para.para_file_size);
			printf(GREEN"file_size= %llu byte \r\n"NONE,file_size);
			}
		myoso->signal_bit.file_signal=0;
		pthread_mutex_unlock(&(myoso->mut[MUT_CLIENT_FILE]));*/
		
		//printf(PURPLE"GOT MSG FROM AD THREAD myoso->is_save_file=%d\n"NONE,myoso->is_save_file);
		
		//printf(PURPLE"is_have_space=%d,user_is_save_file=%d,myoso->para.bediscardsd=%d\n"NONE,is_have_space,user_is_save_file,myoso->para.bediscardsd);

		if(is_have_space&&user_is_save_file&&(!myoso->para.bediscardsd)){//是否进入保存文件处理过程--调试模式
		//if(is_have_space&&(myoso->is_save_file)&&user_is_save_file){//是否进入保存文件处理过程--正常模式
		//printf(RED"is_have_space=%d,user_is_save_file=%d,myoso->para.bediscardsd=%d\n"NONE,is_have_space,user_is_save_file,myoso->para.bediscardsd);
			myoso->timer.file_process++;
			ch_data_size=4*(myoso->oso_head->iFs/5)*(myoso->para.para_ch-2);
//控制灯闪===============>
			if((myoso->isbreakdown&0x1)>>0==1){
				init_led(myoso);
				myoso->isbreakdown=myoso->isbreakdown&0xfffffffe;
			}
			//printf(GREEN"real write %llu byte file_size=%llu\r\n"NONE,(get_space_info(myoso)-1000000),file_size);
			myoso->count.file_write_count_all+=myoso->buffer_size;
			myoso->ledctrl.bcbc[PTH_FILE]=(myoso->bc==0)?0:1;
			if(myoso->ledctrl.bcbc[PTH_FILE]==0){
				if(myoso->ledctrl.ledswitch.led1==1)
					;//ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
				//myoso->ledctrl.ledswitch.led2=(myoso->ledctrl.ledswitch.led2==1)?ioctl(fdio, GPIO_IOC_N_SD_LED2_ON,1):1;
			}
			else{
				if(myoso->ledctrl.ledswitch.led1==1)
					;//ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);//读ssc时灯闪
					//ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
				//myoso->ledctrl.ledswitch.led2=(myoso->ledctrl.ledswitch.led2==1)?ioctl(fdio, GPIO_IOC_N_SD_LED2_OFF,1):1;
			}

//控制灯闪<===============




			//printf(GREEN"real write %llu byte to file\r\n"NONE,myoso->count.file_write_count_all);
			//printf(GREEN"real read %llu byte from ad\r\n"NONE,myoso->count.data_read_count_all);
			//printf(GREEN"real send %llu byte to server\r\n"NONE,myoso->count.data_send_count_all);
			//printf(GREEN"real read-send %llu byte\r\n"NONE,myoso->count.data_read_count_all-myoso->count.data_send_count_all);
			//printf(GREEN"space =%lld,file_size=%lld\r\n"NONE,get_space_info(myoso),file_size);
			myoso->count.file_write_count+=myoso->buffer_size;

			///获取重数据->
			int idata_ch=0,idata,ifor;
			unsigned int * pdata;
			pdata=(unsigned int *)myoso->buffer_write;
			int cacu_fs=5;
			double ch_average;
			double qinjiao;
			if((myoso->timer.file_process%cacu_fs)==0){//
				for(ifor=0;ifor<10;ifor++)
				{
					ch_data_sum[ifor]=(ch_data_sum[ifor]/(cacu_fs*myoso->oso_head->iFs/5))/2147483647.0*2.5;
				}
				qinjiao=atan(ch_data_sum[2]/sqrt(ch_data_sum[4]*ch_data_sum[4]+ch_data_sum[3]*ch_data_sum[3]));
				printf(GREEN"qinjiao=%lf ch_data_sum[0]=%lf ch_data_sum[1]=%lf ch_data_sum[2]=%lf ch_data_sum[3]=%lf ch_data_sum[4]=%lf\
			 	\n"NONE,qinjiao,\
			 	ch_data_sum[0],\
				ch_data_sum[1],\
				ch_data_sum[2],\
				ch_data_sum[3],\
				ch_data_sum[4]);
				//printf(GREEN"myoso->timer.file_process=%d\n",myoso->timer.file_process);
				ch_data_sum[0]=ch_data_sum[1]=ch_data_sum[2]=ch_data_sum[3]=ch_data_sum[4]=ch_data_sum[5]=0;
			}
			for(idata=0;idata<myoso->oso_head->iFs/5;idata++)
			{
				for (idata_ch=0;idata_ch<myoso->para.para_ch-2;idata_ch++)
				{
					ADdata[idata*(myoso->para.para_ch-2)+idata_ch]=\
					(*(pdata+myoso->para.para_ch*idata+\
						2+idata_ch));
					//printf(RED"idata_ch=%d,ch_data_sum[idata_ch]=%lf \t",idata_ch,ch_data_sum[idata_ch]);

					ch_data_sum[idata_ch]+=(double)(ADdata[idata*(myoso->para.para_ch-2)+idata_ch]<<4);
				}
				//printf("\n");
				//printf(GREEN"ADdata[idata*(myoso->para.para_ch-2)+idata_ch]=%d\r\n"NONE,ADdata[idata*(myoso->para.para_ch-2)+idata_ch]);
			}
			//获取重数据<-*/
			//printf(BLUE"myoso->oso_head->iFs/5=%d idata*(myoso->para.para_ch-2)+idata_ch=%d\n"NONE,myoso->oso_head->iFs/5,idata*(myoso->para.para_ch-2)+idata_ch);
			//printf(BLUE"ADdata[0]=%X ADdata[1]=%X ADdata[2]=%X ADdata[3]=%X ADdata[4]=%X ADdata[1]F=%lf\n"NONE,\
				ADdata[0]<<4,ADdata[1]<<4,ADdata[2]<<4,ADdata[3]<<4,ADdata[4]<<4,(double)(ADdata[1]<<4));
			//printf(RED"ch_data_sum[0]=%lf ch_data_sum[1]=%lf ch_data_sum[2]=%lf ch_data_sum[3]=%lf ch_data_sum[4]=%lf\
			 \n"NONE,(ch_data_sum[0]/(myoso->oso_head->iFs/5))/2147483647.0*2.5,\
				(ch_data_sum[1]/(myoso->oso_head->iFs/5))/2147483647.0*2.5,\
				(ch_data_sum[2]/(myoso->oso_head->iFs/5))/2147483647.0*2.5,\
				(ch_data_sum[3]/(myoso->oso_head->iFs/5))/2147483647.0*2.5,\
				(ch_data_sum[4]/(myoso->oso_head->iFs/5))/2147483647.0*2.5);

			//printf(GREEN"%lf\n", ch_data_sum[1]);








			if(myoso->count.file_write_count==(unsigned long long)myoso->buffer_size){//新建文件
				myoso->oso_head->nFileOrder++;//文件头序号++
				myoso->para.filename_num++;//文件头名++
				memset(file_url,0,80);
				//strcat(file_url,myoso->projpath_now);
				if(myoso->opts.isdebug)
					printf(GREEN"--------------------myoso->buffer_write=%d------------------\n"NONE,myoso->buffer_write);
				//strcat(file_url,file_time_name);
				//strcat(file_url,abl_file_ext_name);
				snprintf(file_url,80,"%s%s%s",myoso->projpath_now,file_time_name,abl_file_ext_name);//0614正常使用模式
				snprintf(now_file_url,80,"%s",file_time_name);
				//snprintf(file_url,80,"%s%s%s",tmpfilepath,file_time_name,abl_file_ext_name);//测试模式
				//snprintf(needmvfile,80,"%s",file_url);//测试模式
				if(myoso->opts.isdebug)
					printf(GREEN"--------------------NEW FILE %s CREATED------------------\n"NONE,file_url);
						//fclose(myoso->fp_proj_filenum);
				//fwrite(file_url, strlen(file_url),1,myoso->fp_proj_filenum);
				//printf(RED"=====a->NEW FILE CREAT file_time_name====%s\r\n"NONE,file_url);
				myoso->fp=fopen(file_url,"wb+");

				while(myoso->fp==NULL)//如果文件打开错误则重新挂载sd卡
					{
					//exit(0);
					ioctl(fdio, GPIO_IOC_N_LED2_ERR,1);
					system("mount -o remount,rw /mnt/sd");
					//sleep(2);
					//system("mount /dev/mmcblk0p1 /mnt/sd");
					//sleep(1);
					writelog_filewrite(myoso ,"File system has been set read-only");
					myoso->fp=fopen(file_url,"wb+");
					}
				writelog_filewrite(myoso ,file_url);
				myoso->oso_head->gps_lable1=*(unsigned int *)(myoso->buffer_write);
				myoso->oso_head->gps_lable2=*((unsigned int *)(myoso->buffer_write)+1);

#ifdef DEBUG
				if(1){//运行时条件
					memset(file_url,0,80);
					strcat(file_url,myoso->projpath_now);
					//printf(GREEN"--------------------myoso->buffer_write=%d------------------\n"NONE,myoso->buffer_write);
					strcat(file_url,file_time_name);
					strcat(file_url,".dat");

					myoso->fp_data=fopen(file_url,"wb+");
					fwrite(myoso->oso_head, sizeof(oso_head),1,myoso->fp_data);
					for(ifor_ch=0;ifor_ch<myoso->para.para_ch;ifor_ch++)
					{
						memset(file_url,0,80);
						snprintf(file_url,80,"%s%s-ch%d.chn",myoso->projpath_now,file_time_name,ifor_ch);
						fp_ch[ifor_ch]=fopen(file_url,"wb+");
						fwrite(myoso->oso_head, sizeof(oso_head),1,fp_ch[ifor_ch]);
					}
				}
#endif
#ifdef DEBUG_1
				if(1){//运行时条件
					memset(file_url,0,80);
					strcat(file_url,myoso->projpath_now);
					//printf(GREEN"--------------------myoso->buffer_write=%d------------------\n"NONE,myoso->buffer_write);
					strcat(file_url,file_time_name);
					strcat(file_url,".data");

					fp_data_1=fopen(file_url,"wb+");
					//fwrite(myoso->oso_head, sizeof(oso_head),1,myoso->fp_data_1);
				}
#endif











#ifdef FFT


				memset(file_url,0,80);
				strcat(file_url,myoso->projpath_now);
				//printf(GREEN"--------------------myoso->buffer_write=%d------------------\n"NONE,myoso->buffer_write);
				strcat(file_url,file_time_name);
				strcat(file_url,".fft");
				fp_fft=fopen(file_url,"wb+");

#endif

				//printf(RED"=====b->NEW FILE CREAT file_time_name====%s\r\n"NONE,file_url);
				//memset(file_url,0,80);
				//fwrite(filehead.para, myoso->oso_size.para_s,1,fp);
				//fwrite(filehead.filename_write, 20,1,fp);
				//fwrite(filehead.count, myoso->oso_size.counter,1,fp);
				//fwrite(filehead.info, myoso->oso_size.oso_n_info_s,1,fp);
				fwrite(myoso->oso_head, sizeof(oso_head),1,myoso->fp);// write main header
				//showhead(myoso->oso_head);
				//printf(GREEN"after open file%s\r\n"NONE);
				is_file_open=1;
				//printf("file_time_name=%s,datanumname=%s\n",file_time_name,datanamenum);
				//printf(GREEN"after open file%s\r\n"NONE);
			}




			else if(myoso->count.file_write_count==file_size){//文件满
				if(myoso->opts.isdebug)
					printf(GREEN"-------------------------------------------file full-----------------------------------------\n"NONE);
				//printf(GREEN"WRITE FILE\r\n"NONE);
				/*
				realwrite=nwrite=writetimes=0;


				//for(ifor_write=0;ifor_write<=myoso->oso_sig.sig_data_save;ifor_write++)
					//{
				do{
					//if(writetimes)
						//printf(RED"writetimes =%d nwrite=%d\
						 myoso->buffer_size=%d realwrite=%d will write:%d\n"NONE,\
						writetimes,nwrite,myoso->buffer_size,realwrite,myoso->buffer_size-realwrite);
					ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);
					nwrite=fwrite((char *)(myoso->buffer_write)+realwrite, 1,myoso->buffer_size-realwrite,myoso->fp);
					writetimes++;
					realwrite+=nwrite;
					ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
					}while(realwrite!=myoso->buffer_size);
					//printf(BLUE"myoso->oso_sig.sig_data_save=%d\n"NONE,myoso->oso_sig.sig_data_save);
					//printf(RED"myoso->buffer_write=%d\n"NONE,myoso->buffer_write);

				//	}

				*/
				writedata((char *)(myoso->buffer_write),myoso->buffer_size,myoso->fp);
				myoso->buffer_write=(void *)loopwrite->loopbufnext+6144;//调整写缓冲到下一个
				loopwrite=loopwrite->loopbufnext;


#ifdef DEBUG//测试分通道写如n个文件
				realwrite=nwrite=writetimes=0;
				do{
					//if(writetimes)
						//printf(RED"writetimes =%d nwrite=%d\
						 myoso->buffer_size=%d realwrite=%d will write:%d\n"NONE, writetimes,nwrite,myoso->buffer_size,realwrite,myoso->buffer_size-realwrite);
					nwrite=fwrite((char *)(myoso->buffer_write)+writetimes*16+8, 1,8,myoso->fp_data);
					writetimes++;
					realwrite+=nwrite;
					}while(realwrite!=800*2*4);


				for(ifor_ch=0;ifor_ch<myoso->para.para_ch;ifor_ch++)
				{
				realwrite=nwrite=writetimes=0;
				do{
					//if(writetimes)
						//printf(RED"writetimes =%d nwrite=%d\
						 myoso->buffer_size=%d realwrite=%d will write:%d\n"NONE,\
						writetimes,nwrite,myoso->buffer_size,realwrite,myoso->buffer_size-realwrite);
					nwrite=fwrite((char *)(myoso->buffer_write)+writetimes*16+ifor_ch*4, 1,4,fp_ch[ifor_ch]);
					writetimes++;
					realwrite+=nwrite;
					}while(realwrite!=800*1*4);//800个数，1个通道，4字节
				}



//printf(RED"writetimes =%d nwrite=%d\
						 myoso->buffer_size=%d realwrite=%d will write:%d\n"NONE,\
						writetimes,nwrite,myoso->buffer_size,realwrite,myoso->buffer_size-realwrite);
				//fwrite(myoso->buffer_write, myoso->buffer_size,1,myoso->fp_data);
#endif

#ifdef DEBUG_1
				writedata((char *)(ADdata),ch_data_size,fp_data_1);
#endif







#ifdef FFT

				for(ifft=0;ifft<256;ifft++)
				{
					s[ifft].real=*((unsigned int *)(myoso->buffer_write)+3+ifft*4);
					s[ifft].imag=0 ;
				}
				fft256();
				fwrite(result, 1,257*4,fp_fft);
#endif






				//ioctl(fdio,103,0);//写文件时灯闪
								
/*
				if(myoso->fp!=NULL){
					fclose(myoso->fp);
					myoso->fp=NULL;
				}
				if(myoso->fp_data!=NULL){
					fclose(myoso->fp_data);
					myoso->fp_data=NULL;
				}
*/
				pthread_mutex_lock(&(myoso->mut[MUT_FILE_CLOSE]));//通知文件关闭线程关闭文件
#ifdef DEBUG
				myoso->fp_data_close=myoso->fp_data;
				for(ifor_ch=0;ifor_ch<myoso->para.para_ch;ifor_ch++)
				{

					fclose(fp_ch[ifor_ch]);

				}

#endif
#ifdef DEBUG_1
				fclose(fp_data_1);
#endif
				myoso->fp_close=myoso->fp;
				pthread_cond_broadcast(&(myoso->cond[COND_FILE_CLOSE]), &(myoso->mut[MUT_FILE_CLOSE]));
				pthread_mutex_unlock(&(myoso->mut[MUT_FILE_CLOSE]));
				is_file_open=0;//设置文件打开标志为否
				//pthread_cond_signal(&cond_cilent); 
				myoso->count.file_write_count=0;//文件写计数复位为0
				is_have_space=((get_space_info(myoso)-1000000)>file_size)?1:0;//更新存储空间信息状态;
				//printf(GREEN"MISS_WRITE =%d nwrite=%d myoso->buffer_size=%d myoso->fp=%d\n"NONE,--myoso->miss_write,nwrite,myoso->buffer_size,myoso->fp);

			}

			if(1==is_file_open){//如果文件为打开状态则执行此过程
								//printf(RED"WRITE FILE\r\n"NONE);
								//memcpy(datacacu,myoso->buffer_write,12800);
				if(current_algorithm==0){
					//printf(RED"myoso->para.para_ch=%d\n"NONE,myoso->para.para_ch);
					int iivalidnum=0;
					memset(&tx_current,0,sizeof(CURRENT));
					tx_current.pcurrent=(int *)myoso->buffer_write;
					//printf(GREEN"*(tx_current.pcurrent)=%X\n"NONE,*(tx_current.pcurrent));
					for(tx_current.ifor=0;tx_current.ifor<myoso->oso_head->iFs/5;tx_current.ifor++)
					{
						tx_current.ad_current=(*(tx_current.pcurrent+myoso->para.para_ch*tx_current.ifor+myoso->para.para_ch-current_reference_ch))<<4;
						/*
						ADdata[tx_current.ifor]=tx_current.ad_current;
						for (idata_ch=0;idata_ch<myoso->para.para_ch-2;idata_ch++)
						{
							ADdata[tx_current.ifor*(myoso->para.para_ch-2)+idata_ch]=\
							(*(tx_current.pcurrent+myoso->para.para_ch*tx_current.ifor+\
								2+idata_ch));
						}
						*/
				/*
						if(tx_current.ifor<1){
						printf(GREEN"CH%d=%X\n",current_reference_ch,(*(tx_current.pcurrent+myoso->para.para_ch*tx_current.ifor+myoso->para.para_ch-current_reference_ch)));
						int itemp;
						for(itemp=myoso->para.para_ch-1;itemp>=0;itemp--)
							{
						printf(RED"CH%d=%X\n",itemp,(*(tx_current.pcurrent+myoso->para.para_ch*tx_current.ifor+myoso->para.para_ch+itemp)));
							}
						}
				*/
						//printf("\n");
						if(tx_current.ad_current<0){
							tx_current.ad_current=tx_current.ad_current*(-1);
							//printf("a tx_current.sumcurrent=%lld\n",tx_current.sumcurrent);
							}
							tx_current.sumcurrent+=tx_current.ad_current;
					}
				/*
					if(tx_current.sumcurrent<0){
						printf("b tx_current.sumcurrent=%lld\n",tx_current.sumcurrent);
						tx_current.sumcurrent=tx_current.sumcurrent*(-1);
						//printf("a tx_current.sumcurrent=%lld\n",tx_current.sumcurrent);
						}*/
						tx_current.current=(tx_current.sumcurrent/(myoso->oso_head->iFs/5))/2147483647.0*2.5;
							//printf("tx_current.current=%f\n",tx_current.current);
						isneedupdate=1;
						iscurrent_needupdate=1;
				}
				else if(current_algorithm==1)//0为默认
				{
					
								//printf(RED"myoso->para.para_ch=%d\n"NONE,myoso->para.para_ch);
					memset(&tx_current,0,sizeof(CURRENT));
					tx_current.pcurrent=(int *)myoso->buffer_write;
								//printf(RED"*(tx_current.pcurrent)=%X\n"NONE,*(tx_current.pcurrent));
					for(tx_current.ifor=0;tx_current.ifor<800;tx_current.ifor++)
					{
						tx_current.ad_current=(*(tx_current.pcurrent+myoso->para.para_ch*tx_current.ifor+myoso->para.para_ch-current_reference_ch))<<4;
							//printf(RED"tx_current.ad_current=%d\n"NONE,tx_current.ad_current);
						if(tx_current.ad_current>0){
							tx_current.sumcurrentp+=tx_current.ad_current;
							tx_current.pnum++;
							}
						else{
							tx_current.sumcurrentn+=tx_current.ad_current;
							//printf(GREEN"tx_current.sumcurrentn=%lld,tx_current.ad_current=%d\n"NONE,tx_current.sumcurrentn,tx_current.ad_current);
							tx_current.nnum++;
							}

					}

					//printf(GREEN"tx_current.nnum=%d,tx_current.pnum=%d\n"NONE,tx_current.nnum,tx_current.pnum);

					if(tx_current.nnum>20){
					//printf(GREEN"tx_current.sumcurrentn=%lld,abs(tx_current.sumcurrentn)=%lld\n",tx_current.sumcurrentn,abs(tx_current.sumcurrentn));
					tx_current.iscurrentvalidn=1;
					//printf(BLUE"tx_current.sumcurrentn=%lld,tx_current.nnum=%d\n",tx_current.sumcurrentn,tx_current.nnum);
					tx_current.sumcurrentn=tx_current.sumcurrentn*(-1);
					//printf(BLUE"tx_current.sumcurrentn*(-1)=%lld\n",tx_current.sumcurrentn);
					tx_current.currentn=tx_current.sumcurrentn/tx_current.nnum/2147483647.0*2.5;
					//printf(BLUE"tx_current.currentn=%f\n",tx_current.currentn);

					}
					if(tx_current.pnum>20){
					tx_current.iscurrentvalidp=1;
					tx_current.currentp=(tx_current.sumcurrentp/tx_current.pnum)/2147483647.0*2.5;
					//printf(RED"tx_current.sumcurrentp=%lld,tx_current.pnum=%d\n",tx_current.sumcurrentp,tx_current.pnum);
					//printf(RED"tx_current.currentp=%f\n",tx_current.currentp);
					}

					if(tx_current.iscurrentvalidn&&tx_current.iscurrentvalidp)
					{

					tx_current.current=(tx_current.currentp+tx_current.currentn)/2;
					//printf(GREEN"tx_current.currentp=%f,tx_current.currentn=%f,tx_current.pnum=%d,tx_current.nnum=%d,tx_current.current=%f\n"NONE,tx_current.currentp,tx_current.currentn,tx_current.pnum,tx_current.nnum,tx_current.current);
											isneedupdate=1;
							iscurrent_needupdate=1;
					}
				}
				else if(current_algorithm==2)
				{
					memset(&tx_current,0,sizeof(CURRENT));
					tx_current.pcurrent=(int *)myoso->buffer_write;

					for(tx_current.ifor=0;tx_current.ifor<800;tx_current.ifor++)
					{
					tx_current.ad_current=(*(tx_current.pcurrent+myoso->para.para_ch*tx_current.ifor+myoso->para.para_ch-current_reference_ch))<<4;

					tx_current.sumcurrent+=tx_current.ad_current;

					}

					if(tx_current.sumcurrent<0){
						//printf("b tx_current.sumcurrent=%lld\n",tx_current.sumcurrent);
						tx_current.sumcurrent=tx_current.sumcurrent*(-1);
						//printf("a tx_current.sumcurrent=%lld\n",tx_current.sumcurrent);
						}
					tx_current.current=(tx_current.sumcurrent/800)/2147483647.0*2.5;
						//printf("tx_current.current=%f\n",tx_current.current);
					isneedupdate=1;
					iscurrent_needupdate=1;

				}
				else if(current_algorithm==3)
				{

					if(firstisrun){


					if ( pthread_create(&pt_cacu_current, NULL, cacu_current, &firstisrun) ){//从客户读数据
							printf(YELLOW"error creating tcp_server."NONE);
							abort();

							}
						else{
							firstisrun=0;
							printf(YELLOW"CURRENT THREAD GOING\n"NONE);
							}





					}
					else{
									pthread_mutex_lock(&(myoso->mut[MUT_FILE_CLOSE]));

									pthread_cond_broadcast(&(myoso->cond[COND_FILE_CLOSE]), &(myoso->mut[MUT_FILE_CLOSE]));
									pthread_mutex_unlock(&(myoso->mut[MUT_FILE_CLOSE]));
					}



				}






				/*
				realwrite=nwrite=writetimes=0;

				//for(ifor_write=0;ifor_write<=myoso->oso_sig.sig_data_save;ifor_write++)
				//	{

				do{//写文件部分
					ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);
					//if(writetimes)
						//printf(RED"writetimes =%d nwrite=%d\
						 myoso->buffer_size=%d realwrite=%d will write:%d\n"NONE,\
						writetimes,nwrite,myoso->buffer_size,realwrite,myoso->buffer_size-realwrite);
					nwrite=fwrite((char *)(myoso->buffer_write)+realwrite, 1,myoso->buffer_size-realwrite,myoso->fp);
					writetimes++;
					realwrite+=nwrite;
					ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
					}while(realwrite!=myoso->buffer_size);
					//printf(BLUE"myoso->oso_sig.sig_data_save=%d\n"NONE,myoso->oso_sig.sig_data_save);
						//printf(RED"myoso->buffer_write=%d\n"NONE,myoso->buffer_write);
				*/

/*
int debugtest=0;
for(debugtest=0;debugtest<16;debugtest++)//TODO 
{
				realwrite=nwrite=writetimes=0;

				//for(ifor_write=0;ifor_write<=myoso->oso_sig.sig_data_save;ifor_write++)
				//	{
				
				do{//写文件部分
					ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);
					//if(writetimes)
						//printf(RED"writetimes =%d nwrite=%d\
						 myoso->buffer_size=%d realwrite=%d will write:%d\n"NONE,\
						writetimes,nwrite,myoso->buffer_size,realwrite,myoso->buffer_size-realwrite);
					nwrite=fwrite((char *)(myoso->buffer_write)+realwrite, 1,myoso->buffer_size-realwrite,myoso->fp);
					writetimes++;
					realwrite+=nwrite;
					ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
					}while(realwrite!=myoso->buffer_size);
						//printf(BLUE"myoso->oso_sig.sig_data_save=%d\n"NONE,myoso->oso_sig.sig_data_save);
						//printf(RED"myoso->buffer_write=%d\n"NONE,myoso->buffer_write);
}
*/









				//	}
#ifdef DEBUG

				realwrite=nwrite=writetimes=0;
				do{
					//if(writetimes)
						//printf(RED"writetimes =%d nwrite=%d\
						 myoso->buffer_size=%d realwrite=%d will write:%d\n"NONE,\
						writetimes,nwrite,myoso->buffer_size,realwrite,myoso->buffer_size-realwrite);
					nwrite=fwrite((char *)(myoso->buffer_write)+writetimes*16+8, 1,8,myoso->fp_data);
					writetimes++;
					realwrite+=nwrite;
					}while(realwrite!=800*2*4);
						ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);
				for(ifor_ch=0;ifor_ch<myoso->para.para_ch;ifor_ch++)
				{
				realwrite=nwrite=writetimes=0;
				do{
					//if(writetimes)
						//printf(RED"writetimes =%d nwrite=%d\
						 myoso->buffer_size=%d realwrite=%d will write:%d\n"NONE,\
						writetimes,nwrite,myoso->buffer_size,realwrite,myoso->buffer_size-realwrite);
					nwrite=fwrite((char *)(myoso->buffer_write)+writetimes*myoso->para.para_ch*4+ifor_ch*4, 1,4,fp_ch[ifor_ch]);
					writetimes++;
					realwrite+=nwrite;
					}while(realwrite!=800*1*4);//800个数，1个通道，4字节
				}


				ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);


//printf(RED"writetimes =%d nwrite=%d\
						 myoso->buffer_size=%d realwrite=%d will write:%d\n"NONE,\
						writetimes,nwrite,myoso->buffer_size,realwrite,myoso->buffer_size-realwrite);

				//fwrite(myoso->buffer_write, myoso->buffer_size,1,myoso->fp_data);
#endif



#ifdef DEBUG_1
		writedata((char *)(ADdata),ch_data_size,fp_data_1);
#endif















#ifdef FFT

						for(ifft=0;ifft<256;ifft++)
						{
							s[ifft].real=*((unsigned int *)(myoso->buffer_write)+3+ifft*4);
							s[ifft].imag=0 ;
						}
						fft256();
						fwrite(result, 1,257*4,fp_fft);
#endif



				writedata((char *)(myoso->buffer_write),myoso->buffer_size,myoso->fp);
				myoso->buffer_write=(void *)loopwrite->loopbufnext+6144;
				loopwrite=loopwrite->loopbufnext;



				//ioctl(fdio,103,0);
				//((char *)myoso->buffer_write+3);
				//myoso->count.file_write_count_all+=myoso->count.count_one_read;
				//printf(GREEN"after write  file\n"NONE);
				//printf(GREEN"MISS_WRITE =%d nwrite=%d myoso->buffer_size=%d myoso->fp=%d\n"NONE,--myoso->miss_write,nwrite,myoso->buffer_size,myoso->fp);
			}
			myoso->count.count_one_read=0;
			//printf(GREEN"AFTER WRITE MYOSO->MISS_WRITE =%d\r\n"NONE,--myoso->miss_write);
		}
		else{//如果空间不足或者不保存
			if(is_need_update_sdinfo){
				is_have_space=((get_space_info(myoso)-1000000)>file_size)?1:0;//更新存储空间信息状态;
				is_need_update_sdinfo=0;				
			}

			if(is_have_space==0){
			
					;//ioctl(fdio, GPIO_IOC_N_GPIO_DOWN,AT91_PIN_PD24);
			}
				//for(ifor_write=0;ifor_write<=myoso->oso_sig.sig_data_save;ifor_write++)
					//{
						//printf(RED"myoso->buffer_write=%d\n"NONE,myoso->buffer_write);
						//printf(BLUE"myoso->oso_sig.sig_data_save=%d\n"NONE,myoso->oso_sig.sig_data_save);
			myoso->buffer_write=(void *)loopwrite->loopbufnext+6144;
			loopwrite=loopwrite->loopbufnext;
				//	}
			myoso->change_bit.isnospace=1;
/*			if(myoso->isbreakdown==0){
				breakdown_led(myoso);
				myoso->isbreakdown=1;
			}*/
			//myoso->isbreakdown=myoso->isbreakdown| SSCUNKNOWERR;
			//ioctl(fdio,GPIO_IOC_N_LED_BLINK_ALL,1);
			//sleep(1);
			//printf(GREEN"NO SPACE OR OTHER TO WRITE JUST SEND TO WEB real write %lu byte to file\r\n"NONE,myoso->count.file_write_count_all);
			//printf(GREEN"is_have_space=%d,user_is_save_file=%d\n"NONE,is_have_space,user_is_save_file);
			if(1==is_file_open){
				is_file_open=0;
				//printf(GREEN"----------------------------------inside %s needmvfile:%s--------------------------------\n"NONE,__FUNCTION__,needmvfile);
				pthread_mutex_lock(&(myoso->mut[MUT_FILE_CLOSE]));
#ifdef DEBUG
				myoso->fp_data_close=myoso->fp_data;//将文件指针复制到另一变量,以便接下来新打开文件流指向之前的指针
#endif

				myoso->fp_close=myoso->fp;
				pthread_cond_broadcast(&(myoso->cond[COND_FILE_CLOSE]), &(myoso->mut[MUT_FILE_CLOSE]));
				pthread_mutex_unlock(&(myoso->mut[MUT_FILE_CLOSE]));
				myoso->count.file_write_count=0;//关闭文件后下次再读数据需要新建文件
			}	
		}
	}

	if(myoso->fp!=NULL){
		fclose(myoso->fp);
		myoso->fp=NULL;
	}
	//printf(GREEN"YOU GUESS WHAT ? IT WORKS ,WE JUST FINISH MY JOB!---FILE PROG\n"NONE);
	myoso->status_bit.file_status=0;

}
