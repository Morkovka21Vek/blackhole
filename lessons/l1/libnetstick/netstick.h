int init_sock(int port, const char* serv_url);
int send_req(const int sockfd, const char *get_req, char* recvBuff, const int recvBuffSize);
