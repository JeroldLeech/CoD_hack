#include "CoD.h"
int		IsNumPanPressed() {
	if (GetKeyState(VK_NUMPAD1) & 0x8000) return (1);
	else if (GetKeyState(VK_NUMPAD2) & 0x8000) return (2);
	else if (GetKeyState(VK_NUMPAD3) & 0x8000) return (3);
	else if (GetKeyState(VK_NUMPAD7) & 0x8000) return (7);
	else if (GetKeyState(VK_NUMPAD8) & 0x8000) return (8);
	else if (GetKeyState(VK_NUMPAD9) & 0x8000) return (9);
	return (0);
}

void	MovementCheats(MainValues* main_struct) {
	bool	jump = false;

	while (!main_struct->exit_cheat) {
		ReadProcessMemory(main_struct->process_handle, (LPVOID*)main_struct->player_z_velo_pointer, &main_struct->player_z_velo, sizeof(main_struct->player_z_velo), NULL);
		if (main_struct->player_z_velo >= 150.0f && !jump && main_struct->high_jump) {
			main_struct->player_z_velo = 500.0f;
			WriteProcessMemory(main_struct->process_handle, (LPVOID*)main_struct->player_z_velo_pointer, &main_struct->player_z_velo, sizeof(main_struct->player_z_velo), NULL);
			jump = true;
		}
		else if (main_struct->player_z_velo <= 0)
				jump = false;
		if (main_struct->no_gravity) {
			main_struct->player_z_velo = 0.0f;
			WriteProcessMemory(main_struct->process_handle, (LPVOID*)main_struct->player_z_velo_pointer, &main_struct->player_z_velo, sizeof(main_struct->player_z_velo), NULL);
		}
		if (!(FindWindow(NULL, L"Call of Duty")))
			main_struct->exit_cheat = true;
		this_thread::sleep_for(50ms);
	}
}

void	GetAllPointers(MainValues* main_struct) {
	main_struct->all_ammo_pointer = GetPointerAddress(main_struct->process_handle, main_struct->process_ID,
		main_struct->module_base_address, AMMO_OFFSET, main_struct->offsets->all_ammo_offset);

	main_struct->health_pointer = GetPointerAddress(main_struct->process_handle, main_struct->process_ID,
		main_struct->module_base_address, HEALTH_OFFSET, main_struct->offsets->health_offset);

	main_struct->player_x_pointer = GetPointerAddress(main_struct->process_handle, main_struct->process_ID,
		main_struct->module_base_address, PLAYER_COORDS_XYZ_OFFSET, main_struct->offsets->player_x_offset);
	main_struct->player_y_pointer = GetPointerAddress(main_struct->process_handle, main_struct->process_ID,
		main_struct->module_base_address, PLAYER_COORDS_XYZ_OFFSET, main_struct->offsets->player_y_offset);
	main_struct->player_z_pointer = GetPointerAddress(main_struct->process_handle, main_struct->process_ID,
		main_struct->module_base_address, PLAYER_COORDS_XYZ_OFFSET, main_struct->offsets->player_z_offset);

	main_struct->player_xy_speed_pointer = GetPointerAddress(main_struct->process_handle, main_struct->process_ID,
		main_struct->module_base_address, PLAYER_COORDS_XYZ_OFFSET, main_struct->offsets->player_xy_speed_offset);
	main_struct->player_z_velo_pointer = GetPointerAddress(main_struct->process_handle, main_struct->process_ID,
		main_struct->module_base_address, PLAYER_COORDS_XYZ_OFFSET, main_struct->offsets->player_z_velo_offset);
}

void	HealthAndAmmo(MainValues* main_struct) {
	DWORD	ammo_start = main_struct->all_ammo_pointer;
	WriteProcessMemory(main_struct->process_handle, (LPVOID*)main_struct->health_pointer, &main_struct->health, sizeof(main_struct->health), NULL);
	for (size_t i = 0; i < 128; i++){
		WriteProcessMemory(main_struct->process_handle, (LPVOID*)main_struct->all_ammo_pointer, &main_struct->ammo, sizeof(main_struct->ammo), NULL);
		main_struct->all_ammo_pointer += 4;
	}
	main_struct->all_ammo_pointer = ammo_start;
}

void	StructInit(MainValues* main_struct, DWORD pID, HANDLE p_handle) {
	main_struct->ammo = INT16_MAX;
	main_struct->health = INT32_MAX;

	main_struct->player_xy_speed = 1.0f;

	main_struct->x5_run = false;
	main_struct->high_jump = false;
	main_struct->no_gravity = false;
	main_struct->exit_cheat = false;

	main_struct->process_ID = pID;
	main_struct->process_handle = p_handle;

	main_struct->module_base_address = GetModuleBaseAddress(CoD, main_struct->process_ID);

	main_struct->offsets = new ValuesOffests;
}

int	CompareStrings(char* str_1, const char* str_2) {
	int index = 0;
	
	while (str_1[index] && str_2[index]) {
		if (str_1[index] != str_2[index])
			return (1);
		index++;
	}
	return (0);
}

DWORD	GetModuleBaseAddress(const char* module_name, DWORD pID) {
	HANDLE modules_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 one_module;

	one_module.dwSize = sizeof(MODULEENTRY32); //we have to initialize the size of the structure since it cannot (MAN)
	if (Module32First(modules_snapshot, &one_module)) { //get information about the first module
		do {
			if (!CompareStrings((char*)one_module.szModule, module_name)) //if the module name is the same as its variable name we retunr address
				return ((DWORD)one_module.modBaseAddr);
		} while (Module32Next(modules_snapshot, &one_module));
	}
	return NULL;
}

DWORD	GetPointerAddress(HANDLE process_handle, DWORD pID, DWORD module_base_address, DWORD some_address, vector<DWORD> offsets) {
	DWORD pointer_address = NULL;
	ReadProcessMemory(process_handle, (LPVOID*)(module_base_address + some_address), &pointer_address, sizeof(pointer_address), NULL);
	for (size_t i = 0; i < offsets.size() - 1; i++) //it is not necessary to change the last pointer
		ReadProcessMemory(process_handle, (LPVOID*)(pointer_address + offsets.at(i)), &pointer_address, sizeof(pointer_address), NULL);
	return (pointer_address += offsets.at(offsets.size() - 1)); //return the pointer with the last offset
}