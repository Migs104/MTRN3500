#using <System.dll>

#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <Windows.h>

#include "..\ProcessManagement\SMFcn.h"
#include "..\ProcessManagement\SMObject.h"
#include "..\ProcessManagement\SMStructs.h"

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::IO;
using namespace System::Text;

#define PI 3.1416

int main() {
	TCHAR ProM[] = TEXT("ProcessManagement");
	TCHAR LASER[] = TEXT("Laser");
	SMObject PMObj(ProM, sizeof(ProcessManagement));
	SMObject LaserObj(LASER, sizeof(Laser));
	PMObj.SMAccess();
	LaserObj.SMAccess();
	ProcessManagement* PM = (ProcessManagement*)PMObj.pData;
	Laser* laser = (Laser*)LaserObj.pData;

	// LMS151 port number must be 23000
	int PortNumber = 23000;
	// Pointer to TcpClent type object on managed heap
	TcpClient^ Client;
	// arrays of unsigned chars to send and receive data
	array<unsigned char>^ SendData;
	array<unsigned char>^ ReadData;
	// String command to ask for Channel 1 analogue voltage from the PLC
	// These command are available on Galil RIO47122 command reference manual
	// available online
	String^ AskScan = gcnew String("sRN LMDscandata");
	// String to store received data for display
	String^ ResponseData;

	// Creat TcpClient object and connect to it
	Client = gcnew TcpClient("192.168.1.200", PortNumber);
	// Configure connection
	Client->NoDelay = true;
	Client->ReceiveTimeout = 1500;//ms
	Client->SendTimeout = 1500;//ms
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;

	// unsigned char arrays of 16 bytes each are created on managed heap
	SendData = gcnew array<unsigned char>(16);
	ReadData = gcnew array<unsigned char>(2500);
	char RxData[2500];
	// Convert string command to an array of unsigned char
	SendData = System::Text::Encoding::ASCII->GetBytes(AskScan);


	// Get the network stream object associated with clien so we 
	// can use it to read and write
	NetworkStream^ Stream = Client->GetStream();

	while (!PM->ShutDown.Flags.Laser) {
		PM->HeartBeat.Flags.Laser = 1;
		


		/*int auth = 0;
		while (!auth) {*/
		String^ Auth = gcnew String("5117463\n");
		//String^ AuthMessage = gcnew String("OK\n");
		array<unsigned char>^ SendAuth = gcnew array<unsigned char>(16);
		//array<unsigned char>^ ReadAuth = gcnew array<unsigned char>(16);
		SendAuth = System::Text::Encoding::ASCII->GetBytes(Auth);
		Stream->Write(SendAuth, 0, SendAuth->Length);
		System::Threading::Thread::Sleep(500);
		/*Stream->Read(ReadAuth, 0, ReadAuth->Length);
		String^ ResponseAuth;
		ResponseAuth = System::Text::Encoding::ASCII->GetString(ReadData);
		if (ResponseAuth == AuthMessage) {
			auth = 1;
		}
	}*/
		printf("Authenticated!\n");

		//Loop
		//Laser LaserData;

		/*if (!Stream->DataAvailable) {
			printf("Data not available!\n");
		}*/

		// Write command asking for data
		Stream->WriteByte(0x02);
		Stream->Write(SendData, 0, SendData->Length);
		Stream->WriteByte(0x03);
		// Wait for the server to prepare the data, 1 ms would be sufficient, but used 10 ms
		System::Threading::Thread::Sleep(500);
		// Read the incoming data
		Stream->Read(ReadData, 0, ReadData->Length);


		for (int i = 0; i < 2500; i++)
			RxData[i] = ReadData[i];

		std::string ScanResp = RxData;
		std::istringstream is(ScanResp);
		std::string Fragments;
		std::string Resol;
		std::string Range[361];
		double conRange[361];
		double numPoints;
		int count = 0;
		double resolution;
		double startAngle;

		for (int i = 0; i < 26; i++) {
			is >> Fragments;
			if (i == 23) {
				is >> std::hex >> startAngle;
			} else if (i == 24) {
				is >> std::hex >> resolution;
				const char* res = Resol.c_str();
				resolution = std::strtol(res, NULL, 16);
				resolution = resolution / 10000;
			} else if (i == 25) {
				is >> std::hex >> numPoints;
			}		
		}

		printf("DATA!\n");
		for (int i = 0; i < numPoints; i++) {
			//Grab hex data
			is >> std::hex >> Range[i];
			//Convert to decimal
			const char* conv = Range[i].c_str();
			conRange[i] = std::strtol(conv, NULL, 16);

			laser->x[count] = conRange[i] * (cos(180 - (resolution*count)*(PI / 180)));
			laser->y[count] = conRange[i] * (sin(180 - (resolution*count)*(PI / 180)));
			count++;
			System::Threading::Thread::Sleep(100);
			std::cout << "x: " << laser->x[count] << " y: " << laser->y[count] << std::endl;
			if (PM->ShutDown.Flags.Laser == 1) {
				exit(0);
			}
		}

		if (count > 361) {
			count = 0;
		}

		// Convert incoming data from an array of unsigned char bytes to an ASCII string
		//ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
		// Print the received string on the screen
		//Console::WriteLine(ResponseData);

	}
		Stream->Close();
		Client->Close();

		//Console::ReadKey();
		//Console::ReadKey();

		PM->HeartBeat.Flags.Laser = 0;
		return 0;
} 




