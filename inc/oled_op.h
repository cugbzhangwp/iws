#include <spi_rw.h>
extern int open_oled(char * oleddevice,SPICONF_S * p_oledconfig);
extern void init_oled(int fid_spi);
extern void write2line(int fid_spi,char * dipbuf,int dispmethod);