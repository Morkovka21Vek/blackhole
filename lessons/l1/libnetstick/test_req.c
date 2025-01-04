#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "netstick.h"

int main(int argc, char** argv){
  int result = 0;
  int* sockfd = (int*) malloc(sizeof(int));

  const unsigned int recvBuffSize = 1024;
  char recvBuff[recvBuffSize];
  memset(recvBuff,0,recvBuffSize);

  if(argc == 1)
  {
    const char *get_req = "GET /get HTTP/1.1\r\nHost: httpbin.org\r\nUser-Agent: netstick\r\nAccept: */*\r\n\r\n";

    result = init_sock(sockfd, 80, "34.200.57.114");
    if (result != 1)
    {
      printf("\033[0;31mError! err id: %d\033[0m\n", result);
      goto err_exit;
    }

    result = send_req(*sockfd, get_req, recvBuff, recvBuffSize);
    printf("\033[0;32msend_req result = %d\033[0m\n", result);
    fputs(recvBuff, stdout);
  }
  else if(argc == 2)
  {
    const char *get_req = "GET /api/get_buttons_count HTTP/1.1\r\nHost: 192.168.4.1\r\nUser-Agent: netstick/0.0.1\r\nAccept: */*\r\n\r\n";
    //TODO ОБНОВИТЬ ПРОШИВКУ НА ESP32C2 и раскомментировать строку ниже взамен строки выше
    //const char *get_req = "GET /api/v1/get_buttons_count HTTP/1.1\r\nHost: 192.168.4.1\r\nUser-Agent: netstick/0.0.1\r\nAccept: */*\r\n\r\n";

    result = init_sock(sockfd, 80, argv[1]);
    if (result != 1)
    {
      printf("\033[0;31mError! err id: %d\033[0m\n", result);
      goto err_exit;
    }

    result = send_req(*sockfd, get_req, recvBuff, recvBuffSize);
    printf("\033[0;32msend_req result = %d\033[0m\n", result);
    fputs(recvBuff, stdout);
  }
  else
    printf("\nUsage: %s <ip of server>\nOr\nUsage: %s\tfor req to httpbin.org\n", argv[0], argv[0]);

err_exit:
  printf("\nExit\n");
  return 0;
}
