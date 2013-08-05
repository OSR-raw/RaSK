//#include "GraphiscOGL.h"
#include "Input.h"
#include <math.h>

#include "Render.h"
Render::Render()
{
	running = true;
	rtarget1 = NULL;
	rtarget2 = NULL;
	is_left = false;
	is_right = false;
	rayc = NULL;
	text3D = NULL;
	text3Dfmri = NULL;
	transfer = NULL;
	rerender_scene = true;
	windowWidth = 512;
	windowHeight = 512;
}

Render::~Render()
{
	delete shader;
	delete text3D;

	if (text3Dfmri)
		delete text3Dfmri;

	for ( unsigned int i = 0; i < vaos.size(); i++ )
		delete vaos[i];

	for ( unsigned int i = 0; i < vbos.size(); i++ )
		delete vbos[i];

	delete rtarget1;
	delete rtarget2;
	delete rayc;

	delete fullscreenVAO;
	delete fullscreenVBO;
	delete framebuffer_shader;
	delete transfer;
}

void Render::Init()
{
	SetupScene();
}

void Render::Draw()
{
	if ( stereo )
	{
		//if ( rerender_scene )
		{
			renderBasics( projectionMatrixR, viewMatrixR, modelMatrix, rtarget1->rtt_id() );		
			renderBasics( projectionMatrixL, viewMatrixL, modelMatrix, rtarget2->rtt_id() );	
			rerender_scene = false;
		}
		renderStereoBuffers();
	}
	else
	{		
		renderBasics( projectionMatrix, viewMatrix, modelMatrix, 0 );
	}
	//glFlush();
}

void Render::CalcStereoMatr()
{
	float focall = 0.01f;
	float displ = focall/60.0f;

	float ratio = (float)windowWidth / (float)windowHeight;
	glm::vec3 vp = glm::vec3(0.0f, 0.0f, 1.f);
	glm::vec3 r = glm::vec3(1.0f, 0.0f, 0.0f);
	r *= displ/2.0f;

	float radians = (float)(180.0/3.14159265359 * 60.0 / 2.0);
	float wd2     = 0.01f * tan(radians);
	float ndfl    = (float)(0.01 / focall);

		
	float left  = (float)(- ratio * wd2 - 0.5 * displ * ndfl);
	float right =   (float)(ratio * wd2 - 0.5 * displ * ndfl);
	float top    =   wd2;
	float bottom = - wd2;

	projectionMatrixR = glm::frustum(left,right,bottom,top,0.01f,1000.f);
	viewMatrixR = glm::lookAt( vp + r, vp + r - glm::vec3(0,0,1), glm::vec3(0,1,0));

	left  = (float)(- ratio * wd2 + 0.5 * displ * ndfl);
	right =  (float)( ratio * wd2 + 0.5 * displ * ndfl);


	projectionMatrixL = glm::frustum(left,right,bottom,top,0.01f,1000.f);
	viewMatrixL = glm::lookAt( vp - r, vp - r - glm::vec3(0,0,1), glm::vec3(0,1,0));

}

void Render::SetupScene()
{
	//glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader = new Shader();
	shader->loadFragmentShader("Shaders/shader.frag");
	shader->loadVertexShader("Shaders/shader.vert");
	shader->link();
	
	projectionMatrix = glm::perspective(60.0f, (float)windowWidth / (float)windowHeight, 0.01f, 100.f);
	viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.f));

	modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	CalcStereoMatr();

	//return;
	if (rayc)
		delete rayc;
	rayc = new Raycaster(windowWidth, windowHeight );
	
	//Stereo data
	if ( rtarget1 )
		delete rtarget1;
	rtarget1 = new RTT( windowWidth, windowHeight );

	if ( rtarget2 )
		delete rtarget2;
	rtarget2 = new RTT( windowWidth, windowHeight );

	framebuffer_shader = new Shader();
	framebuffer_shader->loadFragmentShader("Shaders/stereoblend.frag");
	framebuffer_shader->loadVertexShader("Shaders/stereoblend.vert");
	framebuffer_shader->link();

	stereo_texID1 = glGetUniformLocation(framebuffer_shader->id(), "renderedTexture1");
	stereo_texID2 = glGetUniformLocation(framebuffer_shader->id(), "renderedTexture2");

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
	stereo = 0;
	glViewport(0, 0, windowWidth, windowHeight);
}

