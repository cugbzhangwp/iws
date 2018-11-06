#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include "oso_common.h"
//extern int count,startsend,startgettime,startgetdata,fdio,ctltotal,io_cmd,io_arg,io_do,read_send;//\B6\C1ȡ\CA\FD\BEݼ\C6\CA\FD\B1\E4\C1\BF

//#define ABL_FILE_EXT_NAME ".abl"//extensions; txt; file extensions
static const char * abl_file_ext_name=".abl";

extern int ctltotal;
