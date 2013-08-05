//#include <QOpenGLFunctions>
class Shader;
class RTT//: protected QOpenGLFunctions
{
public:
	RTT(unsigned w, unsigned h);
	void bind();
	void unbind();

	unsigned rtt_id();
	unsigned tex_id();

	~RTT();
	
private:
	RTT();
	unsigned int fbufer[1];
	unsigned int renderedTexture;
	unsigned int depthrenderbuffer;
	unsigned int renderBuffer;
	unsigned int x, y;
};