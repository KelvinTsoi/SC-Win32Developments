#include "stdafx.h"

#include "MPU6050-SerialReceiver.h"

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

int main()
{
	int ret = 0;

	if ((ret = SensorManager::Instance()->Process("COM4", 115200, ProcessCallBackFunc)) != 0)
	{
		printf("System Error, Error Code: %d\r\n", ret);
		exit(1);
	}

	while (1);

	return 0;
}