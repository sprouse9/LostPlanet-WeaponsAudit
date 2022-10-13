// LostPlanet Weapons Audit.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <tlhelp32.h>
#include "LostPlanetObjects.h"
#include <vector>
#include <conio.h>
#include <string>


#define ENTER 13
#define ESC	  27

using namespace std;

DWORD GetModuleBaseAddress(LPCWSTR lpszModuleName, DWORD pID);

// Created with ReClass.NET 1.2 by KN4CK3R

class Base
{
public:

	Base();


	char pad_0000[188]; //0x0000
	class World* worldPtr; //0x00BC
	char pad_00C0[128]; //0x00C0
};


Base::Base()
{
}

//class World
//{
//public:
//	char pad_0000[48]; //0x0000
//	class Player* playerPtr; //0x0030
//	char pad_0034[16]; //0x0034
//};


class Player
{
public:
	char pad_0000[3504]; //0x0000
	class Weapon* currentWeapon; //0x0DB0
	char pad_0DB4[656]; //0x0DB4
}; //Size: 0x1044


class Weapon
{
public:
	char pad_0000[8]; //0x0000
	class Weapon* nextWeapon; //0x0008
	class Weapon* prevWeapon; //0x000C
	char pad_0010[6640]; //0x0010
	class WeaponDetails* details; //0x1A00
	char pad_1A04[576]; //0x1A04
}; //Size: 0x1C44


class WeaponDetails
{
public:
	char pad_0000[7]; //0x0000
	char name[11]; //0x0007
	char pad_0012[54]; //0x0012
	int16_t zoom; //0x0048
	char pad_004A[4]; //0x004A
}; //Size: 0x004E
//static_assert(sizeof(WeaponDetails) == 0x4E);


int main()
{
	// attempt to find the LP game window
	HWND hGameWindow = FindWindow(NULL, _T("Lost Planet Colonies Ver 1.0.1.0 for Dx9"));
	DWORD pLostPlanetProcessID = NULL;
	HANDLE LostPlanetProcessHandle = NULL;
	BOOL foundGame = TRUE;
	DWORD gameBaseAddress = NULL;
	DWORD LostPlanetColoniesDX9_928318 = NULL;
	DWORD firstWeaponAddress = NULL;
	DWORD playerBaseAddress = NULL;

	if (hGameWindow == NULL) {
		MessageBox(NULL, _T("Couldn't find the game!"), _T("Lost Planet Trainer DX9"), NULL);
		foundGame = FALSE;
		return 0;
	}

	if (foundGame) {
		if (GetWindowThreadProcessId(hGameWindow, &pLostPlanetProcessID) > 0) {
			//MessageBox(NULL, _T("FOUND!"), _T("Lost Planet Trainer DX9"), NULL);

			LostPlanetProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pLostPlanetProcessID);

			if (LostPlanetProcessHandle != INVALID_HANDLE_VALUE && LostPlanetProcessHandle != NULL)
				gameBaseAddress = GetModuleBaseAddress(_T("LostPlanetColoniesDX9.exe"), pLostPlanetProcessID);

		}

		// Get value of: LostPlanetColoniesDX9.exe + 928318;
		ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(gameBaseAddress + 0x928318), &LostPlanetColoniesDX9_928318, 4, NULL);

		firstWeaponAddress = LostPlanetColoniesDX9_928318;
		std::vector<DWORD> pointsOffsets{ 0xBC, 0x30, 0x0DB0 };
		for (int i = 0; i < pointsOffsets.size() - 1; i++) {
			ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(firstWeaponAddress + pointsOffsets.at(i)), &firstWeaponAddress, 4, NULL);
			if (pointsOffsets.at(i) == 0x30) {
				//save the Player Offset
				playerBaseAddress = firstWeaponAddress;
			}


		}
		firstWeaponAddress += pointsOffsets.at(pointsOffsets.size() - 1);	// Add the last offset, done!
		ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(firstWeaponAddress ), &firstWeaponAddress, 4, NULL);


		// Now we have the current weapon, but we have the option of starting from user entered address
		//std::cout << std::hex << std::uppercase << firstWeaponAddress;

		DWORD currentWeaponAddress = 0;
		DWORD detailsAddress, customAddress = NULL;
		CHAR weaponName[11];
		BOOL forward;

		std::cout << "enter a weapon address or ENTER to default: ";
		string input;
		char ch;
		
		do {
			ch = _getch();
			if (ch == ESC) {
				// user entered ESC
				currentWeaponAddress = firstWeaponAddress;
			}
			else if (ch == ENTER) {
				// done with input. covert string to int
				//istringstream ss(input);
				currentWeaponAddress = strtoul(input.c_str(), NULL, 16);



				//currentWeaponAddress = strtold(input, 0, 0);
			}
			else {
				// build the address string
				input += ch;
				//input.append(ch);
				std::cout << ch;
			}
		} while ((ch != ENTER) && (ch != ESC));


		//cin >> customAddress;

		std::cout << endl << "Traverse the list forward or back? ";
		std::cin >> forward;

		DWORD prevAddress = currentWeaponAddress;

		// Loop thru all weapons and print out the address
		while (currentWeaponAddress != NULL) {

			
			// now get the weapon details { +1A00 }
			ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(currentWeaponAddress + 0x1A00), &detailsAddress, 4, NULL);
			ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(detailsAddress		  + 0x0007), &weaponName, 11, NULL);

			std::cout << hex << uppercase << currentWeaponAddress << "\t" << (currentWeaponAddress - prevAddress) << "\t" << weaponName << endl;
			prevAddress = currentWeaponAddress;
			// now advance the pointer to the next weapon in the linked list
			if(forward)
				ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(currentWeaponAddress + 0x0008), &currentWeaponAddress, 4, NULL);
			else
				ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(currentWeaponAddress + 0x000C), &currentWeaponAddress, 4, NULL);
						
		}

		std::cout << "==================================================================" << endl << endl;

		// trying to find other weapons not on the linked list
