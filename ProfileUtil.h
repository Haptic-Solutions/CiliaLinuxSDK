#include <string>
#include <string.h>
#include <list>
#include <iostream>
#include <fstream>

#include "CiliaManager.h"

namespace CiliaSDK
{
    class ProfileUtil
    {
        private:
        /*Member Variables*/
        std::string mCiliaDirectory;
        std::string mGameDirectory;
        std::string mGameFolder = "Default";
        std::string mSmellLibraryPath;
        const std::string mAppDataDirectory = "C:/Windows/ServiceProfiles/LocalService/AppData/Roaming";//path to Windows Services appdata
        const std::string mSmellLibraryName = "SmellLibrary.txt";
        std::list<std::string>* mSmellLibraryContentsConst = new std::list<std::string> { "Apple", "BahamaBreeze", "CleanCotton", "Leather", "Lemon", "Rose" };
        std::list<std::string>* mSmellLibraryContents = new std::list<std::string> { "Apple", "BahamaBreeze", "CleanCotton", "Leather", "Lemon", "Rose" };
        CiliaManager* mCiliaManager;
        NetworkStream* mStream;
        /*Static Class Variables*/
        static ProfileUtil* sInstance;
        public:
        /**
         * Returns singleton of ProfileUtil.
         * If the profile util instance doesn't exist create it and then return it.
         * @param aCiliaManager for initializing ProfileUtil
         * @param aNetworkStream for initializing ProfileUtil
         * @return ProfileUtil instance.
         */
        static ProfileUtil* GetProfileUtil(CiliaManager* aCiliaManager, NetworkStream* aNetworkStream)
        {
            if (!sInstance)
            {
                sInstance = new ProfileUtil(aCiliaManager, aNetworkStream);
            }
            return sInstance;
        }
        /**
         * Constructor for ProfileUtil.
         * Stores CiliaManager and NetworkStream. Then
         */
        ProfileUtil(CiliaManager *aCiliaManager, NetworkStream* aNetworkStream)
        {
            mCiliaManager = aCiliaManager;
            mStream = aNetworkStream;
            /*Set the Cilia Directory Path from appdata path and cilia folder*/
            mCiliaDirectory = mAppDataDirectory + "/Cilia";
            /*Set Smell Library Path to a combination of the Cilia Appdata Directory and the Smell Library Name*/
            mSmellLibraryPath = mCiliaDirectory + "/" + mSmellLibraryName;
            /*Set the current Game Folder name to the Default name*/
            mGameFolder = "Default";
            /*Set the current GameDirectory to a combination of the CiliaDirectory path and the game folder name*/
            mGameDirectory = mCiliaDirectory + "/" + mGameFolder;
        }
        /**
         * Updates the network stream.
         * @param aNetworkStream to update the mStream with.
         */
        void SetNetworkStream(NetworkStream* aNetworkStream)
        {
            mStream = aNetworkStream;
        }
        /**
         * Loads a game profile.
         * <pre>
         * Loads a game profile and optionally creates a new one if the payload contains information about the profile.
         * Load Profile
         * !#LoadProfile|[GameProfile]
         * Create Profile
         * !#LoadProfile|[GameProfile],[surroundgroup],[Smell 1],[Smell 2],[Smell 3],[Smell4],[Smell5],[Smell6],[light1],[light2],[light3],[light4],[light5],[light6],[Any Number of Surround Positions]
         * </pre>
         * @param aProcessString for loading or creating profile.
         */
        void LoadProfile(std::string aProcessString)
        {
            std::string ss = aProcessString.Split('|')[1];
            std::string sS[] = ss.Split(',');
            //sS[0] contains game folder sS may contain the rest of the info needed
            if (aProcessString.Split('|')[0].Contains("Force"))
                LoadGameProfile(sS[0], sS, true);
            else
                LoadGameProfile(sS[0], sS, false);
        }
        /**
         * Sends a list of the game profiles to the Control Panel.
         */
        void GetProfiles()
        {
            std::string directoryies[] = Directory.GetDirectories(mCiliaDirectory);

            std::string directoryListString = "[";
            for (int ai = 0; ai < directoryies.Length; ai++)
            {
                directoryListString += directoryies[ai].Substring(mCiliaDirectory.length() + 1, directoryies[ai].Length - (mCiliaDirectory.length() + 1)) + ",";
            }
            directoryListString += mGameFolder;
            directoryListString += "]\n";
            unsigned char directoryBytes[] = System.Text.Encoding.UTF8.GetBytes(directoryListString);
            mStream.Write(directoryBytes, 0, directoryBytes.Length);
        }
        /**
         * Deletes all game profiles and recreates the default profile.
         */
        void FactoryReset()
        {
            //System.IO.DirectoryInfo ciliaDirectoryInfo = new DirectoryInfo(CiliaDirectory);
            DeleteFolder(mCiliaDirectory);
            CreateDefaultGameProfile();
            std::string sS[] = { };//no parameters since loading an existing profile.
            LoadGameProfile("Default", sS, false);
            unsigned char completeRefresh[] = { 99 };
            mStream.Write(completeRefresh, 0, 1);
            int i = 0;
        }
        /**
         * Deletes the currently selected game profile and if it is the default profile recreate it.
         */
        void DeleteProfile()
        {
            DeleteFolder(mGameDirectory);
            if (mGameFolder.compare("Default")==0)
            {
                CreateDefaultGameProfile();
            }
            std::string sS[] = { };//no parameters since loading an existing profile.
            LoadGameProfile("Default", sS, false);
            unsigned char completeRefresh[] = { 99 };
            mStream.Write(completeRefresh, 0, 1);
        }
        /**
         * Creates the default game folder if it doesn't exist.
         */
        void CreateDefaultGameProfile()
        {
            std::string comName = "";
            std::string ciliaContentsTemplate = "0\nLemon\nApple\nLeather\nCleanCotton\nBahamaBreeze\nRose\n000128064\n000128064\n000128064\n000128064\n000128064\n000128064";
            std::string surroundGroups = "FrontCenter,FrontLeft,SideLeft,RearLeft,RearCenter,RearRight,SideRight,FrontRight";

            if (!Directory.Exists(mCiliaDirectory))
            {
                Directory.CreateDirectory(mCiliaDirectory);
            }
            if (!Directory.Exists(mGameDirectory))
            {
                Directory.CreateDirectory(mGameDirectory);
            }

            System.IO.File.WriteAllText(Path.Combine(mGameDirectory, "SurroundGroups"), surroundGroups);

            if (!File.Exists(mSmellLibraryPath))
            {
                System.IO.File.WriteAllLines(mSmellLibraryPath, mSmellLibraryContentsConst.ToArray());
            }
            else
            {
                mSmellLibraryContents = new std::list<std::string>(System.IO.File.ReadAllLines(mSmellLibraryPath));
                if (mSmellLibraryContents.Count == 0)
                {
                    System.IO.File.WriteAllLines(mSmellLibraryPath, mSmellLibraryContentsConst.ToArray());
                    mSmellLibraryContents = new std::list<std::string>(System.IO.File.ReadAllLines(mSmellLibraryPath));
                }
            }
            for (int comI = 0; comI < GVS.MAX_NUMBER_OF_CILIAS; comI++)
            {
                comName = "COMPORT" + comI.ToString();
                if (!File.Exists(Path.Combine(mGameDirectory, comName)))
                {
                    System.IO.File.WriteAllText(Path.Combine(mGameDirectory, comName), ciliaContentsTemplate);
                }
                mCiliaManager.OverrideCilia((unsigned char)comI, System.IO.File.ReadAllLines(Path.Combine(mGameDirectory, comName)));
            }
        }

