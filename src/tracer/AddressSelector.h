#pragma once

#include <QWidget>

#include <memory>

class QLineEdit;
class QListView;

class RegionsProvider : public QObject
{
	Q_OBJECT

public:
	RegionsProvider(RegionsProvider &&) = delete;
	RegionsProvider(const RegionsProvider &) = delete;
	RegionsProvider &operator=(RegionsProvider &&) = delete;
	RegionsProvider &operator=(const RegionsProvider &) = delete;

	explicit RegionsProvider(QObject *parent);

	virtual int getRegionsCount() const = 0;
	virtual size_t getRegionBegin(int region) const = 0;
	virtual size_t getRegionEnd(int region) const = 0;

signals:
	void regionsChanged();
};

class AddressSelector : public QWidget
{
	Q_OBJECT

public:
	AddressSelector(std::unique_ptr<RegionsProvider> provider, QWidget *parent = nullptr);

	size_t getMinAddress() const;
	size_t getMaxAddress() const;

signals:
	void minAddressChanged(size_t min);
	void maxAddressChanged(size_t max);

private:
	void set_min_address(size_t addr);
	void set_max_address(size_t addr);

private:
	QListView *list_view_{};

	QLineEdit *line_edit_min_{};
	QLineEdit *line_edit_max_{};
};
