#include "brush.h"

Brush::Brush(QObject *parent)
	: QObject(parent)
{

}

Brush::~Brush()
{

}
QRect Brush::mousePress(const QString &brush, QPainter &painter,
                                   const QPoint &pos)
{
	painter.save();
	painter.setCompositionMode(QPainter::CompositionMode_Plus);
	painter.drawPoint(pos);
	painter.restore();

	int rad = painter.pen().width() / 2;
    QRect boundingRect = QRect(pos, pos).normalized()
                                              .adjusted(-rad, -rad, +rad, +rad);
    return boundingRect;
}

QRect Brush::mouseMove(const QString &brush, QPainter &painter,
                                  const QPoint &oldPos, const QPoint &newPos)
{
    painter.save();

    int rad = painter.pen().width() / 2;
    QRect boundingRect = QRect(oldPos, newPos).normalized()
                                              .adjusted(-rad, -rad, +rad, +rad);

	painter.setCompositionMode(QPainter::CompositionMode_Plus);
    if (brush == tr("Brush")) 
	{
        painter.drawLine(oldPos, newPos);
    } 

    painter.restore();
    return boundingRect;
}

QRect Brush::mouseRelease(const QString & /* brush */,
                                     QPainter & /* painter */,
                                     const QPoint & /* pos */)
{
    return QRect(0, 0, 0, 0);
}