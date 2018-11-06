#include <time_geo.h>

int get_sys_time()
{
    struct timeval start, end;
    gettimeofday( &start, NULL );
    printf("start : %d.%d\n", start.tv_sec, start.tv_usec);
    sleep(1);
    gettimeofday( &end, NULL );
    printf("end   : %d.%d\n", end.tv_sec, end.tv_usec);

    return 0;
}




int clock_gettime_geo()
{        
    struct timespec time1 = {0, 0};   
      
    clock_gettime(CLOCK_REALTIME, &time1);        
    printf("CLOCK_REALTIME: %d, %d\n", time1.tv_sec, time1.tv_nsec);  
       
    clock_gettime(CLOCK_MONOTONIC, &time1);        
    printf("CLOCK_MONOTONIC: %d, %d\n", time1.tv_sec, time1.tv_nsec);    
     
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);        
    printf("CLOCK_PROCESS_CPUTIME_ID: %d, %d\n", time1.tv_sec, time1.tv_nsec);   
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time1);        
    printf("CLOCK_THREAD_CPUTIME_ID: %d, %d\n", time1.tv_sec, time1.tv_nsec);   
     
    printf("\n%d\n", time(NULL)); 
     
    sleep(5);
 
    return 0;
}