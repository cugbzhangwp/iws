#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <libgen.h>

#include <printcolor.h>
#include <cstp.h>
#include <app_main.h>
extern APP_S app;
#define BUFSIZE 1024
//IWS_CSTP iws_cstp_globe;










int init_iws_reg_pak(IWS_REGISTER * iws_reg_pak)
{
    iws_reg_pak->type_lable[0]='R';
    iws_reg_pak->type_lable[1]='E';
    iws_reg_pak->package_number=0;
    snprintf(iws_reg_pak->sta_id,8,"%s","jd.00001");
    snprintf(iws_reg_pak->key,32,"%s","xxxxxxxxxxxxxx");
    iws_reg_pak->control_authority=0;
    iws_reg_pak->init_tran_mode=1;
/*
    char type_lable[2];
    int package_number;
    char sta_id[8];
    char key[32];
    short control_authority;
    int longitude;
    int latitude;
    short altitude;
    int sensitivity;
    short init_tran_mode;
    short init_trigger_threshold;
    char unuse[192];
*/
}
/*
    IWS_REGISTER iws_reg_pak;//注册信息包
    IWS_REGISTER_RE iws_reg_re_pak;//注册信息相应包
    IWS_TRANS_CTRL iws_control_pak;//传输模式控制包
    IWS_TIMEDATA_REQUEST iws_timedata_request_pak;//时段请求数据包
    IWS_TRIGGER_THRESHOLD_CTRL iws_trigger_threshold_pak;//触发阈值设置包
    IWS_CC_RE iws_cc_re_pak;//控制命令应答包
    IWS_UP_SI iws_up_si_pak;//状态信息包
    IWS_UP_TI iws_up_ti_pak;//触发信息包
*/
int init_iws_up_ti_pak(IWS_UP_TI * iws_up_ti_pak)
{
/*
    char type_lable[2];
    int package_number;
    char sta_id[8];
    char key[32];
    short control_authority;
    int longitude;
    int latitude;
    short altitude;
    int sensitivity;
    short init_tran_mode;
    short init_trigger_threshold;
    char unuse[192];
*/
}

int init_iws_up_si_pak(IWS_UP_SI * iws_up_si_pak)
{
/*
    char type_lable[2];
    int package_number;
    char sta_id[8];
    char key[32];
    short control_authority;
    int longitude;
    int latitude;
    short altitude;
    int sensitivity;
    short init_tran_mode;
    short init_trigger_threshold;
    char unuse[192];
*/
}
int init_iws_cc_re_pak(IWS_CC_RE * iws_cc_re_pak)
{
/*
    char type_lable[2];
    int package_number;
    char sta_id[8];
    char key[32];
    short control_authority;
    int longitude;
    int latitude;
    short altitude;
    int sensitivity;
    short init_tran_mode;
    short init_trigger_threshold;
    char unuse[192];
*/
}
int init_iws_trigger_threshold_pak(IWS_TRIGGER_THRESHOLD_CTRL * iws_trigger_threshold_pak)
{
/*
    char type_lable[2];
    int package_number;
    char sta_id[8];
    char key[32];
    short control_authority;
    int longitude;
    int latitude;
    short altitude;
    int sensitivity;
    short init_tran_mode;
    short init_trigger_threshold;
    char unuse[192];
*/
}
int init_iws_timedata_request_pak(IWS_TIMEDATA_REQUEST * iws_timedata_request_pak)
{
/*
    char type_lable[2];
    int package_number;
    char sta_id[8];
    char key[32];
    short control_authority;
    int longitude;
    int latitude;
    short altitude;
    int sensitivity;
    short init_tran_mode;
    short init_trigger_threshold;
    char unuse[192];
*/
}


int init_iws_control_pak(IWS_TRANS_CTRL * iws_control_pak)
{
/*
    char type_lable[2];
    int package_number;
    char sta_id[8];
    char key[32];
    short control_authority;
    int longitude;
    int latitude;
    short altitude;
    int sensitivity;
    short init_tran_mode;
    short init_trigger_threshold;
    char unuse[192];
*/
}

