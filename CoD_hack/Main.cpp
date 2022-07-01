#include "CoD.h"

MainValues		main_struct;

int	main() {
	HWND	handle_to_game_window = NULL;
	HANDLE	process_handle = NULL;
	DWORD	pID = NULL;

	if ((handle_to_game_window = FindWindow(NULL, L"Call of Duty")))
		GetWindowThreadProcessId(handle_to_game_window, &pID);
	else {
		std::printf("Fail!\nStart the game, before launch!\n");
		this_thread::sleep_for(2000ms);
		return (1);
	}

	process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

	if (!process_handle || process_handle == INVALID_HANDLE_VALUE) {
		std::printf("Fail to open process!\n");
		this_thread::sleep_for(2000ms);
		return (1);
	}
	
	StructInit(&main_struct, pID, process_handle);
	GetAllPointers(&main_struct);
	system("CLS");
	printf("%s", CONTROLS);
	this_thread::sleep_for(5000ms);
	system("CLS");

	thread memory_modify (MovementCheats, &main_struct);

	while (!main_struct.exit_cheat) {
		if (GetKeyState(VK_F1) & 0x8000) {
			HealthAndAmmo(&main_struct);
			printf("Max health and ammo!\n");
			this_thread::sleep_for(500ms);
		}
		else if (GetKeyState(VK_F2) & 0x8000) {
			main_struct.high_jump = !main_struct.high_jump;
			printf("High jump: %s!\n", main_struct.high_jump == true ? "on" : "off");
			this_thread::sleep_for(500ms);
		}
		else if (GetKeyState(VK_F3) & 0x8000) {
			main_struct.x5_run = !main_struct.x5_run;
			if (main_struct.x5_run) {
				main_struct.player_xy_speed = 5.0f;
				WriteProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_xy_speed_pointer, &main_struct.player_xy_speed, sizeof(main_struct.player_xy_speed), NULL);
			}
			else {
				main_struct.player_xy_speed = 1.0f;
				WriteProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_xy_speed_pointer, &main_struct.player_xy_speed, sizeof(main_struct.player_xy_speed), NULL);
			}
			printf("High speed: %s!\n", main_struct.x5_run == true ? "on" : "off");
			this_thread::sleep_for(500ms);
		}
		else if (int move = IsNumPanPressed()) {
			ReadProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_x_pointer, &main_struct.player_x, sizeof(main_struct.player_x), NULL);
			ReadProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_y_pointer, &main_struct.player_y, sizeof(main_struct.player_y), NULL);
			ReadProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_z_pointer, &main_struct.player_z, sizeof(main_struct.player_z), NULL);

			main_struct.player_z_velo = 0.0f;
			WriteProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_z_velo_pointer, &main_struct.player_z_velo, sizeof(main_struct.player_z_velo), NULL);

			switch (move) {
			case 1:	main_struct.player_x -= 100.0f; WriteProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_x_pointer, &main_struct.player_x, sizeof(main_struct.player_x), NULL); break;
			case 2:	main_struct.player_z -= 100.0f; WriteProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_z_pointer, &main_struct.player_z, sizeof(main_struct.player_z), NULL); break;
			case 3:	main_struct.player_y -= 100.0f; WriteProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_y_pointer, &main_struct.player_y, sizeof(main_struct.player_y), NULL); break;
			case 7:	main_struct.player_x += 100.0f; WriteProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_x_pointer, &main_struct.player_x, sizeof(main_struct.player_x), NULL); break;
			case 8:	main_struct.player_z += 100.0f; WriteProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_z_pointer, &main_struct.player_z, sizeof(main_struct.player_z), NULL); break;
			case 9:	main_struct.player_y += 100.0f; WriteProcessMemory(main_struct.process_handle, (LPVOID*)main_struct.player_y_pointer, &main_struct.player_y, sizeof(main_struct.player_y), NULL); break;
			default: break;
			}
			printf("Player was moved to - x:%f y:%f z:%f\n", main_struct.player_x, main_struct.player_y, main_struct.player_z);
			this_thread::sleep_for(100ms);
		}
		else if (GetKeyState(VK_NUMPAD5) & 0x8000) {
			main_struct.no_gravity = !main_struct.no_gravity;
			printf("No gravity: %s\n", main_struct.no_gravity == true ? "on" : "off");
			this_thread::sleep_for(500ms);
		}
		else if (GetKeyState(72) & 0x8000 || GetKeyState(104) & 0x8000) {
			system("CLS");
			printf("%s", CONTROLS);
			this_thread::sleep_for(5000ms);
			system("CLS");
		}
		else if (GetKeyState(89) & 0x8000 || GetKeyState(121) & 0x8000)
			GetAllPointers(&main_struct);
		else if (GetKeyState(VK_F4) & 0x8000)
			main_struct.exit_cheat = true;
		this_thread::sleep_for(50ms);
	}

	memory_modify.join();
	CloseHandle(main_struct.process_handle);
	return (0);
}