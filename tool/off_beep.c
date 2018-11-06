#include <bcm2835.h>
#include <stdio.h>

#define BEEP RPI_V2_GPIO_P1_37
#define BEEP_ON() 	bcm2835_gpio_write(BEEP,LOW)
#define BEEP_OFF() 	bcm2835_gpio_write(BEEP,HIGH)



int beep_pin_init()
{
    bcm2835_gpio_fsel(BEEP, BCM2835_GPIO_FSEL_OUTP);//
    return 0;
}
int main(int argc,char ** argv)
{
	if (!bcm2835_init()){
		exit(1);
	}
	else{
		beep_pin_init();
	}
	BEEP_ON();
	return 0;
}

