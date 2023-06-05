#include <iostream>

#include <time.h>

volatile int some_var = 123;

unsigned long int cur = 0;

int main()
{
	while (true)
	{
		std::cout << ++cur <<  " | " << some_var << std::endl;
	}
	return 0;
}
