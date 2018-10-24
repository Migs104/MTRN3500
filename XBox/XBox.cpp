#using <System.dll>

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <xinput.h>

#include "SMFcn.h"
#include "SMObject.h"
#include "SMStructs.h"

#include "SMFcn.cpp"
#include "SMObject.cpp"

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

#define MAX_COORDINATE 32767

int main()
{
	TCHAR ProM[] = TEXT("ProcessManagement");
	TCHAR XBOX[] = TEXT("XBox");
	SMObject PMObj(ProM, sizeof(ProcessManagement));
	SMObject XBoxObj(XBOX, sizeof(Remote));
	PMObj.SMAccess();
	XBoxObj.SMAccess();
	ProcessManagement* PM = (ProcessManagement*)PMObj.pData;
	Remote* xbox = (Remote*)XBoxObj.pData;

	bool Quit = false;
	XINPUT_STATE State;

	while (!PM->ShutDown.Flags.Remote) {
		PM->HeartBeat.Flags.Remote = 1;

		//XInputEnable(true)

		while (!Quit) {
			if (State.Gamepad.wButtons == 1) {
				Quit = true;
				xbox->Terminate = 1;
				PM->ShutDown.Status = 1;
			}
			if (PM->ShutDown.Flags.Remote == 1) {
				exit(0);
			}

			std::cout << State.Gamepad.wButtons << "   " << State.Gamepad.sThumbLX << "   " << State.Gamepad.sThumbLY << "   " << State.Gamepad.sThumbRX << "   " << State.Gamepad.sThumbRY << "   ";

			xbox->SetSpeed = State.Gamepad.sThumbLY;
			xbox->SetSteering = State.Gamepad.sThumbRX;

		}
		std::cout << "Program terminated " << std::endl;

		//if(state.)
	}
	PM->HeartBeat.Flags.Remote = 0;
	return 0;
}