#include "FAT32.h"

FAT32::FAT32(const WCHAR *pFileName) : BaseFileSystem(pFileName) {
	LARGE_INTEGER startOffset;
	startOffset.QuadPart = 0;

	// Читаем 1-й сектор
	BYTE *pBootRecordBuffer = new BYTE[512];
	readBytesFromOffset(startOffset, pBootRecordBuffer, 512);

	// Вытаскиваем нужные значения, считаем и записываем размер кластера
	int bytesPerSector = pBootRecordBuffer[11] | pBootRecordBuffer[12] << 8;
	int sectorsPerCluster = pBootRecordBuffer[13];
	int clusterSize = bytesPerSector * sectorsPerCluster;
	setClusterSize(clusterSize);

	// Вытаскиваем нужные значения, считаем и записываем смещение 2-го кластера
	int initialSector = pBootRecordBuffer[14] | pBootRecordBuffer[15] << 8;
	int countOfFAT = pBootRecordBuffer[16];
	int sizeOfFAT = pBootRecordBuffer[36] | pBootRecordBuffer[37] << 8 | pBootRecordBuffer[38] << 16 | pBootRecordBuffer[39] << 24;
	secondClusterOffset = (initialSector + countOfFAT * sizeOfFAT) * 512;

	delete[] pBootRecordBuffer;
}

FAT32::~FAT32() {}

void FAT32::readClusterNumber(DWORD clusterNumber, BYTE *pResultBuffer, DWORD bytesToRead = 0) {
	DWORD clusterSize = getClusterSize();
	if (clusterSize == 0) {
		throw "Ошибка чтения кластера: размер кластера не определён";
	}

	if (clusterNumber < 2) {
		throw "Ошибка: номер кластера должен быть не менее 2";
	}

	LARGE_INTEGER startOffset;
	startOffset.QuadPart = secondClusterOffset + (clusterNumber - 2) * clusterSize;

	if (bytesToRead == 0) {
		readBytesFromOffset(startOffset, pResultBuffer, clusterSize);
	}
	else {
		readBytesFromOffset(startOffset, pResultBuffer, bytesToRead);
	}
}
