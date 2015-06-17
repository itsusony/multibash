#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"
#define LENSIZE 10240

void usage(int master_mode){
    if(!master_mode)
        fprintf(stderr,"usage: \n\n./multibash -s 3 -u meng.xiangliang -c CMD -h 10.0.0.1 -h 10.0.0.2 -h 10.0.0.3 ...\n\n");
    else
        fprintf(stderr,"usage: \n\n./multibash -s 3 -u meng.xiangliang -c CMD -H 10.0.0 -S 1 -E 255 \n\n");
    exit(1);
}

int main(int argc, char* argv[]){
    int result;
    char* usr=NULL;
    char** hosts = calloc(100,sizeof(char*));
    int hosts_len = 0;
    char* cmd=NULL;

    int sleep_sec=0;
    int ip_start=0;
    int ip_end=0;
    char* master_host = NULL;

    while((result=getopt(argc,argv,"u:h:c:s:S:E:H:"))!=-1){
        switch(result){
            case 'u':
                usr=strdup(optarg);break;
            case 'h':
                if(hosts_len+1>100){
                    fprintf(stderr,"hosts max limit error\n");
                    return 1;
                }
                hosts[hosts_len++]=strdup(optarg);break;
            case 'c':
                cmd=strdup(optarg);break;
            case 's':
                if(optarg && strlen(optarg)>0)sleep_sec=atoi(optarg);break;
            case 'S':
                if(optarg && strlen(optarg)>0)ip_start=atoi(optarg);break;
            case 'E':
                if(optarg && strlen(optarg)>0)ip_end=atoi(optarg);break;
            case 'H':
                master_host=strdup(optarg);break;
            case ':':
            case '?':
                usage(0);break;
        }   
    }    
    if(master_host == NULL){
        if(usr==NULL||hosts_len==0||cmd==NULL||strlen(usr)==0||strlen(cmd)==0)usage(0);
    }else{
        if(usr==NULL||strlen(master_host)==0||cmd==NULL||strlen(usr)==0||strlen(cmd)==0||ip_start==0||ip_end==0||ip_end<ip_start)usage(1);
    }

    char* tc = strchr(cmd,'\'');
    while(1){
        if(!tc)break;
        tc[0]='"';
        tc=strchr(cmd,'\'');
    }

    int i;
    if(master_host == NULL){
        for(i=0;i<hosts_len;i++){
            char* ip = hosts[i];
            fprintf(stderr,KCYN "%s" RESET,ip);
            fprintf(stderr,"\n");
            char sh[102400] = "";
            strcat(sh,"ssh ");
            strcat(sh,usr);
            strcat(sh,"@");
            strcat(sh,ip);
            strcat(sh," '");
            strcat(sh,cmd);
            strcat(sh,"'");
            system(sh);
            if(sleep_sec>0 && i != hosts_len-1)sleep(sleep_sec);
        }
    }else{
        for(i=ip_start;i<=ip_end;i++){
            char ip[128] = "";
            if(master_host[strlen(master_host)-1]=='.')
                sprintf(ip,"%s%d",master_host,i);
            else
                sprintf(ip,"%s.%d",master_host,i);
            fprintf(stderr,KCYN "%s" RESET,ip);
            fprintf(stderr,"\n");
            char sh[102400] = "";
            strcat(sh,"ssh ");
            strcat(sh,usr);
            strcat(sh,"@");
            strcat(sh,ip);
            strcat(sh," '");
            strcat(sh,cmd);
            strcat(sh,"'");
            system(sh);
            if(sleep_sec>0 && i != ip_end)sleep(sleep_sec);
        }
    }
    return 0;
}
