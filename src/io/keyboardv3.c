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
// #include <net/if.h>
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
#include <current.h>
//#define uchar (unsigned char)

/*
#define	KEY1_START_STOP AT91_PIN_PD25
#define	KEY2_DOWN AT91_PIN_PD26
#define	KEY3_UP AT91_PIN_PD27
#define	KEY4_SELECT AT91_PIN_PD28
#define	KEY5_MENU AT91_PIN_PD29
*/
extern CURRENT tx_current;
extern int sync_status;
extern float getvoltage1( float *);
extern float getvoltage2();
extern float gettemperature();
extern float getcurrent();
extern int getparas(oso * myoso);
extern unsigned short setch(oso *myoso,unsigned short para_ch_spi,int chnum);
extern int getlocalips(char * ipaddr);//获取本地网卡信息
extern int getconf();
extern startfpga_1282(oso * myoso);
extern int init_oso_head(oso * myoso);
extern menu(oso * myoso);
//定义在file_name.c中的外部变量>>>>>>>
extern int freqtable_index;
extern int current_algorithm;
extern int man_sync_wait_sec;
extern int freqtable_fpga[];
extern int freqtable_i[];
extern double freqtable_arm[];
extern char tcpdispstr[];
extern char datadispstr[];
extern char localip[];
extern char now_file_url[80];
//extern int command_spi(unsigned char cmd);
extern int ordertable[13];
extern struct gps_data_t gps_data;
extern int gobankhometime;//有任何按键此变量重置0
extern long tcp_server_tv_sec;//tcpserver.c中定义
extern int is_file_open;//file_process.c
extern struct timeval  tv_server;
extern PARA_INFO para_info[16];//指示每个通道的信息
extern void initoled(int bs);
extern char* readconf(char * envname);
//定义在file_name.c中的外部变量<<<<<<<
int refreshscreen(int * p_buf_index);
int not_man_sync_again=TRUE;//用户没有选择重复
pthread_t oledtmp;//屏幕持续刷新线程
int fd_keys,txlen;//键盘文件
int user_cancel=FALSE;//长时间等待时用户可以放弃
int is_auto_sync=OFF;//是否自动同步
int is_rm_all_file=NO;//是否删除文件
int super_user=FALSE;
int user_is_save_file=FALSE;//用户控制是否存储数据,
int is_syncing=NO;//标志是否正在同步等待中
char *transsetting_str[]={"order setting->","freq setting->","m_phz setting->","m_polynomial->"};
char *transplan_str[]={"week noise","Medium noise","STRONG noise"};
char *syncsetting_str[]={"Man sync->","Auto sync->"};
char *flashsetting_str[]={"clean storage->","format flash->","diaconnect flash->"};
char *setting_str[]={"trans setting-> ","sync setting-> ","flash setting-> ","about->       ","rx setting-> "};
char *rxsetting_str[]={"channel setting-> ","channel gain-> "};
char *transmode_str[]={"week noise","Medium noise","STRONG noise"};
char *aboutsetting_str[4]={FPGAVER,ARMVER};
char exstr[4]="   ";//{0xe1,0xe1,0xe1};
char *battinfo_str[]={"V: ","I: ","T: "};
char abl_sn[20];
char * abl_hwversion;
int transplan_now=0;
int is_sync_now=0;
int is_need_update_sdinfo=0;
int ifor_sync=0;//用户重复点击同步时用来重置等待时间
int change_transplan();
extern char dispbuff_in[20];//定义在display.c中
	pthread_mutex_t mut_vkey;
	pthread_cond_t cond_vkey;
	pthread_t vkey;
	pthread_t rkey;
	pthread_t newmenu;















ABL_TX_CMD user_para_m[3]={{0,7,13,127,0x7f,0x41,0,0,0,0,0},
							{0,7,14,127,0x7f,0x41,0,0,0,0,0},
							{0,8,14,255,0xff,0x8e,0,0,0,0,0}};
//ABL_TX_CMD user_para_m[3]={{0,11,4,2047,0x7ff,0x402,0,0,0,0,0},{0,7,13,2047,0x7f,0x41,0,0,0,0,0},{0,11,4,2047,0x7ff,0x402,0,0,0,0,0},{0,11,4,2047,0x7ff,0x402,0,0,0,0,0}};




oso * p_oso;
#define	KEY1_MENU 1
#define	KEY2_SELECT 2
#define	KEY3_UP 3
#define	KEY4_DOWN 4
#define	KEY5_START_STOP 5
ABL_TX_CMD abollo_para_m;
int nowindex=0,preindex;
int keynum,rekeynum;
int waitsec;
int is_transmitter=0;//控制fpga是否发射




typedef struct kbtab{
unsigned char keystateindex;//当前状态索引号
unsigned char  keymenustate;//按下menu键时转向的状态索引号
unsigned char  keyselectstate;//按下select键时转向的状态索引号
unsigned char  keyupstate;//按下up键时转向的状态索引号
unsigned char  keydownstate;//按下down键时转向的状态索引号
int  (* currentoperate)();//进入当前状态应该执行的功能*作
int  (* exitoperate)();//跳出当前状态应该执行的功能*作
}kbtab;





typedef struct kbtab_new{
unsigned char keystateindex;//当前状态索引号
unsigned char  keymenustate;//按下menu键时转向的状态索引号
unsigned char  keyselectstate;//按下select键时转向的状态索引号
unsigned char  keyupstate;//按下up键时转向的状态索引号
unsigned char  keydownstate;//按下down键时转向的状态索引号
int  (* currentoperate)();//进入当前状态应该执行的功能*作
int  (* exitoperate)();//跳出当前状态应该执行的功能*作
struct keyboard_table *key_table;
}kbtab_new;

kbtab oso_abollomenu_main_new[2] =
{
	{MAINSCREEN ,MAINMENU ,MAINSCREEN ,MAINSCREEN ,MAINSCREEN ,  mainjob1,dodefault ,&oso_abollomenu_main_new},
	{MAINSCREEN ,MAINMENU ,MAINSCREEN ,MAINSCREEN ,MAINSCREEN ,  mainjob1,dodefault ,&oso_abollomenu_main_new},
}



typedef struct keyboard_table{
int thisindex;
struct kbtab_new * table;
}keyboard_table;













int mainjob1(int para);
int dsppoint();
int dspcurve();
int dspkout();
int disclosedown();
int gpssetting_action(int i_para);
int transsetting_action(int i_para);
static int mainmenu_action(int i_para);
int freqsetting_action(int i_para);
int ordersetting_action(int i_para);
int syncsetting_action(int i_para);
int manusyncsetting_action(int i_para);
int autosyncsetting_action(int i_para);
int dodefault(int i_para);
int rmflashsetting_action(int i_para);
int formatflashsetting_action(int i_para);
int disconnectflashsetting_action(int i_para);
int flashsetting_action(int i_para);
int updatedispstr(int buf_index,char * str2disp,int startpos);
int aboutsetting_action(int para);
int fpgasetting_action(int i_para);
int rxsetting_action(int i_para);//rx
int chsetting_action(int i_para);//rx
int chgainsetting_action(int i_para);//rx
int getethstatus();//检测网卡状态
#define GPSSTATUS 10//GPS状态页
#define GPSSTATUS GPSSTATUS_1//M序列参数设置
# define size_of_keybd_menu 50  //菜单总长度


unsigned int sendsetting_index=0;//发射控制菜单索引
unsigned int rxsetting_index=0;
unsigned int setting_index=0;//主菜单索引
unsigned int setting_selected=0;
unsigned int freqsetting_index=0;
unsigned int freqsetting_selected=0;
unsigned int mainscreen_index=0;
unsigned int ordersetting_index=0;
unsigned int syncsetting_index=0;
unsigned int flashsetting_index=0;
unsigned int aboutsetting_index=0;
unsigned int transplan_index=0;
unsigned int autosyncswitch_index=0;//
unsigned int autosyncswitch_now=0;
unsigned int chswitch_index=0;
char eth_status_str[4];
struct state_index//当前状态索引
{
int preindex;
int nowindex;
int thisindex;
int keynum;
}menuindex={0,0,0};

kbtab * oso_abollomenu;

kbtab oso_abollomenu_main[size_of_keybd_menu] =
{
//当前状态 		按下menu	按下select	按下up		按下down
 {MAINSCREEN ,MAINMENU ,MAINSCREEN ,MAINSCREEN ,MAINSCREEN ,  mainjob1,( * dodefault) },//0
 {MAINMENU ,MAINSCREEN ,MAINMENU ,MAINMENU ,MAINMENU , (* mainmenu_action) ,(* dodefault) },   //1
 {SENDSETTING ,MAINSCREEN ,SENDSETTING ,SENDSETTING ,SENDSETTING , (* transsetting_action) ,(* dodefault) },   //2
 {SYNCSETTING ,MAINMENU ,SYNCSETTING ,SYNCSETTING ,SYNCSETTING , (* syncsetting_action) ,(* dodefault) },   //3
 {FLASHSETTING ,MAINMENU ,FLASHSETTING ,FLASHSETTING ,FLASHSETTING , (* flashsetting_action) ,(* dodefault) },   //4
 {ABOUTSETTING ,MAINMENU ,ABOUTSETTING ,ABOUTSETTING ,ABOUTSETTING , (* aboutsetting_action) ,(* dodefault) },   //4//5
 {RXSETTING ,MAINMENU ,RXSETTING ,RXSETTING ,RXSETTING , (* rxsetting_action) ,(* dodefault) },   //4//5
{},//7
{},//8
{},//9
 {SENDSETTING_FREQ ,SENDSETTING ,SENDSETTING_FREQ ,SENDSETTING_FREQ ,SENDSETTING_FREQ , (* freqsetting_action) ,(* dodefault) },   //11
 {SENDSETTING_FREQ ,SENDSETTING ,SENDSETTING_FREQ ,SENDSETTING_FREQ ,SENDSETTING_FREQ , (* freqsetting_action) ,(* dodefault) },   //11
 {SENDSETTING_ORDER ,SENDSETTING ,SENDSETTING_ORDER ,SENDSETTING_ORDER ,SENDSETTING_ORDER , (* ordersetting_action) ,(* dodefault) },   //13
 {SENDSETTING_FREQ ,SENDSETTING ,SENDSETTING_FREQ ,SENDSETTING_FREQ ,SENDSETTING_FREQ , (* freqsetting_action) ,(* dodefault) },   //14
 {SENDSETTING_FREQ ,SENDSETTING ,SENDSETTING_FREQ ,SENDSETTING_FREQ ,SENDSETTING_FREQ , (* freqsetting_action) ,(* dodefault) },   //15
 {SENDSETTING_FREQ ,SENDSETTING ,SENDSETTING_FREQ ,SENDSETTING_FREQ ,SENDSETTING_FREQ , (* freqsetting_action) ,(* dodefault) },   //16
{},
 {SENDSETTING_FREQ ,SENDSETTING ,SENDSETTING_FREQ ,SENDSETTING_FREQ ,SENDSETTING_FREQ , (* autosyncsetting_action) ,(* dodefault) },   //17
 {SYNCSETTING_MANU ,SYNCSETTING ,SYNCSETTING_MANU ,SYNCSETTING_MANU ,SYNCSETTING_MANU , (* manusyncsetting_action) ,(* dodefault) },   //19
 {SYNCSETTING_AUTO ,SYNCSETTING ,SYNCSETTING_AUTO ,SYNCSETTING_AUTO ,SYNCSETTING_AUTO , (* autosyncsetting_action) ,(* dodefault) },   //18
 {FLASHSETTING_RM ,FLASHSETTING ,FLASHSETTING_RM ,FLASHSETTING_RM ,FLASHSETTING_RM , (* rmflashsetting_action) ,(* dodefault) },   //20
 {FLASHSETTING_FORMAT ,FLASHSETTING ,FLASHSETTING_FORMAT ,FLASHSETTING_FORMAT ,FLASHSETTING_FORMAT , (* formatflashsetting_action) ,(* dodefault) },   //21
 {FLASHSETTING_DISCONNECT ,FLASHSETTING ,FLASHSETTING_DISCONNECT ,FLASHSETTING_DISCONNECT ,FLASHSETTING_DISCONNECT , (* disconnectflashsetting_action) ,(* dodefault) },   //22
{},//23
{},//24
{},//25
{},//26
{},//27
{},//28
 {FPGASETTING ,MAINSCREEN ,MAINSCREEN ,MAINSCREEN ,MAINSCREEN , (* fpgasetting_action) ,(* dodefault) },   //11,//29

 {30 ,MAINSCREEN ,30 ,30 ,30 , (* chsetting_action) ,(* dodefault) },  //30
 {31 ,MAINSCREEN ,31 ,31 ,31 , (* chgainsetting_action) ,(* dodefault) },//31
};



