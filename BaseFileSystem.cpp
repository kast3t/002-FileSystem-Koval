#include "BaseFileSystem.h"

BaseFileSystem::BaseFileSystem(const WCHAR *pFileName) {
	clusterSize = 0;
	fileHandle = INVALID_HANDLE_VALUE;

	HANDLE fh = CreateFileW(pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE) {
		CloseHandle(fh);
		throw "Ошибка инициализации fileHandle";
	}

	fileHandle = fh;
}

BaseFileSystem::~BaseFileSystem() {
	CloseHandle(fileHandle);
}

void BaseFileSystem::readBytesFromOffset(LARGE_INTEGER startOffset, BYTE *pResultBuffer, DWORD bytesToRead) {
	unsigned long filePointer = SetFilePointer(fileHandle, startOffset.LowPart, &startOffset.HighPart, FILE_BEGIN);
	if (filePointer == INVALID_SET_FILE_POINTER) {
		throw "Ошибка установки filePointer";
	}

	DWORD bytesRead;
	bool readResult = ReadFile(fileHandle, pResultBuffer, bytesToRead, &bytesRead, NULL);
	if (!readResult || bytesRead != bytesToRead) {
		throw "Ошибка чтения readResult";
	}
}

void BaseFileSystem::setClusterSize(unsigned int clSize) {
	clusterSize = clSize;
}

unsigned int BaseFileSystem::getClusterSize() const {
	return clusterSize;
}
