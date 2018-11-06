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
#include "../inc/ltc2943.h"
#include "../inc/oso_common.h"
//#include "i2c-dev.h"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
int batteryfile;

//! Read a byte, store in "value".
//! @return 0 on success, 1 on failure

const float LTC2943_CHARGE_lsb = 0.34E-3;
const float LTC2943_VOLTAGE_lsb = 1.44E-3;
const float LTC2943_CURRENT_lsb = 29.3E-6;
const float LTC2943_TEMPERATURE_lsb = 0.25;
const float LTC2943_FULLSCALE_VOLTAGE = 23.6;
const float LTC2943_FULLSCALE_CURRENT = 60E-3;
const float LTC2943_FULLSCALE_TEMPERATURE = 510;
const float LTC2943_FULLSCALE_RESISTOR=50;//50兆欧







typedef struct COULOMBS{
float coulombs;
uint16_t adc_code;
float resistor;
uint16_t prescalar;
}COULOMBS;
typedef struct mAh{
float mah;
uint16_t adc_code;
float resistor;
uint16_t prescalar;
}mAh;

typedef struct CURRENT{
float current;
uint16_t adc_code;
float resistor;
}CURRENT;



typedef struct VOLTAGE{
float voltage;
uint16_t adc_code;
}VOLTAGE;
typedef struct TEMPERATURE{
float temperature;
uint16_t adc_code;
}TEMPERATURE;

typedef struct LTC2943{
VOLTAGE voltage;
TEMPERATURE temperature;
CURRENT current;
mAh mah;
COULOMBS coulombs;
}LTC2943;



LTC2943 myltc2943={0};








float LTC2943_code_to_coulombs(uint16_t adc_code, float resistor, uint16_t prescalar)
// The function converts the 16-bit RAW adc_code to Coulombs
{
  float coulomb_charge;
  coulomb_charge =  1000*(float)(adc_code*LTC2943_CHARGE_lsb*prescalar*50E-3)/(resistor*4096);
  coulomb_charge = coulomb_charge*3.6f;
  return(coulomb_charge);
}

float LTC2943_code_to_mAh(uint16_t adc_code, float resistor, uint16_t prescalar )
// The function converts the 16-bit RAW adc_code to mAh
{
  float mAh_charge;
  mAh_charge = 1000*(float)(adc_code*LTC2943_CHARGE_lsb*prescalar*50E-3)/(resistor*4096);
  return(mAh_charge);
}

float LTC2943_code_to_voltage(uint16_t adc_code)
// The function converts the 16-bit RAW adc_code to Volts
{
  float voltage;
  voltage = ((float)adc_code/(65535))*LTC2943_FULLSCALE_VOLTAGE;
  return(voltage);
}

float LTC2943_code_to_current(uint16_t adc_code, float resistor)
// The function converts the 16-bit RAW adc_code to Amperes
{
  float current;
  current = (((float)adc_code-32767)/(32767))*((float)(LTC2943_FULLSCALE_CURRENT)/resistor);
  return(current);
}

float LTC2943_code_to_kelvin_temperature(uint16_t adc_code)
// The function converts the 16-bit RAW adc_code to Kelvin
{
  float temperature;
  temperature = adc_code*((float)(LTC2943_FULLSCALE_TEMPERATURE)/65535);
  return(temperature);
}

float LTC2943_code_to_celcius_temperature(uint16_t adc_code)
// The function converts the 16-bit RAW adc_code to Celcius
{
  float temperature;
  temperature = adc_code*((float)(LTC2943_FULLSCALE_TEMPERATURE)/65535) - 273.15;
  return(temperature);
}







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





