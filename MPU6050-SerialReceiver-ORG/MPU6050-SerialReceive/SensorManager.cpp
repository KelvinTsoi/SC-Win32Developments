#include "stdafx.h"
#include "SensorManager.h"

SensorManager* SensorManager::_instance = NULL;

DWORD WINAPI ReceiverThreadProc(LPVOID lpParam)
{
	SensorManager *proxy = (SensorManager*)lpParam;
	proxy->ReceiverThreadProcCallBack();
	return 0;
}

DWORD WINAPI PusherThreadProc(LPVOID lpParam)
{
	SensorManager *proxy = (SensorManager*)lpParam;
	proxy->PusherThreadProcCallBack();
	return 0;
}

void SensorManager::ReceiverThreadProcCallBack()
{
	char TmpData = 0x00;
	char Buffer[1024] = { 0x00 };
	int RecvLength = 0;
	bool HaveNewMsg = false;
	char Direction = 0x00;
	isReceiverRunning = true;
	while (isReceiverRunning)
	{
		SensorSerial::Instance()->Read(&TmpData, 1);

		if (!HaveNewMsg)
		{
			if (TmpData == LEFT_INCEPTION || TmpData == RIGHT_INCEPTION)
			{
				HaveNewMsg = true;

				switch (TmpData)
				{
				case LEFT_INCEPTION:
					Direction = LEFT; break;
				case RIGHT_INCEPTION:
					Direction = RIGHT; break;
				default:
					Direction = 0x00; break;
				}
			}
		}
		else
		{
			if (TmpData != 'E')
			{
				Buffer[RecvLength++] = TmpData;
			}
			else
			{
				if (RecvLength == CONTENT_SIZE)
				{
					SensorData sd;
					memset(&sd, 0, sizeof(SensorData));
					Decode(&sd.pitch, &sd.roll, &sd.yaw, Buffer);
					sd.direction = Direction;
					SensorQueue::Instance()->Enqueue(sd);
					RecvLength = 0;
					memset(Buffer, 0x00, sizeof(Buffer));
					HaveNewMsg = false;
				}
				else
				{
					RecvLength = 0;
					Direction = 0x00;
					memset(Buffer, 0x00, sizeof(Buffer));
					HaveNewMsg = false;
				}
			}
		}
	}
	return;
}

void SensorManager::PusherThreadProcCallBack()
{
	SensorData *res;
	isPusherRunning = true;
	while (isPusherRunning)
	{
		res = NULL;
		if ((res = SensorQueue::Instance()->Dequeue()) != NULL)
		{
			(*pcbFunc)(res->direction, res->pitch, res->roll, res->yaw);
		}
	}
	return;
}

SensorManager* SensorManager::Instance()
{
	if (_instance == 0)
	{
		_instance = new SensorManager();
	}
	return _instance;
}

SensorManager::SensorManager()
{
	isPusherRunning = false;
	isReceiverRunning = false;
	isInitialized = false;
}

int SensorManager::Process(const char *COMx, const int BaudRate, ProcCallBack pcb)
{
	int ret = 0;

	if (isInitialized)
	{
		printf("Sensor Manager has been initialized!\r\n");
		return 1;
	}

	if ((ret = SensorSerial::Instance()->Start(COMx, BaudRate)) != 0)
	{
		printf("Sensor Serial Start Failed, Error code: %d\r\n", ret);
		return 2;
	}

	if (!(hThread = CreateThread(NULL, 0, ReceiverThreadProc, this, 0, &threadIDh)))
	{
		DWORD dwError = GetLastError();
		printf("Create Receiver Thread Failed, Error code %d\r\n", dwError);
		return 3;
	}

	if (!(kThread = CreateThread(NULL, 0, PusherThreadProc, this, 0, &threadIDk)))
	{
		DWORD dwError = GetLastError();
		printf("Create Pusher Thread Failed, Error code %d\r\n", dwError);
		return 4;
	}

	pcbFunc = pcb;

	isInitialized = true;

	return 0;
}

int SensorManager::Decode(float *pitch, float *roll, float *yaw, const char * buffer)
{
	int ret = 0; 
	sscanf(buffer, "[%f][%f][%f]", pitch, roll, yaw);
	return 0;
}

int SensorManager::KillProcess()
{
	if (!isInitialized)
	{
		printf("Sensor Manager has not been initialized!\r\n");
		return 1;
	}

	isPusherRunning = false;
	isReceiverRunning = false;

	while (!SensorQueue::Instance()->IsEmpty())
	{
		SensorQueue::Instance()->Dequeue();
	}

	int ret = 0;

	if ((ret = SensorSerial::Instance()->Stop()) != 0)
	{
		printf("Sensor Serial Stop Failed, Error code: %d\r\n", ret);
		return 2;
	}

	isInitialized = false;

	return 0;
}