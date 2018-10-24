#using <System.dll>

#include <Windows.h>
#include <conio.h>
#include <stdio.h>

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
	TCHAR Ugv[] = TEXT("UGV");
	TCHAR XBOX[] = TEXT("XBox");
	SMObject PMObj(ProM, sizeof(ProcessManagement));
	SMObject UGVObj(Ugv, sizeof(UGV));
	SMObject XBoxObj(XBOX, sizeof(Remote));
	PMObj.SMAccess();
	UGVObj.SMAccess();
	XBoxObj.SMAccess();
	ProcessManagement* PM = (ProcessManagement*)PMObj.pData;
	UGV* ugv = (UGV*)UGVObj.pData;
	Remote* xbox = (Remote*)XBoxObj.pData;

	int PortNumber = 25000;
	TcpClient^ Client;
	bool flag = true;

	array<unsigned char>^ SendData = gcnew array<unsigned char>(100);

	Client = gcnew TcpClient("192.168.1.200", PortNumber);
	Client->NoDelay = true;
	Client->ReceiveTimeout = 1500;//ms
	Client->SendTimeout = 500;//ms
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;

	NetworkStream^ Stream = Client->GetStream();

	while (!PM->ShutDown.Flags.UGV) {
		if (PM->HeartBeat.Flags.PM) {
			PM->HeartBeat.Flags.UGV = 1;

			String^ Auth = gcnew String("5117463\n");
			array<unsigned char>^ SendAuth = gcnew array<unsigned char>(16);
			SendAuth = System::Text::Encoding::ASCII->GetBytes(Auth);
			Stream->Write(SendAuth, 0, SendAuth->Length);
			System::Threading::Thread::Sleep(500);

			double speed = xbox->SetSpeed / MAX_COORDINATE;
			double steering = xbox->SetSteering * 40 / MAX_COORDINATE;
			flag = !flag;
			char a[100];
			sprintf(a, "# %f %f %f #", steering, speed, flag);

			String^ SendString = gcnew String(a);
			SendData = System::Text::Encoding::ASCII->GetBytes(SendString);
			Stream->Write(SendData, 0, SendData->Length);
			System::Threading::Thread::Sleep(500);
		} else {
			PM->ShutDown.Flags.UGV = 0;
		}
	}

	Stream->Close();
	Client->Close();

	PM->HeartBeat.Flags.UGV = 0;

	return 0;
}