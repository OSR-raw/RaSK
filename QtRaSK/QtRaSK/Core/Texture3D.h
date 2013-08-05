struct Color;
class Texture3D
{
public:
	Texture3D( unsigned x, unsigned y, unsigned z, Color* texels );
	unsigned id();
	~Texture3D();
private:
	Texture3D();
	Texture3D( const Texture3D& in);
	unsigned tex[1];
};