#include "MainWindow.h"

#include <QApplication>
#include <QPushButton>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow main_window{};
	main_window.show();

	return QApplication::exec();


//	int pid = 0;
//	std::cin >> pid;
//	Tracer tracer{pid};
//
//	auto aboba = [&tracer](Region region) {
//		auto data = tracer.readData(region.begin, region.end);
//		std::cout << std::hex;
//		size_t cur = region.begin;
//		for (const auto &byte : data)
//		{
//			if (byte == 123)
//			{
//				size_t addr_123 = cur;
//
//				std::cout << "found = " << cur << std::endl;
//				bool ret = tracer.writeWord(addr_123, 456);
//				std::cout << "succ: " << ret << std::endl;
//			}
//			cur += 1;
//		}
//	};
//
//	std::vector<Region> regions = getRegions(pid);
//	for (const auto &region : regions)
//	{
//		aboba(region);
//	}
//
//	auto print_regs = [](const Regs &regs) {
//		RegsUnion u{};
//		u.regs = regs;
//
//		for (int i = 0; i < REGS_COUNT; ++i)
//		{
//			std::cout << regName(i) << " " << std::hex << u.vals[i] << std::dec << std::endl;
//		}
//	};
//

	return 0;
}
