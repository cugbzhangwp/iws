int osonewthread(pthread_t * newthread,int  (* threadfun)(void *),void * paras)
{
	if ( pthread_create(newthread, NULL, threadfun, paras) ){
		printf("error creating gpsthread.");
		return -1;
	}
	printf(PURPLE"UDP DATA THREAD DONE\n"NONE);	
	return 0;
}


int setpthattr(pthread_attr_t *attr)//配置线程优先级等参数
{
	int ret = 0;   
	  
	/*初始化属性线程属性*/
	pthread_attr_init (attr);   
	pthread_attr_setscope (attr, PTHREAD_SCOPE_SYSTEM);   
	//pthread_attr_setdetachstate (attr, PTHREAD_CREATE_DETACHED); 
	struct sched_param param;
	pthread_attr_getschedparam(attr, &param);
	pthread_attr_setschedparam (attr, &param);
}