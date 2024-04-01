#pragma once
#include "BaseFileSystem.h"

class FAT32 : public BaseFileSystem
{
private:
    unsigned int secondClusterOffset;

public:
    virtual void readClusterNumber(DWORD clusterNumber, BYTE *pResultBuffer);

    FAT32(const WCHAR *pFileName);
    ~FAT32();
};
