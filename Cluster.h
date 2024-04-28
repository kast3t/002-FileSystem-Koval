#pragma once
#include <windows.h>
#include "BaseFileSystem.h"

enum class ClusterEnum { PNG, None };

class Cluster
{
private:
	unsigned int number;
	BaseFileSystem *fs;
	
public:
	Cluster();
	Cluster(int clusterNumber, BaseFileSystem *fsPtr);
	int getNumber();
	void readCluster(BYTE *pBuffer);
	ClusterEnum getClusterType();
};

const int maxCount = 100000;

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

template<class Type> class IteratorDecorator : public Iterator<Type>
{
protected:
	Iterator<Type> *it;
public:
	IteratorDecorator(Iterator<Type> *iterator) { it = iterator; };
	virtual ~IteratorDecorator() { delete it; };
	virtual void first() { it->first(); };
	virtual void next() { it->next(); };
	virtual bool isDone() const { return it->isDone(); };
	virtual Type getCurrent() const { return it->getCurrent(); };
};

class ClusterContainerIteratorDecorator : public IteratorDecorator<Cluster>
{
private:
	ClusterEnum targetClusterType;
public:
	ClusterContainerIteratorDecorator(Iterator<Cluster> *it, ClusterEnum clusterType) : IteratorDecorator(it) { targetClusterType = clusterType; }
	void first();
	void next();
};
