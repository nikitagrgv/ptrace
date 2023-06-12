#include <iostream>

#include <time.h>

unsigned long int cur = 0;

int main()
{
	auto addr = reinterpret_cast<void *>(&main);

	volatile auto some_var = new volatile size_t{123};

	std::string str1
		= "bobabaobaobjoarbobabaobaobjoarbobabaobaobjoarbobabaobaobjoarbobabaobaobjoar";

	while (true)
	{
		const char str2[] = "fffffffffffbxcbbxsbsbsbfffffaaaaaaaaaaasfafaf";

		std::cout << addr << " | " << ++cur << " | " << *some_var << " " << str1[0] << str2[0] << std::endl;
		volatile long int counter = 0xfffffff;
		while (--counter)
		{}
	}
	return 0;
}
