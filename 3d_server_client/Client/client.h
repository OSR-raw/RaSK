#pragma once

//
//
//  Client
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
//Render related files
#include "..\Core\main.h"
#include "..\Includes\glm\glm.hpp"
#include "..\Includes\glm\gtc\type_ptr.hpp"

//Libs
#include <..\Includes\zmq.hpp>

#include <windows.h> //Has to be included last to avoid problems with WinSock2

class client
{
public:
        enum eye
        {
                RIGHT,
                LEFT
        };

        enum dataType
        {
                MAT4_PROJECTION,
                MAT4_MODEL,
                COLOR
        };

        client();
        ~client();

        bool receiveData(client::dataType* enumOut,void** dataOut, int* idPtr, int* x, int* y);
        bool initConnect();
        bool isData();

        client::eye eyeSide; //Which eye is this client for

private:
        const std::string replyString;
        zmq::message_t reply;

        //Network related
        zmq::context_t context;
        zmq::socket_t subSocket;

        //PORT is +1 higher than set if eye is right, unchanged if left eye
        unsigned int PORT;

        //Helper functions to get things from server, important to call everything in correct order
        //First recive enum, then id, then the floats, see client::run()
        client::dataType receiveEnum();
        int receiveId();
        float receiveFloat();
        bool receiveMore();
        bool replyServer();
};