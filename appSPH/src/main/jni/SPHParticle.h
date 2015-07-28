#ifndef SPH_SPHPARTICLE_H
#define SPH_SPHPARTICLE_H

#include "Vector.h"

class SPHParticle {

public:
    Vector3d position;
    Vector3d velocity;
    Vector3d velocityEval;
    Vector3d force;
    double pressure;
    double density;

};

#endif
