#include "BFSCreator.h"
#include "FAT32.h"

BaseFileSystem *ConcreteBFSCreator::createFileSystem(FSType fsType, const WCHAR *pFileName) {
	switch (fsType) {
		case FSType::FAT32: return new FAT32(pFileName);
		// TODO: Добавить остальные ФС после их рефакторинга
		default: return NULL;
	}
}

