#include "Cilia.h"

namespace CiliaSDK
{
	std::list<unsigned char> Cilia::sCiliaGroups[GVS::NUMBER_OF_SURROUND_POSITIONS];
	std::list<unsigned char> Cilia::sActiveCilias;
	RGB::RGB(const std::string aRGBString) : red(0), green(0), blue(0)
	{
		std::stringstream redStream;
		std::stringstream greenStream;
		std::stringstream blueStream;

		if (redStream << aRGBString.substr(0, 3))
		{
			redStream >> red;
		}
		if (greenStream << aRGBString.substr(3, 3))
		{
			greenStream >> green;
		}
		if (blueStream << aRGBString.substr(6, 3))
		{
			blueStream >> blue;
		}
	}

	RGB::RGB(unsigned char aRed, unsigned char aBlue, unsigned char aGreen)
	{
		red = aRed;
		green = aGreen;
		blue = aBlue;
	}

	std::string RGB::ToString()
	{
		std::stringstream redStream;
		std::stringstream greenStream;
		std::stringstream blueStream;
		redStream << std::setw(3) << std::setfill('0') << red;
		greenStream << std::setw(3) << std::setfill('0') << green;
		blueStream << std::setw(3) << std::setfill('0') << blue;
		std::string iString = redStream.str() + greenStream.str() + blueStream.str();
		return iString;
	}

	Cilia::Cilia(unsigned char aCilia, std::string aInitializationValues[])
	{
		//store serial port
		mSerialPort = aCilia;
		//store surround position
		std::stringstream initializationValue;
		initializationValue << aInitializationValues[0];
		initializationValue >> mSurroundPosition;
		//store smells and lights
		int i = 0;
		for (auto smells : mSmells)
		{
			mSmells[i] = aInitializationValues[i + GVS::SMELLS_OFFSET];
			mLights[i] = RGB(aInitializationValues[i + GVS::LIGHT_OFFSET]);
			i++;
		}
	}

	std::string Cilia::GetLightValue(int aLightIndex)
	{
		return mLights[aLightIndex].ToString();
	}

	void Cilia::SetLightValue(int aLightIndex, std::string aLightValue)
	{
		mLights[aLightIndex] = RGB(aLightValue);
	}

	void Cilia::prepareGroups()
	{
		for (int cPI = 0; cPI < GVS::NUMBER_OF_SURROUND_POSITIONS; cPI++)
			sCiliaGroups[cPI] = std::list<unsigned char>();
	}

	void Cilia::SetCiliaPosition(unsigned char aGroup)
	{
		//if possible remove from previous group.
		if
			(
				Find(sCiliaGroups[mSurroundPosition], mSerialPort)
				)
		{
			sCiliaGroups[mSurroundPosition].remove(mSerialPort);
		}
		//assigne new group
		mSurroundPosition = aGroup;
		//add to group list and sortS
		sCiliaGroups[aGroup].emplace_back(mSerialPort);
		sCiliaGroups[aGroup].sort();
		//add as an active Cilia
		if
			(
				!Find(sActiveCilias, mSerialPort)
				)
		{
			sActiveCilias.emplace_back(mSerialPort);
		}
	}

	void Cilia::RemoveCiliaFromGroup(unsigned char aGroup, unsigned char aCilia)
	{
		if (Find(sCiliaGroups[aGroup], aCilia))
		{
			sCiliaGroups[aGroup].remove(aCilia);
		}
		if (Find(sActiveCilias, aCilia))
		{
			sActiveCilias.remove(aCilia);
		}
	}

	int Cilia::GetCountOfGroup(unsigned char aGroup)
	{
		if (!sCiliaGroups[aGroup].empty())
			return sCiliaGroups[aGroup].size();
		else
			return 0;
	}

	unsigned char Cilia::GetCiliaInPositions(unsigned char aGroup, unsigned char aIndex)
	{
		auto ciliaGroupsItterator = sCiliaGroups[aGroup].begin();
		std::advance(ciliaGroupsItterator, aIndex);
		return (unsigned char)*ciliaGroupsItterator;
	}

	unsigned char Cilia::GetCiliaPosition()
	{
		return mSurroundPosition;
	}

	std::list<unsigned char> Cilia::GetActiveCilias()
	{
		return sActiveCilias;
	}

	std::string Cilia::ToString()
	{
		std::string toReturn = mSerialPort + "," + std::to_string(mSurroundPosition);
		for (unsigned int i = 0; i < mSmells.size(); i++)
		{
			toReturn += "," + mSmells[i];
		}
		for (unsigned int i = 0; i < mLights.size(); i++)
		{
			toReturn += "," + mLights[i].ToString();
		}
		return toReturn;
	}

	std::string Cilia::ToStringForWritingToFile()
	{
		std::string toReturn = std::to_string(mSurroundPosition);
		for (auto smell : mSmells)
		{
			toReturn += "\n" + smell;
		}
		for (auto light : mLights)
		{
			toReturn += "\n" + light.ToString();
		}
		return toReturn;
	}

	std::list<int> Cilia::GetSmellIndexes(std::string aSmellValue)
	{
		std::list<int> smellIndexes;
		int i = 0;
		for (auto smell : mSmells)
		{
			if (smell.compare(aSmellValue))
			{
				smellIndexes.emplace_back(i + 1);
			}
			i++;
		}
		return smellIndexes;
	}

	void Cilia::SetSmell(int aSmellIndex, std::string aSmellValue)
	{
		mSmells[aSmellIndex] = aSmellValue;
	}
}