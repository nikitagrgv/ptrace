#pragma once

#include <QAbstractTableModel>
#include <QFontDatabase>
#include <QHeaderView>
#include <QMainWindow>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

class TracerModel : public QAbstractTableModel
{
public:
	TracerModel(QObject *parent = nullptr)
		: QAbstractTableModel(parent)
	{}

	int rowCount(const QModelIndex &parent = QModelIndex()) const override
	{
		return 14;
		;
	}
	int columnCount(const QModelIndex &parent = QModelIndex()) const override
	{
		return 16;
		;
	}
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
	{
		if (role == Qt::DisplayRole)
		{
			const size_t addr = index_to_address(index);
			return to_hex(addr, HexMode::Without0x);
		}

		return {};
	}

	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const override
	{
		if (role == Qt::DisplayRole)
		{
			if (orientation == Qt::Vertical)
			{
				const QModelIndex index = createIndex(section, 0);
				const size_t addr = index_to_address(index);
				return to_hex(addr, HexMode::With0x, 16);
			}
			else
			{
				const size_t offset = section;
				return to_hex(offset, HexMode::Without0x);
			}
		}

		return QAbstractTableModel::headerData(section, orientation, role);
	}

private:
	enum class HexMode
	{
		With0x,
		Without0x
	};

	QString to_hex(size_t number, HexMode hex_mode, int min_length = 0) const
	{
		// TODO: shitty
		QString hex = hex_mode == HexMode::With0x ? "0x" : "";
		QString num = QString::number(number, 16);
		if (num.size() < min_length)
		{
			num = QString(min_length - num.size(), '0') + num;
		}
		return hex + num;
	}

	size_t index_to_address(const QModelIndex &index) const
	{
		const int row = index.row();
		const int column = index.column();
		return row * columnCount() + column;
	}
};


class TracerWidget : public QWidget
{
public:
	TracerWidget(QWidget *parent = nullptr)
		: QWidget(parent)
	{


		new QVBoxLayout{this};


		auto button = new QPushButton("Hello world!", this);
		layout()->addWidget(button);
		button->resize(200, 100);

		view_ = new QTableView(this);
		QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
		view_->setFont(font);
		view_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		view_->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		layout()->addWidget(view_);


		model_ = new TracerModel(this);
		view_->setModel(model_);
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
