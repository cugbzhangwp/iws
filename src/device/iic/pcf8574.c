#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <bcm2835.h>


#include <pcf8574.h>
#include <i2crw.h>
#include <app_main.h>

#define PCFADDR 0x38
int reset_device(int fid,char * port,int device_port_num,int t)
{	
	//smbus_write_nbytes(int fid,int addr,unsigned char command,unsigned char * value,int length);
}




int pcf8574_pin_init(APP_S * app)
{
    if(app->app_sign.has_bcm2835_init==0){
        wiring_bcm2835_init();
    }
    return 0;
}

int pcf8574_file_id;





int write_pcf8574(char * buf,int len)
{
    
    I2C_WritenBytes(buf,len);
    return 0;
}




int reset_devices()
{
    //piLock(1);
    set_addr(PCFADDR);
    char value[2];
    value[0]=0xff;
    write_pcf8574(value,1);
    usleep(1000*100);
    value[0]=0x00;
    write_pcf8574(value,1);
    usleep(1000*100);
    value[0]=0xff;
    write_pcf8574(value,1);
    //piUnlock(1);
    return 0;
}

int open_pcf8574(APP_S * app)
{
    pcf8574_pin_init(app);
    int ret;
    char batteryfilename[20];
    //beep_init();
    printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    snprintf(batteryfilename, 19, "/dev/i2c-1");
    pcf8574_file_id= open(batteryfilename, O_RDWR);
    if (pcf8574_file_id < 0) {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        printf("err happen!");
        exit(1);
    }
    int addr = PCFADDR; 
    char data[4]={0,1,2,3};
    if (ioctl(pcf8574_file_id, I2C_SLAVE_FORCE, addr) < 0) {
        printf("err happen in ioctl!");
    }
    printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    unsigned char value[2];
    //unsigned short voltage;
    //smbus_write(DERECT_REG_P0,value,I2C_SMBUS_BYTE_DATA);
	smbus_write_nbytes(pcf8574_file_id,addr,0xff,value,1);
    //smbus_write_nbytes(batteryfile,addr,LTC2943_CONTROL_REG,value,I2C_SMBUS_BYTE_DATA);
	//sleep(1);
	smbus_write_nbytes(pcf8574_file_id,addr,0x10,value,1);
	usleep(1000);
	smbus_write_nbytes(pcf8574_file_id,addr,0xff,value,1);
     printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    return ret;
}







