/*
 * ADS1256_test.c:
 *	Very simple program to test the serial port. Expects
 *	the port to be looped back to itself
 *
 */
 
/*
             define from bcm2835.h                       define from Board DVK511
                 3.3V | | 5V               ->                 3.3V | | 5V
    RPI_V2_GPIO_P1_03 | | 5V               ->                  SDA | | 5V 
    RPI_V2_GPIO_P1_05 | | GND              ->                  SCL | | GND
       RPI_GPIO_P1_07 | | RPI_GPIO_P1_08   ->                  IO7 | | TX
                  GND | | RPI_GPIO_P1_10   ->                  GND | | RX
       RPI_GPIO_P1_11 | | RPI_GPIO_P1_12   ->                  IO0 | | IO1
    RPI_V2_GPIO_P1_13 | | GND              ->                  IO2 | | GND
       RPI_GPIO_P1_15 | | RPI_GPIO_P1_16   ->                  IO3 | | IO4
                  VCC | | RPI_GPIO_P1_18   ->                  VCC | | IO5
       RPI_GPIO_P1_19 | | GND              ->                 MOSI | | GND
       RPI_GPIO_P1_21 | | RPI_GPIO_P1_22   ->                 MISO | | IO6
       RPI_GPIO_P1_23 | | RPI_GPIO_P1_24   ->                  SCK | | CE0
                  GND | | RPI_GPIO_P1_26   ->                  GND | | CE1

::if your raspberry Pi is version 1 or rev 1 or rev A
RPI_V2_GPIO_P1_03->RPI_GPIO_P1_03
RPI_V2_GPIO_P1_05->RPI_GPIO_P1_05
RPI_V2_GPIO_P1_13->RPI_GPIO_P1_13
::
*/
#include <stdio.h>
#define __USE_GNU  
#include <sched.h>
#include <bcm2835.h>  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <app_main.h>
#include <printcolor.h>
#include <time_geo.h>
#include <wiringPi.h>
#include <alg.h>
//#include <sched.h>
//CS      -----   SPICS  
//DIN     -----   MOSI
//DOUT  -----   MISO
//SCLK   -----   SCLK
//DRDY  -----   ctl_IO     data  starting
//RST     -----   ctl_IO     reset

#define ADS1256_DEBUG_COLOR RED

//printf(RED"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__)

char tbuf[100];
IWS_TRIG_PSAGET iws_trig_psaget;
IWS_TRIG_PGAGET iws_trig_pgaget;
void read_data_setch_interupt();
extern int send_go_signal(int to_who);
extern int init_iws_steim2_para(IWS_STEIM2 *steim2_para);

int fengle();
///extern int iws_do_steim_pre(FIR_LIST *list,IWS_STEIEM2 * steim2_init_para);
extern APP_S app;
#define button RPI_V2_GPIO_P1_32
#define isr1 6
char flag = 0;
int rdy_count=0;
int is_cdate=0;
int has_read_id=0;
int has_reset=0;

#define  DRDY  RPI_GPIO_P1_11         //P0
#define  PWDN  RPI_GPIO_P1_13         //P2
#define  RST  RPI_GPIO_P1_12     //P1
//#define	SPICS	RPI_GPIO_P1_15	//P3微雪
#define	SPICS	RPI_GPIO_P1_24	//iws
#define	SPICS_DAC	RPI_GPIO_P1_26	//iws



//#define	SPICS	RPI_GPIO_P1_15	//P3
#define CS_1() bcm2835_gpio_write(SPICS,HIGH)
#define CS_0()  bcm2835_gpio_write(SPICS,LOW)

#define  DRDY_IS_LOW()	((bcm2835_gpio_lev(DRDY)==0))
#define  PWDN_IS_LOW()	bcm2835_gpio_write(PWDN,LOW)
#define  PWDN_IS_HIGH()	bcm2835_gpio_write(PWDN,HIGH)




#define RST_1() 	bcm2835_gpio_write(RST,HIGH)
#define RST_0() 	bcm2835_gpio_write(RST,LOW)


/* Unsigned integer types  */
#define uint8_t unsigned char
#define uint16_t unsigned short    
#define uint32_t unsigned long     









ADS1256_VAR_T g_tADS1256;





void  bsp_DelayUS(uint64_t micros);
void ADS1256_StartScan(uint8_t _ucScanMode);
static void ADS1256_Send8Bit(uint8_t _data);
void ADS1256_CfgADC(ADS1256_GAIN_E _gain, ADS1256_DRATE_E _drate);
static void ADS1256_DelayDATA(void);
static uint8_t ADS1256_Recive8Bit(void);
static void ADS1256_WriteReg(uint8_t _RegID, uint8_t _RegValue);
static uint8_t ADS1256_ReadReg(uint8_t _RegID);
static void ADS1256_WriteCmd(uint8_t _cmd);
uint8_t ADS1256_ReadChipID(void);
static void ADS1256_SetChannal(uint8_t _ch);
static void ADS1256_SetDiffChannal(uint8_t _ch);
static void ADS1256_WaitDRDY(void);
static int32_t ADS1256_ReadData(void);
//int key_interupt();
int32_t ADS1256_GetAdc(uint8_t _ch);
void ADS1256_ISR(void);
uint8_t ADS1256_Scan(void);
int data_rdy();
void myInterrupt();
void ( * call_back_func )(void);


#define LOOPBUFNUM 100
#define LOOPBUFSIZE 15512
#define LOOPBUFTOTALSIZE LOOPBUFSIZE*LOOPBUFNUM//4000HZ*4BYTE*16CH*loopnum
//unsigned char buffer[]

int init_loopbuf(APP_S * app)
{

	app->buffer.databuf_base=malloc (LOOPBUFTOTALSIZE);
	if(app->buffer.databuf_base==NULL){
		printf("init_loopbuf error\n");
		exit(0);
	}

	memset(app->buffer.databuf_base,0,LOOPBUFTOTALSIZE);
	int ifor;
	for(ifor=0;ifor<PBUFFERNUM;ifor++)
	{
		app->buffer.databuf[ifor]=(PI_LOOPBUFFER *)app->buffer.databuf_base;
		//loopbufindex
	}
	for(ifor=0;ifor<LOOPBUFNUM;ifor++)
	{
		if(app->cmd_opt.is_ad_debug){
			//printf(RED"ifor =%d,BEFORE loopbuflist=%d\n"NONE,ifor,app->buffer.databuf[LOOPBUFLIST]);
		}
		app->buffer.databuf[LOOPBUFLIST]->data_start=(DATA_FRAME_GEO_S *)((char *)app->buffer.databuf[LOOPBUFLIST]+512);
		app->buffer.databuf[LOOPBUFLIST]->loopbufnext=(char *)app->buffer.databuf_base+((ifor+1)%LOOPBUFNUM)*LOOPBUFSIZE/*this is head*/;
		app->buffer.databuf[LOOPBUFLIST]->loopbuflast=(char *)app->buffer.databuf_base+((LOOPBUFNUM+ifor-1)%LOOPBUFNUM)*LOOPBUFSIZE/*this is head*/;
		app->buffer.databuf[LOOPBUFLIST]->loopbufindex=ifor;
		if(app->cmd_opt.is_ad_debug){
			printf(BLUE"AFTER loopbuflist.last=%d\n"NONE,app->buffer.databuf[LOOPBUFLIST]->loopbuflast);	
		}
		app->buffer.databuf[LOOPBUFLIST]=app->buffer.databuf[LOOPBUFLIST]->loopbufnext;
		if(app->cmd_opt.is_ad_debug){
			printf(GREEN"AFTER loopbuflist=%d\n"NONE,app->buffer.databuf[LOOPBUFLIST]);
		}

	}
	return 0;

}

//
//#define NEXTBUF(buf) buf=







#define IWS_STEIM2_OUT_BUFNUM 100
#define IWS_STEIM2_OUT_SIZE sizeof(IWS_STEIM2_OUT)
#define IWS_STEIM2_OUT_TOTAL_SIZE IWS_STEIM2_OUT_SIZE*IWS_STEIM2_OUT_BUFNUM//4000HZ*4BYTE*16CH*loopnum
//unsigned char buffer[]

int init_steim2buf(APP_S * app)
{

	app->buffer.steim2_base=malloc (IWS_STEIM2_OUT_TOTAL_SIZE);
	if(app->buffer.steim2_base==NULL){
		printf("init_loopbuf error\n");
		exit(0);
	}

	memset(app->buffer.steim2_base,0,IWS_STEIM2_OUT_TOTAL_SIZE);
	int ifor;
	for(ifor=0;ifor<10;ifor++)
	{
		app->buffer.steim2_out_buf[ifor]=(IWS_STEIM2_OUT *)app->buffer.steim2_base;
		//loopbufindex
	}
	for(ifor=0;ifor<IWS_STEIM2_OUT_BUFNUM;ifor++)
	{
		//app->buffer.steim2_out_buf[STEIM2_NOW]=(IWS_STEIM2_OUT *)((char *)app->buffer.databuf[LOOPBUFLIST]+512);
		app->buffer.steim2_out_buf[STEIM2_NOW]->next=(char *)app->buffer.steim2_base+((ifor+1)%IWS_STEIM2_OUT_BUFNUM)*IWS_STEIM2_OUT_SIZE/*this is head*/;
		app->buffer.steim2_out_buf[STEIM2_NOW]->prev=(char *)app->buffer.steim2_base+((IWS_STEIM2_OUT_BUFNUM+ifor-1)%IWS_STEIM2_OUT_BUFNUM)*IWS_STEIM2_OUT_SIZE/*this is head*/;
		
		app->buffer.steim2_out_buf[STEIM2_NOW]->index=ifor;
		//printf(RED"ifor=%d,[STEIM2_NOW]=%d,[STEIM2_NOW]->next=%d\n"NONE,ifor,app->buffer.steim2_out_buf[STEIM2_NOW],app->buffer.steim2_out_buf[STEIM2_NOW]->next);	
		app->buffer.steim2_out_buf[STEIM2_NOW]=app->buffer.steim2_out_buf[STEIM2_NOW]->next;
	}

	return 0;

}




#define IWS_STATUS_OUT_BUFNUM 100
#define IWS_STATUS_OUT_SIZE sizeof(IWS_STATUS_OUT)
#define IWS_STATUS_OUT_TOTAL_SIZE IWS_STATUS_OUT_BUFNUM*IWS_STATUS_OUT_SIZE//4000HZ*4BYTE*16CH*loopnum
//unsigned char buffer[]

int init_status_out_buf(APP_S * app)
{

	app->buffer.status_out_base=malloc (IWS_STATUS_OUT_TOTAL_SIZE);
	if(app->buffer.status_out_base==NULL){
		printf("init_loopbuf error\n");
		exit(0);
	}

	memset(app->buffer.status_out_base,0,IWS_STATUS_OUT_TOTAL_SIZE);
	int ifor;
	for(ifor=0;ifor<10;ifor++)
	{
		app->buffer.status_out_buf[ifor]=(IWS_STATUS_OUT *)app->buffer.status_out_base;
		//loopbufindex
	}
	for(ifor=0;ifor<IWS_STATUS_OUT_BUFNUM;ifor++)
	{
		//app->buffer.steim2_out_buf[STEIM2_NOW]=(IWS_STEIM2_OUT *)((char *)app->buffer.databuf[LOOPBUFLIST]+512);
		app->buffer.status_out_buf[STATUS_NOW]->next=(char *)app->buffer.status_out_base+((ifor+1)%IWS_STATUS_OUT_BUFNUM)*IWS_STATUS_OUT_SIZE/*this is head*/;
		app->buffer.status_out_buf[STATUS_NOW]->prev=(char *)app->buffer.status_out_base+((IWS_STATUS_OUT_BUFNUM+ifor-1)%IWS_STATUS_OUT_BUFNUM)*IWS_STATUS_OUT_SIZE/*this is head*/;
		
		app->buffer.status_out_buf[STATUS_NOW]->index=ifor;
		//printf(BLUE"ifor=%d,[STATUS_NOW]=%d,[STATUS_NOW]->next=%d\n"NONE,ifor,app->buffer.status_out_buf[STATUS_NOW],app->buffer.status_out_buf[STATUS_NOW]->next);	
		app->buffer.status_out_buf[STATUS_NOW]=app->buffer.status_out_buf[STATUS_NOW]->next;
	}

	return 0;

}











//
//#define NEXTBUF(buf) buf=






















//1024000
#define FIR_BUF_NUM 2160000
#define FIR_BUF_SIZE sizeof(FIR_LIST)
FIR_LIST * fir_list_head, * fir_list,* fir_list_async,*fir_list_async_head;
int init_fir_loopbuf(APP_S * app)
{
	//printf("FIR_BUF_SIZE=%d\n",FIR_BUF_SIZE);
	//exit(0);
	unsigned char * databuf_base;
	databuf_base=malloc (FIR_BUF_NUM*FIR_BUF_SIZE);
	if(databuf_base==NULL){
		printf("init_loopbuf error\n");
		exit(0);
	}
	memset(databuf_base,0,FIR_BUF_NUM);
	int ifor;
	// for(ifor=0;ifor<FIR_BUF_NUM;ifor++)
	// {
	// 	app->buffer.databuf[ifor]=(PI_LOOPBUFFER *)app->buffer.databuf_base;
	// }
	fir_list_head=fir_list_async_head=fir_list_async=fir_list=(FIR_LIST *)databuf_base;
	for(ifor=0;ifor<FIR_BUF_NUM;ifor++)
	{
		fir_list->count=ifor;
		fir_list->next=(FIR_LIST *)(databuf_base+((ifor+1)%FIR_BUF_NUM)*FIR_BUF_SIZE);/*this is head*/;
		fir_list->last=databuf_base+((FIR_BUF_NUM+ifor-1)%FIR_BUF_NUM)*FIR_BUF_SIZE;/*this is head*/;
		//app->buffer.databuf[LOOPBUFLIST]->loopbuflast=(char *)app->buffer.databuf_base+((LOOPBUFNUM+ifor-1)%LOOPBUFNUM)*LOOPBUFSIZE/*this is head*/;
		//printf(BLUE"ifor=%d,fir_list=%d,fir_list->next=%d\n"NONE,ifor,fir_list,fir_list->next);	

		if(app->cmd_opt.is_ad_debug){
			//printf(BLUE"AFTER loopbuflist.last=%d\n"NONE,app->buffer.databuf[LOOPBUFLIST]->loopbuflast);	
		}
		fir_list=fir_list->next;
		if(app->cmd_opt.is_ad_debug){
			//printf(GREEN"AFTER loopbuflist=%d\n"NONE,app->buffer.databuf[LOOPBUFLIST]);
		}
	}
	//fir_list=fir_list_head;
	//printf(BLUE"fir_list=%d,fir_list->next=%d,fir_list->last=%d,fir_list->count=%d\n"NONE,fir_list,fir_list->next,fir_list->last,fir_list->count);	

	return 0;

}



// //1024000
// #define FIR_BUF_NUM 2160000
// #define FIR_BUF_SIZE sizeof(FIR_LIST)
// FIR_LIST * fir_list_head, * fir_list;
// int init_fir_loopbuf(APP_S * app)
// {
// 	//printf("FIR_BUF_SIZE=%d\n",FIR_BUF_SIZE);
// 	//exit(0);
// 	unsigned char * databuf_base;
// 	databuf_base=malloc (FIR_BUF_NUM*FIR_BUF_SIZE);
// 	if(databuf_base==NULL){
// 		printf("init_loopbuf error\n");
// 		exit(0);
// 	}
// 	memset(databuf_base,0,FIR_BUF_NUM);
// 	int ifor;
// 	// for(ifor=0;ifor<FIR_BUF_NUM;ifor++)
// 	// {
// 	// 	app->buffer.databuf[ifor]=(PI_LOOPBUFFER *)app->buffer.databuf_base;
// 	// }
// 	fir_list_head=fir_list=(FIR_LIST *)databuf_base;
// 	for(ifor=0;ifor<FIR_BUF_NUM;ifor++)
// 	{
// 		fir_list->count=ifor;
// 		fir_list->next=(FIR_LIST *)(databuf_base+((ifor+1)%FIR_BUF_NUM)*FIR_BUF_SIZE);/*this is head*/;
// 		fir_list->last=databuf_base+((FIR_BUF_NUM+ifor-1)%FIR_BUF_NUM)*FIR_BUF_SIZE;/*this is head*/;
// 		//app->buffer.databuf[LOOPBUFLIST]->loopbuflast=(char *)app->buffer.databuf_base+((LOOPBUFNUM+ifor-1)%LOOPBUFNUM)*LOOPBUFSIZE/*this is head*/;
// 		printf(BLUE"ifor=%d,fir_list=%d,fir_list->next=%d\n"NONE,ifor,fir_list,fir_list->next);	

// 		if(app->cmd_opt.is_ad_debug){
// 			//printf(BLUE"AFTER loopbuflist.last=%d\n"NONE,app->buffer.databuf[LOOPBUFLIST]->loopbuflast);	
// 		}
// 		fir_list=fir_list->next;
// 		if(app->cmd_opt.is_ad_debug){
// 			//printf(GREEN"AFTER loopbuflist=%d\n"NONE,app->buffer.databuf[LOOPBUFLIST]);
// 		}
// 	}
// 	//fir_list=fir_list_head;
// 	//printf(BLUE"fir_list=%d,fir_list->next=%d,fir_list->last=%d,fir_list->count=%d\n"NONE,fir_list,fir_list->next,fir_list->last,fir_list->count);	

// 	return 0;

// }















#define FIR_BUF_NUM1 1000
#define FIR_BUF_SIZE1 sizeof(FIR_LIST1)
FIR_LIST1 * fir_list1_head, * fir_list1,* fir_list1_async;
int init_fir_loopbuf1(APP_S * app)
{
	//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	//printf("FIR_BUF_SIZE=%d\n",FIR_BUF_SIZE);
	//exit(0);
	unsigned char * databuf_base;
	databuf_base=malloc (FIR_BUF_NUM1*FIR_BUF_SIZE1);
	if(databuf_base==NULL){
		printf("init_loopbuf error\n");
		exit(0);
	}
//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	memset(databuf_base,0,FIR_BUF_NUM1);
	int ifor;
	// for(ifor=0;ifor<FIR_BUF_NUM;ifor++)
	// {
	// 	app->buffer.databuf[ifor]=(PI_LOOPBUFFER *)app->buffer.databuf_base;
	// }
	fir_list1_head=fir_list1_async=fir_list1=(FIR_LIST1 *)databuf_base;
	for(ifor=0;ifor<FIR_BUF_NUM1;ifor++)
	{
		fir_list1->count=ifor;
		fir_list1->next=(FIR_LIST1 *)(databuf_base+((ifor+1)%FIR_BUF_NUM1)*FIR_BUF_SIZE1);/*this is head*/;
		fir_list1->last=databuf_base+((FIR_BUF_NUM1+ifor-1)%FIR_BUF_NUM1)*FIR_BUF_SIZE1;/*this is head*/;
		//app->buffer.databuf[LOOPBUFLIST]->loopbuflast=(char *)app->buffer.databuf_base+((LOOPBUFNUM+ifor-1)%LOOPBUFNUM)*LOOPBUFSIZE/*this is head*/;
		//printf(BLUE"fir_list=%d,fir_list->next=%d\n"NONE,fir_list,fir_list->next);	

		if(app->cmd_opt.is_ad_debug){
			//printf(BLUE"AFTER loopbuflist.last=%d\n"NONE,app->buffer.databuf[LOOPBUFLIST]->loopbuflast);	
		}
		fir_list1=fir_list1->next;
		if(app->cmd_opt.is_ad_debug){
			//printf(GREEN"AFTER loopbuflist=%d\n"NONE,app->buffer.databuf[LOOPBUFLIST]);
		}
	}
	//fir_list=fir_list_head;
	//printf(BLUE"fir_list=%d,fir_list->next=%d,fir_list->last=%d,fir_list->count=%d\n"NONE,fir_list,fir_list->next,fir_list->last,fir_list->count);	
//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	return 0;

}
















