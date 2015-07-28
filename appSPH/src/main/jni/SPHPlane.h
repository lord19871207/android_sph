#ifndef SPH_SPHPLANE_H
#define SPH_SPHPLANE_H

#include "Vector2.h"

class SPHPlane {

public:
    SPHPlane(Vector2d normal, double offset) {
        this->normal = normal;
        this->offset = offset;
    }

    Vector2d normal;
    double offset;

};

#endif
