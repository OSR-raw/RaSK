#include "OGLWidget.h"
#include "..\Core\Loader.h"
#include "Input.h"
#include "client.h"

int main( int argc, char** argv) 
//int main(int argc, char *argv[])
{

	OGLWidget openglContext;
	
	Input inp( &openglContext );
	openglContext.setInput(&inp);

	int w,h;
	w = 256;
	h = 256;
	Color* tf = new Color[w*h];
	memset(tf, 1, w*h);

	openglContext.updateTransferFunction(tf, w, h);

	
	Loader ldr(&openglContext);
	
	//Load some staff
	ldr.DoLoadData();
	//end load

	while (openglContext.alive())
	{
		openglContext.doMessage();
		inp.DoRecieve();
		openglContext.renderScene();
	}
	delete [] tf;
	return 0;
}
