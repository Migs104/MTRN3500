#using <System.dll>

#include <conio.h>
#include <stdio.h>
#include <iostream>

#include "SMFcn.h"
#include "SMObject.h"
#include "SMStructs.h"

#define CRC32_POLYNOMIAL			0xEDB88320L

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

unsigned long CalculateBlockCRC32(unsigned long ulCount, unsigned char *ucBuffer);

struct GPS1 {
	unsigned char discard[44];
	double Northing;
	double Easting;
	double Height;
	unsigned char discard2[40];
	unsigned int Checksum;
};

int main()
{
	TCHAR ProM[] = TEXT("ProcessManagement");
	TCHAR Gps[] = TEXT("GPS");
	SMObject PMObj(ProM, sizeof(ProcessManagement));
	SMObject GPSObj(Gps, sizeof(GPS));
	PMObj.SMAccess();
	GPSObj.SMAccess();
	ProcessManagement* PM = (ProcessManagement*)PMObj.pData;
	GPS* gps = (GPS*)GPSObj.pData;
	


		// LMS151 port number must be 2111
		int PortNumber = 24000;
		// Pointer to TcpClent type object on managed heap
		TcpClient^ Client;
		// arrays of unsigned chars to send and receive data
		//array<unsigned char>^ SendData;
		array<unsigned char>^ ReadData;
		// String command to ask for Channel 1 analogue voltage from the PLC
		// These command are available on Galil RIO47122 command reference manual
		// available online
		//String^ AskScan = gcnew String("sRN LMDscandata");
		// String to store received data for display
		//String^ ResponseData;

		// Creat TcpClient object and connect to it
		Client = gcnew TcpClient("192.168.1.200", PortNumber);
		// Configure connection
		Client->NoDelay = true;
		Client->ReceiveTimeout = 500;//ms
		Client->SendTimeout = 500;//ms
		Client->ReceiveBufferSize = 1024;
		Client->SendBufferSize = 1024;

		// unsigned char arrays of 16 bytes each are created on managed heap
		//SendData = gcnew array<unsigned char>(16);
		ReadData = gcnew array<unsigned char>(112);
		// Convert string command to an array of unsigned char
		//SendData = System::Text::Encoding::ASCII->GetBytes(AskScan);

		// Get the network stream object associated with clien so we 
		// can use it to read and write
		NetworkStream^ Stream = Client->GetStream();

		while (!PM->ShutDown.Flags.GPS) {
			PM->HeartBeat.Flags.GPS = 1;
		//Loop
			// Write command asking for data
			/*Stream->WriteByte(0x02);
			Stream->Write(SendData, 0, SendData->Length);
			Stream->WriteByte(0x03);*/
			// Wait for the server to prepare the data
			//System::Threading::Thread::Sleep(10);
			// Read the incoming data
		if (Stream->DataAvailable) {
			System::Threading::Thread::Sleep(10);
			Stream->Read(ReadData, 0, ReadData->Length);

			GPS1 gpsdata;
			unsigned char* BytePtr;
			BytePtr = (unsigned char*)&gpsdata;

			for (int i = 0; i < 112; i++) {
				Console::Write("{0:X2} ", ReadData[i]);
				*(BytePtr + i) = ReadData[i];
			}
			printf("\n\n");

			GPS SharedGPS;
			unsigned char* BPtr;
			BPtr = (unsigned char*)&SharedGPS;
			for (int i = 0; i < 112; i++) {
				*(BPtr + i) = *(BytePtr + i);
			}

			/*for (int i = 0; i < 112; i++) {
				Console::Write("{0:X2} ", *(BytePtr + i));
			}

			printf("\n");*/

			gps->Northing = SharedGPS.Northing;
			gps->Easting = SharedGPS.Easting;
			gps->Height = SharedGPS.Height;
			gps->Checksum = SharedGPS.Checksum;

			unsigned int tempChecksum = CalculateBlockCRC32(108, BytePtr);
			/*if (tempChecksum == gpsdata.Checksum) {
				printf("Checksum validated!\n");
			}
			else {
				printf("Checksum incorrect!\n");
			}*/

			//gpsdata.Checksum = CalculateBlockCRC32(108, BytePtr);

			printf("Northing: ");
			Console::WriteLine(gpsdata.Northing);
			printf("Easting: ");
			Console::WriteLine(gpsdata.Easting);
			printf("Height: ");
			Console::WriteLine(gpsdata.Height);
			printf("Calculated CRC: ");
			Console::WriteLine(tempChecksum);
			printf("Received CRC: ");
			Console::WriteLine(gpsdata.Checksum);
			printf("\n");


			/*Convert incoming data from an array of unsigned char bytes to an ASCII string
			ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
			Print the received string on the screen
			Console::WriteLine(ResponseData);*/
		}
		

		

		//Console::ReadKey();
		//Console::ReadKey();
	}
		Stream->Close();
		Client->Close();

		PM->HeartBeat.Flags.GPS = 0;
		return 0;
}

unsigned long CRC32Value(int i)
{
	int j;
	unsigned long ulCRC;
	ulCRC = i;
	for (j = 8; j > 0; j--)
	{
		if (ulCRC & 1)
			ulCRC = (ulCRC >> 1) ^ CRC32_POLYNOMIAL;
		else
			ulCRC >>= 1;
	}
	return ulCRC;
}

unsigned long CalculateBlockCRC32(unsigned long ulCount, /* Number of bytes in the data block */
	unsigned char *ucBuffer) /* Data block */
{
	unsigned long ulTemp1;
	unsigned long ulTemp2;
	unsigned long ulCRC = 0;
	while (ulCount-- != 0)
	{
		ulTemp1 = (ulCRC >> 8) & 0x00FFFFFFL;
		ulTemp2 = CRC32Value(((int)ulCRC ^ *ucBuffer++) & 0xff);
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	return(ulCRC);
}