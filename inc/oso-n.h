#include "oso_common.h"
#include "readconf.h"
//#define SSCDEBUG
//#define GPSDEBUG
//#define SERVERDEBUG
//#define CILENTDEBUG
//#define  GPIODEBUG
//#define TCPSERDEBUG
/*#define BS 16*4000
#define BOOL int
#define TRUE 1
#define FALSE 0
#define DEBUGRUN 1
#define DEBUGPRINT(sentence) if(DEBUGRUN) sentence	*/	





//###########functions that not in this file##############//
//in server.c
extern int udp_serverthread(oso *);
extern int gpio_ctl(oso *);

//in cilent.c
extern int tcp_server(oso *);
extern unsigned int*  read_data(oso * myoso);
extern char * get_gps_info(oso * myoso);
extern int udp_clientthread(oso * myoso);
extern unsigned int file_process(oso * myoso);
extern unsigned int  file_name(oso * myoso);
extern  int  display(oso * myoso);
//in read_data.c
//###########functions that not in this file##############//





//###########global vars that  in this file##############//
char *strtime,strname[20]="";
char showstr[100]="";
unsigned int* testbuf;
BOOL is_sdcard=FALSE;
pthread_mutex_t mut,mut_gpio;
pthread_mutex_t mtx;// = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond,cond_cilent,cond_gpio;// = PTHREAD_COND_INITIALIZER;
int count,startsend,startgettime,startgetdata,fdio,mainctl,ctltotal,io_cmd,io_arg,io_do,read_send;
	FILE *fp_para; //读取配置参数文件句柄
	para_s para;
//###########global vars that  in this file##############//

unsigned long long get_space_info(oso * myoso);
void init_oso(char * testbuf,oso * myoso);
