#include <iostream>
#include <Windows.h>
#include <XInput.h>
#include "Controller.h"

using namespace std;

int main() {

	bool End = false;
	XInputEnable(true);

	while (End == false) {

		if (controller(0).getConnect(0) != ERROR_DEVICE_NOT_CONNECTED) {
			controller(0).setVibrate(0, 65000, 65000);
			Sleep(1000);
			controller(0).stopVibrate(0);
			Sleep(1000);
		} if (controller(0).checkA(0) == 1 & controller(0).checkB(0) == 1 & controller(0).checkX(0) == 1 & controller(0).checkY(0) == 1) {
			End = true;
		} if (controller(0).checkUp(0) == 1) {
			controller(0).setDeadzone(1000);
		} if (controller(0).checkDown(0) == 1) {
			controller(0).setDeadzone(0);
		}
		cout << controller(0).checkRX(0) << " " << controller(0).checkRY(0) << endl;

	}
}