// MPU6050-SerialReceiver-DynamicLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MPU6050-SerialReceiver-DynamicLibrary.h"

// This is an example of an exported function.
MPU6050SERIALRECEIVERDYNAMICLIBRARY_API int fnMPU6050SerialReceiverDynamicLibrary(const char *COMx, const int BaudRate, ProcCallBack ProcessCallBackFunc)
{
	int ret = 0;

	if ((ret = SensorManager::Instance()->Process(COMx, BaudRate, ProcessCallBackFunc)) != 0)
	{
		printf("System Error, Error Code: %d\r\n", ret);
	}

	return ret;
}