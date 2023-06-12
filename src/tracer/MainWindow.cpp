#include "MainWindow.h"

#include "TestRegionsProvider.h"

TracerWidget::TracerWidget(QWidget *parent)
	: QWidget(parent)
{
	auto layout = new QVBoxLayout{this};

	// process select
	process_selector_ = new ProcessSelector(this);
	layout->addWidget(process_selector_);

	// region select
	auto regions_provider = std::make_unique<TestRegionsProvider>();
	regions_provider_ = regions_provider.get(); // TODO shitty

	connect(process_selector_, &ProcessSelector::pidSelected, this, [this](int pid) {
		std::cout << "rel pr : " << regions_provider_ << std::endl;
		static_cast<TestRegionsProvider*>(regions_provider_)->reload(pid);
		create_model(pid);
	});

	address_selector_ = new AddressSelector(std::move(regions_provider), this);
	layout->addWidget(address_selector_);

	// hex view
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
	tracer_ = std::make_unique<Tracer>(pid);

	class TestDataProvider : public DataProvider
	{
	public:
		TestDataProvider(Tracer *tracer)
			: tracer_(tracer)
		{}

		size_t getData(size_t addr) const override
		{
			size_t data = 0x00;
			tracer_->readWord(addr, data);
			const int byte_num = addr % 8;
			addr >> 8 * byte_num;
			return data & 0xFF;
		}
		size_t getMinAddress() const override { return 0x0; }
		size_t getMaxAddress() const override { return -1; }

	private:
		Tracer *tracer_{};
	};
	auto provider = std::make_unique<TestDataProvider>(tracer_.get());

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
