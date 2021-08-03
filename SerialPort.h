#ifndef SerialPort_H
#define SerialPort_H
/*Include Libraries*/
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include "iostream"
#include "string.h"
#include "list"
#include <string>


/*
*  Class for talking with Cilia Devices over serial port
*  @author Peter Sassaman
*  @date last upated 1/6/19
*/
/*
*	Refferences
*	https://stackoverflow.com/questions/18108932/linux-c-serial-port-reading-writing
*/
class SerialPort
{
private:
	/*Private variables*/
	std::string COMPortName;
    static std::list<std::string> COMPortNames;
	termios termiosInstance;
	termios termiosCopy;
	int COMX;
	const static int INPUTBUFFERMAX = 1024;
    bool isOpen;
public:
	/**
	* Default Constructor
	*/
	SerialPort();
	/**
	* Set the name of the com port
	* @param name to be set
	*/
	void SetComPortName(std::string COMPortNameString);
	/**
	* Returns the name of the com port as a string
	* @return name of com port
	*/
	std::string GetComPortName();
    static std::list<std::string> GetPortNames(){return COMPortNames;}
    bool IsOpen(){return isOpen;}//TODO add check to make sure really open
	/**
	* Opens COM Port
	* @return -1 if failure or number of the com port
	*/
	int OpenComPort();
	/**
	* Closes COM Port
	* @return -1 if failure or 0 if successful
	*/
	int CloseComPort();
	/**
	* Sends a string message of the serial port to the Cilia
	* @param stringToSend to the Cilia
	* @return 0
	* Need to check for error  states???
	*/
	int SendMessage(std::string stringToSend);
	/**
	* Receives a string message from the Cilia over the serial port
	* @param pointer to the string that we will be receiving over the serial port
	* @return 0
	* Need to check for error states???
	*/
	int ReceiveMessage(std::string *stringToReceive);
};
#endif