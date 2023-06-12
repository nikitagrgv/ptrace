#include "TestRegionsProvider.h"

void TestRegionsProvider::reload(int pid)
{
	std::cout << "in pr : " << this << std::endl;
	regions_ = getRegions(pid);
	emit regionsChanged();
}
