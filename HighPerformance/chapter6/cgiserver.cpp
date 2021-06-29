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

#define BUF_SIZE 1024
//tcp三次握手是在客户端发动connect（）时完成的，然后连接就排列在服务器listen队列中，服务器通过accept获取连接。
//listen队列中的连接至少已进入SYN_RCVD状态（完成了三次握手中的前两次）

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

    int ret=bind(sock,(struct sockaddr*)&address,sizeof(address));
    assert(ret!=-1);
    ret=listen(sock,5);
    assert(ret!=-1);

    struct sockaddr_in client;
    socklen_t client_length=sizeof(client);
    int connfd=accept(sock,(sockaddr*)&client,&client_length);
    if(connfd<0){
        printf("error is:%d\n",errno);
    }else{
        close(STDOUT_FILENO);
        dup(connfd);
        printf("1234\0");
        close(connfd);
    }
    close(sock);
    return 0;
}
