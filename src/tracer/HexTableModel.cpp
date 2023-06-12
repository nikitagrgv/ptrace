#include "HexTableModel.h"

#include "DataChooser.h"
#include "DataProvider.h"
#include "Tracer.h"

#include <cmath>

HexTableModel::HexTableModel(std::unique_ptr<DataProvider> provider,
	std::unique_ptr<DataChooser> chooser, QObject *parent)
	: QAbstractTableModel(parent)
	, provider_(std::move(provider))
	, chooser_(std::move(chooser))
{
	assert(provider_);
	assert(chooser_);

	auto update_all = [this]() {
		QModelIndex top_left = index(0, 0);
		QModelIndex bottom_right = index(rowCount(), columnCount());
		emit dataChanged(top_left, bottom_right);
		emit headerDataChanged(Qt::Vertical, top_left.column(), bottom_right.column());
		emit headerDataChanged(Qt::Horizontal, top_left.row(), bottom_right.row());
	};

	auto update_data = [=](size_t addr) {
		// TODO
		update_all();
	};

	connect(provider_.get(), &DataProvider::dataChanged, this, update_data);
	connect(provider_.get(), &DataProvider::maxAddressChanged, this, update_all);
	connect(provider_.get(), &DataProvider::minAddressChanged, this, update_all);

	connect(chooser_.get(), &DataChooser::maxAddressChanged, this, update_all);
	connect(chooser_.get(), &DataChooser::minAddressChanged, this, update_all);
}

int HexTableModel::rowCount(const QModelIndex &parent) const
{
	const double data_size = double(chooser_->getMaxAddress() - chooser_->getMinAddress() + 1);
	const double column_count = (double)columnCount({});
	return std::ceil(data_size / column_count);
}

int HexTableModel::columnCount(const QModelIndex &parent) const
{
	return 16 + show_ascii_;
}

QVariant HexTableModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (is_ascii_info_index(index))
		{
			return get_ascii_from_row(index.row());
		}

		const size_t addr = index_to_address(index);
		if (is_valid_addr(addr))
		{
			const size_t value = get_value_by_addr(addr);
			return to_hex(value, HexMode::Without0x, 2);
		}
		else
		{
			return QString("--");
		}
	}
	else if (role == Qt::ToolTipRole)
	{
		if (is_ascii_info_index(index))
		{
			return {};
		}

		const size_t addr = index_to_address(index);
		return to_hex(addr, HexMode::Without0x, 16);
	}

	return {};
}

QVariant HexTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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
			if (is_ascii_info_index(createIndex(0, section)))
			{
				return "ASCII";
			}

			return to_hex((get_first_cell_addr() + section) % 16, HexMode::Without0x);
		}
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}

QString HexTableModel::get_ascii_from_row(int row) const
{
	QString ascii{};
	QModelIndex index = createIndex(row, 0);
	size_t begin = index_to_address(index);
	size_t end = begin + columnCount({}) - show_ascii_;
	for (size_t addr = begin; addr < end; ++addr)
	{
		if (!is_valid_addr(addr))
		{
			ascii.append(' ');
			continue;
		}

		const unsigned char ch = (unsigned char)get_value_by_addr(addr);
		if (std::isprint(ch))
		{
			ascii.append(ch);
		}
		else
		{
			ascii.append('.');
		}
	}
	return ascii;
}

size_t HexTableModel::get_value_by_addr(size_t addr) const
{
	return provider_->getData(addr);
}

bool HexTableModel::is_valid_addr(size_t addr) const
{
	return addr <= provider_->getMaxAddress() && addr >= provider_->getMinAddress();
}

QString HexTableModel::to_hex(size_t number, HexTableModel::HexMode hex_mode, int min_length) const
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

size_t HexTableModel::index_to_address(const QModelIndex &index) const
{
	const int row = index.row();
	const int column = index.column();
	const int column_count = columnCount() - show_ascii_;
	return get_first_cell_addr() + row * column_count + column;
}

size_t HexTableModel::get_first_cell_addr() const
{
	return size_t(std::floor((double)chooser_->getMinAddress() / 16.) * 16.);
}

bool HexTableModel::is_ascii_info_index(const QModelIndex &index) const
{
	return show_ascii_ && index.column() == columnCount() - 1;
}
