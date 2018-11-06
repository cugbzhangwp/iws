#ifndef LTC2943_H
#define LTC2943_H
#include <i2crw.h>
float LTC2943_code_to_coulombs(uint16_t adc_code, float resistor, uint16_t prescalar);
float LTC2943_code_to_mAh(uint16_t adc_code, float resistor, uint16_t prescalar );
float LTC2943_code_to_voltage(uint16_t adc_code);
float LTC2943_code_to_current(uint16_t adc_code, float resistor);
float LTC2943_code_to_kelvin_temperature(uint16_t adc_code);
float LTC2943_code_to_celcius_temperature(uint16_t adc_code);
int smbus_write(unsigned char command,unsigned char * value,int length);
int smbus_read(unsigned char command,unsigned char * value,int length);
extern float getvoltage1( float *);
extern float getvoltage2();
extern float gettemperature();
extern float getcurrent();
#endif  // LTC2943_H






