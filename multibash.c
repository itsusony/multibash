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

void usage(){
    fprintf(stderr,"usage: \n\n./multibash -u meng.xiangliang -c CMD -h 10.0.0.1 -h 10.0.0.2 -h 10.0.0.3 ...\n\n");
    exit(1);
}

int main(int argc, char* argv[]){
    int result;
    char* usr=NULL;
    char** hosts = calloc(100,sizeof(char*));
    int hosts_len = 0;
    char* cmd=NULL;
    
    while((result=getopt(argc,argv,"u:h:c:"))!=-1){
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
            case ':':
            case '?':
                usage();break;
        }
    }
    if(usr==NULL||hosts_len==0||cmd==NULL||strlen(usr)==0||strlen(cmd)==0)usage();

    char* tc = strchr(cmd,'\'');
    while(1){
        if(!tc)break;
        tc[0]='"';
        tc=strchr(cmd,'\'');
    }

    int i;for(i=0;i<hosts_len;i++){
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
    }
    return 0;      
}
