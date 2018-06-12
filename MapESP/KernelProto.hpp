#pragma once

#include <Windows.h>

//和内核一致定义类型
using byte = BYTE;
using uint64_t = ULONGLONG;
using int64_t = LONGLONG;
using uint32_t = UINT32;
using int32_t = INT32;
using uint16_t = USHORT;
using int16_t = SHORT;


//定义和内核沟通的行为
enum KERNEL_MSG {
	READ = 0,
	BASEADD = 1
};

//定义具体数据包
typedef struct dataPack
{
	ULONGLONG UserBufferAdress;
	ULONGLONG GameAddressOffset;
	ULONGLONG ReadSize;
	ULONG     User_PID;
	ULONG     Game_PID;
	BOOLEAN   IsWrite;
	UINT32	  KernelMsg;
} DataPack, *pDataPack;