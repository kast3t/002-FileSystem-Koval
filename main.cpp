#include <iostream>
#include <iomanip>
#include "BFSCreator.h"
#include "Cluster.h"

void printBuffer(unsigned int clusterSize, BYTE *pBuffer) {
	for (int i = 0; i < clusterSize; i++) {
		if (i % 16 == 0) {
			std::cout << std::endl;
		}
		if (i % 8 == 0 && i % 16 != 0 && i != 0) {
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
		
		// Добавляем первые 4 кластера (с 2 по 5)
		for (int i = 2; i < 6; i++) {
			clustersFAT32.addCluster(Cluster(i, fs));
		}

		Iterator<Cluster> *iterator = clustersFAT32.getIterator();

		for (iterator->first(); !iterator->isDone(); iterator->next()) {
			Cluster currentCluster = iterator->getCurrent();

			// Выделяем память для буфера, куда запишутся данные кластера
			BYTE *pBuffer = new BYTE[clusterSize];
			currentCluster.readCluster(pBuffer);

			std::cout << std::dec << "Кластер №" << currentCluster.getNumber() << ". Чтобы увидеть данные, нажмите Enter:";
			std::cin.get();
			printBuffer(clusterSize, pBuffer);
			std::cout << std::endl << std::endl;

			delete[] pBuffer;
		}
		delete iterator;

		// Добавляем ещё кластеров (с 6 по 10000)
		for (int i = 6; i < 10001; i++) {
			clustersFAT32.addCluster(Cluster(i, fs));
		}

		// Итератор только для кластеров с сигнатурой PNG
		Iterator<Cluster> *iteratorPNG = new ClusterContainerIteratorDecorator(clustersFAT32.getIterator(), ClusterEnum::PNG);
		
		for (iteratorPNG->first(); !iteratorPNG->isDone(); iteratorPNG->next())	{
			Cluster currentCluster = iteratorPNG->getCurrent();
			
			// Выделяем память для буфера, куда запишутся данные кластера
			BYTE *pBuffer = new BYTE[clusterSize];
			currentCluster.readCluster(pBuffer);

			std::cout << std::dec << "Кластер №" << currentCluster.getNumber() << " содержит PNG сигнатуру. Чтобы увидеть данные, нажмите Enter:";
			std::cin.get();
			printBuffer(clusterSize, pBuffer);
			std::cout << std::endl << std::endl;

			delete[] pBuffer;
		}
		delete iteratorPNG;

		delete[] fsCreator;
	}
	catch (const char *errorMessage) {
		std::cout << errorMessage << std::endl;
		return -1;
	}
}
