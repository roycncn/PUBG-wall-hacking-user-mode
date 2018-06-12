#pragma once
#include <deque>
#include "json.hpp"
#include "Game.hpp"
#include "DataSender.hpp"
#include "KernelProto.hpp"
#include "KernelReader.hpp"

using namespace nlohmann;
class DataReader
{
public:
	DataReader(char *CURLAddr) 
	{
		hKreader = new KernelReader;
		dataSender = new DataSender;
		URL = CURLAddr;
	}
	
	~DataReader()
	{
		delete hKreader;
	}

	void doRead(HWND txtEdit)
	{
		json dataPack;
		
		dataPack["players"] = json::array();
		dataPack["items"] = json::array();
		dataPack["vehicles"] = json::array();

		initBasicInfo();
		readOut(dataPack);

		if (dataSender->isReady() && !(dataPack["items"].empty() && dataPack["vehicles"].empty() && dataPack["players"].empty()))
		{
			//std::string xxoo = dataPack.dump();
			//SetWindowTextW(txtEdit, std::wstring(xxoo.begin(), xxoo.end()).c_str());
			dataSender->sendData(URL,dataPack.dump());//

		}
	
	
	}

	int64_t getGameBase()
	{
		return hKreader->getGameBase();
	}

	int64_t readGameBase()
	{
		return hKreader->readGameBase();
	}


private:


	void initBasicInfo()
	{
		//hUWorld = hKreader->readMemByType<int64_t>(hKreader->getGameBase() + 0x3A74208, READ); //2
		hUWorld = hKreader->readMemByType<int64_t>(hKreader->getGameBase() + 0x401BDB0, READ); // 3.5.5.6:
		hgameInstance = hKreader->readMemByType<int64_t>(hUWorld + 0x140, READ);
		hULocalPlayer = hKreader->readMemByType<int64_t>(hgameInstance + 0x38, READ);
		hlocalPlayer = hKreader->readMemByType<int64_t>(hULocalPlayer, READ);
		hviewportclient = hKreader->readMemByType<int64_t>(hlocalPlayer + 0x58, READ);
		//hlocalPawn = hKreader->readMemByType<int64_t>(hlocalPlayer + 0x3A8, READ);
		//hlocalPlayerState = hKreader->readMemByType<int64_t>(hlocalPawn + 0x3C0, READ);
		hPWorld = hKreader->readMemByType<int64_t>(hviewportclient + 0x80, READ);
		hULevel = hKreader->readMemByType<int64_t>(hPWorld + 0x30, READ);
		hplayerCount = hKreader->readMemByType<int32_t>(hULevel + 0xB8, READ);//useful

		//hlocalPlayerPosition = hKreader->readPosition(hlocalPlayer + 0x70, READ);//useful
		//hlocalPlayerBasePointer = hKreader->readMemByType<int64_t>(hlocalPlayer, READ);//useful

		//hlocalTeam = hKreader->readMemByType<int32_t>(hlocalPlayerState + 0x0444, READ);

		hAActorPtr = hKreader->readMemByType<int64_t>(hULevel + 0xA0, READ);//useful

	}

