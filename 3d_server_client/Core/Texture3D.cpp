#include "Texture3D.h"
#include "GraphiscOGL.h"
//#include <qopenglext.h>
#include "main.h"

Texture3D::Texture3D( unsigned x, unsigned y, unsigned z, Color* texs )
{
	//glPixelStoref(GL_UNPACK_ALIGNMENT,1);
	
//	QOpenGLFunctions fun;
	glGenTextures(1, &tex[0]);
	glBindTexture(GL_TEXTURE_3D, tex[0]);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	//glTexParameteri(GL_TEXTURE_3D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, x, y, z, 0, GL_RGBA, GL_FLOAT, texs);
	//glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, x, y, z, 0, GL_RGBA, GL_UNSIGNED_BYTE, texs);

	//glGenerateMipmap(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, 0);
}

unsigned Texture3D::id()
{
	return tex[0];
}

Texture3D::~Texture3D()
{
	glDeleteTextures(1, &tex[0]);
}