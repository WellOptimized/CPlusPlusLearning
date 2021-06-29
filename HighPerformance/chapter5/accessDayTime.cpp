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
#include<netdb.h>

int main(int argc,char* argv[]){
    if(argc<=1){
        printf("error:argument <=1!");
        return 1;
    }
    const char* name=argv[1];
    printf("host name:%s\n",name);
    struct hostent* host=gethostbyname(name);
    assert(host);
    printf("%d\n",host->h_length);
    int sock=socket(PF_INET,SOCK_STREAM,0);
    assert(sock>=0);

    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family=AF_INET;
    
    address.sin_addr=*(struct in_addr*)(*host->h_addr_list);
    char buf[1024];
    printf("address : %s \n",inet_ntop(AF_INET,&address.sin_addr,buf,1024));

    struct servent* service=getservbyname("daytime","tcp");
    address.sin_port=service->s_port;
    printf("port : %d\n",ntohs(address.sin_port));
    int result=connect(sock,(struct sockaddr*)&address,sizeof(address));
    assert(result!=-1);

    bzero(buf,1024);
    result=read(sock,buf,sizeof(buf));
    assert(result>0);
    buf[result]='\0';
    printf("the day time is : %s\n",buf);    
    close(sock);
    return 0;
}
