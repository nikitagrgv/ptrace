#pragma once

#include <QWidget>

class QLineEdit;

class AddressSelector : public QWidget
{
	Q_OBJECT

public:
	AddressSelector(QWidget *parent = nullptr);

	size_t getMinAddress() const;
	size_t getMaxAddress() const;

signals:
	void minAddressChanged(size_t min);
	void maxAddressChanged(size_t max);

private:
	QLineEdit *line_edit_min_{};
	QLineEdit *line_edit_max_{};
};
