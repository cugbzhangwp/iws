#ifndef __COMMON_TCP_H
#define __COMMON_TCP_H	
extern int readfromsocket(char * buffer,int needwritebyte,int fid);
extern int writetosocket(char * buffer,int needwritebyte,int fid);
#endif