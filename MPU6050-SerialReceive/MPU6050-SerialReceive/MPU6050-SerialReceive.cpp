// MPU6050-SerialReceive.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "MPU6050-SerialReceive.h"


HANDLE Serial_open(LPCWSTR COMx, int BaudRate)
{
	HANDLE hCom;//串口设备句柄
	DCB dcb = { 0 };
	hCom = CreateFile(COMx,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		0,//FILE_FLAG_OVERLAPPED,   //同步方式 或 重叠方式   
		0
	);

	if (hCom == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		printf("Sorry, failed to open the serial\n");
		//return -1;  
		printf("The program will terminate in 3 seconds\n");
		Sleep(3000);
		exit(0);
	}
	else
		printf("The serial is successfully opened in a Baudrate %d!\n", BaudRate);

	dcb.DCBlength = sizeof(DCB);

	if (!GetCommState(hCom, &dcb))
	{
		DWORD dwError = GetLastError();
		return(HANDLE)(-1);
	}

	dcb.BaudRate = BaudRate;   //波特率   
	dcb.ByteSize = 8;          //位数   
	dcb.Parity = NOPARITY;     //奇偶检验   
	dcb.StopBits = ONESTOPBIT;  //停止位数   

	if (!SetCommState(hCom, &dcb))
	{
		DWORD dwError = GetLastError();
		return(HANDLE)(-1);
	}
	if (!PurgeComm(hCom, PURGE_RXCLEAR))   return(HANDLE)(-1);

	SetupComm(hCom, 1024, 1024);
	return hCom;
}

int Serial_read(HANDLE hCom, void* OutBuf, int size)
{
	DWORD cnt = 0;
	ReadFile(hCom, OutBuf, size, &cnt, 0);
	return cnt;

}


void Serial_close(HANDLE hCom)
{
	CloseHandle(hCom);
}

/*main.cpp*/
HANDLE hCom;
int main()
{
	hCom = Serial_open(_T("COM3"), 115200); 

	while (1)
	{
		char data = 0x00;
		Serial_read(hCom, &data, 1);
		PurgeComm(hCom, PURGE_RXABORT);
		printf("%c", data);
	}

	PurgeComm(hCom, PURGE_RXCLEAR);
	Serial_close(hCom);
	return 0;

}