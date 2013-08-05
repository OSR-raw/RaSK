struct Vertex;
struct Color;


class VBO
{
public:
	VBO( const Vertex* vrt, const Color* clr, unsigned int len, float* uv = 0 );
	~VBO();
	unsigned int size() const;
	const void bind () const;
private:
	VBO();
	unsigned int vboID[3];
	//unsigned int *vboID;
	unsigned int sz;
	unsigned int vbo_cnt;
};