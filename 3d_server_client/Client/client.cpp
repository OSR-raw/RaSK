#include "client.h"


client::client():replyString("Recived"),reply(replyString.size()),eyeSide(RIGHT),context(1),subSocket(context, ZMQ_REP),PORT(5555)
{
        //Move reply message to reply message object
        memcpy ((void *) reply.data (), &replyString,replyString.size());

        //Ask user if this client is for left or right eye
        std::string line;
        std::cerr << "Right or left eye?" << std::endl;
        std::cin >> line;
        if(line=="RIGHT"||line=="right") eyeSide=client::RIGHT;
        else if(line=="LEFT"||line=="left") eyeSide=client::LEFT;
        else
        {
                std::cerr<< "Wrong input argument, should be left or right."<<std::endl;;
                throw std::invalid_argument("Wrong input argument, should be left or right.");  
        }

        if(eyeSide==RIGHT) PORT++;
        std::cerr << "Code " << zmq_strerror(errno) << std::endl;
}


client::~client()
{
        //delete subSocket;
        //delete context;
}

bool client::isData()
{
        //Check if there is data to recive from the socket without blocking
        int* optionValue=new int();
        size_t* optionSize = new size_t(sizeof (optionValue));
        subSocket.getsockopt(ZMQ_EVENTS,optionValue,optionSize);
        delete optionSize;
        if(*optionValue==ZMQ_POLLIN) return true;
        else return false;
}

client::dataType client::receiveEnum()
{
        //Recieve the enum from the server
        zmq::message_t updateEnum;
        //if(!subSocket.recv(&updateEnum,ZMQ_DONTWAIT)){
        if(!subSocket.recv(&updateEnum)){
                std::cerr << "Failed to recive enum" << std::endl;
        }
        return(*(client::dataType*)updateEnum.data());
}

int client::receiveId()
{
        //Receive id from server
        zmq::message_t updateId;
        if(!subSocket.recv(&updateId)){
                std::cerr << "Failed to recive id" << std::endl;
        }
        return(*(int*)updateId.data());
}

float client::receiveFloat()
{
        //Receive float from server
        zmq::message_t updateData;
        if(!subSocket.recv(&updateData))
        {
                std::cerr << "Failed to receive data" << std::endl;
        }
        return(*(float*)updateData.data());
}

bool client::receiveMore()
{
        //Check if there is more to recive in multipart message
        bool* optionValue=new bool();
        size_t* optionSize = new size_t(sizeof (optionValue));
        subSocket.getsockopt(ZMQ_RCVMORE,optionValue,optionSize);
        delete optionSize;
        return(*optionValue);
}

bool client::replyServer()
{
        try
        {
                //Send reply back to client
                //std::cerr << "Sending back reply " << zmq_strerror(errno) << std::endl;
                return(subSocket.send(reply));
        }
        catch(zmq::error_t e)
        {
                printf(e.what());
                std::cerr << "Failed to reply to server" << std::endl;
                throw std::exception("Failed to reply to server");
        }
}

bool client::receiveData(client::dataType* enumReceive,void** dataOut, int* idPtr, int* x, int* y)
{
        //Collect the data from the server
        //std::cerr << "Checking for data " << zmq_strerror(errno) << std::endl;

        try
        {
                if(!isData())
                {
                        return false;
                }
               // std::cerr << "Reciving data " << zmq_strerror(errno) << std::endl;


                //First we want to get the enum for the type
                *enumReceive=client::dataType(receiveEnum());
                //std::cerr << "Received enum "<< *enumReceive << " " << zmq_strerror(errno) << std::endl;

                //Check that the server will send more data
                bool moreData=receiveMore();
                if(!moreData)
                {
                      //  std::cerr << "Server is not sending enough data" << std::endl;
                        throw std::exception("Server is not sending enough data");
                }

                //Recive the id
                *idPtr=receiveId();
               // std::cerr << "Received id "<< *idPtr << " " << zmq_strerror(errno) << std::endl;

                //Recive and put data in correct strucutre based on typeRecive
                if(*enumReceive==COLOR)
                {                      
					zmq::message_t updateData;
					subSocket.recv(&updateData);
					*x = (*(float*)updateData.data());
					*y = (*((float*)(updateData.data()) + 1));
						 
                    Color* col=new Color[(*x)*(*y)];
                    int compPos=0;
                    int colPos=0;
					memcpy(col,((float*)(updateData.data()) + 2), (*x)*(*y)*4 );
                    // std::cerr << "Done receiving color data" << std::endl;
                    *dataOut=col;
                    if(!replyServer()) //Have to reply to server so it knows we got the message
                    {
                            //std::cerr << "Failed to reply to server" << std::endl;
                            throw std::exception("Failed to reply to server");
                    }
						
                    return true;
                }
                else if(*enumReceive==MAT4_PROJECTION||*enumReceive==MAT4_MODEL)
                {
                        float* tmpArr=new float[16]; //Know we will get an array of length 16, bit ugly code

                        zmq::message_t updateData;
						subSocket.recv(&updateData);

						memcpy(tmpArr,((float*)(updateData.data())), 16*sizeof(float));

                        //std::cerr << "Done receiving mat4 data of type " << *enumReceive << std::endl;

                        //Return data
                        *dataOut=new glm::mat4(glm::make_mat4(tmpArr)); //Move constructor
                        *x=1;
                        *y=1;
                        delete[] tmpArr;
                        if(!replyServer()) //Have to reply to server so it knows we got the message
                        {
                                //std::cerr << "Failed to reply to server" << std::endl;
                                throw std::exception("Failed to reply to server");
                        }
                        return true;
                }
                else
                {
                        std::cerr << "Enum not recognised" << std::endl;
                        throw std::exception("Enum not recognised");
                }
        }
        catch(zmq::error_t e)
        {
                printf(e.what());
                std::cerr << "Failed to receive data" << std::endl;
                throw std::exception("Failed to receive data");
        }
}

bool client::initConnect()
{
        //Connect to server
        try{
                std::cerr << "Binding port for connection. Port: " << PORT << std::endl;

                std::ostringstream oss;
                //oss << "tcp://*:" << PORT;
                oss << "tcp://0.0.0.0:" << PORT;
                const std::string& tmp= oss.str();
                const char* c = tmp.c_str();
                subSocket.bind(c);
        }
        catch(zmq::error_t e)
        {
                printf(e.what());
                return false;
        }
        return true;
}