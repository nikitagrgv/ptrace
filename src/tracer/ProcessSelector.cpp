#include "ProcessSelector.h"

#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

ProcessSelector::ProcessSelector(QWidget *parent)
	: QWidget(parent)
{
	auto layout = new QHBoxLayout(this);

	auto label = new QLabel("PID:", this);
	layout->addWidget(label);

	line_edit_ = new QLineEdit(this);
	layout->addWidget(line_edit_);
	line_edit_->setValidator(new QIntValidator(line_edit_));

	auto select_button = new QPushButton("Select", this);
	layout->addWidget(select_button);
	connect(select_button, &QPushButton::clicked, this, [this]() { emit pidSelected(getPid()); });
}

int ProcessSelector::getPid() const
{
	QString text = line_edit_->text();
	return text.toInt();
}
