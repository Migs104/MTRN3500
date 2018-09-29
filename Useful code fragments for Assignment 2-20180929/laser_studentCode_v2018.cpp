//Either include metadata from System.dll by including it here or by going to project properties
// and then C++->Advanced->Forced #using file
//#using <System.dll>

// #include <sstream>
// #include <iostream>
// #include <Windows.h>

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::IO;
using namespace System::Text;

value struct Point
{
	double X;
	double Y;
};

ref class Laser
{
private:
	String ^ HostName;
	int PortNumber;
	int NumData;
	double StartAngle;
	double Resolution;
	TcpClient^ Client;
	NetworkStream^ Stream;
public:
	array<unsigned char>^ Data = gcnew array<unsigned char>(2048);
	array<Point>^ Ranges;
public:
	Laser() {}
	~Laser() {};
	Laser(String^ hostName, int portNumber)
	{
		HostName = hostName;
		PortNumber = portNumber;
		Client = gcnew TcpClient(HostName, PortNumber);
		// Client settings
		Client->NoDelay = true;
		Client->ReceiveBufferSize = 1024;
		Client->ReceiveTimeout = 500; //ms
		Client->SendBufferSize = 1024;
		Client->SendTimeout = 500;//ms

		Stream = Client->GetStream();

		//authenticate with the server
		// i.e. send student number and check response


	}
	array<Point>^ GetSingleScan()
	{
		String^ RequestScan = gcnew String("sRN LMDscandata");
		//String^ RequestScan = gcnew String("sMN Run");
		array<unsigned char>^ WriteBuf = gcnew array<unsigned char>(128);
		Int32 bytes;
		WriteBuf = System::Text::Encoding::ASCII->GetBytes(RequestScan);
		Stream->WriteByte(0x02); //STX
		Stream->Write(WriteBuf, 0, WriteBuf->Length);
		Stream->WriteByte(0x03); //ETX
		System::Threading::Thread::Sleep(10);
		if (Client->Available != 0)
		{
			bytes = Stream->Read(Data, 0, Data->Length);
		}
		if (bytes == 0)
			return nullptr;
		else
		{
			GetResolution();
			GetStartAngle();
			GetNumData();
			UpdateRanges();
			return Ranges;
		}
	}
private:
	void GetResolution()
	{
		Resolution = 0;//process Data to get resolution, i.e. skip 24 spaces (0x20) in Data,
					   //then fill in an Int16 byte by byte, divide result by 10000.
		
		// one way to do this is to copy data into a stringstream object
		// then read from it into a string object until you get to the data you want
		// then read in the resolution (its stored a hex value)

	}

	void GetStartAngle()
	{
		StartAngle = 45.0; // process Data to get StartAngle (skip 23 spaces)
						   // then fill in an int32 byte by byte and divide the result by 10000.

		// do similar to above
	}

	void GetNumData()
	{
		NumData = 0; // procrss Data to get NumData skip 25 spaces and
					 // then fill in an uint_16  byte by byte.

		// do similar to above
	}

	void UpdateRanges()
	{
		Ranges = gcnew array<Point>(NumData);
		// process Data to update ranges skip 26 spaces, then run a for loop
		for (int i = 0; i < NumData; i++)
		{
			Ranges[i].X = 0;// Process data to fill these values
			Ranges[i].Y = 0;// Process data to fill these values
		}
	}

};

int main()
{
	double X[361], Y[361];
	Laser^ MyLaser = gcnew Laser("192.168.1.200", 23000);
	MyLaser->GetSingleScan();
	// first data X point
	X[0] = MyLaser->Ranges[0].X;
	Y[0] = MyLaser->Ranges[0].Y;

	return 0;
}