kbtab oso_rx_main[8] =
{
//当前状态 		按下menu	按下select	按下up		按下down
 {MAINSCREEN ,MAINMENU ,MAINSCREEN ,MAINSCREEN ,MAINSCREEN ,  mainjob1,( * dodefault) },//0
 {MAINMENU ,MAINSCREEN ,MAINMENU ,MAINMENU ,MAINMENU , (* mainmenu_action) ,(* dodefault) },   //1
 {SENDSETTING ,MAINSCREEN ,SENDSETTING ,SENDSETTING ,SENDSETTING , (* transsetting_action) ,(* dodefault) },   //2
 {SYNCSETTING ,MAINMENU ,SYNCSETTING ,SYNCSETTING ,SYNCSETTING , (* syncsetting_action) ,(* dodefault) },   //3
 {FLASHSETTING ,MAINMENU ,FLASHSETTING ,FLASHSETTING ,FLASHSETTING , (* flashsetting_action) ,(* dodefault) },   //4
 {ABOUTSETTING ,MAINMENU ,ABOUTSETTING ,ABOUTSETTING ,ABOUTSETTING , (* aboutsetting_action) ,(* dodefault) },   //4//5
 {RXSETTING ,MAINMENU ,RXSETTING ,RXSETTING ,RXSETTING , (* rxsetting_action) ,(* dodefault) },   //4//5
{},//7
{},//8
};









    #define BUFLEN 20480  
      
    int check_eth0()  
    {  
        int fd, retval;  
        char buf[BUFLEN] = {0};  
        int len = BUFLEN;  
        struct sockaddr_nl addr;  
        struct nlmsghdr *nh;  
        struct ifinfomsg *ifinfo;  
        struct rtattr *attr;  
      
        fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);  
        setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &len, sizeof(len));  
        memset(&addr, 0, sizeof(addr));  
        addr.nl_family = AF_NETLINK;  
        addr.nl_groups = RTNLGRP_LINK;  
        bind(fd, (struct sockaddr*)&addr, sizeof(addr));  
        while ((retval = read(fd, buf, BUFLEN)) > 0)  
        {  
		printf("ii\n");
            for (nh = (struct nlmsghdr *)buf; NLMSG_OK(nh, retval); nh = NLMSG_NEXT(nh, retval))  
            {  
		printf("22\n");
                if (nh->nlmsg_type == NLMSG_DONE)  
                    break;  
                else if (nh->nlmsg_type == NLMSG_ERROR)  
                    return;  
                else if (nh->nlmsg_type != RTM_NEWLINK)  
                    continue;  
                ifinfo = NLMSG_DATA(nh);  
                sprintf(eth_status_str,"%s",
                        (ifinfo->ifi_flags & IFF_LOWER_UP) ? "O" : "X" );
			update_net_info2ledbuff();
			isneedupdate=1;
                attr = (struct rtattr*)(((char*)nh) + NLMSG_SPACE(sizeof(*ifinfo)));  
                len = nh->nlmsg_len - NLMSG_SPACE(sizeof(*ifinfo));  
                for (; RTA_OK(attr, len); attr = RTA_NEXT(attr, len))  
                {  
                    if (attr->rta_type == IFLA_IFNAME)  
                    {  
		printf("33\n");
                        printf(" %s", (char*)RTA_DATA(attr));  
                        break;  
                    }  
                }  
                printf("\n");  
            }  
        }  
      close(fd);
        return 0;  
    }  





int fpgasetting_action(int i_para)
{

printf(RED"inside %s nowindex=%d\n"NONE, __FUNCTION__,nowindex);
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;
	char freqsetting_str[20];
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	switch(tmpkeynum)
	{


		case KEY1_MENU:
		nowindex=oso_abollomenu[nowindex].keymenustate;
		break;
		case KEY2_SELECT:
		nowindex=oso_abollomenu[nowindex].keystateindex;//+setting_index+1;
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		break;
		case KEY4_DOWN:
		nowindex=oso_abollomenu[nowindex].keydownstate;
		//ordersetting_index=ordersetting_index==8?0:++ordersetting_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
printf(RED"inside %s nowindex=%d\n"NONE, __FUNCTION__,nowindex);
refreshscreen(&nowindex);
}





struct ethtool_value {
        __uint32_t      cmd;
        __uint32_t      data;
};

int getethstatus()
{
    struct ethtool_value edata;
    int fd = -1, err = 0;
    struct ifreq ifr;

        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, "eth0");
        fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd < 0) {
                perror("Cannot get control socket");
                return 70;
        }

        edata.cmd = 0x0000000a;
        ifr.ifr_data = (caddr_t)&edata;
        err = ioctl(fd, 0x8946, &ifr);
        if (err == 0) {
	//printf("iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
               sprintf(eth_status_str, "%s",
                        edata.data ? "O":"X");
	
        } else if (errno != EOPNOTSUPP) {
                perror("Cannot get link status");
        }
	close(fd);
   return 0;
}




















