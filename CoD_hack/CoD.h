#ifndef COD_HEADER
# define COD_HEADER

#define HEALTH_OFFSET				0x0112BC58
#define AMMO_OFFSET					0x0112BC64

#define PLAYER_COORDS_XYZ_OFFSET	0x0112BC64

#define CONTROLS	"Sad Cat trainer\n\nF1 - Max health and ammo\nF2 - High jump\nF3 - X5 speed\nNUM( 1-3, 7-9) - TP | NUM(1-3) = -X,-Z,-Y | NUM(7-9) = +X,+Z,+Y | Z is vertical\nNUM5 - Low gravity\n\nH - Controls\nY - Trainer update (Press on every new lvl)\nF4 - Exit\n"

#define CoD "CoDSP.exe"

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <thread>
#include <chrono>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <tchar.h>
#include <psapi.h>

using namespace std;

struct ValuesOffests{
	vector<DWORD> all_ammo_offset = { 0xf0 };
	vector<DWORD> health_offset = { 0x1d8 };

	vector<DWORD> player_x_offset = { 0x14 };
	vector<DWORD> player_y_offset = { 0x18 };
	vector<DWORD> player_z_offset = { 0x1c };
	
	vector<DWORD> player_xy_speed_offset = { 0x338 };
	vector<DWORD> player_z_velo_offset = { 0x28 };
};

struct MainValues{
	int		ammo, health;

	float	player_x, player_y, player_z;
	float	player_z_velo, player_xy_speed;

	bool	x5_run, high_jump, no_gravity, exit_cheat;

	DWORD	module_base_address;

	DWORD	all_ammo_pointer;
	DWORD	health_pointer;

	DWORD	player_x_pointer;
	DWORD	player_y_pointer;
	DWORD	player_z_pointer;

	DWORD	player_xy_speed_pointer;
	DWORD	player_z_velo_pointer;

	DWORD	process_ID;
	HANDLE	process_handle;

	ValuesOffests*	offsets;
};

int		CompareStrings(char*, const char*);
DWORD	GetModuleBaseAddress(const char*, DWORD);
DWORD	GetPointerAddress(HANDLE, DWORD, DWORD, DWORD, vector<DWORD>);
void	StructInit(MainValues*, DWORD, HANDLE);
void	HealthAndAmmo(MainValues*);
void	MovementCheats(MainValues*);
void	GetAllPointers(MainValues*);
int		IsNumPanPressed();

#endif COD_HEADER