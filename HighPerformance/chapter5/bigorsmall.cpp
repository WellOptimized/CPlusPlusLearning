#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
void byteorder(){
    union{
        short value;
        char union_bytes[sizeof(short)];
    }test;
    test.value=0x0102;
    if(test.union_bytes[0]==0x01 && test.union_bytes[1]==0x02){
        printf("big endian\n");
    }else if(test.union_bytes[0]==0x02 && test.union_bytes[1]==0x01){
        printf("little endian\n");
    }else{
        printf("unknown...\n");
    }
}
int main(int argc,char* argv[]){
    byteorder();
    struct in_addr v1,v2;
    v1.s_addr=0x01020304;
    v2.s_addr=0x0a0b0c0d;

    char* szValue1=inet_ntoa(v1);
    char* szValue2=inet_ntoa(v2);
    printf("address 1:%s\n",szValue1);
    printf("address 2:%s\n",szValue2);
    return 0;
}