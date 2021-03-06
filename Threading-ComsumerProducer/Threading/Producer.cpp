#include "Producer.h"



Producer::Producer(std::queue<Item> & q) :qu(&q)
{
}


Producer::~Producer()
{
}

void Producer::produce(Item & it)
{
	qu->push(it);
}