#ifndef CILIAMANAGER_H
#include "Cilia.h"
#include "ComSender.h"
namespace CiliaSDK
{
	class CiliaManager
	{
		/*Static Class Variables*/
		static CiliaManager* sInstance;
		/*Member Variables*/
		Cilia mCilias[GVS::MAX_NUMBER_OF_CILIAS];
		COMSender* mCOMSender;
		std::list<std::string> mPorts;

		/**
		 * Returns the singleton of CiliaManager.
		 * @param aCOMSender for use in sending serial messages to Cilias.
		 */
		static CiliaManager* GetCiliaManager(COMSender* aCOMSender)
		{
			if (!sInstance)
			{
				sInstance = new CiliaManager(aCOMSender);
			}
			return sInstance;
		}

		/**
		 * Constructor creates a Cilia Manager.
		 * @param aCOMSender for use in sending serial messages to Cilias.
		 */
		CiliaManager(COMSender *aCOMSender)
		{
			mCOMSender = aCOMSender;
		}
		/**
		 * Callback for when Cilia connected by USB
		 * Is used to handle adding a cilia dynamically.
		 * @param aSender not used
		 * @param args not used
		 */
		/*void CiliaAttachedEvent(object sender, EventArrivedEventArgs args)
		{
			std::list<std::string> ports;
			ports = new std::list<std::string>(SerialPort.GetPortNames());

			std::list<std::string> addedPorts = ports.Except(mPorts).ToList();
			CiliasAttach(addedPorts);
		}*/
		/**
		 * Attaches a group of Cilias by port names.
		 * @param portsToAdd list of port names we want to add.
		 */
		void CiliasAttach(std::list<std::string> portsToAdd)
		{
			for(std::string port : portsToAdd)
			{
				CiliaAttach(port);
			}
		}
		/**
		 * Attaches a Cilia by port name.
		 * @param aPortToAdd is the string name of the port we want to add.
		 */
		void CiliaAttach(std::string aPortToAdd)
		{
			int ComInt = 0;
			std::string readString = "";

			if (aPortToAdd.find("FC") || !aPortToAdd.find("COM"))
				return;
			try
			{
				ComInt = COMSender::GetComSender()->GetComInt(aPortToAdd);
				if (ComInt == -1)
					return;
				readString = COMSender::GetComSender()->SetupComport(ComInt);
				if (readString.find("CILIA"))
				{
					SetCiliaToDefaultState(ComInt);
					mCilias[ComInt].SetCiliaPosition(mCilias[ComInt].GetCiliaPosition());
					//Cilia.AddNewCiliaToPositions(mCilias[ComInt].GetCiliaPosition(),(byte)ComInt);
					//CiliaPipeSender.SendMessageByPipeToCP("AddCilia," + mCilias[ComInt].ToString());
					mPorts.emplace_back(aPortToAdd);
				}
				else
				{
					COMSender::GetComSender()->ClosePort(ComInt);
				}
			}
			catch(error_t e)
			{
				COMSender::GetComSender()->ClosePort(ComInt);
			}

		}
		/**
		 * Sets a Cilia to its default state.
		 * This involves setting its lighting to its default lighting configuration and its fans to the off state.
		 * @param aCilia to set to the default state
		 */
		void SetCiliaToDefaultState(int aCilia)
		{
			COMSender::GetComSender()->WriteString(aCilia, "N1" + mCilias[aCilia].GetLightValue(0));
			COMSender::GetComSender()->WriteString(aCilia, "N2" + mCilias[aCilia].GetLightValue(1));
			COMSender::GetComSender()->WriteString(aCilia, "N3" + mCilias[aCilia].GetLightValue(2));
			COMSender::GetComSender()->WriteString(aCilia, "N4" + mCilias[aCilia].GetLightValue(3));
			COMSender::GetComSender()->WriteString(aCilia, "N5" + mCilias[aCilia].GetLightValue(4));
			COMSender::GetComSender()->WriteString(aCilia, "N6" + mCilias[aCilia].GetLightValue(5));
			COMSender::GetComSender()->WriteString(aCilia, "F1000");
			COMSender::GetComSender()->WriteString(aCilia, "F2000");
			COMSender::GetComSender()->WriteString(aCilia, "F3000");
			COMSender::GetComSender()->WriteString(aCilia, "F4000");
			COMSender::GetComSender()->WriteString(aCilia, "F5000");
			COMSender::GetComSender()->WriteString(aCilia, "F6000");
		}

