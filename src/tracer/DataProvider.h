#pragma once

#include <QObject>

class DataProvider : public QObject
{
	Q_OBJECT

public:
	virtual ~DataProvider() = default;

	virtual size_t getData(size_t addr) const = 0;
	virtual size_t getMinAddress() const = 0;
	virtual size_t getMaxAddress() const = 0;

signals:
	void dataChanged(size_t addr);
	void minAddressChanged(size_t min);
	void maxAddressChanged(size_t max);
};
