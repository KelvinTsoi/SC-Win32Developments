// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MPU6050SERIALRECEIVERDYNAMICLIBRARY_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MPU6050SERIALRECEIVERDYNAMICLIBRARY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MPU6050SERIALRECEIVERDYNAMICLIBRARY_EXPORTS
#define MPU6050SERIALRECEIVERDYNAMICLIBRARY_API __declspec(dllexport)
#else
#define MPU6050SERIALRECEIVERDYNAMICLIBRARY_API __declspec(dllimport)
#endif

#include "SensorManager.h"

typedef int(*ProcCallBack)(char, float, float, float);

extern "C" MPU6050SERIALRECEIVERDYNAMICLIBRARY_API int fnMPU6050SerialReceiverDynamicLibrary(const char *COMx, const int BaudRate, ProcCallBack ProcessCallBackFunc);
