#include "Raycaster.h"
#include "Texture3D.h"
#include "Texture2D.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "RTT.h"
#include "main.h"
#include "GraphiscOGL.h"
Raycaster::Raycaster( unsigned w, unsigned h ): windowWidth(w), windowHeight(h), tex(NULL)
{
	createSquare();
	backface_shader = new Shader();
	backface_shader->loadFragmentShader("Shaders/shader_backface.frag");
	backface_shader->loadVertexShader("Shaders/shader_backface.vert");
	backface_shader->link();
	
	//QOpenGLFunctions fun;
	pvmMatrixLocationBack = glGetUniformLocation(backface_shader->id(), "PVM");

	framebuffer_shader = new Shader();
	framebuffer_shader->loadFragmentShader("Shaders/shader_raycast.frag");
	framebuffer_shader->loadVertexShader("Shaders/shader_raycast.vert");
	framebuffer_shader->link();

	texID1 = glGetUniformLocation(framebuffer_shader->id(), "backface");
	texID3D = glGetUniformLocation(framebuffer_shader->id(), "text3d");
	do_color_ID = glGetUniformLocation(framebuffer_shader->id(), "do_color");
	cam_ID = glGetUniformLocation(framebuffer_shader->id(), "cam_view");
	pvmMatrixLocation = glGetUniformLocation(framebuffer_shader->id(), "pvm");
	transLocation = glGetUniformLocation(framebuffer_shader->id(), "transfer");

	backface = new RTT( w, h );

	const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	fullscreenVAO = new VAO();
	fullscreenVBO = new VBO( (Vertex*)g_quad_vertex_buffer_data, 0, 6 );
	fullscreenVAO->bind(*fullscreenVBO);
}

void Raycaster::RenderBackFace( glm::mat4& pvm )
{
	//glCullFace(GL_FRONT);
	//glDisable(GL_BLEND);
	backface_shader->bind();
	glUniformMatrix4fv(pvmMatrixLocationBack, 1, GL_FALSE, &pvm[0][0]);

	glBindVertexArray(vao->id());	
	glDrawArrays(GL_TRIANGLES, 0, vao->size());
	backface_shader->unbind();
	//glDisable(GL_CULL_FACE);
}

void Raycaster::Render( glm::mat4& proj, glm::mat4& view, glm::mat4& obj, int target  )
{
	if (!tex || !trans)
		return;

	glm::mat4 pvo = proj*view*obj;

	glEnable(GL_CULL_FACE);

	backface->bind();
	RenderBackFace( pvo );
	backface->unbind();	
	
	
	glBindFramebufferEXT(GL_FRAMEBUFFER, target);
	
	//glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	framebuffer_shader->bind();

	glUniformMatrix4fv(pvmMatrixLocation, 1, GL_FALSE, &pvo[0][0]);
	glUniform1i(do_color_ID, 1);

	glm::vec4 cam_pos4(0,0,0,1);
	
	cam_pos4 = cam_pos4*(view*obj)._inverse();
	cam_pos4 = cam_pos4/cam_pos4.w;
	glm::vec3 cam_pos( cam_pos4.x, cam_pos4.y, cam_pos4.z );

	cam_pos = (cam_pos + 1.0f)/2.0f;

	glUniform3f( cam_ID, cam_pos[0], cam_pos[1], cam_pos[2] );

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backface->tex_id());
	glUniform1i(texID1, 0);

	
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, tex->id());
	glUniform1i(texID3D, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, trans->id());
	glUniform1i(transLocation, 2);

	if ( tex_fmri )
	{
		glActiveTexture(GL_TEXTURE3);
		glEnable(GL_TEXTURE_3D);
		glBindTexture(GL_TEXTURE_3D, tex_fmri->id());
		int tt = glGetUniformLocation(framebuffer_shader->id(), "text3d_fmri");
		glUniform1i(tt, 3);
	}
	glBindVertexArray(vao->id());	
	glDrawArrays(GL_TRIANGLES, 0, vao->size());

	
	glCullFace(GL_FRONT);
	glUniform1i(do_color_ID, 0);	
	glDrawArrays(GL_TRIANGLES, 0, vao->size());
	

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);
	//glActiveTexture(GL_TEXTURE0);
	glDisable(GL_BLEND);
	backface_shader->unbind();
}

