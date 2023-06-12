#include "Tracer.h"

#include <QList>

const char *regName(int n)
{
	static const char *names[] = {"r15", "r14", "r13", "r12", "rbp", "rbx", "r11", "r10", "r9",
		"r8", "rax", "rcx", "rdx", "rsi", "rdi", "orig_rax", "rip", "cs", "eflags", "rsp", "ss",
		"fs_base", "gs_base", "ds", "es", "fs", "gs"};

	assert(n >= 0 && n < REGS_COUNT);
	return names[n];
}

QString checkError()
{
	// TODO shitty
	std::string error_buf{};
	error_buf.resize(200);
	perror(error_buf.c_str());
	return {error_buf.data()};
}

bool isSuccess()
{
	bool success = errno == 0;
	errno = 0;
	return success;
}

QString getMapsFilename(int pid)
{
	QString maps_filename = "/proc/";
	maps_filename += QString::number(pid);
	maps_filename += "/maps";
	return maps_filename;
}

QList<Region> getRegions(int pid)
{
	// TODO shitty
	std::ifstream maps_file{getMapsFilename(pid).toStdString()};

	if (!maps_file.is_open())
	{
		std::cout << "cannot open!" << std::endl;
		return {};
	}

	QList<Region> regions;

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

Tracer::Tracer(int pid)
	: pid_{pid}
{
	auto ret = ptrace(PTRACE_ATTACH, pid, 0, 0);
	assert(ret == 0 && "Cannot attach");
}

Tracer::~Tracer()
{
	auto ret = ptrace(PTRACE_DETACH, pid_, 0, 0);
	assert(ret == 0 && "Cannot detach");
}

bool Tracer::readWord(size_t address, size_t &data)
{
	errno = 0;
	data = ptrace(PTRACE_PEEKDATA, pid_, address, 0);
	return isSuccess();
}

bool Tracer::writeWord(size_t address, size_t data)
{
	errno = 0;
	ptrace(PTRACE_POKEDATA, pid_, address, data);
	return isSuccess();
}

QList<unsigned char> Tracer::readData(size_t begin, size_t end)
{
	QList<unsigned char> data;
	data.reserve(end - begin);

	for (size_t i = begin; i < end; i += 8)
	{
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
