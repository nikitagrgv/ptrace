#pragma once

#include <QObject>

class DataChooser : public QObject
{
	Q_OBJECT

public:
	virtual ~DataChooser() = default;

	virtual size_t getMinAddress() const = 0;
	virtual size_t getMaxAddress() const = 0;

signals:
	void minAddressChanged(size_t min);
	void maxAddressChanged(size_t max);
};
