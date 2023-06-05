#include <iostream>

#include <time.h>

volatile int some_var = 123;

unsigned long int cur = 0;

int main()
{
	auto addr = reinterpret_cast<void *>(&main);

	while (true)
	{
		std::cout << addr << " | " << ++cur << " | " << some_var << std::endl;
		volatile long int counter = 0xfffffff;
		while (--counter)
		{}
	}
	return 0;
}
