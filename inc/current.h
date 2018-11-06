typedef struct CURRENT
{
int ifor;
unsigned int * pcurrent;
int ad_current;
int ad_max;
int ad_min;
float current;
float currentp;
float currentn;
long long sumcurrentn;
long long sumcurrentp;
long long sumcurrent;
int nnum;
int pnum;
int iscurrentvalidn;
int iscurrentvalidp;
}CURRENT;



extern int cacu_current();