int init_iws_reg_re_pak(IWS_REGISTER_RE * iws_reg_re_pak)
{
/*
    char type_lable[2];
    int package_number;
    char sta_id[8];
    char key[32];
    short control_authority;
    int longitude;
    int latitude;
    short altitude;
    int sensitivity;
    short init_tran_mode;
    short init_trigger_threshold;
    char unuse[192];
*/
}




int init_cstp_globe(IWS_CSTP *iws_cstp_globe)
{
    init_iws_reg_pak(&(iws_cstp_globe->iws_reg_pak));
/*
    char type_lable[2];
    int package_number;
    char sta_id[8];
    char key[32];
    short control_authority;
    int longitude;
    int latitude;
    short altitude;
    int sensitivity;
    short init_tran_mode;
    short init_trigger_threshold;
    char unuse[192];
*/
}
int wait_for_device_ok(APP_S * app)
{
    while(1)
    {
        //检查各设备状态
    }
    return 0;
}

#define CSTP_UP_PAKLEN 256
#define CSTP_DOWN_PAKLEN 64
int check_reg_re(char * buf)
{
    /*
    char type_lable[2];
    int package_number;
    char sta_id[8];
    short auth_state;
    int last_package_number;
    char unuse[44];
    */
    IWS_REGISTER_RE * reg_re=(IWS_REGISTER_RE * )buf;
    if(reg_re->type_lable[0]=='R'&&reg_re->type_lable[1]=='R'){
        ;
    }
    else{
        return -1;
    }
    if(reg_re->auth_state==0){
        return 0;
    }
    else{
        return -2;
    }
}
int register_device(APP_S * app,IWS_REGISTER * iws_reg_pak,int sockfd)//等待设备状态就绪，从而开始设备注册
{
    char readbuf[1024];
    int success_len,sendlen=0,recvlen=0;
    while(1){
        sendlen = write(sockfd, iws_reg_pak, sizeof(IWS_REGISTER));
        if(sendlen==CSTP_UP_PAKLEN){
            recvlen = read(sockfd, readbuf, CSTP_DOWN_PAKLEN);
            if(check_reg_re(readbuf)==0){
                //&(iws_cstp_globe->iws_reg_pak)
                return 0;
            }
        }
        
    }
}




int rw_select(int fid)
{
    fd_set t_set1,t_set_server,t_set_read,t_set_write;
    struct timeval  tv;
    FD_ZERO(&t_set_write);
    FD_ZERO(&t_set_read);
    FD_SET(fid, &t_set_write);
    FD_SET(fid, &t_set_read);
    tv.tv_sec= 10;
    tv.tv_usec= 1000;
    int i_state= 0;
    //printf("--------------->3\ttcp_ser wait for select\n");
    i_state= select(fid+1,  &t_set_read, &t_set_write,NULL, &tv);//*/

    if(FD_ISSET(fid,&t_set_read)){//判断是否可读
        printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        //exit(0);
        return 1;
    }
    if (i_state < 0) {
        printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
        printf("在读取数据报文时SELECT检测到异常，该异常导致线程终止！\n");
        return -1;
    };
    if (i_state == 0){
        printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    }
    if(FD_ISSET(fid,&t_set_write)){//判断是否可写
        return 2;
    }


}





int iws_read_process(int fid)
{
    char buf[1024];
    int realread;
    realread=read(fid,buf,1024);
    int ifor;
    if(realread<1)
        return realread;
    for(ifor=0;ifor<realread;ifor++){
        printf("%X ",buf[ifor]);
    }
    printf("\n");
    return realread;
}


extern int writenbytes(char * ch,int sockfd,int needread);

