#include "simpletcp.h"

//创建服务器端的socket
int createServerSocket(char* ip,int port){
    struct sockaddr_in name;
    int sock;
    Logging("debug","geek network world start ");
    // 初始化socket, 
    // AF_INET | AF_INET6 
    // SOCK_STREAM | SOCK_DGRAM
    sock = socket(AF_INET,SOCK_STREAM,0);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    if(sock < 0 ){
        Logging("error","socket create failed");
        exit(1);
    }

    name.sin_family = AF_INET;
    name.sin_port = htons(port);

    //name.sin_addr.s_addr = htonl(INADDR_ANY);
    name.sin_addr.s_addr = inet_addr(ip);

    if(bind(sock,(struct sockaddr *)&name,sizeof(name)) < 0 ){
        Logging("error","bind error !");
        exit(1);
    }

    return sock;
}

// 服务器端一定要先接收数据
void loopClientSocket(int clientSocket,struct sockaddr_in clientAddr){
    char sendBuffer[BUFFER_LEN] = {'\0'};
    char recvBuffer[BUFFER_LEN] = {'\0'};
    int recvResult ;
    int sendResult;
    uint8_t isEnding = 0;
    Logging("debug","one client come...");

    // strcpy(sendBuffer,"hello geeknetwork server : ");
    // send(clientSocket,sendBuffer,strlen(sendBuffer),0);
    while(isEnding != 1) {
        bzero(sendBuffer,BUFFER_LEN);
        bzero(recvBuffer,BUFFER_LEN);
        recvResult = recv(clientSocket,recvBuffer,1024,0);
        if(recvResult == -1){
            break;
        }
        Logging("debug","data get from client : %s , receive result : %d ",recvBuffer,recvResult);
        
        if(strncmp("quit",recvBuffer,4) == 0){
            isEnding = 1;
            strcpy(sendBuffer,"bye, welcome back.\n");
        } else {
            if(recvResult != 0 ) {
                strcpy(sendBuffer,recvBuffer);
            } else {
                strcpy(sendBuffer,"empty");
            }
        }
        sendResult = send(clientSocket,sendBuffer,strlen(sendBuffer),0);
        Logging("debug","data send to client  : %s , send result : %d ",sendBuffer,sendResult);
        if(isEnding){
            close(clientSocket);
        }
    }
}

int main(int argc,char** argv){
    int serverSocket = createServerSocket("0.0.0.0",PORT);
    int backLog = 12;
    int clientSocket;

    // struct sockaddr 叫做通用地址格式
    // 可以是 ipv4 套接字，ipv6 套接字，或者本地套接字 
    // sockaddr_in , sockaddr_in6, sockaddr_un
    struct sockaddr_in clientAddr;
    uint clientAddrLen;
    
    listen(serverSocket,backLog);
    while(1){
        clientSocket = accept(serverSocket,(struct sockaddr *)&clientAddr,&clientAddrLen);
        loopClientSocket(clientSocket,clientAddr);
    }
    return 0;
}
