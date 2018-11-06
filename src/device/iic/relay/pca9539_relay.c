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
#include <app_main.h>
#include <pca9539_relay.h>





#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
int relay_file_id;
int smbus_write(unsigned char command,unsigned char * value,int length)
{
    union i2c_smbus_data smdata;
    struct i2c_smbus_ioctl_data msgset;
    smdata.block[0]=value[0];
    smdata.block[1]=value[1];
    smdata.block[2]=0x0;
    //msgset.
    //
    int ifor=0;
    //printf("sizeof smdata=%d\n",sizeof(smdata));

    msgset.read_write=I2C_WRITE_BIT;
    msgset.command=command;
    msgset.size=length;//I2C_SMBUS_WORD_DATA;
    msgset.data=&smdata;
    unsigned long funcs;
    //printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    int addr = RELAY_ADDR; 
    if (ioctl(relay_file_id, I2C_SLAVE_FORCE, addr) < 0) {
        printf("err happen in ioctl!");
    }
    usleep(1000);
    //printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    if (ioctl(relay_file_id, I2C_SMBUS,(struct i2c_smbus_ioctl_data *)&msgset) < 0) {
        printf("err happen in ioctl!  I2C_SMBUS\n");
        printf("errno=%d\n",errno);
        //exit(1);
    }
    //printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    if(msgset.read_write==I2C_READ_BIT){
        for(ifor=0;ifor<I2C_SMBUS_WORD_DATA;ifor++)
        {
            ;//printf("read data from 2943 is %x\n",smdata.block[ifor]);

        }
    }
    return 0;
}





int smbus_read(unsigned char command,unsigned char * value,int length)
{
    union i2c_smbus_data smdata;
    struct i2c_smbus_ioctl_data msgset;
    smdata.block[0]=0x0;
    smdata.block[1]=0x1;
    smdata.block[2]=0x0;
    int ifor=0;
    msgset.read_write=I2C_READ_BIT;
    msgset.command=command;
    msgset.size=length;//I2C_SMBUS_WORD_DATA;
    msgset.data=&smdata;
    unsigned long funcs;
    int addr = 0x64; 
    if (ioctl(relay_file_id, I2C_SLAVE_FORCE, addr) < 0) {
        printf("err happen in ioctl!");
    }
    if (ioctl(relay_file_id, I2C_SMBUS,(struct i2c_smbus_ioctl_data *)&msgset) < 0) {
        printf("err happen in ioctl!  I2C_SMBUS\n");
        printf("errno=%d\n",errno);
    }
    if(length==I2C_SMBUS_BYTE_DATA){
        value[0]=smdata.block[0];
    }
    else if(length==I2C_SMBUS_WORD_DATA){
        value[0]=smdata.block[1];
        //printf("value 0=%x\n",value[0]);
        value[1]=smdata.block[0];
        //printf("value 1=%x\n",value[1]);
    }
    return 0;
}




int set_pca9539_output()
{
printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    unsigned char value[2];
    value[0]=0;
    value[1]=0;
    smbus_write(DERECT_REG_P0,value,I2C_SMBUS_WORD_DATA);
    printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    //i2c_smbus_write_i2c_block_data(int file, __u8 command, __u8 length, __u8 *values);
    return 0;
}

int set_pca9539(unsigned char * value)
{
    smbus_write(OUTPUT_REG_P0,value,I2C_SMBUS_WORD_DATA);
    return 0;
}


int pca9539_pin_init()
{
    if(app.app_sign.has_bcm2835_init==0){
        wiring_bcm2835_init();
    }
    //bcm2835_gpio_fsel(RST, BCM2835_GPIO_FSEL_OUTP);//
    bcm2835_gpio_fsel(RELAY, BCM2835_GPIO_FSEL_OUTP);//
    //bcm2835_gpio_fsel(SENSOR, BCM2835_GPIO_FSEL_OUTP);//
    RELAY_EN();
    SENSOR_EN();
    // //bsp_DelayUS(10*1000);
    // bcm2835_delay(10);
    // RST_1();
    // bcm2835_delay(10);
    // //printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    // RST_0();
    // bcm2835_delay(10);
    // RST_1();
    return 0;
}



