#include "Includes\FreeImage.h"
#include "main.h"


class Loader
{
public:
	Loader( OGLWidget* const cnt );
	~Loader();

	void DoLoadData();
	
private:
	bool LoadTGA(const char*);
	bool ProcessImage( Color* col );
	void PostToRender();
	Loader();
	void Clear();

	//pointer to the image, once loaded
	FIBITMAP *dib;

	//pointer to the image data
	BYTE* bits;
	//image width and height
	unsigned int width, height, depth;

	OGLWidget* const _cntx;

	Color* colors;
	//tmp
	Color* fdata;
};