int iws_write_process(int fid,int connect_index,APP_S * app)
{

    printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
    char buf[1024];
    int realread=0;

    writenbytes(app->app_server.connect[connect_index].sendbuf->data_start,fid,app->app_para.fs*sizeof(DATA_FRAME_GEO_S));
    //write(sockfd,p_app->app_server.connect[connect_index].sendbuf->data_start,200*sizeof(DATA_FRAME_GEO_S));
    app->app_server.connect[connect_index].sig_send--;
    //printf(GREEN"function：%s,line:%d,app.app_sig.sig_data_send=%d\n"NONE,__FUNCTION__,__LINE__,p_app->app_sig.sig_data_send);
    app->app_server.connect[connect_index].sendbuf=app->app_server.connect[connect_index].sendbuf->loopbufnext;
    return realread; 
}
















int iws_client1( APP_S * app)
{
    //IWS_CSTP iws_cstp;
    printf("in iws_client ping_pang=%d\n",app->ping_pang);
    int ifor,i_connect;
            // for(ifor=0;ifor<10;ifor++)
            // {
            //    //if(0==app->app_server.connect[ifor].is_connect){
            //         app.app_server.connect[ifor].is_connect=1;
            //         //app->app_server.connect[ifor].sendbuf=app->buffer.databuf[LOOPREAD];
            //         i_connect=ifor;
            //         printf("i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",i_connect,app.app_server.connect[ifor].is_connect);
            //         //break;
            //    //}
            // }
    while(1){
            for(ifor=0;ifor<10;ifor++)
            {
               //if(0==app->app_server.connect[ifor].is_connect){
                    //app.app_server.connect[ifor].is_connect=1;
                    //app->app_server.connect[ifor].sendbuf=app->buffer.databuf[LOOPREAD];
                    i_connect=ifor;
                    printf("i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",i_connect,app->app_server.connect[ifor].is_connect);
                    //break;
               //}
            }
            return 0;
    }


            //exit(0);
//     init_cstp_globe(&iws_cstp_globe);
//     struct sockaddr_in servaddr;
//     char buf[BUFSIZE];
//     int sockfd, n;

//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     bzero(&servaddr, sizeof(servaddr));
//     servaddr.sin_family = AF_INET;
//     //inet_pton(AF_INET, "222.222.119.11", &servaddr.sin_addr);
//     inet_pton(AF_INET, "192.168.12.5", &servaddr.sin_addr);
//     servaddr.sin_port = htons(8888);

//     /*连接服务端*/
//     printf("try to connect\n");
//     //wait_for_device_ok(app);//等待设备状态就绪，从而开始设备注册
//     int reg_status;
// /*
//     pthread_t  newthread;
//     int ii=osonewthread(&newthread,app_server_start_ctrl,(void * )app);
// */
//     int on_connect=0;
//     int realread,realwrite,rw_select_ret;
//     //int i_connect;//标记连接索引,以方便ad给信号
//     while(1){
//         int ret=connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
//         if(ret==0){
//             on_connect=1;
//             for(ifor=0;ifor<10;ifor++)
//             {
//                if(0==app->app_server.connect[ifor].is_connect){
//                     app->app_server.connect[ifor].is_connect=1;
//                     app->app_server.connect[ifor].sendbuf=app->buffer.databuf[LOOPREAD];
//                     i_connect=ifor;
//                     printf("i_connect=%d,app->app_server.connect[ifor].is_connect=%d\n",i_connect,app->app_server.connect[ifor].is_connect);
//                     break;
//                }
//             }
//         }
//         else{
//             on_connect=-1;
//         }
//         while(on_connect){
            
//             reg_status=0;//register_device(app,&(iws_cstp_globe.iws_reg_pak),sockfd);//等待设备状态就绪，从而开始设备注册
//             if(reg_status==0){
//                 rw_select_ret=rw_select(sockfd);
//                 printf(RED"inside function %s line:%d rw_select_ret=%d\n"NONE,__FUNCTION__,__LINE__,rw_select_ret);
//                 switch(rw_select_ret)
//                 {
//                     case 1:
//                         realread=iws_read_process(sockfd);
//                         if(realread==-1){
//                             close(sockfd);
//                             on_connect=0; 
//                         }

//                     break;
//                     case 2:
//                         printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
//                         //app->app_server.connect[i_connect].sig_send=123;
//                         printf(RED"sig_send=%d\n"NONE,app->app_server.connect[ifor].sig_send);

//                         while(app->app_server.connect[i_connect].sig_send<=0)
//                         {
//                             //printf(RED"sig_send=%d\n"NONE,app->app_server.connect[ifor].sig_send);
//                             app->app_server.connect[i_connect].is_connect=1;
//                             //app.app_server.connect[ifor].sig_send++
//                             usleep(250*1000);
//                             //
//                             //wait_go_signal(1);
                            
//                             printf(GREEN"i_connect=%d,app.app_server.connect[i_connect].is_connect=%d\n"NONE,i_connect,app->app_server.connect[i_connect].is_connect);

//                             printf(RED"inside function %s line:%d\n"NONE,__FUNCTION__,__LINE__);
//                         }
//                         realwrite=iws_write_process(sockfd,i_connect,app);
//                         if(realwrite==-1){
//                             close(sockfd);
//                             on_connect=0; 
//                         }
//                     break;
//                     case -1:
//                         close(sockfd);
//                         on_connect=0;
//                     break;
//                     default:
//                     break;
                        
//                 }
//                 //realsend=write(client_sockfd[i_connect],sendbuf+sendcount,sizeof(NET_packet)+DATATOSENDSIZE-sendcount);
//                 //n = read(sockfd, buf, BUFSIZE);
//             }
//             else{
//                 close(sockfd);
//                 on_connect=0;
//             }
//         }


//         printf("end ret=%d\n",ret);
//     }
//     // while(fgets(buf, BUFSIZE, stdin) != NULL){
//     //     /*通过sockfd给服务端发送数据*/
//     //     write(sockfd, buf, strlen(buf));
//     //     n = read(sockfd, buf, BUFSIZE);
//     //     if(n == 0)
//     //         printf("the other side has been closed.\n");
//     //     else/*打印输出服务端传过来的数据*/
//     //         write(STDOUT_FILENO, buf, n);
//     // }
//     return 0;
}










