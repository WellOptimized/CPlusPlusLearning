#include<unistd.h>
#include<sys/socket.h>
#include<assert.h>
#include<stdio.h>

int main(int argc,char* argv[]){
    int fds[2];
    int ret=socketpair(AF_UNIX,SOCK_STREAM,0,fds);
    assert(ret==0);
    char buf[2];
    buf[0]='8';
    buf[1]='\0';
    write(fds[1],buf,2);
    char buf1[2];
    read(fds[0],buf1,2);
    printf("%s\n",buf1);

    buf[0]='9';
    write(fds[0],buf,2);
    read(fds[1],buf1,2);
    printf("%s\n",buf1);
    return 0;
}