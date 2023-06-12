#pragma once

#include "DataChooser.h"
#include "DataProvider.h"
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
		new QVBoxLayout{this};

		create_pid_selector_widget();

		auto button = new QPushButton("Hello world!", this);
		layout()->addWidget(button);
		button->resize(200, 100);

		view_ = new QTableView(this);
		QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
		view_->setFont(font);
		view_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		view_->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		layout()->addWidget(view_);


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

		model_ = new TracerModel(std::move(provider), std::move(chooser), this);
		view_->setModel(model_);


		connect(button, &QPushButton::clicked, this, [this]() {
			auto &chooser = static_cast<TestDataChooser &>(model_->getDataChooser());
			auto &provider = static_cast<TestDataProvider &>(model_->getDataProvider());

			chooser.setMaxAddress(chooser.getMaxAddress() + 23);
			chooser.setMinAddress(chooser.getMinAddress() - 15);
		});
	}

private:
	void create_pid_selector_widget()
	{
		auto pid_selector = new QWidget();
		new QHBoxLayout(pid_selector);

		layout()->addWidget(pid_selector);
	}

private:
	TracerModel *model_{};
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
