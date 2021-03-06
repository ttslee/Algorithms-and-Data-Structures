#pragma once
#ifndef ITEM_H
#define ITEM_H
#include "Globals.h"
struct Item
{
public:
	std::string name;
	double cost;
	Item(int i);
	~Item();
};

#endif