#include "stdafx.h"
#include "tchar.h"
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
	SerialData *res;

	const int DataContentSize = 39;

	int ret = 0;

	char RecvBuffer[2048] = { 0x00 };
	int RecvLength = 0;

	bool HaveNewMsg = false;

	char Direction = 0x00;

	int TargetPosition = 0;
	int TargetInterval = 0;
	char TargetBuffer[2048] = { 0x00 };

	isReceiverRunning = true;
	while (isReceiverRunning)
	{
		res = NULL;
		if ((res = SerialQueue::Instance()->Dequeue()) != NULL)
		{
			memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
			memcpy(RecvBuffer, res->content, res->length);
			RecvLength = res->length;

			for (int i = 0; i < RecvLength; i++)
			{
				if (RecvBuffer[i] == 'L' || RecvBuffer[i] == 'R')
				{
					HaveNewMsg = true;
					TargetPosition = i + 1;
					TargetInterval = 0;

					if (RecvBuffer[i] == 'L')
					{
						Direction = 0x01;
					}
					else if(RecvBuffer[i] == 'R')
					{
						Direction = 0x02;
					}
				}
				else if (RecvBuffer[i] == 'E')
				{
					if (HaveNewMsg && (TargetInterval == DataContentSize))
					{
#if 1
						SensorData sd;
						memset(&sd, 0, sizeof(SensorData));
						sd.direction = Direction;
						ret = Decode(&sd.pitch, &sd.roll, &sd.yaw, TargetBuffer);
						if (ret == 3)
						{
							SensorQueue::Instance()->Enqueue(sd);
						}
#endif
						HaveNewMsg = false;
						memset(TargetBuffer, 0x00, sizeof(TargetBuffer));
						TargetPosition = i + 1;
						TargetInterval = 0;
					}
				}
				else
				{
					if (HaveNewMsg)
					{
						if (i <= TargetInterval)
						{
							TargetBuffer[TargetInterval] = RecvBuffer[i];
						}
						else if(i > TargetInterval)
						{
							TargetBuffer[TargetInterval] = RecvBuffer[TargetPosition + TargetInterval];
						}
						TargetInterval++;
					}
				}
			}
		}
	}
	return;

#if 0
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
#endif
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
	FirstTimeInit = true;
}

void SensorManager::OnSendMessage(unsigned char* str, int port, int str_len)
{

#if 0
	printf("Port: %d, Len: %d, Data: %s\r\n", port, str_len, str);
#endif

	SerialData tmp;
	memset(tmp.content, 0x00, sizeof(tmp.content));
	memcpy(tmp.content, (char *)str, str_len);
	tmp.length = str_len;
	SerialQueue::Instance()->Enqueue(tmp);

#if 0
	if (MsgLengthCounter < 82)
	{
		memcpy(MsgTmp+ MsgLengthCounter, (const char *)str, str_len);
		MsgLengthCounter += str_len;
	}
	else
	{
		//printf("%s\r\n", MsgTmp);
		memset(MsgTmp, 0x00, sizeof(MsgTmp));
		MsgLengthCounter = 0;
	}
#endif
}

int SensorManager::Process(const int port, const int BaudRate, ProcCallBack pcb)
{
	int ret = 0;

	if (isInitialized)
	{
		printf("Sensor Manager has been initialized!\r\n");
		return 1;
	}

	HWND m_handle = HWND_BROADCAST;

	//if (FirstTimeInit)
	{
		ret = m_SerialPort.InitPort(m_handle, port, BaudRate, 'n', 8, 1, EV_RXCHAR, 82);
		m_SerialPort.StartMonitoring();

#ifdef _SEND_DATA_WITH_SIGSLOT
		m_SerialPort.sendMessageSignal.connect(this, &SensorManager::OnSendMessage);
#endif
	}
#if 0
	else
	{
		m_SerialPort.ResumeMonitoring();
	}
#endif

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

	FirstTimeInit = false;

	return 0;
}

int SensorManager::Decode(float *pitch, float *roll, float *yaw, const char * buffer)
{
	int ret = 0; 
	ret = sscanf(buffer, "[%f][%f][%f]", pitch, roll, yaw);
	return ret;
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

	while (!SerialQueue::Instance()->IsEmpty())
	{
		SerialQueue::Instance()->Dequeue();
	}

	int ret = 0;

	m_SerialPort.ClosePort();

#if 0
	if ((ret = SensorSerial::Instance()->Stop()) != 0)
	{
		printf("Sensor Serial Stop Failed, Error code: %d\r\n", ret);
		return 2;
	}
#endif


	isInitialized = false;

	return 0;
}