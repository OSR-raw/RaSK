#pragma once

//
//
//  Server
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
//Render related files
#include "Core\main.h"
#include "Includes\glm\glm.hpp"
#include "Includes\glm\gtc\type_ptr.hpp"

//Libs
#include <Includes\zmq.hpp>

#include <windows.h> //Has to be included last to avoid problems with WinSock2

class server
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

        server();
        ~server();

        bool initConnect();
        bool connectClients();
        bool sendMat4(const glm::mat4& matLeft,const glm::mat4& matRight,server::dataType enumType);
        bool sendColor(const Color* color,const int x, const int y);

private:
        //Helper functions
        bool sendArray(const float arrIn[],int arrSize,server::eye eyeIn, server::dataType typeIn);
        void readClientIps();

        //Network realted variables
        zmq::context_t* context;
        zmq::socket_t* leftSocket;
        zmq::socket_t* rightSocket;

        //PORT is +1 higher than set if eye is right, unchanged if left eye
        unsigned int LEFT_PORT;
        unsigned int RIGHT_PORT;
        std::string LEFT_IP;
        std::string RIGHT_IP;

        int* matrixId;
        int* colorId;
};