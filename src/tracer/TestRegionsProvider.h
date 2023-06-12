#pragma once

#include "AddressSelector.h"
#include "Tracer.h"

class TestRegionsProvider : public RegionsProvider
{
	Q_OBJECT

public:
	explicit TestRegionsProvider(QObject *parent = nullptr)
		: RegionsProvider(parent)
	{}

	void reload(int pid);

	int getRegionsCount() const override { return regions_.size(); }
	size_t getRegionBegin(int region) const override { return regions_[region].begin; }
	size_t getRegionEnd(int region) const override { return regions_[region].end; }

private:
	QList<Region> regions_;
};