void Render::Resize(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
	//glViewport(0, 0, windowWidth, windowHeight);
	projectionMatrix = glm::perspective(60.0f, (float)windowWidth / (float)windowHeight, 0.01f, 100.f);

	if ( rtarget1 )
		delete rtarget1;
	rtarget1 = new RTT( windowWidth, windowHeight );

	if ( rtarget2 )
		delete rtarget2;
	rtarget2 = new RTT( windowWidth, windowHeight );

	if (rayc)
		delete rayc;
	rayc = new Raycaster(windowWidth, windowHeight );
	rayc->Set3DTexture( text3D, text3Dfmri );
	rayc->SetTransferFunction(transfer);

	CalcStereoMatr();

	if ( is_left )
	{
		projectionMatrix = projectionMatrixL;
		viewMatrix = viewMatrixL;
	}

	if ( is_right )
	{
		projectionMatrix = projectionMatrixR;
		viewMatrix = viewMatrixR;
	}

	rerender_scene = true;
}


void Render::SetObjectMatrix( glm::mat4& mt )
{
	modelMatrix = mt;
	rerender_scene = true;
}

void Render::renderBasics( glm::mat4& proj, glm::mat4& view, glm::mat4& obj, int targ )
{
	if ( rayc )
		rayc->Render( proj, view, obj, targ );

	//glFinish();
}

void Render::renderStereoBuffers()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	glDisable(GL_BLEND);
	framebuffer_shader->bind();

	/*static bool even = true;
	glEnable(GL_TEXTURE_2D);
	if (even)
	{
		glDrawBuffer(GL_BACK_LEFT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rtarget1->tex_id());
		glUniform1i(stereo_texID1, 0);

	}
	else
	{
		glDrawBuffer(GL_BACK_RIGHT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rtarget2->tex_id());
		glUniform1i(stereo_texID1, 0);

	}

	even = !even;
	*/
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rtarget1->tex_id());
	glUniform1i(stereo_texID1, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rtarget2->tex_id());
	glUniform1i(stereo_texID2, 1);

	
	glBindVertexArray(fullscreenVAO->id());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	framebuffer_shader->unbind();
	//glFinish();
	//glEnable(GL_BLEND);
}

void Render::addVBO( const Vertex* vrt, const Color* clr, unsigned int len, float* uv )
{
	VBO* vbo = new VBO(vrt, clr, len, uv );
	vbos.push_back( vbo );
	VAO* vao = new VAO();
	vao->bind(*vbo);
	vaos.push_back( vao );
}

void Render::add3DTexture( unsigned x, unsigned y, unsigned z, Color* clr, Color* clr1 )
{
	if (text3D)
		delete text3D;
	
	text3D = new Texture3D( x, y, z, clr );

	if (text3Dfmri)
		delete text3Dfmri;

	if (clr1)
	{
		text3Dfmri = new Texture3D( x, y, z, clr1 );
	}

	rayc->Set3DTexture( text3D, text3Dfmri );
}

void Render::SetTransferFunction( unsigned int x, unsigned int y, Color* pixels)
{
	if ( transfer )
		delete transfer;
	
	transfer = new Texture2D( x, y, pixels);

	if (rayc) 
		rayc->SetTransferFunction(transfer);
}

void Render::SetStereo(bool val)
{
	stereo = val;
}

void Render::SetProjectionMatrix( glm::mat4& mt )
{
	projectionMatrix = mt;
	rerender_scene = true;
}