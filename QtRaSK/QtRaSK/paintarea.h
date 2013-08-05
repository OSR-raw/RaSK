#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QColor>
#include <QImage>
#include <QPainterPath>
#include <QWidget>

class Brush;

class PaintArea : public QWidget
{
    Q_OBJECT

public:
    PaintArea(QWidget *parent = 0);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setImage(const QImage &image);
    void insertShape(const QPainterPath &path);
    void setBrushColor(const QColor &color);
    void setBrushWidth(int width);
    void setBrush(Brush *brush, const QString &brushnm);
	void clearImage();
	void setBG(const QImage &image);

    QImage image() const { return theImage; }
    QColor brushColor() const { return color; }
    int brushWidth() const { return thickness; }
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void setupPainter(QPainter &painter);

    QImage theImage;

	QImage bg;
	QBrush br;

    QColor color;
    int thickness;

    Brush *brush;
    QString brushnm;
    QPoint lastPos;

    QPainterPath pendingPath;
};

#endif