        /**
        * Loades a Game Profile
        * Brings in all the set smells and surround groups etc for a particular game profile.
        * If it doesn't exist it gets created.
        * @param aGameFolder we want to load or create
        * @param aSS other parameters that will be used if we need to create the game profile.
        */
        void LoadGameProfile(std::string aGameFolder, std::string aSS[], bool forceUpdate)
        {
            mGameFolder = aGameFolder;
            std::string surroundGroups = "";
            //this function will check if a game profile exists profile is directory
            mGameDirectory = mCiliaDirectory + "/" + mGameFolder;
            std::string ciliaContentsTemplate = "";
            if (!Directory.Exists(mGameDirectory))
            {
                Directory.CreateDirectory(mGameDirectory);
            }
            if (aSS.Length > 1)
            {
                //create template
                std::string sP = aSS[1];
                std::string s1 = aSS[2]; std::string s2 = aSS[3]; std::string s3 = aSS[4]; std::string s4 = aSS[5]; std::string s5 = aSS[6]; std::string s6 = aSS[7];
                std::string n1 = aSS[8]; std::string n2 = aSS[9]; std::string n3 = aSS[10]; std::string n4 = aSS[11]; std::string n5 = aSS[12]; std::string n6 = aSS[13];
                ciliaContentsTemplate = sP + "\n" + s1 + "\n" + s2 + "\n" + s3 + "\n" + s4 + "\n" + s5 + "\n" + s6 + "\n" +
                                                n1 + "\n" + n2 + "\n" + n3 + "\n" + n4 + "\n" + n5 + "\n" + n6;

                for (int i = 14; i < aSS.Length; i++)
                {
                    surroundGroups += aSS[i] + ",";
                }
                surroundGroups = surroundGroups.TrimEnd(',');
                //surroundGroups gets updated no matter what.
                System.IO.File.WriteAllText(Path.Combine(mGameDirectory, "SurroundGroups"), surroundGroups);
            }

            for (int comI = 0; comI < GVS.MAX_NUMBER_OF_CILIAS; comI++)
            {
                string comName = "COMPORT" + comI.ToString();
                //if contents dont exist write contents
                if ((!File.Exists(mGameDirectory + "/" + comName)) || forceUpdate)
                {
                    System.IO.File.WriteAllText(mGameDirectory + "/" + comName, ciliaContentsTemplate);
                }
                //load contents
                mCiliaManager.OverrideCilia((unsigned char)comI, System.IO.File.ReadAllLines(Path.Combine(mGameDirectory, comName)));
                //ciliaManager.GetCiliaContents()[comI] = ;
            }
            //update what Cilias are in each group
            std::list<unsigned char> activeCilias = Cilia::GetActiveCilias();
            for (int i = 0; i < activeCilias.size(); i++)
            {
                mCiliaManager.SetGroup(activeCilias[i], mCiliaManager.GetCiliaPosition(activeCilias[i]));
            }
        }

