#ifndef COM_H
#define COM_H


#define BS 16*4000
#define BOOL int
#define TRUE 1
#define FALSE 0
#define DEBUGRUN 0
#define DEBUGPRINT(sentence) if(DEBUGRUN) sentence		

typedef struct oso_n_para{
	unsigned long  para_ad;
	unsigned long para_ch;
	int para_file_size;
	char para_ip_server[20];
	}para_s,*para_t;
int testnum;
#endif
