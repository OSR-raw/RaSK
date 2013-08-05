#ifndef __LIBS
#define __LIBS
#include "GL\glew.h"
#include "GL\wglew.h"

#ifdef _M_IX86
	#pragma comment(lib, "glew32.lib")
	//#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "FreeImage.lib")
	
#if _DEBUG
	#pragma comment(lib, "libboost_thread-vc110-mt-gd-1_53.lib")
	#pragma comment(lib, "libboost_date_time-vc110-mt-gd-1_53.lib")
	#pragma comment(lib, "libboost_system-vc110-mt-gd-1_53.lib")
	#pragma comment(lib, "libboost_chrono-vc110-mt-gd-1_53.lib")
	#pragma comment(lib, "libboost_filesystem-vc110-mt-gd-1_53.lib")
#else
	#pragma comment(lib, "libboost_thread-vc110-mt-1_53.lib")
	#pragma comment(lib, "libboost_date_time-vc110-mt-1_53.lib")
	#pragma comment(lib, "libboost_system-vc110-mt-1_53.lib")
	#pragma comment(lib, "libboost_chrono-vc110-mt-1_53.lib")
	#pragma comment(lib, "libboost_filesystem-vc110-mt-1_53.lib")
#endif
#else
	#ifdef _M_X64
		#pragma comment(lib, "glew64.lib")
		#pragma comment(lib, "FreeImage64.lib")
	#endif
#endif
#define QT_NO_OPENGL_ES
#endif __LIBS
