#pragma once

#include <Windows.h>




class LostPlanetObjects
{
public:
	LostPlanetObjects(HANDLE LostPlanetProcessHandle, DWORD LostPlanetColoniesDX9_base);

};

// Add + BC to get to WorldPtr
class World
{
public:
	World(DWORD LostPlanetColoniesDX9_928318);



	//char pad_0000[48]; //0x0000
	class Player* playerPtr; //0x0030
	//char pad_0034[16]; //0x0034
};

