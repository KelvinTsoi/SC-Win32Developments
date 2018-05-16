#pragma once
#pragma warning(disable:4996)

#include "SensorSerial.h"
#include "SensorQueue.h"

#define LEFT				0x01
#define RIGHT				0x02

#define CONTENT_SIZE		39

#define LEFT_INCEPTION		'L'
#define RIGHT_INCEPTION		'R'
#define END_OPERATOR		'E'

typedef int(*ProcCallBack)(char, float, float, float);

class SensorManager
{
public:

	int Process(const char *COMx, const int BaudRate, ProcCallBack func);	

	void ReceiverThreadProcCallBack();

	void PusherThreadProcCallBack();

	static SensorManager* Instance();

protected:

	SensorManager();

private:

	int Decode(float *pitch, float *roll, float *yaw, const char * buffer);

private:

	bool isRunning;

	DWORD threadIDh;

	HANDLE hThread;

	DWORD threadIDk;

	HANDLE kThread;

	ProcCallBack pcbFunc;

	static SensorManager* _instance;  /* A static pointer of Class WaveProcessorQueue used for calling back */

};