int rmflashsetting_action(int i_para)
{
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;
	char freqsetting_str[20];
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	switch(tmpkeynum)
	{
		case KEY2_SELECT:
		nowindex=oso_abollomenu[nowindex].keystateindex;//+setting_index+1;
		//m_sync_manu(p_oso);	
		is_rm_all_file=1;
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		is_rm_all_file=0;
		//ordersetting_index=ordersetting_index==0?8:--ordersetting_index;
		break;
		case KEY4_DOWN:
		is_rm_all_file=0;
		nowindex=oso_abollomenu[nowindex].keydownstate;
		//ordersetting_index=ordersetting_index==8?0:++ordersetting_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
/*
int dspstrlen;
//dspstrlen=strlen(transsetting_str[setting_index]);
sprintf(freqsetting_str, "%5.2f", freqtable_arm[freqsetting_index]);
dspstrlen=strlen(freqsetting_str);
//sprintf(tmpstr, "%4.2f", freqtable_arm[freqsetting_index]);
memcpy(displaybuffer[nowindex]+40,"                    ",20);
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
memcpy(displaybuffer[nowindex]+40+2,freqsetting_str,dspstrlen);*/
refreshscreen(&nowindex);
}



int formatflashsetting_action(int i_para)
{
oled_writeline(displaybuffer[nowindex]+LINE2START,"DONE!",20);
isneedupdate=1;
}

int disconnectflashsetting_action(int i_para)
{
;//printf("do nothing \n");
}

int flashsetting_action(int i_para)
{
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;


	int preindex=i_para;
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	int this_index=nowindex;
	switch(tmpkeynum)
	{
		case KEY2_SELECT:
		nowindex=flashsetting_index+FLASHSETTING_BASE_SPACE;
		refreshscreen(&nowindex);///必须更新列出各个选项
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		flashsetting_index=flashsetting_index==0?2:--flashsetting_index;
		break;
		case KEY4_DOWN:
		nowindex=oso_abollomenu[nowindex].keydownstate;
		flashsetting_index=flashsetting_index==2?0:++flashsetting_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
int dspstrlen;
dspstrlen=strlen(flashsetting_str[flashsetting_index]);
//memcpy(displaybuffer[this_index]+LINE3START,"====================",20);
cleandispbuf(displaybuffer[this_index]+LINE3START,20);
memcpy(displaybuffer[this_index]+LINE3START+(int)floor((20-dspstrlen)/2),flashsetting_str[flashsetting_index],dspstrlen);
//printf(GREEN"flashsetting_str[flashsetting_index]=%s\n"NONE,flashsetting_str[flashsetting_index]);
}














int cleandispbuf(char * buf,int buflen)
{
int ifor;
for(ifor=0;ifor<=buflen-1;ifor++)
{
buf[ifor]=0x20;
//printf("mainscreen_index==============ifor=%d\n",ifor);
}
return 0;
}



int dodefault(int i_para)
{
;//printf("do nothing \n");

}






//定义信息显示常量
const char * ctrlhost="HOST:";
const char * datahost="DATA:";
const char * localipstr="IP:";

int aboutsetting_action(int para)
{
//printf("mainjob1 para=%d\n",para);
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;
	switch(menuindex.keynum)
	{
		case KEY2_SELECT:
		//nowindex=oso_abollomenu[nowindex].keystateindex+setting_index+1;
		//printf("nowindex=%d\n",nowindex);
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
			//printf("KEY3_UP 1=====================mainscreen_index==============%d\n",aboutsetting_index);
		aboutsetting_index=(aboutsetting_index==0)?1:--aboutsetting_index;
			//printf("KEY3_UP 2=====================mainscreen_index==============%d\n",aboutsetting_index);
		break;
		case KEY4_DOWN:
		nowindex=oso_abollomenu[nowindex].keydownstate;
		aboutsetting_index=(aboutsetting_index==1)?0:++aboutsetting_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
int dspstrlen;
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn==============%d\n",setting_index);
//dspstrlen=strlen(transsetting_str[setting_index]);

//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn1\n");
//memcpy(displaybuffer[menuindex.thisindex]+40,"                    ",20);
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
//memcpy(displaybuffer[menuindex.thisindex]+40+(int)floor((20-dspstrlen)/2),setting_str[setting_index],dspstrlen);
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn3\n");


			//printf("=====================aboutsetting_index==============%d\n",aboutsetting_index);

	switch(aboutsetting_index)
	{
		case 0:
			cleandispbuf((char *)displaybuffer[nowindex]+LINE2START,60);
			memcpy(displaybuffer[nowindex]+LINE2START,aboutsetting_str[0],strlen(aboutsetting_str[0]));
			//sprintf(tmpstr, "%5.2f", freqtable_arm[freqtable_i[p_oso->para_m.freq-1]-1]);
			//printf("=====================p_oso->para_m.freq==============%d\n",p_oso->para_m.freq);
			//memcpy(displaybuffer[0]+LINE2START+strlen("FREQ : "),tmpstr,strlen(tmpstr));
			//memcpy(displaybuffer[0]+LINE2START+strlen("FREQ : ")+strlen(tmpstr),"Hz",strlen("Hz"));
			memcpy(displaybuffer[nowindex]+LINE3START,aboutsetting_str[1],strlen(aboutsetting_str[1]));
			//sprintf(tmpstr, "%2d",p_oso->para_m.order);
			//memcpy(displaybuffer[0]+LINE3START+strlen("ORDER : "),tmpstr,strlen(tmpstr));
			//memcpy(displaybuffer[0]+LINE3START+strlen("ORDER : ")+strlen(tmpstr),"J",strlen("J"));

			memcpy(displaybuffer[nowindex]+LINE4START,aboutsetting_str[2],strlen(aboutsetting_str[2]));
			//sprintf(tmpstr, "%5.2f",(pow(2,p_oso->para_m.order)-1)/freqtable_arm[freqtable_index]);
			//memcpy(displaybuffer[0]+LINE4START+strlen("CYCLE : "),tmpstr,strlen(tmpstr));
			//memcpy(displaybuffer[0]+LINE4START+strlen("CYCLE : ")+strlen(tmpstr),"S",strlen("S"));

		break;
		case 1:
			//memcpy(displaybuffer[0]+LINE2START+strlen("FREQ : ")+strlen(tmpstr),"Hz",strlen("Hz"));
			/*memcpy(displaybuffer[0]+LINE3START,"FREE SD : ",strlen("FREE SD : "));
			sprintf(tmpstr, "%2.2f",(float)(p_oso->info.free_sd)/1024/1024/1024);
			printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[0]+LINE3START+strlen("FREE SD : "),tmpstr,strlen(tmpstr));
			memcpy(displaybuffer[0]+LINE3START+strlen("FREE SD : ")+strlen(tmpstr),"GB",strlen("GB"));*/
			cleandispbuf((char *)displaybuffer[nowindex]+LINE2START,60);
			memcpy(displaybuffer[nowindex]+LINE2START,abl_sn,strlen(abl_sn));
		break;
		default:
		mainscreen_index=0;
			cleandispbuf((char *)displaybuffer[0]+LINE2START,60);
		break;
	}


}


int oled_writeline(char * buf,char * str2disp,int len)
{
	char tmpstr[80];
	cleandispbuf(buf,20);
	//sprintf(buf, "%s",str2disp);
	memcpy(buf,str2disp,strlen(str2disp));
}



int update_gps_info2ledbuff()
{
	char tmpstr[80];
			cleandispbuf((char *)displaybuffer[0]+LINE1START,80);
			oled_writeline((char *)displaybuffer[0]+LINE1START,"   GPS INFO",20);
			//cleandispbuf((char *)displaybuffer[0]+LINE2START,60);

			//memcpy(displaybuffer[0]+LINE2START,battinfo_str[0],strlen(battinfo_str[0]));

			sprintf(tmpstr, "satellites: %d", gps_data.count);
			oled_writeline((char *)displaybuffer[0]+LINE2START,tmpstr,20);
			sprintf(tmpstr, "%c %03d.%d (%c)", gps_data.ns,gps_data.ns_d,gps_data.ns_m10000,gps_data.status);
			oled_writeline((char *)displaybuffer[0]+LINE3START,tmpstr,20);
			sprintf(tmpstr, "%c %03d.%d (%c)", gps_data.ew,gps_data.ew_d,gps_data.ew_m10000,gps_data.status);
			oled_writeline((char *)displaybuffer[0]+LINE4START,tmpstr,20);

}


int update_sync_info2ledbuff(int waitsec,int times)
{
	char tmpstr[80];
	int ifor;
			cleandispbuf((char *)displaybuffer[40]+LINE1START,80);
			oled_writeline((char *)displaybuffer[40]+LINE1START,"AUTO SYNCING...",20);
			//cleandispbuf((char *)displaybuffer[0]+LINE2START,60);
			sync_status=1;
			//memcpy(displaybuffer[0]+LINE2START,battinfo_str[0],strlen(battinfo_str[0]));

			myoled.nowscreennum=40;
			//printf(YELLOW"p_oso->signal_bit.is_sync=%d,waitsec=%d\n",p_oso->signal_bit.is_sync,waitsec);
			for(ifor=0;ifor<waitsec;ifor++)
				{
				if(not_man_sync_again){
					//printf(YELLOW"p_oso->signal_bit.is_sync=%d,waitsec=%d\n",p_oso->signal_bit.is_sync,waitsec-ifor);
					if(p_oso->signal_bit.is_sync==0){
					sprintf(tmpstr, "try %d time(s) sync",times);
					oled_writeline((char *)displaybuffer[40]+LINE2START,tmpstr,20);
					sprintf(tmpstr, "Please wait for %dS",waitsec-ifor);
					oled_writeline((char *)displaybuffer[40]+LINE3START,tmpstr,20);
					sprintf(tmpstr, "DON'T press any key");
					oled_writeline((char *)displaybuffer[40]+LINE4START,tmpstr,20);
					isneedupdate=1;
					sleep(1);//已一秒为单位
					//sprintf(tmpstr, "lon.%c %03d.%d (%c)", gps_data.ew,gps_data.ew_d,gps_data.ew_m10000,gps_data.status);
					//oled_writeline((char *)displaybuffer[0]+LINE4START,tmpstr,20);
						}
					else{
						sync_status=0;
						return 0;//同步成功退出
						}
					}
				else{
					//printf(YELLOW"USER CHOOSE MAN SYNC,SO WE RETURN 1\n");
					not_man_sync_again=1;//用户选择了手动同步,将其置位以便下次有效
					sync_status=0;
					return 1;//自动同步期间用户又选择了手动同步
					}
				}
	sync_status=0;
	return 0;
}






int update_time_info2ledbuff()
{
	char tmpstr[80];
			cleandispbuf((char *)displaybuffer[0]+LINE1START,80);
			oled_writeline((char *)displaybuffer[0]+LINE1START,"  TIME INFO",20);
			//cleandispbuf((char *)displaybuffer[0]+LINE2START,60);

			//memcpy(displaybuffer[0]+LINE2START,battinfo_str[0],strlen(battinfo_str[0]));

			sprintf(tmpstr, "%4d-%02d-%02d", gps_data.tm_time.tm_year,gps_data.tm_time.tm_mon,gps_data.tm_time.tm_mday);
			oled_writeline((char *)displaybuffer[0]+LINE2START,tmpstr,20);
			sprintf(tmpstr, "%02d:%02d:%02d", gps_data.tm_time.tm_hour,gps_data.tm_time.tm_min,gps_data.tm_time.tm_sec);
			oled_writeline((char *)displaybuffer[0]+LINE3START,tmpstr,20);
			//sprintf(tmpstr, "lon.%c %03d.%d (%c)", gps_data.ew,gps_data.ew_d,gps_data.ew_m10000,gps_data.status);
			//oled_writeline((char *)displaybuffer[0]+LINE4START,tmpstr,20);

}

int update_net_info2ledbuff()
{
	char tmpstr[80];
			getethstatus();//获取网卡信息
			//check_eth0();
			oled_writeline((char *)displaybuffer[0]+LINE1START,"   NET INFO",20);
			cleandispbuf((char *)displaybuffer[0]+LINE2START,60);
			sprintf(tmpstr, "%s%s", ctrlhost,tcpdispstr);
			memcpy(displaybuffer[0]+LINE2START,tmpstr,strlen(tmpstr));
			sprintf(tmpstr, "%s%s", datahost,datadispstr);
			memcpy(displaybuffer[0]+LINE3START,tmpstr,strlen(tmpstr));
			//printf("dddddddddddddddddddddddddddddddddddddddeth_status_str=%s\n",eth_status_str);
			sprintf(tmpstr, "%s%s(%s)", localipstr,localip,eth_status_str);
			memcpy(displaybuffer[0]+LINE4START,tmpstr,strlen(tmpstr));
			//displaybuffer[0][LINE4END]=')';
			//printf("ddddddddddddddddddddddddddddddddddddddddisplaybuffer[0]+LINE4START=%s\n",displaybuffer[0]+LINE4START);
}

int update_tx_super_user_info2ledbuff()
{
	char tmpstr[80];
			oled_writeline((char *)displaybuffer[0]+LINE1START,"    Tx INFO",20);
			cleandispbuf((char *)displaybuffer[0]+LINE2START,60);
			sprintf(tmpstr, "FREQ : %5.2f(Hz)", freqtable_arm[freqtable_i[p_oso->para_m.freq-1]-1]);
			//printf("=====================p_oso->para_m.freq==============%d\n",p_oso->para_m.freq);
			memcpy(displaybuffer[0]+LINE2START,tmpstr,strlen(tmpstr));
			sprintf(tmpstr, "ORDER : %2d",p_oso->para_m.order);
			memcpy(displaybuffer[0]+LINE3START,tmpstr,strlen(tmpstr));
			//sprintf(tmpstr, "CYCLE :%5.2f",(pow(2,p_oso->para_m.order)-1)/freqtable_arm[freqtable_index]);
			//sprintf(tmpstr, "CYCLE :%5.2f(S)",(pow(2,p_oso->para_m.order)-1)/freqtable_arm[freqtable_i[p_oso->para_m.freq-1]-1]);
			//sprintf(tmpstr, "CYCLE :%5.2f",(pow(2,p_oso->para_m.order)-1)/freqtable_arm[freqtable_i[p_oso->para_m.freq]]);
			//printf(BLUE"p_oso->para_m.order=%d,p_oso->para_m.freq=%d,freqtable_i[p_oso->para_m.freq]=%d\n"NONE,p_oso->para_m.order,p_oso->para_m.freq,freqtable_i[p_oso->para_m.freq]);
			//memcpy(displaybuffer[0]+LINE4START,tmpstr,strlen(tmpstr));
}




int update_tx_info2ledbuff()
{

//printf(BLUE"IN update_tx_info2ledbuff"NONE);
	char tmpstr[80];
			char *now;
			now=transplan_now==0?transmode_str[0]:transplan_now==1?transmode_str[1]:transplan_now==2?transmode_str[2]:"";
			oled_writeline((char *)displaybuffer[0]+LINE1START,"    Tx INFO",20);
			cleandispbuf((char *)displaybuffer[0]+LINE2START,60);
			sprintf(tmpstr, "Mode:%s", now);
			//printf("=====================p_oso->para_m.freq==============%d\n",p_oso->para_m.freq);
			memcpy(displaybuffer[0]+LINE2START,tmpstr,strlen(tmpstr));
			//sprintf(tmpstr, "ORDER : %2d",p_oso->para_m.order);
			//memcpy(displaybuffer[0]+LINE3START,tmpstr,strlen(tmpstr));
			//sprintf(tmpstr, "CYCLE :%5.2f",(pow(2,p_oso->para_m.order)-1)/freqtable_arm[freqtable_i[user_para_m[transplan_now].freq]]);
			sprintf(tmpstr, "Cycle:%5.2f(S)",(pow(2,p_oso->para_m.order)-1)/freqtable_arm[freqtable_i[p_oso->para_m.freq-1]-1]);
			//printf(BLUE"p_oso->para_m.order=%d,p_oso->para_m.freq=%d,freqtable_i[p_oso->para_m.freq]=%d,freqtable_arm[freqtable_i[p_oso->para_m.freq]]=%d\n"NONE,p_oso->para_m.order,(int)p_oso->para_m.freq,(int)freqtable_i[(int)p_oso->para_m.freq],freqtable_arm[freqtable_i[p_oso->para_m.freq]]);
			memcpy(displaybuffer[0]+LINE3START,tmpstr,strlen(tmpstr));
}

/*
int update_tx_info2ledbuff()
{
	char tmpstr[80];
			oled_writeline((char *)displaybuffer[0]+LINE1START,"    Tx INFO",20);
			cleandispbuf((char *)displaybuffer[0]+LINE2START,60);
			sprintf(tmpstr, "FREQ  : %5.2f(Hz)", freqtable_arm[freqtable_i[p_oso->para_m.freq-1]-1]);
			//printf("=====================p_oso->para_m.freq==============%d\n",p_oso->para_m.freq);
			memcpy(displaybuffer[0]+LINE2START,tmpstr,strlen(tmpstr));
			sprintf(tmpstr, "ORDER : %2d",p_oso->para_m.order);
			memcpy(displaybuffer[0]+LINE3START,tmpstr,strlen(tmpstr));
}


*/




int exit_abl()
{


//ifconfig|grep addr:|awk -F ':' '{print $2}'|awk '{print $1}'
	system("stopabl");
	//system("showftp");
	//close(fd_keys);
	//system("keyalone");
	exit(0);
//00028_00-01-01-082341.abl
}







int poweroff_abl()
{


//ifconfig|grep addr:|awk -F ':' '{print $2}'|awk '{print $1}'
	p_oso->is_save_file=0;
	ctltotal=0;
	usleep(200000);
	ioctl(fdio,GPIO_IOC_N_FPGA_M_SWITCH,OFF);
	p_oso->control.file_loop=1;//结束文件存储线程
	p_oso->control.oled_display_loop=1;
	p_oso->control.gps_loop=1;
	p_oso->control.gpio_loop=1;
	p_oso->control.ad_loop=1;
	p_oso->control.tcp_server_loop=1;
	p_oso->control.udp_beat_loop=1;
	p_oso->control.udp_data_loop=1;
	p_oso->control.tcp_data_loop=1;

	p_oso->control.file_name_loop=1;
	pthread_mutex_lock(&(p_oso->mut[MUT_CLIENT_FILE]));//通知线程继续往下执行
	pthread_cond_broadcast(&(p_oso->cond[COND_CLIENT_FILE]), &(p_oso->mut[MUT_CLIENT_FILE]));
	pthread_mutex_unlock(&(p_oso->mut[MUT_CLIENT_FILE]));
	tv_server.tv_sec=0;


	p_oso->control.file_usyn_close_loop=1;
	system("halt -d 4");
	sleep(1);

}


int superpasswd[8]={1,2,3,4,2,2,3,4};

int read_ifacqexit()
{
				char tmpstr[80];
				cleandispbuf((char *)displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,60);
				sprintf(tmpstr, " close ACQ...");
				memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,tmpstr,strlen(tmpstr));
				isneedupdate=1;
				if ( pthread_create( &oledtmp, NULL, exit_abl, NULL) ){
					printf(YELLOW"error creating tcp_server."NONE);
					abort();
					}
				else{
					;//printf(YELLOW"IS SYNCING!PLEASE WAIT!\n"NONE);
					}
				sleep(4);
				ioctl(fdio,GPIO_IOC_N_LED_CLOSE_ALL,0);
				cleandispbuf((char *)displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,60);
				sprintf(tmpstr, "DONE!");
				memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,tmpstr,strlen(tmpstr));
				sprintf(tmpstr, "you CAN press POWER-");
				memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE3START,tmpstr,strlen(tmpstr));
				sprintf(tmpstr, "POWER OFF");
				memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE4START,tmpstr,strlen(tmpstr));
				isneedupdate=1;
				command_spi(0x08); //display off, cursor off, blink off
				sleep(1);

				sleep(5);
				;//here do something power off!

}

