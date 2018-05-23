// MPU6050-SerialReceiver-DynamicLibrary-Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>

#define COMx	"COM4"

#define TARGET_PATH			"MPU6050-SerialReceiver-DynamicLibrary.dll"

#define TARGET_PROCESS		"?fnMPU6050SerialReceiverDynamicLibraryProcess@@YAHPBDHP6AHDMMM@Z@Z"

#define TARGET_KILLPROCESS	"?fnMPU6050SerialReceiverDynamicLibraryKillProcess@@YAHXZ"

typedef int(*ProcCallBack)(char, float, float, float);

typedef int(*TYPE_fnDllProcess) (const char *, int, ProcCallBack);

typedef int(*TYPE_fnDllKillProcess) (void);

TYPE_fnDllProcess fnDllProcess;

TYPE_fnDllKillProcess fnDllKillProcess;

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

bool ConsoleHandler(DWORD fdwctrltype)
{
	int ret = 0;

	switch (fdwctrltype)
	{
	case CTRL_C_EVENT:
	{
		if ((ret = fnDllKillProcess()) != 0)
		{
			printf("Kill Process Error, Error Code: %d\r\n", ret);
			exit(1);
		}
		break;
	}
	case CTRL_BREAK_EVENT:
	{
		if ((ret = fnDllProcess(COMx, 115200, ProcessCallBackFunc)) != 0)
		{
			printf("Start Process Error, Error Code: %d\r\n", ret);
			exit(1);
		}
		break;
	}
	default:
		return false;
	}

	return true;
}


int main()
{
	HMODULE hModule = LoadLibrary(TARGET_PATH);
	if (hModule == NULL || hModule == INVALID_HANDLE_VALUE)
	{
		printf("Load Library Failed\r\n");
		exit(1);
	}

	fnDllProcess = (TYPE_fnDllProcess)GetProcAddress(hModule, TARGET_PROCESS);

	fnDllKillProcess = (TYPE_fnDllKillProcess)GetProcAddress(hModule, TARGET_KILLPROCESS);

	if (!fnDllProcess || !fnDllKillProcess)
	{
		printf("Load Function Failed\r\n");
		exit(2);
	}

	int ret = 0;
	if ((ret = fnDllProcess(COMx, 115200, ProcessCallBackFunc)) != 0)
	{
		printf("Start Process Error, Error Code: %d\r\n", ret);
		exit(1);
	}

	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, true))
	{
		while (1);
	}

	exit(0);
}