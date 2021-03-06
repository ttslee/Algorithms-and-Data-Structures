#pragma once
#ifndef PRODUCER_H
#define PRODUCER_H
#include <queue>
#include "Item.h"
class Producer
{
private:
	std::queue<Item> *qu;
public:
	Producer(std::queue<Item> &q);
	std::queue<Item> * getQueue()
	{
		return qu;
	}
	void produce(Item & it);
	~Producer();
};

#endif