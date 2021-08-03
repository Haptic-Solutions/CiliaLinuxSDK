#ifndef COMSENDER_H
#define COMSENDER_H
#include <string>
#include <list>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <unistd.h>

#include <string.h>
#include "SerialPort.h"
#include "GVS.h"

namespace CiliaSDK
{
	class COMSender
	{
	private:
		/*Static Variable*/
		static COMSender* sInstance;
		/*Member Variables*/
		SerialPort mCOMX[GVS::MAX_NUMBER_OF_CILIAS];
		std::mutex mToken[GVS::MAX_NUMBER_OF_CILIAS];
	public:
		/**
		 * Returns a singleton of COMSender.
		 * Creates COMSender if it doesn't already exist.
		 * @return COMSender instance.
		 */
		static COMSender* GetComSender()
		{
			if (!sInstance)
			{
				sInstance = new COMSender();
			}
			return sInstance;
		}
		/**
		 * Constructor of COMSender.
		 * Creates all the new serial ports and tokens.
		 */
		COMSender()
		{
			for (int cCI = 0; cCI < GVS::MAX_NUMBER_OF_CILIAS; cCI++)
			{
				/*Initialize to a list of new Serial Ports for communication with Cilias*/
				mCOMX[cCI] = SerialPort();
				/*Initialize a mutex for each Cilia to make sure only one thread tries to talk with
				  a Cilia at a time*/
				//mToken[cCI];
			}
		}

		/**
		 * Writes a string to a COM(Serial) Port.
		 * Waits until mutex for a comport index of COMXi is free and
		 * claims it using WaitOne. Then if the comport is open
		 * writes the Input string message to the com port.
		 * Finally uses ReleaseMutex function to release the comport.
		 * @param COMXi index of COM(Serial) port to send a message to.
		 * @param InputString to send to the COM(Serial) port.
		 */
		void WriteString(int COMXi, std::string InputString)
		{
            //buffer

            char bufferToSend[InputString.size()];
            strcpy(bufferToSend, InputString.c_str());
            //current pointer in buffer
            int bufferPointer = 0;
            //remaining buffer to send
            int bufferSize = InputString.size();
            //how much buffer has been written
            int bufferWritten = 0;

            
            return;
			try
			{
                mToken[COMXi].lock();
				if (mCOMX[COMXi].IsOpen())
                    mCOMX[COMXi].SendMessage(InputString);
                mToken[COMXi].unlock();
			}
			catch (error_t e)
			{

			}
            return;
		}
		/**
		 * Returns a list of all serial port names.
		 */
		std::list<std::string> GetListOfPorts()
		{
			return SerialPort::GetPortNames();
		}
		/**
		 * Sets up a Serial port.
		 * Sets up a Serial port and inquires if it is a cilia by sending a C character. Returns the response of the inquiry.
		 * @param aComInt to try to setup.
		 * @return Response to inquiry.
		 * @throws exception if cannot connect to port or fails to read/write to port.
		 */
		std::string SetupComport(int aComInt)
		{
			mCOMX[aComInt].SetComPortName("USB" + aComInt);
			//mCOMX[aComInt].BaudRate = 19200;
			//mCOMX[aComInt].ReadTimeout = 10;
			//mCOMX[aComInt].WriteTimeout = 10;
			mCOMX[aComInt].OpenComPort();
			//mCOMX[aComInt].DiscardInBuffer();
			//mCOMX[aComInt].DiscardOutBuffer();
			mCOMX[aComInt].SendMessage("C");
			sleep(20);
            std::string *returnString = new std::string();
            mCOMX[aComInt].ReceiveMessage(returnString);
			return *returnString;
		}
		/**
		 * Closes a serial port.
		 * @param aComInt port to close.
		 */
		void ClosePort(int aComInt)
		{
			try
			{
				if (mCOMX[aComInt].IsOpen())
					mCOMX[aComInt].CloseComPort();
			}
			catch(error_t e)
			{
				//was open but failed to close
			}
		}

		/**
		 * Get integer value of COM Port from string name.
		 * removes COM from the name and casts to an int.
		 * @param comName string
		 * @return int value coresponding to the come name.
		 */
		int GetComInt(std::string comName)
		{
			try
			{
                size_t start = comName.find("USB");
                if(start == std::string::npos)
                {
                    return -1;
                }
				return atoi(comName.replace(start,comName.length(),"").c_str());
			}
			catch(error_t e)
			{
				return -1;
			}
		}
		/**
		 * Returns if a serial port is open.
		 * @param aComInt index to check.
		 * @return bool value of whether the por is open.
		 */
		bool IsComOpen(int aComInt)
		{
			return mCOMX[aComInt].IsOpen();
		}
	};
}
#endif // !COMSENDER_H