#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>//文件打开属性
#include <signal.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <sys/vfs.h>
#include <errno.h>
#include <time.h> //C语言的头文件 
#include <linux/rtc.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <locale.h>
#include <pthread.h>

#include <spidev.h>
#include <display.h>
#include <ltc2943.h>
#include <current.h>
#include <commonfun.h>
#include <spi_rw.h>
#include <runconf.h>
#include <app_main.h>
#include <printcolor.h>



const char * device="/dev/zwp_spi1.1";//oled设备
int fd_led;
static inline void transfer(int fd,uint8_t * tx_buff,uint8_t * rx_buff,int buff_size)
{
	int ret,txlen;
	static uint16_t delay=0;//每个发送字之后的延迟
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx_buff,
		.rx_buf = (unsigned long)rx_buff,
		.len =buff_size,
		.delay_usecs = delay,
		.speed_hz = 0,
		.bits_per_word = 0,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret == 1)
	;
}

int command_spi(unsigned char cmd)
{
	uint8_t rx[] = {//接收缓冲及初始化
		0x00, 0x00, 0x00
	};

	uint8_t tx[] = {//接收缓冲及初始化
		0xf8, 0x00, 0x00
	};
cmd=msb2lsb(cmd);//一个字节大小端转换
//printf("cmd=0x%X\n",(unsigned int)cmd);
tx[1]=cmd&0xf0;
//printf("tx1=0x%X\n",(unsigned int)tx[1]);
tx[2]=(cmd<<4)&0xf0;
//printf("tx2=0x%X\n",(unsigned int)tx[2]);
transfer(fd_led,tx,rx,sizeof(tx));
}

int data_spi(unsigned char cmd)
{
	uint8_t rx[] = {//接收缓冲及初始化
		0x00, 0x00, 0x00
	};

	uint8_t tx[] = {//接收缓冲及初始化
		0xfA, 0x00, 0x00
	};
cmd=msb2lsb(cmd);
printf("cmd=0x%X\n",(unsigned int)cmd);
tx[1]=cmd&0xf0;
tx[2]=(cmd<<4)&0xf0;
transfer(fd_led,tx,rx,sizeof(tx));
}



int data_spi_continue(unsigned char * cmd,int datlen)
{
int ifor;
char data[80];
	uint8_t rx[161] = {//接收缓冲及初始化
		0x20
	};

	uint8_t tx[161] = {//接收缓冲及初始化
		0x20
	};
	datlen=datlen<81?datlen:80;
tx[0]=0xFA;
for(ifor=0;ifor<datlen;ifor++)
{
data[ifor]=msb2lsb(cmd[ifor]);
tx[ifor*2+1]=data[ifor]&0xf0;
tx[ifor*2+2]=(data[ifor]<<4)&0xf0;
}
transfer(fd_led,tx,rx,datlen*2+1);
}






