#ifndef _UBLOX_H
#define _UBLOX_H
typedef struct GPGGA_data{
char nmea[7]; 
float utcdatetime;//YMDHMS
float Latitude;
char NS[2]; //N/S
float Longitude; //dddmm.mmmm
char EW[2]; //E/W
int FS;
int NoSV;
float HDOP;
float msl;
char uMsl;
float Altref;
char uSep;
int DiffAge;
int DiffStation;
char cs;
}__attribute__ ((packed,aligned(1)))GPGGA;


typedef        struct _DATE_TIME                    //年月日时分秒毫秒
{
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    unsigned short msel;                        //millisecond
    unsigned short usel;                        //millisecond
    unsigned short nsel;                        //millisecond
} DATE_TIME;


struct MyTm
{
	int tm_sec;     
	int tm_min;     
	int tm_hour;    
	int tm_mday;    
	int tm_mon;     
	int tm_year;    
	int tm_zone;    
};

struct gps_data_t
{
	struct MyTm tm_time;    
	unsigned long ns_d;     
	unsigned long ns_m10000;
	unsigned long ew_d;     
	unsigned long ew_m10000;
	char ns;                
	char ew;                
	char status;            
	char card;              
	unsigned long speed100; 
	unsigned long cog100;   
	unsigned long count;    
	unsigned long height_d; 
	unsigned long height_m; 
};


typedef struct CMD_ECHO{
int cmdecho_num;
char unuse[6];
}__attribute__ ((packed,aligned(1)))CMD_ECHO;
typedef struct GPSRMC_str{
char nmea[7]; 
char utcdatetime[10];//YMDHMS
char Status[2]; //A/V
char Latitude[10]; //ddmm.mmmm
char NS[2]; //N/S
char Longitude[10]; //dddmm.mmmm
char EW[2]; //E/W
char Speed[5]; //速率000.0~999.9节
char Course[5]; //航向000.0~359.9度
char utcymdtime[6];
char mag1[10];
char mag2[2];
char mode[2];
}__attribute__ ((packed,aligned(1)))GPSRMC_str;


extern void handle_udp_gps(int fd);
extern void handle_gpsd_gps_nmea(int fd);


#endif