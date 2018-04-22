#pragma once

#include <stdlib.h>
#include<TCHAR.H>   
#include<windows.h>  
#include<string.h>

/**
open serial
@param COMx: eg:_T("COM1")
@param BaudRate:
return 0 success ,return Negative is haed err
*/
HANDLE Serial_open(LPCWSTR, int); 

/**
serial read
@param Buf:data buf
@param size:
@return The len of read
*/
int Serial_read(HANDLE, void*, int);

/**
serial close
*/
void Serial_close(HANDLE);