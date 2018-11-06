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
#include <wiringPi.h>



#include <ltc2943.h>
#include <app_main.h>
#include <printcolor.h>
//#include "i2c-dev.h"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define LTC2943ADDR 0x64



const float LTC2943_CHARGE_lsb = 0.34E-3;
const float LTC2943_VOLTAGE_lsb = 1.44E-3;
const float LTC2943_CURRENT_lsb = 29.3E-6;
const float LTC2943_TEMPERATURE_lsb = 0.25;
const float LTC2943_FULLSCALE_VOLTAGE = 23.6;
const float LTC2943_FULLSCALE_CURRENT = 60E-3;
const float LTC2943_FULLSCALE_TEMPERATURE = 510;
const float LTC2943_FULLSCALE_RESISTOR=50;//50兆欧


int batteryfile;




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
    int addr = 0x64;
    smbus_read_nbytes(batteryfile,addr,LTC2943_VOLTAGE_MSB_REG,value,I2C_SMBUS_WORD_DATA);
    //smbus_read(LTC2943_VOLTAGE_MSB_REG,value,I2C_SMBUS_WORD_DATA);
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




int get_bat_percent()
{
    float *ftemp,ss;
    ftemp=&ss;
    float voltage;
    int bat;
    voltage=getvoltage1(ftemp);
    //printf(BLUE"inside %s voltage=%f\n"NONE,__FUNCTION__,voltage);
    voltage=((voltage-8)/4)*100;
    bat=(int)voltage;
    if(bat>100){
        bat=100;
    }
    if(bat<0){
        bat=1;
    }
    return bat;
}





float getvoltage()
{
    float *ftemp,ss;
    ftemp=&ss;
    float voltage;
    int bat;
    voltage=getvoltage1(ftemp);
    return voltage;
}













int conv_bat_2_percent(float *voltage)
{
    *voltage=((*voltage-8)/4)*100;
    int bat=(int)*voltage;
    //printf(BLUE"inside %s voltage=%f,bat=%d\n"NONE,__FUNCTION__,voltage,bat);
    if(bat>100){
        bat=100;
    }
    if(bat<0){
        bat=1;
    }
    return bat;
}













float gettemperature()
{
    printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
    unsigned char  value[2];
    unsigned short temperature;
    int addr = 0x64;
    //smbus_read(LTC2943_TEMPERATURE_MSB_REG,value,I2C_SMBUS_WORD_DATA);
    smbus_read_nbytes(batteryfile,addr,LTC2943_TEMPERATURE_MSB_REG,value,I2C_SMBUS_WORD_DATA);
    printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
    temperature=*(unsigned short *)value;
    printf("temperature=%d\n",temperature);
    myltc2943.temperature.adc_code=temperature;
    //printf("rrrrrrrrrrrrrrrrrrrrr V===============%d\n",temperature);
    myltc2943.temperature.temperature=LTC2943_code_to_celcius_temperature(temperature);
    return myltc2943.temperature.temperature;
}








float getcurrent()
{
    unsigned char  value[2];
    unsigned short current;
    int addr = 0x64;
    //smbus_read(LTC2943_CURRENT_MSB_REG ,value,I2C_SMBUS_WORD_DATA);
    smbus_read_nbytes(batteryfile,addr,LTC2943_CURRENT_MSB_REG,value,I2C_SMBUS_WORD_DATA);
    current=*(short *)value;
    myltc2943.current.adc_code=current;
    //printf("rrrrrrrrrrrrrrrrrrrrr V===============%d\n",current);
    myltc2943.current.current=LTC2943_code_to_current(current, LTC2943_FULLSCALE_RESISTOR);
    return myltc2943.current.current;
}
#define LTC_SWITCH  RPI_GPIO_P1_15     //P3
#define LTC_EN()     bcm2835_gpio_write(LTC_SWITCH,HIGH)
#define LTC_UNEN()     bcm2835_gpio_write(LTC_SWITCH,LOW)


int ltc2943_pin_init()
{
    if(app.app_sign.has_bcm2835_init==0){
        wiring_bcm2835_init();
    }
    bcm2835_gpio_fsel(LTC_SWITCH, BCM2835_GPIO_FSEL_OUTP);//
    LTC_EN();
    return 0;
}












//I2C_ReadnBytes(char * buf,int len)

int write_ltc2943(char * buf,int len)
{
    
    I2C_WritenBytes(buf,len);
    return 0;
}

int read_ltc2943(char * buf,int len)
{
    
    I2C_ReadnBytes(buf,len);
    return 0;
}


