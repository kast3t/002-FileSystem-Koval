#include <iostream>
#include "BFSCreator.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	const WCHAR *pFileName = L"\\\\?\\F:";

	try {
		FSType fsType = FSType::FAT32;

		BFSCreator *fsCreator = new ConcreteBFSCreator;
		BaseFileSystem *fs = fsCreator->createFileSystem(fsType, pFileName);

		std::cout << "Размер кластера: " << fs->getClusterSize() << std::endl;

		BYTE *pBuffer = new BYTE[fs->getClusterSize()];
		fs->readClusterNumber(2, pBuffer);

		std::cout << "Кластер успешно прочитан! Часть буфера (до нуль-байта): " << pBuffer;
		std::cin.get();

		delete[] pBuffer;
		delete[] fsCreator;
	}
	catch (const char *errorMessage) {
		std::cout << errorMessage << std::endl;
		return -1;
	}
}
