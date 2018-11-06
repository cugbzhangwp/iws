#include <DEV_Config.h>
#include <bcm2835.h>
#include <stdio.h>      //printf()
#include <i2crw.h>
//wiring
//#include "DEV_Config.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <printcolor.h>

int wiring_fd;


int wiringPiI2C0Setup (const int devId)
{
  int rev ;
  const char *device ;

  rev = piGpioLayout () ;

  
  device = "/dev/i2c-0" ;

  return wiringPiI2CSetupInterface (device, devId) ;
}








/********************************************************************************
function:   System Init and exit
note:
    Initialize the communication method
********************************************************************************/
uint8_t System_Init_wiring(void)
{
    //1.wiringPiSetupGpio
    //if(wiringPiSetup() < 0)//use wiringpi Pin number table
    //return 0;
    // if(wiringPiSetupGpio() < 0) { //use BCM2835 Pin number table
    //     printf("set wiringPi lib failed !!! \r\n");
    //     return 1;
    // } else {
    //     printf("set wiringPi lib success  !!! \r\n");
    // }
    //return 0;
    //2.GPIO config
    pinMode(OLED_RST,      OUTPUT);
    //pinMode(OLED_DC,       OUTPUT);
    pinMode(OLED_CS,       OUTPUT);

#if USE_SPI_4W
    //3.spi init
    printf("USE_SPI\r\n");       
    //wiringPiSPISetup(0,9000000);
    wiringPiSPISetupMode(0, 9000000, 0);
#elif USE_IIC
    //OLED_DC_1;
    OLED_CS_0;
    printf("USE_IIC\r\n");
    wiring_fd = wiringPiI2CSetup(0x3c);
    printf(LIGHT_CYAN"wiring_fd=%d\n"NONE,wiring_fd);
    while(wiring_fd<0){
        perror("err open i2c\n");
        wiring_fd = wiringPiI2CSetup(0x3c);
        printf(LIGHT_CYAN"wiring_fd=%d\n"NONE,wiring_fd);
    }

#endif

    return 0;
}

void System_Exit_wiring(void)
{
    OLED_CS_0;
    OLED_RST_1;
    //OLED_DC_0;
}

/********************************************************************************
function:   Hardware interface
note:
    SPI4W_Write_Byte(value) : 
        hardware SPI
    I2C_Write_Byte(value, cmd):
        hardware I2C
********************************************************************************/
void SPI4W_Write_Byte_wiring(uint8_t value)
{
    int read_data;
    read_data = wiringPiSPIDataRW(0,&value,1);
    if(read_data < 0)
        perror("wiringPiSPIDataRW failed\r\n");
}

void I2C_Write_Byte_wiring(uint8_t value, uint8_t Cmd)
{
    int ref=ioctl(wiring_fd, I2C_SLAVE_FORCE, 0x3c);
    while(ref<0) {
        ioctl(wiring_fd, I2C_SLAVE_FORCE, 0x3c);
        usleep(2000);
        printf("err happen in ioctl!");
    }
    ref = wiringPiI2CWriteReg8(wiring_fd, (int)Cmd, (int)value);
    while(ref != 0) {
        ref = wiringPiI2CWriteReg8 (wiring_fd, (int)Cmd, (int)value);
        if(ref == 0)
            break;
    }   
}
/*
7e 00 04 08 01 48 56 58
*/
/*********************************************
*
*
*********************************************/
void Driver_Delay_ms_wiring(uint32_t xms)
{
    delay(xms);
}

void Driver_Delay_us_wiring(uint32_t xus)
{
    int j;
    for(j=xus; j > 0; j--);
}










































///bcm
/********************************************************************************
function:   System Init and exit
note:
    Initialize the communication method
********************************************************************************/
uint8_t System_Init(void)
{
    if(!bcm2835_init()) {
        printf("bcm2835 init failed   !!! \r\n");
        return 1;
    } else {
        printf("bcm2835 init success  !!! \r\n");
    }

    bcm2835_gpio_fsel(OLED_RST,      BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(OLED_DC,       BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(OLED_CS,       BCM2835_GPIO_FSEL_OUTP);

#if USE_SPI_4W
    printf("USE_SPI\r\n");
    bcm2835_spi_begin();                                         //Start spi interface, set spi pin for the reuse function
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);     //High first transmission
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                  //spi mode 1
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_2048);   //Frequency
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                     //set CE0
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);     //enable cs0
#elif USE_SPI_3W    
#elif USE_IIC
    OLED_DC_1;
    OLED_CS_0;
    printf("USE_IIC\r\n");
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(0x3c);  //i2c address
    
    /*
    BCM2835_I2C_CLOCK_DIVIDER_2500 ：2500 = 10us = 100 kHz 
    BCM2835_I2C_CLOCK_DIVIDER_626  ：622 = 2.504us = 399.3610 kHz
    150 = 60ns = 1.666 MHz (default at reset)
    148 = 59ns = 1.689 MHz
    */  
    bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);
//  bcm2835_i2c_set_baudrate(100000);    //100k baudrate

#endif

    return 0;
}

void System_Exit(void)
{
#if USE_SPI_4W
    bcm2835_spi_end();
#elif USE_SPI_3W    

#elif USE_IIC
    bcm2835_i2c_end();
#endif
    bcm2835_close();
}

/********************************************************************************
function:   Hardware interface
note:
    SPI4W_Write_Byte(value) : 
        hardware SPI
    SPI3W_Write_Byte(Value, Cmd) : 
        Gpio analog SPI
    I2C_Write_Byte(value, cmd):
        hardware I2C
********************************************************************************/
void SPI4W_Write_Byte(uint8_t value)
{
    char buf[1] = {0XFF};
    buf[0] = (char)value;
    bcm2835_spi_transfern(buf, 1);
}

void I2C_Write_Byte(uint8_t value, uint8_t Cmd)
{
    char buf[2] = {IIC_RAM,0XFF};
    int ref;
    buf[1] = (char)value;
    
    if(Cmd == IIC_RAM)
        buf[0] = IIC_RAM;
    else
        buf[0] = IIC_CMD;
    
    ref = bcm2835_i2c_write(buf, 2);
    while(ref != 0) {
        ref = bcm2835_i2c_write(buf, 2);
        if(ref == 0)
            break;
    }   
}

/********************************************************************************
function:   Delay function
note:
    Driver_Delay_ms(xms) : Delay x ms
    Driver_Delay_us(xus) : Delay x us
********************************************************************************/
void Driver_Delay_ms(uint32_t xms)
{
    bcm2835_delay(xms);
}

void Driver_Delay_us(uint32_t xus)
{
    int j;
    for(j=xus; j > 0; j--);
}
