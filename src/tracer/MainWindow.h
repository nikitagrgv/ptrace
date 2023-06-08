#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>



class TracerWidget : public QWidget
{
public:
	TracerWidget(QWidget *parent) : QWidget(parent)
	{
		new QVBoxLayout{this};


		auto button = new QPushButton("Hello world!", this);
		layout()->addWidget(button);
		button->resize(200, 100);

		auto button2 = new QPushButton("abababab", this);
		layout()->addWidget(button2);
		button2->resize(200, 100);


	}


};



class MainWindow : public QMainWindow
{
public:
	MainWindow()
		: QMainWindow()
	{
		tracer_widget_ = new TracerWidget{this};
		setCentralWidget(tracer_widget_);
	}


private:
	TracerWidget *tracer_widget_{};
};
