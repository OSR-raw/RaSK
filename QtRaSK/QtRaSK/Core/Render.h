#ifndef RENDER_H
#define RENDER_H
#include "GraphiscOGL.h"
#include "main.h"
#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "Texture3D.h"
#include "Texture2D.h"
#include "Raycaster.h"
#include "RTT.h"

class Render
{
public:
	Render();
	~Render();
	void Init();
	void SetupScene();
	void Resize(int w, int h);
	void Draw();

	void rotateModel( float angle, glm::vec3 vec );
	void zoom( float val );

	void SetObjectMatrix( glm::mat4 mt );

	void addVBO( const Vertex* vrt, const Color* clr, unsigned int len, float* uv = 0  );
	void add3DTexture( unsigned x, unsigned y, unsigned z, Color* clr, Color* clr1  );
	void SetStereo( bool val );

	void SetTransferFunction( unsigned int x, unsigned int y, Color* pixels);

private:
	void renderBasics( glm::mat4& proj, glm::mat4& view, glm::mat4& obj, int targ );
	void renderStereoBuffers();

	void CalcStereoMatr();
	int windowWidth;
	int windowHeight;

	Shader *shader;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 projectionMatrixL;
	glm::mat4 viewMatrixL;
	glm::mat4 projectionMatrixR;
	glm::mat4 viewMatrixR;
	
	vector<VAO*> vaos;
	vector<VBO*> vbos;

	Raycaster* rayc;
	RTT* rtarget1;
	RTT* rtarget2;
	VAO* fullscreenVAO;
	VBO* fullscreenVBO;
	Shader* framebuffer_shader;
	bool stereo;
	Texture3D* text3D;
	Texture3D* text3Dfmri;
	Texture2D* transfer;
	GLuint stereo_texID1;
	GLuint stereo_texID2;
	bool rerender_scene;
	bool running;

};
#endif