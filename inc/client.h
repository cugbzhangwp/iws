#include "zwpdebug.h"
#include "printcolor.h"
#include "oso_common.h"

#define PORT 6666 /* Open Port on Remote Host */


extern pthread_mutex_t mut,mtx,mut_gpio;
extern pthread_cond_t cond,cond_cilent,cond_gpio;
extern int fdio,ctltotal,io_cmd,io_arg,io_do,read_send;
extern char showstr[];
extern char file_time_name[20];
extern para_s para;


unsigned long data_send_count;