//		currentWeaponAddress = firstWeaponAddress;
//		prevAddress = currentWeaponAddress;

		//while (currentWeaponAddress < 0x19000000) {

		//	// now get the weapon details { +1A00 }
		//	ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(currentWeaponAddress + 0x1A00), &detailsAddress, 4, NULL);
		//	ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(detailsAddress + 0x0007), &weaponName, 11, NULL);

		//	cout << hex << uppercase << currentWeaponAddress << "\t" << (currentWeaponAddress - prevAddress) << "\t" << weaponName << endl;
		//	prevAddress = currentWeaponAddress;
		//	// now advance the pointer to the next weapon in the linked list
		//	//if (forward)
		//		//ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(currentWeaponAddress + 0x0008), &currentWeaponAddress, 4, NULL);
		//	//else

		//	currentWeaponAddress += 0x1A50;
		//	//ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(currentWeaponAddress), &currentWeaponAddress, 4, NULL);
		//}

		// let's start at LostPlanetColoniesDX9.exe+8ABA80 --> 17427980+0224

		// Get value of: LostPlanetColoniesDX9.exe + 8ABA80;
		DWORD LostPlanetColoniesDX9_8ABA80;
		ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(gameBaseAddress + 0x8ABA80), &LostPlanetColoniesDX9_8ABA80, 4, NULL);

		//currentWeaponAddress = LostPlanetColoniesDX9_8ABA80 + 0x0224;	// Address of first weapon in our list
		// Advance our pointer
		ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(LostPlanetColoniesDX9_8ABA80 + 0x0224), &currentWeaponAddress, 4, NULL);

		vector<DWORD> weaponsList;

		prevAddress = currentWeaponAddress;
		int i = 0;
		DWORD objectType;
		// Loop thru all weapons and print out the address
		while (currentWeaponAddress != NULL) {

			weaponsList.push_back( currentWeaponAddress );

			// now get the weapon details { +1A00 }
			ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(currentWeaponAddress + 0x1A00), &detailsAddress, 4, NULL);
			ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(detailsAddress + 0x0007), &weaponName, 11, NULL);

			ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(currentWeaponAddress + 0x0000), &objectType, 4, NULL);

			std::cout << std::dec << ++i << "   ";

			std::cout << hex << uppercase << currentWeaponAddress << "\t" << (objectType == 0x00BB2850 ? "COIN" : "   " ) << "\t" << weaponName << endl;
			//std::cout << hex << uppercase << currentWeaponAddress << "\t" << (objectType) << "\t" << weaponName << endl;


			prevAddress = currentWeaponAddress;
			// now advance the pointer to the next weapon in the linked list

			// only forward in this case
			ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(currentWeaponAddress + 0x0008), &currentWeaponAddress, 4, NULL);
			

		}

		int addressSelected = NULL;
		do {

			std::cout << endl << "Select an address to teleport to you: ";


			input.clear();
			addressSelected = NULL;

			do {
				ch = _getch();
				if (ch == ESC) {
					// user entered ESC, so EXIT
					// currentWeaponAddress = firstWeaponAddress;
				}
				else if (ch == ENTER) {
					// done with input. covert string to int
					//istringstream ss(input);
					//currentWeaponAddress = strtoul(input.c_str(), NULL, 16);
					addressSelected = atoi(input.c_str());
				}
				else {
					// build the address string
					input += ch;
					//input.append(ch);
					std::cout << ch;
				}
			} while ((ch != ENTER) && (ch != ESC));

			// was an address selected?
			if (addressSelected >= 0) {

				float playerXcoord, playerZcoord, playerYcoord;

				if (addressSelected == 0) {

					// Loop thru all the weapons and place them at the player's coordinate.
					// The idea is the player will move and every second the next item will be placed on the ground
					// so that they don't all pile up in 1 location

					for (int i = 0; i < weaponsList.size() - 1; i++) {
						Sleep(1000);
						
						ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(playerBaseAddress + 0x0030), &playerXcoord, 4, NULL);
						ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(playerBaseAddress + 0x0034), &playerZcoord, 4, NULL);
						ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(playerBaseAddress + 0x0038), &playerYcoord, 4, NULL);

						// Now overwrite the weapon selected coordinates
						BOOL work = WriteProcessMemory(LostPlanetProcessHandle, (LPVOID)(weaponsList.at(i) + 0x0030), &playerXcoord, 4, NULL);
						     work = WriteProcessMemory(LostPlanetProcessHandle, (LPVOID)(weaponsList.at(i) + 0x0034), &playerZcoord, 4, NULL);
						     work = WriteProcessMemory(LostPlanetProcessHandle, (LPVOID)(weaponsList.at(i) + 0x0038), &playerYcoord, 4, NULL);
					
					}




				}
				else {


					std::cout << endl << weaponsList.at(addressSelected - 1) << endl;
					DWORD weaponAddress = weaponsList.at(addressSelected - 1);

					// get the coordinates of the weapon selected and change them to player coordinates

					

					ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(playerBaseAddress + 0x0030), &playerXcoord, 4, NULL);
					ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(playerBaseAddress + 0x0034), &playerZcoord, 4, NULL);
					ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(playerBaseAddress + 0x0038), &playerYcoord, 4, NULL);

					// Now overwrite the weapon selected coordinates
					BOOL work = WriteProcessMemory(LostPlanetProcessHandle, (LPVOID)(weaponAddress + 0x0030), &playerXcoord, 4, NULL);
					work = WriteProcessMemory(LostPlanetProcessHandle, (LPVOID)(weaponAddress + 0x0034), &playerZcoord, 4, NULL);
					work = WriteProcessMemory(LostPlanetProcessHandle, (LPVOID)(weaponAddress + 0x0038), &playerYcoord, 4, NULL);
				}



			}


		} while ((ch != ESC) && (addressSelected != 0));

	}

	// Attempt to pass the address to Base and have it calculate the offsets
	//Base base = Base(gameBaseAddress);
	//LostPlanetObjects LPObjects = LostPlanetObjects(LostPlanetProcessHandle, gameBaseAddress);












}


DWORD GetModuleBaseAddress(LPCWSTR lpszModuleName, DWORD pID)
	{
		DWORD dwModuleBaseAddress = 0;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
		MODULEENTRY32 ModuleEntry32 = { 0 };
		ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

		/*	TCHAR typedef explained:
				TCHAR --> WCHAR
				WCHAR --> wchar_t

				_tcscmp(const wchar_t*, const wchar_t*);
		*/

		if (Module32First(hSnapshot, &ModuleEntry32)) {
			do {

				if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) {

					dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
					break;
				}

			} while (Module32Next(hSnapshot, &ModuleEntry32));
		}

		CloseHandle(hSnapshot);

		return dwModuleBaseAddress;
	}

