#include "VBO.h"
#include "GraphiscOGL.h"

#include "main.h"
VBO::VBO( const Vertex* vrt, const Color* clr, unsigned int len, float* uv )
{
	sz = len;

	vbo_cnt = 1;

	vbo_cnt += clr ? 1 : 0;
	vbo_cnt += uv ? 1 : 0;

	glGenBuffers(vbo_cnt, &vboID[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, len*sizeof(Vertex), vrt, GL_STATIC_DRAW);

	if ( clr )
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
		glBufferData(GL_ARRAY_BUFFER, len*sizeof(Color), clr, GL_STATIC_DRAW);  
	}
	else
		vboID[1] = 0;
	
	if ( uv )
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID[2]);
		glBufferData(GL_ARRAY_BUFFER, len*2*sizeof(float), uv, GL_STATIC_DRAW); 
	}
	else
		vboID[2] = 0;
}


const void VBO::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glVertexAttribPointer((GLuint)0, sizeof(Vertex)/sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);		
	
		
	if ( vboID[1] )
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
		glVertexAttribPointer((GLuint)1, sizeof(Color)/sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);	    
	}
	if ( vboID[2] )
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboID[2]);
		glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);	 
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


unsigned int VBO::size() const
{
	return sz;
}

VBO::~VBO()
{
	glDeleteBuffers(vbo_cnt, &vboID[0]);	
}