int set_ltc2943()
{
    piLock(1);
    set_addr(LTC2943ADDR);
    char value[2];
    value[0]=LTC2943_CONTROL_REG;
    value[1]=0xFC;
    write_ltc2943(value,2);
    piUnlock(1);
    return 0;
}

int read_ltc2943_reg(char * value)
{
    piLock(1);
    set_addr(LTC2943ADDR);
    // value[0]=LTC2943_CURRENT_MSB_REG;
    // value[1]=0xFC;
    write_ltc2943(value,1);
    read_ltc2943(value,2);
    piUnlock(1);
    return 0;
}

float getcurrent_bcm()
{
    unsigned char  value[2];
    unsigned short current;
    value[0]=0x0E;
    value[1]=0xaa;
    printf("before %X,%X\n",value[0],value[1]);
    //smbus_read(LTC2943_CURRENT_MSB_REG ,value,I2C_SMBUS_WORD_DATA);
    read_ltc2943_reg(value);
    printf("after %X,%X\n",value[0],value[1]);
    current=*(short *)value;
    myltc2943.current.adc_code=current;
    //printf("rrrrrrrrrrrrrrrrrrrrr V===============%d\n",current);
    myltc2943.current.current=LTC2943_code_to_current(current, LTC2943_FULLSCALE_RESISTOR);
    return myltc2943.current.current;
}





int get_batt_info()
{
    set_ltc2943();
    printf("current=%f\n",-1000*1000*getcurrent_bcm());
}








int openltc2943(APP_S * app)
{
    ltc2943_pin_init();
    // get_batt_info();
    // exit(0);
    int ret;
    char batteryfilename[20];
    printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
    snprintf(app->app_fid[LTC2943_FID].path, 19, "/dev/i2c-1");
    batteryfile = app->app_fid[LTC2943_FID].fid = open(app->app_fid[LTC2943_FID].path, O_RDWR);
    if (app->app_fid[LTC2943_FID].fid < 0) {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        printf("err happen!");
        exit(1);
    }
printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
/*
    snprintf(batteryfilename, 19, "/dev/i2c-1");
    bq20z95 = open(batteryfilename, O_RDWR);
    if (bq20z95 < 0) {
        // ERROR HANDLING; you can check errno to see what went wrong 
        printf("err happen!");
        //exit(1);
    }
*/
    int addr = 0x64;
    char data[4]={0,1,2,3};

    if (ioctl(app->app_fid[LTC2943_FID].fid, I2C_SLAVE_FORCE, addr) < 0) {
        printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
        printf("err happen in ioctl!");
    }
    unsigned char value[2];
    value[0]=0xFc;
    //unsigned short voltage;
    printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
    smbus_write_nbytes(batteryfile,addr,LTC2943_CONTROL_REG,value,2);
    //smbus_write_nbytes(app->app_fid[LTC2943_FID].fid,addr,LTC2943_CONTROL_REG,value,I2C_SMBUS_BYTE);
    //exit(0);
    //smbus_write(LTC2943_CONTROL_REG,value,I2C_SMBUS_BYTE_DATA);
    printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
    printf("current=%f\n",-1000*1000*getcurrent());
    //printf("current=%f\n",-1000*1000*getcurrent());

    float *ftemp , ss,stemp;
    ftemp = &ss;
    stemp=getvoltage1(ftemp);
    printf("voltage=%f\n",stemp);





/*
    int addr = 0xb; 
    if (ioctl(bq20z95, I2C_SLAVE_FORCE, addr) < 0) {
        printf("err happen in ioctl!");
        exit(1);
    }*/
    //bq20z95_read(LTC2943_CONTROL_REG,value,I2C_SMBUS_BYTE_DATA);
    return ret;
}
/*
repeat start demo
  struct i2c_msg rdwr_msgs[2] = {
    {  // Start address
      .addr = 0x50,
      .flags = 0, // write
      .len = 1,
      .buf = &reg_address
    },
    { // Read buffer
      .addr = 0x50,
      .flags = I2C_M_RD, // read
      .len = 16,
      .buf = buffer
    }
  };

  struct i2c_rdwr_ioctl_data rdwr_data = {
    .msgs = rdwr_msgs,
    .nmsgs = 2
  };

  file = open( "/dev/i2c-1", O_RDWR );
...

  result = ioctl( file, I2C_RDWR, &rdwr_data );

  if ( result < 0 ) {
    printf( "rdwr ioctl error: %d\n", errno );
    perror( "reason" );
  } else {
    printf( "rdwr ioctl OK\n" );
    for ( i = 0; i < 16; ++i ) {
      printf( "%c", buffer[i] );
    }
    printf( "\n" );
  }
*/
