#include <iostream>

#include <time.h>

unsigned long int cur = 0;

int main()
{
	auto addr = reinterpret_cast<void *>(&main);

	volatile auto some_var = new volatile size_t{123};

	while (true)
	{
		std::cout << addr << " | " << ++cur << " | " << *some_var << std::endl;
		volatile long int counter = 0xfffffff;
		while (--counter)
		{}
	}
	return 0;
}
