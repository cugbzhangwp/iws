/*
 *  fpga config and get info
 *
 *
 *  Copyright (C) 2017 orangelamp.
 *	121996025@qq.com(zwp@orangelamp.com.cn)
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
//#include <oso_common.h>

//#include <stdlib.h>
//#include <stdint.h>
//#include <sys/sysinfo.h>
//#include <sys/sysinfo.h>
//#include <sys/vfs.h>
//#include <string.h>
//#include <sys/vfs.h>
//#include <linux/jiffies.h>
//#include <errno.h>
//#include <ifaddrs.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <stdio.h>
#include <fcntl.h>//文件打开属性
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
//#include <linux/spi/spidev.h>

//#include <string.h>

//#include <errno.h>
//#include <time.h> //C语言的头文件 
//#include <linux/rtc.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <signal.h>
#include <locale.h>
//#include <linux/spi/spi.h>  // 在头文件处添加两个头文件  
#include <spidev.h>
#include <spi_rw.h>
#include <printcolor.h>

void transfer(int fd,uint8_t * tx_buff,uint8_t * rx_buff,int buff_size);
SPICONF_S spidefaultconf = {
  .mode=3,//spi模式
  .lsb=0x08,//spi模式
  .bits = 8,//传输数据位数
  .speed = 1000000,//时钟频率
  .delay=10,//发送延迟
};

int open_spi(char * device,SPICONF_S * config)
{
	printf(RED"inside %s\n"NONE, __FUNCTION__);
	if(config==NULL)
		config=&spidefaultconf;//如果用戶沒給配置內容則用默認配置
	int ret = 0;
	int fd;
	char filetotran[20];
	uint8_t mode = config->mode;
	 uint8_t bits = config->bits;
	 uint32_t speed = config->speed;
	 uint16_t delay = config->delay;
	//parse_opts(argc, argv);
        //printf("parse_opts(argc, argv);");
	 printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
	fd = open(device, O_RDWR);
	if (fd < 0)
		perror("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		perror("can't set spi mode");
	//printf("the mode =%d\r\n",mode);
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	//printf("after ioctl(fd, SPI_IOC_RD_MODE, &mode) the mode =%d\r\n",mode);
	if (ret == -1)
		perror("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		perror("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		perror("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		perror("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		perror("can't get max speed hz");
printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
	//printf("spi mode: %d\n", mode);
	//printf("bits per word: %d\n", bits);
	//printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	//char * rx_buff=malloc(buff_size);
	//free(rx_buff);
	printf(RED"inside %s\n"NONE, __FUNCTION__);
	return fd;

}


int send_to_spi(int fid_spi,uint8_t * tx_buff,uint8_t * rx_buff ,int buff_size)
{
	transfer(fid_spi,tx_buff,rx_buff,buff_size);
	return 0;
}





inline void transfer(int fd,uint8_t * tx_buff,uint8_t * rx_buff,int buff_size)
{
	int ret,txlen,sendlen;
	//FILE *fid,*fid_in; 
	//printf("dev %s\r\n",device);
	static uint16_t delay=0;
	//fid=fopen(filetotran,"r");
	//fid_in=fopen("recv.txt","w+");
	//while(!feof(fid))  
	//{	
	//uint8_t tx[BUFSIZE];
	//	char tx[1024];
	//printf("in while\r\n");
	//txlen=fread(tx,1,BUFSIZE,fid);
	/*if(txlen=1)
	sendlen=BUFSIZE;
	else
	sendlen=txlen;*/
	//fgets(tx,100,fid);
	/* {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
		0xF0, 0x0D,
	};*/
	//printf("txlen=%u\r\n",txlen);
	//printf("tx=%s\r\n",tx);
	//txlen=strlen(tx);
	//uint8_t rx[buff_size];
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx_buff,
		.rx_buf = (unsigned long)rx_buff,
		.len =buff_size,
		.delay_usecs = delay,
		.speed_hz = 0,
		.bits_per_word = 0,
	};

	//if (write(fd, tx, ARRAY_SIZE(tx)) != ARRAY_SIZE(tx))
		//;//perror("Write Error");
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	//printf("ret ==========%d\r\n",ret);
	//printf("speed_hz ==========%d\r\n",tr.speed_hz);
	//printf("bits_per_word ==========%d\r\n",tr.bits_per_word);
	if (ret == 1)
	;
		//pabort("can't send spi message");
/*
	for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
		if (!(ret % 6))
			puts("");
		printf("%.2X ", rx[ret]);
		
	}*/
	//fwrite(rx,txlen,1,fid_in);
	//fputs(rx,fid_in);
	//puts("");
	//}
	//fclose(fid);
	//fclose(fid_in);
}
