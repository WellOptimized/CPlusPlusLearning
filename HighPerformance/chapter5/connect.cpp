#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<errno.h>

int main(int argc,char* argv[]){
    if(argc<=2){
        printf("error:argument <=2!");
        return 1;
    }
    const char* ip=argv[1];
    int port=atoi(argv[2]);

    int sock=socket(PF_INET,SOCK_STREAM,0);
    assert(sock>=0);

    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family=AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port=htons(port);

    if(connect(sock,(struct sockaddr*)&address,sizeof(address))<0){
        printf("connection failed\n");
    }else{
        const char* oob_data="abc";
        const char* normal_data="12357238975982375982374892739479283749832";
        send(sock,normal_data,strlen(normal_data),0);

        char buf[512];
        bzero(buf,512);
        
        recv(sock,buf,512,0);
        printf("%s\n",buf);
        // send(sock,oob_data,strlen(oob_data),MSG_OOB);
        // send(sock,normal_data,strlen(normal_data),0);
        
        // char buf[1024];
        // recv(sock,buf,1024,0);
        // printf("%s\n",buf);
        // send(sock,buf,1024,0);

        // char buf[1024];
        // buf[0]='0';
        // buf[1]='1';
        // buf[2]='2';
        // buf[3]='\0';
        // buf[1023]='\0';
        // send(sock,buf,3,0);
        // recv(sock,buf,3,0);
        // printf("%s\n",buf);
        // printf("???\n");
    }
    close(sock);
    return 0;
}
