#include "zwpdebug.h"
#include "printcolor.h"
#include "oso_common.h"
#include <stdlib.h>
unsigned long file_write_count,data_read_count;//
extern pthread_mutex_t mtx,mut,mut_gpio;
extern pthread_cond_t cond,cond_cilent,cond_gpio;
extern int count,startsend,startgettime,startgetdata,fdio,ctltotal,io_cmd,io_arg,io_do,read_send;//\B6\C1ȡ\CA\FD\BEݼ\C6\CA\FD\B1\E4\C1\BF
extern para_s para;
extern char showstr[];
//extern  unsigned long long get_space_info(char * target);
	char file_time_name[20];	
	char file_name_temp[20];
