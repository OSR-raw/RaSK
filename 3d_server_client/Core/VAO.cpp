#include "VAO.h"
#include "VBO.h"
#include "GraphiscOGL.h"

#include "main.h"
VAO::VAO( )
{
	sz = 0;
	//glEnableVertexAttribArray(0);
	glGenVertexArrays(1, &vaoID[0]);
}

unsigned int VAO::id()
{
	return vaoID[0];
}


void VAO::bind ( const VBO& vbo)
{
	sz += vbo.size();
	glBindVertexArray(vaoID[0]);	
	vbo.bind();
	glBindVertexArray(0);
}

unsigned int VAO::size()
{
	return sz;
}

VAO::~VAO( )
{	
	glDeleteVertexArrays(1, &vaoID[0]);	
}