#ifndef _OLED_H
#define _OLED_H


extern int app_display_init();
extern void disp_on_oled(int dispmethod,char * displaybuffer);
extern int tcp_oled_callback ( int size,char * buf,void * buff );
#endif

