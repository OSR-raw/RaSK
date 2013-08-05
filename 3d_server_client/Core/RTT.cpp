//#include <qopenglext.h>
#include "RTT.h"
#include "GraphiscOGL.h"
#include "main.h"
#include "Shader.h"
RTT::RTT(unsigned w, unsigned h)
{
	x = w;
	y = h;

	//RT
	glGenFramebuffersEXT(1, &fbufer[0]);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbufer[0]);

	//Color tex
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0, GL_RGB,GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, w, h, 0, GL_RGBA,GL_UNSIGNED_BYTE, 0);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Depth tex
	glGenTextures(1, &depthrenderbuffer);
	glBindTexture(GL_TEXTURE_2D, depthrenderbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, w, h, 0, GL_DEPTH_COMPONENT,GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthrenderbuffer, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenRenderbuffersEXT(1, &renderBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT32F, w, h);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderBuffer);
	
	

	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	int res = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
}

unsigned RTT::rtt_id()
{
	return fbufer[0];
}

unsigned RTT::tex_id()
{
	return renderedTexture;
}


void RTT::bind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbufer[0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

}

void RTT::unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}


RTT::~RTT()
{
	glDeleteTextures(1, &renderedTexture);
	glDeleteTextures(1, &depthrenderbuffer);	
	glDeleteRenderbuffers(1, &renderBuffer);
	glDeleteFramebuffers(1, &fbufer[0]);
}