int ifacqreset()
{
				char tmpstr[80];
				cleandispbuf((char *)displaybuffer[DISP_SCREEN_POWEROFF]+LINE1START,80);
				sprintf(tmpstr, "reset ACQ");
				memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,tmpstr,strlen(tmpstr));
				sprintf(tmpstr, "PLEASE WAIT...");
				memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE3START,tmpstr,strlen(tmpstr));
				isneedupdate=1;
				sleep(1);
}

int read_passwd()

{
				char tmpstr[80];
				if(keynum==KEY4_DOWN){//third times down
					int ifor;
					for(ifor=0;ifor<sizeof(superpasswd)/sizeof(int);ifor++)
						{
					//TODO 
						rekeynum=vreadkey();
						//txlen=read(fd_keys,&keynum,4);
						if(keynum!=superpasswd[ifor])
							break;
						}
						printf(RED"sizeof(superpasswd)/sizeof(int)=%d,ifor=%d,keynum=%d\n"NONE,sizeof(superpasswd)/sizeof(int),ifor,keynum);
					if(ifor==(sizeof(superpasswd)/sizeof(int))){
						super_user=super_user==TRUE?FALSE:TRUE;
						printf(RED"CHANGE TO SUPER USER!\n"NONE);
						}
					}
				else if(keynum==KEY3_UP){//third times down
						;
					}
}

int down_next()
{
				char tmpstr[80];
			printf(BLUE"YOU PREES UP\n"NONE);
			cleandispbuf((char *)displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,60);
			//memcpy(displaybuffer[0]+LINE2START,"FREE SD : ",strlen("FREE SD : "));
			sprintf(tmpstr, "press UP to ""EXIT ACQ""");
			//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,tmpstr,strlen(tmpstr));
			isneedupdate=1;
					//TODO 
						rekeynum=vreadkey();
						//txlen=read(fd_keys,&keynum,4);
			if(keynum==KEY3_UP){
				read_ifacqexit();
				}
			if(keynum==KEY4_DOWN){//second times down
					//TODO 
						rekeynum=vreadkey();
						//txlen=read(fd_keys,&keynum,4);
				read_passwd();
				}
}

int up_next()
{
			char tmpstr[80];
			cleandispbuf((char *)displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,60);
		//memcpy(displaybuffer[0]+LINE2START,"FREE SD : ",strlen("FREE SD : "));
			sprintf(tmpstr, "press DOWN to sync");
		//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,tmpstr,strlen(tmpstr));
			sprintf(tmpstr, "press UP to change");
		//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE3START,tmpstr,strlen(tmpstr));
			isneedupdate=1;
					//TODO 
						rekeynum=vreadkey();
						//txlen=read(fd_keys,&keynum,4);
				if(keynum==KEY4_DOWN)
					{
					cleandispbuf((char *)displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,60);
					sprintf(tmpstr, " syncing.........");
					memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,tmpstr,strlen(tmpstr));
					isneedupdate=1;
					if(is_syncing==NO){
						if(p_oso->signal_bit.is_sync==1){//如果重新同步则等待2秒
							m_sync_manu(p_oso);	
							sleep(1);//等待2秒以使signal_bit.is_sync置0
							ifor_sync=0;
							//printf(YELLOW"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYsignal_bit.is_sync=%d\n"NONE,p_oso->signal_bit.is_sync);
							}
						else
						m_sync_manu(p_oso);	
						is_sync_now=1;
						not_man_sync_again=0;//用户选择了手动同步,通知正在自动同步的线程结束;
						user_para_m[transplan_now].m_sync_Sec=is_auto_sync;//将这两个变量借位置存储在m序列的配置文件中以便下次恢复
						user_para_m[transplan_now].m_sync_mSec=(short)transplan_now;
						update_m_parafile_A(&user_para_m[transplan_now]);
						}
						if(is_syncing==NO){
							int tmp=SYNCSETTING_MANU;
							//myoled.nowscreennum=tmp;
							if ( pthread_create( &oledtmp, NULL, refreshscreen, &tmp) ){
								printf(YELLOW"error creating tcp_server."NONE);
								abort();
								}
							else{
								;//printf(YELLOW"IS SYNCING!PLEASE WAIT!\n"NONE);
								}
							}
					}
				else if(keynum==KEY3_UP){
					transplan_index=transplan_index==2?0:++transplan_index;
					change_transplan();
					}
}
int showhotkey()//select hotkey
{
		char down=0xe0;
		char up=0xde;
		char tmpstr[80];
		nowindex=oso_abollomenu[nowindex].keystateindex;
		myoled.nowscreennum=DISP_SCREEN_POWEROFF;
		oled_writeline((char *)displaybuffer[DISP_SCREEN_POWEROFF]+LINE1START,"HOT KEY:",20);
		cleandispbuf((char *)displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,60);
		//memcpy(displaybuffer[0]+LINE2START,"FREE SD : ",strlen("FREE SD : "));
		sprintf(tmpstr, "%c%c sync",up,down);
		//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
		memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,tmpstr,strlen(tmpstr));
		sprintf(tmpstr, "%c%c close ACQ",down,up);
		//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
		memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE3START,tmpstr,strlen(tmpstr));
		sprintf(tmpstr, "%c%c change plan",up,up);
		//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
		memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE4START,tmpstr,strlen(tmpstr));
		isneedupdate=1;

}
int s_s_d_next()
{
		if(keynum==KEY2_SELECT){
			;	
			}
		else if(keynum==KEY3_UP){//2nd select
					
			//TODO
			;
			}
		else if(keynum==KEY4_DOWN){//reset acq
			ifacqreset();
			system("sync");
			//TODO
			exit(0);
			}

}

int select_next()
{
			char tmpstr[80];
			cleandispbuf((char *)displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,60);
		//memcpy(displaybuffer[0]+LINE2START,"FREE SD : ",strlen("FREE SD : "));
			sprintf(tmpstr, "press UP change(c)");
		//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,tmpstr,strlen(tmpstr));
			sprintf(tmpstr, "press DOWN to reset");
		//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE3START,tmpstr,strlen(tmpstr));
			isneedupdate=1;
					//TODO 
						rekeynum=vreadkey();
						//txlen=read(fd_keys,&keynum,4);
				if(keynum==KEY2_SELECT){
					initoled(0);//重置oled
					system("source /etc/abollo.conf");
					getconf();
					
					printf(RED"YOU JUST RESET THE SCREEN\n"NONE);
					}
				else if(keynum==KEY3_UP){//2nd select
					
					//TODO
					current_algorithm=(current_algorithm==0)?1:0;
					printf(RED"YOU JUST change the current algorithm current_algorithm=%d\n"NONE,current_algorithm);
					}
				else if(keynum==KEY4_DOWN){//reset acq
						cleandispbuf((char *)displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,60);
						memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE2START,tmpstr,strlen(tmpstr));
						sprintf(tmpstr, "press DOWN to reset");
					//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
						//memcpy(displaybuffer[DISP_SCREEN_POWEROFF]+LINE3START,tmpstr,strlen(tmpstr));
						isneedupdate=1;
					//TODO 
						rekeynum=vreadkey();
						//txlen=read(fd_keys,&keynum,4);
						s_s_d_next();
					}


}


int update_sd_info()
{
			char tmpstr[80];
			oled_writeline((char *)displaybuffer[0]+LINE1START,"    SD INFO",20);
			cleandispbuf((char *)displaybuffer[0]+LINE2START,60);
			//memcpy(displaybuffer[0]+LINE2START,"FREE SD : ",strlen("FREE SD : "));
			sprintf(tmpstr, "FREE SD :%2.4fGB",(float)(p_oso->info.free_sd)/1024/1024/1024);
			//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[0]+LINE2START,tmpstr,strlen(tmpstr));
			sprintf(tmpstr, "TOTAL SD:%2.4fGB",(float)(p_oso->info.free_mem)/1024/1024/1024);
			oled_writeline((char *)displaybuffer[0]+LINE3START,tmpstr,20);
			sprintf(tmpstr, "REMAIN: %4.1f (min)",(float)((p_oso->info.free_sd)/76800512)*20);
			oled_writeline((char *)displaybuffer[0]+LINE4START,tmpstr,20);

}
int update_tx_info()
{
			char tmpstr[80];
			int ifor;
			oled_writeline((char *)displaybuffer[0]+LINE1START,"    Rx INFO",20);
			cleandispbuf((char *)displaybuffer[0]+LINE2START,60);
			sprintf(tmpstr, "FS : %4dHz",p_oso->oso_head->iFs);
			//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[0]+LINE2START,tmpstr,strlen(tmpstr));
			memset(tmpstr,0,80);
			sprintf(tmpstr, "CH :");
			for(ifor=0;ifor<16;ifor++)
				{
				//printf(GREEN"para_info[%d].ch_isopen=%d"NONE,ifor,para_info[ifor].ch_isopen);
				if(para_info[ifor].ch_isopen==1)
								sprintf(tmpstr, "%s%d ",tmpstr,ifor+1);
				}
			memcpy(displaybuffer[0]+LINE3START,tmpstr,strlen(tmpstr));

}


int update_batt_info()
{
			float *ftemp , ss,stemp;
			ftemp = &ss;
			char tmpstr[80];
			oled_writeline((char *)displaybuffer[0]+LINE1START,"  BATT INFO",20);
			cleandispbuf((char *)displaybuffer[0]+LINE2START,60);
			memcpy(displaybuffer[0]+LINE2START,battinfo_str[0],strlen(battinfo_str[0]));
			sprintf(tmpstr, "%2.2f", getvoltage1(ftemp));
			//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[0]+LINE2START+strlen(battinfo_str[0]),tmpstr,strlen(tmpstr));
			memcpy(displaybuffer[0]+LINE2START+strlen(battinfo_str[0])+strlen(tmpstr)," (V)",strlen(" (V)"));

			memcpy(displaybuffer[0]+LINE3START,battinfo_str[1],strlen(battinfo_str[1]));
			sprintf(tmpstr, "%2.2f", -1000*1000*getcurrent());
			//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[0]+LINE3START+strlen(battinfo_str[0]),tmpstr,strlen(tmpstr));
			memcpy(displaybuffer[0]+LINE3START+strlen(battinfo_str[0])+strlen(tmpstr)," (mA)",strlen(" (mA)"));

			memcpy(displaybuffer[0]+LINE4START,battinfo_str[2],strlen(battinfo_str[2]));
			sprintf(tmpstr , "%2.2f", gettemperature());
			//printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[0]+LINE4START+strlen(battinfo_str[0]),tmpstr,strlen(tmpstr));
			memcpy(displaybuffer[0]+LINE4START+strlen(battinfo_str[0])+strlen(tmpstr)," (C)",strlen(" (C)"));

}

