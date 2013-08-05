#include "client.h"
//#include "../Core/main.h"
class OGLWidget;

class Input
{
public:
	Input(OGLWidget* const cnt);
	~Input();
	void DoRecieve();
	bool is_left;
	bool is_right;

private:
	Input();
	OGLWidget* const _cntx;
	client* cl;
};