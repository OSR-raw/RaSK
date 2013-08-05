#ifndef CONTROL_TRACK_HPP_
#define	CONTROL_TRACK_HPP_

#include <boost/thread.hpp>
#include <Core/main.h>
#include <iostream>

#include "control/filter.hpp"
#include "NiTE.h"
namespace control {

/**
 * The hand tracker reads the current position of the tracked hand of the user.
 * The hand tracker is designed to run asynchronously and receives updates from
 * the underlying NiTE tracker as often as possible. The current position of
 * the hand can be polled any time – the most recent reading will be returned.
 * The read hand positions are specified relative to the camera coordinate
 * frame.
 */
class HandPositionTracker {

  public:
    
    enum State {
        /** No hand is tracked. */
        unavailable,
        /** A hand is tracked, but the buffer of readings has yet to be
         * completely filled. */
        booting,
        /** A hand is tracked, and the buffer of readings has completely been
         * filled. */
        tracking,
		uninited,
		freezing
    };
    
  private:
    
    State state_;
    openni::Device		m_device;
    /**
     * Ring buffer that stores the most recently read hand positions. The current
     * hand position is at index time_, the second-most recent hand position is
     * at index time_ - 1. This ring buffer keeps a certain history of readings.
     * The spacing between two readings is roughly read_interval_ms_.
     */
    std::vector<glm::vec3> positions_;

    /**
     * Specifies the location of the hand when the tracker starts tracking. By
     * subtracting the center from all subsequent positions, we will make the
     * estimation tasks based on the hand tracker much easier.
     */
    glm::vec3 center_;
    
    int time_;
    
    int read_interval_ms_;

    int fail_reset_count_;

    mutable boost::mutex mutex_;

	bool needs_exit;
    
  public:
      
    HandPositionTracker();

    void run();
	void Stop( );

    State get_state() const;
    
    /**
     * Returns the current reading (t_past = 0) or one of the previous readings.
     * The parameter t_past defines how far to get into the past. The history
     * is bounded by the maximum number of items stored in the ring buffer.
     */
    glm::vec3 get_position(int t_past = 0) const;
    
    /**
     * Runs the specified filter on the current and past readings.
     */
    glm::vec3 filter_positions(const Filter &filter) { return filter(positions_); }
    
    /**
     * Returns the size of the ring buffer storing the hand positions. In other
     * words this is the number of readings in history that are stored in the
     * ring buffer.
     */
    std::size_t max_readings() const { return positions_.size(); }
    
};


std::ostream& operator<<(std::ostream&, const HandPositionTracker::State&);
    
}

#endif
