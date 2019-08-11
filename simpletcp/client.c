#include "simpletcp.h"

//创建连接到服务器端的socket
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

//循环处理客户端的socket
void loopClientSocket(int clientSocket){
    char sendBuffer[BUFFER_LEN];
    char recvBuffer[BUFFER_LEN];
    int recvStatus,sendStatus;
    send(clientSocket,"-",1,0);
    while(1){
        bzero(recvBuffer,BUFFER_LEN);
        bzero(sendBuffer,BUFFER_LEN);
        recvStatus = recv(clientSocket,recvBuffer,BUFFER_LEN,0);
        Logging("debug","get message from server: %s ",recvBuffer);
        if(strncmp("bye",recvBuffer,3) == 0 ){
            break;
        }
        if(recvStatus == 0) {
            break;
        }
        printf("Write Some:>");
        scanf("%[^\n]%*c",sendBuffer);
        // 输入内容，遇到回车键结束，并且舍弃最后的回车键。
        sendBuffer[strlen(sendBuffer)] = '\0';
        Logging("debug","You input > %s",sendBuffer);
        send(clientSocket,sendBuffer,strlen(sendBuffer),0);
    }
}

int main(int argc,char** argv){
    int clientSocket = connectServer("127.0.0.1",PORT); 
    if(clientSocket != -1){
        loopClientSocket(clientSocket);
    }
    return 0;
}