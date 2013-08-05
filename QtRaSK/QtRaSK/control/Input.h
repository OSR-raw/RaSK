#include "Core/main.h"
class OGLWidget;
#include "pose.hpp"
#include "filter.hpp"
#include "manip.hpp"
#include "track.hpp"

using namespace control;

class Input
{
public:
	Input(OGLWidget* const cnt);
	~Input();
	void OnKeyPressed( char c );
	void OnMouseLBDown( int x, int y );
	void OnMouseMove( int dx, int dy );
	void OnMouseLBUp( int x, int y );
	void OnSroll( int dx );
	

	//Kinect
	glm::mat4 GetPosition();

private:
	Input();
	OGLWidget* const _cntx;
	int _mouseStartX;
	int _mouseStartY;
	int _prevX;
	int _prevY;
	bool _lbtn_pressed;
	float _angleXS;
	float _angleX;
    float _angleYS;
	float _angleY;

	glm::mat4 _obj_mat;
	float zoom_val;

	//Kinect
	PoseEstimator* estimator_;
	HandPositionTracker* tracker;
	RampFilter* filter;
    YawPitchZoomManipulator* manipulator;

	boost::thread* t;
	boost::thread* t1;

};