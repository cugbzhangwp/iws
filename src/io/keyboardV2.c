#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>   
//#include <net/if.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <asm/types.h>  
#include <linux/netlink.h>  
#include <linux/rtnetlink.h>
#include <stdlib.h>  
#include <stdio.h>  
#include <sys/ioctl.h>  
#include <linux/if.h>  
#include <string.h>
#include <keyboard.h>
#include <version.h>

int keyprocess()
{
		printf(RED"keynum=%d\n"NONE,keynum);
		

}



int keyboard()
{
	char tmpstr[20];
	fd_keys = open("/dev/keys0", O_RDWR);//打开键盘
	if(fd_keys<0)
		printf("open /dev/gpioctl0 err!\n");
	while(ctltotal&&!(myoso->control.keyboard_loop))
	{	
		//printf("=====================p_oso->para_m.freq==============%d\n",p_oso->para_m.freq);
		txlen=read(fd_keys,&keynum,4);
		keyprocess(keynum);
	}
	printf(GREEN"YOU GUESS WHAT ? IT WORKS ,WE JUST FINISH MY JOB!-----KEYBOARD\n"NONE);
	myoso->status_bit.keyboard_status=0;
}