void  bsp_DelayUS(uint64_t micros)
{
		bcm2835_delayMicroseconds (micros);
}


/*
*********************************************************************************************************
*	name: bsp_InitADS1256
*	function: Configuration of the STM32 GPIO and SPI interface£¬The connection ADS1256
*	parameter: NULL
*	The return value: NULL
*********************************************************************************************************
*/


void bsp_InitADS1256(void)
{
#ifdef SOFT_SPI
	CS_1();
	SCK_0();
	DI_0();
#endif

//ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_1000SPS);	/* ÅäÖÃADC²ÎÊý£º ÔöÒæ1:1, Êý¾ÝÊä³öËÙÂÊ 1KHz */
}




/*
*********************************************************************************************************
*	name: ADS1256_StartScan
*	function: Configuration DRDY PIN for external interrupt is triggered
*	parameter: _ucDiffMode : 0  Single-ended input  8 channel£¬ 1 Differential input  4 channe
*	The return value: NULL
*********************************************************************************************************
*/
void ADS1256_StartScan(uint8_t _ucScanMode)
{
	g_tADS1256.ScanMode = _ucScanMode;
	/* ¿ªÊ¼É¨ÃèÇ°, ÇåÁã½á¹û»º³åÇø */
	{
		uint8_t i;

		g_tADS1256.Channel = 0;

		for (i = 0; i < 8; i++)
		{
			g_tADS1256.AdcNow[i] = 0;
		}
	}

}

/*
*********************************************************************************************************
*	name: ADS1256_Send8Bit
*	function: SPI bus to send 8 bit data
*	parameter: _data:  data
*	The return value: NULL
*********************************************************************************************************
*/
static void ADS1256_Send8Bit(uint8_t _data)
{

	bsp_DelayUS(2);
	bcm2835_spi_transfer(_data);
}

/*
*********************************************************************************************************
*	name: ADS1256_CfgADC
*	function: The configuration parameters of ADC, gain and data rate
*	parameter: _gain:gain 1-64
*                      _drate:  data  rate
*	The return value: NULL
*********************************************************************************************************
*/
void ADS1256_CfgADC(ADS1256_GAIN_E _gain, ADS1256_DRATE_E _drate)
{
	g_tADS1256.Gain = _gain;
	g_tADS1256.DataRate = _drate;

	ADS1256_WaitDRDY();

	{
		uint8_t buf[4];		/* Storage ads1256 register configuration parameters */

		/*Status register define
			Bits 7-4 ID3, ID2, ID1, ID0  Factory Programmed Identification Bits (Read Only)

			Bit 3 ORDER: Data Output Bit Order
				0 = Most Significant Bit First (default)
				1 = Least Significant Bit First
			Input data  is always shifted in most significant byte and bit first. Output data is always shifted out most significant
			byte first. The ORDER bit only controls the bit order of the output data within the byte.

			Bit 2 ACAL : Auto-Calibration
				0 = Auto-Calibration Disabled (default)
				1 = Auto-Calibration Enabled
			When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
			the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
			values.

			Bit 1 BUFEN: Analog Input Buffer Enable
				0 = Buffer Disabled (default)
				1 = Buffer Enabled

			Bit 0 DRDY :  Data Ready (Read Only)
				This bit duplicates the state of the DRDY pin.

			ACAL=1  enable  calibration
		*/
		//buf[0] = (0 << 3) | (1 << 2) | (1 << 1);//enable the internal buffer
        buf[0] = (0 << 3) | (1 << 2) | (0 << 1);  // The internal buffer is prohibited

        //ADS1256_WriteReg(REG_STATUS, (0 << 3) | (1 << 2) | (1 << 1));

		buf[1] = 0x08;	

		/*	ADCON: A/D Control Register (Address 02h)
			Bit 7 Reserved, always 0 (Read Only)
			Bits 6-5 CLK1, CLK0 : D0/CLKOUT Clock Out Rate Setting
				00 = Clock Out OFF
				01 = Clock Out Frequency = fCLKIN (default)
				10 = Clock Out Frequency = fCLKIN/2
				11 = Clock Out Frequency = fCLKIN/4
				When not using CLKOUT, it is recommended that it be turned off. These bits can only be reset using the RESET pin.

			Bits 4-3 SDCS1, SCDS0: Sensor Detect Current Sources
				00 = Sensor Detect OFF (default)
				01 = Sensor Detect Current = 0.5 ¦Ì A
				10 = Sensor Detect Current = 2 ¦Ì A
				11 = Sensor Detect Current = 10¦Ì A
				The Sensor Detect Current Sources can be activated to verify  the integrity of an external sensor supplying a signal to the
				ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.

			Bits 2-0 PGA2, PGA1, PGA0: Programmable Gain Amplifier Setting
				000 = 1 (default)
				001 = 2
				010 = 4
				011 = 8
				100 = 16
				101 = 32
				110 = 64
				111 = 64
		*/
		buf[2] = (0 << 5) | (0 << 3) | (_gain << 0);
		//ADS1256_WriteReg(REG_ADCON, (0 << 5) | (0 << 2) | (GAIN_1 << 1));	/*choose 1: gain 1 ;input 5V/
		buf[3] = s_tabDataRate[_drate];	// DRATE_10SPS;	

		CS_0();	/* SPIÆ¬Ñ¡ = 0 */
		ADS1256_Send8Bit(CMD_WREG | 0);	/* Write command register, send the register address */
		ADS1256_Send8Bit(0x03);			/* Register number 4,Initialize the number  -1*/

		ADS1256_Send8Bit(buf[0]);	/* Set the status register */
		ADS1256_Send8Bit(buf[1]);	/* Set the input channel parameters */
		ADS1256_Send8Bit(buf[2]);	/* Set the ADCON control register,gain */
		ADS1256_Send8Bit(buf[3]);	/* Set the output rate */

		CS_1();	/* SPI  cs = 1 */
	}

	bsp_DelayUS(50);
}



/*
*********************************************************************************************************
*	name: ADS1256_config
*	function: The configuration parameters of ADC, gain and data rate
*	parameter: _gain:gain 1-64
*                      _drate:  data  rate
*	The return value: NULL
*********************************************************************************************************
*/
void ADS1256_config()
{
	int ifor;
	ADS1256_WaitDRDY();

	{
		uint8_t buf[4];		/* Storage ads1256 register configuration parameters */

		/*Status register define
			Bits 7-4 ID3, ID2, ID1, ID0  Factory Programmed Identification Bits (Read Only)

			Bit 3 ORDER: Data Output Bit Order
				0 = Most Significant Bit First (default)
				1 = Least Significant Bit First
			Input data  is always shifted in most significant byte and bit first. Output data is always shifted out most significant
			byte first. The ORDER bit only controls the bit order of the output data within the byte.

			Bit 2 ACAL : Auto-Calibration
				0 = Auto-Calibration Disabled (default)
				1 = Auto-Calibration Enabled
			When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
			the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
			values.

			Bit 1 BUFEN: Analog Input Buffer Enable
				0 = Buffer Disabled (default)
				1 = Buffer Enabled

			Bit 0 DRDY :  Data Ready (Read Only)
				This bit duplicates the state of the DRDY pin.

			ACAL=1  enable  calibration
		*/
		//buf[0] = (0 << 3) | (1 << 2) | (1 << 1);//enable the internal buffer
        //buf[0] = (0 << 3) | (1 << 2) | (0 << 1);  // The internal buffer is prohibited

        //ADS1256_WriteReg(REG_STATUS, (0 << 3) | (1 << 2) | (1 << 1));

		//buf[1] = 0x08;	

		/*	ADCON: A/D Control Register (Address 02h)
			Bit 7 Reserved, always 0 (Read Only)
			Bits 6-5 CLK1, CLK0 : D0/CLKOUT Clock Out Rate Setting
				00 = Clock Out OFF
				01 = Clock Out Frequency = fCLKIN (default)
				10 = Clock Out Frequency = fCLKIN/2
				11 = Clock Out Frequency = fCLKIN/4
				When not using CLKOUT, it is recommended that it be turned off. These bits can only be reset using the RESET pin.

			Bits 4-3 SDCS1, SCDS0: Sensor Detect Current Sources
				00 = Sensor Detect OFF (default)
				01 = Sensor Detect Current = 0.5 ¦Ì A
				10 = Sensor Detect Current = 2 ¦Ì A
				11 = Sensor Detect Current = 10¦Ì A
				The Sensor Detect Current Sources can be activated to verify  the integrity of an external sensor supplying a signal to the
				ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.

			Bits 2-0 PGA2, PGA1, PGA0: Programmable Gain Amplifier Setting
				000 = 1 (default)
				001 = 2
				010 = 4
				011 = 8
				100 = 16
				101 = 32
				110 = 64
				111 = 64
		*/
		//buf[2] = (0 << 5) | (0 << 3) | (_gain << 0);
		//ADS1256_WriteReg(REG_ADCON, (0 << 5) | (0 << 2) | (GAIN_1 << 1));	/*choose 1: gain 1 ;input 5V/
		//buf[3] = s_tabDataRate[_drate];	// DRATE_10SPS;	

		CS_0();	/* SPIÆ¬Ñ¡ = 0 */
		ADS1256_Send8Bit(CMD_WREG | 0);	/* Write command register, send the register address */
		ADS1256_Send8Bit(0x03);			/* Register number 4,Initialize the number  -1*/
		for(ifor=0;ifor<11;ifor++)
		{
			printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);

			ADS1256_Send8Bit(app.app_para.ad_reg.reg[ifor]);	/* Set the status register */
		}
		CS_1();	/* SPI  cs = 1 */
	}

	bsp_DelayUS(50);
}








/*
*********************************************************************************************************
*	name: ADS1256_DelayDATA
*	function: delay
*	parameter: NULL
*	The return value: NULL
*********************************************************************************************************
*/
static void ADS1256_DelayDATA(void)
{
	/*
		Delay from last SCLK edge for DIN to first SCLK rising edge for DOUT: RDATA, RDATAC,RREG Commands
		min  50   CLK = 50 * 0.13uS = 6.5uS
	*/
	bsp_DelayUS(5);	/* The minimum time delay 6.5us */
}




/*
*********************************************************************************************************
*	name: ADS1256_Recive8Bit
*	function: SPI bus receive function
*	parameter: NULL
*	The return value: NULL
*********************************************************************************************************
*/
static uint8_t ADS1256_Recive8Bit(void)
{
	uint8_t read = 0;
	read = bcm2835_spi_transfer(0xff);
	return read;
}


static int ADS1256_Recivenb(char *rbuf,int n)
{
	//uint8_t read = 0;

	bcm2835_spi_transfernb(tbuf,rbuf,n);
	//read = bcm2835_spi_transfer(0xff);
	return 0;
}











/*
*********************************************************************************************************
*	name: ADS1256_WriteReg
*	function: Write the corresponding register
*	parameter: _RegID: register  ID
*			 _RegValue: register Value
*	The return value: NULL
*********************************************************************************************************
*/
static void ADS1256_WriteReg(uint8_t _RegID, uint8_t _RegValue)
{
	CS_0();	/* SPI  cs  = 0 */
	ADS1256_Send8Bit(CMD_WREG | _RegID);	/*Write command register */
	ADS1256_Send8Bit(0x00);		/*Write the register number */
	ADS1256_Send8Bit(_RegValue);	/*send register value */
	CS_1();	/* SPI   cs = 1 */
}

/*
*********************************************************************************************************
*	name: ADS1256_ReadReg
*	function: Read  the corresponding register
*	parameter: _RegID: register  ID
*	The return value: read register value
*********************************************************************************************************
*/
static uint8_t ADS1256_ReadReg(uint8_t _RegID)
{
	uint8_t read;

	CS_0();	/* SPI  cs  = 0 */
	ADS1256_Send8Bit(CMD_RREG | _RegID);	/* Write command register */
	ADS1256_Send8Bit(0x00);	/* Write the register number */

	ADS1256_DelayDATA();	/*delay time */

	read = ADS1256_Recive8Bit();	/* Read the register values */
	CS_1();	/* SPI   cs  = 1 */

	return read;
}

/*
*********************************************************************************************************
*	name: ADS1256_WriteCmd
*	function: Sending a single byte order
*	parameter: _cmd : command
*	The return value: NULL
*********************************************************************************************************
*/
static void ADS1256_WriteCmd(uint8_t _cmd)
{
	CS_0();	/* SPI   cs = 0 */
	ADS1256_Send8Bit(_cmd);
	CS_1();	/* SPI  cs  = 1 */
}

/*
*********************************************************************************************************
*	name: ADS1256_ReadChipID
*	function: Read the chip ID
*	parameter: _cmd : NULL
*	The return value: four high status register
*********************************************************************************************************
*/
uint8_t ADS1256_ReadChipID(void)
{
	uint8_t id;

	ADS1256_WaitDRDY();
	id = ADS1256_ReadReg(REG_STATUS);
	return (id >> 4);
}


/*
*********************************************************************************************************
*	name: ADS1256_ReadChipID
*	function: Read the chip ID
*	parameter: _cmd : NULL
*	The return value: four high status register
*********************************************************************************************************
*/
uint8_t ADS1256_ReadChipID_ISR(void)
{
	uint8_t id;
	id = ADS1256_ReadReg(REG_STATUS);
		
	if ((id >> 4) != 3)
	{
		printf(YELLOW"Error, ASD1256 Chip ID = 0x%d\r\n"NONE, (int)id);
	}
	else
	{
		//call_back_func=myInterrupt;
		app.ad1256.run_ctrl.has_read_id=1;
		printf(YELLOW"Ok, ASD1256 Chip ID = 0x%d\r\n"NONE, (int)(id >> 4));
		//exit(0);
		return (id >> 4);
	}
	
}





/*
*********************************************************************************************************
*	name: ADS1256_SetChannal
*	function: Configuration channel number
*	parameter:  _ch:  channel number  0--7
*	The return value: NULL
*********************************************************************************************************
*/
static void ADS1256_SetChannal(uint8_t _ch)
{
	/*
	Bits 7-4 PSEL3, PSEL2, PSEL1, PSEL0: Positive Input Channel (AINP) Select
		0000 = AIN0 (default)
		0001 = AIN1
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are ¡°don¡¯t care¡±)

		NOTE: When using an ADS1255 make sure to only select the available inputs.

	Bits 3-0 NSEL3, NSEL2, NSEL1, NSEL0: Negative Input Channel (AINN)Select
		0000 = AIN0
		0001 = AIN1 (default)
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are ¡°don¡¯t care¡±)
	*/
	if (_ch > 7)
	{
		return;
	}
	ADS1256_WriteReg(REG_MUX, (_ch << 4) | (1 << 3));	/* Bit3 = 1, AINN connection AINCOM */
}




static void ADS1256_SetChannal_zwp(uint8_t _ch)
{
	/*
	Bits 7-4 PSEL3, PSEL2, PSEL1, PSEL0: Positive Input Channel (AINP) Select
		0000 = AIN0 (default)
		0001 = AIN1
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are ¡°don¡¯t care¡±)

		NOTE: When using an ADS1255 make sure to only select the available inputs.

	Bits 3-0 NSEL3, NSEL2, NSEL1, NSEL0: Negative Input Channel (AINN)Select
		0000 = AIN0
		0001 = AIN1 (default)
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are ¡°don¡¯t care¡±)
	*/
	ADS1256_WriteReg(REG_MUX, _ch);	/* Bit3 = 1, AINN connection AINCOM */
}







/*
*********************************************************************************************************
*	name: ADS1256_SetDiffChannal
*	function: The configuration difference channel
*	parameter:  _ch:  channel number  0--3
*	The return value:  four high status register
*********************************************************************************************************
*/
static void ADS1256_SetDiffChannal(uint8_t _ch)
{
	/*
	Bits 7-4 PSEL3, PSEL2, PSEL1, PSEL0: Positive Input Channel (AINP) Select
		0000 = AIN0 (default)
		0001 = AIN1
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are ¡°don¡¯t care¡±)

		NOTE: When using an ADS1255 make sure to only select the available inputs.

	Bits 3-0 NSEL3, NSEL2, NSEL1, NSEL0: Negative Input Channel (AINN)Select
		0000 = AIN0
		0001 = AIN1 (default)
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are ¡°don¡¯t care¡±)
	*/
	if (_ch == 0)
	{
		ADS1256_WriteReg(REG_MUX, (0 << 4) | 1);	/* DiffChannal  AIN0£¬ AIN1 */
	}
	else if (_ch == 1)
	{
		ADS1256_WriteReg(REG_MUX, (2 << 4) | 3);	/*DiffChannal   AIN2£¬ AIN3 */
	}
	else if (_ch == 2)
	{
		ADS1256_WriteReg(REG_MUX, (4 << 4) | 5);	/*DiffChannal    AIN4£¬ AIN5 */
	}
	else if (_ch == 3)
	{
		ADS1256_WriteReg(REG_MUX, (6 << 4) | 7);	/*DiffChannal   AIN6£¬ AIN7 */
	}
}

/*
*********************************************************************************************************
*	name: ADS1256_WaitDRDY
*	function: delay time  wait for automatic calibration
*	parameter:  NULL
*	The return value:  NULL
*********************************************************************************************************
*/
static void ADS1256_WaitDRDY(void)
{
	uint32_t i;

	for (i = 0; i < 400000; i++)
	{
		if (DRDY_IS_LOW())
		{
			break;
		}
	}
	if (i >= 400000)
	{
		printf("ADS1256_WaitDRDY() Time Out ...\r\n");		
	}
}

/*
*********************************************************************************************************
*	name: ADS1256_ReadData
*	function: read ADC value
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/
static int32_t ADS1256_ReadData(void)
{
	uint32_t read = 0;
    static uint8_t buf[3];

	CS_0();	/* SPI   cs = 0 */

	ADS1256_Send8Bit(CMD_RDATA);	/* read ADC command  */

	ADS1256_DelayDATA();	/*delay time  */

	/*Read the sample results 24bit*/
    buf[0] = ADS1256_Recive8Bit();
    buf[1] = ADS1256_Recive8Bit();
    buf[2] = ADS1256_Recive8Bit();

    read = ((uint32_t)buf[0] << 16) & 0x00FF0000;
    read |= ((uint32_t)buf[1] << 8);  /* Pay attention to It is wrong   read |= (buf[1] << 8) */
    read |= buf[2];

	CS_1();	/* SPIÆ¬Ñ¡ = 1 */

	/* Extend a signed number*/
    if (read & 0x800000)
    {
	    read |= 0xFF000000;
    }

	return (int32_t)read;
}


