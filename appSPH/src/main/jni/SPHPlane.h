#ifndef SPH_SPHPLANE_H
#define SPH_SPHPLANE_H

#include "Vector.h"

class SPHPlane {

public:
    SPHPlane(Vector3d normal, double offset) {
        this->normal = normal;
        this->offset = offset;
    }

    Vector3d normal;
    double offset;

};

#endif
