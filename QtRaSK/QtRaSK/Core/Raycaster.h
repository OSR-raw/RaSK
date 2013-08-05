class Shader;
class Texture3D;
class Texture2D;
class RTT;
class VBO;
class VAO;
#include "main.h"
//#include <QOpenGLFunctions>
class Raycaster//: protected QOpenGLFunctions
{
public:
	Raycaster( unsigned w, unsigned h );
	~Raycaster();
	void Render( glm::mat4& proj, glm::mat4& view, glm::mat4& obj, int target );
	void Set3DTexture( Texture3D* text, Texture3D* text1 = 0 );
	void SetTransferFunction( Texture2D* text );

private:
	Raycaster();
	void RenderBackFace( glm::mat4& pvm );
	void createSquare();	
	//void build3Dtexture();
	Shader* backface_shader;
	Shader* framebuffer_shader;
	Texture3D* tex;
	Texture3D* tex_fmri;
	Texture2D* trans;
	VAO* vao;
	VBO* vbo;
	RTT* backface;
	unsigned windowWidth, windowHeight;
	int texID1;
	int texID3D;
	int do_color_ID;
	int cam_ID;
	int pvmMatrixLocation;
	int pvmMatrixLocationBack;
	int transLocation;
	VAO* fullscreenVAO;
	VBO* fullscreenVBO;

};