#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <XInput.h>

class controller
{
private:
	int Index;
	short Deadzone;
	XINPUT_STATE state;
	XINPUT_VIBRATION vibration;
	XINPUT_BATTERY_INFORMATION battery;
public:
	//constructors
	controller();	//default
	controller(int index);
	//getters
	void getState(int index);
	void getBattery(int index);
	int getConnect(int index);
	int getIndex();
	//setters
	void setIndex(int index);
	void setVibrate(int index, WORD left, WORD right);
	void setDeadzone(int size);
	//class specific functions
	void stopVibrate(int index);
	int checkLeftTrigger(int index);
	int checkRightTrigger(int index);
	int checkA(int index);
	int checkB(int index);
	int checkX(int index);
	int checkY(int index);
	int checkUp(int index);
	int checkDown(int index);
	int checkLeft(int index);
	int checkRight(int index);
	int checkStart(int index);
	int checkBack(int index);
	int checkLthumb(int index);
	int checkRthumb(int index);
	int checkLshoulder(int index);
	int checkRshoulder(int index);
	int checkLX(int index);
	int checkLY(int index);
	int checkRX(int index);
	int checkRY(int index);
	int radiusDead();
	int radius(short x, short y);
	//destructors
	~controller();	//default
};

#endif
