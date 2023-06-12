#pragma once

#include <QWidget>

class QLineEdit;

class ProcessSelector : public QWidget
{
	Q_OBJECT

public:
	explicit ProcessSelector(QWidget *parent = nullptr);

	int getPid() const;

signals:
	void pidSelected(int pid);

private:
	QLineEdit *line_edit_{};
};
