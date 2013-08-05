//#include "interfaces.h"
#include "paintarea.h"
#include "brush.h"
#include <QPainter>
#include <QMouseEvent>

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent),
    theImage(256, 256, QImage::Format_ARGB32),
    color(255,255,255,25),
    thickness(100),
    brush(0),
    lastPos(-1, -1)
{
	setAttribute(Qt::WA_StaticContents);
    //setAttribute(Qt::WA_TranslucentBackground);
	setBackgroundRole(QPalette::Dark);
	setAutoFillBackground(true);

    theImage.fill(qRgba(0, 0, 0, 0));
}

void PaintArea::clearImage()
{
	QPainter painter(&theImage);
    setupPainter(painter);
	painter.setCompositionMode(QPainter::CompositionMode_Clear);
	painter.fillRect(0,0,256,256,QColor(0, 0, 0, 0));

	update(0,0,256,256);
}

bool PaintArea::openImage(const QString &fileName)
{
    QImage image;
    if (!image.load(fileName))
        return false;

    setImage(image);
    return true;
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
    return theImage.save(fileName, fileFormat);
}

void PaintArea::setImage(const QImage &image)
{
    theImage = image.convertToFormat(QImage::Format_ARGB32 );
    update();
    updateGeometry();
}

void PaintArea::setBG(const QImage &image)
{
	bg = image.convertToFormat(QImage::Format_ARGB32 );
	br.setTextureImage( bg );
	update();
	updateGeometry();
}

void PaintArea::insertShape(const QPainterPath &path)
{
    pendingPath = path;
#ifndef QT_NO_CURSOR
    setCursor(Qt::CrossCursor);
#endif
}

void PaintArea::setBrushColor(const QColor &color)
{
    this->color = color;
}

void PaintArea::setBrushWidth(int width)
{
    thickness = width;
}

void PaintArea::setBrush(Brush *brush, const QString &brushnm)
{
    this->brush = brush;
    this->brushnm = brushnm;
}

QSize PaintArea::sizeHint() const
{
    return theImage.size();
}

void PaintArea::paintEvent(QPaintEvent * e )
{
	QPainter p( this );
 
    br.setMatrix( QMatrix(
             qreal( width() ) / qreal( bg.width() ), 0, 0,
             qreal( height() ) / qreal( bg.height() ), 0, 0 ) );
    p.setPen( Qt::NoPen );
    p.setBrush( br );
    p.drawRect( e->rect() );

    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), theImage);
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) 
	{
        if (brush) 
		{
            QPainter painter(&theImage);
            setupPainter(painter);
            const QRect rect = brush->mousePress(brushnm, painter,
                                                            event->pos());
            update(rect);
        }

        lastPos = event->pos();        
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && lastPos != QPoint(-1, -1)) {
        if (brush) {
            QPainter painter(&theImage);
            setupPainter(painter);
            const QRect rect = brush->mouseMove(brushnm, painter, lastPos,
                                                         event->pos());
            update(rect);
        }

        lastPos = event->pos();
    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && lastPos != QPoint(-1, -1)) {
        if (brush) {
            QPainter painter(&theImage);
            setupPainter(painter);
            QRect rect = brush->mouseRelease(brushnm, painter,
                                                      event->pos());
            update(rect);
        }

        lastPos = QPoint(-1, -1);
    }
}

void PaintArea::setupPainter(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(color, thickness, Qt::SolidLine, Qt::RoundCap,
                   Qt::RoundJoin));
}