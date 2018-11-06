


int check_update_from_udisk()
{	//update the para.osp file when myoso.para have some change;
	FILE *fp_para;
	//printf("/mnt/.m_para.abo updated%s\n",myoso->parafile);
	if(fp_para=fopen("/mnt/udisk/abollo_update.bin","rb")){//证明有更新文件
		//tar -cZ -f old.tar * 压缩时所用命令
		//解压所需命令:tar -xZ -f old.tar
		fclose(fp_para);
		system("abl_updata");
		system("reboot");
		}
}



int checksys()
{
	check_supercontrol();//check supercontrol;
	check_update_from_udisk();//check update from udisk
	check_sd_mount();
}
