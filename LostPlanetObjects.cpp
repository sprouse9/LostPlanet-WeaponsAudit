#include "LostPlanetObjects.h"



LostPlanetObjects::LostPlanetObjects(HANDLE LostPlanetProcessHandle, DWORD LostPlanetColoniesDX9_base)
{
	// constructor
	DWORD LostPlanetColoniesDX9_928318;


	//LostPlanetColoniesDX9_base + 928318;
	ReadProcessMemory(LostPlanetProcessHandle, (LPCVOID)(LostPlanetColoniesDX9_base + 0x928318), &LostPlanetColoniesDX9_928318, 4, NULL);

	// create WorldClass
	World world(LostPlanetColoniesDX9_928318);




}

World::World(DWORD LostPlanetColoniesDX9_928318)
{



}
