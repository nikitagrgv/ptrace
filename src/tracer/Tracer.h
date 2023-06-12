#pragma once

#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>

#include <QString>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Regs = user_regs_struct;

constexpr int REGS_COUNT = sizeof(Regs) / sizeof(unsigned long long int);
inline const char *regName(int n);

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

inline QString checkError();
inline bool isSuccess();

class Tracer
{
public:
	Tracer(int pid);
	~Tracer();

	bool readWord(size_t address, size_t &data);
	bool writeWord(size_t address, size_t data);

	QList<unsigned char> readData(size_t begin, size_t end);

	int getPid() const { return pid_; }

private:
	int pid_;
};


inline QString getMapsFilename(int pid);

inline QList<Region> getRegions(int pid);
