#include "control/track.hpp"

//#include <Common/NiteSampleUtilities.h>


#include "control/filter.hpp"
#include "control/util.hpp"

namespace control {
  
 HandPositionTracker::HandPositionTracker() : state_(uninited), positions_(15),
                            time_(0), read_interval_ms_(25),
							fail_reset_count_(4), needs_exit(false) 
 {

 }


void HandPositionTracker::Stop( )
{
	needs_exit = true;
}


void HandPositionTracker::run() {
    nite::HandTracker handTracker;
	nite::Status niteRc;
	
	openni::Status rc = openni::OpenNI::initialize();
	if (rc != openni::STATUS_OK)
	{
		printf("Failed to initialize OpenNI\n%s\n", openni::OpenNI::getExtendedError());
		return;
	}


	const char* deviceUri = openni::ANY_DEVICE;
	rc = m_device.open(deviceUri);

	if (rc != nite::STATUS_OK)
	{
		printf("Couldn't create device\n");
		return;
	}

	nite::NiTE::initialize();

	niteRc = handTracker.create(  );
	if (niteRc != nite::STATUS_OK)
	{
		printf("Couldn't create user tracker\n");
		return;
	}

	handTracker.startGestureDetection(nite::GESTURE_WAVE);
	handTracker.startGestureDetection(nite::GESTURE_CLICK);
	printf("\nWave or click to start tracking your hand...\n");

    // Only one hand is tracke  d. Making a second gesture will change the hand
    // that is tracked.
    nite::HandId id;

    int t_boot = 0;
    int t_fail = 0;

    control::AverageFilter avg_filter;
    
	nite::HandTrackerFrameRef handTrackerFrame;
	needs_exit = false;
	state_ = unavailable;
	while (!needs_exit) {
				niteRc = handTracker.readFrame(&handTrackerFrame);
		if (niteRc != nite::STATUS_OK) {
			printf("Get next frame failed\n");
			continue;
		}

		const nite::Array<nite::GestureData>& gestures = handTrackerFrame.getGestures();
		for (int i = 0; i < gestures.getSize(); ++i) {
			if (gestures[i].isComplete()) {
				handTracker.startHandTracking(gestures[i].getCurrentPosition(), &id);
			}
		}

        // TODO: what to do if multiple hands are in the scene?
		const nite::Array<nite::HandData>& hands = handTrackerFrame.getHands();
		for (int i = 0; i < hands.getSize(); ++i) {
			const nite::HandData& hand = hands[i];
			if (hand.isTracking() && hand.getId() == id) {
                if (get_state() == unavailable) {
                    // We start the booting stage when the tracker previously
                    // was in state unavaible, i.e. no hand was tracked. The
                    // booting stage is done after the ring buffer has been
                    // completely filled. How many readings were performed is
                    // denoted by the counter t_boot.
                    boost::lock_guard<boost::mutex> guard(mutex_);
                    state_ = booting;
                    t_boot = 0;
                    t_fail = 0;
                    center_ = glm::vec3(0, 0, 0);
                }
             
                {
                    boost::lock_guard<boost::mutex> guard(mutex_);
                    positions_[time_] = control::nite2glm(hand.getPosition()) - center_;
                    time_++;
                    time_ %= positions_.size();
                }

                t_boot++;

                if (get_state() == booting && t_boot == int(max_readings())) {
                    boost::lock_guard<boost::mutex> guard(mutex_);
                    state_ = tracking;
                    center_ = avg_filter(positions_);
                }
                
                boost::this_thread::sleep(boost::posix_time::milliseconds(read_interval_ms_));
            }
		}

        if (hands.getSize() == 0) {
            if (t_fail >= fail_reset_count_) {
                // If no hand is tracked, the tracker changes its state to
                // unavailable. Any previous readings are frozen at this point such
                // that subsequent retrievals of the position will yield the
                // last-known position.
                boost::lock_guard<boost::mutex> guard(mutex_);
                state_ = unavailable;
                t_fail = 0;
            } else {
                // There is no point in resetting and declaring the hand
                // tracker as unavailable if no reading could be obtained. This
                // should only happen if multiple subsequent readings fail.
                t_fail++;
            }
        } else if (hands.getSize() > 1) {
            {
                boost::lock_guard<boost::mutex> guard(mutex_);
                state_ = freezing;
			}
             boost::this_thread::sleep(boost::posix_time::milliseconds(3000));
			{
                boost::lock_guard<boost::mutex> guard(mutex_);
                state_ = unavailable;
                t_fail = 0;
			}

        }
	}

	nite::NiTE::shutdown();
}

HandPositionTracker::State HandPositionTracker::get_state() const {
    boost::lock_guard<boost::mutex> guard(mutex_);
    return state_;
}

glm::vec3 HandPositionTracker::get_position(int t) const {
    boost::lock_guard<boost::mutex> guard(mutex_);
    return positions_[(time_ + t) % positions_.size()];
}

std::ostream& operator<<( std::ostream& os, const HandPositionTracker::State& state) {
   switch(state) {
      case HandPositionTracker::unavailable: os << "unavailable"; break;
      case HandPositionTracker::booting: os << "booting"; break;
      case HandPositionTracker::tracking: os << "tracking"; break;
	  case HandPositionTracker::freezing: os << "freezing"; break;
   }
   return os;
}

}
