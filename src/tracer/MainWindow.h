#pragma once

#include "AddressSelector.h"
#include "DataChooser.h"
#include "DataProvider.h"
#include "HexTableModel.h"
#include "ProcessSelector.h"
#include "Tracer.h"

#include <QAbstractTableModel>
#include <QFontDatabase>
#include <QHeaderView>
#include <QMainWindow>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

#include <cmath>
#include <memory>

class TestRegionsProvider;

class TracerWidget : public QWidget
{
public:
	TracerWidget(QWidget *parent = nullptr);

private:
	void create_model(int pid);


private:
	AddressSelector *address_selector_{};
	ProcessSelector *process_selector_{};
	RegionsProvider *regions_provider_{};

	std::unique_ptr<HexTableModel> model_;
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