////Either include metadata from System.dll by including it here or by going to project properties
//// and then C++->Advanced->Forced #using file
////#using <System.dll>
//
//// #include <sstream>
//// #include <iostream>
//// #include <Windows.h>
//
//using namespace System;
//using namespace System::Net::Sockets;
//using namespace System::Net;
//using namespace System::IO;
//using namespace System::Text;*/
//
//value struct Point
//{
//	double X;
//	double Y;
//};
//
//ref class Laser1
//{
//private:
//	String ^ HostName;
//	int PortNumber;
//	int NumData;
//	double StartAngle;
//	double Resolution;
//	TcpClient^ Client;
//	NetworkStream^ Stream;
//public:
//	array<unsigned char>^ Data = gcnew array<unsigned char>(2048);
//	array<Point>^ Ranges;
//public:
//	Laser1() {}
//	~Laser1() {};
//	Laser1(String^ hostName, int portNumber)
//	{
//		HostName = hostName;
//		PortNumber = portNumber;
//		Client = gcnew TcpClient(HostName, PortNumber);
//		// Client settings
//		Client->NoDelay = true;
//		Client->ReceiveBufferSize = 1024;
//		Client->ReceiveTimeout = 500; //ms
//		Client->SendBufferSize = 1024;
//		Client->SendTimeout = 500;//ms
//
//		Stream = Client->GetStream();
//
//		//authenticate with the server
//			//i.e.send student number and check response
//
//			
//		String^ Auth = gcnew String("5117463\n");	
//		array<unsigned char>^ SendAuth = gcnew array<unsigned char>(16);
//		SendAuth = System::Text::Encoding::ASCII->GetBytes(Auth);
//		Stream->Write(SendAuth, 0, SendAuth->Length);
//		System::Threading::Thread::Sleep(500);
//			
//
//
//	}
//	array<Point>^ GetSingleScan()
//	{
//		String^ RequestScan = gcnew String("sRN LMDscandata");
//		//String^ RequestScan = gcnew String("sMN Run");
//		array<unsigned char>^ WriteBuf = gcnew array<unsigned char>(128);
//		WriteBuf = System::Text::Encoding::ASCII->GetBytes(RequestScan);
//		Stream->WriteByte(0x02); //STX
//		Stream->Write(WriteBuf, 0, WriteBuf->Length);
//		Stream->WriteByte(0x03); //ETX
//		System::Threading::Thread::Sleep(10);
//		if (Client->Available != 0)
//		{
//			Stream->Read(Data, 0, Data->Length);
//			
//			char RxData[2500];
//			String^ ResponseData;
//
//			for (int i = 0; i < 2048; i++)
//				RxData[i] = Data[i];
//
//			std::string ScanResp = RxData;
//			std::istringstream is(ScanResp);
//			std::string Fragments;
//			uint16_t Range[361];
//			int numPoints;
//
//			for (int i = 0; i < 25; i++) {
//				is >> Fragments;
//			}
//			is >> std::hex >> numPoints;
//
//			printf("DATA!\n");
//
//			for (int i = 0; i < numPoints; i++) {
//				//printf("Inside Data!\n");
//				is >> std::hex >> Range[i];
//				std::cout << (int)Range[i] << std::endl;
//			}
//
//			// Convert incoming data from an array of unsigned char bytes to an ASCII string
//			ResponseData = System::Text::Encoding::ASCII->GetString(Data);
//
//			// Print the received string on the screen
//			Console::WriteLine(ResponseData);
//		} 
//		printf("Making it to stream testing.\n");
//		/*if (!Stream->Read(Data, 0, Data->Length)) {
//			return nullptr;
//		} else {
//			GetResolution();
//			GetStartAngle();
//			GetNumData();
//			UpdateRanges();
//			return Ranges;
//		}*/
//		return nullptr;
//	}
//private:
//	void GetResolution()
//	{
//		Resolution = 0;//process Data to get resolution, i.e. skip 24 spaces (0x20) in Data,
//					   //then fill in an Int16 byte by byte, divide result by 10000.
//
//					   // one way to do this is to copy data into a stringstream object
//					   // then read from it into a string object until you get to the data you want
//					   // then read in the resolution (its stored a hex value)
//
//	}
//
//	void GetStartAngle()
//	{
//		StartAngle = 45.0; // process Data to get StartAngle (skip 23 spaces)
//						   // then fill in an int32 byte by byte and divide the result by 10000.
//
//						   // do similar to above
//	}
//
//	void GetNumData()
//	{
//		NumData = 0; // procrss Data to get NumData skip 25 spaces and
//					 // then fill in an uint_16  byte by byte.
//
//					 // do similar to above
//	}
//
//	void UpdateRanges()
//	{
//		Ranges = gcnew array<Point>(NumData);
//		// process Data to update ranges skip 26 spaces, then run a for loop
//
//		for (int i = 0; i < NumData; i++)
//		{
//			Ranges[i].X = 0;// Process data to fill these values
//			Ranges[i].Y = 0;// Process data to fill these values
//		}
//	}
//
//};
//
//int main()
//{
//	TCHAR ProM[] = TEXT("ProcessManagement");
//	TCHAR LASER[] = TEXT("GPS");
//	SMObject PMObj(ProM, sizeof(ProcessManagement));
//	SMObject LaserObj(LASER, sizeof(Laser));
//	PMObj.SMAccess();
//	LaserObj.SMAccess();
//	ProcessManagement* PM = (ProcessManagement*)PMObj.pData;
//	Laser* laser = (Laser*)LaserObj.pData;
//
//	double X[361], Y[361];
//	while (!PM->ShutDown.Flags.Laser) {
//		PM->HeartBeat.Flags.Laser = 1;
//		Laser1^ MyLaser = gcnew Laser1("192.168.1.200", 23000);
//		MyLaser->GetSingleScan();
//		printf("Crash after scan.\n");
//		// first data X point
//		//X[0] = MyLaser->Ranges[0].X;
//		//Y[0] = MyLaser->Ranges[0].Y;
//	}
//
//	return 0;
//}
