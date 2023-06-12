#include "AddressSelector.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <string>

AddressSelector::AddressSelector(QWidget *parent)
	: QWidget(parent)
{
	auto layout = new QHBoxLayout(this);

	{
		auto label_min = new QLabel("Min:", this);
		layout->addWidget(label_min);

		line_edit_min_ = new QLineEdit(this);
		layout->addWidget(line_edit_min_);
	}
	{
		auto label_max = new QLabel("Max:", this);
		layout->addWidget(label_max);

		line_edit_max_ = new QLineEdit(this);
		layout->addWidget(line_edit_max_);
	}
	{
		auto apply_button = new QPushButton("Select", this);
		layout->addWidget(apply_button);

		connect(apply_button, &QPushButton::clicked, this, [this]() {
			emit minAddressChanged(getMinAddress());
			emit maxAddressChanged(getMaxAddress());
		});
	}
}

size_t AddressSelector::getMinAddress() const
{
	return line_edit_min_->text().toULongLong(nullptr, 16);
}

size_t AddressSelector::getMaxAddress() const
{
	return line_edit_max_->text().toULongLong(nullptr, 16);
}