/*
*********************************************************************************************************
*	name: ADS1256_ReadData
*	function: read ADC value
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/
static int32_t ADS1256_ReadData_spic(void)
{
	uint32_t read = 0;
    static uint8_t buf[3];

	CS_0();	/* SPI   cs = 0 */

	ADS1256_Send8Bit(CMD_RDATA);	/* read ADC command  */

	ADS1256_DelayDATA();	/*delay time  */

	/*Read the sample results 24bit*/
    ADS1256_Recivenb(buf,3);

    read = ((uint32_t)buf[0] << 16) & 0x00FF0000;
    read |= ((uint32_t)buf[1] << 8);  /* Pay attention to It is wrong   read |= (buf[1] << 8) */
    read |= buf[2];

	CS_1();	/* SPIÆ¬Ñ¡ = 1 */

	/* Extend a signed number*/
    if (read & 0x800000)
    {
	    read |= 0xFF000000;
    }

	return (int32_t)read;
}








/*
*********************************************************************************************************
*	name: ADS1256_ReadData
*	function: read ADC value
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/
static int32_t ADS1256_ReadDataC(void)
{
	uint32_t read = 0;
    static uint8_t buf[3];

	CS_0();	/* SPI   cs = 0 */
	/*Read the sample results 24bit*/
    buf[0] = ADS1256_Recive8Bit();
    buf[1] = ADS1256_Recive8Bit();
    buf[2] = ADS1256_Recive8Bit();

    read = ((uint32_t)buf[0] << 16) & 0x00FF0000;
    read |= ((uint32_t)buf[1] << 8);  /* Pay attention to It is wrong   read |= (buf[1] << 8) */
    read |= buf[2];

	CS_1();	/* SPIÆ¬Ñ¡ = 1 */

	/* Extend a signed number*/
    if (read & 0x800000)
    {
	    read |= 0xFF000000;
    }

	return (int32_t)read;
}










/*
*********************************************************************************************************
*	name: ADS1256_GetAdc
*	function: read ADC value
*	parameter:  channel number 0--7
*	The return value:  ADC vaule (signed number)
*********************************************************************************************************
*/
int32_t ADS1256_GetAdc(uint8_t _ch)
{
	int32_t iTemp;

	if (_ch > 7)
	{
		return 0;
	}

	iTemp = g_tADS1256.AdcNow[_ch];

	return iTemp;
}

/*
*********************************************************************************************************
*	name: ADS1256_ISR
*	function: Collection procedures
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/

void ADS1256_ISR(void)
{
	//printf("inside function：%s,line : %d,g_tADS1256.ScanMode=%d\n",__FUNCTION__,__LINE__,g_tADS1256.ScanMode);
	if (g_tADS1256.ScanMode == 0)	/*  0  Single-ended input  8 channel£¬ 1 Differential input  4 channe */
	{

		ADS1256_SetChannal(g_tADS1256.Channel);	/*Switch channel mode */
		//printf("inside function：%s,line : %d,g_tADS1256.Channel=%d\n",__FUNCTION__,__LINE__,g_tADS1256.Channel);

		bsp_DelayUS(5);

		ADS1256_WriteCmd(CMD_SYNC);
		bsp_DelayUS(5);

		ADS1256_WriteCmd(CMD_WAKEUP);
		bsp_DelayUS(25);

		if (g_tADS1256.Channel == 0)
		{
			g_tADS1256.AdcNow[7] = ADS1256_ReadData();
			//printf("inside function：%s,line : %d,g_tADS1256.AdcNow[7]=%d\n",__FUNCTION__,__LINE__,g_tADS1256.AdcNow[7]);

		}
		else
		{
			//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);

			g_tADS1256.AdcNow[g_tADS1256.Channel-1] = ADS1256_ReadData();	
		}

		if (++g_tADS1256.Channel >= 1)
		{
			g_tADS1256.Channel = 0;
		}
	}
	else	/*DiffChannal*/
	{
			//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
	
		ADS1256_SetDiffChannal(g_tADS1256.Channel);	/* change DiffChannal */
		// bsp_DelayUS(5);

		// ADS1256_WriteCmd(CMD_SYNC);
		// bsp_DelayUS(5);

		// ADS1256_WriteCmd(CMD_WAKEUP);
		// bsp_DelayUS(25);

		if (g_tADS1256.Channel == 0)
		{
			g_tADS1256.AdcNow[3] = ADS1256_ReadData();	
			//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);

		}
		else
		{
						//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);

			g_tADS1256.AdcNow[g_tADS1256.Channel-1] = ADS1256_ReadData();	
		}

		if (++g_tADS1256.Channel >= 4)
		{
						//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);

			g_tADS1256.Channel = 0;
		}
	}
}




/*
*********************************************************************************************************
*	name: ADS1256_ISR
*	function: Collection procedures
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/


void ADS1256_ISR_zwp(void)
{
	//printf("inside function：%s,line : %d,g_tADS1256.ScanMode=%d\n",__FUNCTION__,__LINE__,g_tADS1256.ScanMode);
	if (app.ad1256.g_tADS1256.ScanMode == 0)	/*  0  Single-ended input  8 channel£¬ 1 Differential input  4 channe */
	{

		//ADS1256_SetChannal_zwp(app.ad1256.g_tADS1256.Channel);
		ADS1256_SetChannal(app.ad1256.g_tADS1256.Channel);	/*Switch channel mode */
		//printf("inside function：%s,line : %d,g_tADS1256.Channel=%d\n",__FUNCTION__,__LINE__,g_tADS1256.Channel);
		if (app.ad1256.g_tADS1256.Channel == 0)
		{
			app.buffer.databuf[LOOPREAD]->data_start[app.ad1256.ad_count.read_times%200].ad_data = ADS1256_ReadData();
			//printf("inside function：%s,line : %d,g_tADS1256.AdcNow[7]=%d\n",__FUNCTION__,__LINE__,g_tADS1256.AdcNow[7]);
		}
		else
		{
			//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
			app.buffer.databuf[LOOPREAD]->data_start[app.ad1256.ad_count.read_times%200].ad_data = ADS1256_ReadData();	
		}
		if (++app.ad1256.g_tADS1256.Channel >= 7)
		{
			app.ad1256.g_tADS1256.Channel = 0;
		}
		printf("inside function：%s,line : %d,ad=%d\n",__FUNCTION__,__LINE__,app.buffer.databuf[LOOPREAD]->data_start[app.ad1256.ad_count.read_times%200].ad_data);
	}
	else	/*DiffChannal*/
	{
		//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
	
		ADS1256_SetDiffChannal(app.ad1256.g_tADS1256.Channel);	/* change DiffChannal */
		//bsp_DelayUS(5);

		//ADS1256_WriteCmd(CMD_SYNC);
		//bsp_DelayUS(5);

		//ADS1256_WriteCmd(CMD_WAKEUP);
		//bsp_DelayUS(25);

		if (app.ad1256.g_tADS1256.Channel == 0)
		{
			app.buffer.databuf[LOOPREAD]->data_start[app.ad1256.ad_count.read_times%200].ad_data = ADS1256_ReadData();	
			//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);

		}
		else
		{
						//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);

			app.buffer.databuf[LOOPREAD]->data_start[app.ad1256.ad_count.read_times%200].ad_data = ADS1256_ReadData();	
		}

		if (++app.ad1256.g_tADS1256.Channel >= 4)
		{
						//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);

			app.ad1256.g_tADS1256.Channel = 0;
		}
		//printf(RED"inside function：%s,line : %d,ad=%d\n",__FUNCTION__,__LINE__,app.buffer.databuf[LOOPREAD]->data_start[app.ad1256.ad_count.read_times%200].ad_data);

	}
}




/*
*********************************************************************************************************
*	name: ADS1256_Setch_ReadData
*	function: Collection procedures
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/

int ADS1256_Setch_ReadData(int ch)
{
	//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	unsigned char aa=app.app_para.ad_reg.reg[12+ch];
	ADS1256_WriteReg(REG_MUX,aa);
	return  ADS1256_ReadData();
}













/*
*********************************************************************************************************
*	name: ADS1256_ISR
*	function: Collection procedures
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/

void ADS1256_change_channel(void)
{
	//printf("inside function：%s,line : %d,g_tADS1256.ScanMode=%d\n",__FUNCTION__,__LINE__,g_tADS1256.ScanMode);
	if (g_tADS1256.ScanMode == 0)	/*  0  Single-ended input  8 channel£¬ 1 Differential input  4 channe */
	{

		ADS1256_SetChannal(g_tADS1256.Channel);	/*Switch channel mode */
		//printf("inside function：%s,line : %d,g_tADS1256.Channel=%d\n",__FUNCTION__,__LINE__,g_tADS1256.Channel);
		if (g_tADS1256.Channel == 0)
		{
			g_tADS1256.AdcNow[7] = ADS1256_ReadData();
			//printf("inside function：%s,line : %d,g_tADS1256.AdcNow[7]=%d\n",__FUNCTION__,__LINE__,g_tADS1256.AdcNow[7]);

		}
		else
		{
			//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);

			g_tADS1256.AdcNow[g_tADS1256.Channel-1] = ADS1256_ReadData();	
		}
		if (++g_tADS1256.Channel >= 1)
		{
			g_tADS1256.Channel = 0;
		}
	}
	else	/*DiffChannal*/
	{
			//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
	
		ADS1256_SetDiffChannal(g_tADS1256.Channel);	/* change DiffChannal */
		bsp_DelayUS(5);

		ADS1256_WriteCmd(CMD_SYNC);
		bsp_DelayUS(5);

		ADS1256_WriteCmd(CMD_WAKEUP);
		bsp_DelayUS(25);

		if (g_tADS1256.Channel == 0)
		{
			g_tADS1256.AdcNow[3] = ADS1256_ReadData();	
			//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);

		}
		else
		{
						//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);

			g_tADS1256.AdcNow[g_tADS1256.Channel-1] = ADS1256_ReadData();	
		}

		if (++g_tADS1256.Channel >= 4)
		{
						//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);

			g_tADS1256.Channel = 0;
		}
	}
}















/*
*********************************************************************************************************
*	name: ADS1256_Scan
*	function: 
*	parameter:NULL
*	The return value:  1
*********************************************************************************************************
*/
uint8_t ADS1256_Scan(void)
{
	if (DRDY_IS_LOW())
	{
		ADS1256_ISR();
		return 1;
	}

	return 0;
}

/*
*********************************************************************************************************
*	name: ADS1256_Scan
*	function: 
*	parameter:NULL
*	The return value:  1
*********************************************************************************************************
*/
uint8_t ADS1256_Scan_rdy(void)
{
	if (DRDY_IS_LOW())
	{
		//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
		ADS1256_WriteCmd(CMD_RDATAC);
		//while(1);
		//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
		while(DRDY_IS_LOW()){
			//printf("inside function：%s,line : %d\n",__FUNCTION__,__LINE__);
			g_tADS1256.AdcNow[7] = ADS1256_ReadDataC();
		}
		return 0;

	}

	return 1;
}








/*
*********************************************************************************************************
*	name: Write_DAC8552
*	function:  DAC send data 
*	parameter: channel : output channel number 
*			   data : output DAC value 
*	The return value:  NULL
*********************************************************************************************************
*/
void Write_DAC8552(uint8_t channel, uint16_t Data)
{
	uint8_t i;

	 CS_1() ;
	 CS_0() ;
      bcm2835_spi_transfer(channel);
      bcm2835_spi_transfer((Data>>8));
      bcm2835_spi_transfer((Data&0xff));  
      CS_1() ;
}
/*
*********************************************************************************************************
*	name: Voltage_Convert
*	function:  Voltage value conversion function
*	parameter: Vref : The reference voltage 3.3V or 5V
*			   voltage : output DAC value 
*	The return value:  NULL
*********************************************************************************************************
*/
uint16_t Voltage_Convert(float Vref, float voltage)
{
	uint16_t _D_;
	_D_ = (uint16_t)(65536 * voltage / Vref);
    
	return _D_;
}

/*
*********************************************************************************************************
*	name: main
*	function:  
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/

int debug_stop()
{
	while(1)
   {
   		bsp_DelayUS(1000000);
   }
}
FILE * fp;









void get_hight_Pri(int cpux){
    cpu_set_t mask;
    CPU_ZERO(&mask);
    //cpux=cpux>1?cpux-2:cpux;
    CPU_SET(cpux,&mask);
	//CPU_SET(2,&mask);
	printf("CPU_COUNT(cpu_set_t *set)=%d\n",CPU_COUNT(&mask));
    if (sched_setaffinity(0,sizeof(mask),&mask)==-1){
    	printf("affinity set fail!");
    	exit(0);
    }
            
	if(piHiPri (99)){
		printf("piHiPri error piHiPri=%d\n",piHiPri (50));
        exit(0);
    }
}
static inline int read_data_setch_interupt_async(int tid);

int scan_read(int * cpux)
{
	
	printf(LIGHT_GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	if(wiringPiSetup() < 0)return 1;
	get_hight_Pri(3-((*cpux)%3));
	int tid=pthread_self();
	int ret;
	while (1){
		//piLock(0);
		//LOCAT();
        pthread_mutex_lock(&(app.app_mut[19]));

		while(DRDY_IS_LOW())
		{
			LOCAT();

			//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
			ret=read_data_setch_interupt_async(tid);
			while(DRDY_IS_LOW())
			{
			}
			//exit(0);
		}

		//bsp_DelayUS(50);
		//piUnlock(0);
		pthread_mutex_unlock(&(app.app_mut[19]));

	}
}

int reset_1256()
{
	bcm2835_gpio_fsel(RST, BCM2835_GPIO_FSEL_OUTP);//
    //bcm2835_gpio_fsel(RST, BCM2835_GPIO_FSEL_OUTP);//
	// RST_1();
	// bsp_DelayUS(100);
	// RST_0();
	// bsp_DelayUS(100);
	// RST_1();
	return 0;
}


int ad_spi_config()
{
	printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256); // The default
    bcm2835_gpio_fsel(SPICS, BCM2835_GPIO_FSEL_OUTP);//
    bcm2835_gpio_write(SPICS, HIGH);
    /*
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_2); 
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
    */
	printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

int ad_gpio_config()
{
	bcm2835_gpio_fsel(PWDN,       BCM2835_GPIO_FSEL_OUTP);
	PWDN_IS_HIGH();
	printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
    return 0;
}











int set_relay(APP_S * app)
{

}


FILE * fp_aaaa;



