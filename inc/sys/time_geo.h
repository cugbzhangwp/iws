#ifndef	__TIME_GEO_H__
#define	__TIME_GEO_H__
#include<stdio.h>
#include<time.h>
#include <sys/time.h>    // for gettimeofday()
#include <unistd.h>        // for sleep()
/*
获取系统时间函数
*/
extern int get_sys_time();
extern int clock_gettime_geo();
#endif