#ifndef SPH_SPHPARTICLE_H
#define SPH_SPHPARTICLE_H

#include "Vector.h"

class SPHParticle {

public:
    Vector3d position;
    Vector3d velocity;
    double pressure;

};

#endif