int  main_ad(APP_S * app)
{



	IWS_STATUS_OUT *pp=app->buffer.status_out_buf[STATUS_NOW];
	//app->
	int jfor;
	for(jfor=0;jfor<100;jfor++)
	{
		//app->buffer.steim2_out_buf[STEIM2_NOW]=(IWS_STEIM2_OUT *)((char *)app->buffer.databuf[LOOPBUFLIST]+512);
		//printf(BLUE"ifor=%d,[pp]=%d,[pp]->next=%d\n"NONE,jfor,pp,pp->next);	
		pp=pp->next;
		//app->buffer.status_out_buf[STATUS_NOW]=app->buffer.status_out_buf[STATUS_NOW]->next;
	}

//exit(0);

char tmp[256];
snprintf(tmp,256,"%s/%s","/home/pi","origdata.dat");
if (fp_aaaa=fopen(tmp,"w+")){//打开日志文件;
    ;
}
else{
    exit(0);
}



	//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
            // for(ifor=0;ifor<10;ifor++)
            // {
            //    //if(0==app->app_server.connect[ifor].is_connect){
            //         app->app_server.connect[ifor].is_connect=1;
            //         //app->app_server.connect[ifor].sendbuf=app->buffer.databuf[LOOPREAD];
            //         i_connect=ifor;
            //         printf("i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",i_connect,app->app_server.connect[ifor].is_connect);
            //         //break;
            //    //}
            // }
    //while(1);
   	//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	init_loopbuf(app);
	init_fir_loopbuf(app);
	init_fir_loopbuf1(app);
	//init_steim2buf(app);
	//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	int fs;
	app->app_para.ad_reg.reg[3]=0xB0;
	switch(app->app_para.ad_reg.reg[3])
	{
		case 0xB0:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		fs=200;
		//firlen=67;
		break;
		case 0xa1:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		fs=100;
		//firlen=83;
		break;
		case 0x92:
		//firlen=165;
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		fs=50;
		break;
		default:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		app->app_para.ad_reg.reg[3]=0xB0;
		fs=200;
		break;
	}
	app->app_para.ad_reg.reg[3]=0xB0;
	app->app_para.fs=fs;

//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);


	fengle();
	//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
	pthread_t pt_readdata[10];
	pthread_t pt_savefile;
	//exit(0);
    uint8_t id;
  	int32_t adc[8];
	int32_t volt[8];
	uint8_t i;
	uint8_t ch_num;
	int32_t iTemp;
	uint8_t buf[3];
	int ifor;
	app->AdcNow[7]=0;
	app->ping_pang=0;
	//printf(BLUE"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__);
	//reset_1256();
	//bcm2835_gpio_fsel(RST, BCM2835_GPIO_FSEL_OUTP);//
	//RST_1();
	int a=5;
	//RST_1();
	ad_spi_config();
	ad_gpio_config();
	//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
    //bcm2835_gpio_fsel(DRDY, BCM2835_GPIO_FSEL_INPT);
    //bcm2835_gpio_set_pud(DRDY, BCM2835_GPIO_PUD_UP);    	
    //ADS1256_WriteReg(REG_MUX,0x01);
    //ADS1256_WriteReg(REG_ADCON,0x20);
   // ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_15SPS);
    
    //debug_stop();
	//call_back_func=myInterrupt;//连续模式
	g_tADS1256.ScanMode =1;
	app->ad1256.g_tADS1256.ScanMode =1;
	call_back_func=read_data_setch_interupt;//切换通道模式
	fp=fopen("data.bin","wb+");
	if(piHiPri (99)){
		printf("piHiPri error piHiPri=%d\n",piHiPri (50));
        exit(0);
    }
    //data_rdy();//中断方式







	//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

	//pthread_create (&pt_savefile, NULL, save_file, NULL) ;
	//pares_para();
	//app->app_para.fs=200;
	for(ifor=0;ifor<2;ifor++)//扫描方式
	{
		pthread_create (&pt_readdata[ifor], NULL, scan_read, &ifor) ;
		bsp_DelayUS(500);
	}

	while (1){
	// 	while(DRDY_IS_LOW())
	// 	{
	// 		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	// 		read_data_setch_interupt();
	// 		bsp_DelayUS(5);
	// 		//exit(0);
	// 	}
	// //piLock (int keyNum) ;
		sleep(1);
	}
	




	data_rdy();


	id = ADS1256_ReadChipID();
	printf("ID=%d\n",(int)id);  
 	debug_stop();

	printf("\r\n");
	printf("ID=\r\n");  
	if (id != 3)
	{
		printf("Error, ASD1256 Chip ID = 0x%d\r\n", (int)id);
	}
	else
	{
		printf("Ok, ASD1256 Chip ID = 0x%d\r\n", (int)id);
	}
	ADS1256_WriteCmd(CMD_RESET);
  	ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_7500SPS);
    ADS1256_StartScan(0);
    ADS1256_WriteCmd(CMD_RDATAC);
	ch_num = 8;	
	//if (ADS1256_Scan() == 0)
		//{
			//continue;
		//}
	int count=0;
	
	fp=fopen("data.bin","wb+");
	if(fp==NULL)
		exit(0);

	data_rdy();

	while(1)
	{
		//printf("\ncount=%d\n",count++);
        fprintf(fp,"count=%d\n",count++);  
	    while((ADS1256_Scan_rdy() == 0));
		//printf("\33[%dA", (int)ch_num);  
		//bsp_DelayUS(1);	
	}










	while(1)
	{
		//printf("\ncount=%d\n",count++);
        fprintf(fp,"count=%d\n",count++);  
	    while((ADS1256_Scan() == 0));
		for (i = 0; i < ch_num; i++)
		{
			adc[i] = ADS1256_GetAdc(i);
              	 volt[i] = (adc[i] * 100) / 167;	
		}
		
		for (i = 0; i < ch_num; i++)
		{
            buf[0] = ((uint32_t)adc[i] >> 16) & 0xFF;
            buf[1] = ((uint32_t)adc[i] >> 8) & 0xFF;
            buf[2] = ((uint32_t)adc[i] >> 0) & 0xFF;
			//fwrite(buffer_write+realwrite, 1,needwritebyte-realwrite,fp);
            //printf("%d=%02X%02X%02X, %8ld", (int)i, (int)buf[0], 
                   //(int)buf[1], (int)buf[2], (long)adc[i]);              //
            fprintf(fp,"%d=%02X%02X%02X, %8ld\n", (int)i, (int)buf[0], 
                   (int)buf[1], (int)buf[2], (long)adc[i]);                

            iTemp = volt[i];	/* uV  */
			if (iTemp < 0)
			{
				iTemp = -iTemp;
              	//printf(" (-%ld.%03ld %03ld V) \r\n", iTemp /1000000, (iTemp%1000000)/1000, iTemp%1000);
			}
			else
			{
                //printf(" ( %ld.%03ld %03ld V) \r\n", iTemp /1000000, (iTemp%1000000)/1000, iTemp%1000);                    
			}	
		}
		
		//printf("\33[%dA", (int)ch_num);  
		//bsp_DelayUS(1);	
	}
	fclose(fp);	
    bcm2835_spi_end();
    bcm2835_close();
	
    return 0;
}
/* Interrupt.c
 * you can build this like: 
 * gcc -Wall Interrupt.c -o Interrupt -lwiringPi
 * sudo ./Interrupt
*/
int aa;
int countdata=0;
void myInterrupt()
{
	if(has_reset==1){
		if(has_read_id==0){
			ADS1256_ReadChipID_ISR();
			has_read_id=1;
		}
		else{

			if(is_cdate==0){
				ADS1256_WriteCmd(CMD_RDATAC);
				bsp_DelayUS(5);
				ADS1256_SetChannal(7);	//Switch channel mode 
				is_cdate=1;
			}
			else{
				//g_tADS1256.Channel++;
				//ADS1256_SetChannal(g_tADS1256.Channel);	//Switch channel mode 
				//app.buf[app.ping_pang][countdata++]=app.AdcNow[7]++;
				if(app.AdcNow[7]==0){
					clock_gettime(CLOCK_REALTIME, &app.time.first_data_time);        
		    		printf(GREEN"count=%d,start:CLOCK_REALTIME: %d, %d\n"NONE, app.AdcNow[7],app.time.first_data_time.tv_sec, app.time.first_data_time.tv_nsec); 

				}
				else if(app.AdcNow[7]==2000*60*60*8)
				{
					clock_gettime(CLOCK_REALTIME, &app.time.first_data_time);        
		    		printf(RED"count=%d,end:CLOCK_REALTIME: %d, %d\n"NONE, app.AdcNow[7],app.time.first_data_time.tv_sec, app.time.first_data_time.tv_nsec); 

				}
				if(0){
					
					app.buf[app.ping_pang][countdata++]=ADS1256_ReadDataC();
					g_tADS1256.Channel=0;
					
				}
				else{
					aa= ADS1256_ReadDataC();
					app.buf[app.ping_pang][countdata++]=app.AdcNow[7]++;
					//fprintf(fp,"count=%d\n",app.buf[app.ping_pang][countdata]);
				}
				//ADS1256_ISR();
				
				if(countdata==400){
					//clock_gettime(CLOCK_REALTIME, &app.time.first_data_time);        
	    			//printf(GREEN"start:CLOCK_REALTIME: %d, %d\n"NONE, app.time.first_data_time.tv_sec, app.time.first_data_time.tv_nsec); 
					//printf(GREEN"inside function：%s,line : %d,ping_pang=%d,app.AdcNow[7]=%d\n"NONE,__FUNCTION__,__LINE__,app.ping_pang,app.AdcNow[7]);
					clock_gettime(CLOCK_REALTIME, &app.time.first_data_time);        
		    		printf(BLUE"count=%d,end:CLOCK_REALTIME: %d, %d\n"NONE, app.AdcNow[7],app.time.first_data_time.tv_sec, app.time.first_data_time.tv_nsec); 
					countdata=0;
					app.ping_pang=!app.ping_pang;
					send_go_signal(1);
				}
				
			    flag ++;
				rdy_count++;
			}
		}
	}
	else{
		has_reset=1;
		ADS1256_WriteCmd(CMD_RESET);
	    ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_500SPS);
	}
}




int app_reset()//重新配置了ad，所以缓冲区，算法等需要重新配置
{

}





int change_ch_lable=0;
int alg_pre_count;
int tmp_float;
long detat;
long last_sec,last_nsec;





int init_psa_pga()
{


	memset(&app.iws_trig_pgaget,0,sizeof(IWS_TRIG_PGAGET));
	app.iws_trig_pgaget.multiple_PGA=3.73;
	app.iws_trig_pgaget.increment_PGA=-1.23;
	app.iws_trig_pgaget.multiple_PGV=3.61;
	app.iws_trig_pgaget.increment_PGV=2.72;
	app.iws_trig_pgaget.trig=1;
	app.iws_trig_pgaget.countt=1;
	//app.iws_trig_pgaget.period[2]=3;
	return 0;
}



int init_pga(IWS_TRIG_PGAGET *iws_trig_pgaget)
{

	memset(iws_trig_pgaget,0,sizeof(IWS_TRIG_PGAGET));
	iws_trig_pgaget->multiple_PGA=3.73;
	iws_trig_pgaget->increment_PGA=-1.23;
	iws_trig_pgaget->multiple_PGV=3.61;
	iws_trig_pgaget->increment_PGV=2.72;
	iws_trig_pgaget->trig=1;
	iws_trig_pgaget->countt=1;
	iws_trig_pgaget->PGV_type=2;
	//app.iws_trig_pgaget.period[2]=3;
	return 0;
}
int init_psa(IWS_TRIG_PSAGET *iws_trig_psaget)
{
	memset(iws_trig_psaget,0,sizeof(IWS_TRIG_PSAGET));
	iws_trig_psaget->first=1;
	iws_trig_psaget->zuni=0.5;
	iws_trig_psaget->period[0]=0.3;
	iws_trig_psaget->period[1]=1;
	iws_trig_psaget->period[2]=3;
	return 0;
}


int fengle()
{
	unsigned int fs;
	unsigned int gain;
	printf("app.app_para.ad_reg.reg[2]&0x07=%X\n",(app.app_para.ad_reg.reg[2]&0x07));
	printf("app.app_para.ad_reg.reg[3]=%X\n",(app.app_para.ad_reg.reg[3]));
	switch(app.app_para.ad_reg.reg[2]&0x07)
	{
		case 0:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		gain=1;
		break;
		case 1:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		gain=2;
		break;
		case 2:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		gain=4;
		break;
		case 3:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		gain=8;

		break;
		case 4:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		gain=16;
		break;
		case 5:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		gain=32;
		break;
		case 6:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		gain=64;
		break;
		case 7:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		gain=64;
		break;
		default:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		app.app_para.ad_reg.reg[2]=app.app_para.ad_reg.reg[2]&0xf8;
		gain=1;
		break;

	}
	int firlen;
	// switch(app.app_para.ad_reg.reg[3])
	// {
	// 	case 0xB0:
	// 	//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	// 	fs=200;
	// 	firlen=67;
	// 	break;
	// 	case 0xa1:
	// 	//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	// 	fs=100;
	// 	firlen=83;
	// 	break;
	// 	case 0x92:
	// 	firlen=165;
	// 	//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	// 	fs=50;
	// 	break;
	// 	default:
	// 	printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	// 	app.app_para.ad_reg.reg[3]=0xB0;
	// 	fs=200;
	// 	break;

	// }
	switch(app.app_para.fs)
	{
		case 200:
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		if(app.iws_para.filter_chose==0){
			firlen=301;
		}
		else{
			firlen=541;
		}
		
		break;
		case 100:
		if(app.iws_para.filter_chose==0){
			firlen=191;
		}
		else{
			firlen=301;
		}
		
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		
		break;
		case 50:
		if(app.iws_para.filter_chose==0){
			firlen=81;
		}
		else{
			firlen=151;
		}
		
		//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		break;
		default:
		printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		if(app.iws_para.filter_chose==0){
			firlen=301;
		}
		else{
			firlen=541;
		}
		
		break;

	}
	init_psa(&iws_trig_psaget);
	init_pga(&iws_trig_pgaget);
	int ifor;
	for(ifor=0;ifor<6;ifor++)//清空之前采样率的发送缓存
	{
		app.iws_server[ifor].sig=0;
		app.iws_server[ifor].is_first_send=0;
	}
	//app.app_para.fs=200;
	//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	app.iws_para.location=app.iws_para.first_time=1;
	app.iws_para.band_passstruct_count=0;
	app.iws_para.firlen=firlen;
	alg_pre_count=0;
	//app.app_para.fs=fs;
	app.app_para.gain=gain;
	app.buffer.databuf[LOOPREAD]=app.buffer.databuf[LOOPREAD]->loopbufnext;
	app.buffer.databuf[LOOPSAVE]=app.buffer.databuf[LOOPREAD];
	app.buffer.databuf[LOOPSEND]=app.buffer.databuf[LOOPSAVE];
	printf("fs=%d,gain=%d\n",app.app_para.fs,app.app_para.gain);
	fir_list=fir_list_async=fir_list_head;
	fir_list1=fir_list1_async=fir_list1_head;
	for(ifor=0;ifor<10;ifor++)
	{
		app.app_server.connect[ifor].sig_send=0;
            //printf(GREEN"function：%s,line:%d,app.app_sig.sig_data_send=%d\n"NONE,__FUNCTION__,__LINE__,p_app->app_sig.sig_data_send);
    	app.app_server.connect[ifor].sendbuf=app.buffer.databuf[LOOPSAVE];

    }
	init_iws_steim2_para(&app.steim2_para);




	return 0;

	//app.app_para.ad_reg.reg[ifor];
/*
000 = 1 (default)
001 = 2 
010 = 4 
011 = 8 
100 = 16 
101 = 32 
110 = 64 
111 = 64





03h
11110000 = 30,000SPS (default)
11100000 = 15,000SPS 
11010000 = 7,500SPS 
11000000 = 3,750SPS 
10110000 = 2,000SPS 
10100001 = 1,000SPS 
10010010 = 500SPS 
10000010 = 100SPS 
01110010 = 60SPS 
01100011 = 50SPS 
01010011 = 30SPS 
01000011 = 25SPS 
00110011 = 15SPS 
00100011 = 10SPS 
00010011 = 5SPS 
00000011 = 2.5SPS
*/
}




void read_data_setch_interupt_bak1()
{
	//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	static int count=0;
	static int has_sync=0;
	static int has_capture=0;
	int aa;
	int tmp;
	int ifor;

//
	int data[161];
	int dataOut,i;
	int mode;
	// for(i=0;i<161;i++)
	// {
	// 	data[i]=pow(-1.0,double(i))*i*i;
	// }
	
//











	// if(has_capture){
	// 	has_capture=0;
	// 	return;
	// }
	// else{
	// 	has_capture=1;
	// }
	//exit(0);
	if(app.app_sig.sig_setad_ready==1)
	{
		app.ad1256.run_ctrl.has_reset=0;
		app.ad1256.run_ctrl.pre_read_count=0;
		app.ad1256.ad_count.read_times=0;
		app.ad1256.run_ctrl.has_read_id==0;
		app.ad1256.run_ctrl.has_sync=0;
		app.app_para.ad_ch_select=0;
		printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

	}
	if(app.ad1256.run_ctrl.has_reset==1){
		//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		if(app.ad1256.run_ctrl.has_read_id==0){
			printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
			ADS1256_ReadChipID_ISR();
			//app.ad1256.run_ctrl.has_read_id=1;
		}
		else{
			if(app.ad1256.run_ctrl.has_sync==0){
				printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
				if(app.ad1256.run_ctrl.pre_read_count==10)//app.ad1256.run_ctrl.pre_read_para
				{
					printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
					//app.ad1256.run_ctrl.has_reset=1;
					app.ad1256.ad_count.read_times=0;
					ADS1256_Setch_ReadData(0);
					app.app_para.ad_ch_select=0;
					alg_pre_count=0;
					app.ad1256.run_ctrl.has_sync=1;

				}
				else{
					printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
					//ADS1256_SetDiffChannal(0);	/* change DiffChannal */
					//ADS1256_SetChannal(7);	//Switch channel mode 
					if(app.ad1256.run_ctrl.pre_read_count==0){
						printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						bsp_DelayUS(5);
						ADS1256_WriteCmd(CMD_SYNC);
						bsp_DelayUS(5);
						ADS1256_WriteCmd(CMD_WAKEUP);
						bsp_DelayUS(25);
							//ADS1256_WriteCmd(CMD_RDATAC);
						bsp_DelayUS(5);
						ADS1256_SetDiffChannal(0);	//Switch channel mode 
						app.ad1256.run_ctrl.pre_read_count++;
					}
					else{
						printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						app.ad1256.run_ctrl.pre_read_count++;
						aa = ADS1256_ReadData();
					}
				}				
			}
			else{

				//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
				//exit(0);
				app.ad1256.run_ctrl.change_ch_lable=app.ad1256.run_ctrl.change_ch_lable==0?1:0;
				//printf(RED"app.ad1256.run_ctrl.change_ch_lable=%d\n"NONE,app.ad1256.run_ctrl.change_ch_lable);
				if(app.ad1256.run_ctrl.change_ch_lable==0){
					//aa = ADS1256_ReadData();
					//aa = ADS1256_Setch_ReadData(0);
					//app.buffer.databuf[LOOPREAD]->data_start[app.ad1256.ad_count.read_times%200].ad_data=ADS1256_Setch_ReadData(0);
					//app.app_para.ad_ch_select=(app.app_para.ad_ch_select==5)?0:(app.app_para.ad_ch_select++);

					if(app.app_para.ad_ch_select<4){
						tmp=app.ad1256.ad_count.read_times%app.app_para.fs;
						app.buffer.databuf[LOOPREAD]->data_start[tmp].datacount=app.ad1256.ad_count.read_times;
						app.buffer.databuf[LOOPREAD]->data_start[tmp].ch=(app.ad1256.ad_count.read_times)%app.app_para.ad_reg.reg[CTRL_OPEN_CHNUM];
						app.ad1256.ad_count.read_times++;

						clock_gettime(CLOCK_REALTIME, &app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time);
	//vapp->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_sec,app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_nsec
						if(app.ad1256.ad_count.read_times>1){
							if(app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec-last_sec){
								detat=1000000000+app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec-last_nsec;
								//printf(GREEN">>>>>>>detat=%ld\n"NONE,detat);
							}
							else{
								detat=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec-last_nsec;
								//printf(RED"detat=%ld\n"NONE,detat);
							}
							if(app.app_para.ad_ch_select==0){
								//printf(YELLOW"t=%d\n"NONE,(200/app.app_para.fs*3000000));

								if(detat>(200/app.app_para.fs*2000000)+(200/app.app_para.fs*1000000)*1){
									printf(YELLOW"detat=%ld,t=%d\n"NONE,detat,(200/app.app_para.fs*2000000)+(200/app.app_para.fs*1000000));
									//exit(0);//15999635
								}	
							}
							else{
								//printf(BLUE"t=%d\n"NONE,(200/app.app_para.fs*1000000)+(200/app.app_para.fs*1000000));

								if(detat>((200/app.app_para.fs*1000000)+(200/app.app_para.fs*1000000)*1)){
									printf(YELLOW"detat=%ld,t=%d\n"NONE,detat,(200/app.app_para.fs*1000000)+(200/app.app_para.fs*1000000));
									//exit(0);
								}	
							}

							//printf("detat=%d\n",detat);
							last_sec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
							last_nsec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
							//printf(BLUE"detat=%ld\n"NONE,detat);
						}
						else{
							printf(BLUE"detat=%ld\n"NONE,detat);
							last_sec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
							last_nsec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
						}


						//TODO change to list1



						app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data = ADS1256_Setch_ReadData(app.app_para.ad_ch_select+1);
						fir_list->data[app.app_para.ad_ch_select]=5*app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data/8388607;
						fir_list->utc_second=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
						fir_list->utc_nanosecond=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
						fir_list->sample_rata=app.app_para.fs;
						//printf(RED"app.app_para.ad_ch_select=%d,ch=%d,datacount=%d\n"NONE,app.app_para.ad_ch_select,app.buffer.databuf[LOOPREAD]->data_start[tmp].ch,app.buffer.databuf[LOOPREAD]->data_start[tmp].datacount);
						//resample(data,1,100,&dataOut);
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						//fprintf(fp,"app.ad_count.read_times=%d\n",app.ad1256.ad_count.read_times);	
					}
					else{//空采通道,完成一次采样


						// if(mode==0){
						// 	trig_wave();
						// }
						// else if(mode==2){
						// 	trig_info();

						// }
						// else if(mode==1){
						// 	alltime_wave();
						// }

						//printf(GREEN"app.app_para.ad_ch_select=%d\n"NONE,app.app_para.ad_ch_select);
						aa = ADS1256_Setch_ReadData(0);
						alg_pre_count++;





						if(alg_pre_count>300)
						{//
							int slt_length=6,llt_length=100,threshold=15;
							float p_result,tmp=0;
							//band_passstruct(fir_list);
							int ifor;
							for(ifor=0;ifor<3;ifor++)
							{
								if(app.iws_para.i_l[ifor]==1){

									tmp+=(fir_list->data[ifor])*(fir_list->data[ifor]);
								}
							}
							fir_list->band_pass_data[app.app_para.ad_ch_select]=powf(tmp,0.5);
							if(alg_pre_count>400){
								//printf("alg_pre_count=%d\n",alg_pre_count);
								if(slt_lltstru_2(fir_list,app.iws_para.slt_length,app.iws_para.llt_length,&p_result,app.iws_para.threshold,app.iws_para.trg_ch)==2){
									printf(RED"has triggered，ch=%d\n"NONE,app.app_para.ad_ch_select);
									
									//exit(0);
								}
								if(slt_lltstru(fir_list,app.iws_para.slt_length,app.iws_para.llt_length,&p_result,app.iws_para.threshold,app.iws_para.trg_ch)==2){
									printf(PURPLE"has triggered，ch=%d\n"NONE,app.app_para.ad_ch_select);
									//exit(0);
								}
							}
							//printf("alg_pre_count=%d\n",alg_pre_count);
							//band_passstruct(fir_list_head);
						}
						fir_list=fir_list->next;
							//alg_pre_count++;
						//printf(RED"app.app_para.ad_ch_select=%d\n"NONE,app.app_para.ad_ch_select);
					}
					if(app.app_para.ad_ch_select==4){

						app.app_para.ad_ch_select=0;
						
					}
					else{
						app.app_para.ad_ch_select++;
					}

					app.app_sig.iswork=1;//告诉指示灯操作程序在工作中
					if((app.ad1256.ad_count.read_times%app.app_para.fs)==0 && app.app_para.ad_ch_select != 0)
					{	
						//if(myoso->thread_status.tcp_data_status.isconnect==1){
						//myoso->oso_sig.sig_data_send++;
						//myoso->thread_status.tcp_data_status.isconnect=0;
						//printf(RED"myoso->thread_status.tcp_data_status.isconnect=0\n"NONE);
						//}
						for(ifor=0;ifor<10;ifor++)
						{
	            			//printf(BLUE"ifor=%d,app.app_server.connect[ifor].is_connect=%d\n"NONE,ifor,app.app_server.connect[ifor].is_connect);

							//p_app->app_server.connect[connect_index].is_connect=1;
							if(1==app.app_server.connect[ifor].is_connect){
	            				app.app_server.connect[ifor].sig_send++;
	            				//printf("ifor=%d,ad1282 sig_send=%d\n",ifor,app.app_server.connect[ifor].sig_send);
							}
						}
						//if(app.app_sig.sig_is_data_connect==1)
						//{
							//app.app_sig.sig_data_send++;
						//printf(RED"function：%s,line:%d,app.app_sig.sig_data_send=%d\n"NONE,__FUNCTION__,__LINE__,app.app_sig.sig_data_send);
			
						//}
						//printf(RED"app.buffer.databuf[LOOPREAD].index=%d\n"NONE,app.buffer.databuf[LOOPREAD]->loopbufindex);

						app.app_sig.sig_data_save++;
						//printf("app.app_sig.sig_data_save=%d\n",app.app_sig.sig_data_save);
						if(app.app_sig.sig_data_save>LOOPBUFNUM)
						{
							printf("over buf,app exit\n");
							//exit(0);
						}
						//printf("app.ad1256.ad_count.read_times=%d,app.app_para.fs=%d\n",app.ad1256.ad_count.read_times,app.app_para.fs);
						//send_go_signal(1);
						//fwrite(app.buffer.databuf[LOOPREAD]->data_start, 1,200*sizeof(DATA_FRAME_GEO_S),fp);
						//printf(RED"app.buffer.databuf[LOOPREAD]=%d\n"NONE,app.buffer.databuf[LOOPREAD]);
						//printf(GREEN"app.ad1256.ad_count.read_times=%d\n"NONE,app.ad1256.ad_count.read_times);
						app.buffer.databuf[LOOPREAD]=app.buffer.databuf[LOOPREAD]->loopbufnext;
					}
						
				}
				else{
					aa = ADS1256_ReadData();	
				}
			}
			//clock_gettime(CLOCK_REALTIME, &app.time.first_data_time);        
    		//printf(GREEN"count=%d,start:CLOCK_REALTIME: %d, %d\n"NONE, app.AdcNow[7],app.time.first_data_time.tv_sec, app.time.first_data_time.tv_nsec); 
		}
	}
	else{
		app.app_sig.sig_data_save=0;
		app.app_sig.sig_setad_ready=2;
		app.ad1256.run_ctrl.has_reset=1;
		ADS1256_WriteCmd(CMD_RESET);
		bsp_DelayUS(50);
		//debug_stop();
		//exit(0);
		fengle();
	    ADS1256_config();
	    app_reset();//重置变量
	    //ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_2000SPS);
	}
}





