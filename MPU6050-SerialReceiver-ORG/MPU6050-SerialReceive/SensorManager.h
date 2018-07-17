#pragma once
#pragma warning(disable:4996)

#include "SensorSerial.h"
#include "CSerialPort.h"
#include "SensorQueue.h"
#include "SerialQueue.h"

#define LEFT				0x01
#define RIGHT				0x02

#define CONTENT_SIZE		39

#define LEFT_INCEPTION		'L'
#define RIGHT_INCEPTION		'R'
#define END_OPERATOR		'E'

typedef int(*ProcCallBack)(char, float, float, float);

class SensorManager : public has_slots<>
{
public:

	int Process(const int port, const int BaudRate, ProcCallBack func);	

	int KillProcess();

	void ReceiverThreadProcCallBack();

	void PusherThreadProcCallBack();

	static SensorManager* Instance();

	void OnSendMessage(unsigned char* str, int port, int str_len);

protected:

	SensorManager();

private:

	int Decode(float *pitch, float *roll, float *yaw, const char * buffer);

private:

	bool isReceiverRunning;

	bool isPusherRunning;

	bool isInitialized;

	DWORD threadIDh;

	HANDLE hThread;

	DWORD threadIDk;

	HANDLE kThread;

	CSerialPort m_SerialPort;

	ProcCallBack pcbFunc;

	static SensorManager* _instance;  /* A static pointer of Class WaveProcessorQueue used for calling back */

};
