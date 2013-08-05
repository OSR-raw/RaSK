#ifndef __MAIN_H
#define __MAIN_H
#include <iostream>
#include <string>
//#include <fstream>
#include <vector>
#include <boost/filesystem/fstream.hpp>

#include "..\Includes\glm\glm.hpp"  
#include "..\Includes\glm\gtc\matrix_transform.hpp"
#include "..\Includes\glm\gtx\orthonormalize.hpp"
#include "..\Includes\glm\gtx\quaternion.hpp"
#include "..\Includes\glm\gtc\matrix_transform.hpp"
#include "..\Includes\glm\gtc\quaternion.hpp"

struct Vertex
{
	float coord[3];
};

struct Color
{
	float comp[4];
	//unsigned char comp[4];
};


using namespace std;


static string textFileRead(const char *fileName) 
{
	string fileString;
	string line;
	
	//string tmp("D:/STUD/DD2257/Project/dd2257/render/QtRaSK/Win32/Debug/");
	//tmp += fileName;
	ifstream file(fileName,ios_base::in);

	if (file.is_open()) 
	{
		while (!file.eof()) 
		{
			getline(file, line);
		  	fileString.append(line);
			fileString.append("\n");
		}
		file.close();
	}
	else
		cout<<"Unable to open "<<fileName<<"\n";

    return fileString;
}
#endif