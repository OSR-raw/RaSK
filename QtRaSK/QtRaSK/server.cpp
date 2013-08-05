#include "server.h"


server::server():colorId(new int(0)),matrixId(new int(0)),context(new zmq::context_t(1)),leftSocket(new zmq::socket_t (*context, ZMQ_REQ)),rightSocket(new zmq::socket_t (*context, ZMQ_REQ)),LEFT_PORT(5555),RIGHT_PORT(LEFT_PORT+1)
{
        readClientIps();
}

server::~server()
{
	leftSocket->close();
	rightSocket->close();
    delete leftSocket;
    delete rightSocket;
    delete context;
}

void server::readClientIps()
{
        //Get client ips from file
        //Read ip from file
        std::cerr << "Reading ip from file" << std::endl;
        std::string line;
        std::ifstream ipFile ("../server_client_ip.txt");
        if(ipFile.is_open())
        {
                while(ipFile.good())
                {
                        getline (ipFile,line);
                        //Check for server ip
                        unsigned pos=line.find("Client right:");
                        if(pos!=std::string::npos)
                        {
                                RIGHT_IP=line.substr(13,line.size()-13);
                        }
                        pos=line.find("Client left:");
                        if(pos!=std::string::npos)
                        {
                                LEFT_IP=line.substr(12,line.size()-12);
                        }
                }
                ipFile.close();
        }
        else
        {
                std::cerr << "Unable to open file" << std::endl;
                throw std::exception("Unable to open file");
        }
}

bool server::initConnect()
{
        //Open socket and such
        std::cerr << "Connecting to clients right: " << RIGHT_PORT << " left " << LEFT_PORT << std::endl;
        try
        {
                //Connect leftSocket to LEFT_PORT
                std::ostringstream ossLeft;
                ossLeft << "tcp://" << LEFT_IP  << ":" << LEFT_PORT;
                const std::string& tmpLeft = ossLeft.str();
                const char* cLeft = tmpLeft.c_str();
                leftSocket->connect (cLeft);
                std::cerr << "Connected with left " << zmq_strerror(errno) << std::endl;

                //Connect rightSocket to RIGHT_PORT
                std::ostringstream ossRight;
                ossRight << "tcp://" << RIGHT_IP  << ":" << RIGHT_PORT;
                const std::string& tmpRight = ossRight.str();
                const char* cRight = tmpRight.c_str();
                rightSocket->connect (cRight);
                ossRight << "Connected with right " << zmq_strerror(errno) << std::endl;
        }
        catch(zmq::error_t e)
        {
                printf(e.what());
                return false;
        }
        return true;
}

bool server::sendMat4(const glm::mat4& matLeft,const glm::mat4& matRight,server::dataType enumType)
{
		if (!(leftSocket->connected()) || !(rightSocket->connected()))
			return false;
        //Function for transfering the matrix into an array and then sending it
        const int size=4;

        //Transfer left
        float arrLeft[size*size];
        for(int i=0; i<size;++i){
                glm::vec4 vec=matLeft[i];
                for(int j=0; j<size;++j) arrLeft[i*size+j]=vec[j];
        }

        //Transfer right
        float arrRight[size*size];
        for(int i=0; i<size;++i){
                glm::vec4 vec=matRight[i];
                for(int j=0; j<size;++j) arrRight[i*size+j]=vec[j];
        }

        //Send
        if(!sendArray(arrLeft,size*size*sizeof(float),LEFT,enumType)||!sendArray(arrRight,size*size*sizeof(float),RIGHT,enumType)) 
        {
                ++*matrixId;
                return false;
        }
        else
        {
                ++*matrixId;
                return true;
        }
}

bool server::sendColor(const Color* col,const int x,const int y)
{
		if (!leftSocket->connected() || !rightSocket->connected())
			return false;
        //Function for sending the Colors comp element to the clients
        const int sizeComp=sizeof(Color)/sizeof(float); //Size of color comp array sizeof?
        const int sizeMod=2; //To store x and y in
        const int sizeColArr=x*y;
        int sizeTot=sizeComp*sizeColArr+sizeMod;
        float* colorArr=new float[sizeTot];
        colorArr[0]=(float)x;
        colorArr[1]=(float)y;

		memcpy(colorArr + 2, col, (sizeColArr-2)*sizeof(float));


        if(!sendArray(colorArr,sizeTot,LEFT,COLOR)||!sendArray(colorArr,sizeTot,RIGHT,COLOR))
        {
                delete[] colorArr;
                ++*colorId;
                return false;
        }
        else
        {
                delete[] colorArr;
                ++*colorId;
                return true;
        }
}

bool server::sendArray(const float arrIn[],int arrSize,server::eye eyeIn, server::dataType typeIn)
{
        //Function for sending the array to the clients
       // std::cerr << "Status " << zmq_strerror(errno) << std::endl;

        //Convert enum to enum pointers
        server::dataType* typePtr=new server::dataType(typeIn); 

        //Pointer to id
        int* idPtr;
        if(typeIn==COLOR) idPtr=colorId;
        else idPtr=matrixId;

        //Which client should it be sent to
        zmq::socket_t* clSocket;
        std::string clStr;
        if(eyeIn==RIGHT)
        {
                clSocket=rightSocket;
                clStr="right";
        }
        else 
        {
                clSocket=leftSocket;
                clStr="left";
        }
        //Send the data
        try
        {
                //Send type enum to client
               // std::cerr << "Sending enum to " << clStr << " " << zmq_strerror(errno)  << std::endl;
                zmq::message_t messageEnum((void*)typePtr, sizeof(typePtr), NULL);
                if(!clSocket->send(messageEnum,ZMQ_SNDMORE))
                {
                        std::cerr << "Failed to send enum to " << clStr << std::endl;
                        return false;
                }
               // std::cerr << "Sent enum to " << clStr << " " << zmq_strerror(errno)  << std::endl;

                //Send id
                zmq::message_t messageId((void*)idPtr, sizeof(idPtr), NULL);
                if(!clSocket->send(messageId,ZMQ_SNDMORE))
                {
                        std::cerr << "Failed to send id to " << clStr << std::endl;
                        return false;
                }

                zmq::message_t messageArr((void*)arrIn, arrSize, NULL);
                if(!clSocket->send(messageArr,0))
                {
                        std::cerr << "Failed to send to " << clStr << std::endl;
                        return false;
                }
                

                //Get the reply
                zmq::message_t replyArr;
                if(!clSocket->recv (&replyArr))
                {
                        std::cerr << "Failed to recive reply from " << clStr << std::endl;
                        return false;
                }
                //std::cerr << "Received reply from " << clStr << " " << zmq_strerror(errno)  << std::endl;
        }
        catch(zmq::error_t e)
        {
                printf(e.what());
                std::cerr << "Failed to send data" << std::endl;
                return false;
        }

        delete typePtr;
        return true;
}