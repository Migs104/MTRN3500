#include <iostream>
#include <Windows.h>
#include <XInput.h>
#include "Controller.h"

using namespace std;

//Constructors
controller::controller() {

}

controller::controller(int index) {
	if (index > 3) {
		cout << "Index must be a number from 0-3" << endl;
	} else {
		Index = index;
	}
}

//Getters
void controller::getState(int index) {
	XInputGetState(index, &state);
}

void controller::getBattery(int index) {
	XInputGetBatteryInformation(index, BATTERY_DEVTYPE_GAMEPAD, &battery);

	if (battery.BatteryType != BATTERY_TYPE_WIRED) {
		if (battery.BatteryLevel == BATTERY_LEVEL_EMPTY) {
			cout << "Battery level empty" << endl;
		}
		else if (battery.BatteryLevel == BATTERY_LEVEL_LOW) {
			cout << "Battery level low" << endl;
		}
		else if (battery.BatteryLevel == BATTERY_LEVEL_MEDIUM) {
			cout << "Battery level medium" << endl;
		}
		else {
			cout << "Battery level full" << endl;
		}
	}
	else {
		cout << "Controller is wired" << endl;
	}
}

int controller::getConnect(int index) {

	if (XInputGetState(index, &state) == ERROR_DEVICE_NOT_CONNECTED) {
		cout << "Controller " << index << " is not connected." << endl;
	} else {
		cout << "Controller " << index << " is connected." << endl;
	}
	return XInputGetState(index, &state);
}

int controller::getIndex() {
	cout << "Index is " << Index << endl;
	return Index;
}

//setters
void controller::setIndex(int index) {
	if (index > -1 && index < 4) {
		Index = index;
	} else {
		cout << "Index must be a number from 0-3." << endl;
	}
}

void controller::setVibrate(int index, WORD left, WORD right) {
	if (XInputGetState(index, &state) == ERROR_DEVICE_NOT_CONNECTED) {
		cout << "Controller " << index << " is not connected." << endl;
	}
	else {
		vibration.wLeftMotorSpeed = left;
		vibration.wRightMotorSpeed = right;
	}
	XInputSetState(index, &vibration);
}

void controller::setDeadzone (int size) {
	Deadzone = size;
}

//Class-specific Functions
void controller::stopVibrate(int index) {
	controller(index).setVibrate(index, 0, 0);
}

int controller::checkLeftTrigger(int index) {
	XInputGetState(index, &state);
	return state.Gamepad.bLeftTrigger;
}

int controller::checkRightTrigger(int index) {
	XInputGetState(index, &state);
	return state.Gamepad.bRightTrigger;
}

int controller::checkA(int index) {
	XInputGetState(index, &state);
		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			return 1;
		} else {
			return 0;
		}
}

int controller::checkB(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkX(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkY(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkUp(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkDown(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkLeft(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkRight(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkStart(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkBack(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkLthumb(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkRthumb(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkLshoulder(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkRshoulder(int index) {
	XInputGetState(index, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		return 1;
	}
	else {
		return 0;
	}
}

int controller::checkLX(int index) {
	XInputGetState(index, &state);
	if (radiusDead() > radius(state.Gamepad.sThumbLX, state.Gamepad.sThumbLY)) {
		return 0;
	} else {
		return state.Gamepad.sThumbLX;
	}
}

int controller::checkLY(int index) {
	XInputGetState(index, &state);
	if (radiusDead() > radius(state.Gamepad.sThumbLX, state.Gamepad.sThumbLY)) {
		return 0;
	}
	else {
		return state.Gamepad.sThumbLY;
	}
}

int controller::checkRX(int index) {
	XInputGetState(index, &state);
	if (radiusDead() > radius(state.Gamepad.sThumbRX, state.Gamepad.sThumbRY)) {
		return 0;
	}
	else {
		return state.Gamepad.sThumbRX;
	}
}

int controller::checkRY(int index) {
	XInputGetState(index, &state);
	if (radiusDead() > radius(state.Gamepad.sThumbRY, state.Gamepad.sThumbRY)) {
		return 0;
	}
	else {
		return state.Gamepad.sThumbRY;
	}
}

int controller::radiusDead() {
	int a = sqrt(2 * Deadzone*Deadzone);
	return a;
}

int controller::radius(short x, short y) {
	int a = sqrt(x*x + y*y);
	return a;
}

controller::~controller() {

}