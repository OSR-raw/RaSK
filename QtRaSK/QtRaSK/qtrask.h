#ifndef QTRASK_H
#define QTRASK_H

#include <QtWidgets/QMainWindow>
#include "ui_qtrask.h"
#include "oglwidget.h"
class Brush;
class QtRaSK : public QMainWindow
{
	Q_OBJECT

public:
	QtRaSK(QWidget *parent = 0);
	~QtRaSK();
	
public Q_SLOTS:
    void brushColor();
    void brushWidth();
	void loadData();
	void updateTRF();
	void resetColor();

private:
	Ui::QtRaSKClass ui;
	Brush* brush;
	Input* inp;
	Loader* ldr;
};

#endif // QTRASK_H
