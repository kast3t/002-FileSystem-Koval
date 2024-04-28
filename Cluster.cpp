#include "Cluster.h"

Cluster::Cluster() {
	number = 0;
	fs = NULL;
}

Cluster::Cluster(int clusterNumber, BaseFileSystem *fsPtr) {
	number = clusterNumber;
	fs = fsPtr;
}

int Cluster::getNumber() {
	return number;
}

void Cluster::readCluster(BYTE *pBuffer) {
	fs->readClusterNumber(number, pBuffer);
}

ClusterEnum Cluster::getClusterType() {
	// Сигнатуры файлов
	BYTE PNG[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

	BYTE *pBuffer = new BYTE[512];
	fs->readClusterNumber(number, pBuffer, 512);
	
	if (memcmp(pBuffer, PNG, 8) == 0) {
		return ClusterEnum::PNG;
	}
	
	delete[] pBuffer;
	return ClusterEnum::None;
}

ClusterContainer::ClusterContainer() {
	clusters = new Cluster[maxCount];
	count = 0;
}

void ClusterContainer::addCluster(const Cluster &newCluster) {
	clusters[count] = newCluster;
	count++;
}

int ClusterContainer::getCount() const {
	return count;
}

Cluster ClusterContainer::getByIndex(int clusterIndex) const {
	return clusters[clusterIndex];
}

Iterator<Cluster> *ClusterContainer::getIterator() {
	return new ClusterContainerIterator(clusters, count);
}

ClusterContainerIterator::ClusterContainerIterator(const Cluster *containerItems, int clustersCount) {
	clusters = containerItems;
	count = clustersCount;
	position = 0;
}

void ClusterContainerIterator::first() {
	position = 0;
}

void ClusterContainerIterator::next() {
	position++;
}

bool ClusterContainerIterator::isDone() const {
	return position >= count;
}

Cluster ClusterContainerIterator::getCurrent() const {
	return clusters[position];
}

void ClusterContainerIteratorDecorator::first() {
	if (it->getCurrent().getClusterType() != targetClusterType) {
		next();
	}
}

void ClusterContainerIteratorDecorator::next() {
	do {
		it->next();
	} while (!it->isDone() && it->getCurrent().getClusterType() != targetClusterType);
}
