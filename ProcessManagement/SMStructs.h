#ifndef SMSTRUCTS_H
#define SMSTRUCTS_H

#pragma pack(1)
struct GPS {
	unsigned char discard[44];
	double Northing;
	double Easting;
	double Height;
	unsigned char discard2[40];
	unsigned int Checksum;
};

struct Laser {
	double x[361];
	double y[361];
	int NumPoints;
	double LaserTimeStamp;
};
#pragma pack(1)

struct Plotting {
	double X;
	double Y;
	double Heading;
	double Steering;
	double PlotTimeStamp;
};
#pragma pack(1)

struct UnitFlags {
	unsigned short PM : 1,
		Laser : 1,
		Plot : 1,
		Remote : 1,
		Simulator : 1,
		UGV : 1,
		GPS : 1,
		OpenGlView : 1,
		HLevel : 1,
		Unused : 7;
};

union ExecFlags {
	UnitFlags Flags;
	unsigned short Status;
};

struct ProcessManagement {
	ExecFlags HeartBeat;
	ExecFlags ShutDown;
	double PMTimeStamp;
};
#pragma pack(1)

struct Remote {
	double SetSteering;
	double SetSpeed;
	unsigned char UGV_SIM;
	int Terminate;
	double XBoxTimeStamp;
};
#pragma pack(1)

struct Simulator {
	double X;
	double Y;
	double Heading;
	double SimTimeStamp;
};
#pragma pack(1)

struct UGV {
	double X;
	double Y;
	double UGVTimeStamp;
};


#endif
