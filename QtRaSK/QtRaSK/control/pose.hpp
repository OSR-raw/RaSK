#ifndef CONTROL_POSE_HPP_
#define	CONTROL_POSE_HPP_

#include "boost/thread/thread.hpp"

#include "Core/main.h"

namespace control {

// Forward declarations
class Filter;
class HandPositionTracker;
class Manipulator;

/**
 * The pose estimator calculates position and orientation of the manipulated
 * virtual object based on the input received from the hand-tracker. The
 * pose is updated in regular intervals. The current pose can be retrieved by
 * polling from the estimator.
 */
class PoseEstimator {

  public:
    
    enum State {
        /** No hand is tracked. */
        unavailable,
        /** Estimates are produced. */
        estimating
    };
  
  private:

	bool needs_exit;
    
    State state_;
    
    // --- pose  ---
    
    glm::vec3 position_;

    glm::quat orientation_;
    
    // --- tracking ---
    
    HandPositionTracker *tracker_;

    std::size_t time_;

    int read_interval_ms_;

    Filter *filter_;

    Manipulator *manipulator_;

    mutable boost::mutex mutex_;

  public:

    PoseEstimator(HandPositionTracker *tracker = NULL, Manipulator *manipulator = NULL) :
        state_(unavailable), position_(0, 0, 0), orientation_(0, 0, 0, 1),
        tracker_(tracker), time_(0), read_interval_ms_(25), filter_(NULL), manipulator_(manipulator) {}

    void set_tracker(HandPositionTracker *tracker) { tracker_ = tracker; }

    HandPositionTracker* get_tracker() const { return tracker_; }

    void set_filter(Filter *filter) { filter_ = filter; }

    void set_manipulator(Manipulator *manipulator) { manipulator_ = manipulator; }

    void run();

	void Exit();

    State get_state() const;

    glm::vec3 get_position() const;

    glm::quat get_orientation() const;

    glm::mat4 get_pose() const;

  private:

    void estimate_pose();

};

std::ostream& operator<<(std::ostream&, const PoseEstimator::State&);

}

#endif
