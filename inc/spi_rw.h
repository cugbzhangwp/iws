#ifndef SPI_RW_H
#define SPI_RW_H
#include <stdint.h>
#include <stdio.h>
//#include <oso_common.h>
typedef struct spiconf{
	uint8_t mode;//spi模式
	uint8_t lsb;//spi模式
	uint8_t bits;//传输数据位数
	uint32_t speed;//时钟频率
	uint16_t delay;//发送延迟
}__attribute__ ((packed,aligned(1)))SPICONF_S, * SPICONF_T;

extern int open_spi(char * device,SPICONF_S * config);
extern int send_to_spi(int fid_spi,uint8_t * tx_buff,uint8_t * rx_buff ,int buff_size);
#endif
