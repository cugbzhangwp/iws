#include <stdio.h>		//printf()
#include <stdlib.h>		//exit()

#include <OLED_Driver.h>
#include <OLED_GUI.h>
#include <DEV_Config.h>
#include <time.h>
#include <app_main.h>
#include <DEV_Config.h>
#include <i2crw.h>




int oled_pin_init()
{
    // bcm2835_gpio_fsel(OLED_RST,      BCM2835_GPIO_FSEL_OUTP);
    // bcm2835_gpio_fsel(OLED_DC,       BCM2835_GPIO_FSEL_OUTP);
    // bcm2835_gpio_fsel(OLED_CS,       BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(OLED_EN,       BCM2835_GPIO_FSEL_OUTP);
	//OLED_DC_1;
    //OLED_CS_0;
    OLED_EN_HIGH;
}













char oled_reg_init[]={0xae,0x15,0x00,0x7f,0x75,0x00,0x7f,0x81,0x80,0xa0,0x51\
	,0xa1,0x00,0xa2,0x00,0xa4,0xa8,0x7f,0xb1,0xf1,0xb3,0x00,0xab,0x01,0xb6\
	,0x0f,0xbe,0x0f,0xbc,0x08,0xd5,0x62,0xfd,0x12};







// #define OLEDADDR 0x3c
// int open_oled(APP_S * app)
// {
//     int ret;
//     char oledfilename[20];
//     //beep_init();
//     printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
//     snprintf(oledfilename, 19, "/dev/i2c-1");
//     int oled_file_id;
//     oled_file_id= open(oledfilename, O_RDWR);
//     if (oled_file_id < 0) {
//         /* ERROR HANDLING; you can check errno to see what went wrong */
//         printf("err happen!");
//         exit(1);
//     }
//     int addr = OLEDADDR; 
//     char data[4]={0,1,2,3};
//     if (ioctl(oled_file_id, I2C_SLAVE_FORCE, addr) < 0) {
//         printf("err happen in ioctl!");
//     }
//     //printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
// 	printf("**********Init OLED**********\r\n");
// 	OLED_SCAN_DIR OLED_ScanDir = SCAN_DIR_DFT;//SCAN_DIR_DFT = D2U_L2R
// 	ioctl_OLED_Init(OLED_ScanDir);	
//     //unsigned short voltage;
//     //smbus_write(DERECT_REG_P0,value,I2C_SMBUS_BYTE_DATA);
// 	//smbus_write_nbytes(pcf8574_file_id,addr,0xff,value,1);
// 	//sleep(1);
// 	//smbus_write_nbytes(pcf8574_file_id,addr,0x00,value,1);
// 	//usleep(1000);
// 	//smbus_write_nbytes(pcf8574_file_id,addr,0xff,value,1);
//     return 0;
// }

int oled_main(APP_S * app)
{
	//1.System Initialization
	
	
	//2.show
	
	printf("**********Init OLED**********\r\n");
	OLED_SCAN_DIR OLED_ScanDir = SCAN_DIR_DFT;//SCAN_DIR_DFT = D2U_L2R
	OLED_Init(OLED_ScanDir);	

	printf("Show GUI\r\n");
	GUI_Show();
	
	OLED_Clear(OLED_BACKGROUND);//OLED_BACKGROUND
	OLED_Display();	
	
	printf("Show Pic\r\n");
	GUI_Disbitmap(0  , 2, Signal816  , 16, 8);
	GUI_Disbitmap(24 , 2, Bluetooth88, 8 , 8);
	GUI_Disbitmap(40 , 2, Msg816     , 16, 8);
	GUI_Disbitmap(64 , 2, GPRS88     , 8 , 8);
	GUI_Disbitmap(90 , 2, Alarm88    , 8 , 8);
	GUI_Disbitmap(112, 2, Bat816     , 16, 8);

	printf("Show 16 Gray Map\r\n");
	GUI_DisGrayMap(0,73,gImage_flower);

	GUI_DisString_EN(0 , 52, "MUSIC", &Font12, FONT_BACKGROUND, WHITE); 
	GUI_DisString_EN(48, 52, "MENU" , &Font12, FONT_BACKGROUND, WHITE); 
	GUI_DisString_EN(90, 52, "PHONE", &Font12, FONT_BACKGROUND, WHITE);
	//set_addr(0x3c);
	OLED_Display();

	printf("Show time\r\n");
	time_t now;
	struct tm *timenow;
	uint8_t now_time,new_time = 11;
	DEV_TIME sDev_time;
	while(1){
		sleep(1);
		time(&now);
		timenow = localtime(&now);
		 
		sDev_time.Hour = timenow->tm_hour;
		sDev_time.Min = timenow->tm_min;
		sDev_time.Sec = timenow->tm_sec;
		now_time = sDev_time.Sec;
		
		OLED_ClearWindow(0, 22, 127, 47, WHITE);
		GUI_Showtime(0, 22, 127, 47, &sDev_time, WHITE);
		if(now_time != new_time){
			OLED_DisWindow(0, 22, 127, 47);		
			new_time = now_time;
		}		
	}
	
	//3.System Exit
	System_Exit();
	return 0;
	
}

