#include "OGLWidget.h"
#include "Input.h"
#include <ctime>


Render *OGLWidget::GetObjectFromHWnd( HWND hWnd )
{
    return reinterpret_cast<Render*>(GetWindowLong(hWnd,GWL_USERDATA));
}

LRESULT CALLBACK OGLWidget::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	Render *pWnd = GetObjectFromHWnd( hWnd );
	switch (message) {
		case WM_SIZE: // If our window is resizing
		{
			glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
			pWnd->Resize(LOWORD(lParam), HIWORD(lParam)); // Send the new window size to our OGLWidget
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

OGLWidget::OGLWidget() 
{
	LPCWSTR title = L"Ble";

	WNDCLASSW windowClass;
	HWND hWnd;
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	hInstance = GetModuleHandle(NULL);

	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = (WNDPROC) WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = title;

	if (!RegisterClassW(&windowClass)) {
		return;
	}

	hWnd = CreateWindowExW(dwExStyle, title, title, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 512, 512, NULL, NULL, hInstance, NULL);

	this->hwnd = hWnd;

	create30Context(); // Create a context given a HWND
	
	ShowWindow(this->hwnd, SW_SHOW);
    UpdateWindow(this->hwnd);
}

OGLWidget::OGLWidget(HWND hwnd) 
{
	this->hwnd = hwnd;
	create30Context(); // Create a context given a HWND
	
	ShowWindow(this->hwnd, SW_SHOW);
    UpdateWindow(this->hwnd);
}

OGLWidget::~OGLWidget() 
{
    wglMakeCurrent(hdc, 0);
    wglDeleteContext(hrc);

    ReleaseDC(hwnd, hdc); 

	delete render;
}

bool OGLWidget::create30Context() 
{
	running = true;
	render = NULL;
	hdc = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); 
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (nPixelFormat == 0)
		return false;

	int bResult = SetPixelFormat(hdc, nPixelFormat, &pfd);
	if (!bResult)
		return false;

	HGLRC tempOGLWidget = wglCreateContext(hdc);
	wglMakeCurrent(hdc, tempOGLWidget);

	GLenum error = glewInit();
	if (error != GLEW_OK)
		return false;		 

	int attributes[] = 
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1) 
	{
		hrc = wglCreateContextAttribsARB(hdc, NULL, attributes);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempOGLWidget);
		wglMakeCurrent(hdc, hrc);
	}
	else 
	{
		hrc = tempOGLWidget; 
	}

	int glVersion[2] = {-1, -1}; 
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

	std::cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << std::endl; // Output which version of OpenGL we are using
	
	render = new Render();
	render->Init();

	SetWindowLong( hwnd, GWL_USERDATA, reinterpret_cast<long>(render));

	return true; // We have successfully created a context, return true
}


void OGLWidget::doMessage()
{
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
			if (msg.message == WM_QUIT) 
			{
				running = false;
			}
			else 
			{
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
}

void OGLWidget::setInput( Input* inp)
{
	input = inp;
	render->is_left = inp->is_left;
	render->is_right = inp->is_right;
}

bool OGLWidget::alive()
{
	return running;
}

void OGLWidget::add3DTexture( unsigned x, unsigned y, unsigned z, Color* clr, Color* clr1 )
{
	render->add3DTexture( x, y, z, clr, clr1 );
}

void OGLWidget::renderScene(void)
{
	
	render->Draw();
	
	SwapBuffers(hdc);
}

void OGLWidget::updateTransferFunction( Color* img, int x, int y )
{
	if (render)
	{		
		render->SetTransferFunction( x, y, img);
	}
}

void OGLWidget::setObject( glm::mat4& obj )
{
	render->SetObjectMatrix(obj);
}

void OGLWidget::setProjection( glm::mat4& proj )
{
	render->SetProjectionMatrix(proj);
}


