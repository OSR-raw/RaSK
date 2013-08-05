#ifndef __CONTEXT
#define __CONTEXT

#include "../Core/GraphiscOGL.h"
#include "../Core/main.h"
#include "../Core/Render.h"

class Input;
class Loader;

class OGLWidget {
public:
	OGLWidget();
	~OGLWidget();

	void renderScene();
	void doMessage();
	bool alive();
	void setInput( Input* );
	void setupScene();
	void add3DTexture( unsigned x, unsigned y, unsigned z, Color* clr, Color* clr1 = 0 );
	void updateTransferFunction( Color* img, int x, int y );
	void setObject( glm::mat4& obj );
	void setProjection( glm::mat4& obj );
private:	
	bool create30Context();
	OGLWidget(HWND hwnd);

	Render* render;
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static Render* GetObjectFromHWnd( HWND hWnd );
	Input *input;
		

protected:
	HINSTANCE hInstance;
	HGLRC hrc; // Rendering context
	HDC hdc; // Device context
	HWND hwnd; // Window identifier
	MSG msg;
	bool running;
};

#endif 