int xishu_globe=1;
float sta_lta;

float floattest_ipga1(float pga,float multiple,float increment)
{
	printf("pga=%f\n",pga);
	printf("multiple=%f\n",multiple);
	printf("increment=%f\n",increment);
	//printf("log10f=%f\n",log10f(100.0));
	float tmp=(float)(multiple*log10f(pga)+increment);
	printf("ipga=%f\n",tmp);
	return tmp;
}


float floattest_ipgv1(float pgv,float multiple,float increment)
{
	printf("pgv=%f\n",pgv);
	printf("log10f=%f\n",log10f(100.0));
	float tmp=(float)(multiple*log10f(pgv)+increment);
		printf("ipgv=%f\n",tmp);
	return tmp;
}








int start_evt()
{
	char url[256];
	if(app.evt_record.fp!=NULL)
	{
		app.evt_record.fp=fclose(app.evt_record.fp);
		app.evt_record.is_file_open=0;
	}
	snprintf(url,256,"%s/evt_%d.%d.evt",app.evt_record.path,app.evt_record.utc_time[0],app.evt_record.utc_time[1]);
	app.evt_record.fp=fopen(url,"w+");
	printf("url=%s,app.evt_record.fp=%d\n",url,app.evt_record.fp);
	//exit(0);
	if(app.evt_record.fp!=NULL){
			printf("url=%s,app.evt_record.fp=%d\n",url,app.evt_record.fp);
		app.evt_record.is_file_open=1;
		return 0;
	}
		printf("url=%s,app.evt_record.fp=%d\n",url,app.evt_record.fp);
	return 0;
}






int start_record_evt()
{
	char url[256];
	if(app.evt_record.fp!=NULL)
	{
		app.evt_record.fp=fclose(app.evt_record.fp);
		app.evt_record.is_file_open=0;
	}
	snprintf(url,256,"%s/evt_%d.%d.evt",app.evt_record.path,app.evt_record.utc_time[0],app.evt_record.utc_time[1]);
	app.evt_record.fp=fopen(url,"w+");
	printf("url=%s,app.evt_record.fp=%d\n",url,app.evt_record.fp);
	//exit(0);
	if(app.evt_record.fp!=NULL){
			printf("url=%s,app.evt_record.fp=%d\n",url,app.evt_record.fp);
		app.evt_record.is_file_open=1;
		return 0;
	}
		printf("url=%s,app.evt_record.fp=%d\n",url,app.evt_record.fp);
	return 0;
}














short d_d_d_d=0;










float Intensity_calculation(float pga,float pga_multiple,float pga_increment,float pgv,float pgv_multiple,float pgv_increment)
	{
	int flag_80=1;
	if(flag_80==1)
		{
		pga_multiple=3.322;    //这组系数为1980烈度标准 默认就使用它（参数列表里的值先留着，便于以后修改（通过上位机以某种方式修改））
		pga_increment=0.033;   //
		pgv_multiple=3.322;    //
		pgv_increment=3.677;   //
		}
	float ipga=pga_multiple*log10(pga/10.0)+pga_increment;
	float ipgv=pgv_multiple*log10(pgv/10.0)+pgv_increment;
	if(ipga<4&&ipgv<4)
		{
		return(ipga);
		}
	else if(ipga>8&&ipgv>8)
		{
		return(ipgv);
		}
	else
		{
		return(0.41*ipga+0.59*ipgv);
		}


	}










float avg_data[3]={0};
float count_data[3]={0};





#define AVG_SECOND 20