void initoled(int bs)
{
open_led_spi();
int com_seg_direct=0;
switch(bs)
	{
	case BS_SPI:


/*
	command_spi(0x38);//function set
	command_spi(0x06);//entry mode set
	command_spi(0x02);//return home
	command_spi(0x01);//clear display
	command_spi(0x0c);//display on
	command_spi(0x80);//line 1 character 1

*/

		usleep(1000); //delay
		//command_spi(0x07); //打开移动
		//command_spi(0x0f); //function set (extended command set)
		command_spi(0x2A); //function set (extended command set)
		//command_spi(0x81); //垂向滑动
		//command_spi(0x12); 
		//command_spi(0x11); 
		//command_spi(0x18); //function set (extended command set)
		command_spi(0x71); //function selection A
		data_spi(0x00); // disable internal VDD regulator (2.8V I/O). data(0x5C) = enable regulator (5V I/O)
		command_spi(0x28); //function set (fundamental command set)
		command_spi(0x08); //display off, cursor off, blink off

		
		command_spi(0x2A); //function set (extended command set)
		//command_spi(0x0B); 
		command_spi(0x79); //OLED command set enabled
		command_spi(0xD5); //set display clock divide ratio/oscillator frequency
		command_spi(0x70); //set display clock divide ratio/oscillator frequency
		command_spi(0x78); //OLED command set disabled
		command_spi(0x09); //extended function set (4-lines)
		if(com_seg_direct==0)
			command_spi(0x06); //COM SEG direction
		else
			command_spi(0x05); //COM SEG direction	
		command_spi(0x72); //function selection B
		data_spi(0x00); //ROM CGRAM selection

		command_spi(0x2A); //function set (extended command set)

		//command_spi(0x2B); //reverse display反相显示效果不好
		command_spi(0x79); //OLED command set enabled


//新加的初始化内容=>
		command_spi(0xDA); //set SEG pins hardware configuration1
		command_spi(0x10); //set SEG pins hardware configuration1
		command_spi(0xDC); //function selection C
		command_spi(0x00); //function selection C
		command_spi(0x81); //set contrast control1
		command_spi(0x7F); //set contrast control1
		command_spi(0xD9); //set phase length1
		command_spi(0xF1); //set phase length1
		command_spi(0xDB); //set VCOMH deselect level1
		command_spi(0x40); //set VCOMH deselect level1
//新加的初始化内容=<
		command_spi(0x78); //OLED command set disabled
		command_spi(0x28); //function set (fundamental command set)

		command_spi(0x01); //clear display
		command_spi(0x80); //set DDRAM address to 0x00
		command_spi(0x0C); //display ON
		usleep(100*1000); //delay
		printf("--------------------------------------------------------------------------------------------------------------------");
	break;
	case BS_IIC:
	break;
	case BS_PARALLE:
		/*
		int RES = 1; //reset HIGH – inactive
		delayms(1); //delay
		command(0x2A); //function set (extended command set)
		command(0x71); //function selection A
		data(0x00); // disable internal VDD regulator (2.8V I/O). data(0x5C) = enable regulator (5V I/O)
		command(0x28); //function set (fundamental command set)
		command(0x08); //display off, cursor off, blink off
		command(0x2A); //function set (extended command set)
		command(0x79); //OLED command set enabled
		command(0xD5); //set display clock divide ratio/oscillator frequency
		command(0x70); //set display clock divide ratio/oscillator frequency
		command(0x78); //OLED command set disabled
		command(0x09); //extended function set (4-lines)
		command(0x06); //COM SEG direction
		command(0x72); //function selection B
		data(0x00); //ROM CGRAM selection
		command(0x2A); //function set (extended command set)
		command(0x79); //OLED command set enabled
		command(0xDA); //set SEG pins hardware configuration
		command(0x10); //set SEG pins hardware configuration
		command(0xDC); //function selection C
		command(0x00); //function selection C
		command(0x81); //set contrast control
		command(0x7F); //set contrast control
		command(0xD9); //set phase length
		command(0xF1); //set phase length
		command(0xDB); //set VCOMH deselect level
		command(0x40); //set VCOMH deselect level
		command(0x78); //OLED command set disabled
		command(0x28); //function set (fundamental command set)
		command(0x01); //clear display
		command(0x80); //set DDRAM address to 0x00
		command(0x0C); //display ON
		delayms(100); //delay*/
	break;
	default:
	break;
	}
}


















void delay(unsigned int n)				//Delay subroutine
{
	unsigned int i,j;
	for (i=0;i<n;i++)
  		for (j=0;j<250;j++)
  			{;}
}


