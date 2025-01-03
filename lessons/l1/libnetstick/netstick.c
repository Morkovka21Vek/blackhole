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
  int result = 0, sockfd = 0;

  const unsigned int recvBuffSize = 1024;
  char recvBuff[recvBuffSize];
  memset(recvBuff,0,recvBuffSize);

  if(argc == 1)
  {
    const char *get_req = "GET /get HTTP/1.1\r\nHost: httpbin.org\r\nUser-Agent: netstick\r\nAccept: */*\r\n\r\n";

    result = init_sock(80, "34.200.57.114");
    if (result >= 0) sockfd = result;
    else
    {
      printf("\033[0;31mError! err id: %d\033[0m\n", result);
      goto err_exit;
    }

    result = send_req(sockfd, get_req, recvBuff, recvBuffSize);
    printf("\033[0;32msend_req result = %d\033[0m\n", result);
    fputs(recvBuff, stdout);
  }
  else if(argc == 2)
  {
    const char *get_req = "GET /api/get_buttons_count HTTP/1.1\r\nHost: 192.168.4.1\r\nUser-Agent: netstick/0.0.1\r\nAccept: */*\r\n\r\n";
    //TODO ОБНОВИТЬ ПРОШИВКУ НА ESP32C2 и раскомментировать строку ниже взамен строки выше
    //const char *get_req = "GET /api/v1/get_buttons_count HTTP/1.1\r\nHost: 192.168.4.1\r\nUser-Agent: netstick/0.0.1\r\nAccept: */*\r\n\r\n";

    result = init_sock(80, argv[1]);
    if (result >= 0) sockfd = result;
    else {
      printf("\033[0;31mError! err id: %d\033[0m\n", result);
      goto err_exit;
    }

    result = send_req(sockfd, get_req, recvBuff, recvBuffSize);
    printf("\033[0;32msend_req result = %d\033[0m\n", result);
    fputs(recvBuff, stdout);
  }
  else
    printf("\nUsage: %s <ip of server>\nOr\nUsage: %s\tfor req to httpbin.org\n", argv[0], argv[0]);

err_exit:
  printf("\nExit\n");
  return 0;
}

int init_sock(int port, const char* serv_url){
  int result = 0, sockfd = 0;

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  do {
    if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
      printf("\n Error : Could not create socket \n");
      result = -2;
      break;
    }

    if(inet_pton(AF_INET, serv_url, &serv_addr.sin_addr) <= 0)
    {
      printf("\n inet_pton error occured\n");
      result = -3;
      break;
    }

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
      printf("\n Error : Connect Failed \n");
      result = -4;
      break;
    }
  } while(0);

  if (!result) result = sockfd;
  return result;
}


int send_req(const int sockfd, const char *get_req, char* recvBuff, const int recvBuffSize) {
  int n=0, result = 1;
  do {

    n = write(sockfd, get_req, strlen(get_req));

    if (n < strlen(get_req) && n >= 0)
    {
       unsigned int total_sent_length = n;
       while (total_sent_length < strlen(get_req)) {
        n = write(sockfd, get_req + total_sent_length, strlen(get_req) - total_sent_length);
        total_sent_length += n;
       }
    }
    else if (n == -1)
    {
       perror("Can't sent request\n");
    }
    else if (n != strlen(get_req))
    {
      perror("n != strlen(get_req)\n");
    }
    else
    {
      unsigned int total_buff_len = 0;
      while(1) {
        const unsigned int tempBuffSize = 1024;
        char tempBuff[tempBuffSize];
        memset(&tempBuff, 0, tempBuffSize);

        n = read(sockfd, tempBuff, tempBuffSize);
        if (n < 0) perror("ERROR! N < 0\n");

        if (n >= 0) {
          total_buff_len += n;
          strcat(recvBuff, tempBuff);
        }

        unsigned int response_len = 0;
        do {
          const char* content_length_header = "Content-Length: ";
          if(strstr(recvBuff, "\r\n\r\n") == NULL) break;

          char* content_length_start = strstr(recvBuff, content_length_header);
          if(content_length_start == NULL) break;
          content_length_start += strlen(content_length_header);

          char* content_length_end = strstr(content_length_start, "\r\n");
          if(content_length_end == NULL) break;

          size_t length_value_size = content_length_end - content_length_start;
          char *length_value = (char *)malloc((length_value_size + 1) * sizeof(char));
          if(length_value == NULL) break;

          strncpy(length_value, content_length_start, length_value_size);
          length_value[length_value_size] = 0;

          //char* headers_string = strstr(recvBuff, "\r\n\r\n");
          //response_len = atoi(length_value) + (headers_string - recvBuff) + 4;
          response_len = atoi(length_value) + (strlen(recvBuff) - strlen(strstr(recvBuff, "\r\n\r\n"))) + 4;
          free(length_value);
        } while(0);

        //printf("response_len = %d\n", response_len);
        //printf("total_buff_len = %d\n", total_buff_len);
        if (response_len != 0 && total_buff_len >= response_len) {
          recvBuff[total_buff_len] = 0;
          break;
        }
      }
    }
  } while (0);
  return result;
}
