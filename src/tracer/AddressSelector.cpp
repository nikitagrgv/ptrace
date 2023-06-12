#include "AddressSelector.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>

#include <memory>
#include <string>

RegionsProvider::RegionsProvider(QObject *parent)
	: QObject(parent)
{}

class AddressListModel : public QAbstractListModel
{
public:
	enum
	{
		ADDRESS_BEGIN_ROLE = Qt::UserRole + 1,
		ADDRESS_END_ROLE
	};

	AddressListModel(std::unique_ptr<RegionsProvider> provider, QObject *parent = nullptr)
		: QAbstractListModel(parent)
		, provider_(std::move(provider))
	{
		auto update_all = [this]() {
			QModelIndex top_left = index(0, 0);
			QModelIndex bottom_right = index(rowCount(), 0);
			emit dataChanged(top_left, bottom_right);
			emit headerDataChanged(Qt::Vertical, top_left.column(), bottom_right.column());
			emit headerDataChanged(Qt::Horizontal, top_left.row(), bottom_right.row());
		};

		connect(provider_.get(), &RegionsProvider::regionsChanged, this, update_all);
	}

	int rowCount(const QModelIndex &parent = QModelIndex()) const override
	{
		return provider_->getRegionsCount();
	}

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
	{
		if (role == Qt::DisplayRole)
		{
			const size_t begin = provider_->getRegionBegin(index.row());
			const size_t end = provider_->getRegionEnd(index.row());
			QString info;
			info += "0x";
			info += QString::number(begin, 16);
			info += " - 0x";
			info += QString::number(end, 16);
			return info;
		}
		if (role == ADDRESS_BEGIN_ROLE)
		{
			return (qulonglong)provider_->getRegionBegin(index.row());
		}
		if (role == ADDRESS_END_ROLE)
		{
			return (qulonglong)provider_->getRegionEnd(index.row());
		}

		return {};
	}

private:
	std::unique_ptr<RegionsProvider> provider_;
};

AddressSelector::AddressSelector(std::unique_ptr<RegionsProvider> provider, QWidget *parent)
	: QWidget(parent)
{
	auto layout = new QHBoxLayout(this);
	{
		list_view_ = new QListView(this);
		layout->addWidget(list_view_);
		list_view_->setSizePolicy(QSizePolicy::Policy::MinimumExpanding,
			QSizePolicy::Policy::MinimumExpanding);

		auto model = new AddressListModel(std::move(provider), this);
		list_view_->setModel(model);

		connect(list_view_, &QListView::activated, this, [this](const QModelIndex &index) {
			set_min_address(list_view_->model()
								->data(index, AddressListModel::ADDRESS_BEGIN_ROLE)
								.toULongLong());
			set_max_address(
				list_view_->model()->data(index, AddressListModel::ADDRESS_END_ROLE).toULongLong());
		});
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

void AddressSelector::set_min_address(size_t addr)
{
	line_edit_min_->setText(QString::number(addr, 16));
}

void AddressSelector::set_max_address(size_t addr)
{
	line_edit_max_->setText(QString::number(addr, 16));
}
