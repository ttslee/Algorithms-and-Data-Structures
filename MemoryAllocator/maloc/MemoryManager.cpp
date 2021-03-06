#include "MemoryManager.h"

#include <iostream>

namespace MemoryManager
{
	// IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT 
	//
	// This is the only static memory that you may use, no other global variables may be
	// created, if you need to save data make it fit in MM_pool
	//
	// IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
	void forwardInfo(bool*&, int*&);

	const int MM_POOL_SIZE = 65536;
	char MM_pool[MM_POOL_SIZE];
	
	// Initialize set up any data needed to manage the memory pool
	void initializeMemoryManager(void)
	{
		bool* isAvail = (bool*)MM_pool;
		int* size = (int*)&MM_pool[1];
		*isAvail = true;
		*size = MM_POOL_SIZE - sizeof(int) - sizeof(bool);
	}

	// return a pointer inside the memory pool
	// If no chunk can accommodate aSize call onOutOfMemory()
	void* allocate(int aSize)
	{
		if (aSize > (largestFree() - 5))
			onOutOfMemory();

		bool* available = (bool*)MM_pool;
		int* size = (int*)&MM_pool[1];
		int free = MM_POOL_SIZE - 5;
		//std::cout << *available << ' ' << *size << std::endl;
		// TODO: IMPLEMENT ME
		while (!(*available) || (*size) < aSize)
		{
			forwardInfo(available, size);
			free = *size;
			if (*available)
				break;
		}
		//set current available and size pointer; save the return pointer to address after available and size.
		*available = false;
		*size = aSize;
		//std::cout << *available << ' ' << *size << std::endl;
		void* ret = available + 5;

		// Reduce the free space of current memory block by aSize + 5 and update the next available and size
		free -= aSize + 5;
		available += *size + 5;
		*available = true;
		size = (int*)(available + 1);
		*size = free;
		//std::cout << *available << ' ' << *size << std::endl;

		return (ret);
	}

	// Free up a chunk previously allocated
	void deallocate(void* aPointer)
	{
		bool* available = (bool*)aPointer;
		int* size = (int*)(available - 4);
		available -= 5;
		*available = true;
		//std::cout << freeRemaining() << std::endl;
		// TODO: IMPLEMENT ME
	}

	//---
	//--- support routines
	//--- 
	void forwardInfo(bool* &available, int* &size)
	{
		available += *size + 5;
		size = (int*)(available + 1);
	}
	// Will scan the memory pool and return the total free space remaining
	int freeRemaining(void)
	{
		int count = 5, free = MM_POOL_SIZE - 5;
		bool* available = (bool*)MM_pool;
		int* size = (int*)&MM_pool[1];
		//std::cout << *available << ' ' << *size << std::endl;
		while (count < MM_POOL_SIZE)
		{
			if (!(*available))
				free -= *size + 5;
			count += *size + 5;
			forwardInfo(available, size);
		}
		// TODO: IMPLEMENT ME
		return free;
	}

	// Will scan the memory pool and return the largest free space remaining
	int largestFree(void)
	{
		int count = 5;
		bool* available = (bool*)MM_pool;
		int* size = (int*)&MM_pool[1];
		int largest = 0;

		while (count < MM_POOL_SIZE)
		{
			if (largest == 0 && *available)
				largest = *size;
			else if (*size > largest)
				largest = *size;
			count += *size + 5;
			forwardInfo(available, size);
		}
		// TODO: IMPLEMENT ME

		return largest;
	}

	// will scan the memory pool and return the smallest free space remaining
	int smallestFree(void)
	{
		int count = 5;
		bool* available = (bool*)MM_pool;
		int* size = (int*)&MM_pool[1];
		int smallest = 0;

		while (count < MM_POOL_SIZE)
		{
			if (smallest == 0 && *available)
				smallest = *size;
			else if (*size < smallest)
				smallest = *size;
			count += *size + 5;
			forwardInfo(available, size);
		}
		// TODO: IMPLEMENT ME

		return smallest;
	}
}