int smbus_write(unsigned char command,unsigned char * value,int length)
{
union i2c_smbus_data smdata;
struct i2c_smbus_ioctl_data msgset;
smdata.block[0]=value[0];
smdata.block[1]=0x1;
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


  if (ioctl(batteryfile, I2C_SMBUS,(struct i2c_smbus_ioctl_data *)&msgset) < 0) {
	printf("err happen in ioctl!  I2C_SMBUS\n");
printf("errno=%d\n",errno);
    //exit(1);
  }
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
    if (ioctl(batteryfile, I2C_SMBUS,(struct i2c_smbus_ioctl_data *)&msgset) < 0) {
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



extern 		unsigned long stackbottom;

float getvoltage1( float *ftemp)
{
    //float ftemp;
    //*ftemp = 9.0;
    //	memcpy( &ftemp , &ffs , 4);
    //printf("in function x===============%x\n",*ftemp);
    //printf("in function d===============%f\n",*ftemp);
    //return 0;
    unsigned char  value[2];
    unsigned short voltage;
    unsigned long stacktop=&stacktop;
    unsigned long stacktop1=&stacktop1;
    //printf("stackbottom= %lu\n",stackbottom);
    //printf("stack used %lu\n",stackbottom-stacktop);
    //printf("stacktop= %lu\n",stacktop);
    //printf("stacktop1= %lu\n",stacktop1);
    smbus_read(LTC2943_VOLTAGE_MSB_REG,value,I2C_SMBUS_WORD_DATA);
    voltage=*(short *)value;
    myltc2943.voltage.adc_code=voltage;
    //printf("rrrrrrrrrrrrrrrrrrrrr V===============%d\n",voltage);
    myltc2943.voltage.voltage=LTC2943_code_to_voltage(voltage);
    //usleep(100000);
    int inttmp;
    float V=myltc2943.voltage.voltage;
    memcpy(&inttmp,&V,4);
    //printf("in getvoltage function v f===============%f\n",V);
    //printf("in getvoltage function v x===============%x\n",V);
    //printf("in getvoltage function inttmp d===============%d\n",inttmp);
    //printf("in getvoltage function inttmp x===============%x\n",inttmp);
    *ftemp=V;
    return V;
}



float gettemperature()
{
    unsigned char  value[2];
    unsigned short temperature;
    smbus_read(LTC2943_TEMPERATURE_MSB_REG,value,I2C_SMBUS_WORD_DATA);
    temperature=*(short *)value;
    myltc2943.temperature.adc_code=temperature;
    //printf("rrrrrrrrrrrrrrrrrrrrr V===============%d\n",temperature);
    myltc2943.temperature.temperature=LTC2943_code_to_celcius_temperature(temperature);
    return myltc2943.temperature.temperature;
}








float getcurrent()
{
    unsigned char  value[2];
    unsigned short current;
    smbus_read(LTC2943_CURRENT_MSB_REG ,value,I2C_SMBUS_WORD_DATA);
    current=*(short *)value;
    myltc2943.current.adc_code=current;
    //printf("rrrrrrrrrrrrrrrrrrrrr V===============%d\n",current);
    myltc2943.current.current=LTC2943_code_to_current(current, LTC2943_FULLSCALE_RESISTOR);
    return myltc2943.current.current;
}





int open_z95()
{

    int ret;
    char batteryfilename[20];

    snprintf(batteryfilename, 19, "/dev/i2c-1");
    batteryfile = open(batteryfilename, O_RDWR);
    if (batteryfile < 0) {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        printf("err happen!");
        //exit(1);
    }

    int addr = 0x64; /* The I2C address */
    char data[4]={0,1,2,3};
    char readdata=0x14;
    if (ioctl(batteryfile, I2C_SLAVE_FORCE, addr) < 0) {
    /*ERROR HANDLING; you can check errno to see what went wrong */
    printf("err happen in ioctl!");
    //exit(1);
    }
    unsigned char  value[2];
    char p;
    int ifor=0;
    value[0]=0xFc;
    unsigned short voltage;
    smbus_write(LTC2943_CONTROL_REG,value,I2C_SMBUS_BYTE_DATA);
    return ret;
}


