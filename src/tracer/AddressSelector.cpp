#include "AddressSelector.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>

#include <string>

class AddressListModel : public QAbstractListModel
{
public:
	AddressListModel(QObject *parent = nullptr)
		: QAbstractListModel(parent)
	{}

	int rowCount(const QModelIndex &parent = QModelIndex()) const override
	{
		return 5;
	}

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
	{
		if (role == Qt::DisplayRole)
		{
			return index.row();
		}

		return {};
	}

};

AddressSelector::AddressSelector(QWidget *parent)
	: QWidget(parent)
{
	auto layout = new QHBoxLayout(this);
	{
		list_view_ = new QListView(this);
		layout->addWidget(list_view_);
		list_view_->setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);

		auto model = new AddressListModel(this);
		list_view_->setModel(model);
	}

	auto min_max_layout = new QVBoxLayout();
	layout->addLayout(min_max_layout);

	{
		auto label_min = new QLabel("Min:", this);
		min_max_layout->addWidget(label_min);

		line_edit_min_ = new QLineEdit(this);
		min_max_layout->addWidget(line_edit_min_);
	}
	{
		auto label_max = new QLabel("Max:", this);
		min_max_layout->addWidget(label_max);

		line_edit_max_ = new QLineEdit(this);
		min_max_layout->addWidget(line_edit_max_);
	}
	{
		auto apply_button = new QPushButton("Select", this);
		min_max_layout->addWidget(apply_button);

		connect(apply_button, &QPushButton::clicked, this, [this]() {
			emit minAddressChanged(getMinAddress());
			emit maxAddressChanged(getMaxAddress());
		});
	}

	setFixedHeight(min_max_layout->sizeHint().height());
}

size_t AddressSelector::getMinAddress() const
{
	return line_edit_min_->text().toULongLong(nullptr, 16);
}

size_t AddressSelector::getMaxAddress() const
{
	return line_edit_max_->text().toULongLong(nullptr, 16);
}
