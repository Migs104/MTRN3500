//defining start up sequence
// Start up sequence
TCHAR* Units[10] = //
{
	TEXT("Laser.exe"),
	TEXT("Plotting.exe"),
	TEXT("XBox.exe"),
	TEXT("OpenGL.exe"),
	TEXT("Simulator.exe"),
	TEXT("UGV.exe"),
	TEXT("GPS.exe"),
};

// Module execution based variable declarations
STARTUPINFO s[10];
PROCESS_INFORMATION p[10];

// Starting the processes
for (int i = 0; i < NUM_UNITS; i++)
{
	// Check if each process is running
	if (!IsProcessRunning(Units[i]))
	{
		ZeroMemory(&s[i], sizeof(s[i]));
		s[i].cb = sizeof(s[i]);
		ZeroMemory(&p[i], sizeof(p[i]));
		// Start the child processes.

		if (!CreateProcess(NULL,   // No module name (use command line)
			Units[i],        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			CREATE_NEW_CONSOLE,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory
			&s[i],            // Pointer to STARTUPINFO structure
			&p[i])           // Pointer to PROCESS_INFORMATION structure
			)
		{
			printf("%s failed (%d).\n", Units[i], GetLastError());
			_getch();
			return -1;
		}
	}
	std::cout << "Started: " << Units[i] << endl;
	Sleep(1000);
}

//Is provess running function
bool IsProcessRunning(const char *processName)
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_stricmp(entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}
