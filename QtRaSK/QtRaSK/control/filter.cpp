#include "control/filter.hpp"

namespace control {
    
glm::vec3 AverageFilter::operator ()(const std::vector<glm::vec3>& signal) const {
    glm::vec3 avg(0, 0, 0);
    for (std::size_t i = 0; i < signal.size(); i++) {
        avg += signal[i] / float(signal.size());
    }
    return avg;
}

glm::vec3 RampFilter::operator ()(const std::vector<glm::vec3>& signal) const {
    glm::vec3 wavg(0, 0, 0);
    float s(signal.size());
    for (std::size_t i = 0; i < signal.size(); i++) {
        wavg += signal[i] * (s - i);
    }
    wavg /= ((s + 1) * s) / 2;
    return wavg;
}
    
}