// /*
// Copyright (c) 2009-2018 Roger Light <roger@atchoo.org>

// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v1.0
// and Eclipse Distribution License v1.0 which accompany this distribution.
 
// The Eclipse Public License is available at
//    http://www.eclipse.org/legal/epl-v10.html
// and the Eclipse Distribution License is available at
//   http://www.eclipse.org/org/documents/edl-v10.php.
 
// Contributors:
//    Roger Light - initial implementation and documentation.
// */

// /* For nanosleep */
// /*
// #define _POSIX_C_SOURCE 200809L

// #include <errno.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include <mosquitto.h>
// #include <client_shared.h>

// #define STATUS_CONNECTING 0
// #define STATUS_CONNACK_RECVD 1
// #define STATUS_WAITING 2
// #define STATUS_DISCONNECTING 3

// /* Global variables for use in callbacks. See sub_client.c for an example of
//  * using a struct to hold variables for use in callbacks. */
// static char *topic = NULL;
// static char *message = NULL;
// static long msglen = 0;
// static int qos = 0;
// static int retain = 0;
// static int mode = MSGMODE_NONE;
// static int status = STATUS_CONNECTING;
// static int mid_sent = 0;
// static int last_mid = -1;
// static int last_mid_sent = -1;
// static bool connected = true;
// static char *username = NULL;
// static char *password = NULL;
// static bool disconnect_sent = false;
// static bool quiet = false;

// void my_connect_callback(struct mosquitto *mosq, void *obj, int result)
// {
//     int rc = MOSQ_ERR_SUCCESS;

