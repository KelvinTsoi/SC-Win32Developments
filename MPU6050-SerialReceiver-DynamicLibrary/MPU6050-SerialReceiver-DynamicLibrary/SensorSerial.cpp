#include "stdafx.h"
#include "SensorSerial.h"

SensorSerial* SensorSerial::_instance = NULL;

SensorSerial* SensorSerial::Instance()
{
	if (_instance == 0)
	{
		_instance = new SensorSerial();
	}
	return _instance;
}

SensorSerial::SensorSerial()
{
	hCom = NULL;
	memset(&dcb, 0, sizeof(dcb));
}

int SensorSerial::Start(const char * COMx, const int BaudRate)
{
	if (hCom != NULL)
	{
		return 1;
	}

	hCom = CreateFile(COMx,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		0,
		0
	);

	if (hCom == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		printf("Failed to open the serial, Error code %d\r\n", dwError);
		Sleep(500);
		return 2;
	}

	dcb.DCBlength = sizeof(DCB);

	if (!GetCommState(hCom, &dcb))
	{
		DWORD dwError = GetLastError();
		printf("Getting serial port Failed\r\n", dwError);
		return 3;
	}

	dcb.BaudRate = BaudRate;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	if (!SetCommState(hCom, &dcb))
	{
		DWORD dwError = GetLastError();
		printf("Setting up serial port Failed\r\n", dwError);
		return 4;
	}

	if (!PurgeComm(hCom, PURGE_RXCLEAR))
	{
		DWORD dwError = GetLastError();
		printf("Clean up serial port buffer Failed\r\n", dwError);
		return 5;
	}

	SetupComm(hCom, 1024, 1024);

	return 0;
}

int SensorSerial::Stop(void)
{
	if (!PurgeComm(hCom, PURGE_RXCLEAR))
	{
		DWORD dwError = GetLastError();
		printf("Clean up serial port buffer Failed\r\n", dwError);
		return 1;
	}

	if (!CloseHandle(hCom))
	{
		DWORD dwError = GetLastError();
		printf("Close serial handle Failed\r\n", dwError);
		return 1;
	}
	return 0;
}

int SensorSerial::Read(void* buffer, int size)
{
	DWORD cnt = 0;
	ReadFile(hCom, buffer, size, &cnt, 0);
	return cnt;
}
