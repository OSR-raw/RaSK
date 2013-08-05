#include "qtrask.h"
#include <QtWidgets/QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QtRaSK w;
	w.resize(w.sizeHint());
	w.show();
	return a.exec();
}