//     if(!result){
//         switch(mode){
//             case MSGMODE_CMD:
//             case MSGMODE_FILE:
//             case MSGMODE_STDIN_FILE:
//                 rc = mosquitto_publish(mosq, &mid_sent, topic, msglen, message, qos, retain);
//                 break;
//             case MSGMODE_NULL:
//                 rc = mosquitto_publish(mosq, &mid_sent, topic, 0, NULL, qos, retain);
//                 break;
//             case MSGMODE_STDIN_LINE:
//                 status = STATUS_CONNACK_RECVD;
//                 break;
//         }
//         if(rc){
//             if(!quiet){
//                 switch(rc){
//                     case MOSQ_ERR_INVAL:
//                         fprintf(stderr, "Error: Invalid input. Does your topic contain '+' or '#'?\n");
//                         break;
//                     case MOSQ_ERR_NOMEM:
//                         fprintf(stderr, "Error: Out of memory when trying to publish message.\n");
//                         break;
//                     case MOSQ_ERR_NO_CONN:
//                         fprintf(stderr, "Error: Client not connected when trying to publish.\n");
//                         break;
//                     case MOSQ_ERR_PROTOCOL:
//                         fprintf(stderr, "Error: Protocol error when communicating with broker.\n");
//                         break;
//                     case MOSQ_ERR_PAYLOAD_SIZE:
//                         fprintf(stderr, "Error: Message payload is too large.\n");
//                         break;
//                 }
//             }
//             mosquitto_disconnect(mosq);
//         }
//     }else{
//         if(result && !quiet){
//             fprintf(stderr, "%s\n", mosquitto_connack_string(result));
//         }
//     }
// }

// void my_disconnect_callback(struct mosquitto *mosq, void *obj, int rc)
// {
//     connected = false;
// }

// void my_publish_callback(struct mosquitto *mosq, void *obj, int mid)
// {
//     last_mid_sent = mid;
//     if(mode == MSGMODE_STDIN_LINE){
//         if(mid == last_mid){
//             mosquitto_disconnect(mosq);
//             disconnect_sent = true;
//         }
//     }else if(disconnect_sent == false){
//         mosquitto_disconnect(mosq);
//         disconnect_sent = true;
//     }
// }

// void my_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str)
// {
//     printf("%s\n", str);
// }

// int load_stdin(void)
// {
//     long pos = 0, rlen;
//     char buf[1024];
//     char *aux_message = NULL;

//     mode = MSGMODE_STDIN_FILE;

//     while(!feof(stdin)){
//         rlen = fread(buf, 1, 1024, stdin);
//         aux_message = realloc(message, pos+rlen);
//         if(!aux_message){
//             if(!quiet) fprintf(stderr, "Error: Out of memory.\n");
//             free(message);
//             return 1;
//         } else
//         {
//             message = aux_message;
//         }
//         memcpy(&(message[pos]), buf, rlen);
//         pos += rlen;
//     }
//     msglen = pos;

//     if(!msglen){
//         if(!quiet) fprintf(stderr, "Error: Zero length input.\n");
//         return 1;
//     }

//     return 0;
// }

// int load_file(const char *filename)
// {
//     long pos, rlen;
//     FILE *fptr = NULL;

//     fptr = fopen(filename, "rb");
//     if(!fptr){
//         if(!quiet) fprintf(stderr, "Error: Unable to open file \"%s\".\n", filename);
//         return 1;
//     }
//     mode = MSGMODE_FILE;
//     fseek(fptr, 0, SEEK_END);
//     msglen = ftell(fptr);
//     if(msglen > 268435455){
//         fclose(fptr);
//         if(!quiet) fprintf(stderr, "Error: File \"%s\" is too large (>268,435,455 bytes).\n", filename);
//         return 1;
//     }else if(msglen == 0){
//         fclose(fptr);
//         if(!quiet) fprintf(stderr, "Error: File \"%s\" is empty.\n", filename);
//         return 1;
//     }else if(msglen < 0){
//         fclose(fptr);
//         if(!quiet) fprintf(stderr, "Error: Unable to determine size of file \"%s\".\n", filename);
//         return 1;
//     }
//     fseek(fptr, 0, SEEK_SET);
//     message = malloc(msglen);
//     if(!message){
//         fclose(fptr);
//         if(!quiet) fprintf(stderr, "Error: Out of memory.\n");
//         return 1;
//     }
//     pos = 0;
//     while(pos < msglen){
//         rlen = fread(&(message[pos]), sizeof(char), msglen-pos, fptr);
//         pos += rlen;
//     }
//     fclose(fptr);
//     return 0;
// }


