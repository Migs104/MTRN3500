#include <windows.h>
#include <tchar.h>


//Code framework by A/Prof. J.Katupitiya(c)2013

HANDLE SMCreateReadWrite(int objectSize, TCHAR szName[])
{
	HANDLE h;
	h = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,      // read/write access
		0,
		objectSize,
		szName);

	return h;
}

HANDLE SMOpenReadWrite(TCHAR szName[])
{
	HANDLE h;
	h = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,
		szName);
	return h;
}

void*  SMAccessReadWrite(HANDLE openedHandle, int objectSize)
{
	void *p;
	p = MapViewOfFile(openedHandle,
		FILE_MAP_ALL_ACCESS,    // read/write permission
		0,
		0,
		objectSize);

	return p;
}