int mainjob1(int para)
{
	//printf(RED"inside %s\n"NONE, __FUNCTION__);
//printf("mainjob1 para=%d\n",para);
	char tmpstr[80];

	int ifor,txlen;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;
	switch(menuindex.keynum)
	{
		case KEY2_SELECT:
			showhotkey();
					//TODO 
						rekeynum=vreadkey();
						//txlen=read(fd_keys,&keynum,4);
			if(keynum==KEY4_DOWN)
				{
				down_next();
				}
			else if(keynum==KEY3_UP){
				up_next();
				}
			else if(keynum==KEY2_SELECT){//2nd select
				select_next();			
				}
			break;
		case KEY3_UP:
			nowindex=oso_abollomenu[nowindex].keyupstate;
				//printf("KEY3_UP 1=====================mainscreen_index==============%d\n",mainscreen_index);
			mainscreen_index=(mainscreen_index==0)?6:--mainscreen_index;
		break;
		case KEY4_DOWN:
			nowindex=oso_abollomenu[nowindex].keydownstate;
			mainscreen_index=(mainscreen_index==6)?0:++mainscreen_index;
		break;
		default:
		break;
	}
int dspstrlen;
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn==============%d\n",setting_index);
//dspstrlen=strlen(transsetting_str[setting_index]);

//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn1\n");
//memcpy(displaybuffer[menuindex.thisindex]+40,"                    ",20);
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
//memcpy(displaybuffer[menuindex.thisindex]+40+(int)floor((20-dspstrlen)/2),setting_str[setting_index],dspstrlen);
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn3\n");


			//printf(RED"=====================mainscreen_index==============%d\n"NONE,mainscreen_index);

	switch(mainscreen_index)
	{
		case 0:
		if(super_user){
		update_tx_super_user_info2ledbuff();
			}
		else{
		update_tx_info2ledbuff();

			}
		break;
		case 1:
			getlocalips(localip) ;
			update_net_info2ledbuff();
			//memcpy(displaybuffer[0]+LINE2START+strlen("FREQ : ")+strlen(tmpstr),"Hz",strlen("Hz"));
			/*memcpy(displaybuffer[0]+LINE3START,"FREE SD : ",strlen("FREE SD : "));
			sprintf(tmpstr, "%2.2f",(float)(p_oso->info.free_sd)/1024/1024/1024);
			printf("=====================p_oso->info.free_sd==============%llu\n",p_oso->info.free_sd);
			memcpy(displaybuffer[0]+LINE3START+strlen("FREE SD : "),tmpstr,strlen(tmpstr));
			memcpy(displaybuffer[0]+LINE3START+strlen("FREE SD : ")+strlen(tmpstr),"GB",strlen("GB"));*/

		break;
		case 2:
			update_sd_info();

		break;
		case 3:
			update_tx_info();
		break;
		case 4:
			update_batt_info();

		break;
		case 5:
		update_gps_info2ledbuff();

		break;
		case 6:
		update_time_info2ledbuff();

		break;

		default:
		mainscreen_index=0;
			cleandispbuf((char *)displaybuffer[0]+LINE2START,60);
		break;
	}


}
















int dsppoint()
{
printf("dsppoint\n");

}
int dspcurve()
{
printf("dspcurve\n");

}
int dspkout()
{
printf("dspkout\n");

}
int disclosedown()
{
printf("disclosedown\n");

}



