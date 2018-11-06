







int getlocalips(char * ipaddr) {//获取本地网卡信息
    struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct!=NULL) {
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            //printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer); 

	if(strstr(ifAddrStruct->ifa_name,"eth0")!=NULL)
			memcpy(ipaddr,addressBuffer,strlen(addressBuffer));
        } else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer); 
        } 
        ifAddrStruct=ifAddrStruct->ifa_next;
    }
    return 0;
}


unsigned long long get_space_info(oso_n_statfs * sd_info)//获取磁盘信息
{
	//oso_n_statfs mystatfs;
	statfs(sd_info->tgt,&(sd_info->diskInfo));
	unsigned long long blocksize = sd_info->diskInfo.f_bsize;// 每个block里面包含的字节数
	unsigned long long totalsize = blocksize * sd_info->diskInfo.f_blocks;//总的字节数
	//unsigned long long freeDisk = sd_info->diskInfo.f_bfree * blocksize; //剩余空间的大小  
	unsigned long long free_sd = sd_info->diskInfo.f_bfree * blocksize; //剩余空间的大小  
	unsigned long long free_mem = blocksize * sd_info->diskInfo.f_blocks; //剩余空间的大小  
	unsigned long long availableDisk =sd_info->diskInfo.f_bavail * blocksize;   //可用空间大小
	return free_sd;
}


static inline int get_sys_info (struct sysinfo * s_info)//获取系统启动时间等信息
{
	int error = sysinfo(s_info);
	if(error<0)
	printf("error");
}








