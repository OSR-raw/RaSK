#ifndef BRUSH_H
#define BRUSH_H

#include <QRect>
#include <QPainter>
#include <QImage>
#include <QObject>

class Brush : public QObject
{
	Q_OBJECT

public:
	Brush(QObject *parent);
	QRect mousePress(const QString &brush, QPainter &painter,
                     const QPoint &pos);
    QRect mouseMove(const QString &brush, QPainter &painter,
                    const QPoint &oldPos, const QPoint &newPos);
    QRect mouseRelease(const QString &brush, QPainter &painter,
                       const QPoint &pos);

	~Brush();

private:
	
};

#endif // BRUSH_H
