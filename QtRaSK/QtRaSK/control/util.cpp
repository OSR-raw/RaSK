#include "control/util.hpp"

glm::vec3 control::nite2glm(const nite::Point3f &v) {
    return glm::vec3(v.x, v.y, v.z);
}