// #define VERSION 0
// void print_usage(void)
// {
//     int major, minor, revision;

//     mosquitto_lib_version(&major, &minor, &revision);
//     printf("mosquitto_pub is a simple mqtt client that will publish a message on a single topic and exit.\n");
//     printf("mosquitto_pub version %s running on libmosquitto %d.%d.%d.\n\n", VERSION, major, minor, revision);
//     printf("Usage: mosquitto_pub {[-h host] [-p port] [-u username [-P password]] -t topic | -L URL}\n");
//     printf("                     {-f file | -l | -n | -m message}\n");
//     printf("                     [-c] [-k keepalive] [-q qos] [-r]\n");
// #ifdef WITH_SRV
//     printf("                     [-A bind_address] [-S]\n");
// #else
//     printf("                     [-A bind_address]\n");
// #endif
//     printf("                     [-i id] [-I id_prefix]\n");
//     printf("                     [-d] [--quiet]\n");
//     printf("                     [-M max_inflight]\n");
//     printf("                     [-u username [-P password]]\n");
//     printf("                     [--will-topic [--will-payload payload] [--will-qos qos] [--will-retain]]\n");
// #ifdef WITH_TLS
//     printf("                     [{--cafile file | --capath dir} [--cert file] [--key file]\n");
//     printf("                      [--ciphers ciphers] [--insecure]]\n");
// #ifdef WITH_TLS_PSK
//     printf("                     [--psk hex-key --psk-identity identity [--ciphers ciphers]]\n");
// #endif
// #endif
// #ifdef WITH_SOCKS
//     printf("                     [--proxy socks-url]\n");
// #endif
//     printf("       mosquitto_pub --help\n\n");
//     printf(" -A : bind the outgoing socket to this host/ip address. Use to control which interface\n");
//     printf("      the client communicates over.\n");
//     printf(" -d : enable debug messages.\n");
//     printf(" -f : send the contents of a file as the message.\n");
//     printf(" -h : mqtt host to connect to. Defaults to localhost.\n");
//     printf(" -i : id to use for this client. Defaults to mosquitto_pub_ appended with the process id.\n");
//     printf(" -I : define the client id as id_prefix appended with the process id. Useful for when the\n");
//     printf("      broker is using the clientid_prefixes option.\n");
//     printf(" -k : keep alive in seconds for this client. Defaults to 60.\n");
//     printf(" -L : specify user, password, hostname, port and topic as a URL in the form:\n");
//     printf("      mqtt(s)://[username[:password]@]host[:port]/topic\n");
//     printf(" -l : read messages from stdin, sending a separate message for each line.\n");
//     printf(" -m : message payload to send.\n");
//     printf(" -M : the maximum inflight messages for QoS 1/2..\n");
//     printf(" -n : send a null (zero length) message.\n");
//     printf(" -p : network port to connect to. Defaults to 1883 for plain MQTT and 8883 for MQTT over TLS.\n");
//     printf(" -P : provide a password\n");
//     printf(" -q : quality of service level to use for all messages. Defaults to 0.\n");
//     printf(" -r : message should be retained.\n");
//     printf(" -s : read message from stdin, sending the entire input as a message.\n");
// #ifdef WITH_SRV
//     printf(" -S : use SRV lookups to determine which host to connect to.\n");
// #endif
//     printf(" -t : mqtt topic to publish to.\n");
//     printf(" -u : provide a username\n");
//     printf(" -V : specify the version of the MQTT protocol to use when connecting.\n");
//     printf("      Can be mqttv31 or mqttv311. Defaults to mqttv311.\n");
//     printf(" --help : display this message.\n");
//     printf(" --quiet : don't print error messages.\n");
//     printf(" --will-payload : payload for the client Will, which is sent by the broker in case of\n");
//     printf("                  unexpected disconnection. If not given and will-topic is set, a zero\n");
//     printf("                  length message will be sent.\n");
//     printf(" --will-qos : QoS level for the client Will.\n");
//     printf(" --will-retain : if given, make the client Will retained.\n");
//     printf(" --will-topic : the topic on which to publish the client Will.\n");
// #ifdef WITH_TLS
//     printf(" --cafile : path to a file containing trusted CA certificates to enable encrypted\n");
//     printf("            communication.\n");
//     printf(" --capath : path to a directory containing trusted CA certificates to enable encrypted\n");
//     printf("            communication.\n");
//     printf(" --cert : client certificate for authentication, if required by server.\n");
//     printf(" --key : client private key for authentication, if required by server.\n");
//     printf(" --ciphers : openssl compatible list of TLS ciphers to support.\n");
//     printf(" --tls-version : TLS protocol version, can be one of tlsv1.2 tlsv1.1 or tlsv1.\n");
//     printf("                 Defaults to tlsv1.2 if available.\n");
//     printf(" --insecure : do not check that the server certificate hostname matches the remote\n");
//     printf("              hostname. Using this option means that you cannot be sure that the\n");
//     printf("              remote host is the server you wish to connect to and so is insecure.\n");
//     printf("              Do not use this option in a production environment.\n");
// #  ifdef WITH_TLS_PSK
//     printf(" --psk : pre-shared-key in hexadecimal (no leading 0x) to enable TLS-PSK mode.\n");
//     printf(" --psk-identity : client identity string for TLS-PSK mode.\n");
// #  endif
// #endif
// #ifdef WITH_SOCKS
//     printf(" --proxy : SOCKS5 proxy URL of the form:\n");
//     printf("           socks5h://[username[:password]@]hostname[:port]\n");
//     printf("           Only \"none\" and \"username\" authentication is supported.\n");
// #endif
//     printf("\nSee http://mosquitto.org/ for more information.\n\n");
// }

