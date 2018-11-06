#include <stdio.h>		//printf()
#include <stdlib.h>		//exit()

#include <OLED_Driver.h>
#include <OLED_GUI.h>
#include <DEV_Config.h>
#include <time.h>
#include <app_main.h>
#include <DEV_Config.h>
#include <i2crw.h>
#include <printcolor.h>
extern APP_S app;


extern char eth_ip[];
extern char wlan_ip[];
extern char eth_ip_now[];
extern char wlan_ip_now[];
extern getlocalips(char * ,char * );


char line1[48];
char line2[48];
char line3[48];
char line4[48];


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
	if(System_Init())
		exit(0);
	
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
	OLED_Display();
	
	printf("Show time\r\n");
	time_t now;
	struct tm *timenow;
	uint8_t now_time,new_time = 11;
	DEV_TIME sDev_time;
	while(1){
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











int oled_main1(APP_S * app)
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


int oled_net_info2()
{
		snprintf(eth_ip,100,eth_ip_now);
		snprintf(wlan_ip,100,wlan_ip_now);
		OLED_ClearWindow_wiring(0, 56, 128, 128, WHITE);
		//GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
		GUI_DisString_EN_wiring(0, 56, eth_ip, &Font12, FONT_BACKGROUND, WHITE);
		GUI_DisString_EN_wiring(0, 76, wlan_ip, &Font12, FONT_BACKGROUND, WHITE);
		OLED_DisWindow_wiring(0, 56, 128, 128);
	return 0;
}

int oled_net_info()
{
	if((strstr(eth_ip_now,eth_ip)==NULL)||(strstr(wlan_ip_now,wlan_ip)==NULL)){
		printf("eth_ip_now=%s,eth_ip=%s,wlan_ip_now=%s,wlan_ip=%s",eth_ip_now,eth_ip,wlan_ip_now,wlan_ip);
		snprintf(eth_ip,100,eth_ip_now);
		snprintf(wlan_ip,100,wlan_ip_now);
		OLED_ClearWindow_wiring(0, 56, 128, 128, WHITE);
		//GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
		GUI_DisString_EN_wiring(0, 56, eth_ip, &Font12, FONT_BACKGROUND, WHITE);
		GUI_DisString_EN_wiring(0, 76, wlan_ip, &Font12, FONT_BACKGROUND, WHITE);
		OLED_DisWindow_wiring(0, 56, 128, 128);
	}	
	return 0;
}







int oled_net_info1()
{
	snprintf(eth_ip,100,eth_ip_now);
	snprintf(wlan_ip,100,wlan_ip_now);
	OLED_ClearWindow_wiring(0, 56, 128, 128, WHITE);
	//GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 56, eth_ip, &Font12, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 76, wlan_ip, &Font12, FONT_BACKGROUND, WHITE);
	OLED_DisWindow_wiring(0, 0, 128, 128);
	return 0;
}






char batt_v[24];
char batt_i[24];
char batt_t[24];
extern float getvoltage1( float *ftemp);
extern float getcurrent();
extern float gettemperature();

int oled_batt_info(){
	float ftemp;
	snprintf(batt_v,24,"V = %f(v)",getvoltage1(&ftemp));
	snprintf(batt_i,24,"I = %f(mA)",400*1000*getcurrent());
	snprintf(batt_t,24,"T = %f(C)",gettemperature());

	OLED_ClearWindow_wiring(0, 56, 128, 128, WHITE);
	//GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 56, batt_v, &Font12, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 76, batt_i, &Font12, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 96, batt_t, &Font12, FONT_BACKGROUND, WHITE);
	OLED_DisWindow_wiring(0, 0, 128, 128);
	return 0;
}

int oled_sd_info(){
//    sprintf(sendbuf,"DT=%s,DTC=%s,DFC=%s,DP=%s,%d",di->disk_format,di->disk_total_capacity\
        ,di->disk_free_capacity,di->disk_permissions,app.app_file[APP_FILE_DATA].control);
	// snprintf(batt_v,24,"V=%f",getvoltage1(&ftemp));
	// snprintf(batt_i,24,"I=%f",400*1000*getcurrent());
	// snprintf(batt_t,24,"T=%f",gettemperature());
	snprintf(line1,48,"Fs:%s",app.di.disk_format);
	snprintf(line2,48,"Total:%s(GB)",app.di.disk_total_capacity);
	snprintf(line3,48,"Free:%s(GB)",app.di.disk_free_capacity);
	OLED_ClearWindow_wiring(0, 56, 128, 128, WHITE);
	//GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 56, line1, &Font12, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 76, line2, &Font12, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 96, line3, &Font12, FONT_BACKGROUND, WHITE);
	//GUI_DisString_EN_wiring(0, 100, app.di.disk_permissions, &Font12, FONT_BACKGROUND, WHITE);
	OLED_DisWindow_wiring(0, 0, 128, 128);








}
int oled_gps_info(){

	snprintf(line1,20,"lon:%010d",app.gps_data->ew_d*1000000+app.gps_data->ew_m10000);
	snprintf(line2,20,"lat:%010d",app.gps_data->ns_d*1000000+app.gps_data->ns_m10000);
	snprintf(line3,20,"height:%010d",app.gps_data->height_m);
	printf("line1=%s\n",line1);
	printf("line2=%s\n",line2);
	printf("line3=%s\n",line3);
	OLED_ClearWindow_wiring(0, 56, 128, 128, WHITE);
	//GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 56, line1, &Font12, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 76, line2, &Font12, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 96, line3, &Font12, FONT_BACKGROUND, WHITE);
	//GUI_DisString_EN_wiring(0, 100, app.di.disk_permissions, &Font12, FONT_BACKGROUND, WHITE);
	OLED_DisWindow_wiring(0, 0, 128, 128);



}








