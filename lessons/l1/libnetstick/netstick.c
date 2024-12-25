/* --- client.c --- */
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


int main(int argc, char** argv){

  int sockfd = 0, n = 0;
  int result = 1;
  const int recvBuffSize = 1024;
  char recvBuff[recvBuffSize];

  struct sockaddr_in serv_addr;

  do {

      if(argc != 2)
      {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        break;
      }

      memset(recvBuff,0,recvBuffSize);

      if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
      {
        printf("\n Error : Could not create socket \n");
        result = 2;
        break;
      }
    
      memset(&serv_addr, 0, sizeof(serv_addr));
    
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(80);
    
      if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
      {
         printf("\n inet_pton error occured\n");
         result = 3;
        break;
      }
            
      if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
      {
         printf("\n Error : Connect Failed \n");
         result = 4;
        break;
      }

      const char *get_req = "GET /api/get_buttons_count HTTP/1.1\r\nHost: 192.168.4.1\r\nUser-Agent: curl/8.5.0\r\nAccept: */*\r\n\r\n";
      n = write(sockfd,get_req,strlen(get_req));

      if (n != strlen(get_req))
      {
         //TODO если n != длинне get_req, нужно с нужного offset повторить операцию
      }
      else if (n == -1)
      {
         perror("Can't sent request");
      }
      else
      {
        n = read(sockfd,recvBuff, recvBuffSize - 1);
        recvBuff[n] = 0;
        fputs(recvBuff, stdout);

        n = read(sockfd,recvBuff, 3 + 4);
        recvBuff[n] = 0;
        fputs(recvBuff, stdout);
      }


      //n = read(sockfd,recvBuff,recvBuffSize - 1);

      // if (n > 0){
      //   recvBuff[n] = 0;
      //   fputs(recvBuff, stdout);
      // }
  } while (0);

  return 0;
}

