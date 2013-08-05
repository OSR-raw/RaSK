#include "oglwidget.h"
#include "control/Input.h"
#include "paintarea.h"
#include <QGLFormat>
#include <QImage>
#include <QTimer>

#include <QMouseEvent>

#ifndef QT_NO_OPENGL
#include <QDebug>
#endif

OGLWidget::OGLWidget(QWidget *parent)
	: QGLWidget(parent)
{
	stereo = false;
	makeCurrent();
	GLenum error = glewInit();
	srv = NULL;
	//Server
	/*srv = new server();


	if(!srv->initConnect())
    {
            std::cerr << "Unable to iniate server" << std::endl;
            throw std::exception("Unable to iniate server");
    }*/
	render = new Render();
}

OGLWidget::~OGLWidget()
{
	delete render;
}

void OGLWidget::initializeGL()
{
	updateOverlayGL();
}

void OGLWidget::initializeGLLocal()
{
	makeCurrent();
	render->Init();
	ptimer = new QTimer(this);
    connect(ptimer, SIGNAL(timeout()), SLOT(update()));
    ptimer->start(30);
}

void OGLWidget::paintGL()
{
	makeCurrent();
	glm::mat4 obj = input->GetPosition();

	render->SetObjectMatrix(obj);
	
	if ( srv )
		srv->sendMat4(obj,obj,server::MAT4_MODEL);

	render->Draw();
	updateOverlayGL();
}

void OGLWidget::resizeGL(int w, int h)
{
	ptimer->stop();
	windowWidth = w;
	windowHeight = h;
	makeCurrent();
	glViewport(0, 0, windowWidth, windowHeight);
	render->Resize(w, h);
	updateOverlayGL();
	ptimer->start();
}

void OGLWidget::SetModelMatrix( glm::mat4 mt )
{
	render->SetObjectMatrix( mt );
	updateOverlayGL();
}


void OGLWidget::addVBO( const Vertex* vrt, const Color* clr, unsigned int len, float* uv )
{
	render->addVBO(vrt, clr, len, uv);
}

void BuildHistogram( QImage* img, unsigned x, unsigned y, unsigned z, Color* clr )
{
	unsigned char val = 0;
	double cmp0 = 0, cmp1 =0 , cmp2 =0 ;
	double min = 256.0, max = 0.0;

	double* hyst = new double[256*256];
	memset( hyst, 0, 256*256*sizeof(double) );
	
	Color* tmp;
	for (int i = 0; i < z; i++)
	{
		for (int j = 0; j < x; j++)
		{
			for (int k = 0; k < y; k++)
			{
				tmp = &clr[ i*x*y + j*y + k ];
				val = (tmp->comp[3])*255;
				if ( val )
				{					
					cmp0 = double(tmp->comp[0])*2.0- 1.0;
					cmp1 = double(tmp->comp[1])*2.0- 1.0;
					cmp2 = double(tmp->comp[2])*2.0- 1.0;
					
					double mag = sqrt( cmp0*cmp0 + cmp1*cmp1 + cmp2*cmp2 );//*255.0;

					unsigned int u_mag = (unsigned int) (mag*255.0);
					if ( u_mag > 255 )
					{
						qDebug() << u_mag;
						u_mag = 255;
					}

					hyst[val*256 + u_mag] += 1.0;

					double hyst_el = hyst[val*256 + u_mag];

					min = hyst_el < min ? hyst_el : min;
					max = hyst_el > max ? hyst_el : max;
				}
			}
		}
	}
	
	double delta = max - min;
	double invLog = 1.0/log(max + 1);

	QRgb value = qRgba(0, 0, 0, 0);
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			double intensity = hyst[ i*256+j ];
			if ( intensity )
			{
				double power = (double)(log(intensity+1) * invLog);
				int lum = (int)(power * 255);

				if ( lum != 0 )
				{
					value = qRgba(lum,lum,lum,255);
					img->setPixel(i, 255-j, value);
				}
			}
		}

	}
	
	delete[] hyst;
}

void OGLWidget::add3DTexture( unsigned x, unsigned y, unsigned z, Color* clr, Color* clr1 )
{
	update();

	render->add3DTexture( x, y, z, clr, clr1 );
		
	QImage img(256, 256, QImage::Format_ARGB32);
	img.fill(qRgba(0, 0, 0, 0));
	BuildHistogram( &img, x, y, z, clr );

	p_area->setBG(img);
	updateOverlayGL();
}

void OGLWidget::setInput( Input* inp)
{
	input = inp;
}

void OGLWidget::setPaintArea( PaintArea* arr )
{
	p_area = arr;
}

void OGLWidget::mousePressEvent(QMouseEvent *e)
{
	input->OnMouseLBDown((int)(e->localPos().x()), (int)(e->localPos().y()) );
}

void OGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	input->OnMouseLBUp((int)(e->localPos().x()), (int)(e->localPos().y()) );
}

void OGLWidget::mouseMoveEvent(QMouseEvent *e)
{
	input->OnMouseMove((int)(e->localPos().x()), (int)(e->localPos().y()) );
}

void OGLWidget::keyPressEvent( QKeyEvent* e )
{
	char c = e->text()[0].unicode() ;
	input->OnKeyPressed(c);

	if ( c == 's')
	{
		stereo = !stereo;
		render->SetStereo(stereo);
		updateOverlayGL();
	}
}

void OGLWidget::wheelEvent( QWheelEvent * event )
{
	input->OnSroll( event->delta() );
}

void OGLWidget::updateTransferFunction()
{
	if (p_area)
	{		
		render->SetTransferFunction( p_area->image().width(), p_area->image().height(), (Color*)p_area->image().bits());
		//if ( srv )
		//	srv->sendColor( (Color*)p_area->image().bits(),  p_area->image().width(), p_area->image().height() );
	}

	updateOverlayGL();
}
