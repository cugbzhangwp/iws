#include <printcolor.h>


int readfromsocket(char * buffer,int needreadbytes,int fid)
{

  int realreadbytes,nread,readtimes;
  realreadbytes=nread=readtimes=0;
  do{//写文件部分
  //ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);

  nread= recv(fid, buffer+realreadbytes, needreadbytes-realreadbytes, 0);
  printf(RED"inside %s line %d read %d bytes needreadbytes %d bytes\n"NONE, __FUNCTION__,__LINE__,nread,needreadbytes);
  readtimes++;
  realreadbytes+=nread;
  if(nread<=0){
    printf(BLUE"inside %s line %d client exit\n"NONE, __FUNCTION__,__LINE__);
    break;
  }

  //nwrite=fwrite(buffer+realwrite, 1,needwritebyte-realwrite,fid);

  //printf(RED"inside %s line %d read %d bytes realwrite %d bytes\n"NONE, __FUNCTION__,__LINE__,nread,realreadbytes);

  //ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
  }while(realreadbytes!=needreadbytes);
  //printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);

  return realreadbytes;
}

int writetosocket(char * buffer,int needwritebyte,int fid)
{

  int realwrite,nwrite,writetimes;
  realwrite=nwrite=writetimes=0;
  do{//写文件部分
  //ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);
  nwrite= send(fid, buffer+realwrite, needwritebyte-realwrite, 0);
  //nwrite=fwrite(buffer+realwrite, 1,needwritebyte-realwrite,fid);
  writetimes++;
  realwrite+=nwrite;
  //ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
  }while(realwrite!=needwritebyte);
  return realwrite;
}