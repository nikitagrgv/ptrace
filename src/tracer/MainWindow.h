#pragma once

#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow
{
public:
	MainWindow()
		: QMainWindow()
	{

		auto button = new QPushButton("Hello world!", nullptr);
		button->resize(200, 100);
	}
};