int set_pca9539_direct()
{
    piLock(1);
    unsigned char value[3];
    value[0]=6;//寄存器6
    value[1]=0;//p0，p1方向为输出
    value[2]=0;
    set_addr(RELAY_ADDR);
    I2C_WritenBytes(value,3);
    piUnlock(1);
    return 0;
}

int set_pca9539_value(char * buf)
{
    piLock(1);
    unsigned char value[3];
    value[0]=OUTPUT_REG_P0;//寄存器6
    value[1]=buf[0];//p0，p1方向为输出
    value[2]=buf[1];
    set_addr(RELAY_ADDR);
    I2C_WritenBytes(value,3);
    piUnlock(1);
    return 0;
}


 





int bcm_open_pca9539(APP_S * app)
{

    set_pca9539_direct();
    usleep(1000);
    char value[2];
    value[0]=0x00;//
    value[1]=0x00;//
    // value[0]=0xff;//ok
    // value[1]=0x0f;//ok
    set_pca9539_value(value);
    value[0]=0xff;//
    value[1]=0xff;//
    // value[0]=0xff;//ok
    // value[1]=0x0f;//ok
    set_pca9539_value(value);
    value[0]=0xff;//
    value[1]=0x0f;//
    // value[0]=0xff;//ok
    // value[1]=0x0f;//ok
    set_pca9539_value(value);
    // while(1)
    // {
    //     value[0]=0x00;
    //     value[1]=0x00;
    //     set_pca9539_value(value);
    //     usleep(1000*1000);
    //     value[0]=0xff;
    //     value[1]=0xff;
    //     set_pca9539_value(value);
    //     usleep(1000*1000);
    //     //exit(0);
    // }
    return 0;
}


int p0p1_init(char * p0p1)
{
    #define BEEPBIT 4//1不响
    #define SW0BIT 5//输入范围切换
    #define LED1BIT 8//1不亮
    #define LED2BIT 9
    #define LED3BIT 10
    #define STBIT 11//1使能
    #define RELAYCH3BIT 12//0吸合
    #define RELAYCH2BIT 13
    #define RELAYCH1BIT 14
    #define RELAYCH0BIT 15
    p0p1[0]=0x00;
    p0p1[1]=0x00;
    return 0;
}


int open_pca9539(APP_S * app)
{
    pca9539_pin_init();
    p0p1_init(app->pca9539);
    int ret;
    char batteryfilename[20];
    //beep_init();
    printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    snprintf(batteryfilename, 19, "/dev/i2c-1");
    relay_file_id= open(batteryfilename, O_RDWR);
    if (relay_file_id < 0) {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        printf("err happen!");
        exit(1);
    }

/*
    snprintf(batteryfilename, 19, "/dev/i2c-1");
    bq20z95 = open(batteryfilename, O_RDWR);
    if (bq20z95 < 0) {
        // ERROR HANDLING; you can check errno to see what went wrong 
        printf("err happen!");
        //exit(1);
    }
*/





    int addr = RELAY_ADDR; 
    char data[4]={0,1,2,3};
    if (ioctl(relay_file_id, I2C_SLAVE_FORCE, addr) < 0) {
        printf("err happen in ioctl!");
    }
    printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    unsigned char value[2];
    //unsigned short voltage;
    //smbus_write(DERECT_REG_P0,value,I2C_SMBUS_BYTE_DATA);
    sleep(1);
    set_pca9539_output();
    usleep(1000);
    set_pca9539(app->pca9539);
    // while(1)
    // {
    //     value[0]=0x00;
    //     value[1]=0x00;
    //     set_pca9539(value);
    //     sleep(1);
    //     value[0]=0xff;
    //     value[1]=0xff;
    //     set_pca9539(value);
    //     sleep(1);
    //     //exit(0);
    // }
    printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
/*
    int addr = 0xb; 
    if (ioctl(bq20z95, I2C_SLAVE_FORCE, addr) < 0) {
        printf("err happen in ioctl!");
        exit(1);
    }*/
    //bq20z95_read(LTC2943_CONTROL_REG,value,I2C_SMBUS_BYTE_DATA);
    return ret;
}