	void readOut(json& dataPack)
	{

		for (int i = 0; i < hplayerCount; i++) //All Actor in this game
		{
			//Old Version
		    int64_t curActor = hKreader->readMemByType<int64_t>(hAActorPtr + (i * 0x8), READ);
			/*
			//TLS Encrypted https://github.com/jussihi/PUBG-map-hack/issues/111
			auto encrypted_actor = hKreader->readMemByType<TSLEncryptDataObj>(hAActorPtr + (i * sizeof(TSLEncryptDataObj)), READ);
			uint64_t tablePtr = 0x397B5A0 + hKreader->getGameBase();
			uint16_t decoded_xor = encrypted_actor.enc_xor ^ 0xCBAC;
			int64_t Decoded_Index = encrypted_actor.enc_index ^ 0xD7AF5ABC;
			auto Xor1 = hKreader->readMemByType<DWORD>(tablePtr + sizeof(DWORD)*((byte)(Decoded_Index)+0x300), READ);
			auto Xor2 = hKreader->readMemByType<DWORD>(tablePtr + sizeof(DWORD)*((byte)((DWORD_PTR)Decoded_Index >> 0x8) + 0x200), READ);
			auto Xor3 = hKreader->readMemByType<DWORD>(tablePtr + sizeof(DWORD)*((byte)((DWORD_PTR)Decoded_Index >> 0x10) + 0x100), READ);
			auto Xor4 = hKreader->readMemByType<DWORD>(tablePtr + sizeof(DWORD)*((DWORD_PTR)(Decoded_Index >> 0x18)), READ);
			int64_t Real_index = (Xor1 ^ Xor2 ^ Xor3 ^ ~Xor4) % 0x2B;

			int64_t curActor = encrypted_actor.ptrs[Real_index] ^ decoded_xor;
			*/
			//ID and Name will be consistant in one game
			int32_t curActorID = hKreader->readMemByType<int32_t>(curActor + 0x0018, READ);
			std::string curActorName = hKreader->getGNameFromId(curActorID);

			//If the Actor is one kind of Item, just Show
			if (curActorName == "DroppedItemGroup" ||
				curActorName == "DroppedItemInteractionComponent")
			{
				int64_t rootComponent = hKreader->readMemByType<int64_t>(curActor + 0x188, READ);
				Position actorPostion = hKreader->readPosition(rootComponent + 0x02D0, READ);
				int64_t DroppedItemArray = hKreader->readMemByType<int64_t>(curActor + 0x2E8, READ);
				int32_t DroppedItemCount = hKreader->readMemByType<int32_t>(curActor + 0x2F0, READ);

				for (int j = 0; j < DroppedItemCount; j++)
				{
					int64_t ADroppedItem = hKreader->readMemByType<int64_t>(DroppedItemArray + j * 0x10, READ);
					Position AbsoLocation = hKreader->readPosition(ADroppedItem + 0x02D0, READ);
					Position ItemLocation = {0,0};

					ItemLocation.X = AbsoLocation.X + actorPostion.X + hKreader->readMemByType<int32_t>(hPWorld + 0x928, READ);
					ItemLocation.Y = AbsoLocation.Y + actorPostion.Y + hKreader->readMemByType<int32_t>(hPWorld + 0x92C, READ);

					int64_t UItem = hKreader->readMemByType<int64_t>(ADroppedItem + 0x538, READ);
					int32_t UItemID = hKreader->readMemByType<int32_t>(UItem + 0x18, READ);
					std::string itemName = hKreader->getGNameFromId(UItemID);

					// if in the Useful Staff Vector
					for (std::map<std::string, std::string>::iterator it = dropGNameMap.begin(); it != dropGNameMap.end(); ++it)
					{
						if (itemName.substr(0, it->first.length()) == it->first)
						{
							dataPack["items"].emplace_back(json::object({ { "n", it->second },{ "x", ItemLocation.X },{ "y", ItemLocation.Y } }));
						}
					}
				}


			}

			else if (curActorName.substr(0, strlen("CarePackage")) == "CarePackage" || 
					 curActorName.substr(0, strlen("AircraftCarePackage")) == "AircraftCarePackage" || 
					 curActorName.substr(0, strlen("Carapackage_RedBox")) == "Carapackage_RedBox")
			{
				int64_t rootCmpPtr = hKreader->readMemByType<int64_t>(curActor + 0x188, READ);
				int64_t playerState = hKreader->readMemByType<int64_t>(curActor + 0x3D0, READ);
				Position actorLocation =hKreader->readPosition(rootCmpPtr + 0x2D0, READ);

				actorLocation.X += hKreader->readMemByType<int32_t>(hPWorld + 0x928, READ);
				actorLocation.Y += hKreader->readMemByType<int32_t>(hPWorld + 0x92C, READ);

				dataPack["vehicles"].emplace_back(json::object({ { "v", "Drop" },{ "x", actorLocation.X },{ "y", actorLocation.Y } }));

			}
			//Not Drop Item, then need recogize what item is it and record the ActorID in this game
			else if (std::find(ActorIDs.begin(), ActorIDs.end(), curActorID) == ActorIDs.end())
			{
				ActorIDs.push_back(curActorID);

				//Fail
				if (curActorName == "FAIL")
				{
					continue;

				}
				else //Check Vehicle or Player
				{
					// iterate thru playerGnameVec
					for (std::vector<std::string>::iterator it = playerGNameVec.begin(); it != playerGNameVec.end(); ++it)
					{
			
						if (*it == curActorName.substr(0, (*it).length()))
						{
							//Record the PlayerID info
							playerIDs.push_back(curActorID);
							break;
						}

						// Do the same for vehicle
						for (std::vector<std::string>::iterator it = vehicleGNameVec.begin(); it != vehicleGNameVec.end(); ++it)
						{
							
							if (*it == curActorName.substr(0, (*it).length()))
							{
								vehicleIDs.push_back(curActorID);
								break;
							}
						}

					}


				}


			}// End of Recogize by actorName

			if (std::find(vehicleIDs.begin(), vehicleIDs.end(), curActorID) != vehicleIDs.end())
			{

				int64_t rootComponent = hKreader->readMemByType<int64_t>(curActor + 0x188, READ);
				Position vehiclePos = hKreader->readPosition(rootComponent + 0x02D0, READ);

				vehiclePos.X += hKreader->readMemByType<int32_t>(hPWorld + 0x928, READ);
				vehiclePos.Y += hKreader->readMemByType<int32_t>(hPWorld + 0x92C, READ);

				std::string carName = hKreader->getGNameFromId(curActorID);

				dataPack["vehicles"].emplace_back(json::object({ { "v", carName.substr(0, 3) },{ "x", vehiclePos.X },{ "y", vehiclePos.Y } }));

			}

			if (std::find(playerIDs.begin(), playerIDs.end(), curActorID) != playerIDs.end())
			{
				/* //2
				int64_t rootCmpPtr = hKreader->readMemByType<int64_t>(curActor + 0x180, READ);
				int64_t playerState = hKreader->readMemByType<int64_t>(curActor + 0x3C0, READ);
				Position playerPos = hKreader->readPosition(rootCmpPtr + 0x1A0, READ);
				Position relativeRotation = hKreader->readPosition(rootCmpPtr + 0x01EC, READ);
				float hp = hKreader->readMemByType<float>(curActor + 0x107C, READ); 


				int32_t actorTeam = hKreader->readMemByType<int32_t>(playerState + 0x0444, READ);

				playerPos.X += hKreader->readMemByType<int32_t>(hPWorld + 0x918, READ);
				playerPos.Y += hKreader->readMemByType<int32_t>(hPWorld + 0x91C, READ);
				*/

				
				int64_t rootCmpPtr = hKreader->readMemByType<int64_t>(curActor + 0x188, READ);
				int64_t playerState = hKreader->readMemByType<int64_t>(curActor + 0x3D0, READ);
				Position playerPos = hKreader->readPosition(rootCmpPtr + 0x2D0, READ);
				Position relativeRotation = hKreader->readPosition(rootCmpPtr + 0x2DC, READ);
				float hp = hKreader->readMemByType<float>(curActor + 0x113C, READ);



				int32_t actorTeam = hKreader->readMemByType<int32_t>(playerState + 0x0484, READ);

				playerPos.X += hKreader->readMemByType<int32_t>(hPWorld + 0x928, READ);
				playerPos.Y += hKreader->readMemByType<int32_t>(hPWorld + 0x92C, READ);
				

				dataPack["players"].emplace_back(json::object({ { "t", actorTeam },{"hp",hp},{ "x", playerPos.X },{ "y", playerPos.Y },{ "r", relativeRotation.Y } }));
			}




		}




	}



	KernelReader* hKreader;
	DataSender* dataSender;
	string URL;


	//Basic Info Get from game
	int64_t hUWorld;
	int64_t hgameInstance;
	int64_t hULocalPlayer;
	int64_t hlocalPlayer;
	int64_t hviewportclient;
	int64_t hlocalPawn;
	int64_t hlocalPlayerState;
	int64_t hPWorld;
	int64_t hULevel;
	int64_t hplayerCount;
	Position hlocalPlayerPosition;
	int64_t hlocalPlayerBasePointer;
	int32_t hlocalTeam;
	int64_t hAActorPtr;

	//Data structure where we put ID
	std::deque<int32_t> ActorIDs;

	std::vector<int32_t> playerIDs;
	std::vector<int32_t> vehicleIDs;



	struct TSLEncryptDataObj
	{
		uint64_t ptrs[0x2B];
		uint16_t enc_index;
		byte unk2[0x6];
		uint16_t enc_xor;
		byte unk3[0x6];
	};




};
