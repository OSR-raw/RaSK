#include "../oglwidget.h"
#include "Loader.h"


Loader::Loader( OGLWidget* const cnt  ): dib(NULL), bits(NULL), colors(NULL), _cntx(cnt), fdata(NULL)
{

}

void Loader::Clear()
{	
	width = 0;
	height = 0;

	if (dib)
		FreeImage_Unload(dib);

	dib = NULL;
}

Loader::~Loader()
{
	Clear();
	if (colors)
		delete [] colors; 
}

void build3Dtexture( Color* txt, unsigned x, unsigned y, unsigned z )
{
	Color* cmp;
	for ( unsigned i = 1; i < z-1; i++ )
	{
		for ( unsigned j = 1; j < x-1; j++ )
		{
			for ( unsigned k = 1; k < y-1; k++ )
			{
				cmp = &txt[ i*y*x + j*y + k ];
	
				cmp->comp[3] = 64;
				if ( k > y/4  && k < y/2 )
				{
					cmp->comp[3] = 255;
				}
			}
		}
	}
}

void GenerateGradients( Color* in, unsigned x, unsigned y, unsigned z )
{
	Color* tmp;
	double len = sqrt(3.0);
	double cmp0, cmp1, cmp2;
	for ( int i = 0; i < z; i++ )
	{
		for ( int j = 0;  j< x; j++ )
		{
			for ( int k = 0; k < y; k++ )
			{
				tmp = &in[ i*x*y + j*y + k ];

				cmp0 = ((double)( ((i > 0)? in[ (i-1)*x*y + j*y + k].comp[3] : 0) - ( ( i < z-1) ? in[ (i+1)*x*y + j*y + k].comp[3]: 0) ));//255.0;
				cmp1 = ((double)( ((j > 0)? in[ i*x*y + (j-1)*y + k].comp[3] : 0) - ( ( j < x-1) ? in[ i*x*y + (j+1)*y + k].comp[3]: 0) ));//255.0;
				cmp2 = ((double)( ((k > 0)? in[ i*x*y + j*y + k - 1].comp[3] : 0) - ( ( k < y-1) ? in[ i*x*y + j*y + k + 1].comp[3]: 0) ));//255.0;
		
				len = sqrt( cmp0*cmp0 + cmp1*cmp1 + cmp2*cmp2 );

				if ( len > 1.0 )
				{
					cmp0 /= len;
					cmp1 /= len;
					cmp2 /= len;
				}

				tmp->comp[0] = (( cmp0 + 1.0 ))/2.0;//*127.5);
				tmp->comp[1] = (( cmp1 + 1.0 ))/2.0;//*127.5);
				tmp->comp[2] = (( cmp2 + 1.0 ))/2.0;//*127.5);
			}
		}
	}
}

void LoadFMRI( Color* in, Color* fdata, unsigned x, unsigned y, unsigned z )
{
	FILE *infile, *infile1;
    fopen_s(&infile, "../../FMRI/subj.dat", "r");
	fopen_s(&infile1, "../../FMRI/speech.dat", "r");
	float d, d1;
	double mx = 0.0;
	for ( int i = 0; i < z; i++ )
	{
		for ( int j = 0;  j< x; j++ )
		{
			for ( int k = 0; k < y; k++ )
			{
				fscanf_s(infile,"%f",&d);
				fscanf_s(infile1,"%f",&d1);
				in[ i*x*y + j*y + k ].comp[3] = d/927.0;
				if ( d1 > 0 )
					fdata[ i*x*y + j*y + k ].comp[0] = d1/10.0;
				else
					fdata[ i*x*y + j*y + k ].comp[2] = -d1/5.0;

				mx = (d1 < mx)? d1: mx;
			}
		}
	}


	fclose(infile);
	fclose(infile1);
	return;
}

void Loader::DoLoadData()//freaking hack. sorry.
{
	if( 0 )//FMRI
	{
		width = 63;
		height = 46;
		depth = 53;
		colors = new Color[width*height*depth];
		memset( colors, 0, width*height*depth*sizeof(Color) );

		fdata = new Color[width*height*depth];
		memset( fdata, 0, width*height*depth*sizeof(Color) );

		LoadFMRI(colors,fdata, width, height, depth);

		GenerateGradients(colors, width, height, depth);
		PostToRender();
		delete [] colors;
		delete [] fdata;
		return;
	}

	unsigned  x,y;

	if ( 1 )
	{
		x = 256;
		y = 256;
		depth = 256;
		colors = new Color[x*y*depth];
		memset( colors, 0, x*y*depth*sizeof(Color) );
		char buffer[50];
		for ( unsigned i = 1; i < 100; i++ )
		{
			sprintf_s (buffer, "../cthead-8bit/cthead-8bit%03d.tif", i);
			LoadTGA(buffer);
			ProcessImage( colors + x*y*(i) );
		}
		GenerateGradients(colors, x, y, depth);
		PostToRender();
		delete [] colors; 
	}
	else
	{
		x = 512;
		y = x;
		depth = 32;
		colors = new Color[x*y*depth];
		memset( colors, 0, x*y*depth*sizeof(Color) );
		char buffer[50];
		for ( unsigned i = 1; i <= 24; i++ )
		{
			sprintf_s (buffer, "../Tif/IM%d.tif", i);
			LoadTGA(buffer);
			ProcessImage( colors + x*y*(i-1) );
		}
		GenerateGradients(colors, x, y, depth);
		PostToRender();
		delete [] colors; 
	}
	colors = NULL;
}

bool Loader::LoadTGA(const char* filename)
{
	Clear();

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure


	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);


	return true;
}

bool Loader::ProcessImage( Color* col )
{
	memset ( col, 0, height*width*sizeof(Color) );
	for ( unsigned i = 0; i < width; i++ )
	{
		for ( unsigned j = 0; j < height; j++ )
		{
			//if ( bits[i* height + j] > 35)
			{
				col[i* height + j].comp[3] =  (float)bits[i* height + j]/255.0f;
			}
		}	
	}

	return true;
}

void Loader::PostToRender()
{
	_cntx->add3DTexture( width, height, depth, colors, fdata);
}