#ifndef CONTROL_MANIP_HPP_
#define CONTROL_MANIP_HPP_

#include "Core/main.h"


namespace control {

class Manipulator {

  public:

    virtual ~Manipulator() {}

    virtual void map(const glm::vec3 &hand_position, glm::vec3 *position, glm::quat *orientation) const = 0;

};

class TranslateManipulator : public Manipulator {

  public:

    void map(const glm::vec3 &hand_position, glm::vec3 *position, glm::quat *orientation) const;

};

class YawPitchZoomManipulator : public Manipulator {

  public:

    void map(const glm::vec3 &hand_position, glm::vec3 *position, glm::quat *orientation) const;

};

}

#endif
