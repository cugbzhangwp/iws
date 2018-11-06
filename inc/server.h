#include "oso_common.h"
extern pthread_mutex_t mtx;
extern pthread_cond_t cond;
extern int mainctl,ctltotal;
extern para_s para;
extern FILE *fp_para;
#define PORT_HOST 5555 /* Port that will be opened */
#define UPSERVERADDR "192.168.10.200"
//#include "oso-n.h"

extern char* join3(char *s1, char *s2) ;
extern int send_to_spi0(uint8_t * tx_buff,uint8_t * rx_buff,int buff_size);





