#include "Cluster.h"

Cluster::Cluster()
{
	number = 0;
	pFileName = NULL;
}

Cluster::Cluster(int clusterNumber, const WCHAR *pFileName)
{
	number = clusterNumber;
	pFileName = pFileName;
}

int Cluster::getNumber()
{
	return number;
}

void Cluster::readCluster(BaseFileSystem *fs, BYTE *pBuffer)
{
	fs->readClusterNumber(number, pBuffer);
}

ClusterContainer::ClusterContainer()
{
	clusters = new Cluster[maxCount];
	count = 0;
}

void ClusterContainer::addCluster(const Cluster &newCluster)
{
	clusters[count] = newCluster;
	count++;
}

int ClusterContainer::getCount() const
{
	return count;
}

Cluster ClusterContainer::getByIndex(int clusterIndex) const
{
	return clusters[clusterIndex];
}

Iterator<Cluster> *ClusterContainer::getIterator()
{
	return new ClusterContainerIterator(clusters, count);
}

ClusterContainerIterator::ClusterContainerIterator(const Cluster *containerItems, int clustersCount)
{
	clusters = containerItems;
	count = clustersCount;
	position = 0;
}

void ClusterContainerIterator::first()
{
	position = 0;
}

void ClusterContainerIterator::next()
{
	position++;
}

bool ClusterContainerIterator::isDone() const
{
	return position >= count;
}

Cluster ClusterContainerIterator::getCurrent() const
{
	return clusters[position];
}
