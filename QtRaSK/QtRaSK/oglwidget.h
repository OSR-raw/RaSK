#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include "Core/Render.h"

#include <QGLWidget>
#include "server.h"
class Input;
class Loader;

class PaintArea;

class OGLWidget : public QGLWidget//, protected QOpenGLFunctions
{
	Q_OBJECT
		public slots:
		void paintGL();
public:
	explicit OGLWidget(QWidget *parent);
	~OGLWidget();
	void initializeGL();
	void initializeGLLocal();
	void resizeGL(int w, int h);
	

	void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

	void keyPressEvent( QKeyEvent* e );

	void wheelEvent ( QWheelEvent * event );

	void SetModelMatrix( glm::mat4 mt );

	void addVBO( const Vertex* vrt, const Color* clr, unsigned int len, float* uv = 0  );
	void add3DTexture( unsigned x, unsigned y, unsigned z, Color* clr, Color* clr1 = 0 );
	void setInput( Input* );

	void setPaintArea( PaintArea* arr );
	void updateTransferFunction();

private:
	int windowWidth;
	int windowHeight;
	bool stereo;
	Input *input;
	PaintArea* p_area;
	Render* render;
	QTimer* ptimer;

	
	//Server
	server* srv;
};

#endif // OGLWIDGET_H