int update_ip()
{
	GUI_DisString_EN_wiring(44, 25, "IWS", &Font20, FONT_BACKGROUND, WHITE);
	//GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 56, eth_ip, &Font12, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 76, wlan_ip, &Font12, FONT_BACKGROUND, WHITE);
	OLED_DisWindow_wiring(0, 25, 128, 128);
	return 0;
}




int update_batt()
{
	GUI_DisString_EN_wiring(44, 25, "IWS", &Font20, FONT_BACKGROUND, WHITE);
	//GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 56, eth_ip, &Font12, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN_wiring(0, 76, wlan_ip, &Font12, FONT_BACKGROUND, WHITE);
	OLED_DisWindow_wiring(0, 25, 128, 128);
	return 0;
}









int oledmain_wiring(void)
{
	//1.System Initialization
	//return 0;
	if(System_Init_wiring()){
		exit(0);
	}
	//sleep(2);
	//return 0;
	//2.show
	printf("**********Init OLED**********\r\n");
	OLED_SCAN_DIR OLED_ScanDir = SCAN_DIR_DFT;//SCAN_DIR_DFT = D2U_L2R
	OLED_Init_wiring(OLED_ScanDir);
	printf("OLED Show \r\n");
	//GUI_Show_wiring();

	OLED_Clear_wiring(OLED_BACKGROUND);//OLED_BACKGROUND
	OLED_Display_wiring();
	// printf("Show Pic\r\n");
	// GUI_Disbitmap_wiring(0  , 2, Signal816  , 16, 8);
	// GUI_Disbitmap_wiring(24 , 2, Bluetooth88, 8 , 8);
	// GUI_Disbitmap_wiring(40 , 2, Msg816     , 16, 8);
	// GUI_Disbitmap_wiring(64 , 2, GPRS88     , 8 , 8);
	// GUI_Disbitmap_wiring(90 , 2, Alarm88    , 8 , 8);
	// GUI_Disbitmap_wiring(112, 2, Bat816     , 16, 8);
	// printf("Show 16 Gray Map\r\n");
	// GUI_DisGrayMap_wiring(0,73,gImage_flower);
	// GUI_DisString_EN_wiring(0 , 52, "MUSIC", &Font12, FONT_BACKGROUND, WHITE); 
	// GUI_DisString_EN_wiring(48, 52, "MENU" , &Font12, FONT_BACKGROUND, WHITE); 
	// GUI_DisString_EN_wiring(90, 52, "PHONE", &Font12, FONT_BACKGROUND, WHITE);
	// OLED_Display_wiring();
	//exit(0);
	//while(1);
	// printf("Show time\r\n");

	time_t now;
	struct tm *timenow;
	uint8_t now_time,new_time = 11;
	DEV_TIME sDev_time;
	char * start_str="booting...";
	char wait_str[21];
	OLED_ClearWindow_wiring(0, 0, 128, 128, WHITE);
	GUI_DisString_EN_wiring(44, 25, "IWS", &Font20, FONT_BACKGROUND, WHITE);
	//GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
	// update_ip();
	// exit(0);
	int ifor;
	// for(ifor=0;ifor<20;ifor++)
	// {
	// 	wait_str[ifor]='.';
	// 	wait_str[ifor+1]='\0';
	// 	GUI_DisString_EN_wiring(0, 56, start_str, &Font12, FONT_BACKGROUND, WHITE);
	// 	GUI_DisString_EN_wiring(0, 76, wait_str, &Font12, FONT_BACKGROUND, WHITE);
	// 	OLED_DisWindow_wiring(0, 25, 128, 128);	
	// 	usleep(1000*200);
	// }
	// exit(0);

	int loop=0;
	int loop_second=0;
	while(1)
	{
		loop_second++;
		usleep(1000*200);
		time(&now);
		timenow = localtime(&now);
		sDev_time.Hour = timenow->tm_hour;
		sDev_time.Min = timenow->tm_min;
		sDev_time.Sec = timenow->tm_sec;
		now_time = sDev_time.Sec;
		OLED_ClearWindow_wiring(0, 0, 128, 24, WHITE);
		GUI_Showtime_wiring(0, 0, 128, 35, &sDev_time, WHITE);
		if(now_time != new_time){
			OLED_DisWindow_wiring(0, 0, 128, 24);	
			new_time = now_time;
		}
oled_batt_info();
	// float ftemp;
	// snprintf(batt_v,24,"V=%f",getvoltage1(&ftemp));
	// snprintf(batt_i,24,"I=%f",400*1000*getcurrent());
	// snprintf(batt_t,24,"T=%f",gettemperature());

	// OLED_ClearWindow_wiring(0, 56, 128, 128, WHITE);
	// //GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
	// GUI_DisString_EN_wiring(0, 56, batt_v, &Font12, FONT_BACKGROUND, WHITE);
	// GUI_DisString_EN_wiring(0, 76, batt_i, &Font12, FONT_BACKGROUND, WHITE);
	// GUI_DisString_EN_wiring(0, 96, batt_t, &Font12, FONT_BACKGROUND, WHITE);
	// OLED_DisWindow_wiring(0, 0, 128, 128);

sleep(6);
	// snprintf(eth_ip,100,eth_ip_now);
	// snprintf(wlan_ip,100,wlan_ip_now);
	// OLED_ClearWindow_wiring(0, 56, 128, 128, WHITE);
	// //GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
	// GUI_DisString_EN_wiring(0, 56, eth_ip, &Font12, FONT_BACKGROUND, WHITE);
	// GUI_DisString_EN_wiring(0, 76, wlan_ip, &Font12, FONT_BACKGROUND, WHITE);
	// OLED_DisWindow_wiring(0, 0, 128, 128);
	// sleep(6);
oled_net_info1();
sleep(6);
//continue;
oled_sd_info();
sleep(6);
oled_gps_info();
sleep(6);
//int ref=ioctl(wiring_fd, I2C_SLAVE_FORCE, 0x3c);
		continue;
		// loop++;
		// loop=loop==2?0:loop;

		switch(loop)
		{
			case 0:
			sleep(4);
			oled_net_info1();
			break;
			case 1:
			sleep(5);
			oled_batt_info();
			break;
			case 2:
			oled_sd_info();
			break;
			case 3:
			oled_gps_info();
			break;
			case 4:
			//oled_batt_info();
			break;
			case 5:
			//oled_batt_info();
			break;
			default:
			break;
		}

	}
	while(1){
		usleep(1000*200);
		//printf("brefore eth_ip_now=%s,eth_ip=%s,wlan_ip_now=%s,wlan_ip=%s\n",eth_ip_now,eth_ip,wlan_ip_now,wlan_ip);
		if((strstr(eth_ip_now,eth_ip)==NULL)||(strstr(wlan_ip_now,wlan_ip)==NULL)){
			printf("eth_ip_now=%s,eth_ip=%s,wlan_ip_now=%s,wlan_ip=%s",eth_ip_now,eth_ip,wlan_ip_now,wlan_ip);
			snprintf(eth_ip,100,eth_ip_now);
			snprintf(wlan_ip,100,wlan_ip_now);
			OLED_ClearWindow_wiring(0, 56, 128, 128, WHITE);
			//GUI_DisString_EN_wiring(0, 56, "device ip :", &Font20, FONT_BACKGROUND, WHITE);
			GUI_DisString_EN_wiring(0, 56, eth_ip, &Font12, FONT_BACKGROUND, WHITE);
			GUI_DisString_EN_wiring(0, 76, wlan_ip, &Font12, FONT_BACKGROUND, WHITE);
			OLED_DisWindow_wiring(0, 56, 128, 128);
		}	
		time(&now);
		timenow = localtime(&now);
		sDev_time.Hour = timenow->tm_hour;
		sDev_time.Min = timenow->tm_min;
		sDev_time.Sec = timenow->tm_sec;
		now_time = sDev_time.Sec;
		OLED_ClearWindow_wiring(0, 0, 128, 35, WHITE);
		GUI_Showtime_wiring(0, 0, 128, 35, &sDev_time, WHITE);
		if(now_time != new_time){
			OLED_DisWindow_wiring(0, 0, 128, 35);	
			new_time = now_time;
		}
	}
	//3.System Exit
	System_Exit_wiring();
	return 0;
	
}
