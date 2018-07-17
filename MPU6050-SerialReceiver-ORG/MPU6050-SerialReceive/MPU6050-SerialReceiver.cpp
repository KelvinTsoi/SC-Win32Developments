#include "stdafx.h"

#include "MPU6050-SerialReceiver.h"

#define COMx	21

#define BAUDRATE 2000000

int ProcessCallBackFunc(char direction, float pitch, float roll, float yaw)
{
	// Do anything you want...
	if (direction == LEFT)
	{
		printf("Left pitch:%f roll:%f yaw:%f\r\n", pitch, roll, yaw);
	}
	else if (direction == RIGHT)
	{
		printf("Right pitch:%f roll:%f yaw:%f\r\n", pitch, roll, yaw);
	}

	return 0;
}

void InstanceProcess()
{
	int ret = 0;
	if ((ret = SensorManager::Instance()->Process(COMx, BAUDRATE, ProcessCallBackFunc)) != 0)
	{
		printf("Start Process Error, Error Code: %d\r\n", ret);
		exit(1);
	}
}

void InstanceKillProcess()
{
	int ret = 0;
	if ((ret = SensorManager::Instance()->KillProcess()) != 0)
	{
		printf("Kill Process Error, Error Code: %d\r\n", ret);
		exit(1);
	}
}

bool ConsoleHandler(DWORD fdwctrltype)
{
	int ret = 0;

	switch (fdwctrltype)
	{
	case CTRL_C_EVENT:
	{
		InstanceKillProcess();
		break;
	}
	case CTRL_BREAK_EVENT:
	{
		InstanceProcess();
		break;
	}
	default:
		return false;
	}

	return true;
}

int main()
{
	int ret = 0;

	if ((ret = SensorManager::Instance()->Process(COMx, BAUDRATE, ProcessCallBackFunc)) != 0)
	{
		printf("Start Process Error, Error Code: %d\r\n", ret);
		exit(1);
	}

	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, true))
	{
		while (1);
	}

	while (1);

	return 0;
}