// int main_mqtt(int argc, char *argv[])
// {
//     struct mosq_config cfg;
//     struct mosquitto *mosq = NULL;
//     int rc;
//     int rc2;
//     char *buf, *buf2;
//     int buf_len = 1024;
//     int buf_len_actual;
//     int read_len;
//     int pos;

//     buf = malloc(buf_len);
//     if(!buf){
//         fprintf(stderr, "Error: Out of memory.\n");
//         return 1;
//     }

//     memset(&cfg, 0, sizeof(struct mosq_config));
//     rc = client_config_load(&cfg, CLIENT_PUB, argc, argv);
//     if(rc){
//         client_config_cleanup(&cfg);
//         if(rc == 2){
//             /* --help */
//             print_usage();
//         }else{
//             fprintf(stderr, "\nUse 'mosquitto_pub --help' to see usage.\n");
//         }
//         free(buf);
//         return 1;
//     }

//     topic = cfg.topic;
//     message = cfg.message;
//     msglen = cfg.msglen;
//     qos = cfg.qos;
//     retain = cfg.retain;
//     mode = cfg.pub_mode;
//     username = cfg.username;
//     password = cfg.password;
//     quiet = cfg.quiet;

//     if(cfg.pub_mode == MSGMODE_STDIN_FILE){
//         if(load_stdin()){
//             fprintf(stderr, "Error loading input from stdin.\n");
//             free(buf);
//             return 1;
//         }
//     }else if(cfg.file_input){
//         if(load_file(cfg.file_input)){
//             fprintf(stderr, "Error loading input file \"%s\".\n", cfg.file_input);
//             free(buf);
//             return 1;
//         }
//     }

