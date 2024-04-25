#include <iostream>
#include <iomanip>
#include "BFSCreator.h"
#include "Cluster.h"

void printBuffer(unsigned int clusterSize, BYTE *pBuffer)
{
	for (int i = 0; i < clusterSize; i++)
	{
		if (i % 16 == 0)
		{
			std::cout << std::endl;
		}
		if (i % 8 == 0 && i % 16 != 0 && i != 0)
		{
			std::cout << " ";
		}
		std::cout << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (0xff & (int)pBuffer[i]) << " ";
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	const WCHAR *pFileName = L"\\\\?\\F:";

	try {
		FSType fsType = FSType::FAT32;

		BFSCreator *fsCreator = new ConcreteBFSCreator;
		BaseFileSystem *fs = fsCreator->createFileSystem(fsType, pFileName);
		unsigned int clusterSize = fs->getClusterSize();

		std::cout << "Размер кластера: " << clusterSize << std::endl << std::endl;

		ClusterContainer clustersFAT32;

		for (int i = 2; i < 5; i++) // Добавляем первые 15 кластеров (с 2 по 16)
		{
			clustersFAT32.addCluster(Cluster(i, pFileName));
		}

		Iterator<Cluster> *iterator = clustersFAT32.getIterator();

		for (iterator->first(); !iterator->isDone(); iterator->next())
		{
			Cluster currentCluster = iterator->getCurrent();

			// Выделяем память для буфера, куда запишутся данные кластера
			BYTE *pBuffer = new BYTE[clusterSize];
			currentCluster.readCluster(fs, pBuffer);

			std::cout << "Кластер №" << currentCluster.getNumber() << ", содержимое:" << std::endl;
			printBuffer(clusterSize, pBuffer);
			std::cout << std::endl;
			std::cin.get();

			// Освобождаем буфер
			delete[] pBuffer;
		}

		delete iterator;
		delete[] fsCreator;
	}
	catch (const char *errorMessage) {
		std::cout << errorMessage << std::endl;
		return -1;
	}
}
