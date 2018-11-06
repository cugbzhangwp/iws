/*
*需要配置FPGA的模块需要包含此头文件
*/

typedef struct fpgareg{
	uint8_t rw;//读写模式
	uint8_t addr;//地写地址
	uint16_t dataw;//读写数据
	uint16_t datar;//读数据
}__attribute__ ((packed,aligned(1)))FPGAREG_S,*FPGAREG_T;


#define REGREAD 0x02
#define REGWRITE 0x01


extern int open_fpga(DEVICEFPGA);//--/dev/spi1.0 .etc
extern int set_adc1282(int fid_spi,para_t para);//orig send_para_spi0_multi1282
extern uint16_t rw_fpga_reg(int fid_spi,FPGAREG_T tx);//orig send_para_spi0_multi1282

