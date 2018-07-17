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

// if you need this dll for C++ Application, then Set this value 1, else Set this value 0
#define USE_FOR_C_PLUS_PLUS 0

typedef int(*ProcCallBack)(char, float, float, float);

#if USE_FOR_C_PLUS_PLUS
MPU6050SERIALRECEIVERDYNAMICLIBRARY_API int fnMPU6050SerialReceiverDynamicLibraryProcess(const int COMx, const int BaudRate, ProcCallBack ProcessCallBackFunc);
MPU6050SERIALRECEIVERDYNAMICLIBRARY_API int fnMPU6050SerialReceiverDynamicLibraryKillProcess(void);
#else
extern "C" MPU6050SERIALRECEIVERDYNAMICLIBRARY_API int fnMPU6050SerialReceiverDynamicLibraryProcess(const int COMx, const int BaudRate, ProcCallBack ProcessCallBackFunc);
extern "C" MPU6050SERIALRECEIVERDYNAMICLIBRARY_API int fnMPU6050SerialReceiverDynamicLibraryKillProcess(void);
#endif


