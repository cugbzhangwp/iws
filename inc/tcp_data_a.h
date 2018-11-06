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

#define LISTEN_NUM 2
#define RECV_BUF_SIZE 512
