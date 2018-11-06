#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/ioctl.h>

#include <time.h>

#include <fcntl.h>

#include <linux/input.h>

#include <sys/time.h>

int main(int argc, char **argv)

{

     int key_state;

     int fd;

     int ret;

     int code;

     struct input_event buf[2];

     int repeat_param[2];

     fd = open("/dev/event0", O_RDONLY);

     if (fd < 0)

     {

         printf("Open gpio-keys failed.\n");

         return -1;

  }

  else

  {

     printf("Open gpio-keys success.\n");

  }

     repeat_param[0]=500;//ms重复按键第一次间隔

     repeat_param[1]=66;//ms重复按键后续间隔

     ret = ioctl(fd,EVIOCSREP,(int *)repeat_param);//设置重复按键参数

     if(ret != 0)

         {

                   printf("set repeat_param fail!\n");

         }

     else

         {

                printf("set repeat_param ok.\n");

         }

    struct timeval tv;


     while(1)

     {

         ret = read(fd,&buf,2*sizeof(struct input_event));
					//ret = read(fd,&buf1,sizeof(struct input_event));
printf("end read->code=%d\n",buf[0].code);
printf("end read->type=%d\n",buf[0].type);
printf("end read->value=%d\n",buf[0].value);
printf("end read.time.tv_sec=%d\n",buf[0].time.tv_sec);
printf("end read.time.tv_use=%d\n",buf[0].time.tv_usec);
    gettimeofday(&tv,NULL);
    printf("second:%ld\n",tv.tv_sec);  //秒
    printf("millisecond:%ld\n",tv.tv_sec*1000 + tv.tv_usec/1000);  //毫秒
    printf("microsecond:%ld\n",tv.tv_sec*1000000 + tv.tv_usec);  //微秒
/*
struct timeval {
	__kernel_time_t		tv_sec;		
	__kernel_suseconds_t	tv_usec;
};

*/
         if(ret <= 0)

              {

                   printf("read fail!\n");

                   return -1;

              }

             

       code = buf[0].code;

       key_state = buf[0].value;

       switch(code)

       {

          case KEY_DOWN:

              code = '1';

              break;

          case KEY_ENTER:

              code = '2';

              break;

          case KEY_HOME:

              code = '3';

              break;

          case KEY_POWER:

              code = '4';

              break;

          default:

              code = 0;

              break;

       }

       if(code!=0)

          {

           printf("Key_%c state= %d.\n",code,key_state);

         }

     }

     close(fd);

     printf("Key test finished.\n"); 

     return 0;

}