		/**
		 * Callback for when Cilia disconnected by USB
		 * Is used to handle removing a cilia dynamically.
		 * @param aSender not used
		 * @param args not used
		 */

		/*void CiliaDetachedEvent(object aSender, EventArrivedEventArgs aArgs)
		{
			std::list<std::string> ports;
			ports = std::list<std::string>(SerialPort.GetPortNames());

			std::list<std::string> removedPorts = mPorts.Except(ports).ToList();
			CiliasDetach(removedPorts);
		}*/
		/**
		 * Removes a group of Cilias by port names.
		 * @param aPortsToRemove list of port names we want to remove.
		 */
		void CiliasDetach(std::list<std::string> aPortsToRemove)
		{
			for(std::string port : aPortsToRemove)
			{
				CiliaDetach(port);
			}
		}
		/**
		* Removes a Cilia by port name.
		* @param aPortToRemove is the string name of the port we want to remove.
		*/
		void CiliaDetach(std::string aPortToRemove)
		{
			mPorts.remove(aPortToRemove);
			int ComInt = mCOMSender->GetComInt(aPortToRemove);
			Cilia::RemoveCiliaFromGroup(mCilias[ComInt].GetCiliaPosition(), (unsigned char)ComInt);
			mCOMSender->ClosePort(ComInt);
			//ForceUpdate();
			//CiliaPipeSender.SendMessageByPipeToCP("RemoveCilia," + ComInt);
		}

		/**
		* Returns index of Smell for a particular Cilia given a aSmell name
		* @param Cilia we want to get the aSmell index for.
		* @param aSmell we want the index of.
		* @return int index of aSmell
		*/
		std::list<int> ReturnSmellNumbers(int Cilia, std::string aSmell)
		{
			return mCilias[Cilia].GetSmellIndexes(aSmell);
		}
		/**
		 * Get default light value of a specified light on a specified cilia.
		 * @param aCilia we want to get the default light value on.
		 * @param aLight we want to get the default light value of.
		 */
		std::string  GetLightValue(int aCilia, int aLight)
		{
			return mCilias[aCilia].GetLightValue(aLight);
		}
		/**
		 * Set a cilia lights devault value.
		 * @param aCilia we want to set the default value on.
		 * @param aLight we want to set the default value of.
		 * @param aValue default value that we want to set.
		 */
		void SetLightValue(int aCilia, int aLight, std::string aValue)
		{
			mCilias[aCilia].SetLightValue(aLight, aValue);
		}
		/**
		 * Sets a specific cilias aSmell slot to a aSmell.
		 * @param aCilia we want to set the aSmell on
		 * @param aIndex of the aSmell slot we want to set.
		 * @param aValue of the aSmell as a string.
		 */
		void SetSmellValue(int aCilia, int aIndex, std::string aValue)
		{
			mCilias[aCilia].SetSmell(aIndex, aValue);
		}
		/**
		 * Gets a comma separated value string representation of a cilia.
		 * @param aCilia to get the to string for.
		 * @return string value representation of a Cilia including Serial port, Group, Smells, and Default Lighting.
		 */
		std::string GetCiliaToString(int aCilia)
		{
			return mCilias[aCilia].ToString();
		}
		/**
		 * Gets the proper ToString value fo a specified Cilia in the format for writing to a save file.
		 * @param aCilia.
		 * @return string for saving to a save file.
		 */
		std::string GetCiliaToStringForWritingToFile(int aCilia)
		{
			return mCilias[aCilia].ToStringForWritingToFile();
		}
		/**
		 * Get the group of a specified Cilia and return it.
		 * @param aCilia to get the group of.
		 * @return byte value of the group the Cilia is in.
		 */
		unsigned char GetCiliaPosition(int aCilia)
		{
			return mCilias[aCilia].GetCiliaPosition();
		}
		/**
		 * Sets the group of a specified Cilia to a specified group.
		 * @param aCilia to set the group of.
		 * @param aGroup to set the Cilia to.
		 */
		void SetGroup(unsigned char aCilia, unsigned char aGroup)
		{
			mCilias[aCilia].SetCiliaPosition(aGroup);
		}
		/**
		 * Overrides an old Cilia with a new Cilia initialized using input parameters.
		 * @param aCilia to override.
		 * @param ANewCilia information to use in initializing new Cilia to override old Cilia.
		 */
		void OverrideCilia(unsigned char aCilia, std::string aNewCilia[])
		{
			mCilias[aCilia] = Cilia(aCilia, aNewCilia);
		}
	};
}
#define CILIAMANAGER_H
#endif // !CILIAMANAGER_H