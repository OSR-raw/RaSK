#include "control/pose.hpp"

#include "Core/main.h"
#include <iostream>

#include "control/filter.hpp"
#include "control/manip.hpp"
#include "control/track.hpp"

namespace control {

void PoseEstimator::estimate_pose() {
    glm::vec3 hand_pos;
    if (filter_ == NULL ) {
        hand_pos = tracker_->get_position(0);
    } else {
        hand_pos = tracker_->filter_positions(*filter_);
    }
    if (manipulator_ == NULL) {
        TranslateManipulator().map(hand_pos, &position_, &orientation_);
    } else {
        manipulator_->map(hand_pos, &position_, &orientation_);
    }
}

void PoseEstimator::Exit()
{
	tracker_->Stop();

	needs_exit = true;
}

void PoseEstimator::run() {
    // TODO: disable changing the filter or the tracker while the estimator is running
	needs_exit = false;
    while (tracker_ != NULL && ( !needs_exit )) {
        if (tracker_->get_state() != HandPositionTracker::tracking) {
            boost::lock_guard<boost::mutex> guard(mutex_);
            state_ = unavailable;
        }
        while (tracker_->get_state() != HandPositionTracker::tracking && ( !needs_exit )) {
            boost::this_thread::sleep(boost::posix_time::milliseconds(read_interval_ms_));
        }
        {
            boost::lock_guard<boost::mutex> guard(mutex_);
            state_ = estimating;
        }
        estimate_pose();
        boost::this_thread::sleep(boost::posix_time::milliseconds(read_interval_ms_));
    }
}

PoseEstimator::State PoseEstimator::get_state() const {
    boost::lock_guard<boost::mutex> guard(mutex_);
    return state_;
}

glm::vec3 PoseEstimator::get_position() const {
    boost::lock_guard<boost::mutex> guard(mutex_);
    return position_;
}

glm::quat PoseEstimator::get_orientation() const {
    boost::lock_guard<boost::mutex> guard(mutex_);
    return orientation_;
}

glm::mat4 PoseEstimator::get_pose() const {
    boost::lock_guard<boost::mutex> guard(mutex_);
    glm::mat4 pose = glm::translate(glm::mat4(1.0), position_)*glm::toMat4(orientation_);
    return pose;
}

std::ostream& operator<<( std::ostream& os, const PoseEstimator::State& state) {
   switch(state) {
      case PoseEstimator::unavailable: os << "unavailable"; break;
      case PoseEstimator::estimating: os << "estimating"; break;
   }
   return os;
}

}
