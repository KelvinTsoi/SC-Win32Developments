// MPU6050-SerialReceiver-DynamicLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MPU6050-SerialReceiver-DynamicLibrary.h"

MPU6050SERIALRECEIVERDYNAMICLIBRARY_API int fnMPU6050SerialReceiverDynamicLibraryProcess(const int COMx, const int BaudRate, ProcCallBack ProcessCallBackFunc)
{
	int ret = 0;

	if ((ret = SensorManager::Instance()->Process(COMx, BaudRate, ProcessCallBackFunc)) != 0)
	{
		printf("Process Error, Error Code: %d\r\n", ret);
	}

	return ret;
}

MPU6050SERIALRECEIVERDYNAMICLIBRARY_API int fnMPU6050SerialReceiverDynamicLibraryKillProcess(void)
{
	int ret = 0;

	if ((ret = SensorManager::Instance()->KillProcess()) != 0)
	{
		printf("KillProcess Error, Error Code: %d\r\n", ret);
	}

	return ret;
}