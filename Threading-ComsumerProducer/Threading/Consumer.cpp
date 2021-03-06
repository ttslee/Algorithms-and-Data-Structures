#include "Consumer.h"



Consumer::Consumer(std::queue<Item> &q) : qu(&q)
{
}

void Consumer::consume()
{
	std::cout << qu->front().name << "\t" << qu->front().cost << std::endl;
	qu->pop();
}

Consumer::~Consumer()
{
}
