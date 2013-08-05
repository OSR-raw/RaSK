struct Color;
class Texture2D
{
public:
	Texture2D( unsigned x, unsigned y, Color* texels );
	unsigned id();
	~Texture2D();
private:
	Texture2D();
	unsigned tex[1];
	Color* texels;
};