#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "../../inc/menu.h"
#define	KEY1_MENU 1
#define	KEY2_SELECT 2
#define	KEY3_UP 3
#define	KEY4_DOWN 4
#define	KEY5_START_STOP 5
extern pthread_mutex_t mut_vkey;
extern pthread_cond_t cond_vkey;
extern pthread_t vkey;
extern pthread_t rkey;
extern int vreadkey();
extern int getvkeynum();
extern int update_net_info2ledbuff();
extern int update_tx_info2ledbuff();
extern int update_sd_info();
extern int update_batt_info();
extern int update_tx_info();
extern int update_gps_info2ledbuff();
extern int update_time_info2ledbuff();

extern int keynum;
static int mainmenu_action(void * i_para);
int main_screen(int * para);
int menu_dodefault(int * i_para);




typedef struct menu_item{
struct menu_item * nowstate;//当前状态索引号
struct menu_item * keymenustate;//按下menu键时转向的状态索引号
struct menu_item * keyselectstate;//按下select键时转向的状态索引号
struct menu_item * keyupstate;//按下up键时转向的状态索引号
struct menu_item * keydownstate;//按下down键时转向的状态索引号
int dispbufnum;//display buffer num;
char dispbuf[80];
char menuname[10];
int menuindex;//当前menu的第i页
int itemindex;
int itemnum;//当前总页数
void  (* currentoperate)(void *);//进入当前状态应该执行的功能*作
void  (* exitoperate)(void *);//跳出当前状态应该执行的功能*作
char unuse[64];
}menu_item;

	menu_item * nowmenu;
	menu_item  mainmenu={
	.nowstate=&mainmenu,
	.keymenustate=&mainmenu,
	.keyselectstate=&mainmenu,
	.keyupstate=&mainmenu,
	.keydownstate=&mainmenu,
	.dispbufnum=0,
	.dispbuf="hello menu",
	.menuname="mainmenu",
	.menuindex=0,
	.itemindex=0,
	.itemnum=0,
	.currentoperate=(* mainmenu_action),
	.exitoperate=(* menu_dodefault),
};


int init_menu()
{
nowmenu=malloc(sizeof(menu_item));
memset(nowmenu,0,sizeof(menu_item));
nowmenu->nowstate=nowmenu;
nowmenu->keymenustate=nowmenu;
nowmenu->keyselectstate=nowmenu;
nowmenu->keyupstate=nowmenu;
nowmenu->keydownstate=nowmenu;
nowmenu->currentoperate=( * main_screen);
nowmenu->exitoperate=(* menu_dodefault);
return 0;
}

int add_menu(menu_item * menustate)
{
menustate=malloc(sizeof(menu_item));
menustate->nowstate=menustate;
}






int menu_dodefault(int * i_para)
{
printf("do nothing \n");

}



static int mainmenu_action(void *i_para)
{
	//printf(RED"inside %s\n"NONE, __FUNCTION__);
	char tmpstr[20];
	int ifor;
//	myoso->para_m.freq=4; // 频率 
//	myoso->para_m.order=11; // 阶数 
//	myoso->para_m.m_code_len=2047; // m序列长度（供截断）
//	myoso->para_m.m_init_phz=-1; // m序列初相 
//	myoso->para_m.m_polynomial=0x402;//10000000010; // m序列多项式

/*
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
*/
}










int main_screen(int * para)
{
	printf("inside %s\n", __FUNCTION__);
//printf("mainjob1 para=%d\n",para);
	char tmpstr[80];

	int ifor,txlen;
	switch(keynum)
	{
		case KEY2_SELECT:
		printf("=====================keynum=%d==============\n",keynum);
		break;
		case KEY3_UP:
			nowmenu=nowmenu->keyupstate;
			nowmenu->menuindex=(nowmenu->menuindex==0)?6:--nowmenu->menuindex;
update_mainmenuinfo(nowmenu->menuindex);
		break;
		case KEY4_DOWN:
			nowmenu=nowmenu->keydownstate;
		printf("=====================nowmenu->menuindex=%d==============\n",nowmenu->menuindex);
			nowmenu->menuindex=(nowmenu->menuindex==6)?0:++nowmenu->menuindex;
update_mainmenuinfo(nowmenu->menuindex);
		break;
		default:
		break;
	}
}

#define	KEY1_MENU 1
#define	KEY2_SELECT 2
#define	KEY3_UP 3
#define	KEY4_DOWN 4
#define	KEY5_START_STOP 5

int update_mainmenuinfo(int infonum)//根据不同页更新信息
{
		printf("=====================infonum=%d==============\n",infonum);
	switch(infonum)
	{
		case 0:
		printf("=====================keynum=%d==============\n",keynum);
update_tx_info2ledbuff();
		break;
		case 1:
		printf("=====================keynum=%d==============\n",keynum);
update_net_info2ledbuff();
		break;
		case 2:
		printf("=====================keynum=%d==============\n",keynum);
update_sd_info();
		break;
		case 3:
update_tx_info();

		printf("=====================keynum=%d==============\n",keynum);
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
update_tx_info2ledbuff();
		break;
	}




}




int menu_keyprocess()
{
		printf("keyprocess keynum=%d\n",keynum);
		int tmp;
		char tmpstr[20];
		nowmenu->exitoperate(&keynum);//跳出当前状态应该执行的功能*作
			switch(keynum)
			{
				case KEY1_MENU:
				nowmenu=nowmenu->keymenustate;
				//printf("nowindex=%d\n",nowindex);
				break;
				case KEY2_SELECT:
				nowmenu=nowmenu->keyselectstate;
				//printf("nowindex=%d\n",nowindex);
				break;
				case KEY3_UP:
				nowmenu=nowmenu->keyupstate;
				//printf("nowindex=%d\n",nowindex);
				break;
				case KEY4_DOWN:
				nowmenu=nowmenu->keydownstate;
				//printf("nowindex=%d\n",nowindex);
				break;
				case KEY5_START_STOP:
				//printf(RED"nowindex=%d\n"NONE,is_transmitter);
				break;
				default:
				break;
			}
		nowmenu->currentoperate(&keynum);//进入时的动作

isneedupdate=YES;
}








int menu(oso * myoso)
{
	if ( pthread_create( &vkey, NULL, getvkeynum, nowmenu) ){
		printf("error creating tcp_server.");
		abort();
		}
	else{
		;//printf(YELLOW"IS SYNCING!PLEASE WAIT!\n"NONE);
		}
	int rekeynum;
	init_menu();
	while(1)
	{	
		//exit(0);
		//printf("=====================p_oso->para_m.freq==============%d\n",p_oso->para_m.freq);

		rekeynum=vreadkey();
		menu_keyprocess(keynum);
	}
}

