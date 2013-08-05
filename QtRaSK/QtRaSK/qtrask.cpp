#include "qtrask.h"
#include "brush.h"
#include "Core/Loader.h"

#include "control/Input.h"

#include <QColorDialog>
#include <QInputDialog>
QtRaSK::QtRaSK(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.widget->initializeGLLocal();
	brush = new Brush(this);
	ui.pain_w->setBrush(brush, "Brush");
	ui.widget->setPaintArea(ui.pain_w );
	inp = new Input( ui.widget );
	ui.widget->setInput(inp);

	ldr = new Loader(ui.widget);	
}

void QtRaSK::brushColor()
{
    const QColor newColor = QColorDialog::getColor(ui.pain_w->brushColor(), this, "", QColorDialog::ShowAlphaChannel);
    if (newColor.isValid())
        ui.pain_w->setBrushColor(newColor);
}

void QtRaSK::loadData()
{
	//ui.widget->updateTransferFunction();
	if (ldr)
		ldr->DoLoadData();
}

void QtRaSK::resetColor()
{
	ui.pain_w->clearImage();
}

void QtRaSK::updateTRF()
{
	ui.widget->updateTransferFunction();
}

void QtRaSK::brushWidth()
{
    bool ok;
    const int newWidth = QInputDialog::getInt(this, tr(""),
                                              tr("Select brush width:"),
                                              ui.pain_w->brushWidth(),
                                              1, 550, 1, &ok);
    if (ok)
        ui.pain_w->setBrushWidth(newWidth);
}


QtRaSK::~QtRaSK()
{
	delete inp;
	delete ldr;
}
