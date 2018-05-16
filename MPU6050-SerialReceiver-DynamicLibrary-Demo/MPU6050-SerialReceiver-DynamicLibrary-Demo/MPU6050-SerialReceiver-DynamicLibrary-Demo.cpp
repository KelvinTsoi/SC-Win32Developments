// MPU6050-SerialReceiver-DynamicLibrary-Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>

#define TARGET_PATH		"MPU6050-SerialReceiver-DynamicLibrary.dll"

#define TARGET_LPPROCNAME	"?fnMPU6050SerialReceiverDynamicLibrary@@YAHPBDHP6AHDMMM@Z@Z"

typedef int(*ProcCallBack)(char, float, float, float);

typedef int(*TYPE_fnDllDemo) (const char *, int, ProcCallBack);

int ProcessCallBackFunc(char direction, float pitch, float roll, float yaw)
{
	// For exmaple: 
	if (direction == 1)
	{
		printf("Left pitch:%f roll:%f yaw:%f\r\n", pitch, roll, yaw);
	}
	else if (direction == 2)
	{
		printf("Right pitch:%f roll:%f yaw:%f\r\n", pitch, roll, yaw);
	}
	// eg end.

	// Do anything you want.
	// ...

	return 0;
}

int main()
{
	HMODULE hModule = LoadLibrary(TARGET_PATH);

	if (hModule == NULL || hModule == INVALID_HANDLE_VALUE)
	{
		printf("Load Library Failed\r\n");
		exit(1);
	}

	TYPE_fnDllDemo fnDllDemo = (TYPE_fnDllDemo)GetProcAddress(hModule, TARGET_LPPROCNAME);
	
	if (!fnDllDemo)
	{
		printf("Load Function Failed\r\n");
		exit(2);
	}

	// Start all process.
	fnDllDemo("COM4", 115200, ProcessCallBackFunc);
	
	// Keep alive.
	while (1);

	exit(0);
}