        /**
         * Deletes a folder
         * @param folder name of folder we want to delete.
         */
        void DeleteFolder(std::string aFolder)
        {
            for (int i = 0; i < 1000; i++)
            {
                try
                {
                    if (Directory.Exists(aFolder))
                    {
                        Directory.Delete(aFolder, true);
                    }
                    else
                        break;
                }
                catch (int e)
                {
                    std::string exception = e.Message;
                };
            }
        }
        /**
         * Returns path to current game profile.
         * @return string of game profile directory path
         */
        std::string GetGameDirectory()
        {
            return mGameDirectory;
        }
        /**
         * Returns a list of the smells to the control panel over TCP/IP.
         */
        void GetSmellLibrary()
        {
            std::string smellsLibraryString = "[";
            for (int ai = 0; ai < mSmellLibraryContents.Count; ai++)
            {
                smellsLibraryString += mSmellLibraryContents[ai] + ",";
            }
            smellsLibraryString = smellsLibraryString.TrimEnd(',');
            smellsLibraryString += "]\n";
            unsigned char smellBytes[] = System.Text.Encoding.UTF8.GetBytes(smellsLibraryString);
            mStream.Write(smellBytes, 0, smellBytes.Length);
        }
        /**
         * Updates the smell library to a new list of smells.
         * Saves list to file.
         * @param aSmells list of smells.
         */
        void UpdateSmellLibrary(std::string aSmells[])
        {
            mSmellLibraryContents->clear();
            for(int i = 0; i < aSmells->size(); i++)
            {
                mSmellLibraryContents->push_back(aSmells[i]);
            }

            std::ofstream out(mSmellLibraryPath);
            for(auto content : *mSmellLibraryContents)
            {
                out << mSmellLibraryContents << std::endl;
            }
            out.close();
            return;
        }
        /**
         * Add list of smells to existing list of smells and then sorts the list.
         * Saves list to file.
         * @param aSmellsToAdd list of smells to add to existing list of smells.
         */
        void AddToLibrary(std::string aSmellsToAdd[])
        {
            //get a list of new smells to add to the old list of smells.
            std::list<std::string> SmellLibraryAditionalContents;

            for(auto smell : aSmellsToAdd)
            {
                SmellLibraryAditionalContents->push_back(smell);
            }
            //check to see if each smell is already in old list
            for (auto newSmell : SmellLibraryAditionalContents)
            {
                if (std::find(mSmellLibraryContents->begin(),mSmellLibraryContents->end(),newSmell) != mSmellLibraryContents->end())
                    mSmellLibraryContents->push_back(newSmell);
            }
            mSmellLibraryContents->sort();

            std::ofstream out(mSmellLibraryPath);
            for(auto content : *mSmellLibraryContents)
            {
                out << mSmellLibraryContents << std::endl;
            }
            out.close();
            //System.IO.File.WriteAllLines(mSmellLibraryPath, mSmellLibraryContents->ToArray());
        }
    };
}