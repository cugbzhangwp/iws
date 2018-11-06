#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <readconf.h>
//#include <printcolor.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <sys/vfs.h>
#include <getopt.h>
#include <cJSON.h>

/*

void print_usage(const char *prog,int type)
{
	printf(	"Usage: %s [-M para][-d]\n", prog);
	puts(	"	Orangelamp & CrystalGlobal	\n"
			" 	abollo-s is copyrighted by frank between 2015-2020.\n"
			" 	zwp@orangelamp.com.cn\n"
			" 	tel:18622283833\n"
     );


	puts("  -M --mode   run mod(oso/abollo/other/debug)\n"
	     "  -V --version    printf program version\n"
	     "  -d --debug    debug run(print more info)\n"
	     "  -H --help     print this info \n"
	     );
	if(type==0)
		;
	else
		exit(1);
}



extern CMD_LINE_OPT opts;
void parse_opts(oso* myoso,int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "mode",  1, 0, 'M' },
			{ "debug",    1, 0, 'd' },
			{ "version",   0, 0, 'V' },
			{ "help",   0, 0, 'H' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "M:d:VH", lopts, NULL);
               	printf("=============c===============%d\r\n",c);
		if (c == -1){
			print_usage(argv[0],0);
			break;
			}

		switch (c) {
		case 'M':
			if(!strcmp("oso",optarg))
				myoso->opts.mode = 1;
			else if(!strcmp("abollo",optarg))
				myoso->opts.mode = 0;
			else{
				printf("give wrong -M opt\n");
				printf("use default mode abollo\n");
				myoso->opts.mode = 0;
				//exit(0);
			}
			break;
		case 'V':
			GetCompileTime(compile_t);
			printf("App version : %s\n",compile_t);
			//print_usage(argv[0],1);
			exit(0);
			break;
		case 'd':
			{
			if(!strcmp("main",optarg))
				myoso->opts.is_main_debug = 1;
			else if(!strcmp("gps",optarg))
				myoso->opts.is_gps_debug = 1;
			else{
				printf("give wrong -d opt\n");
				//exit(0);
			}
			myoso->opts.isdebug=1;
			//delay = atoi(optarg);
			break;
			}	
		case 'H':
			print_usage(argv[0],1);
			exit(0);
			//delay = atoi(optarg);
			break;	
		default:
			print_usage(argv[0],1);
			break;
		}

	}
}




*/








/*
 *去除字符串右端空格
 */
char *strtrimr(char *pstr)
{
    int i;
    i = strlen(pstr) - 1;
    while (isspace(pstr[i]) && (i >= 0))
        pstr[i--] = '\0';
    return pstr;
}
/*
 *去除字符串左端空格
 */
char *strtriml(char *pstr)
{
    int i = 0,j;
    j = strlen(pstr) - 1;
    while (isspace(pstr[i]) && (i <= j))
        i++;
    if (0<i)
        strcpy(pstr, &pstr[i]);
    return pstr;
}
/*
 *去除字符串两端空格
 */
char *strtrim(char *pstr)
{
    char *p;
    p = strtrimr(pstr);
    return strtriml(p);
}


/*
 *从配置文件的一行读出key或value,返回item指针
 *line--从配置文件读出的一行
 */
int  get_item_from_line(char *line, ITEM *item)
{
    char *p = strtrim(line);
    int len = strlen(p);
    if(len <= 0){
        return 1;//空行
    }
    else if(p[0]=='#'){
        return 2;
    }else{
        char *p2 = strchr(p, '=');
        *p2++ = '\0';
        item->key = (char *)malloc(strlen(p) + 1);
        item->value = (char *)malloc(strlen(p2) + 1);
        strcpy(item->key,p);
        strcpy(item->value,p2);

        }
    return 0;//查询成功
}






int file_to_items(const char *file, ITEM *items,int *num)
{
    char line[1024];
    FILE *fp;
    fp = fopen(file,"r");
    if(fp == NULL)
        return 1;
    int i = 0;
    while(fgets(line, 1023, fp)){
            char *p = strtrim(line);
        int len = strlen(p);
        if(len <= 0){
            continue;
        }
        else if(p[0]=='#'){
            continue;
        }else{
            char *p2 = strchr(p, '=');
            /*这里认为只有key没什么意义*/
            if(p2 == NULL)
                continue;
            *p2++ = '\0';
            items[i].key = (char *)malloc(strlen(p) + 1);
            items[i].value = (char *)malloc(strlen(p2) + 1);
            strcpy(items[i].key,p);
            strcpy(items[i].value,p2);

            i++;
        }
    }
    (*num) = i;
    fclose(fp);
    return 0;
}

