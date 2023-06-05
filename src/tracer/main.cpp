#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>

#include <cassert>
#include <iostream>


using Regs = user_regs_struct;

constexpr int REGS_COUNT = sizeof(Regs) / sizeof(unsigned long long int);

const char *regName(int n)
{
	static const char *names[] = {"r15", "r14", "r13", "r12", "rbp", "rbx", "r11", "r10", "r9",
		"r8", "rax", "rcx", "rdx", "rsi", "rdi", "orig_rax", "rip", "cs", "eflags", "rsp", "ss",
		"fs_base", "gs_base", "ds", "es", "fs", "gs"};

	assert(n >= 0 && n < REGS_COUNT);
	return names[n];
}

union RegsUnion
{
	Regs regs;
	unsigned long long int vals[27];
};


int main()
{
	int pid = 0;

	long ret = -1;
	do
	{
		std::cin >> pid;
		ret = ptrace(PTRACE_ATTACH, pid, 0, 0);
	}
	while (ret != 0 && (std::cout << "error " << ret << std::endl));

	auto print_regs = [](const Regs &regs) {
		RegsUnion u{};
		u.regs = regs;

		for (int i = 0; i < REGS_COUNT; ++i)
		{
			std::cout << regName(i) << " " << std::hex << u.vals[i] << std::dec << std::endl;
		}
	};

	while (true)
	{
		Regs r;
		long regs_ret = ptrace(PTRACE_GETREGS, pid, NULL, &r);
		if (regs_ret == 0)
		{
			print_regs(r);
		}
		else
		{
			std::cout << "error getregs " << regs_ret << std::endl;
		}
		std::cout << std::endl;
	}
	return 0;
}
