#ifndef _PCA9539_H
#define _PCA9539_H


#define RELAY_ADDR 0x76
#define DERECT_REG_P0 6
#define DERECT_REG_P1 7
#define OUTPUT_REG_P0 2
#define OUTPUT_REG_P1 3
#define I2C_READ_BIT    0x01
#define I2C_WRITE_BIT   0x00


#define  RELAY  RPI_GPIO_P1_16     //P4
#define  SENSOR  RPI_GPIO_P1_22     //P4
#define RELAY_EN()     bcm2835_gpio_write(RELAY,HIGH)
#define RELAY_UNEN()     bcm2835_gpio_write(RELAY,LOW)
#define SENSOR_EN()     bcm2835_gpio_write(SENSOR,HIGH)
#define SENSOR_UNEN()     bcm2835_gpio_write(SENSOR,LOW)

extern int set_pca9539(unsigned char * value);
extern int bcm_open_pca9539(APP_S * app);
extern int set_pca9539_value(char * buf);
#endif