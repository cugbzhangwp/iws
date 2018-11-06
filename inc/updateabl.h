#include <string.h>
typedef struct _UPDATE_HEAD
{
int pak_type;
int cmd;
int nextreadlen;
char symbol[8];
char checksum[2];
char md5[64];
char basepath[64];
char updatecmd[64];
char unuse[298];
}__attribute__ ((packed,aligned(1)))UPDATE_HEAD_S,*UPDATE_HEAD_T;
#define TARPAK 0 //
#define HEADTAR 1 //HAS HEAD
extern int check_updatefile(char *file_url,int type);
extern int update_anyname(char * file_url);
