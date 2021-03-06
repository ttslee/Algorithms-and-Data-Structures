
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>

#include "MemoryManager.h"

int main(void)
{
	using namespace MemoryManager;

	initializeMemoryManager();

	long* int_pointer;
	char* string_pointer;

	std::cout << "Free memory= " << freeRemaining() << std::endl;
	int_pointer = (long *)allocate(sizeof(long));
	*int_pointer = 0xDEADBEEF;
	//std::cout << *int_pointer << std::endl;
	string_pointer = (char*)allocate(255);
	
	strcpy(string_pointer, "It was the best of times, it was the worst of times");
	//std::cout << string_pointer << std::endl;
	std::cout << "Free memory = " << freeRemaining() << std::endl;

	deallocate(int_pointer);
	/*std::cout << "Smallest free " << smallestFree() << std::endl;
	std::cout << "Largest free " << largestFree() << std::endl;*/
	deallocate(string_pointer);
	/*std::cout << "Smallest free " << smallestFree() << std::endl;
	std::cout << "Largest free " << largestFree() << std::endl;*/
	return 0; 
}

namespace MemoryManager
{
	void onOutOfMemory(void)
	{
		std::cerr << "Memory pool out of memory" << std::endl;
		exit(1);
	}

	// call for any other error condition, providing meaningful error messages are appreciated
	void onIllegalOperation(const char* fmt, ...)
	{
		if (fmt == NULL)
		{
			std::cerr << "Unknown illegal operation" << std::endl;
			exit(1);
		}
		else
		{
			char	buf[8192];

			va_list argptr;
			va_start(argptr, fmt);
			vsprintf(buf, fmt, argptr);
			va_end(argptr);

			std::cerr << "Illegal operation: \"" << buf << "\"" << std::endl;
			exit(1);
		}
	}
}
