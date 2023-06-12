#pragma once

#include "AddressSelector.h"
#include "DataChooser.h"
#include "DataProvider.h"
#include "ProcessSelector.h"
#include "Tracer.h"
#include "TracerModel.h"

#include <QAbstractTableModel>
#include <QFontDatabase>
#include <QHeaderView>
#include <QMainWindow>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

#include <cmath>
#include <memory>

class TracerWidget : public QWidget
{
public:
	TracerWidget(QWidget *parent = nullptr)
		: QWidget(parent)
	{
		auto layout = new QVBoxLayout{this};

		process_selector_ = new ProcessSelector(this);
		layout->addWidget(process_selector_);

		address_selector_ = new AddressSelector(this);
		layout->addWidget(address_selector_);

		connect(process_selector_, &ProcessSelector::pidSelected, this,
			&TracerWidget::create_model);

		view_ = new QTableView(this);
		QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
		view_->setFont(font);
		view_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		view_->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		layout->addWidget(view_);
	}

private:
	void create_model(int pid)
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

		model_ = std::make_unique<TracerModel>(std::move(provider), std::move(chooser), this);
		view_->setModel(model_.get());
	}


private:
	AddressSelector *address_selector_{};
	ProcessSelector *process_selector_{};

	std::unique_ptr<TracerModel> model_;
	QTableView *view_{};
};


class MainWindow : public QMainWindow
{
public:
	MainWindow()
		: QMainWindow()
	{
		resize(800, 800);

		tracer_widget_ = new TracerWidget{this};
		setCentralWidget(tracer_widget_);
	}


private:
	TracerWidget *tracer_widget_{};
};
