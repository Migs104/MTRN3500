#include <Windows.h>
#include <tchar.h>
#include <SMfcn.h>
#include <SMObject.h>


SMObject::SMObject()
{
	CreateHandle = NULL;
	AccessHandle = NULL;
	pData = NULL;
	Size = 1;
}

SMObject::SMObject(TCHAR* szname, int size)
{
	CreateHandle = NULL;
	AccessHandle = NULL;
	pData = NULL;
	szName = szname;
	Size = size;
}

SMObject::~SMObject()
{
	if (CreateHandle != NULL)
		CloseHandle(CreateHandle);
	if (AccessHandle != NULL)
		CloseHandle(AccessHandle);
	if (pData != NULL)
		UnmapViewOfFile(pData);

}

int SMObject::SMCreate()
{
	CreateHandle = SMCreateReadWrite(Size, szName);
	if (CreateHandle == NULL)
		SMCreateError = 1;
	else
		SMCreateError = 0;

	return SMCreateError;
}

int SMObject::SMAccess()
{
	// Requesting a view of Modules memory
	AccessHandle = SMOpenReadWrite(szName);
	if (AccessHandle == NULL)
		SMAccessError = 1;
	else
		SMAccessError = 0;

	// Requesting access to Modules memory
	pData = SMAccessReadWrite(AccessHandle, Size);
	if (pData == NULL)
		SMAccessError = 1;
	else
		SMAccessError = 0;

	return SMAccessError;
}
