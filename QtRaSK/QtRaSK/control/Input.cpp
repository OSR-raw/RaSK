#include <boost/thread.hpp>
#include "Input.h"

#include "../oglwidget.h"




void run_tracker(control::HandPositionTracker *tracker) {
    tracker->run();
}

void run_estimator(control::PoseEstimator *estimator) {
    estimator->run();
}

Input::Input(OGLWidget* const cnt): _cntx(cnt), _lbtn_pressed(false), zoom_val(0.0), estimator_(NULL)
{
	_angleXS = 0;
	_angleYS = 0;
	_obj_mat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	
	tracker = new control::HandPositionTracker();
    t1 = new boost::thread(run_tracker, tracker);
	t1->start_thread();

    filter = new control::RampFilter();
    manipulator = new control::YawPitchZoomManipulator();
    
	estimator_ = new control::PoseEstimator();
    estimator_->set_filter(filter);
    estimator_->set_tracker(tracker);
    estimator_->set_manipulator(manipulator);
	t = new boost::thread(&control::PoseEstimator::run, estimator_);
	t->start_thread();


}

Input::~Input()
{
	if ( estimator_ && estimator_->get_state() != HandPositionTracker::uninited )
		estimator_->Exit();

	t1->join();
	t->join();

	t1->interrupt();
	t->interrupt();

	delete estimator_;
	delete tracker;
	delete filter;
    delete manipulator;
	delete t1;
	delete t;
}

void Input::OnKeyPressed( char c )
{

}

void Input::OnMouseLBDown( int x, int y )
{
	_mouseStartX = x;
	_mouseStartY = y;
	_prevX = x;
	_prevY = y;
	_lbtn_pressed = true;
}

void Input::OnMouseMove( int x, int y )
{
	if (_lbtn_pressed)
	{
		float rotSpeed = 0.1f;
		_angleX =  (x - _prevX) * rotSpeed;
		_angleY =  (y - _prevY) * rotSpeed;
		glm::vec3 axis(_angleY, _angleX, 0);
		
		float angle = glm::length(axis);
		if ( abs(angle) > 0.1 )
		{
			axis = glm::normalize(axis);			
			_obj_mat = glm::rotate(glm::mat4(1.0), angle, axis)*_obj_mat;
			glm::mat4 mt = glm::translate(glm::mat4(1.0), glm::vec3(0.f, 0.f, zoom_val))*_obj_mat;
			_cntx->SetModelMatrix(mt);
		}
		_prevX = x;
		_prevY = y;
	}
}

void Input::OnSroll( int dx )
{
	zoom_val += 0.1f*float(dx)/120;
	glm::mat4 mt = glm::translate(glm::mat4(1.0), glm::vec3(0.f, 0.f, zoom_val))*_obj_mat;
	_cntx->SetModelMatrix( mt );
}

void Input::OnMouseLBUp( int x, int y )
{
	_lbtn_pressed = false;
	_angleXS = _angleX;
    _angleYS = _angleY;
}

glm::mat4 Input::GetPosition()
{	
	glm::mat4 rm = _obj_mat;
	if ( estimator_ && estimator_->get_state() != HandPositionTracker::uninited  )
		rm *= estimator_->get_pose();

	glm::mat4 mt = glm::translate(glm::mat4(1.0), glm::vec3(0.f, 0.f, zoom_val))*(rm);

	return mt;
}