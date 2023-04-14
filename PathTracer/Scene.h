#pragma once

#include <vector>
#include "Sphere.h"

class Scene {

public:
    std::vector<Sphere> Spheres;
    void addSphere(Sphere sphere) {
        Spheres.push_back(sphere);
    }
  


};
