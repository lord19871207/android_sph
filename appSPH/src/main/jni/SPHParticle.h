#ifndef SPH_SPHPARTICLE_H
#define SPH_SPHPARTICLE_H

#include "Vector2.h"

class SPHParticle {

public:
    Vector2d position;
    Vector2d velocity;
    Vector2d velocityEval;
    Vector2d force;
    double pressure;
    double density;

};

#endif