/*
 *读取value
 */
int read_conf_value(const char *key,char *value,const char *file)
{
    char line[1024];
    FILE *fp;
    fp = fopen(file,"r");
    if(fp == NULL)
        return 1;//文件打开错误
    while (fgets(line, 1023, fp)){
        ITEM item;
        get_item_from_line(line,&item);
        if(!strcmp(item.key,key)){
            strcpy(value,item.value);

            fclose(fp);
            free(item.key);
            free(item.value);
            break;
        }

    }
    return 0;//成功

}



int write_conf_value(const char *key,char *value,const char *file)
{
    ITEM items[200];// 假定配置项最多有20个
    int num;//存储从文件读取的有效数目
    file_to_items(file, items, &num);

    int i=0;
    //查找要修改的项
    for(i=0;i<num;i++){
        if(!strcmp(items[i].key, key)){
            items[i].value = value;
            break;
        }
    }

    // 更新配置文件,应该有备份，下面的操作会将文件内容清除
    FILE *fp;
    fp = fopen(file, "w");
    if(fp == NULL)
        return 1;

    i=0;
    for(i=0;i<num;i++){
        fprintf(fp,"%s=%s\n",items[i].key, items[i].value);
        //printf("%s=%s\n",items[i].key, items[i].value);
    }
    fclose(fp);
    //清除工作
/*    i=0;
    for(i=0;i<num;i++){
        free(items[i].key);
        free(items[i].value);
    }*/

    return 0;

}


int check_pak_conf(int size,char * buf)
{
    ;
}
/*
{
    "filename":  "/etc/abl.conf",
    "configs":    ["a":"1","b":"2"]
}






{
    "name": "Jack (\"Bee\") Nimble",
    "format": {
        "type":       "rect",
        "width":      1920,
        "height":     1080,
        "interlace":  false,
        "frame rate": 24
    }
}

*/  
int tcp_conf_callback( int size,char * buf,void * buff )
{   

    //    char * json_str="{\
    \"filename\":\"/etc/abl.conf\",\
    \"type\":\"read\",\
    \"configs\":[{\"key\":\"a\",\"value\":\"2\"},{\"key\":\"b\",\"value\":\"3\"}]\
}";
    int ifor;
    if(!check_pak_conf(size,buf)){
        cJSON * root = NULL;
        root=cJSON_Parse(buf);
        if(root==NULL)
            return -1;
        cJSON *filename = cJSON_GetObjectItemCaseSensitive(root, "filename");
        cJSON *type = cJSON_GetObjectItemCaseSensitive(root, "type");
        cJSON *configs = cJSON_GetObjectItemCaseSensitive(root, "configs");
        int confignum = cJSON_GetArraySize(configs);
        for(ifor=0;ifor<confignum;ifor++)
        {
            cJSON * config=cJSON_GetArrayItem(configs,ifor);
            cJSON * key = cJSON_GetObjectItemCaseSensitive(config, "key");
            cJSON * value = cJSON_GetObjectItemCaseSensitive(config, "value");
            printf("key=%s value=%s\n",key->valuestring,value->valuestring);
        }
        cJSON_Delete(root);
    }
}




int tcp_service_callback( int size,char * buf,void * buff )
{   

    //    char * json_str="{\
    \"filename\":\"/etc/abl.conf\",\
    \"type\":\"read\",\
    \"configs\":[{\"key\":\"a\",\"value\":\"2\"},{\"key\":\"b\",\"value\":\"3\"}]\
}";
    int ifor;
    if(!check_pak_conf(size,buf)){
        cJSON * root = NULL;
        root=cJSON_Parse(buf);
        if(root==NULL)
            return -1;
        cJSON *filename = cJSON_GetObjectItemCaseSensitive(root, "filename");
        cJSON *type = cJSON_GetObjectItemCaseSensitive(root, "type");
        cJSON *configs = cJSON_GetObjectItemCaseSensitive(root, "configs");
        int confignum = cJSON_GetArraySize(configs);
        for(ifor = 0;ifor<confignum;ifor++)
        {
            cJSON * config = cJSON_GetArrayItem(configs,ifor);
            cJSON * key = cJSON_GetObjectItemCaseSensitive(config, "key");
            cJSON * value = cJSON_GetObjectItemCaseSensitive(config, "value");
            printf("key=%s value=%s\n",key->valuestring,value->valuestring);
        }
        cJSON_Delete(root);
    }
}















