#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


#include "logging.h"

#define PORT 9009

int connectServer(char* host,int port){
    int clientSocket ;
    int connectResult;
    struct sockaddr_in serverAddr ;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(host);
    serverAddr.sin_port = htons(port);

    clientSocket = socket(AF_INET,SOCK_STREAM,0);

    Logging("debug","Create Client Socket...");
    connectResult = connect(clientSocket,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
    if( connectResult != 0 ) {
        Logging("error","connnect failed! %s ","");
        return -1;
    }
    return clientSocket;
}

void loopClientSocket(int clientSocket){
    char sendBuffer[1024];
    char recvBuffer[1024];
    int recvStatus,sendStatus;
    send(clientSocket,"-",1,0);
    while(1){
        bzero(recvBuffer,1024);
        bzero(sendBuffer,1024);
        recvStatus = recv(clientSocket,recvBuffer,1024,0);
        Logging("debug","get message from server: %s ",recvBuffer);
        if(strncmp("bye",recvBuffer,3) == 0 ){
            break;
        }
        if(recvStatus == 0) {
            break;
        }
        printf("Write Some:>");
        scanf("%s",sendBuffer);
        send(clientSocket,sendBuffer,strlen(sendBuffer),0);
    }
}

int main(){
    int clientSocket = connectServer("127.0.0.1",PORT); 
    if(clientSocket != -1){
        loopClientSocket(clientSocket);
    }
    return 0;
}