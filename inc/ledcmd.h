/*
 * zwpgpio.h
 *
 * Copyright (C) zhangwp People
 *
 * Serial Synchronous Controller (OSO) registers.
 * Based on AT91RM9200 datasheet revision E.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */



#define GPIO_IOC_MAGIC			'I'
#define __u8 unsigned char 
#define __u32 unsigned int
#define __u16 unsigned short
#define __u64 unsigned long long



/* 控制SSC采集指示灯的亮跟灭*/
#define GPIO_IOC_N_SSC_LED0_ON			_IO(GPIO_IOC_MAGIC, 1)
#define GPIO_IOC_N_SSC_LED0_OFF			_IO(GPIO_IOC_MAGIC, 2)

#define GPIO_IOC_N_GPS_LED2_ON			_IO(GPIO_IOC_MAGIC, 3)
#define GPIO_IOC_N_GPS_LED2_OFF			_IO(GPIO_IOC_MAGIC, 4)

#define GPIO_IOC_N_SD_LED1_ON			_IO(GPIO_IOC_MAGIC, 5)
#define GPIO_IOC_N_SD_LED1_OFF			_IO(GPIO_IOC_MAGIC, 6)

#define GPIO_IOC_N_SYNC_LED3_ON			_IO(GPIO_IOC_MAGIC, 7)
#define GPIO_IOC_N_SYNC_LED3_OFF			_IO(GPIO_IOC_MAGIC, 8)


#define GPIO_IOC_N_NET_LED3_BLANK_WITHDATA			_IO(GPIO_IOC_MAGIC, 16)





#define GPIO_IOC_N_NET_LED3_USYNC_ON			_IO(GPIO_IOC_MAGIC, 9)//异步
#define GPIO_IOC_N_NET_LED3_USYNC_OFF			_IO(GPIO_IOC_MAGIC, 10)


#define GPIO_IOC_N_LED_BLINK_ALL 			_IO(GPIO_IOC_MAGIC, 41)//全部同时闪烁,提示故障



#define GPIO_IOC_N_LED_CLOSE_ALL 			_IO(GPIO_IOC_MAGIC, 101)

#define GPIO_IOC_N_GPIO_UP 			_IO(GPIO_IOC_MAGIC, 200)
#define GPIO_IOC_N_GPIO_DOWN			_IO(GPIO_IOC_MAGIC, 201)
#define GPIO_IOC_N_GPIO_UP_ANYWAY			_IO(GPIO_IOC_MAGIC, 203)
#define GPIO_IOC_N_GPIO_DOWN_ANYWAY		_IO(GPIO_IOC_MAGIC, 204)
#define GPIO_IOC_N_LED3_BEAT 			_IO(GPIO_IOC_MAGIC, 31)
#define GPIO_IOC_N_LED2_ERR 			_IO(GPIO_IOC_MAGIC, 21)

#define GPIO_IOC_N_TASK_TEST 			_IO(GPIO_IOC_MAGIC, 150)

#define GPIO_IOC_N_OLED_SETBS 			_IO(GPIO_IOC_MAGIC, 160)//设置oled的接口模式

#define GPIO_IOC_N_FPGA_M_SWITCH 			_IO(GPIO_IOC_MAGIC, 170)//设置FPGA m序列发送控制

#define GPIO_IOC_N_IC2_TEST 650

#define GPIO_FPGA 0   
#define GPIO_LED_CLOSE_ALL 10
#define GPIO_IO_OUT 100   
#define GPIO_IO_OUT_S 101   
//#define GPIO_IO_OUT_BEAT 102
#define GPIO_IO_OUT_FILE 103
#define GPIO_IOCTL 2 
#define GPIO_IO_RW  200
#define GPIO_IO_RW_1  201
#define GPIO_INIT_LED_OFF 300
#define GPIO_INIT_LED_ON 301
#define GPIO_HIGH 1   
#define GPIO_LOW 0   
#define GPIO_SSC_TEST 1000
#define GPIO_SSC_TEST_1 1001

#define GPIO_PWM_CH0 2000



#define GPIO_SPI_TEST 3000
#define GPIO_CTL_TEST 4000



#define GPIO_IO_SSC_ON 401   
#define GPIO_IO_SSC_OFF 400   

#define GPIO_IO_TCP_ON 501   
#define GPIO_IO_TCP_OFF 500   

#define GPIO_IO_TCP_CTL_ON 601
#define GPIO_IO_TCP_CTL_OFF 600

#define GPIO_CLK_EN 10000







