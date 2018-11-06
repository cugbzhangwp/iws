#ifndef _PCF8574_H
#define _PCF8574_H

#define RST_2102_219 0
#define RST_MESH2102 1
#define RST_MESH 2
#define RST_GPS2102 3
#define RST_GPS 4
#define RST_OLED 5
#define RST_AD 6
#define RST_4G 7

#define ADDR_PCF8574 0x70





extern int set_pca9539(unsigned char * value);
#endif