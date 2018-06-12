#pragma once
#include <string>
#include <exception>
#include "KernelProto.hpp"
#include "Game.hpp"

class KernelReader
{
public:
	KernelReader() : hDriver(0), hGameBase(0)
	{
		hDriver = CreateFileA("\\\\.\\dnsqa", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
		if (hDriver == INVALID_HANDLE_VALUE)
		{
			throw std::runtime_error("Could not open the driver handle!");
		}

	}

	~KernelReader()
	{
		CloseHandle(hDriver);
	}

	/*
	* CLASS METHODS
	*/
	int64_t readGameBase()
	{
		int64_t base = 0;

		void* read = nullptr;

		// create a struct that requests the base address
		dataPack pack{ (uint64_t)&base, (uint64_t)read, sizeof(int64_t), (uint32_t)GetCurrentProcessId(), 0, TRUE, 1 };
		// send the struct to IOCTL
		WriteFile(hDriver, (LPCVOID)&pack, sizeof(DataPack), NULL, NULL);

		if (base)
		{
			hGameBase = base;
		}

		return base;
	}

	template<typename T>
	T readMemByType(const int64_t& offset, const KERNEL_MSG& kernelmsg) const
	{
		T buffer;

		if (kernelmsg == READ)
		{
			dataPack pack{ (uint64_t)&buffer, (uint64_t)offset, sizeof(T), (uint32_t)GetCurrentProcessId(), 0, TRUE, 0 };
			// send the struct to IOCTL
			WriteFile(hDriver, (LPCVOID)&pack, sizeof(DataPack), NULL, NULL);
		}

		return buffer;
	}

	// caution: if you use this method, you need to delete the allocated heap from where ever you called this method from
	byte* readMemBySize(const int64_t& offset, const int32_t& sizetoread, const  KERNEL_MSG& kernelmsg) const
	{
		byte* buffer = new byte[sizetoread];


		if (kernelmsg == READ)
		{
			dataPack pack{ (uint64_t)buffer, (uint64_t)offset, sizetoread - 2, (uint32_t)GetCurrentProcessId(), 0, TRUE, 0 };
			// send the struct to IOCTL
			WriteFile(hDriver, (LPCVOID)&pack, sizeof(DataPack), NULL, NULL);
		}

		if (*buffer == 0)
		{
			delete buffer;
			return NULL;
		}

		return buffer;
	}

	Position readPosition(const int64_t& offset, const  KERNEL_MSG& kernelmsg) const
	{
		Position buffer;

		if (kernelmsg == READ)
		{
			dataPack pack{ (uint64_t)&buffer, (uint64_t)offset, sizeof(Position), (uint32_t)GetCurrentProcessId(), 0, TRUE, 0 };

			WriteFile(hDriver, (LPCVOID)&pack, sizeof(DataPack), NULL, NULL);
		}

		return buffer;
	}

	
	std::string getGNameFromId(const int32_t& w_id) const
	{
		//int64_t GNames = readMemByType<int64_t>(hGameBase + 0x3975010, READ); //2
		int64_t GNames = readMemByType<int64_t>(hGameBase + 0x3EFE1C8, READ); // 3.5.5.6:
		int64_t singleNameChunk = readMemByType<int64_t>(GNames + (w_id / 0x4000) * 8, READ);
		int64_t singleNamePtr = readMemByType<int64_t>(singleNameChunk + 8 * (w_id % 0x4000), READ);

		char* name;
		//Delete the point after using this function
		if ((name = (char*)readMemBySize(singleNamePtr + 16, 64, READ)) != NULL)
		{
			std::string s = std::string(name);
			delete name;
			return s;
		}
		else
		{
			return std::string("FAIL");
		}
	}

//Getter Setter
	HANDLE getKernelReader() const
	{
		return hDriver;
	}

	int64_t getGameBase() 
	{
		if (hGameBase) {
			return hGameBase;
		}
		else {
			return readGameBase();
		}
	}

	bool isRuning() const
	{
		return isRun;
	}

private:
	HANDLE hDriver;
	int64_t hGameBase;
	bool isRun;
};