//     if(!topic || mode == MSGMODE_NONE){
//         fprintf(stderr, "Error: Both topic and message must be supplied.\n");
//         print_usage();
//         free(buf);
//         return 1;
//     }


//     mosquitto_lib_init();

//     if(client_id_generate(&cfg, "mosqpub")){
//         free(buf);
//         return 1;
//     }

//     mosq = mosquitto_new(cfg.id, true, NULL);
//     if(!mosq){
//         switch(errno){
//             case ENOMEM:
//                 if(!quiet) fprintf(stderr, "Error: Out of memory.\n");
//                 break;
//             case EINVAL:
//                 if(!quiet) fprintf(stderr, "Error: Invalid id.\n");
//                 break;
//         }
//         mosquitto_lib_cleanup();
//         free(buf);
//         return 1;
//     }
//     if(cfg.debug){
//         mosquitto_log_callback_set(mosq, my_log_callback);
//     }
//     mosquitto_connect_callback_set(mosq, my_connect_callback);
//     mosquitto_disconnect_callback_set(mosq, my_disconnect_callback);
//     mosquitto_publish_callback_set(mosq, my_publish_callback);

//     if(client_opts_set(mosq, &cfg)){
//         free(buf);
//         return 1;
//     }
//     rc = client_connect(mosq, &cfg);
//     if(rc) return rc;

//     if(mode == MSGMODE_STDIN_LINE){
//         mosquitto_loop_start(mosq);
//     }

//     do{
//         if(mode == MSGMODE_STDIN_LINE){
//             if(status == STATUS_CONNACK_RECVD){
//                 pos = 0;
//                 read_len = buf_len;
//                 while(fgets(&buf[pos], read_len, stdin)){
//                     buf_len_actual = strlen(buf);
//                     if(buf[buf_len_actual-1] == '\n'){
//                         buf[buf_len_actual-1] = '\0';
//                         rc2 = mosquitto_publish(mosq, &mid_sent, topic, buf_len_actual-1, buf, qos, retain);
//                         if(rc2){
//                             if(!quiet) fprintf(stderr, "Error: Publish returned %d, disconnecting.\n", rc2);
//                             mosquitto_disconnect(mosq);
//                         }
//                         break;
//                     }else{
//                         buf_len += 1024;
//                         pos += 1023;
//                         read_len = 1024;
//                         buf2 = realloc(buf, buf_len);
//                         if(!buf2){
//                             free(buf);
//                             fprintf(stderr, "Error: Out of memory.\n");
//                             return 1;
//                         }
//                         buf = buf2;
//                     }
//                 }
//                 if(feof(stdin)){
//                     if(last_mid == -1){
//                         /* Empty file */
//                         mosquitto_disconnect(mosq);
//                         disconnect_sent = true;
//                         status = STATUS_DISCONNECTING;
//                     }else{
//                         last_mid = mid_sent;
//                         status = STATUS_WAITING;
//                     }
//                 }
//             }else if(status == STATUS_WAITING){
//                 if(last_mid_sent == last_mid && disconnect_sent == false){
//                     mosquitto_disconnect(mosq);
//                     disconnect_sent = true;
//                 }
// #ifdef WIN32
//                 Sleep(100);
// #else
//                 struct timespec ts;
//                 ts.tv_sec = 0;
//                 ts.tv_nsec = 100000000;
//                 nanosleep(&ts, NULL);
// #endif
//             }
//             rc = MOSQ_ERR_SUCCESS;
//         }else{
//             rc = mosquitto_loop(mosq, -1, 1);
//         }
//     }while(rc == MOSQ_ERR_SUCCESS && connected);

//     if(mode == MSGMODE_STDIN_LINE){
//         mosquitto_loop_stop(mosq, false);
//     }

//     if(message && mode == MSGMODE_FILE){
//         free(message);
//     }
//     mosquitto_destroy(mosq);
//     mosquitto_lib_cleanup();

//     if(rc){
//         fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
//     }
//     return rc;
// }
// */






