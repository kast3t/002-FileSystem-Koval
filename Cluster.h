#pragma once
#include <windows.h>
#include "BaseFileSystem.h"

class Cluster
{
private:
	unsigned int number;
	
public:
	const WCHAR *pFileName;
	Cluster();
	Cluster(int clusterNumber, const WCHAR *pFileName);
	int getNumber();
	void readCluster(BaseFileSystem *fs, BYTE *pBuffer);
};

const int maxCount = 1000;

template<class Type> class Iterator
{
protected:
	Iterator() {};
public:
	virtual ~Iterator() {};
	virtual void first() = 0;
	virtual void next() = 0;
	virtual bool isDone() const = 0;
	virtual Type getCurrent() const = 0;
};

class ClusterContainer 
{
private:
	Cluster *clusters;
	int count;
public:
	ClusterContainer();
	void addCluster(const Cluster &newCluster);
	int getCount() const;
	Cluster getByIndex(int clusterIndex) const;
	Iterator<Cluster> *getIterator();
};

class ClusterContainerIterator : public Iterator<Cluster>
{
private:
	const Cluster *clusters;
	int count;
	int position;
public:
	ClusterContainerIterator(const Cluster *containerItems, int clustersCount);
	void first();
	void next();
	bool isDone() const;
	Cluster getCurrent() const;
};