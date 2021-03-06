#pragma once
#ifndef CONSUMER_H
#define CONSUMER_H
#include "Item.h"
#include <queue>
class Consumer
{
private:
	std::queue<Item>* qu;
public:
	Consumer(std::queue<Item> & q);
	std::queue<Item> *getQueue()
	{
		return qu;
	}
	void consume();
	~Consumer();
};

#endif