#include "SerialPort.h"

std::list<std::string> SerialPort::COMPortNames;

SerialPort::SerialPort(): isOpen(false)
{
}

void SerialPort::SetComPortName(std::string COMPortNameString)
{
    COMPortName = COMPortNameString;
}

std::string SerialPort::GetComPortName()
{
    return COMPortName;
}

int SerialPort::OpenComPort()
{
    COMPortName.append(COMPortName);
    COMX = open(COMPortName.c_str(), O_RDWR | O_NOCTTY);

    memset(&termiosInstance, 0, sizeof termiosInstance);

    if (tcgetattr(COMX, &termiosInstance) != 0)
    {
        printf("Error: getting attributes\n");
        return -1;
    }

    termiosCopy = termiosInstance;

    cfsetospeed(&termiosInstance, (speed_t)B19200);
    cfsetispeed(&termiosInstance, (speed_t)B19200);

    termiosInstance.c_cflag &= ~PARENB;
    termiosInstance.c_cflag &= ~CSTOPB;
    termiosInstance.c_cflag &= ~CSIZE;
    termiosInstance.c_cflag |= CS8;

    termiosInstance.c_cflag &= ~CRTSCTS;
    termiosInstance.c_cc[VMIN] = 1;
    termiosInstance.c_cc[VTIME] = 5;
    termiosInstance.c_cflag |= CREAD | CLOCAL;

    cfmakeraw(&termiosInstance);

    tcflush(COMX, TCIFLUSH);

    if (tcsetattr(COMX, TCSANOW, &termiosInstance) != 0)
    {
        printf("error: setting attributes\n");
        termiosInstance = termiosCopy;
        cfmakeraw(&termiosInstance);
        tcflush(COMX, TCIFLUSH);
        if (tcsetattr(COMX, TCSANOW, &termiosInstance) != 0)
        {
            printf("error: returning to old attributes\n");
            return -1;
        }
        return -1;
    }
    isOpen = true;
    return COMX;
}

int SerialPort::CloseComPort()
{
    COMPortNames.remove(COMPortName);
    termiosInstance = termiosCopy;
    cfmakeraw(&termiosInstance);
    tcflush(COMX, TCIFLUSH);
    if (tcsetattr(COMX, TCSANOW, &termiosInstance) != 0)
    {
        printf("error: returning to old attributes\n");
        return -1;
    }
    else
    {
        isOpen = false;
        return 0;
    }
}

int SerialPort::SendMessage(std::string stringToSend)
{
    //buffer

    char bufferToSend[stringToSend.size()];
    strcpy(bufferToSend, stringToSend.c_str());
    //current pointer in buffer
    int bufferPointer = 0;
    //remaining buffer to send
    int bufferSize = stringToSend.size();
    //how much buffer has been written
    int bufferWritten = 0;

    while (bufferSize > 0)
    {
        bufferWritten = write(COMX, &bufferToSend[bufferPointer], bufferSize);
        bufferPointer += bufferWritten;
        bufferSize -= bufferWritten;
    }
    return 0;
}

int SerialPort::ReceiveMessage(std::string *stringToReceive)
{
    //buffer
    char bufferToReceive[INPUTBUFFERMAX] = {0};
    //current pointer in buffer
    int bufferPointer = 0;
    //remaining buffer to read
    int bufferRemaining = INPUTBUFFERMAX;
    //how much buffer has been read
    int bufferRead = 1;

    while ((bufferRead > 0) && (bufferRemaining > 0) && (bufferToReceive[bufferPointer - 1] != '\n'))
    {
        bufferRead = read(COMX, &bufferToReceive[bufferPointer], 1);
        bufferPointer += bufferRead;
        bufferRemaining -= bufferRead;
    }
    bufferToReceive[bufferPointer++] = '\0';
    stringToReceive->assign(bufferToReceive, bufferPointer);
    return 0;
}