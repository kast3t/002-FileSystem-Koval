#include <iostream>
#include "FAT32.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	const WCHAR *pFileName = L"\\\\?\\F:";

	try {
		FAT32 fat32 = FAT32(pFileName);
		std::cout << "Размер кластера: " << fat32.getClusterSize() << std::endl;

		BYTE *pBuffer = new BYTE[fat32.getClusterSize()];
		fat32.readClusterNumber(2, pBuffer);

		std::cout << "Кластер успешно прочитан! Часть буфера (до нуль-байта): " << pBuffer;
		std::cin.get();

		delete[] pBuffer;
	}
	catch (const char *errorMessage) {
		std::cout << errorMessage << std::endl;
		return -1;
	}
}
