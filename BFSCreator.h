#pragma once
#include "BaseFileSystem.h"

enum class FSType { FAT16, FAT32, exFAT, NTFS, Ext4, HFS };

class BFSCreator
{
public:
    virtual BaseFileSystem *createFileSystem(FSType fsType, const WCHAR *pFileName) = 0;
};

class ConcreteBFSCreator : public BFSCreator
{
public:
    virtual BaseFileSystem *createFileSystem(FSType fsType, const WCHAR *pFileName);
};
