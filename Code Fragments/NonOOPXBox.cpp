#include <Windows.h>
#include <iostream>
#include <xinput.h>


using namespace std;


int main()
{
	bool Quit = false;
	XINPUT_BATTERY_INFORMATION BatteryInformation;
	XINPUT_STATE State;
	XINPUT_VIBRATION Vibration;
	XInputEnable(true);
	XInputGetBatteryInformation(0,BATTERY_DEVTYPE_GAMEPAD,&BatteryInformation);
	switch(BatteryInformation.BatteryLevel)
	{
	case BATTERY_LEVEL_EMPTY : cout <<"Battery level empty " << endl;
		break;
	case BATTERY_LEVEL_LOW : cout << "Battery level low " << endl;
		break;
	case BATTERY_LEVEL_MEDIUM : cout << "Battery level medium " << endl;
		break;
	case BATTERY_LEVEL_FULL: cout << "Battery level full" << endl;
	}
	Sleep(3000);

	while(!Quit)
	{
		if(XInputGetState(0,&State) == ERROR_DEVICE_NOT_CONNECTED)
		{
			cout << "Joystick not connected " << endl;
			return 1;
		}

		if(State.Gamepad.wButtons == 1)
			Quit = true;
		if(State.Gamepad.wButtons == 32)
		{
			cout << "Vibrating    :" << "   ";
			Vibration.wLeftMotorSpeed = 0;
			Vibration.wRightMotorSpeed = 65500;
			XInputSetState(0,&Vibration);
		}
		else
		{
			cout << "Not vibrating:" << "   ";
			Vibration.wLeftMotorSpeed = 0;
			Vibration.wRightMotorSpeed = 0;
			XInputSetState(0,&Vibration);		
		}
		cout << State.Gamepad.wButtons << "   " << State.Gamepad.sThumbLX << "   " << State.Gamepad.sThumbLY << "   "<< State.Gamepad.sThumbRX << "   " << State.Gamepad.sThumbRY << "   ";
		cout << Vibration.wLeftMotorSpeed << "   " << Vibration.wRightMotorSpeed << endl;

	}
	cout << "Program terminated " << endl;

	return 0;
}