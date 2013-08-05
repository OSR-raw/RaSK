#include "control/manip.hpp"

namespace control {

void TranslateManipulator::map(const glm::vec3 &hand_position, glm::vec3 *position, glm::quat *orientation) const {
    *position = hand_position;
    *position /= 100.0;
    *orientation = glm::quat(0, 0, 0, 1);
}

void YawPitchZoomManipulator::map(const glm::vec3 &hand_position, glm::vec3 *position, glm::quat *orientation) const {
    // Panning (or something simliar)
    glm::vec3 y_axis(0, 1, 0);
    *orientation = glm::quat(y_axis * (hand_position.x / 150));

    // Tilting (or something similar)
    glm::vec3 x_axis(1, 0, 0);
    *orientation = glm::quat(x_axis * (-hand_position.y / 150)) * (*orientation);

    // Zoom
    *position = glm::vec3(0, 0,  -hand_position.z / 225.0);
}

}
