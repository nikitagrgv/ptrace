#include <sys/ptrace.h>
#include <unistd.h>

#include <iostream>

int main()
{

	int pid = 0;

	std::cin >> pid;


	long res = ptrace(PTRACE_ATTACH, pid, 0, 0);

	while (true)
	{
		std::cout << res << std::endl;
	}
	return 0;
}
