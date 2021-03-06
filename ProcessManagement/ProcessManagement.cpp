#using <System.dll>

#include <iostream>
#include <conio.h>
#include <tlhelp32.h>
#include <Windows.h>

#include "SMObject.h"
#include "SMFcn.h"
#include "SMStructs.h"

using namespace std;

#define CRITICAL 0x0017
#define NONCRITICAL 0x0008
#define UNITS 4

//defining start up sequence
// Start up sequence
TCHAR Units[10][20] = //
{
	TEXT("Laser.exe"),
	TEXT("Plotting.exe"),
	TEXT("XBox.exe"),
	TEXT("OpenGL.exe"),
	TEXT("Simulator.exe"),
	TEXT("UGV.exe"),
	TEXT("GPS.exe"),
};

int main() {
	__int64 frequency;
	__int64 hpcCount;
	double PMTimeStamp;
	int critCount = 0;
	int noncritCount = 0;

	// Module execution based variable declarations
	STARTUPINFO s[10];
	PROCESS_INFORMATION p[10];

	TCHAR ProM[] = TEXT("ProcessManagement");
	TCHAR GPS[] = TEXT("GPS");
	TCHAR Laser[] = TEXT("Laser");
	TCHAR Plot[] = TEXT("Plotting");
	TCHAR Sim[] = TEXT("Simulator");
	TCHAR UGV[] = TEXT("UGV");
	TCHAR XBox[] = TEXT("XBox");
	SMObject PMObj(ProM, sizeof(ProcessManagement));
	SMObject GPSObj(GPS, sizeof(GPS));
	SMObject LaserObj(Laser, sizeof(Laser));
	SMObject PlotObj(Plot, sizeof(Plotting));
	SMObject SimObj(Sim, sizeof(Simulator));
	SMObject UGVObj(UGV, sizeof(UGV));
	SMObject XBoxObj(XBox, sizeof(Remote));

	PMObj.SMCreate();
	GPSObj.SMCreate();
	LaserObj.SMCreate();
	PlotObj.SMCreate();
	SimObj.SMCreate();
	UGVObj.SMCreate();
	XBoxObj.SMCreate();

	PMObj.SMAccess();
	XBoxObj.SMAccess();

	// Starting the processes
	for (int i = 0; i < UNITS; i++)
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
	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
	ProcessManagement *PM = (ProcessManagement*)PMObj.pData;
	Remote *pXBox = (Remote*)XBoxObj.pData;

	PM->ShutDown.Status = 0x00;

	while (!PM->ShutDown.Flags.PM) {
		PM->HeartBeat.Flags.PM = 1;

		QueryPerformanceFrequency((LARGE_INTEGER *)&hpcCount);
		PMTimeStamp = (double)hpcCount / (double)frequency;
		PM->PMTimeStamp = PMTimeStamp;

		if ((PM->HeartBeat.Status & NONCRITICAL) != NONCRITICAL) {
			noncritCount++;
			if (noncritCount > 100) {
				noncritCount = 0;
			}
		} else {
			critCount = 0;
		}

		if (pXBox->Terminate) {
			break;
		}

		if (_kbhit()) {
			break;
		}

		PM->HeartBeat.Status = 0x00;
	}

	PM->ShutDown.Status = 0xFF;

	PM->ShutDown.Flags.UGV = 1;
	Sleep(200);
	PM->ShutDown.Flags.Laser = 1;
	Sleep(200);
	PM->ShutDown.Status = 0xFF;

	return 0;
}

//Is process running function
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
