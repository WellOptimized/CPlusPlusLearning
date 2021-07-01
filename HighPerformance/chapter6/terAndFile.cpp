#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>

int main(int argc,char* argv[]){
    int filefd=open(argv[1],O_CREAT|O_WRONLY|O_TRUNC,0666);
    assert(filefd>0);
    int fds1[2];
    pipe(fds1);
    int fds2[2];
    pipe(fds2);
    splice(STDIN_FILENO,NULL,fds1[1],NULL,1024,0);
    tee(fds1[0],fds2[1],1024,0);
    splice(fds2[0],NULL,filefd,NULL,1024,0);
    splice(fds1[0],NULL,STDOUT_FILENO,NULL,1024,0);
    close(filefd);
    close(fds1[0]);
    close(fds1[1]);
    close(fds2[0]);
    close(fds2[1]);
    return 0;
}