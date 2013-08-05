#include "Input.h"
#include "oglwidget.h"


Input::Input(OGLWidget* const cnt): _cntx(cnt)
{
	is_left = false;
	is_right = false;
	cl=new client(); 

	if ( cl->eyeSide == client::LEFT )
		is_left = true;
	else
		is_right = true;

	//Iniate
	if(!cl->initConnect())
	{
		std::cerr << "Unable to connect with server" << std::endl;
		throw std::exception("Unable to connect with server");
	}
}

void Input::DoRecieve()
{
	client::dataType enumType;
    void* data = NULL; //Vod pointer to void pointer
    int x = 0;
    int y = 0;
    int* idPtr=new int(0);

	if(!cl->receiveData(&enumType,&data,idPtr,&x,&y))
    {
            return;
    }

    //Print some stuff so we know it works
    if(enumType==client::dataType::COLOR)
    {
            int sizeArr=(x)*(y);
           // std::cerr << "Printing color, id " << *idPtr << " x " << x << " y " << y << std::endl;
            //Cast to color*
            Color* colArr=(Color*)data;

            //Print
           /* for(int i=0; i<sizeArr;++i)
            {
                    for(int j=0; j<4;++j) std::cerr << "Color pos " << i << " comp " << j << " value " << colArr[i].comp[j] << std::endl;
            }*/

			_cntx->updateTransferFunction(colArr,x,y);

    }
    else if(enumType==client::dataType::MAT4_MODEL)
    {
            //Cast to mat4
            glm::mat4* matrix=(glm::mat4*)data;

            //Print
    /*        const int size=4;
            for(int i=0; i<size;++i){
                    glm::vec4 vec=matrix->operator[](i);
                    for(int j=0; j<size;++j) std:: cerr << "Matrix pos: " << i << " " << j << " value " << vec[j] << std::endl;
            }*/

			_cntx->setObject(*matrix);
    }
	else if (enumType==client::dataType::MAT4_PROJECTION)
	{
		glm::mat4* matrix=(glm::mat4*)data;
		_cntx->setProjection(*matrix);
	}
    else
    {
            std::cerr << "Enum not recognised" << std::endl;
            throw std::exception("Enum not recognised");
    }

	if ( data )
		delete [] data;

    delete idPtr;
}

Input::~Input()
{
	delete cl;
}

