#include <stdio.h>

int main(void){
    printf("GET / HTTP/1.1\r\nHost: fuck.com\r\nUser-Agent: TestHdrGenerator/0.0.0.0\r\nAccept: */*\r\n\r\n");
    return 0;
}
