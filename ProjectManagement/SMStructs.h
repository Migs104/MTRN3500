#ifndef SMSTRUCTS_H
#define SMSTRUCTS_H

struct GPS {
	double Northing;
	double Easting;
	double Height;
	double Heading;
	unsigned char GPSQuality;
	double GPSTimeStamp;
};

struct Laser {
	double x[1100];
	double y[1100];
	int NumPoints;
	double LaserTimeStamp;
};

struct Plotting {
	double X;
	double Y;
	double Heading;
	double Steering;
	double PlotTimeStamp;
};

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

struct Remote {
	double SetSteering;
	double SetSpeed;
	unsigned char UGV_SIM;
	int Terminate;
	double XBoxTimeStamp;
};

struct Simulator {
	double X;
	double Y;
	double Heading;
	double SimTimeStamp;
};

struct UGV {
	double X;
	double Y;
	double UGVTimeStamp;
};

struct Laser{
	double X[10000];
	double Y[10000];
};

#endif
