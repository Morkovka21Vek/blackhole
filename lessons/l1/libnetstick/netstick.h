int init_sock(int* sockfd, int port, const char* serv_url);
int send_req(const int sockfd, const char *get_req, char* recvBuff, const int recvBuffSize);
