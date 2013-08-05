#ifndef CONTROL_FILTER_HPP_
#define	CONTROL_FILTER_HPP_

#include "Core/main.h"
#include <vector>

namespace control 
{
    
class Filter 
{
    
 public:
    virtual ~Filter() {}    
    virtual glm::vec3 operator()(const std::vector<glm::vec3>& signal) const = 0;
    
};

class IdentityFilter : public Filter {
 
  public:    
    glm::vec3 operator()(const std::vector<glm::vec3>& signal) const { return signal[0]; }
    
};

class AverageFilter : public Filter {
 
  public:
    
    glm::vec3 operator()(const std::vector<glm::vec3>& signal) const ;
    
};

class RampFilter : public Filter {
 
  public:
    
    glm::vec3 operator()(const std::vector<glm::vec3>& signal) const ;
    
};
    
}

#endif
