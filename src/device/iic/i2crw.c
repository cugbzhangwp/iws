#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <i2crw.h>
#include <errno.h>
#include <bcm2835.h>
#include <printcolor.h>


void I2C_WritenBytes(char * buf,int len)
{
    int ref;
    ref = bcm2835_i2c_write(buf, len);
    while(ref != 0) {
        ref = bcm2835_i2c_write(buf, len);
        if(ref == 0)
            break;
    }   
}

void I2C_ReadnBytes(char * buf,int len)
{
    int ref;
    ref = bcm2835_i2c_read(buf, len);
    while(ref != 0) {
        ref = bcm2835_i2c_read(buf, len);
        if(ref == 0)
            break;
    }   
}















int iic_write_nbytes(int fid,int addr,unsigned char command,unsigned char * value,int length)
{
    union i2c_smbus_data smdata;
    struct i2c_smbus_ioctl_data msgset;
    int ifor;
    for(ifor=0;ifor<length;ifor++)
    {
        smdata.block[ifor]=value[ifor]; 
    }
    msgset.read_write=I2C_WRITE_BIT;
    msgset.command=command;
    msgset.size=length;//I2C_SMBUS_WORD_DATA;
    msgset.data=&smdata;
    unsigned long funcs;
    printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    //int addr = RELAY_ADDR; 
    if (ioctl(fid, I2C_SLAVE_FORCE, addr) < 0) {
        printf("err happen in ioctl!");
    }
    usleep(1000);
    printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    if (ioctl(fid, I2C_SMBUS,(struct i2c_smbus_ioctl_data *)&msgset) < 0) {
        printf("err happen in ioctl!  I2C_SMBUS\n");
        printf("errno=%d\n",errno);
        //exit(1);
    }
    return 0;
}




int ioctl_write_nbytes(int fid,int addr,unsigned char command,unsigned char * value,int length)
{
// /* This is the structure as used in the I2C_RDWR ioctl call */
// struct i2c_rdwr_ioctl_data {
//     struct i2c_msg __user *msgs;    /* pointers to i2c_msgs */
//     __u32 nmsgs;            /* number of i2c_msgs */
// };

//     /*
// struct i2c_msg {
//     __u16 addr; /* slave address            */
//     __u16 flags;
// #define I2C_M_TEN       0x0010  /* this is a ten bit chip address */
// #define I2C_M_RD        0x0001  /* read data, from slave to master */
// #define I2C_M_NOSTART       0x4000  /* if I2C_FUNC_PROTOCOL_MANGLING */
// #define I2C_M_REV_DIR_ADDR  0x2000   if I2C_FUNC_PROTOCOL_MANGLING 
// #define I2C_M_IGNORE_NAK    0x1000  /* if I2C_FUNC_PROTOCOL_MANGLING */
// #define I2C_M_NO_RD_ACK     0x0800  /* if I2C_FUNC_PROTOCOL_MANGLING */
// #define I2C_M_RECV_LEN      0x0400  /* length will be first received byte */
//     __u16 len;      /* msg length               */
//     __u8 *buf;      /* pointer to msg data          */
// };
// */
    struct i2c_rdwr_ioctl_data ioctl_data;
    int ilen;
    char * pbuf=value;
    for(ilen=length;ilen>0;ilen=ilen-34){
        iic_write_nbytes(fid,addr,command,pbuf,ilen);
        pbuf=pbuf+34;
    }
    return 0;
}










int smbus_write_nbytes(int fid,int addr,unsigned char command,unsigned char * value,int length)
{
    union i2c_smbus_data smdata;
    struct i2c_smbus_ioctl_data msgset;
    int ifor;
    for(ifor=0;ifor<length;ifor++)
    {
        smdata.block[ifor]=value[ifor]; 
    }
    msgset.read_write=I2C_WRITE_BIT;
    msgset.command=command;
    msgset.size=length;//I2C_SMBUS_WORD_DATA;
    msgset.data=&smdata;
    unsigned long funcs;
    printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    //int addr = RELAY_ADDR; 
    if (ioctl(fid, I2C_SLAVE_FORCE, addr) < 0) {
        printf("err happen in ioctl!");
    }
    usleep(1000);
    printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
    if (ioctl(fid, I2C_SMBUS,(struct i2c_smbus_ioctl_data *)&msgset) < 0) {
        printf("err happen in ioctl!  I2C_SMBUS\n");
        printf("errno=%d\n",errno);
        //exit(1);
    }
    return 0;
}





int smbus_read_nbytes(int fid,int addr,unsigned char command,unsigned char * value,int length)
{
    union i2c_smbus_data smdata;
    struct i2c_smbus_ioctl_data msgset;
    int ifor;
    for(ifor=0;ifor<length;ifor++)
    {
        smdata.block[ifor]=value[ifor]; 
    }
    msgset.read_write=I2C_READ_BIT;
    msgset.command=command;
    msgset.size=length;//I2C_SMBUS_WORD_DATA;
    msgset.data=&smdata;
    unsigned long funcs;
    if (ioctl(fid, I2C_SLAVE_FORCE, addr) < 0) {
        printf("err happen in ioctl!");
    }
    if (ioctl(fid, I2C_SMBUS,(struct i2c_smbus_ioctl_data *)&msgset) < 0) {
        printf("err happen in ioctl!  I2C_SMBUS\n");
        printf("errno=%d\n",errno);
    }
    // for(ifor=0;ifor<length;ifor++)
    // {
    //     value[ifor]=smdata.block[ifor]; 
    // }
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


















int bcm2835_I2C_Init(void)
{
    bcm2835_i2c_begin();
    //bcm2835_i2c_setSlaveAddress(0x3c);  //i2c address
    
    /*
    BCM2835_I2C_CLOCK_DIVIDER_2500 ：2500 = 10us = 100 kHz 
    BCM2835_I2C_CLOCK_DIVIDER_626  ：622 = 2.504us = 399.3610 kHz
    150 = 60ns = 1.666 MHz (default at reset)
    148 = 59ns = 1.689 MHz
    */
    //bcm2835_i2c_setSlaveAddress(0x3c);  //i2c address  
    bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);
//  bcm2835_i2c_set_baudrate(100000);    //100k baudrate
    printf("init OK\n");
    return 0;
}


int set_addr(unsigned char addr)
{
    printf(RED"addr=%X\n"NONE, addr);
    //bcm2835_i2c_begin();
    //bcm2835_i2c_setSlaveAddress(0x3c);  //i2c address
    
    /*
    BCM2835_I2C_CLOCK_DIVIDER_2500 ：2500 = 10us = 100 kHz 
    BCM2835_I2C_CLOCK_DIVIDER_626  ：622 = 2.504us = 399.3610 kHz
    150 = 60ns = 1.666 MHz (default at reset)
    148 = 59ns = 1.689 MHz
    */
    //bcm2835_i2c_setSlaveAddress(0x3c);  //i2c address 
    bcm2835_i2c_setSlaveAddress(addr);  //i2c address0x3c 
    //usleep(1000*100);
    //bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);
   

}
