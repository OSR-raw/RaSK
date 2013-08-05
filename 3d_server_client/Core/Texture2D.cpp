#include "Texture2D.h"
#include "GraphiscOGL.h"
//#include <qopenglext.h>
#include "main.h"

Texture2D::Texture2D( unsigned x, unsigned y, Color* texs )
{
	texels = texs;
	glGenTextures(1, &tex[0]);
	glBindTexture(GL_TEXTURE_2D, tex[0]);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, 
				 GL_UNSIGNED_BYTE, texels);

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);*/
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned Texture2D::id()
{
	return tex[0];
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &tex[0]);
	//delete texels;
}