int open_led_spi()
{
	int ret = 0;//程序运行状态指示
	static uint8_t mode=3;//spi模式
	static uint8_t lsb=0x08;//spi模式
	static uint8_t bits = 8;//传输数据位数
	static uint32_t speed = 1000000;//时钟频率
	static uint16_t delay=10;//发送延迟
	//parse_opts(argc, argv);
        //printf("parse_opts(argc, argv);");
	fd_led = open(device, O_RDWR);//打开spi设备
	if (fd_led < 0)
		perror("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd_led, SPI_IOC_WR_MODE, &mode);//设置spi写模式
	if (ret == -1)
		perror("can't set spi mode");
	//printf("the mode =%d\r\n",mode);
	ret = ioctl(fd_led, SPI_IOC_RD_MODE, &mode);//设置spi读模式
	//printf("after ioctl(fd, SPI_IOC_RD_MODE, &mode) the mode =%d\r\n",mode);
	if (ret == -1)
		perror("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd_led, SPI_IOC_WR_BITS_PER_WORD, &bits);//设置spi发送位数
	if (ret == -1)
		perror("can't set bits per word");

	ret = ioctl(fd_led, SPI_IOC_RD_BITS_PER_WORD, &bits);//设置spi接收位数
	if (ret == -1)
		perror("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd_led, SPI_IOC_WR_MAX_SPEED_HZ, &speed);//设置spi发送时钟频率
	if (ret == -1)
		perror("can't set max speed hz");

	ret = ioctl(fd_led, SPI_IOC_RD_MAX_SPEED_HZ, &speed);//设置spi接收时钟频率
	if (ret == -1)
		perror("can't get max speed hz");

/*
	ret = ioctl(fd_led, SPI_IOC_WR_LSB_FIRST, &lsb);//设置spi发送时钟频率
	if (ret == -1)
		perror("can't set lsb");

	ret = ioctl(fd_led, SPI_IOC_WR_LSB_FIRST, &lsb);//设置spi接收时钟频率
	if (ret == -1)
		perror("can't get lsb");*/


	















	//printf("spi mode: %d\n", mode);
	//printf("bits per word: %d\n", bits);
	//printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	//char * rx_buff=malloc(buff_size);
	//free(rx_buff);
	return 0;

}
#define SENDCW 1
#define SENDALONE 2

void disp_on_oled(int dispmethod,char * displaybuffer)
{
	int i;

	if(dispmethod==SENDALONE){
		for(i=0;i<80;i++)
		{
		usleep(100000);
		data_spi(displaybuffer[i]); //write solid blocks
		}
	}
	else if(dispmethod==SENDCW){
		data_spi_continue(displaybuffer,80);
	}
}

int initoled_disp()//初始化显示内容
{
	;
}



char dispbuff_in[20]={0};


int showdisp(char * start)
{
int ifor,jfor;
        printf("\033[1A"); //先回到上一行  
        //printf("\033[K");  //清除该行  */
        printf("\033[1A"); //先回到上一行  
        //printf("\033[K");  //清除该行  */
        printf("\033[1A"); //先回到上一行  
       // printf("\033[K");  //清除该行  */
        printf("\033[1A"); //先回到上一行  
        //printf("\033[K");  //清除该行  */
        printf("\033[1A"); //先回到上一行  
        //printf("\033[K");  //清除该行  */
        printf("\033[1A"); //先回到上一行  
	//printf("\n");
        printf("\033[K");  //清除该行  */
		printf("______________________\n");
for(ifor=0;ifor<4;ifor++)
{
        printf("\033[K");  //清除该行  */
		printf("|");
		for(jfor=0;jfor<20;jfor++)
		{
		printf("%c",start[(ifor)*20+jfor]);
		}
		printf("|");
		printf("\n");
}
        printf("\033[K");  //清除该行  */
	printf("----------------------\n");

}






void init_oled(int bs)//spi统一接口方式初始化
{
int com_seg_direct=0;
switch(bs)
	{
	case BS_SPI:


/*
	command_spi(0x38);//function set
	command_spi(0x06);//entry mode set
	command_spi(0x02);//return home
	command_spi(0x01);//clear display
	command_spi(0x0c);//display on
	command_spi(0x80);//line 1 character 1

*/

		usleep(1000); //delay
		//command_spi(0x07); //打开移动
		//command_spi(0x0f); //function set (extended command set)
		command_spi(0x2A); //function set (extended command set)
		//command_spi(0x81); //垂向滑动
		//command_spi(0x12); 
		//command_spi(0x11); 
		//command_spi(0x18); //function set (extended command set)
		command_spi(0x71); //function selection A
		data_spi(0x00); // disable internal VDD regulator (2.8V I/O). data(0x5C) = enable regulator (5V I/O)
		command_spi(0x28); //function set (fundamental command set)
		command_spi(0x08); //display off, cursor off, blink off

		
		command_spi(0x2A); //function set (extended command set)
		//command_spi(0x0B); 
		command_spi(0x79); //OLED command set enabled
		command_spi(0xD5); //set display clock divide ratio/oscillator frequency
		command_spi(0x70); //set display clock divide ratio/oscillator frequency
		command_spi(0x78); //OLED command set disabled
		command_spi(0x09); //extended function set (4-lines)
		if(com_seg_direct==0)
			command_spi(0x06); //COM SEG direction
		else
			command_spi(0x05); //COM SEG direction	
		command_spi(0x72); //function selection B
		data_spi(0x00); //ROM CGRAM selection

		command_spi(0x2A); //function set (extended command set)

		//command_spi(0x2B); //reverse display反相显示效果不好
		command_spi(0x79); //OLED command set enabled


//新加的初始化内容=>
		command_spi(0xDA); //set SEG pins hardware configuration1
		command_spi(0x10); //set SEG pins hardware configuration1
		command_spi(0xDC); //function selection C
		command_spi(0x00); //function selection C
		command_spi(0x81); //set contrast control1
		command_spi(0x7F); //set contrast control1
		command_spi(0xD9); //set phase length1
		command_spi(0xF1); //set phase length1
		command_spi(0xDB); //set VCOMH deselect level1
		command_spi(0x40); //set VCOMH deselect level1
//新加的初始化内容=<
		command_spi(0x78); //OLED command set disabled
		command_spi(0x28); //function set (fundamental command set)

		command_spi(0x01); //clear display
		command_spi(0x80); //set DDRAM address to 0x00
		command_spi(0x0C); //display ON
		usleep(100*1000); //delay
		printf("--------------------------------------------------------------------------------------------------------------------");
	break;
	case BS_IIC:
	break;
	case BS_PARALLE:
		/*
		int RES = 1; //reset HIGH – inactive
		delayms(1); //delay
		command(0x2A); //function set (extended command set)
		command(0x71); //function selection A
		data(0x00); // disable internal VDD regulator (2.8V I/O). data(0x5C) = enable regulator (5V I/O)
		command(0x28); //function set (fundamental command set)
		command(0x08); //display off, cursor off, blink off
		command(0x2A); //function set (extended command set)
		command(0x79); //OLED command set enabled
		command(0xD5); //set display clock divide ratio/oscillator frequency
		command(0x70); //set display clock divide ratio/oscillator frequency
		command(0x78); //OLED command set disabled
		command(0x09); //extended function set (4-lines)
		command(0x06); //COM SEG direction
		command(0x72); //function selection B
		data(0x00); //ROM CGRAM selection
		command(0x2A); //function set (extended command set)
		command(0x79); //OLED command set enabled
		command(0xDA); //set SEG pins hardware configuration
		command(0x10); //set SEG pins hardware configuration
		command(0xDC); //function selection C
		command(0x00); //function selection C
		command(0x81); //set contrast control
		command(0x7F); //set contrast control
		command(0xD9); //set phase length
		command(0xF1); //set phase length
		command(0xDB); //set VCOMH deselect level
		command(0x40); //set VCOMH deselect level
		command(0x78); //OLED command set disabled
		command(0x28); //function set (fundamental command set)
		command(0x01); //clear display
		command(0x80); //set DDRAM address to 0x00
		command(0x0C); //display ON
		delayms(100); //delay*/
	break;
	default:
	break;
	}
}






#define WAITIMESTART 13
int app_display_init(char * device, SPICONF_S * config)
{

	initoled_disp();
	char tmpstr[20];
	int inttmp;
	int ifor;
	int oled_fid;
	oled_fid=open_spi(device,config);
	fd_led=oled_fid;
	printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
	init_oled(1);//spi统一接口方式初始化
	printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
	//openltc2943();
	//initoled(0);
	//disp_on_oled(SENDCW);//一般以连续模式发送字符
	return 0;
}

int tcp_oled_callback ( int size,char * buf,void * buff )
{
	char distr[80];
    snprintf(distr,80,"hello client %d this is epoll's world",size);
    disp_on_oled(1,distr);
    pthread_mutex_lock(&app.app_sync.mut[MUTMAIN]);
	//pthread_cond_broadcast(&app.app_sync.cond[MUTMAIN], &app.app_sync.mut[MUTMAIN]);
	pthread_cond_broadcast(&app.app_sync.cond[MUTMAIN]);
	pthread_mutex_unlock(&app.app_sync.mut[MUTMAIN]);
    return 0;

}