int async_alg_main(APP_S *app)//异步算法处理过程
{
	int mode=0;
	int aa;
	long utc_time_imt[2];
	// printf("alg_pre_count=%d\n",alg_pre_count);
	// exit(0);
	while(1)
	{

		if(app->app_sig.sig_can_alg>0){

			//app.app_sig.sig_can_alg++;
			app->app_count.date_time.tv_sec=fir_list1_async->utc_second;
			app->app_count.date_time.tv_nsec=fir_list1_async->utc_nanosecond;
			app->app_count.sample_times_ch++;
			//printf(ADS1256_DEBUG_COLOR"[%d]:tv_sec:%d,tv_nsec:%d\n"NONE,tid,app->app_count.date_time.tv_sec,app->app_count.date_time.tv_nsec);

			// if(mode==0){
			// 	trig_wave();
			// }
			// else if(mode==2){
			// 	trig_info();

			// }
			// else if(mode==1){
			// 	alltime_wave();
			// }
			//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

			//printf(GREEN"app->app_para.ad_ch_select=%d\n"NONE,app->app_para.ad_ch_select);
			//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

			alg_pre_count++;//30*app->app_para.fs
			if(alg_pre_count>AVG_SECOND*app->app_para.fs+app->iws_para.firlen+200){//有一些数据后开始做重采样
			//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
				float p_result,tmp=0;



/*
					avg_data[0]=avg_data[0]/(AVG_SECOND*app->app_para.fs-1);//fir_list1_async->data[0];
					avg_data[1]=avg_data[1]/(AVG_SECOND*app->app_para.fs-1);//fir_list1_async->data[0];
					avg_data[2]=avg_data[2]/(AVG_SECOND*app->app_para.fs-1);//fir_list1_async->data[0];


*/
				// printf("avg_data[0]=%f\n",avg_data[0]);
				// printf("avg_data[1]=%f\n",avg_data[1]);
				// printf("avg_data[2]=%f\n",avg_data[2]);
				// fir_list1_async->data[0]=fir_list1_async->data[0]-avg_data[0];
				fir_list1_async->data[1]=fir_list1_async->data[1]-avg_data[1];
				fir_list1_async->data[2]=fir_list1_async->data[2]-avg_data[2];
				int resample2_ret=resample2(fir_list1_async,&app->iws_para.location,fir_list_async,&app->iws_para.first_time,2);

				//>>触发测试
				// ret = slt_lltstru_2(fir_list_async,app->iws_para.slt_length,app->iws_para.llt_length,&p_result,app->iws_para.threshold,app->iws_para.trg_ch);
				// printf("ret=%d\n",ret);
				// ret = slt_lltstru(fir_list_async,app->iws_para.slt_length,app->iws_para.llt_length,&p_result,app->iws_para.threshold,app->iws_para.trg_ch);
				// printf("ret=%d\n",ret);
				// if(slt_lltstru_2(fir_list_async,app->iws_para.slt_length,app->iws_para.llt_length,&p_result,app->iws_para.threshold,app->iws_para.trg_ch)==2){
				// 	printf(RED"has triggered，ch=%d\n"NONE,app->app_para.ad_ch_select);
					
				// 	//exit(0);
				// }
				// if(slt_lltstru(fir_list_async,app->iws_para.slt_length,app->iws_para.llt_length,&p_result,app->iws_para.threshold,app->iws_para.trg_ch)==2){
				// 	printf(PURPLE"has triggered，ch=%d\n"NONE,app->app_para.ad_ch_select);
				// 	//exit(0);
				// }
				//<<触发测试

				//printf(RED"fir_list1_async->utc_second=%d,fir_list1_async->count=%d\n"NONE,fir_list1_async->utc_second,fir_list1_async->count);
				fir_list1_async=fir_list1_async->next;//每得到一个新采样，指针后移
				int ifor,band_passstruct_ret=0;
				if(resample2_ret==2){//每得到一个重采样做相应的处理
					app->app_count.resample_times++;

					//printf("resample_times=%d,fir_list_async.count=%d\n",app->app_count.resample_times,fir_list_async->count);
					//int realwrite=mywrite(&fir_list_async->data[0],4,fp_aaaa);

					app->app_count.si_count_second++;
					//printf("app->app_count.si_count=%d\n",app->app_count.si_count);

					if(app->app_count.si_count==0){
						//printf(CYAN"fir_list_async->utc_second=%d\n"NONE,fir_list_async->utc_second);
						app->iws_status.utc_time[0]=fir_list_async->utc_second;
						//exit(0);
					}
					//printf(RED"1 app->iws_status.utc_time[0]=%d\n"NONE,app->iws_status.utc_time[0]);
					app->app_count.si_count++;

					for(ifor=0;ifor<3;ifor++){//计算三个通道的最大最小值
						if(app->iws_status.max[ifor]<fir_list_async->data[ifor]){
							//printf(BLUE"fir_list_async->data[ifor]=%f\n"NONE,fir_list_async->data[ifor]);

							app->iws_status.max[ifor]=fir_list_async->data[ifor];
						}
						if(app->iws_status.min[ifor]>fir_list_async->data[ifor]){
							app->iws_status.min[ifor]=fir_list_async->data[ifor];
							//printf(GREEN"fir_list_async->data[ifor]=%f\n"NONE,fir_list_async->data[ifor]);
						}
						//printf(RED"fir_list_async->data[ifor]=%f\n"NONE,fir_list_async->data[ifor]);
					}
					//printf("app->app_count.si_count_second=%d\n",app->app_count.si_count_second);
					if(app->app_count.si_count_second==app->app_para.fs){
						//printf(GREEN"1 app->iws_status.utc_time[0]=%d\n"NONE,app->iws_status.utc_time[0]);
						//printf(LIGHT_CYAN"si_count_second=%d,app->iws_status.max[0]=%f,app->iws_status.min[0]=%f\n"NONE,\
						app->app_count.si_count_second,app->iws_status.max[0],app->iws_status.min[0]);
						//printf("app->app_count.si_count_second=%d,app->iws_status.min[0]=%f\n",app->app_count.si_count_second,app->iws_status.min[0]);
						app->iws_status.ud[app->app_count.si_count_times%10]=(int)app->xishu_globe*(app->iws_status.max[0]-app->iws_status.min[0]);//app->app_count.si_count_times+1;//(int)(((app->iws_status.max[2]-app->iws_status.min[2])/5)*8388607);
						app->iws_status.ew[app->app_count.si_count_times%10]=(int)app->xishu_globe*(app->iws_status.max[1]-app->iws_status.min[1]);//app->app_count.si_count_times+2;//(int)(((app->iws_status.max[1]-app->iws_status.min[1])/5)*8388607);
						app->iws_status.ns[app->app_count.si_count_times%10]=(int)app->xishu_globe*(app->iws_status.max[2]-app->iws_status.min[2]);//app->app_count.si_count_times+3;//(int)(((app->iws_status.max[0]-app->iws_status.min[0])/5)*8388607);
						//printf("max=%f,min=%f,max-min=%f,int max-min=%d\n",app->iws_status.max[0],app->iws_status.min[0],(app->iws_status.max[0]-app->iws_status.min[0]),(int)(app->iws_status.max[0]-app->iws_status.min[0]));
						app->iws_status.max[0]=app->iws_status.max[1]=app->iws_status.max[2]=-50*1000;
						app->iws_status.min[0]=app->iws_status.min[1]=app->iws_status.min[2]=50*1000;
						app->app_count.si_count_second=0;
						app->app_count.si_count_times++;
					}
					if(app->app_count.si_count==(app->app_para.fs*10)){//收割状态信息
						//printf("yes\n");
						//printf(PURPLE"2 app->iws_status.utc_time[0]=%d\n"NONE,app->iws_status.utc_time[0]);
						iws_do_status_pre(fir_list_async,&app->iws_status);
						app->app_count.si_count=0;
						
					}
					iws_do_steim_pre(fir_list_async,&app->steim2_para);//收割波形数据
					//->get x_y_z;
					int x_y_z;
					if(app->iws_para.i_l[0]==1&&app->iws_para.i_l[1]==1&&app->iws_para.i_l[2]==0){
						app->iws_para.trg_ch=4;
						x_y_z=0;
					}
					else if(app->iws_para.i_l[0]==1&&app->iws_para.i_l[1]==1&&app->iws_para.i_l[2]==1){
						app->iws_para.trg_ch=4;
						x_y_z=1;
					}
					else if(app->iws_para.i_l[2]==1)
					{
						app->iws_para.trg_ch=2;
						x_y_z=0;
					}
					else{
						app->iws_para.trg_ch=4;
						x_y_z=0;
					}
					//<-get x_y_z;
					if(app->app_count.resample_times<(30*app->app_para.fs)){//维护30秒前的数据指针
						app->listbefore30sec=fir_list_async_head;
						band_passstruct2(fir_list_async,app->iws_para.filter_chose,x_y_z);
						//band_passstruct_new(fir_list_async);
					}
					else{//DO TRIG
						band_passstruct2(fir_list_async,app->iws_para.filter_chose,x_y_z);
						//band_passstruct_new(fir_list_async);

                        if(app->iws_para.trig_flag==0)
                        {
							if(slt_lltstru(fir_list_async,app->iws_para.slt_length,app->iws_para.llt_length,&p_result,app->iws_para.threshold,app->iws_para.trg_ch)==2){
								printf(PURPLE"has triggered，ch=%d\n"NONE,app->app_para.ad_ch_select);
								app->is_trig=1;
							}
							else{
								app->is_trig=0;
							}
						}
						else if(app->iws_para.trig_flag==1){
							if(abs(fir_list_async->band_pass_data[4])>app->iws_para.threshold_a[0]){
								printf(PURPLE"has triggered，ch=%d\n"NONE,app->app_para.ad_ch_select);
								app->is_trig=1;
							}
							else{
								app->is_trig=0;
							}
						}
						else if(app->iws_para.trig_flag==2){
							if(abs(fir_list_async->band_pass_dataV[4])>app->iws_para.threshold_a[1]){
								printf(PURPLE"has triggered，ch=%d\n"NONE,app->app_para.ad_ch_select);
								app->is_trig=1;
							}
							else{
								app->is_trig=0;
							}
						}
						else if(app->iws_para.trig_flag==3){
							if(abs(fir_list_async->band_pass_dataD[4])>app->iws_para.threshold_a[2]){
								printf(PURPLE"has triggered，ch=%d\n"NONE,app->app_para.ad_ch_select);
								app->is_trig=1;
							}
							else{
								app->is_trig=0;
							}
						}


						if(app->is_trig==1){//如果触发了则更新触发最后时间
							app->app_count.last_trig=app->app_count.resample_times;
							if(app->is_trig_start==0){//开启触发处理过程
								init_psa(&iws_trig_psaget);
								init_pga(&iws_trig_pgaget);
								d_d_d_d=first_direction(fir_list_async,2,app->iws_para.firlen);//chu dong fang xiang
								sta_lta=p_result;
								get_imt(fir_list_async,iws_trig_psaget.utc_time_imt,iws_trig_psaget.trig_start);
								app->evt_record.utc_time[0]=iws_trig_psaget.trig_start[0];
								app->evt_record.utc_time[1]=iws_trig_psaget.trig_start[1];
								start_evt();
								int det_t=0;//调整到N通道
								app->app_sig.sig_wave_data_send_buf=(app->app_sig.sig_wave_data_write_buf+(int)(1000-3*36*4*(app->app_para.fs/200.0)))%1000;
								app->app_count.trig_start=app->app_count.resample_times;
								int ifor;
								app->sig_trig_wave=3*36*4*(app->app_para.fs/200.0);
								for(ifor=0;ifor<6;ifor++)
								{

									if((app->iws_server[ifor].status.isconnect==1)&&(app->iws_server[ifor].mode==2)){
										//app->buffer.steim2_out_buf[STEIM2_NOW];
										app->iws_server[ifor].sig_trig_wave=3*36*4*(app->app_para.fs/200.0);
										//printf("app->iws_server[ifor].sig_trig_wave=%d\n,\
											app->app_sig.sig_wave_data_send_buf=%d\n\
											app->app_sig.sig_wave_data_write_buf",\
											app->iws_server[ifor].sig_trig_wave,\
											app->app_sig.sig_wave_data_send_buf,\
											app->app_sig.sig_wave_data_write_buf);
										app->iws_server[ifor].fir_list_async_trig_start=app->listbefore30sec;
										app->iws_server[ifor].sig_trig_wave_data_send_buf=app->app_sig.sig_wave_data_send_buf;
										//printf(PURPLE"server %d ready app->iws_server[ifor].sig=%d\n"NONE,ifor,app->iws_server[ifor].sig);
										//app->iws_server[ifor].sig++;
										app->iws_server[ifor].is_trig_start=1;
									}
								}
								app->listbefore30sec=app->listbefore30sec->next;
								//fir_list_async_trig_start
								app->is_trig_start=1;//指示触发传波形开始
								app->app_sig.sig_trig_ti_send_buf=app->app_sig.sig_trig_ti_write_buf;
								
							}
						}
						if(app->is_trig_start==1){//维护触发状态，每个采样点更新一次状态
							int aa=app->app_count.resample_times-app->app_count.last_trig;
							int bb=app->app_count.resample_times-app->app_count.trig_start;
							//printf(RED"aa=%d,bb=%d,app.app_count.last_trig=%d,app.app_count.trig_start=%d\n"NONE,aa,bb,app->app_count.last_trig,app->app_count.trig_start);

							if((aa>(30*app->app_para.fs))\
							    &&(bb>(60*app->app_para.fs))){
							    app->is_trig_start=0;

							}
						}
						//printf(CYAN"app->is_trig=%d,app->is_trig_start=%d\n"NONE,app->is_trig,app->is_trig_start);
						if(app->is_trig_start==1){//一旦事件开始
							//printf(GREEN"is_trig_start\n"NONE);
							psa_struct_2(fir_list_async,(float)app->app_para.fs,iws_trig_psaget.zuni,\
								&(iws_trig_psaget.period[0]),&(iws_trig_psaget.result[0][0]),\
								&(iws_trig_psaget.dx[0][0]),&(iws_trig_psaget.x2[0][0]),\
								&(iws_trig_psaget.y0[0][0]),&(iws_trig_psaget.y1[0][0]),\
								&(iws_trig_psaget.first));
							//int final_liedu_pgv;
//int PGA_V_G(FIR_LIST *i_fir_list,BAND_PASS_RESULT *band_pass_data,int *count,int timespan,int *trig,int PGV_type)
							int pga_ret;
								//printf(LIGHT_BLUE"before PGA_V_G\n"NONE);



							//pga_ret=PGA_V_G(fir_list_async,\
								&(iws_trig_pgaget.band_pass_data),\
								&(iws_trig_pgaget.countt),\
								1,&(iws_trig_pgaget.trig),\
								iws_trig_pgaget.PGV_type);





							pga_ret=PGA_V_G2(fir_list_async,\
								&(iws_trig_pgaget.band_pass_data),\
								&(iws_trig_pgaget.countt),\
								1,&(iws_trig_pgaget.trig),\
								iws_trig_pgaget.PGV_type,app->iws_para.filter_chose);
							/*
	iws_trig_pgaget->multiple_PGA=3.73;
	iws_trig_pgaget->increment_PGA=-1.23;
	iws_trig_pgaget->multiple_PGV=3.61;
	iws_trig_pgaget->increment_PGV=2.72;
							*/
							if(pga_ret==2){//算出了烈度值，更新触发信息
								//printf("iws_trig_pgaget.band_pass_data.final_PGA=%f\n",iws_trig_pgaget.band_pass_data.final_PGA);
								//printf("ipga=%f\n",floattest_ipga1(iws_trig_pgaget.band_pass_data.final_PGA,iws_trig_pgaget.multiple_PGA,iws_trig_pgaget.increment_PGA));
								// printf("iws_trig_pgaget.band_pass_data.final_PGA=%f\n",\
								// 	iws_trig_pgaget.band_pass_data.final_PGA);
								// printf("iws_trig_pgaget.band_pass_data.final_PGA_component[0]=%f\n",\
								// 	iws_trig_pgaget.band_pass_data.final_PGA_component[0]);
								// printf("iws_trig_pgaget.band_pass_data.final_PGV=%f\n",\
								// 	iws_trig_pgaget.band_pass_data.final_PGV);
								// printf("iws_trig_pgaget.band_pass_data.final_PGV_component[0]=%f\n",\
								// 	iws_trig_pgaget.band_pass_data.final_PGV_component[0]);
								printf("iws_trig_pgaget.band_pass_data.final_PGD=%f\n",\
									iws_trig_pgaget.band_pass_data.final_PGD);
								printf("iws_trig_pgaget.band_pass_data.final_PGD_component[0]=%f\n",\
									iws_trig_pgaget.band_pass_data.final_PGD_component[0]);
								printf("fir_list_async->band_pass_dataD[0]=%f\n",\
									fir_list_async->band_pass_dataD[0]);
								printf("fir_list_async->band_pass_dataD[4]=%f\n",\
									fir_list_async->band_pass_dataD[4]);
								printf("fir_list_async->dataD[0]=%f\n",\
									fir_list_async->dataD[0]);
								printf("fir_list_async->dataD[4]=%f\n",\
									fir_list_async->dataD[4]);
								
								//fir_list1_async
								//exit(0);


								// iws_trig_pgaget.band_pass_data.final_PGA=5000;
								// iws_trig_pgaget.band_pass_data.final_PGV=50;
								int final_liedu_pga=(int)10*Intensity_calculation(iws_trig_pgaget.band_pass_data.final_PGA,iws_trig_pgaget.multiple_PGA,iws_trig_pgaget.increment_PGA,iws_trig_pgaget.band_pass_data.final_PGV,iws_trig_pgaget.multiple_PGV,iws_trig_pgaget.increment_PGV);
								//int final_liedu_pga=(int)10*(floattest_ipga1(iws_trig_pgaget.band_pass_data.final_PGA/10,iws_trig_pgaget.multiple_PGA,iws_trig_pgaget.increment_PGA));
								//int final_liedu_pgv=(int)(10*floattest_ipgv1(iws_trig_pgaget.band_pass_data.final_PGV,iws_trig_pgaget.multiple_PGV,iws_trig_pgaget.increment_PGV));
								//printf(RED"final_liedu_pga=%d\n"NONE,final_liedu_pga);
								//printf(LIGHT_BLUE"pga_ret=%d\n"NONE,pga_ret);
								iws_do_trig_pre(&(iws_trig_pgaget.band_pass_data)/*BAND_PASS_RESULT *band_pass_data*/,&(iws_trig_psaget.result[0][0]),final_liedu_pga,iws_trig_psaget.utc_time_imt,sta_lta,fir_list_async->utc_second-iws_trig_psaget.trig_start[0]);
								for(ifor=0;ifor<6;ifor++)
								{

									if((app->iws_server[ifor].status.isconnect==1)&&(app->iws_server[ifor].mode>1)){
										app->iws_server[ifor].sig_trig_ti++;
										printf(PURPLE"server %d ready app->iws_server[ifor].mode=%d\n"NONE,ifor,app->iws_server[ifor].mode);

									}
								}
								app->app_sig.sig_trig_ti_write_buf_total++;
								app->app_sig.sig_trig_ti_write_buf=app->app_sig.sig_trig_ti_write_buf_total%1000;
							}
						}
 					}
					app->listnow=fir_list_async;
					fir_list_async=fir_list_async->next;
				}
				else{//忽略

				}
				//printf("resample2_ret=%d\n",resample2_ret);
			}
			else if(alg_pre_count<AVG_SECOND*app->app_para.fs){//			alg_pre_count++;//30*app->app_para.fs
			//if(alg_pre_count>app->iws_para.firlen+200){//有一些数据后开始做重采样
				if(alg_pre_count<=5*app->app_para.fs)
				{

				}
				else
				{
				avg_data[0]+=fir_list1_async->data[0];
				avg_data[1]+=fir_list1_async->data[1];
				avg_data[2]+=fir_list1_async->data[2];
				if(alg_pre_count==(AVG_SECOND*app->app_para.fs-1)){
					avg_data[0]=avg_data[0]/(AVG_SECOND*app->app_para.fs-1-5*app->app_para.fs);//fir_list1_async->data[0];
					avg_data[1]=avg_data[1]/(AVG_SECOND*app->app_para.fs-1-5*app->app_para.fs);//fir_list1_async->data[0];
					avg_data[2]=avg_data[2]/(AVG_SECOND*app->app_para.fs-1-5*app->app_para.fs);//fir_list1_async->data[0];

				}
			}
				fir_list1_async=fir_list1_async->next;//每得到一个新采样，指针后移
				
			}
			else{
				fir_list1_async->data[0]=fir_list1_async->data[0]-avg_data[0];
				fir_list1_async->data[1]=fir_list1_async->data[1]-avg_data[1];
				fir_list1_async->data[2]=fir_list1_async->data[2]-avg_data[2];
				fir_list1_async=fir_list1_async->next;//每得到一个新采样，指针后移

			}
			//printf("app->app_sig.sig_can_alg=%d\n",app->app_sig.sig_can_alg);
	        pthread_mutex_lock(&(app->app_mut[MUTEX_LOCK_IS_CAN_ALG]));
	        	app->app_sig.sig_can_alg--;
			pthread_mutex_unlock(&(app->app_mut[MUTEX_LOCK_IS_CAN_ALG]));
			
			// fir_list_async=fir_list_async->next;
			// fir_list1_async=fir_list1_async->next;
				//alg_pre_count++;
			//printf(RED"app->app_para.ad_ch_select=%d\n"NONE,app->app_para.ad_ch_select);
		}
		else{
			int sleep_time;
			sleep_time=2000;//(int)(1/app->app_para.fs*900000);
			//printf(RED"app->app_para.fs=%d,1/app->app_para.fs=%f,sleep_time=%d,app->app_sig.sig_can_alg=%d\n"NONE,app->app_para.fs,1/app->app_para.fs,sleep_time,app->app_sig.sig_can_alg);
			usleep(sleep_time);
		}
	}
	return 0;
}






static inline int read_data_setch_interupt_async(int tid)
{
	LOCAT();
	int resample2_ret,ret,tmp,ifor;
	if(app.app_sig.sig_setad_ready==1)
	{
		app.ad1256.run_ctrl.has_reset=0;
		app.ad1256.run_ctrl.pre_read_count=0;
		app.ad1256.ad_count.read_times=0;
		app.ad1256.run_ctrl.has_read_id==0;
		app.ad1256.run_ctrl.has_sync=0;
		app.app_para.ad_ch_select=0;
		ad_gpio_config();
		//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

	}
	if(app.ad1256.run_ctrl.has_reset==1){
		LOCAT();

		//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		if(app.ad1256.run_ctrl.has_read_id==0){
			LOCAT();

			//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
			ADS1256_ReadChipID_ISR();
			//app.ad1256.run_ctrl.has_read_id=1;
		}
		else{
			LOCAT();

			if(app.ad1256.run_ctrl.has_sync==0){
				LOCAT();

				//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
				if(app.ad1256.run_ctrl.pre_read_count==10)//app.ad1256.run_ctrl.pre_read_para
				{
					LOCAT();

					//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
					//app.ad1256.run_ctrl.has_reset=1;
					app.ad1256.ad_count.read_times=0;
					ADS1256_Setch_ReadData(0);
					app.app_para.ad_ch_select=0;
					alg_pre_count=0;
					app.ad1256.run_ctrl.has_sync=1;
				}
				else{
					LOCAT();

					//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
					//ADS1256_SetDiffChannal(0);	/* change DiffChannal */
					//ADS1256_SetChannal(7);	//Switch channel mode 
					if(app.ad1256.run_ctrl.pre_read_count==0){
						//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						bsp_DelayUS(5);
						ADS1256_WriteCmd(CMD_SYNC);
						bsp_DelayUS(5);
						ADS1256_WriteCmd(CMD_WAKEUP);
						bsp_DelayUS(25);
							//ADS1256_WriteCmd(CMD_RDATAC);
						bsp_DelayUS(5);
						ADS1256_SetDiffChannal(0);	//Switch channel mode 
						app.ad1256.run_ctrl.pre_read_count++;
					}
					else{
						//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						app.ad1256.run_ctrl.pre_read_count++;
						aa = ADS1256_ReadData();
					}
				}				
			}
			else{
				LOCAT();
				app.ad1256.run_ctrl.change_ch_lable=app.ad1256.run_ctrl.change_ch_lable==0?1:0;
				if(app.ad1256.run_ctrl.change_ch_lable==0){
					if(app.app_para.ad_ch_select<4){
						tmp=app.ad1256.ad_count.read_times%app.app_para.fs;
						app.buffer.databuf[LOOPREAD]->data_start[tmp].datacount=app.ad1256.ad_count.read_times;
						app.buffer.databuf[LOOPREAD]->data_start[tmp].ch=(app.ad1256.ad_count.read_times)%app.app_para.ad_reg.reg[CTRL_OPEN_CHNUM];
						app.ad1256.ad_count.read_times++;
						clock_gettime(CLOCK_REALTIME, &app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time);
						if(app.ad1256.ad_count.read_times>1){
							if(app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec-last_sec){
								detat=1000000000+app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec-last_nsec;
							}
							else{
								detat=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec-last_nsec;
							}
							if(app.app_para.ad_ch_select==0){
								//printf(YELLOW"t=%d\n"NONE,(200/app.app_para.fs*3000000));

								if(detat>(200/app.app_para.fs*2000000)+(200/app.app_para.fs*1000000)*1){
									printf(YELLOW"detat=%ld,t=%d\n"NONE,detat,(200/app.app_para.fs*2000000)+(200/app.app_para.fs*1000000));
									//exit(0);//15999635
								}	
							}
							else{
								if(detat>((200/app.app_para.fs*1000000)+(200/app.app_para.fs*1000000)*1)){
									printf(YELLOW"detat=%ld,t=%d\n"NONE,detat,(200/app.app_para.fs*1000000)+(200/app.app_para.fs*1000000));
									//exit(0);
								}	
							}
							last_sec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
							last_nsec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
							//printf(BLUE"detat=%ld\n"NONE,detat);
						}
						else{
							printf(BLUE"detat=%ld\n"NONE,detat);
							last_sec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
							last_nsec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
						}
						app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data = ADS1256_Setch_ReadData(app.app_para.ad_ch_select+1);
						float aaa=1.211373*app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data;//新仪器
						//float aaa=1.07*app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data;
						//ad=3927206,fir_list1=-1559.697144

						fir_list1->data[app.app_para.ad_ch_select]=9.8*((aaa*2.5*2.0)/(1.7474*0x494008*app.app_para.gain)*1000-2500);//5*app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data/8388607;
						//printf("app.app_para.ad_ch_select=%d,ad=%d,fir_list1=%f\n",app.app_para.ad_ch_select,aaa,fir_list1->data[app.app_para.ad_ch_select]);

						//printf("tmp=%d\n",tmp);
						fir_list1->utc_second=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						fir_list1->utc_nanosecond=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						fir_list1->sample_rata=app.app_para.fs;
						app.app_count.sample_times_orig_valid++;
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						//printf(RED"app.app_para.ad_ch_select=%d,ch=%d,datacount=%d\n"NONE,app.app_para.ad_ch_select,app.buffer.databuf[LOOPREAD]->data_start[tmp].ch,app.buffer.databuf[LOOPREAD]->data_start[tmp].datacount);
						//resample(data,1,100,&dataOut);
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						//fprintf(fp,"app.ad_count.read_times=%d\n",app.ad1256.ad_count.read_times);	
					}
					else{//空采通道,完成一次采样
						fir_list1->data[2]=fir_list1->data[2]-9800;
				        pthread_mutex_lock(&(app.app_mut[MUTEX_LOCK_IS_CAN_ALG]));
				        	app.app_sig.sig_can_alg++;
						pthread_mutex_unlock(&(app.app_mut[MUTEX_LOCK_IS_CAN_ALG]));

						aa = ADS1256_Setch_ReadData(0);
				//printf(GREEN"fir_list1->utc_second=%d,fir_list1->count=%d\n"NONE,fir_list1->utc_second,fir_list1->count);
						fir_list1=fir_list1->next;//每得到一个新采样，指针后移
						//printf(ADS1256_DEBUG_COLOR"[%d]:fir_list1_async->utc_second:%d,fir_list1_async->utc_nanosecond:%d\n"NONE,tid,fir_list1->utc_second,fir_list1->utc_nanosecond);

					}
					if(app.app_para.ad_ch_select==4){
						app.app_para.ad_ch_select=0;
					}
					else{
						app.app_para.ad_ch_select++;
					}
					app.app_sig.iswork=1;//告诉指示灯操作程序在工作中
					if((app.ad1256.ad_count.read_times%app.app_para.fs)==0 && app.app_para.ad_ch_select != 0)
					{	

						for(ifor=0;ifor<10;ifor++)
						{
							if(1==app.app_server.connect[ifor].is_connect){
	            				app.app_server.connect[ifor].sig_send++;
	            				//printf("ifor=%d,ad1282 sig_send=%d\n",ifor,app.app_server.connect[ifor].sig_send);
							}
						}
						app.app_sig.sig_data_save++;
						if(app.app_sig.sig_data_save>LOOPBUFNUM)
						{
							printf("over buf,app exit\n");
							//exit(0);
						}
						app.buffer.databuf[LOOPREAD]=app.buffer.databuf[LOOPREAD]->loopbufnext;
					}
				}
				else{
					aa = ADS1256_ReadData();	
				}
			}
		}
	}
	else{
		LOCAT();

		app.app_sig.sig_data_save=0;
		app.app_sig.sig_setad_ready=2;
		app.ad1256.run_ctrl.has_reset=1;
		ADS1256_WriteCmd(CMD_RESET);
		bsp_DelayUS(50);
		fengle();
	    ADS1256_config();
	    app_reset();//重置变量
	}
}












