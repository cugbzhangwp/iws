#define __DEBUG__  
#ifdef __DEBUG__  
#define DEBUG(format,...) printf("file:"__FILE__, ##__VA_ARGS__)  
//#define DEBUG(format,...) printf("file:"__FILE__"--time: "__TIME__"--date: "__DATE__"--Line: %05d: "format" ", __LINE__, ##__VA_ARGS__)  
#define DEBUGb(format,...) printf("--date: "__DATE__"--Line: %05d: "format" \r\n", __LINE__, ##__VA_ARGS__)  
#else  
#define DEBUG(format,...)  
#define DEBUGb(format,...)  
#endif 