void Raycaster::Set3DTexture( Texture3D* text, Texture3D* text1 )
{
	tex = text;
	tex_fmri = text1;
}

void Raycaster::SetTransferFunction( Texture2D* text )
{
	trans = text;
}

Raycaster::~Raycaster()
{
	delete vao;
	delete vbo;
	delete backface;
	delete backface_shader;
	delete framebuffer_shader;
	delete fullscreenVAO;
	delete fullscreenVBO;
}

GLfloat vertices[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
		{1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0},
{1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}};
 
 
void polygon(int a, int b, int c , int d, Vertex* vert, Color* clr)
{
  vert[0].coord[0] = vertices[a][0];
  vert[0].coord[1] = vertices[a][1];
  vert[0].coord[2] = vertices[a][2];
  
  vert[1].coord[0] = vertices[b][0];
  vert[1].coord[1] = vertices[b][1];
  vert[1].coord[2] = vertices[b][2];

  vert[2].coord[0] = vertices[c][0];
  vert[2].coord[1] = vertices[c][1];
  vert[2].coord[2] = vertices[c][2];

  vert[3].coord[0] = vertices[c][0];
  vert[3].coord[1] = vertices[c][1];
  vert[3].coord[2] = vertices[c][2];

  vert[4].coord[0] = vertices[d][0];
  vert[4].coord[1] = vertices[d][1];
  vert[4].coord[2] = vertices[d][2];

  vert[5].coord[0] = vertices[a][0];
  vert[5].coord[1] = vertices[a][1];
  vert[5].coord[2] = vertices[a][2];


  for ( int i = 0; i < 6; i++)
  {
	  clr[i].comp[0] = 1;
	  clr[i].comp[1] = 1;
	  clr[i].comp[2] = 1;
	  clr[i].comp[3] = 1;
  }

}

void Raycaster::createSquare() 
{
	unsigned size = 6*6;
	Vertex* verts = new Vertex[size];
	Color* colors = new Color[size];
	//back
	polygon(0,3,2,1, &verts[0], &colors[0]);
 
	//top
	polygon(2,3,7,6, &verts[6], &colors[6]);
 
	//left
	polygon(0,4,7,3, &verts[12], &colors[12]);
 
	//right
	polygon(1,2,6,5, &verts[18], &colors[18]);
 
	//front
	polygon(4,5,6,7, &verts[24], &colors[24]);
 
	//bottom
	polygon(0,1,5,4, &verts[30], &colors[30]);

	GLfloat g_uv_buffer_data[] = { 
		0.000059f, 1.0f-0.000004f, 
		0.000103f, 1.0f-0.336048f, 
		0.335973f, 1.0f-0.335903f, 
		1.000023f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.336024f, 1.0f-0.671877f, 
		0.667969f, 1.0f-0.671889f, 
		1.000023f, 1.0f-0.000013f, 
		0.668104f, 1.0f-0.000013f, 
		0.667979f, 1.0f-0.335851f, 
		0.000059f, 1.0f-0.000004f, 
		0.335973f, 1.0f-0.335903f, 
		0.336098f, 1.0f-0.000071f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.336024f, 1.0f-0.671877f, 
		1.000004f, 1.0f-0.671847f, 
		0.999958f, 1.0f-0.336064f, 
		0.667979f, 1.0f-0.335851f, 
		0.668104f, 1.0f-0.000013f, 
		0.335973f, 1.0f-0.335903f, 
		0.667979f, 1.0f-0.335851f, 
		0.335973f, 1.0f-0.335903f, 
		0.668104f, 1.0f-0.000013f, 
		0.336098f, 1.0f-0.000071f, 
		0.000103f, 1.0f-0.336048f, 
		0.000004f, 1.0f-0.671870f, 
		0.336024f, 1.0f-0.671877f, 
		0.000103f, 1.0f-0.336048f, 
		0.336024f, 1.0f-0.671877f, 
		0.335973f, 1.0f-0.335903f, 
		0.667969f, 1.0f-0.671889f, 
		1.000004f, 1.0f-0.671847f, 
		0.667979f, 1.0f-0.335851f
	};

	vao = new VAO();
	vbo = new VBO( verts, colors, size );
	vao->bind( *vbo );

	delete[] colors;
	delete[] verts;
}