/*
{
    "name": "Jack (\"Bee\") Nimble",
    "format": {
        "type":       "rect",
        "width":      1920,
        "height":     1080,
        "interlace":  false,
        "frame rate": 24
    }
}
Assume that you got this from a file, a webserver, or magic JSON elves, whatever, you have a char * to it. Everything is a cJSON struct. Get it parsed:

cJSON * root = cJSON_Parse(my_json_string);
This is an object. We're in C. We don't have objects. But we do have structs. What's the framerate?

cJSON *format = cJSON_GetObjectItemCaseSensitive(root, "format");
cJSON *framerate_item = cJSON_GetObjectItemCaseSensitive(format, "frame rate");
double framerate = 0;
if (cJSON_IsNumber(framerate_item))
{
  framerate = framerate_item->valuedouble;
}

*/


int json_test(void)
{
    //先创建空对象
    printf("json version: %s\n",cJSON_Version());
    cJSON *json = cJSON_CreateObject();
    //在对象上添加键值对
    cJSON_AddStringToObject(json,"country","china");
    //添加数组
    cJSON *array = NULL;
    cJSON_AddItemToObject(json,"stars",array=cJSON_CreateArray());
    //在数组上添加对象
    cJSON *obj = NULL;
    cJSON_AddItemToArray(array,obj=cJSON_CreateObject());
    cJSON_AddItemToObject(obj,"name",cJSON_CreateString("Faye"));
    cJSON_AddStringToObject(obj,"address","beijing");
    //在对象上添加键值对
    cJSON_AddItemToArray(array,obj=cJSON_CreateObject());
    cJSON_AddItemToObject(obj,"name",cJSON_CreateString("andy"));
    cJSON_AddItemToObject(obj,"address",cJSON_CreateString("HK"));
    
    cJSON_AddItemToArray(array,obj=cJSON_CreateObject());
    cJSON_AddStringToObject(obj,"name","eddie");
    cJSON_AddStringToObject(obj,"address","TaiWan");
    
    //清理工作
    FILE *fp = fopen("create.json","w");
    char *buf = cJSON_Print(json);
    fwrite(buf,strlen(buf),1,fp);
    fclose(fp);
    cJSON_Delete(json);
    //jsos_parse();
    char * json_str="{\
    \"filename\":\"/etc/abl.conf\",\
    \"configs\":[{\"key\":\"a\",\"value\":\"2\"},{\"key\":\"b\",\"value\":\"3\"}]\
}";
    printf("json_str=%s\n",json_str);
    tcp_conf_callback(50,json_str,json_str);
    return 0;
}


int jsos_parse()
{
    char *string = "{\"family\":[\"father\",\"mother\",\"brother\",\"sister\",\"somebody\"]}";
    //从缓冲区中解析出JSON结构
    cJSON *json = cJSON_Parse(string);
    cJSON *node = NULL;
    //cJOSN_GetObjectItem 根据key来查找json节点 若果有返回非空
    node = cJSON_GetObjectItem(json,"family");
    if(node == NULL)
    {
        printf("family node == NULL\n");
    }
    else
    {
        printf("found family node\n");
    }
    node = cJSON_GetObjectItem(json,"famil");
    if(node == NULL)
    {
                printf("famil node == NULL\n");
        }
    else
        {
                printf("found famil node\n");
        }
    //判断是否有key是string的项 如果有返回1 否则返回0
    if(1 == cJSON_HasObjectItem(json,"family"))
    {
        printf("found family node\n");
    }
    else
    {
        printf("not found family node\n");
    }
    if(1 == cJSON_HasObjectItem(json,"famil"))
        {
                printf("found famil node\n");
        }
        else
        {
                printf("not found famil node\n");
        }
    
    node = cJSON_GetObjectItem(json,"family");
    if(node->type == cJSON_Array)
    {
        printf("array size is %d",cJSON_GetArraySize(node));
    }
    //非array类型的node 被当做array获取size的大小是未定义的行为 不要使用
    cJSON *tnode = NULL;
    int size = cJSON_GetArraySize(node);
    int i;
    for(i=0;i<size;i++)
    {
        tnode = cJSON_GetArrayItem(node,i);
        if(tnode->type == cJSON_String)
        {
            printf("value[%d]:%s\n",i,tnode->valuestring);
        }
        else
        {
            printf("node' type is not string\n");
        }
    }

    cJSON_ArrayForEach(tnode,node)
    {
        if(tnode->type == cJSON_String)
        {
            printf("int forEach: vale:%s\n",tnode->valuestring);    
        }
        else
        {
            printf("node's type is not string\n");
        }
    }
    return 0;
}