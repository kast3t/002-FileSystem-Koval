#pragma once
#include <windows.h>

class BaseFileSystem
{
private:
    unsigned int clusterSize;
    HANDLE fileHandle;

protected:
    void readBytesFromOffset(LARGE_INTEGER startOffset, BYTE *pResultBuffer, DWORD bytesToRead);
    void setClusterSize(unsigned int clSize);

public:
    virtual unsigned int getClusterSize() const;
    virtual void readClusterNumber(DWORD clusterNumber, BYTE *pResultBuffer, DWORD bytesToRead = 0) = 0;

    BaseFileSystem(const WCHAR *pFileName);
    ~BaseFileSystem();
};