int manusyncsetting_action(int i_para)
{
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;
	char freqsetting_str[20];
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	switch(tmpkeynum)
	{
		case KEY2_SELECT:
		if(is_syncing==NO){
			nowindex=oso_abollomenu[nowindex].keystateindex;//+setting_index+1;
			if(p_oso->signal_bit.is_sync==1){//如果重新同步则等待2秒
				m_sync_manu(p_oso);	
				sleep(1);//等待2秒以使signal_bit.is_sync置0
				ifor_sync=0;
				//printf(YELLOW"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYsignal_bit.is_sync=%d\n"NONE,p_oso->signal_bit.is_sync);
				}
			else
			m_sync_manu(p_oso);	
			not_man_sync_again=0;//用户选择了手动同步,通知正在自动同步的线程结束;
			is_sync_now=1;//告诉屏幕是否现在同步
			user_para_m[transplan_now].m_sync_Sec=is_auto_sync;//将这两个变量借位置存储在m序列的配置文件中以便下次恢复
			user_para_m[transplan_now].m_sync_mSec=(short)transplan_now;
			update_m_parafile_A(&user_para_m[transplan_now]);
			}
			else{
				exstr[0]=exstr[1]=exstr[2]=0xe1;
			}
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		exstr[0]=exstr[1]=exstr[2]=0xe1;
		is_sync_now=0;
		//ordersetting_index=ordersetting_index==0?8:--ordersetting_index;
		break;
		case KEY4_DOWN:
		if(is_syncing==YES){
			is_sync_now=NO;
			user_cancel=YES;
			m_sync_manu_cancel(p_oso);
			sync_status=0;
			nowindex=oso_abollomenu[nowindex].keydownstate;
			}
		else{
			is_sync_now=0;
			nowindex=oso_abollomenu[nowindex].keydownstate;
			}
		//ordersetting_index=ordersetting_index==8?0:++ordersetting_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
/*
int dspstrlen;
//dspstrlen=strlen(transsetting_str[setting_index]);
sprintf(freqsetting_str, "%5.2f", freqtable_arm[freqsetting_index]);
dspstrlen=strlen(freqsetting_str);
//sprintf(tmpstr, "%4.2f", freqtable_arm[freqsetting_index]);
memcpy(displaybuffer[nowindex]+40,"                    ",20);
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
memcpy(displaybuffer[nowindex]+40+2,freqsetting_str,dspstrlen);*/
if(is_syncing==NO){
	if ( pthread_create( &oledtmp, NULL, refreshscreen, &nowindex) ){
		printf(YELLOW"error creating tcp_server."NONE);
		abort();
		}
	else{
		;//printf(YELLOW"IS SYNCING!PLEASE WAIT!\n"NONE);
		}
	}
}










int autosyncsetting_action(int i_para)
{
		//printf(GREEN"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"NONE);
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;
	char freqsetting_str[20];
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	switch(tmpkeynum)
	{
		case KEY2_SELECT:
		//printf(GREEN"YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"NONE);
		nowindex=oso_abollomenu[nowindex].keystateindex;//+setting_index+1;
		is_auto_sync=is_auto_sync==0?1:0;
		//update_m_parafile(p_oso);
		user_para_m[transplan_now].m_sync_Sec=is_auto_sync;//将这两个变量借位置存储在m序列的配置文件中以便下次恢复		
		user_para_m[transplan_now].m_sync_mSec=(short)transplan_now;
		update_m_parafile_A(&user_para_m[transplan_now]);
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		autosyncswitch_index=autosyncswitch_index==0?2:--autosyncswitch_index;
		break;
		case KEY4_DOWN:
		nowindex=oso_abollomenu[nowindex].keydownstate;
		autosyncswitch_index=autosyncswitch_index==2?0:++autosyncswitch_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
/*
int dspstrlen;
//dspstrlen=strlen(transsetting_str[setting_index]);
sprintf(freqsetting_str, "%5.2f", freqtable_arm[freqsetting_index]);
dspstrlen=strlen(freqsetting_str);
//sprintf(tmpstr, "%4.2f", freqtable_arm[freqsetting_index]);
memcpy(displaybuffer[nowindex]+40,"                    ",20);
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
memcpy(displaybuffer[nowindex]+40+2,freqsetting_str,dspstrlen);*/
refreshscreen(&nowindex);
}









int ordersetting_action(int i_para)
{
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;
	char freqsetting_str[20];
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	switch(tmpkeynum)
	{
		case KEY2_SELECT:
		nowindex=oso_abollomenu[nowindex].keystateindex;//+setting_index+1;
		//printf("nowindex=%d\n",nowindex);
		p_oso->para_m.order=ordersetting_index+3;//设置频率

		//printf("p_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freq=%d",p_oso->para_m.freq);
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		ordersetting_index=ordersetting_index==0?(sizeof(ordertable) / sizeof(ordertable[0])-1):--ordersetting_index;
		break;
		case KEY4_DOWN:
		nowindex=oso_abollomenu[nowindex].keydownstate;
		ordersetting_index=ordersetting_index==(sizeof(ordertable) / sizeof(ordertable[0])-1)?0:++ordersetting_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
/*
int dspstrlen;
//dspstrlen=strlen(transsetting_str[setting_index]);
sprintf(freqsetting_str, "%5.2f", freqtable_arm[freqsetting_index]);
dspstrlen=strlen(freqsetting_str);
//sprintf(tmpstr, "%4.2f", freqtable_arm[freqsetting_index]);
memcpy(displaybuffer[nowindex]+40,"                    ",20);
printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
memcpy(displaybuffer[nowindex]+40+2,freqsetting_str,dspstrlen);*/
//printf(BLUE"(sizeof(ordertable)-1)%d\n"NONE,(sizeof(ordertable) / sizeof(ordertable[0])-1));

refreshscreen(&nowindex);
}








int freqsetting_action(int i_para)
{
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;
	char freqsetting_str[20];
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	switch(tmpkeynum)
	{
		case KEY2_SELECT:
		nowindex=oso_abollomenu[nowindex].keystateindex;//+setting_index+1;
		//printf("nowindex=%d\n",nowindex);
		p_oso->para_m.freq= freqtable_fpga[freqsetting_index];//设置频率

		//printf("p_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freq=%d",p_oso->para_m.freq);
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		freqsetting_index=freqsetting_index==0?15:--freqsetting_index;
		break;
		case KEY4_DOWN:
		nowindex=oso_abollomenu[nowindex].keydownstate;
		freqsetting_index=freqsetting_index==15?0:++freqsetting_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
	freqtable_index=freqsetting_index;
/*
int dspstrlen;
//dspstrlen=strlen(transsetting_str[setting_index]);
sprintf(freqsetting_str, "%5.2f", freqtable_arm[freqsetting_index]);
dspstrlen=strlen(freqsetting_str);
//sprintf(tmpstr, "%4.2f", freqtable_arm[freqsetting_index]);
memcpy(displaybuffer[nowindex]+40,"                    ",20);
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
memcpy(displaybuffer[nowindex]+40+2,freqsetting_str,dspstrlen);*/
refreshscreen(&nowindex);
}

int closeallrmfile()
{
closegpsfile(p_oso);
closelogfile(p_oso);
}









int gpssetting_action(int i_para)

{
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;


	int preindex=i_para;
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	int this_index=nowindex;
	switch(tmpkeynum)
	{
		case KEY2_SELECT:
		nowindex=oso_abollomenu[nowindex].keystateindex+sendsetting_index+SENDSETTING_BASE_SPACE+1;
		//printf("nowindex=%d\n",nowindex);
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		sendsetting_index=sendsetting_index==0?3:--sendsetting_index;
		break;
		case KEY4_DOWN:
		nowindex=oso_abollomenu[nowindex].keydownstate;
		sendsetting_index=sendsetting_index==3?0:++sendsetting_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
int dspstrlen;
dspstrlen=strlen(transsetting_str[sendsetting_index]);
cleandispbuf(displaybuffer[this_index]+LINE3START,20);
memcpy(displaybuffer[this_index]+LINE3START+(int)floor((20-dspstrlen)/2),transsetting_str[sendsetting_index],dspstrlen);

}


int updatedispstr(int buf_index,char * str2disp,int startpos)//
{
char tmpstr[80];
int dspstrlen=0;
sprintf(tmpstr, "%s",str2disp);
dspstrlen=strlen(tmpstr);
cleandispbuf(displaybuffer[buf_index]+startpos,20);
memcpy(displaybuffer[buf_index]+startpos,tmpstr,dspstrlen);
//memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
isneedupdate=1;
}






int refreshscreen(int * p_buf_index)
{
	int buf_index=*p_buf_index;
	//printf(RED"inside %s\n"NONE, __FUNCTION__);
		//printf(RED"buf_index=%d\n"NONE,buf_index);
	int dspstrlen;
	int nowindex;
	char tmpstr[80];
	int txlen;
	int ifor;
	int maxwaitsec;

	switch(buf_index)
	{
		case 0:
		break;
		case 1:
		break;
		case SENDSETTING:
			if(super_user){
				dspstrlen=strlen(transsetting_str[sendsetting_index]);
				cleandispbuf(displaybuffer[buf_index]+LINE2START,60);
				memcpy(displaybuffer[buf_index]+LINE3START+(int)floor((20-dspstrlen)/2),transsetting_str[sendsetting_index],dspstrlen);
				}
			else{
				dspstrlen=strlen(transplan_str[transplan_index]);
				cleandispbuf(displaybuffer[buf_index]+LINE3START,20);
				memcpy(displaybuffer[buf_index]+LINE3START,transplan_str[transplan_index],dspstrlen);
				cleandispbuf(displaybuffer[buf_index]+LINE2START,20);
				sprintf(tmpstr, "NOW:%2s", transplan_str[transplan_now]);
				dspstrlen=strlen(tmpstr);
				memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
				memcpy(displaybuffer[buf_index]+LINE4START,"press up/down change",strlen("press up/down change"));



				}
		break;
		case SYNCSETTING:

			dspstrlen=strlen(syncsetting_str[syncsetting_index]);
			cleandispbuf(displaybuffer[buf_index]+LINE3START,20);
			memcpy(displaybuffer[buf_index]+LINE3START+(int)floor((20-dspstrlen)/2),syncsetting_str[syncsetting_index],dspstrlen);
		break;
		case FLASHSETTING:

			dspstrlen=strlen(flashsetting_str[flashsetting_index]);
			cleandispbuf(displaybuffer[buf_index]+LINE3START,20);
			memcpy(displaybuffer[buf_index]+LINE3START+(int)floor((20-dspstrlen)/2),flashsetting_str[flashsetting_index],dspstrlen);
		break;
		case ABOUTSETTING:
			cleandispbuf((char *)displaybuffer[buf_index]+LINE2START,60);
			memcpy(displaybuffer[buf_index]+LINE2START,aboutsetting_str[0],strlen(aboutsetting_str[0]));
			//sprintf(tmpstr, "%5.2f", freqtable_arm[freqtable_i[p_oso->para_m.freq-1]-1]);
			//printf("=====================p_oso->para_m.freq==============%d\n",p_oso->para_m.freq);
			//memcpy(displaybuffer[0]+LINE2START+strlen("FREQ : "),tmpstr,strlen(tmpstr));
			//memcpy(displaybuffer[0]+LINE2START+strlen("FREQ : ")+strlen(tmpstr),"Hz",strlen("Hz"));
			memcpy(displaybuffer[buf_index]+LINE3START,aboutsetting_str[1],strlen(aboutsetting_str[1]));
			//sprintf(tmpstr, "%2d",p_oso->para_m.order);
			//memcpy(displaybuffer[0]+LINE3START+strlen("ORDER : "),tmpstr,strlen(tmpstr));
			//memcpy(displaybuffer[0]+LINE3START+strlen("ORDER : ")+strlen(tmpstr),"J",strlen("J"));

			memcpy(displaybuffer[buf_index]+LINE4START,aboutsetting_str[2],strlen(aboutsetting_str[2]));
		break;
		case SENDSETTING_ORDER:
			cleandispbuf(displaybuffer[buf_index]+LINE2START,60);
			sprintf(tmpstr, "NOW:%2d (O)", p_oso->para_m.order);
			dspstrlen=strlen(tmpstr);
			memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);

			sprintf(tmpstr, "%2d (O)", ordertable[ordersetting_index]);
			//printf(RED"ordersetting_index=%d,ordertable[ordersetting_index]=%d\n"NONE,ordersetting_index,ordertable[ordersetting_index]);
			dspstrlen=strlen(tmpstr);
			cleandispbuf(displaybuffer[buf_index]+LINE3START,20);
			//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
			memcpy(displaybuffer[buf_index]+LINE3START,tmpstr,dspstrlen);
			memcpy(displaybuffer[buf_index]+LINE4START,"press up/down change",strlen("press up/down change"));
		break;
		case SENDSETTING_FREQ:
			sprintf(tmpstr, "%5.2f", freqtable_arm[freqtable_i[p_oso->para_m.freq-1]-1]);

			strcat(tmpstr," (Hz)");
			dspstrlen=strlen(tmpstr);
			memcpy(displaybuffer[buf_index]+LINE2START,"                    ",20);
			memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
			memcpy(displaybuffer[buf_index]+LINE2START+4,tmpstr,dspstrlen);




			sprintf(tmpstr, "%4.3f", freqtable_arm[freqsetting_index]);

			strcat(tmpstr," (Hz)");
			dspstrlen=strlen(tmpstr);
			memcpy(displaybuffer[buf_index]+LINE3START,"                    ",20);
			//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
			memcpy(displaybuffer[buf_index]+LINE3START,tmpstr,dspstrlen);
			memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
		break;


		case SYNCSETTING_AUTO:
		//printf(RED"buf_index=%d\n"NONE,buf_index);
			if(is_auto_sync)
				sprintf(tmpstr, "%s","ON");
			else
				sprintf(tmpstr, "%s","OFF");
			//strcat(tmpstr," (Hz)");
			dspstrlen=strlen(tmpstr);
			memcpy(displaybuffer[buf_index]+LINE2START,"                    ",20);
			memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
			memcpy(displaybuffer[buf_index]+LINE2START+4,tmpstr,dspstrlen);
			memcpy(displaybuffer[buf_index]+LINE4START,"press select change",strlen("press select change"));
		break;
		case SYNCSETTING_MANU:
			if(is_sync_now){
				maxwaitsec=man_sync_wait_sec+4;//floor((pow(2,p_oso->para_m.order)-1)/freqtable_arm[freqtable_i[user_para_m[transplan_now].freq]]);
				int tmpblink=0;
				is_sync_now=0;
				is_syncing=1;//进入同步等待前置1;
					//printf(BLUE"man_sync_wait_sec=%d\n"NONE,man_sync_wait_sec);

				for(ifor_sync=0;ifor_sync<maxwaitsec;ifor_sync++)
					{
					sync_status=2;
					if(!user_cancel){
						waitsec=man_sync_wait_sec-ifor_sync;
						//printf(BLUE"man_sync_wait_sec=%d\n"NONE,man_sync_wait_sec);
						sprintf(tmpstr, "%s %d S","syncing......",ifor_sync);
						updatedispstr(buf_index,tmpstr,LINE2START);
						sprintf(tmpstr, "%s %d S","waiting ",maxwaitsec-ifor_sync);
						//printf(BLUE"%s\n"NONE,tmpstr);
						sprintf(dispbuff_in, "%d",maxwaitsec-ifor_sync);
						updatedispstr(buf_index,tmpstr,LINE3START);
						tmpblink=(tmpblink==0)?1:0;
						if(tmpblink){
							//printf(RED"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII"NONE);
							sprintf(tmpstr, "%s%s","press down cancel",exstr);
							exstr[0]=exstr[1]=exstr[2]=0x20;
							}
						else{
							sprintf(tmpstr, "%s","                    ");

							}

						//updatedispstr(buf_index,tmpstr,LINE4START);
						oled_writeline(displaybuffer[buf_index]+LINE4START,tmpstr,strlen(tmpstr));
						sleep(1);
						if(p_oso->signal_bit.is_sync==1){
							sprintf(tmpstr, "%s","NOW: sync     ");
							updatedispstr(buf_index,tmpstr,LINE1START);
							sprintf(tmpstr, "%s","sync DONE");
							updatedispstr(buf_index,tmpstr,LINE2START);
							//updatedispstr(buf_index,"DONE",LINE3START);
							//printf(RED"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSp_oso->signal_bit.is_sync=%d\n"NONE,p_oso->signal_bit.is_sync);
							break;
							}
						}
					}
				user_cancel=0;//用户取消已相应结束,置位!
				is_syncing=0;//结束同步等待置0
				p_oso->status_bit.m_sync_status_man=0;
				sync_status=0;
				}
			else{
				//printf(RED"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII----1"NONE);
				if(p_oso->signal_bit.is_sync==1){//如果重新同步则等待2秒
					sprintf(tmpstr, "%s","NOW: sync");
					dspstrlen=strlen(tmpstr);
					memcpy(displaybuffer[buf_index]+LINE1START,tmpstr,dspstrlen);
					}
				else{
					sprintf(tmpstr, "%s","NOW: NOT sync");
					dspstrlen=strlen(tmpstr);
					memcpy(displaybuffer[buf_index]+LINE1START,tmpstr,dspstrlen);	
					}
				}
			sprintf(tmpstr, "%s","want sync?");
			sync_status=0;
			//strcat(tmpstr," (Hz)");
			dspstrlen=strlen(tmpstr);
			cleandispbuf(displaybuffer[buf_index]+LINE2START,40);
			memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
			sprintf(tmpstr, "%s","press select to sync");
			dspstrlen=strlen(tmpstr);
			cleandispbuf(displaybuffer[buf_index]+LINE4START,20);
			memcpy(displaybuffer[buf_index]+LINE4START,tmpstr,dspstrlen);
			nowindex=0;
			myoled.nowscreennum=0;
			mainscreen_index=0;

			update_tx_info2ledbuff();
			isneedupdate=1;
		break;


		case FLASHSETTING_RM:
			if(!user_is_save_file){

				if(is_rm_all_file){
					sprintf(tmpstr, "%s","press down confirm!");
					dspstrlen=strlen(tmpstr);
					cleandispbuf(displaybuffer[buf_index]+LINE2START,40);
					//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
					//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
					memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
					memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
					isneedupdate=1;
					//TODO 
					rekeynum=vreadkey();
					//printf("FLASHSETTING_RM keynum=%d\n",keynum);
					//exit(0);
						//txlen=read(fd_keys,&keynum,4);
					switch(keynum)
						{
						case KEY1_MENU:
						sprintf(tmpstr, "%s","want rm? press select");
						//strcat(tmpstr," (Hz)");
						dspstrlen=strlen(tmpstr);
						cleandispbuf(displaybuffer[buf_index]+LINE1START,60);
						//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
						memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
						memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
						isneedupdate=1;
						break;
						case KEY2_SELECT:
						sprintf(tmpstr, "%s","want rm? press select");
						//strcat(tmpstr," (Hz)");
						dspstrlen=strlen(tmpstr);
						cleandispbuf(displaybuffer[buf_index]+LINE1START,60);
						//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
						memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
						memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
						isneedupdate=1;
						break;
						case KEY3_UP:
						sprintf(tmpstr, "%s","want rm? press select");
						dspstrlen=strlen(tmpstr);
						cleandispbuf(displaybuffer[buf_index]+LINE1START,60);
						//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
						memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
						memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
						isneedupdate=1;
						break;
						case KEY4_DOWN:
						sprintf(tmpstr, "%s","removing......");
						cleandispbuf(displaybuffer[buf_index]+LINE2START,40);
						dspstrlen=strlen(tmpstr);
						//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
						memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
						memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
						isneedupdate=1;
						//printf("projpath_now=%s",p_oso->projpath_now);
						if (is_dir_exist(p_oso->projpath_now)==0){
							
							closeallrmfile();//close all file first and rm *
							system("rmprojfiles");//执行删除命令
							///////////////////////////////////打开gps信息文件
							opengpsfile(p_oso);//reopen
							openlogfile(p_oso);
							///////////////////////////////////打开gps信息文件
							//printf(RED" rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrm2"NONE);
						}
							//printf(RED" rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrm3\n"NONE);
						sprintf(tmpstr, "%s","DONE");
						is_need_update_sdinfo=1;
						cleandispbuf(displaybuffer[buf_index]+LINE2START,40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
						memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
						memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
						isneedupdate=1;
						break;
						case KEY5_START_STOP:
						nowindex=oso_abollomenu[nowindex].keydownstate;
						//printf("nowindex=%d\n",nowindex);
						break;
						default:
						break;
						}
					}
				else//如果按了up或者down
					sprintf(tmpstr, "%s","want rm? press select");
				//strcat(tmpstr," (Hz)");
				dspstrlen=strlen(tmpstr);
				cleandispbuf(displaybuffer[buf_index]+LINE1START,60);
				//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
				//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
				memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
				memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
				}
			else{

					cleandispbuf(displaybuffer[buf_index]+LINE1START,60);
						//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
					sprintf(tmpstr, "%s","file is saving!");
					dspstrlen=strlen(tmpstr);
					memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
					sprintf(tmpstr, "%s","can NOT rm file");
					dspstrlen=strlen(tmpstr);
					memcpy(displaybuffer[buf_index]+LINE3START,tmpstr,dspstrlen);
					memcpy(displaybuffer[buf_index]+LINE4START,"press up stop and rm!",strlen("press up stop and rm!"));
					isneedupdate=1;
//进入读状态等待用户进一步选择=>
/*
					txlen=read(fd_keys,&keynum,4);
					switch(keynum)
						{
						case KEY1_MENU:
						sprintf(tmpstr, "%s","want rm? press select");
						//strcat(tmpstr," (Hz)");
						dspstrlen=strlen(tmpstr);
						cleandispbuf(displaybuffer[buf_index]+LINE1START,60);
						//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
						memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
						memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
						isneedupdate=1;
						break;
						case KEY2_SELECT:
						sprintf(tmpstr, "%s","want rm? press select");
						//strcat(tmpstr," (Hz)");
						dspstrlen=strlen(tmpstr);
						cleandispbuf(displaybuffer[buf_index]+LINE1START,60);
						//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
						memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
						memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
						isneedupdate=1;
						break;
						case KEY3_UP:
						sprintf(tmpstr, "%s","want rm? press select");
						dspstrlen=strlen(tmpstr);
						cleandispbuf(displaybuffer[buf_index]+LINE1START,60);
						//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
						memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
						memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
						isneedupdate=1;
						break;
						case KEY4_DOWN:
							printf(RED" rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrm1\n"NONE);
						sprintf(tmpstr, "%s","removing......");
						cleandispbuf(displaybuffer[buf_index]+LINE2START,40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
						memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
						memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
						isneedupdate=1;
						if (is_dir_exist("/mnt/sd/ABL_TX_PROJ")==0){
							system("rm /mnt/sd/ABL_TX_PROJ/*");//执行删除命令
							printf(RED" rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrm2"NONE);
						}
							printf(RED" rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrm3\n"NONE);
						sprintf(tmpstr, "%s","DONE");
						cleandispbuf(displaybuffer[buf_index]+LINE2START,40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"                    ",40);
						//memcpy(displaybuffer[buf_index]+LINE2START,"NOW:",4);
						memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
						memcpy(displaybuffer[buf_index]+LINE4START,"press up/down choose",strlen("press up/down choose"));
						isneedupdate=1;
						break;
						case KEY5_START_STOP:
						nowindex=oso_abollomenu[nowindex].keydownstate;
						//printf("nowindex=%d\n",nowindex);
						break;
						default:
						break;
						}




*/





				}
		break;
		case FLASHSETTING_FORMAT:


		oled_writeline(displaybuffer[buf_index]+LINE2START,"DONE!",20);
		break;
		case FLASHSETTING_DISCONNECT:
		oled_writeline(displaybuffer[buf_index]+LINE2START,"DONE!",20);
		break;
		case RXSETTING:
				dspstrlen=strlen(rxsetting_str[rxsetting_index]);
				cleandispbuf(displaybuffer[buf_index]+LINE3START,20);
				memcpy(displaybuffer[buf_index]+LINE3START,rxsetting_str[rxsetting_index],dspstrlen);
				memcpy(displaybuffer[buf_index]+LINE4START,"press up/down change",strlen("press up/down change"));
		break;
		case 31:
			
			cleandispbuf(displaybuffer[buf_index]+LINE2START,60);
			sprintf(tmpstr, "CH:%2d (O)", p_oso->para_m.order);
			dspstrlen=strlen(tmpstr);
			memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
			memcpy(displaybuffer[buf_index]+LINE4START,"press up/down change",strlen("press up/down change"));
		break;
	
		case 30:
			cleandispbuf(displaybuffer[buf_index]+LINE2START,60);
			sprintf(tmpstr, "CH%2d:%2d (O)", chswitch_index,para_info[chswitch_index].ch_isopen);
			dspstrlen=strlen(tmpstr);
			memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
			memcpy(displaybuffer[buf_index]+LINE4START,"press up/down change",strlen("press up/down change"));
		break;
		case FPGASETTING:


		printf(YELLOW"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n"NONE);
		if(is_transmitter){
			//memcpy(displaybuffer[29]+0,"Trans ctl",strlen("Trans ctl"));
			sprintf(tmpstr, "%s","Trans status");
				//strcat(tmpstr," (Hz)");
			dspstrlen=strlen(tmpstr);

			cleandispbuf(displaybuffer[buf_index]+LINE1START,20);
			memcpy(displaybuffer[buf_index]+LINE1START,tmpstr,dspstrlen);
			sprintf(tmpstr, "%s","Is trans......");
				//strcat(tmpstr," (Hz)");
			dspstrlen=strlen(tmpstr);

			cleandispbuf(displaybuffer[buf_index]+LINE2START,60);
			memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
/*
			sprintf(tmpstr, "%s","file:");
			dspstrlen=strlen(tmpstr);
			memcpy(displaybuffer[buf_index]+LINE3START,tmpstr,dspstrlen);
			sprintf(tmpstr, "%s",now_file_url);
			dspstrlen=strlen(tmpstr);
			memcpy(displaybuffer[buf_index]+LINE4START,tmpstr,dspstrlen);
*/
		}
		else{
			sprintf(tmpstr, "%s","trans stoped");
				//strcat(tmpstr," (Hz)");
			dspstrlen=strlen(tmpstr);
			cleandispbuf(displaybuffer[buf_index]+LINE2START,60);
			memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
			memcpy(displaybuffer[buf_index]+LINE2START,tmpstr,dspstrlen);
			sprintf(tmpstr, "%s","file:");
			dspstrlen=strlen(tmpstr);
			memcpy(displaybuffer[buf_index]+LINE3START,tmpstr,dspstrlen);
			sprintf(tmpstr, "%s",now_file_url);
			dspstrlen=strlen(tmpstr);
			memcpy(displaybuffer[buf_index]+LINE4START,tmpstr,dspstrlen);
		}
		break;
		default:
		break;
	}


}






int refreshscreen_long_time(int buf_index)
{

;

}












#define MAINMENUNUM 3

static int mainmenu_action(int i_para)
{
	//printf(RED"inside %s\n"NONE, __FUNCTION__);
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;

	int preindex=i_para;
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	int this_index=nowindex;
	switch(tmpkeynum)
	{
		case KEY2_SELECT:
		nowindex=oso_abollomenu[nowindex].keystateindex+setting_index+1;
		//printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~nowindex=%d\n",nowindex);
		//oso_abollomenu[nowindex].currentoperate(preindex);//进入时的动作
		refreshscreen(&nowindex);
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		setting_index=setting_index==0?MAINMENUNUM:--setting_index;
		break;
		case KEY4_DOWN:
		nowindex=oso_abollomenu[nowindex].keydownstate;
		setting_index=setting_index==MAINMENUNUM?0:++setting_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
int dspstrlen;
dspstrlen=strlen(setting_str[setting_index]);
memcpy(displaybuffer[this_index]+LINE3START,"                    ",20);
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
memcpy(displaybuffer[this_index]+LINE3START+(int)floor((20-dspstrlen)/2),setting_str[setting_index],dspstrlen);
}




int syncsetting_action(int i_para)

{
	//printf(RED"inside %s\n"NONE, __FUNCTION__);
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;


	int preindex=i_para;
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	int this_index=nowindex;
	switch(tmpkeynum)
	{
		case KEY2_SELECT:
		nowindex=oso_abollomenu[nowindex].keystateindex+syncsetting_index+SYNCSETTING_BASE_SPACE;
		//printf("nowindex=%d\n",nowindex);
		refreshscreen(&nowindex);
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		syncsetting_index=syncsetting_index==0?1:--syncsetting_index;
		break;
		case KEY4_DOWN:
		nowindex=oso_abollomenu[nowindex].keydownstate;
		syncsetting_index=syncsetting_index==1?0:++syncsetting_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
int dspstrlen;
dspstrlen=strlen(syncsetting_str[syncsetting_index]);
cleandispbuf(displaybuffer[this_index]+LINE3START,20);
memcpy(displaybuffer[this_index]+LINE3START+(int)floor((20-dspstrlen)/2),syncsetting_str[syncsetting_index],dspstrlen);
}





int change_transplan()
{
	transplan_now=transplan_index;
	freqtable_index=freqtable_i[user_para_m[transplan_now].freq];
	nowindex=oso_abollomenu[nowindex].keystateindex;
				//printf("nowindex=%d\n",nowindex);
	memcpy(&p_oso->para_m,&user_para_m[transplan_now],sizeof(ABL_TX_CMD));

	user_para_m[transplan_now].m_sync_Sec=is_auto_sync;//将这两个变量借位置存储在m序列的配置文件中以便下次恢复
				
	user_para_m[transplan_now].m_sync_mSec=(short)transplan_now;



}





int chgainsetting_action(int i_para)
{
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;
	char freqsetting_str[20];
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	switch(tmpkeynum)
	{
		case KEY2_SELECT:
		nowindex=oso_abollomenu[nowindex].keystateindex;//+setting_index+1;
		//printf("nowindex=%d\n",nowindex);
		p_oso->para_m.freq= freqtable_fpga[freqsetting_index];//设置频率

		//printf("p_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freq=%d",p_oso->para_m.freq);
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		freqsetting_index=freqsetting_index==0?15:--freqsetting_index;
		break;
		case KEY4_DOWN:
		nowindex=oso_abollomenu[nowindex].keydownstate;
		freqsetting_index=freqsetting_index==15?0:++freqsetting_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
	freqtable_index=freqsetting_index;
/*
int dspstrlen;
//dspstrlen=strlen(transsetting_str[setting_index]);
sprintf(freqsetting_str, "%5.2f", freqtable_arm[freqsetting_index]);
dspstrlen=strlen(freqsetting_str);
//sprintf(tmpstr, "%4.2f", freqtable_arm[freqsetting_index]);
memcpy(displaybuffer[nowindex]+40,"                    ",20);
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
memcpy(displaybuffer[nowindex]+40+2,freqsetting_str,dspstrlen);*/
refreshscreen(&nowindex);
}




int chsetting_action(int i_para)
{
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;
	char freqsetting_str[20];
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	switch(tmpkeynum)
	{
		case KEY2_SELECT:
		nowindex=oso_abollomenu[nowindex].keystateindex;//+setting_index+1;
		//printf("nowindex=%d\n",nowindex);
		//p_oso->para_m.freq= freqtable_fpga[freqsetting_index];//设置频率
		p_oso->para_next.para_ch_spi=setch(p_oso,p_oso->para_next.para_ch_spi,chswitch_index);
		p_oso->change_bit.isparachange=1;
		memcpy(&p_oso->para,&p_oso->para_next,p_oso->oso_size.para_s);
		p_oso->para.para_ch=getparas(p_oso)+2;
		printf(GREEN"real p_oso->para.para_ch=%d\n"NONE,p_oso->para.para_ch);
		sleep(2);
		init_oso_head(p_oso);
		printf(BLUE"real myoso->buffer_size=%d\n"NONE,p_oso->buffer_size);
		startfpga_1282(p_oso);
		;
		//printf("p_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freqp_oso->para_m.freq=%d",p_oso->para_m.freq);
		break;
		case KEY3_UP:
		nowindex=oso_abollomenu[nowindex].keyupstate;
		chswitch_index=chswitch_index==0?15:--chswitch_index;
		break;
		case KEY4_DOWN:
		nowindex=oso_abollomenu[nowindex].keydownstate;
		chswitch_index=chswitch_index==15?0:++chswitch_index;
		//printf("nowindex=%d\n",nowindex);
		break;
		default:
		break;
	}
/*
int dspstrlen;
//dspstrlen=strlen(transsetting_str[setting_index]);
sprintf(freqsetting_str, "%5.2f", freqtable_arm[freqsetting_index]);
dspstrlen=strlen(freqsetting_str);
//sprintf(tmpstr, "%4.2f", freqtable_arm[freqsetting_index]);
memcpy(displaybuffer[nowindex]+40,"                    ",20);
//printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn dspstrlen=%d,setting index=%d\n",dspstrlen,setting_index);
memcpy(displaybuffer[nowindex]+40+2,freqsetting_str,dspstrlen);*/
refreshscreen(&nowindex);
}













int rxsetting_action(int i_para)//发射参数设置

{

	printf(RED"inside %s oso_abollomenu[nowindex].keystateindex=%d\n"NONE, __FUNCTION__,oso_abollomenu[nowindex].keystateindex);
	printf(RED"RXSETTING_BASE_SPACE=%d\n"NONE,RXSETTING_BASE_SPACE);
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;
	int dspstrlen;

	int preindex=i_para;
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	int this_index=nowindex;
		switch(tmpkeynum)
			{
				case KEY2_SELECT:
				nowindex=rxsetting_index+RXSETTING_BASE_SPACE;
				printf("nowindex=%d,rxsetting_index=%d,RXSETTING_BASE_SPACE=%d\n",nowindex,rxsetting_index,RXSETTING_BASE_SPACE);
				refreshscreen(&nowindex);
				break;
				case KEY3_UP:
				nowindex=oso_abollomenu[nowindex].keyupstate;
				rxsetting_index=rxsetting_index==0?1:--rxsetting_index;
				break;
				case KEY4_DOWN:
				nowindex=oso_abollomenu[nowindex].keydownstate;
				rxsetting_index=rxsetting_index==1?0:++rxsetting_index;
				//printf("nowindex=%d\n",nowindex);
				break;
				default:
				break;
			}
		dspstrlen=strlen(rxsetting_str[rxsetting_index]);
		cleandispbuf(displaybuffer[this_index]+LINE2START,20);
		memcpy(displaybuffer[this_index]+LINE2START,rxsetting_str[rxsetting_index],dspstrlen);

}















int transsetting_action(int i_para)//发射参数设置

{

	//printf(RED"inside %s\n"NONE, __FUNCTION__);
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

	menuindex.preindex=i_para;
	menuindex.nowindex=nowindex;
	menuindex.thisindex=nowindex;
	menuindex.keynum=keynum;


	int preindex=i_para;
	//setting_index=setting_selected;
	int tmpkeynum=keynum;
	int this_index=nowindex;
	if(super_user){
		switch(tmpkeynum)
			{
				case KEY2_SELECT:
				nowindex=oso_abollomenu[nowindex].keystateindex+sendsetting_index+SENDSETTING_BASE_SPACE;
				//printf("nowindex=%d\n",nowindex);
				refreshscreen(&nowindex);
				break;
				case KEY3_UP:
				nowindex=oso_abollomenu[nowindex].keyupstate;
				sendsetting_index=sendsetting_index==0?1:--sendsetting_index;
				break;
				case KEY4_DOWN:
				nowindex=oso_abollomenu[nowindex].keydownstate;
				sendsetting_index=sendsetting_index==1?0:++sendsetting_index;
				//printf("nowindex=%d\n",nowindex);
				break;
				default:
				break;
			}
		int dspstrlen;
		cleandispbuf(displaybuffer[this_index]+LINE2START,60);
		dspstrlen=strlen(transsetting_str[sendsetting_index]);
		cleandispbuf(displaybuffer[this_index]+LINE3START,20);
		memcpy(displaybuffer[this_index]+LINE3START+(int)floor((20-dspstrlen)/2),transsetting_str[sendsetting_index],dspstrlen);
		}
	else{
		switch(tmpkeynum)
			{
				case KEY2_SELECT:
				/*
				transplan_now=transplan_index;
freqtable_index=freqtable_i[user_para_m[transplan_now].freq];
				nowindex=oso_abollomenu[nowindex].keystateindex;
				//printf("nowindex=%d\n",nowindex);
				memcpy(&p_oso->para_m,&user_para_m[transplan_now],sizeof(ABL_TX_CMD));

				user_para_m[transplan_now].m_sync_Sec=is_auto_sync;//将这两个变量借位置存储在m序列的配置文件中以便下次恢复
				
				user_para_m[transplan_now].m_sync_mSec=(short)transplan_now;
				update_m_parafile_A(&user_para_m[transplan_now]);
				//update_m_parafile(p_oso);
				//m_sync_manu(p_oso);*/
				change_transplan();
				update_m_parafile_A(&user_para_m[transplan_now]);
				refreshscreen(&nowindex);

				break;
				case KEY3_UP:
				nowindex=oso_abollomenu[nowindex].keyupstate;
				transplan_index=transplan_index==0?2:--transplan_index;
				break;
				case KEY4_DOWN:
				nowindex=oso_abollomenu[nowindex].keydownstate;
				transplan_index=transplan_index==2?0:++transplan_index;
				//printf("nowindex=%d\n",nowindex);
				break;
				default:
				break;
			}
		int dspstrlen;
		dspstrlen=strlen(transplan_str[transplan_index]);
		cleandispbuf(displaybuffer[this_index]+LINE3START,20);
		memcpy(displaybuffer[this_index]+LINE3START,transplan_str[transplan_index],dspstrlen);
		}
}


int getsn(char * abl_sn)
{
	FILE * fp_abl_conf;
	char tmp[20];
	if(fp_abl_conf=fopen("/etc/abl.conf","r")){
		fgets(tmp,20,fp_abl_conf);
		sprintf(abl_sn,"%s\n",tmp+3);
		return 0;
		}
	else{
		return -1;
		}

}


int keyprocess(int key1)
{
		//printf(RED"in keyprocess keynum=%d\n"NONE,keynum);
		int tmp;
		char tmpstr[20];
		if(!is_transmitter|keynum==KEY5_START_STOP){
			preindex=nowindex;
			gobankhometime=0;
			switch(keynum)
			{
		
				case KEY1_MENU:
				nowindex=oso_abollomenu[nowindex].keymenustate;
				//printf("nowindex=%d\n",nowindex);
				break;
				case KEY2_SELECT:
				nowindex=oso_abollomenu[nowindex].keyselectstate;
				//printf("nowindex=%d\n",nowindex);
				break;
				case KEY3_UP:
				nowindex=oso_abollomenu[nowindex].keyupstate;
				//printf("nowindex=%d\n",nowindex);
				break;
				case KEY4_DOWN:
				nowindex=oso_abollomenu[nowindex].keydownstate;
				//printf("nowindex=%d\n",nowindex);
				break;
				case KEY5_START_STOP:
				myoled.nowscreennum=29;
				tmp=nowindex;
				nowindex=29;
				//printf(RED"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
				//update_tx_super_user_info2ledbuff();
				
				is_transmitter=user_is_save_file=(is_transmitter==0)?1:0;
				ioctl(fdio,GPIO_IOC_N_SD_LED1_OFF,is_transmitter);
				ioctl(fdio,GPIO_IOC_N_FPGA_M_SWITCH,is_transmitter);
				if(is_transmitter){//start transmitter
					;
					}
				else{
				//printf(RED"is_file_open=%d\n"NONE,is_file_open);
					if(is_file_open){//if file not full
					p_oso->para.filename_num=1;
				//printf(RED"is_file_open=%d\n"NONE,is_file_open);
						}
				tx_current.current=0;

				isneedupdate=1;
					}
				oso_abollomenu[29].currentoperate(29);
				isneedupdate=1;
				sleep(1);
				//user_is_save_file=user_is_save_file==0?1:0;
				nowindex=0;
				mainscreen_index=0;
				//printf(RED"nowindex=%d\n"NONE,is_transmitter);
				break;
				default:
				break;
			}

			//printf(RED"before rechange nowindex the nowindex=%d"NONE,nowindex);
			oso_abollomenu[nowindex].currentoperate(preindex);//进入时的动作
			oso_abollomenu[preindex].exitoperate(preindex);//退出时的动作
			myoled.nowscreennum=nowindex;
			//printf(RED"after rechange nowindex the nowindex=%d\n"NONE,nowindex);
			//memcpy(displaybuffer[myoled.nowscreennum]+73,"NI:",strlen("NI:"));
			//displaybuffer[myoled.nowscreennum][77]=(char)(nowindex+48);
			//sprintf(tmpstr, "%02d", nowindex);
			//memcpy(&displaybuffer[myoled.nowscreennum][77],tmpstr,2);
			//displaybuffer[myoled.nowscreennum][79]=0xdf;
			isneedupdate=YES;
			//printf(RED"KEY PROCESS DONE\n"NONE);
			//DEBUGPRINT(printf(BLUE"abollo-s size=%d\n"NONE,file_size("1abollo-s"));)

			//printf(BLUE"abollo-s size=%d\n"NONE,file_size2("abollo-s"));
		}
		else{
			myoled.nowscreennum=DISP_SCREEN_OPERATE;
			getethstatus();//获取网卡信息
			memcpy(displaybuffer[myoled.nowscreennum]+LINE2START,"operate LOCKED!",strlen("operate LOCKED!"));
			memcpy(displaybuffer[myoled.nowscreennum]+LINE3START,"Please STOP firstly",strlen("Please STOP firstly"));


			switch(keynum)
			{
		
				case KEY1_MENU:
				sprintf(tmpstr, "%s%s(%s)", localipstr,localip,eth_status_str);
				break;
				case KEY2_SELECT:
				sprintf(tmpstr, "host:%s", tcpdispstr);
				break;
				case KEY3_UP:
				sprintf(tmpstr, "data:%s", datadispstr);
				break;
				case KEY4_DOWN:
				sprintf(tmpstr, "FREE SD :%2.4fGB",(float)(p_oso->info.free_sd)/1024/1024/1024);
				break;
				default:
				break;
			}
			cleandispbuf(displaybuffer[myoled.nowscreennum]+LINE4START,20);
			memcpy(displaybuffer[myoled.nowscreennum]+LINE4START,tmpstr,strlen(tmpstr));
			isneedupdate=YES;
			sleep(1);
			myoled.nowscreennum=0;
			mainscreen_index=0;
			update_tx_info2ledbuff();
			//mainscreen_index=0;
			isneedupdate=YES;
		}


}

int getvkeynum()//从终端键盘读入
{
int vkeynum;
char getchs[10];
char tmpstr[2]={0,0};
int i=0;
while(1)
{
//printf("int get vkeynum\n");
scanf("%s",getchs);
//printf("getchs=%s\n",getchs);
for(i=0;i<strlen(getchs);i++)
{
//printf("in process getchs[i]=%c keynum=%d\n\n\n\n\n\n",getchs[i],keynum);
if(getchs[i]>48&&getchs[i]<54)
{
tmpstr[0]=getchs[i];
sscanf(tmpstr,"%d",&keynum);
//printf("in process getchs[i]=%c keynum=%d\n\n\n\n\n\n",getchs[i],keynum);


//keynum=vkeynum;
//keyprocess(vkeynum);
	pthread_mutex_lock(&mut_vkey);
	pthread_cond_broadcast(&cond_vkey, &mut_vkey);
	pthread_mutex_unlock(&mut_vkey);
	//sleep(3);	
usleep(100000);
}
}
}

}

int getkeynum()//从真实按键读入
{
	while(1)
	{


	//printf("=====================txlen=read(fd_keys,&keynum,4);==============%d\n",p_oso->para_m.freq);
	txlen=read(fd_keys,&keynum,4);
	pthread_mutex_lock(&mut_vkey);

	//printf("in getkeynum keynum=%d\n",keynum);
	pthread_cond_broadcast(&cond_vkey, &mut_vkey);

	pthread_mutex_unlock(&mut_vkey);
	}
}









int vreadkey()
{
	pthread_mutex_lock(&mut_vkey);
		//printf("=====================before==============%d\n",p_oso->para_m.freq);
	//printf("before vreadkey keynum=%d\n",keynum);
	pthread_cond_wait(&cond_vkey, &mut_vkey);
	//printf("after vreadkey keynum=%d\n",keynum);
		//printf("=====================after==============%d\n",p_oso->para_m.freq);
	pthread_mutex_unlock(&mut_vkey);
	return keynum;
}





int keyboard(oso * myoso)
{
	oso_abollomenu=&oso_abollomenu_main;
	myoso->status_bit.keyboard_status=1;
	char tmpstr[20];
	int rekeynum;//返回的keynum；
	pthread_mutex_init(&mut_vkey,NULL);
	pthread_cond_init(&cond_vkey,NULL);
	aboutsetting_str[2]=abl_hwversion;
	ioctl(fdio,GPIO_IOC_N_FPGA_M_SWITCH,is_transmitter);
	p_oso=myoso;
	getsn(abl_sn);
	ordersetting_index=p_oso->para_m.order-3;
	freqsetting_index=freqtable_i[p_oso->para_m.freq-1]-1;
	freqtable_index=freqsetting_index;
	transplan_now=transplan_index;
			//printf("=====================p_oso->para_m.freq==============%d\n",p_oso->para_m.freq);
	fd_keys = open("/dev/keys0", O_RDWR);//打开键盘
	if(fd_keys<0)
		printf("open /dev/gpioctl0 err!\n");
	/*
	if ( pthread_create( &vkey, NULL, getvkeynum, &nowindex) ){
		printf(YELLOW"error creating tcp_server."NONE);
		abort();
		}
	else{
		;//printf(YELLOW"IS SYNCING!PLEASE WAIT!\n"NONE);
		}*/

	if ( pthread_create( &rkey, NULL, getkeynum, &nowindex) ){
		printf(YELLOW"error creating tcp_server."NONE);
		abort();
		}
	else{
		;//printf(YELLOW"IS SYNCING!PLEASE WAIT!\n"NONE);
		}

/*
	if ( pthread_create( &newmenu, NULL, menu, myoso) ){
		printf(YELLOW"error creating tcp_server."NONE);
		abort();
		}
	else{
		;//printf(YELLOW"IS SYNCING!PLEASE WAIT!\n"NONE);
		}
*/
	while(ctltotal&&!(myoso->control.keyboard_loop))
	{	
		//exit(0);
		//printf("=====================p_oso->para_m.freq==============%d\n",p_oso->para_m.freq);

		rekeynum=vreadkey();
		keyprocess(keynum);//TODO 新菜单没完成前需启用
	}
	printf(GREEN"YOU GUESS WHAT ? IT WORKS ,WE JUST FINISH MY JOB!-----KEYBOARD\n"NONE);
	myoso->status_bit.keyboard_status=0;
}