void read_data_setch_interupt(int tid)
{
	//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	static int count=0;
	static int has_sync=0;
	static int has_capture=0;
	int aa;
	int tmp;
	int ifor;

//
	int data[161];
	int dataOut,i;
	int mode=0;
	// for(i=0;i<161;i++)
	// {
	// 	data[i]=pow(-1.0,double(i))*i*i;
	// }
	
//











	// if(has_capture){
	// 	has_capture=0;
	// 	return;
	// }
	// else{
	// 	has_capture=1;
	// }
	//exit(0);
	int resample2_ret,ret;
	if(app.app_sig.sig_setad_ready==1)
	{
		app.ad1256.run_ctrl.has_reset=0;
		app.ad1256.run_ctrl.pre_read_count=0;
		app.ad1256.ad_count.read_times=0;
		app.ad1256.run_ctrl.has_read_id==0;
		app.ad1256.run_ctrl.has_sync=0;
		app.app_para.ad_ch_select=0;
		printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

	}
	if(app.ad1256.run_ctrl.has_reset==1){
		//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		if(app.ad1256.run_ctrl.has_read_id==0){
			printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
			ADS1256_ReadChipID_ISR();
			//app.ad1256.run_ctrl.has_read_id=1;
		}
		else{
			if(app.ad1256.run_ctrl.has_sync==0){
				printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
				if(app.ad1256.run_ctrl.pre_read_count==10)//app.ad1256.run_ctrl.pre_read_para
				{
					printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
					//app.ad1256.run_ctrl.has_reset=1;
					app.ad1256.ad_count.read_times=0;
					ADS1256_Setch_ReadData(0);
					app.app_para.ad_ch_select=0;
					alg_pre_count=0;
					app.ad1256.run_ctrl.has_sync=1;
				}
				else{
					printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
					//ADS1256_SetDiffChannal(0);	/* change DiffChannal */
					//ADS1256_SetChannal(7);	//Switch channel mode 
					if(app.ad1256.run_ctrl.pre_read_count==0){
						printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						bsp_DelayUS(5);
						ADS1256_WriteCmd(CMD_SYNC);
						bsp_DelayUS(5);
						ADS1256_WriteCmd(CMD_WAKEUP);
						bsp_DelayUS(25);
							//ADS1256_WriteCmd(CMD_RDATAC);
						bsp_DelayUS(5);
						ADS1256_SetDiffChannal(0);	//Switch channel mode 
						app.ad1256.run_ctrl.pre_read_count++;
					}
					else{
						printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						app.ad1256.run_ctrl.pre_read_count++;
						aa = ADS1256_ReadData();
					}
				}				
			}
			else{

				//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
				//exit(0);
				app.ad1256.run_ctrl.change_ch_lable=app.ad1256.run_ctrl.change_ch_lable==0?1:0;
				//printf(RED"app.ad1256.run_ctrl.change_ch_lable=%d\n"NONE,app.ad1256.run_ctrl.change_ch_lable);
				if(app.ad1256.run_ctrl.change_ch_lable==0){
					//aa = ADS1256_ReadData();
					//aa = ADS1256_Setch_ReadData(0);
					//app.buffer.databuf[LOOPREAD]->data_start[app.ad1256.ad_count.read_times%200].ad_data=ADS1256_Setch_ReadData(0);
					//app.app_para.ad_ch_select=(app.app_para.ad_ch_select==5)?0:(app.app_para.ad_ch_select++);

					if(app.app_para.ad_ch_select<4){
						tmp=app.ad1256.ad_count.read_times%app.app_para.fs;
						app.buffer.databuf[LOOPREAD]->data_start[tmp].datacount=app.ad1256.ad_count.read_times;
						app.buffer.databuf[LOOPREAD]->data_start[tmp].ch=(app.ad1256.ad_count.read_times)%app.app_para.ad_reg.reg[CTRL_OPEN_CHNUM];
						app.ad1256.ad_count.read_times++;

						clock_gettime(CLOCK_REALTIME, &app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time);
	//vapp->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_sec,app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_nsec
						if(app.ad1256.ad_count.read_times>1){
							if(app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec-last_sec){
								detat=1000000000+app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec-last_nsec;
								//printf(GREEN">>>>>>>detat=%ld\n"NONE,detat);
							}
							else{
								detat=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec-last_nsec;
								//printf(RED"detat=%ld\n"NONE,detat);
							}
							if(app.app_para.ad_ch_select==0){
								//printf(YELLOW"t=%d\n"NONE,(200/app.app_para.fs*3000000));

								if(detat>(200/app.app_para.fs*2000000)+(200/app.app_para.fs*1000000)*1){
									printf(YELLOW"detat=%ld,t=%d\n"NONE,detat,(200/app.app_para.fs*2000000)+(200/app.app_para.fs*1000000));
									exit(0);//15999635
								}	
							}
							else{
								//printf(BLUE"t=%d\n"NONE,(200/app.app_para.fs*1000000)+(200/app.app_para.fs*1000000));

								if(detat>((200/app.app_para.fs*1000000)+(200/app.app_para.fs*1000000)*1)){
									printf(YELLOW"detat=%ld,t=%d\n"NONE,detat,(200/app.app_para.fs*1000000)+(200/app.app_para.fs*1000000));
									exit(0);
								}	
							}

							//printf("detat=%d\n",detat);
							last_sec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
							last_nsec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
							//printf(BLUE"detat=%ld\n"NONE,detat);
						}
						else{
							printf(BLUE"detat=%ld\n"NONE,detat);
							last_sec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
							last_nsec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
						}


						//TODO change to list1



						app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data = ADS1256_Setch_ReadData(app.app_para.ad_ch_select+1);
						//app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data=10000;

						// fir_list->data[app.app_para.ad_ch_select]=5*app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data/8388607;
						// fir_list->utc_second=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
						// fir_list->utc_nanosecond=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
						// fir_list->sample_rata=app.app_para.fs;
						//printf("app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data=%d\n",app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data );
						//printf("app.app_para.gain=%d\n",app.app_para.gain);
						//printf("app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data=%d\n",app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data );
						float aaa=1.07*app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data;
						//ad=3927206,fir_list1=-1559.697144

						fir_list1->data[app.app_para.ad_ch_select]=9.8*((aaa*2.5*2.0)/(1.7474*0x494008*app.app_para.gain)*1000-2500);//5*app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data/8388607;
						//printf("app.app_para.ad_ch_select=%d,ad=%d,fir_list1=%f\n",app.app_para.ad_ch_select,aaa,fir_list1->data[app.app_para.ad_ch_select]);

						//printf("tmp=%d\n",tmp);
						fir_list1->utc_second=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						fir_list1->utc_nanosecond=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						fir_list1->sample_rata=app.app_para.fs;
						app.app_count.sample_times_orig_valid++;
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						//printf(RED"app.app_para.ad_ch_select=%d,ch=%d,datacount=%d\n"NONE,app.app_para.ad_ch_select,app.buffer.databuf[LOOPREAD]->data_start[tmp].ch,app.buffer.databuf[LOOPREAD]->data_start[tmp].datacount);
						//resample(data,1,100,&dataOut);
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						//fprintf(fp,"app.ad_count.read_times=%d\n",app.ad1256.ad_count.read_times);	
					}
					else{//空采通道,完成一次采样
						app.app_count.date_time.tv_sec=fir_list1->utc_second;
						app.app_count.date_time.tv_nsec=fir_list1->utc_nanosecond;
						app.app_count.sample_times_ch++;
						//printf(ADS1256_DEBUG_COLOR"[%d]:tv_sec:%d,tv_nsec:%d\n"NONE,tid,app.app_count.date_time.tv_sec,app.app_count.date_time.tv_nsec);

						// if(mode==0){
						// 	trig_wave();
						// }
						// else if(mode==2){
						// 	trig_info();

						// }
						// else if(mode==1){
						// 	alltime_wave();
						// }
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

						//printf(GREEN"app.app_para.ad_ch_select=%d\n"NONE,app.app_para.ad_ch_select);
						aa = ADS1256_Setch_ReadData(0);
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						fir_list1=fir_list1->next;//每得到一个新采样，指针后移
						alg_pre_count++;
						if(alg_pre_count>app.iws_para.firlen){//有一些数据后开始做重采样
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
							float p_result,tmp=0;
							resample2_ret=resample2(fir_list1,&app.iws_para.location,fir_list,&app.iws_para.first_time,2);
							//printf(ADS1256_DEBUG_COLOR"[%d]:fir_list1->utc_second:%d,fir_list1->utc_nanosecond:%d\n"NONE,tid,fir_list1->utc_second,fir_list1->utc_nanosecond);
							//>>触发测试
							// ret = slt_lltstru_2(fir_list,app.iws_para.slt_length,app.iws_para.llt_length,&p_result,app.iws_para.threshold,app.iws_para.trg_ch);
							// printf("ret=%d\n",ret);
							// ret = slt_lltstru(fir_list,app.iws_para.slt_length,app.iws_para.llt_length,&p_result,app.iws_para.threshold,app.iws_para.trg_ch);
							// printf("ret=%d\n",ret);
							if(slt_lltstru_2(fir_list,app.iws_para.slt_length,app.iws_para.llt_length,&p_result,app.iws_para.threshold,app.iws_para.trg_ch)==2){
								printf(RED"has triggered，ch=%d\n"NONE,app.app_para.ad_ch_select);
								
								//exit(0);
							}
							if(slt_lltstru(fir_list,app.iws_para.slt_length,app.iws_para.llt_length,&p_result,app.iws_para.threshold,app.iws_para.trg_ch)==2){
								printf(PURPLE"has triggered，ch=%d\n"NONE,app.app_para.ad_ch_select);
								//exit(0);
							}
							//<<触发测试


							//printf(RED"fir_list1->utc_second=%d\n"NONE,fir_list1->utc_second);
							
							int band_passstruct_ret=0;
							if(resample2_ret==2){//每得到一个重采样做相应的处理
								app.app_count.resample_times++;

								//printf("resample_times=%d,fir_list.count=%d\n",app.app_count.resample_times,fir_list->count);
								//int realwrite=mywrite(&fir_list->data[0],4,fp_aaaa);

								app.app_count.si_count_second++;
								//printf("app.app_count.si_count=%d\n",app.app_count.si_count);

								if(app.app_count.si_count==0){
									//printf(CYAN"fir_list->utc_second=%d\n"NONE,fir_list->utc_second);
									app.iws_status.utc_time[0]=fir_list->utc_second;
									//exit(0);
								}
								//printf(RED"1 app.iws_status.utc_time[0]=%d\n"NONE,app.iws_status.utc_time[0]);
								app.app_count.si_count++;

								for(ifor=0;ifor<3;ifor++){
									if(app.iws_status.max[ifor]<fir_list->data[ifor]){
										//printf(BLUE"fir_list->data[ifor]=%f\n"NONE,fir_list->data[ifor]);

										app.iws_status.max[ifor]=fir_list->data[ifor];
									}
									if(app.iws_status.min[ifor]>fir_list->data[ifor]){
										app.iws_status.min[ifor]=fir_list->data[ifor];
										//printf(GREEN"fir_list->data[ifor]=%f\n"NONE,fir_list->data[ifor]);
									}
									//printf(RED"fir_list->data[ifor]=%f\n"NONE,fir_list->data[ifor]);
								}
								//printf("app.app_count.si_count_second=%d\n",app.app_count.si_count_second);
								if(app.app_count.si_count_second==app.app_para.fs){
									//printf(GREEN"1 app.iws_status.utc_time[0]=%d\n"NONE,app.iws_status.utc_time[0]);
									//printf(LIGHT_CYAN"si_count_second=%d,app.iws_status.max[0]=%f,app.iws_status.min[0]=%f\n"NONE,\
									app.app_count.si_count_second,app.iws_status.max[0],app.iws_status.min[0]);
									//printf("app.app_count.si_count_second=%d,app.iws_status.min[0]=%f\n",app.app_count.si_count_second,app.iws_status.min[0]);



									app.iws_status.ud[app.app_count.si_count_times%10]=(int)(app.iws_status.max[0]-app.iws_status.min[0]);//app.app_count.si_count_times+1;//(int)(((app.iws_status.max[2]-app.iws_status.min[2])/5)*8388607);
									app.iws_status.ew[app.app_count.si_count_times%10]=(int)(app.iws_status.max[1]-app.iws_status.min[1]);//app.app_count.si_count_times+2;//(int)(((app.iws_status.max[1]-app.iws_status.min[1])/5)*8388607);
									app.iws_status.ns[app.app_count.si_count_times%10]=(int)(app.iws_status.max[2]-app.iws_status.min[2]);//app.app_count.si_count_times+3;//(int)(((app.iws_status.max[0]-app.iws_status.min[0])/5)*8388607);
									printf("max=%f,min=%f,max-min=%f,int max-min=%d\n",app.iws_status.max[0],app.iws_status.min[0],(app.iws_status.max[0]-app.iws_status.min[0]),(int)(app.iws_status.max[0]-app.iws_status.min[0]));
									app.iws_status.max[0]=app.iws_status.max[1]=app.iws_status.max[2]=-5;
									app.iws_status.min[0]=app.iws_status.min[1]=app.iws_status.min[2]=5;
									app.app_count.si_count_second=0;
									app.app_count.si_count_times++;

								}
								if(app.app_count.si_count==(app.app_para.fs*10)){
									//printf("yes\n");
									//printf(PURPLE"2 app.iws_status.utc_time[0]=%d\n"NONE,app.iws_status.utc_time[0]);
									iws_do_status_pre(fir_list,&app.iws_status);
									app.app_count.si_count=0;
									
								}


								//app.app_count.si_count++;
								//app.app_count.si_count_second++;

										// if(app.app_count.si_count==(app.app_para.fs*10)){
										// 	iws_do_status_pre(fir_list,&app.iws_status);
										// }
										// if((app.app_count.si_count_second%app.app_para.fs)==0){
										// 	app.app_count.si_count_second=0;
										// 	//iws_do_status_pre(fir_list,&app.iws_status);
										// 	for(ifor=0;ifor<10;ifor++){
										// 		app.iws_status.ud[ifor]=0;
										// 		app.iws_status.ew[ifor]=0;
										// 		app.iws_status.ns[ifor]=0;
										// 	}
										// 	app.app_count.si_count=0;
										// }

										//printf("app.app_count.si_count=%d\n",app.app_count.si_count);	
									if(app.is_trig==0){//发状态信息包
										// app.app_count.si_count++;
										// if(app.app_count.si_count==(app.app_para.fs*10)){
										// 	printf(RED"app.app_count.si_count=%d\n"NONE,app.app_count.si_count);
										// 	iws_do_status_pre(fir_list,&app.iws_status);
										// 	app.app_count.si_count=0;
										// }										// for(ifor=0;ifor<3;ifor++){
										// 	if(app.steim2_para.max[ifor]<fir_list->data[ifor]){
										// 		app.steim2_para.max[ifor]=fir_list->data[ifor];
										// 	}
										// 	if(app.steim2_para.min[ifor]>fir_list->data[ifor]){
										// 		app.steim2_para.min[ifor]=fir_list->data[ifor];
										// 	}
										// }
										// if((app.app_count.si_count%app.app_para.fs)==0){
										// 	//iws_do_status_pre(fir_list,&app.iws_status);
										// 	for(ifor=0;ifor<10;ifor++){
										// 		app.iws_status.ud[ifor]=0;
										// 		app.iws_status.ew[ifor]=0;
										// 		app.iws_status.ns[ifor]=0;
										// 	}
										// 	app.app_count.si_count=0;
										// }
									
									}
								#define wave 0
								#define wave_trig 1
								#define trig 2								
								if(mode==wave){//调用压缩算法
									
									iws_do_steim_pre(fir_list,&app.steim2_para);
									// band_passstruct_ret=band_passstruct(fir_list,2,&app.iws_para.band_passstruct_count,1/*TODO change*/);
									// if(slt_lltstru_2(fir_list,app.iws_para.slt_length,app.iws_para.llt_length,&p_result,app.iws_para.threshold,app.iws_para.trg_ch)==2){
									// 	printf(RED"has triggered，ch=%d\n"NONE,app.app_para.ad_ch_select);
									// 	app.is_trig=++;
									// 	//exit(0);
									// }
									// band_passstruct_ret=band_passstruct(fir_list,2,&app.iws_para.band_passstruct_count,1/*TODO change*/);
									// if(2==band_passstruct_ret){
									// 	printf(RED"band_passstruct_ret=%d\n"NONE,band_passstruct_ret);

									// }
									// if(app.is_trig>=1){//生成触发信息包
									// 	if(2==band_passstruct_ret){//触发
									// 		fir_list
									// 	}
									// }
									//printf(GREEN"fir_list->utc_second=%d\n"NONE,fir_list->utc_second);
								}
								else if(mode==wave_trig){
									band_passstruct_ret=band_passstruct(fir_list,2,&app.iws_para.band_passstruct_count,1/*TODO change*/);
									if(2==band_passstruct_ret){
										printf(RED"band_passstruct_ret=%d\n"NONE,band_passstruct_ret);

									}
								}
								else if(mode==trig){
									band_passstruct_ret=band_passstruct(fir_list,2,&app.iws_para.band_passstruct_count,1/*TODO*/);
									if(2==band_passstruct_ret){
										printf(RED"band_passstruct_ret=%d\n"NONE,band_passstruct_ret);

									}
								}

								// //>>触发测试
								// if(slt_lltstru_2(fir_list,app.iws_para.slt_length,app.iws_para.llt_length,&p_result,app.iws_para.threshold,0/*app.iws_para.trg_ch*/)==2){
								// 	printf(RED"has triggered，ch=%d\n"NONE,app.app_para.ad_ch_select);
									
								// 	//exit(0);
								// }
								// if(slt_lltstru(fir_list,app.iws_para.slt_length,app.iws_para.llt_length,&p_result,app.iws_para.threshold,0/*app.iws_para.trg_ch*/)==2){
								// 	printf(PURPLE"has triggered，ch=%d\n"NONE,app.app_para.ad_ch_select);
								// 	//exit(0);
								// }
								// //<<触发测试

								// if(app.app_count.resample_times<30*app.app_para.fs){//维护30秒前的数据指针
								// 	app.listbefore30sec=fir_list_head;
								// }
								// else{
								// 	if(app.is_trig==1){//如果触发了则更新触发最后时间
								// 		app.app_count.last_trig=app.app_count.resample_times;
								// 		if(app.is_trig_start==0){//开启触发处理过程
								// 			app.app_count.trig_start=app.app_count.resample_times;
								// 			int ifor;
								// 			for(ifor=0;ifor<6;ifor++)
								// 			{

								// 				if((app.iws_server[ifor].status.isconnect==1)&&(app.iws_server[ifor].mode==2)){
								// 					//app.buffer.steim2_out_buf[STEIM2_NOW];
								// 					app.iws_server[ifor].fir_list_trig_start=app.listbefore30sec;
								// 					printf(PURPLE"server %d ready app.iws_server[ifor].sig=%d\n"NONE,ifor,app.iws_server[ifor].sig);
								// 					//app.iws_server[ifor].sig++;
								// 				}
								// 			}
								// 			app.listbefore30sec=app.listbefore30sec->next;
								// 			//fir_list_trig_start
								// 			app.is_trig_start=1;//指示触发传波形开始
								// 		}
								// 	}
								// 	//iws_do_steim_pre(app.listbefore30sec,&app.steim2_para);
								// 	app.listbefore30sec=app.listbefore30sec->next;

								// 	//printf("app.listbefore30sec->count=%d\n",app.listbefore30sec->count);
								// }

								//app.listnow=
								app.listnow=fir_list;
								fir_list=fir_list->next;
								

								
								//printf("band_passstruct_ret=%d\n",band_passstruct_ret);





							}
							else{//忽略

							}
							//printf("resample2_ret=%d\n",resample2_ret);
						}
						if(0){//
							float p_result,tmp=0;
							//resample2(FIR_LIST1 *i_fir_list,int* location,FIR_LIST *i_result_list,int* first_time,int vector_type);
							int slt_length=6,llt_length=100,threshold=15;
							
							//band_passstruct(fir_list);
							int ifor;
							for(ifor=0;ifor<3;ifor++)
							{
								if(app.iws_para.i_l[ifor]==1){

									tmp+=(fir_list->data[ifor])*(fir_list->data[ifor]);
								}
							}

							fir_list->band_pass_data[app.app_para.ad_ch_select]=powf(tmp,0.5);
							if(alg_pre_count>400){
								//printf("alg_pre_count=%d\n",alg_pre_count);
								if(slt_lltstru_2(fir_list,app.iws_para.slt_length,app.iws_para.llt_length,&p_result,app.iws_para.threshold,app.iws_para.trg_ch)==2){
									printf(RED"has triggered，ch=%d\n"NONE,app.app_para.ad_ch_select);
									
									//exit(0);
								}
								if(slt_lltstru(fir_list,app.iws_para.slt_length,app.iws_para.llt_length,&p_result,app.iws_para.threshold,app.iws_para.trg_ch)==2){
									printf(PURPLE"has triggered，ch=%d\n"NONE,app.app_para.ad_ch_select);
									//exit(0);
								}
							}
							//printf("alg_pre_count=%d\n",alg_pre_count);
							//band_passstruct(fir_list_head);
						}
						// fir_list=fir_list->next;
						// fir_list1=fir_list1->next;
							//alg_pre_count++;
						//printf(RED"app.app_para.ad_ch_select=%d\n"NONE,app.app_para.ad_ch_select);
					}
















					if(app.app_para.ad_ch_select==4){

						app.app_para.ad_ch_select=0;
						
					}
					else{
						app.app_para.ad_ch_select++;
					}
					app.app_sig.iswork=1;//告诉指示灯操作程序在工作中
					if((app.ad1256.ad_count.read_times%app.app_para.fs)==0 && app.app_para.ad_ch_select != 0)
					{	
						//if(myoso->thread_status.tcp_data_status.isconnect==1){
						//myoso->oso_sig.sig_data_send++;
						//myoso->thread_status.tcp_data_status.isconnect=0;
						//printf(RED"myoso->thread_status.tcp_data_status.isconnect=0\n"NONE);
						//}
						for(ifor=0;ifor<10;ifor++)
						{
	            			//printf(BLUE"ifor=%d,app.app_server.connect[ifor].is_connect=%d\n"NONE,ifor,app.app_server.connect[ifor].is_connect);
							//p_app->app_server.connect[connect_index].is_connect=1;
							if(1==app.app_server.connect[ifor].is_connect){
	            				app.app_server.connect[ifor].sig_send++;
	            				printf("ifor=%d,ad1282 sig_send=%d\n",ifor,app.app_server.connect[ifor].sig_send);
							}
						}
						//if(app.app_sig.sig_is_data_connect==1)
						//{
							//app.app_sig.sig_data_send++;
						//printf(RED"function：%s,line:%d,app.app_sig.sig_data_send=%d\n"NONE,__FUNCTION__,__LINE__,app.app_sig.sig_data_send);
			
						//}
						//printf(RED"app.buffer.databuf[LOOPREAD].index=%d\n"NONE,app.buffer.databuf[LOOPREAD]->loopbufindex);

						app.app_sig.sig_data_save++;
						//printf("app.app_sig.sig_data_save=%d\n",app.app_sig.sig_data_save);
						if(app.app_sig.sig_data_save>LOOPBUFNUM)
						{
							printf("over buf,app exit\n");
							//exit(0);
						}
						//printf("app.ad1256.ad_count.read_times=%d,app.app_para.fs=%d\n",app.ad1256.ad_count.read_times,app.app_para.fs);
						//send_go_signal(1);
						//fwrite(app.buffer.databuf[LOOPREAD]->data_start, 1,200*sizeof(DATA_FRAME_GEO_S),fp);
						//printf(RED"app.buffer.databuf[LOOPREAD]=%d\n"NONE,app.buffer.databuf[LOOPREAD]);
						//printf(GREEN"app.ad1256.ad_count.read_times=%d\n"NONE,app.ad1256.ad_count.read_times);
						app.buffer.databuf[LOOPREAD]=app.buffer.databuf[LOOPREAD]->loopbufnext;
					}
						
				}
				else{
					aa = ADS1256_ReadData();	
				}
			}
			//clock_gettime(CLOCK_REALTIME, &app.time.first_data_time);        
    		//printf(GREEN"count=%d,start:CLOCK_REALTIME: %d, %d\n"NONE, app.AdcNow[7],app.time.first_data_time.tv_sec, app.time.first_data_time.tv_nsec); 
		}
	}
	else{
		LOCAT();

		app.app_sig.sig_data_save=0;
		app.app_sig.sig_setad_ready=2;
		app.ad1256.run_ctrl.has_reset=1;
		ADS1256_WriteCmd(CMD_RESET);
		bsp_DelayUS(50);
		//debug_stop();
		//exit(0);
		fengle();
	    ADS1256_config();
	    app_reset();//重置变量
	    LOCAT();

	    //ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_2000SPS);
	}
}


