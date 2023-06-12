#include "MainWindow.h"

TracerWidget::TracerWidget(QWidget *parent)
	: QWidget(parent)
{
	auto layout = new QVBoxLayout{this};

	process_selector_ = new ProcessSelector(this);
	layout->addWidget(process_selector_);

	class TestRegionsProvider : public RegionsProvider
	{
	public:
		TestRegionsProvider(QObject *parent = nullptr)
			: RegionsProvider(parent)
		{}

		void reload() {}

		int getRegionsCount() const override { return 6; }
		size_t getRegionBegin(int region) const override { return region * 0x1000; }
		size_t getRegionEnd(int region) const override { return region * 0x1000 + 0x555; }

	private:
		QList<Region> regions_;
	};
	auto regions_provider = std::make_unique<TestRegionsProvider>();

	address_selector_ = new AddressSelector(std::move(regions_provider), this);
	layout->addWidget(address_selector_);

	connect(process_selector_, &ProcessSelector::pidSelected, this, &TracerWidget::create_model);

	view_ = new QTableView(this);
	QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
	view_->setFont(font);
	view_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	view_->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	layout->addWidget(view_);

	layout->setStretchFactor(process_selector_, 0);
	layout->setStretchFactor(address_selector_, 0);
	layout->setStretchFactor(view_, 5);
}

void TracerWidget::create_model(int pid)
{
	class TestDataProvider : public DataProvider
	{
	public:
		size_t getData(size_t addr) const override { return addr % 0x100; }
		size_t getMinAddress() const override { return 0x1000 + 5; }
		size_t getMaxAddress() const override { return 0x2000 - 2; }
	};
	auto provider = std::make_unique<TestDataProvider>();

	class TestDataChooser : public DataChooser
	{
	public:
		size_t getMinAddress() const override { return min_; }
		size_t getMaxAddress() const override { return max_; }

		void setMinAddress(size_t min)
		{
			min_ = min;
			emit minAddressChanged(min);
		}
		void setMaxAddress(size_t max)
		{
			max_ = max;
			emit maxAddressChanged(max);
		}

	private:
		size_t min_{0x2000 - 400};
		size_t max_{0x2000 + 10};
	};
	auto chooser = std::make_unique<TestDataChooser>();
	chooser->setMinAddress(pid);
	chooser->setMaxAddress(pid + 0x100);

	connect(address_selector_, &AddressSelector::minAddressChanged, chooser.get(),
		[chooser = chooser.get(), this](size_t addr) { chooser->setMinAddress(addr); });

	connect(address_selector_, &AddressSelector::maxAddressChanged, chooser.get(),
		[chooser = chooser.get(), this](size_t addr) { chooser->setMaxAddress(addr); });

	model_ = std::make_unique<HexTableModel>(std::move(provider), std::move(chooser), this);
	view_->setModel(model_.get());
}
