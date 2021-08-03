#ifndef TCPLISTENER_H
#define TCPLISTENER_H

/*
* Include Files
*/
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string>

/*
* Server Class For Raspberry Pi To receive commands from either an application on the Raspberry Pi or an
* application on another computer which can be then sent to the Cilia over serial. Currently Single threaded
* as the Cilias are a single resource and we only want one application controlling them at a time.
* @author Peter Sassaman
* @date laste updated 1/6/19
*/
class TCPListener
{
    private:
    int serverX;
    int socketX;
    int serverOptions;
    int serverPort;
    sockaddr_in serverAddress;
    int serverAddressLength;
    public:
    /**
     * Default Constructor
     */
    TCPListener();
    /**
     * Opens Server.
     * @return 0 if successful
     *          -1 if failed to get server int
     *          -2 if we failed to setup socket options
     *          -3 if we failed to bind otherwise returns error code.
     *          -4 if listen failed
     *          -5 if accept connection failed
     */
    int OpenServer();
    int ReOpenServer();
    /**
     * Closes Server.
     * May need more work to properly close socket.
     * @return 0.
     */
    int CloserServer();
    /**
     * Receives message over TCP/IP socket from either an application on the PI or from an application on
     * another computer.
     * @param stringToRecieve pointer to string buffer.
     * @return 0
     * May want to add more error checking.
     */
    int ServerReceiveMessage(std::string * stringToReceive);
};

#endif