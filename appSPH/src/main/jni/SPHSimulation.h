#ifndef SPH_SPHSIMULATION_H
#define SPH_SPHSIMULATION_H

#include <vector>
#include "SPHParticle.h"
#include "SPHPlane.h"

class SPHSimulation {

public:
    SPHSimulation(size_t particleCount);

    virtual ~SPHSimulation();

    void initialize(double left, double right, double top, double bottom,
                    double znear, double zfar);

    void simulate();

private:
    void calcPressure();

    void calcForce();

    void calcAdvance();

public:
    std::vector<SPHParticle *> particles;
    SPHPlane **planes;

};

#endif
