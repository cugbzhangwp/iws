#include <stdio.h>

int read_file2buf2(char * buf,int len,const char * filename,const char * filepath,const char * mode)
{
	int is_dirok=0;
	char tmp[256];
	snprintf(tmp,256,"%s/%s",filepath,filename);
	printf("filepath=%s\n",filepath);
	printf("tmp=%s\n",tmp);

	if(is_file_exist(tmp)==0){
		FILE *fp;
		if (fp=fopen(tmp,mode)){//读取配置文件
			printf("inside %s line %d\n", __FUNCTION__,__LINE__);
			int realwrite=myread(buf,len,fp);
			if(realwrite==len){
				printf("inside %s line %d\n", __FUNCTION__,__LINE__);
				return 0;
			}
			else{
				printf("inside %s line %d\n", __FUNCTION__,__LINE__);
				return -1;
			}
		}
	}
	else{
		return -1;
	}

}

int scanf_file2buf(char * buf,int len,const char * filename,const char * filepath,const char * mode)
{
        int is_dirok=0;
        char tmp[256];
        snprintf(tmp,256,"%s/%s",filepath,filename);
        printf("filepath=%s\n",filepath);
        printf("tmp=%s\n",tmp);

        if(is_file_exist(tmp)==0){
                FILE *fp;
                if (fp=fopen(tmp,mode)){//读取配置文件
                        printf("inside %s line %d\n", __FUNCTION__,__LINE__);
                        int realwrite=myread(buf,len,fp);
                        if(realwrite==len){
                                printf("inside %s line %d\n", __FUNCTION__,__LINE__);
                                return 0;
                        }
                        else{
                                printf("inside %s line %d\n", __FUNCTION__,__LINE__);
                                return -1;
                        }
                }
        }
        else{
                return -1;
        }

}


int main(int argc,char ** argv)
{
	

}
