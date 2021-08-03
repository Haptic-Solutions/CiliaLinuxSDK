#include "TcpListener.h"

TCPListener::TCPListener()
{

}

int TCPListener::OpenServer()
{
    serverOptions = 1;
    serverPort = 1995;
    serverAddressLength = sizeof(serverAddress);
    /*Attempt to get server int if we fail return -1*/
    if((serverX = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        return -1;
    /*Attempt to setup socket options. If we fail return -2*/
    if(setsockopt(serverX, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &serverOptions, sizeof(serverOptions)))
        return -2;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(serverPort);
    /*Attempt to bind server. If we fail return -3*/
    if(bind(serverX, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        return -3;
    /**/
    if(listen(serverX, 3) < 0)
        return-4;
    /*Accept incoming connection. If we fail return -5*/
    if((socketX = accept(serverX, (sockaddr *)&serverAddress, (socklen_t*)&serverAddressLength))<0)
        return -5;
    /*Connection established successfully return 0*/
    return 0;
}

int TCPListener::ReOpenServer()
{
    /*Accept incomming connection. If we fail return -5*/
    if((socketX = accept(serverX, (sockaddr *)&serverAddress, (socklen_t*)&serverAddressLength))<0)
        return -5;
    /*Connection established successfully return 0*/
        return 0;
}
int TCPListener::CloserServer()
{
    close(socketX);
    return 0;
}

int TCPListener::ServerReceiveMessage(std::string * stringToReceive)
{
    int INPUTBUFFERMAX = INPUTBUFFERMAX;
    //buffer
    char bufferToReceive[INPUTBUFFERMAX] = {0};
    //current pointer in buffer
    int bufferPointer = 0;
    //remaining buffer to read
    int bufferRemaining = INPUTBUFFERMAX;
    //how much buffer has been read
    int bufferRead = 1;
    
    while((bufferRead > 0) && (bufferRemaining > 0) && (bufferToReceive[bufferPointer-1] != '\n'))
    {
        bufferRead = read(socketX, &bufferToReceive[bufferPointer], 1);
        bufferPointer += bufferRead;
        bufferRemaining -= bufferRead;
    }
    bufferToReceive[bufferPointer++] = '\0';
    stringToReceive->assign(bufferToReceive, bufferPointer);
    return 0;
}