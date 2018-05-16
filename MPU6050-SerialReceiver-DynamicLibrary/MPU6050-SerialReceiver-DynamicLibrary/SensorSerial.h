#pragma once

#include <stdlib.h>
#include<TCHAR.H>   
#include<windows.h>  
#include<string.h>
#include<stdio.h>

class SensorSerial
{

public:

	int Start(const char * COMx, const int BaudRate);

	int Stop(void);

	int Read(void* buffer, int size);

	/**
	* Summary: Singleton Pattern
	* Return: Return a static pointer of Class PacketManager
	*/
	static SensorSerial* Instance();

protected:

	/**
	* Summary: Constructor
	*/
	SensorSerial();

private:

	static SensorSerial* _instance;    /* A static pointer of Class SensorSerial used for Singleton Pattern */

	HANDLE hCom;

	DCB dcb;

};

