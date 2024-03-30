#include <windows.h>
#include "BaseFileSystem.h"
#pragma once

class FAT32 : public BaseFileSystem
{
private:
    unsigned int secondClusterOffset;

public:
    virtual void readClusterNumber(DWORD clusterNumber, BYTE *pResultBuffer);

    FAT32(const WCHAR *pFileName);
    ~FAT32();
};
