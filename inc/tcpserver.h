#include "oso_common.h"

extern pthread_mutex_t mut,mtx,mut_gpio;
extern pthread_cond_t cond,cond_cilent,cond_gpio;
extern int mainctl,ctltotal;
extern para_s para;
extern FILE *fp_para;









typedef struct cmd_list{
struct cmd_list * next;
int cmd;
}cmdlist_s, *cmdlist_t;


typedef struct recive_list{
int a;//wait;
}recive_list_t;

typedef struct queue_cmd
{
    int header;
    int tail;
    int size;
    int capcity;
    void **_buf;
}queue_t;
extern int send_to_spi0(uint8_t * tx_buff,uint8_t * rx_buff,int buff_size);



int init_oso_head(oso * myoso);
int startfpga_1282(oso * myoso);
int client_connect(oso * myoso);
int send_get_spi0(oso * myoso);
int set_para_spi0(oso * myoso);
int send_para_spi0_multi1282(oso * myoso);
int send_para_spi0(oso * myoso);
int getfs_1282(int para_spi);
int getparas(oso * myoso);
int get_ch_gain(oso * myoso);

int init_acq(oso * myoso);













#define LISTEN_NUM 2
#define RECV_BUF_SIZE 2048