void read_data_setch_interupt_orig()
{
	//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
	static int count=0;
	static int has_sync=0;
	static int has_capture=0;
	int aa;
	int tmp;
	int ifor;

//
	int data[161];
	int dataOut,i;
	// for(i=0;i<161;i++)
	// {
	// 	data[i]=pow(-1.0,double(i))*i*i;
	// }
	
//











	// if(has_capture){
	// 	has_capture=0;
	// 	return;
	// }
	// else{
	// 	has_capture=1;
	// }
	//exit(0);
	if(app.app_sig.sig_setad_ready==1)
	{
		app.ad1256.run_ctrl.has_reset=0;
		app.ad1256.run_ctrl.pre_read_count=0;
		app.ad1256.ad_count.read_times=0;
		app.ad1256.run_ctrl.has_read_id==0;
		app.ad1256.run_ctrl.has_sync=0;
		app.app_para.ad_ch_select=0;
		printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);

	}
	if(app.ad1256.run_ctrl.has_reset==1){
		//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
		if(app.ad1256.run_ctrl.has_read_id==0){
			printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
			ADS1256_ReadChipID_ISR();
			app.ad1256.run_ctrl.has_read_id=1;
		}
		else{
			if(app.ad1256.run_ctrl.has_sync==0){
				printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
				if(app.ad1256.run_ctrl.pre_read_count==10)//app.ad1256.run_ctrl.pre_read_para
				{
					printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
					//app.ad1256.run_ctrl.has_reset=1;
					app.ad1256.ad_count.read_times=0;
					ADS1256_Setch_ReadData(0);
					app.app_para.ad_ch_select=0;
					alg_pre_count=0;
					app.ad1256.run_ctrl.has_sync=1;

				}
				else{
					printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
					//ADS1256_SetDiffChannal(0);	/* change DiffChannal */
					//ADS1256_SetChannal(7);	//Switch channel mode 
					if(app.ad1256.run_ctrl.pre_read_count==0){
						printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						bsp_DelayUS(5);
						ADS1256_WriteCmd(CMD_SYNC);
						bsp_DelayUS(5);
						ADS1256_WriteCmd(CMD_WAKEUP);
						bsp_DelayUS(25);
							//ADS1256_WriteCmd(CMD_RDATAC);
						bsp_DelayUS(5);
						ADS1256_SetDiffChannal(0);	//Switch channel mode 
						app.ad1256.run_ctrl.pre_read_count++;
					}
					else{
						printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						app.ad1256.run_ctrl.pre_read_count++;
						aa = ADS1256_ReadData();
					}
				}				
			}
			else{

				//printf(RED"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
				//exit(0);
				app.ad1256.run_ctrl.change_ch_lable=app.ad1256.run_ctrl.change_ch_lable==0?1:0;
				//printf(RED"app.ad1256.run_ctrl.change_ch_lable=%d\n"NONE,app.ad1256.run_ctrl.change_ch_lable);
				if(app.ad1256.run_ctrl.change_ch_lable==0){
					//aa = ADS1256_ReadData();
					//aa = ADS1256_Setch_ReadData(0);
					//app.buffer.databuf[LOOPREAD]->data_start[app.ad1256.ad_count.read_times%200].ad_data=ADS1256_Setch_ReadData(0);
					//app.app_para.ad_ch_select=(app.app_para.ad_ch_select==5)?0:(app.app_para.ad_ch_select++);

					if(app.app_para.ad_ch_select<4){
						tmp=app.ad1256.ad_count.read_times%app.app_para.fs;
						app.buffer.databuf[LOOPREAD]->data_start[tmp].datacount=app.ad1256.ad_count.read_times;
						app.buffer.databuf[LOOPREAD]->data_start[tmp].ch=(app.ad1256.ad_count.read_times)%app.app_para.ad_reg.reg[CTRL_OPEN_CHNUM];
						app.ad1256.ad_count.read_times++;
						clock_gettime(CLOCK_REALTIME, &app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time);
	//vapp->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_sec,app->buffer.databuf[LOOPSAVE]->data_start->date_time.tv_nsec
						if(app.ad1256.ad_count.read_times>1){
							if(app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec-last_sec){
								detat=1000000000+app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec-last_nsec;
								//printf(GREEN">>>>>>>detat=%ld\n"NONE,detat);
							}
							else{
								detat=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec-last_nsec;
								//printf(RED"detat=%ld\n"NONE,detat);
							}
							if(app.app_para.ad_ch_select==0){
								//printf(YELLOW"t=%d\n"NONE,(200/app.app_para.fs*3000000));

								if(detat>(200/app.app_para.fs*2000000)+(200/app.app_para.fs*1000000)*1){
									printf(YELLOW"detat=%ld,t=%d\n"NONE,detat,(200/app.app_para.fs*2000000)+(200/app.app_para.fs*1000000));
									//exit(0);//15999635
								}	
							}
							else{
								//printf(BLUE"t=%d\n"NONE,(200/app.app_para.fs*1000000)+(200/app.app_para.fs*1000000));

								if(detat>((200/app.app_para.fs*1000000)+(200/app.app_para.fs*1000000)*1)){
									printf(YELLOW"detat=%ld,t=%d\n"NONE,detat,(200/app.app_para.fs*1000000)+(200/app.app_para.fs*1000000));
									//exit(0);
								}	
							}

							//printf("detat=%d\n",detat);
							last_sec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
							last_nsec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
							//printf(BLUE"detat=%ld\n"NONE,detat);
						}
						else{
							printf(BLUE"detat=%ld\n"NONE,detat);
							last_sec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_sec;
							last_nsec=app.buffer.databuf[LOOPREAD]->data_start[tmp].date_time.tv_nsec;
						}






						app.buffer.databuf[LOOPREAD]->data_start[tmp].ad_data = ADS1256_Setch_ReadData(app.app_para.ad_ch_select+1);
						//printf(RED"app.app_para.ad_ch_select=%d,ch=%d,datacount=%d\n"NONE,app.app_para.ad_ch_select,app.buffer.databuf[LOOPREAD]->data_start[tmp].ch,app.buffer.databuf[LOOPREAD]->data_start[tmp].datacount);
						//resample(data,1,100,&dataOut);
						//printf(GREEN"function：%s,line:%d\n"NONE,__FUNCTION__,__LINE__);
						//fprintf(fp,"app.ad_count.read_times=%d\n",app.ad1256.ad_count.read_times);	
					}
					else{
						//printf(GREEN"app.app_para.ad_ch_select=%d\n"NONE,app.app_para.ad_ch_select);
						aa = ADS1256_Setch_ReadData(0);
						alg_pre_count++;
						if(alg_pre_count>300)
						{//
							//printf("alg_pre_count=%d\n",alg_pre_count);
							//band_passstruct(fir_list_head);
						}
							//alg_pre_count++;
						//printf(RED"app.app_para.ad_ch_select=%d\n"NONE,app.app_para.ad_ch_select);
					}
					if(app.app_para.ad_ch_select==4){
						app.app_para.ad_ch_select=0;
						
					}
					else{
						app.app_para.ad_ch_select++;
					}
					app.app_sig.iswork=1;//告诉指示灯操作程序在工作中
					if((app.ad1256.ad_count.read_times%app.app_para.fs)==0 && app.app_para.ad_ch_select != 0)
					{	
						//if(myoso->thread_status.tcp_data_status.isconnect==1){
						//myoso->oso_sig.sig_data_send++;
						//myoso->thread_status.tcp_data_status.isconnect=0;
						//printf(RED"myoso->thread_status.tcp_data_status.isconnect=0\n"NONE);
						//}
						for(ifor=0;ifor<10;ifor++)
						{
							//p_app->app_server.connect[connect_index].is_connect=1;
							if(app.app_server.connect[ifor].is_connect==1){
	            				app.app_server.connect[ifor].sig_send++;
							}
						}
						//if(app.app_sig.sig_is_data_connect==1)
						//{
							//app.app_sig.sig_data_send++;
						//printf(RED"function：%s,line:%d,app.app_sig.sig_data_send=%d\n"NONE,__FUNCTION__,__LINE__,app.app_sig.sig_data_send);
			
						//}
						printf(RED"app.buffer.databuf[LOOPREAD].index=%d\n"NONE,app.buffer.databuf[LOOPREAD]->loopbufindex);

						app.app_sig.sig_data_save++;
						//printf("app.app_sig.sig_data_save=%d\n",app.app_sig.sig_data_save);
						if(app.app_sig.sig_data_save>LOOPBUFNUM)
						{
							printf("over buf,app exit\n");
							//exit(0);
						}
						printf("app.ad1256.ad_count.read_times=%d,app.app_para.fs=%d\n",app.ad1256.ad_count.read_times,app.app_para.fs);
						//send_go_signal(1);
						//fwrite(app.buffer.databuf[LOOPREAD]->data_start, 1,200*sizeof(DATA_FRAME_GEO_S),fp);
						//printf(RED"app.buffer.databuf[LOOPREAD]=%d\n"NONE,app.buffer.databuf[LOOPREAD]);
						//printf(GREEN"app.ad1256.ad_count.read_times=%d\n"NONE,app.ad1256.ad_count.read_times);
						app.buffer.databuf[LOOPREAD]=app.buffer.databuf[LOOPREAD]->loopbufnext;
					}
						
				}
				else{
					aa = ADS1256_ReadData();	
				}
			}
			//clock_gettime(CLOCK_REALTIME, &app.time.first_data_time);        
    		//printf(GREEN"count=%d,start:CLOCK_REALTIME: %d, %d\n"NONE, app.AdcNow[7],app.time.first_data_time.tv_sec, app.time.first_data_time.tv_nsec); 
		}
	}
	else{
		app.app_sig.sig_data_save=0;
		app.app_sig.sig_setad_ready=2;
		app.ad1256.run_ctrl.has_reset=1;
		ADS1256_WriteCmd(CMD_RESET);
		bsp_DelayUS(50);
		//debug_stop();
		//exit(0);
		fengle();
	    ADS1256_config();
	    app_reset();//重置变量
	    //ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_2000SPS);
	}
}












int confirm_read()
{
	printf(GREEN"app.ad1256.ad_count.read_times=%d\n"NONE,app.ad1256.ad_count.read_times);
}







int button_push_count=0;
extern int oled_net_info1();
extern int oled_batt_info();

void button_call_back(void)
{
	button_push_count++;
	int iswitch=button_push_count%2;
		// switch(iswitch)
		// {
		// 	case 0:
		// 	oled_net_info1();
		// 	break;
		// 	case 1:
		// 	oled_batt_info();
		// 	break;
		// 	case 2:
		// 	oled_sd_info();
		// 	break;
		// 	case 3:
		// 	oled_gps_info();
		// 	break;
		// 	case 4:
		// 	//oled_batt_info();
		// 	break;
		// 	case 5:
		// 	//oled_batt_info();
		// 	break;
		// 	default:
		// 	break;
		// }
	//printf("button pushed \n");

}



int button_push()
{
	int icount=0;
	int cpux=0;
		//get_hight_Pri(cpux);
        if(wiringPiSetupGpio() < 0)return 1;
        pinMode(button,INPUT);
        pullUpDnControl(button,PUD_UP);
        if(wiringPiISR(button,INT_EDGE_FALLING,button_call_back) < 0)
        {
                printf("Unable to setup ISR \n");
        }
        // pinMode(isr1,INPUT);
        // pullUpDnControl(isr1,PUD_UP);
        // if(wiringPiISR(isr1,INT_EDGE_RISING,call_back_func) < 0)
        // {
        //         printf("Unable to setup ISR \n");
        // }
        printf("Interrupt test program\n");
     //    while(1){
     //    	//printf("count=%d\n",icount++);
     //    	bsp_DelayUS(1000000);
    	// }
        // while(1)
        // {
        //         if(flag)
        //         {
        //                 while(digitalRead(button) ==0);
        //                 printf("button press rdy_count=%d\n",rdy_count);
        //                 flag = 0;
        //         }
        // }
}



int data_rdy()
{
	int icount=0;
	int cpux=0;
		//get_hight_Pri(cpux);
        if(wiringPiSetup() < 0)return 1;
        pinMode(button,INPUT);
        pullUpDnControl(button,PUD_UP);
        if(wiringPiISR(button,INT_EDGE_FALLING,call_back_func) < 0)
        {
                printf("Unable to setup ISR \n");
        }
        // pinMode(isr1,INPUT);
        // pullUpDnControl(isr1,PUD_UP);
        // if(wiringPiISR(isr1,INT_EDGE_RISING,call_back_func) < 0)
        // {
        //         printf("Unable to setup ISR \n");
        // }
        printf("Interrupt test program\n");
        while(1){
        	//printf("count=%d\n",icount++);
        	bsp_DelayUS(1000000);
    	}
        // while(1)
        // {
        //         if(flag)
        //         {
        //                 while(digitalRead(button) ==0);
        //                 printf("button press rdy_count=%d\n",rdy_count);
        //                 flag = 0;
        //         }
        // }
}

