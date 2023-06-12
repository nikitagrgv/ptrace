#pragma once

#include <QAbstractTableModel>

#include <memory>

class DataChooser;
class DataProvider;

class TracerModel : public QAbstractTableModel
{
public:
	explicit TracerModel(std::unique_ptr<DataProvider> provider,
		std::unique_ptr<DataChooser> chooser, QObject *parent = nullptr);

	DataProvider &getDataProvider() { return *provider_; }
	DataProvider &getDataProvider() const { return *provider_; }

	DataChooser &getDataChooser() { return *chooser_; }
	DataChooser &getDataChooser() const { return *chooser_; }

	int rowCount(const QModelIndex &parent = QModelIndex{}) const override;

	int columnCount(const QModelIndex &parent = QModelIndex{}) const override;

	QVariant data(const QModelIndex &index, int role) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
	enum class HexMode
	{
		With0x,
		Without0x
	};

	QString get_ascii_from_row(int row) const;

	size_t get_value_by_addr(size_t addr) const;

	bool is_valid_addr(size_t addr) const;

	QString to_hex(size_t number, HexMode hex_mode, int min_length = 0) const;

	size_t index_to_address(const QModelIndex &index) const;

	size_t get_first_cell_addr() const;

	bool is_ascii_info_index(const QModelIndex &index) const;

private:
	std::unique_ptr<DataChooser> chooser_{};
	std::unique_ptr<DataProvider> provider_{};
	bool show_ascii_ = true;
};
