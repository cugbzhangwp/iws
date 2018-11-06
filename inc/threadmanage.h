//通过调用threadmanage函数创建新线程需包含此文件
typedef struct threadblock{
	char * threadname;
	int  (* threadfun)(void *);
	void * paras;
	char a[128];
}__attribute__ ((packed,aligned(1)))THREADBLOCK_S, * THREADBLOCK_T;
extern int threadmanage(int  (* threadfun)(void *),void * paras);
extern int osonewthread(pthread_t * newthread,void  (* threadfun)(void *),void * paras);
extern int sectimer_taskmanage(unsigned int taskmask);