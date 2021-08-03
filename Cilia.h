#ifndef CILIA_H
#define CILIA_H
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <string.h>
#include <list>
#include <array>
#include <iostream>
#include "GVS.h"

namespace CiliaSDK
{
	template<class Group, class KeyToFind>
	bool Find(Group aList, KeyToFind aKey)
	{
		if
			(
				std::find
				(
					aList.begin(),
					aList.end(),
					aKey
				) != aList.end()
				)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	struct RGB
	{
	public:
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		RGB() {};
		/**
		 * Initializes red green blue values from a string containing the values.
		 * Each three characters are a value.
		 * @param aRGBString containing the RGB values.
		 */
		RGB(const std::string aRGBString);
		/**
		 * Initilizes red green and blue with byte values.
		 * @param aRed byte value.
		 * @param aBlue byte value.
		 * @param aGreen byte value.
		 */
		RGB(unsigned char aRed, unsigned char aBlue, unsigned char aGreen);
		/**
		 * Overrides default ToString function for RGB.
		 * @return string with the red green and blue ToString values formatted such that each value has 3 decimal places.
		 */
		std::string ToString();
	};


	class Cilia
	{
	private:
		/*Member Variables*/
		//Serial port is used to index Cilia.
		unsigned char mSerialPort;
		unsigned char mSurroundPosition = 0;
		std::array<std::string, 6> mSmells;
		std::array<RGB, 6> mLights;
		/*Static Variables*/
		static std::list<unsigned char>sCiliaGroups[GVS::NUMBER_OF_SURROUND_POSITIONS];
		static std::list<unsigned char>sActiveCilias;
	public:
		/**
		 * Default Constructor
		 */
		 /*Cilia()
		 {
		 }*/
		 /**
		  * Constructor sets up values including: mSerialPort, mSurroundPosition, mSmells, mLights.
		  * @param aCilia that this cilia is attached to.
		  * @param aInitializationValues string array containing initialization values.
		  */
		Cilia(unsigned char aCilia, std::string aInitializationValues[]);
        Cilia();
		/**
		 * Returns light value for a light at a specific index as a string
		 * @return string with formatted light value RRRGGGBBB
		 */
		std::string GetLightValue(int aLightIndex);
		/**
		 * Sets a light value at a specified index to a specified value.
		 * @param aLightIndex index of which light is being set.
		 * @param aValue string value the light is being set to.
		 */
		void SetLightValue(int aLightIndex, std::string aLightValue);
		/**
		 * Sets the lists in mCiliaGroups to new byte lists.
		 */
		static void prepareGroups();

		/**
		 * Sets what group a Cilia is in.
		 * Removes the Cilia from any former groups it was in, stores its new group, places it in its new group, and sorts the group.
		 * Also, makes sure the cilia is in the list of active cilias
		 */
		void SetCiliaPosition(unsigned char aGroup);
		/**
		 * Remove Cilia from group and active cilias.
		 * @param aGroup to remove the Cilia from.
		 * @param aCilia to be removed.
		 */
		static void RemoveCiliaFromGroup(unsigned char aGroup, unsigned char aCilia);
		/**
		 * Returns how many cilias are in a group.
		 * @param aGroup to search.
		 * @return int value of how many cilias are in a group.
		 */
		static int GetCountOfGroup(unsigned char aGroup);
		/**
		 * Returns which cilia is in a group at a specified index
		 * @param aGroup to search.
		 * @param aSmellIndex to return the value of.
		 * @return byte value of what cilia was found.
		 */
		static unsigned char GetCiliaInPositions(unsigned char aGroup, unsigned char aIndex);
		/**
		 * Returns the surround position of a Cilia
		 * @return byte value of what surround position a Cilia is in.
		 */
		unsigned char GetCiliaPosition();

		/**
		 * Returns a list of active Cilias.
		 * @return byte list of active Cilias
		 */
		static std::list<unsigned char> GetActiveCilias();
		/**
		 * Overrides the ToString method for Cilia Class.
		 * @returns a comma separated version of the contents of the Cilia including serial port, surround position, smells, and light settings.
		 */
		std::string ToString();

		/**
		 * An alternate ToString method meant for writing to file.
		 * @returns a new line separated version of the contents of the Cilia including surround position, smells, and light settings.
		 */
		std::string ToStringForWritingToFile();
		/**
		 * Returns a list of the indexes a smell is found in.
		 * Searches mSmells for what indexes contain a smell and returns the indexes as an int list.
		 * @param aSmellValue we want the inexes of.
		 * @return int list of what indexes a smell is stored in.
		 **/
		std::list<int> GetSmellIndexes(std::string aSmellValue);
		/**
		 * Sets a smell at a specified index to a smell
		 * @param aSmellIndex we want to store the smell at.
		 * @param aSmellValue string we want to store.
		 */
		void SetSmell(int aSmellIndex, std::string aSmellValue);
	};
}
#endif // !CILIA_H