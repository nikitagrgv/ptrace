#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>

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


struct Region
{
	size_t begin{};
	size_t end{};
};


std::string checkError()
{
	std::string error_buf{};
	error_buf.resize(200);
	perror(error_buf.c_str());
	return error_buf;
}

bool isSuccess()
{
	bool success = errno == 0;
	errno = 0;
	return success;
}

class Tracer
{
public:
	Tracer(int pid)
		: pid_{pid}
	{
		auto ret = ptrace(PTRACE_ATTACH, pid, 0, 0);
		assert(ret == 0 && "Cannot attach");
	}

	~Tracer()
	{
		auto ret = ptrace(PTRACE_DETACH, pid_, 0, 0);
		assert(ret == 0 && "Cannot detach");
	}

	bool readWord(size_t address, size_t &data)
	{
		errno = 0;
		data = ptrace(PTRACE_PEEKDATA, pid_, address, 0);
		return isSuccess();
	}

	std::vector<unsigned char> readData(size_t begin, size_t end)
	{
		std::vector<unsigned char> data;
		data.reserve(end - begin);

		for (size_t i = begin; i < end; i += 8)
		{
//			std::cout << i << " " << end << std::endl;


			union Word
			{
				size_t word;
				unsigned char bytes[8];
			};

			Word word{};
			bool succ = readWord(i, word.word);
//			if (succ) // TODO#
			{
				for (const unsigned char &byte : word.bytes)
				{
					data.push_back(byte);
				}
			}
		}

		return data;
	}

	int getPid() const { return pid_; }

private:
	int pid_;
};


std::string getMapsFilename(int pid)
{
	std::string maps_filename = "/proc/";
	maps_filename += std::to_string(pid);
	maps_filename += "/maps";
	return maps_filename;
}

std::vector<Region> getRegions(int pid)
{
	std::ifstream maps_file{getMapsFilename(pid)};

	if (!maps_file.is_open())
	{
		std::cout << "cannot open!" << std::endl;
		return {};
	}

	std::vector<Region> regions;

	std::string cur_line;
	while (std::getline(maps_file, cur_line))
	{
		if (cur_line.find("[heap]") != std::string::npos
			|| cur_line.find("[stack]") != std::string::npos)
		{
			Region region;

			std::stringstream sstr{cur_line};
			sstr >> std::hex >> region.begin;
			char spam;
			sstr >> spam;
			sstr >> std::hex >> region.end;

			regions.push_back(region);
		}
	}

	return regions;
}




int main()
{
	int pid = 0;
	std::cin >> pid;
	Tracer tracer{pid};

	std::vector<Region> regions = getRegions(pid);

	auto data = tracer.readData(regions[1].begin, regions[1].end);

	std::cout << std::hex;
	size_t cur = regions[1].begin;
	for (const auto &byte : data)
	{
		if (cur % (8 * 4) == 0)
		{
			std::cout << std::endl << cur << ": ";
		}
		cur += 8;

		std::cout.width(2);
		std::cout << (int)byte;
	}

	//	for (size_t i = regions[1].begin; i < regions[1].end; i += 64)
//	{
//		size_t data;
//		bool succ = tracer.readWord(i, data);
//		if (succ)
//		{
//			std::cout << std::hex << i << ": " << std::hex << data << std::endl;
//		}
//		else
//		{
//			std::cout << std::hex << i << ": error" << std::endl;
//		}
//	}

//	ptrace(PTRACE_PEEKDATA, )



	auto print_regs = [](const Regs &regs) {
		RegsUnion u{};
		u.regs = regs;

		for (int i = 0; i < REGS_COUNT; ++i)
		{
			std::cout << regName(i) << " " << std::hex << u.vals[i] << std::dec << std::endl;
		}
	};


	//	while (true)
	//	{
	//		Regs r;
	//		long regs_ret = ptrace(PTRACE_GETREGS, pid, NULL, &r);
	//		if (regs_ret == 0)
	//		{
	//			print_regs(r);
	//		}
	//		else
	//		{
	//			std::cout << "error getregs " << regs_ret << std::endl;
	//		}
	//		std::cout << std::endl;
	//	}


	return 0;
}
