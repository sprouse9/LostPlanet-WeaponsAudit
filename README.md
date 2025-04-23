# Lost Planet Weapons Audit & Teleportation Tool

This C++ reverse engineering project is designed to inspect and manipulate in-game weapon objects in **Lost Planet Colonies Ver 1.0.1.0 for DirectX 9**. It traverses a doubly linked list of weapon objects in memory, displays their properties, and allows the player to teleport weapons to their location—or teleport to them.

## 🎯 Features

- Attaches to the *Lost Planet Colonies* DX9 game process
- Traverses a **doubly linked list** of `Weapon` objects in memory
- Reads and prints object attributes, including weapon names and memory offsets
- Teleports selected weapons to the player’s location or the player to selected weapons
- Allows optional traversal of unlinked objects by direct address scanning
- Implements player/object coordinate transfers using WinAPI memory functions

## 🔧 Technologies Used

- **C++ (C++11-style)** with Win32 API
- `ReadProcessMemory` / `WriteProcessMemory` for memory manipulation
- `FindWindow`, `OpenProcess`, `CreateToolhelp32Snapshot` for process control
- `ReClass.NET` memory structure definitions
- `x64dbg` / `IDA Pro` for static and dynamic analysis

## 📦 Object Mapping Overview

The tool defines and manipulates the following key in-game structures:

- `Base → World → Player → Weapon → WeaponDetails`
- Linked list traversal using:
  - `nextWeapon` (0x0008)
  - `prevWeapon` (0x000C)
- Custom `Vector3` coordinate writes to teleport weapons in 3D space

## 💡 Example Use Cases

- Reverse engineering and learning memory structures in games
- Creating basic in-game mods for testing or fun
- Studying how linked objects are represented and updated in memory
- Demonstrating low-level WinAPI memory access in a practical project

## 🚀 Getting Started

To build and use this tool:

1. Clone this repository and open it in Visual Studio
2. Set build target to **x86 Release**
3. Run with administrator privileges while the game is running
4. Input a custom address or press ENTER to use the default weapon pointer
5. Choose traversal direction, view weapon entries, and teleport objects

## 🧠 Safety Notes

This tool reads and writes directly to memory of a running process. Use it **only in single-player** scenarios for educational or debugging purposes. Modifying game memory may violate terms of service and cause unintended behavior.

## 📄 License

No restrictions.

## 🔗 Related Project

- [LPStructure](https://github.com/sprouse9/LPStructure) – class-based object access and teleportation for Lost Planet DX9