/*
[html] view plaincopy
 
//ISL1208.h  
#ifndef _ISL1208_H_  
#define _ISL1208_H_  
#include "stm32f10x.h"  
  
typedef struct My_Time  
{  
    u8 _year;  
    u8 _month;  
    u8 _day;  
    u8 _hour;  
    u8 _min;  
    u8 _sec;  
    u8 _week;  
} MY_TIME,*pMY_TIME;  
  
extern MY_TIME sMyTime;  
void ISL128Init(void);  
void SetTime(pMY_TIME _mytime);  
void GetTime(pMY_TIME _mytime);  
void Auto_Time_Set(void);  
#endif //_ISL1208_H_  
 
 
[html] view plaincopy
 
//ISL128.C  
#include "isl1208.h"  
#include "i2c_ee.h"  
  
const u8 *COMPILED_DATE=__DATE__;//获得编译日期  
const u8 *COMPILED_TIME=__TIME__;//获得编译时间  
const u8 Month_Tab[12][3]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};   
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表   
  
MY_TIME sMyTime={0x13,0x04,0x13,0x14,0x10,0x05,0x03};  
static int IntToBCD(int i);//十进制转BCD  
static int BCDToInt(int bcd); //BCD转十进制  
static u8 RTC_Get_Week(u16 year,u8 month,u8 day);  
  
int IntToBCD(int i) //十进制转BCD  
{  
    return (((i / 10) << 4) + ((i % 10) & 0x0f));  
}  
  
int BCDToInt(int bcd) //BCD转十进制  
{  
    return (0xff & (bcd >> 4)) * 10 + (0xf & bcd);  
}  
  
//比较两个字符串指定长度的内容是否相等  
//参数:s1,s2要比较的两个字符串;len,比较长度  
//返回值:1,相等;0,不相等  
u8 my_strcmp(u8*s1,u8*s2,u8 len)  
{  
    u8 i;  
    for(i=0;i
    return 1;        
}  
  
void ISL128Init()  
{  
    u8 SR_REG_DATA=0x91;  
    u8 INT_REG_DATA=0xca;  
    EEPROM_ADDRESS=ISL1208_ADDRESS;  
    I2C_EE_ByteWrite(&SR_REG_DATA, 0x07);  
    I2C_EE_ByteWrite(&INT_REG_DATA, 0x08);  
}  
void SetTime(pMY_TIME _mytime)  
{  
    EEPROM_ADDRESS=ISL1208_ADDRESS;  
    I2C_EE_ByteWrite(&_mytime->_sec,  0x00);  
    I2C_EE_ByteWrite(&_mytime->_min,  0x01);  
    I2C_EE_ByteWrite(&_mytime->_hour, 0x02);  
    I2C_EE_ByteWrite(&_mytime->_day,  0x03);  
    I2C_EE_ByteWrite(&_mytime->_month,0x04);  
    I2C_EE_ByteWrite(&_mytime->_year, 0x05);  
    I2C_EE_ByteWrite(&_mytime->_week, 0x06);  
}  
void GetTime(pMY_TIME _mytime)  
{  
    EEPROM_ADDRESS=ISL1208_ADDRESS;  
    I2C_EE_BufferRead(&_mytime->_sec,  0x00,1);  
    I2C_EE_BufferRead(&_mytime->_min,  0x01,1);  
    I2C_EE_BufferRead(&_mytime->_hour, 0x02,1);  
    I2C_EE_BufferRead(&_mytime->_day,  0x03,1);  
    I2C_EE_BufferRead(&_mytime->_month,0x04,1);  
    I2C_EE_BufferRead(&_mytime->_year, 0x05,1);  
    I2C_EE_BufferRead(&_mytime->_week, 0x06,1);  
      
    _mytime->_sec=BCDToInt(_mytime->_sec);  
    _mytime->_min=BCDToInt(_mytime->_min);  
    _mytime->_hour=BCDToInt(_mytime->_hour);  
    _mytime->_day=BCDToInt(_mytime->_day);  
    _mytime->_month=BCDToInt(_mytime->_month);  
    _mytime->_year=BCDToInt(_mytime->_year);  
    _mytime->_week=BCDToInt(_mytime->_week);  
}  
  
void Auto_Time_Set()  
{  
    u8 temp[3];  
    u8 i;  
    u8 mon,date,week;  
    u16 year;  
    u8 sec,min,hour;  
    for(i=0;i<3;i++)temp[i]=COMPILED_DATE[i];     
    for(i=0;i<12;i++)if(my_strcmp((u8*)Month_Tab[i],temp,3))break;     
    mon=i+1;//得到月份  
    if(COMPILED_DATE[4]==' ')date=COMPILED_DATE[5]-'0';   
    else date=10*(COMPILED_DATE[4]-'0')+COMPILED_DATE[5]-'0';    
    year=10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';        
    hour=10*(COMPILED_TIME[0]-'0')+COMPILED_TIME[1]-'0';    
    min=10*(COMPILED_TIME[3]-'0')+COMPILED_TIME[4]-'0';    
    sec=10*(COMPILED_TIME[6]-'0')+COMPILED_TIME[7]-'0';    
    week=RTC_Get_Week(year+2000,mon,date);  
    sMyTime._day=IntToBCD(date);  
    sMyTime._hour=IntToBCD(hour);  
    sMyTime._min=IntToBCD(min);  
    sMyTime._month=IntToBCD(mon);  
    sMyTime._sec=IntToBCD(sec);  
    sMyTime._year=IntToBCD(year);  
    sMyTime._week=IntToBCD(week);  
  
    SetTime(&sMyTime);  
}   
  
  
u8 RTC_Get_Week(u16 year,u8 month,u8 day)  
{     
    u16 temp2;  
    u8 yearH,yearL;  
      
    yearH=year/100; yearL=year%100;   
    // 如果为21世纪,年份数加100    
    if (yearH>19)yearL+=100;  
    // 所过闰年数只算1900年之后的    
    temp2=yearL+yearL/4;  
    temp2=temp2%7;   
    temp2=temp2+day+table_week[month-1];  
    if (yearL%4==0&&month<3)temp2--;  
    return(temp2%7);  
}   


*/












