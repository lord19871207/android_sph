#ifndef SPH_SPHSIMULATION_H
#define SPH_SPHSIMULATION_H

#include <vector>
#include "SPHParticle.h"
#include "SPHPlane.h"

#define GRID_SIZE 32

class SPHSimulation {

public:
    SPHSimulation(size_t particleCount);

    virtual ~SPHSimulation();

    void initialize(double left, double right, double top, double bottom);

    void simulate();

private:
    void calcGrids();

    void calcPressure();

    void calcForce();

    void calcAdvance();

    double applyPressure(const SPHParticle *Pi);

    Vector2d applyForce(const SPHParticle *Pi);

    int getGridIndex(const SPHParticle* P);

public:
    std::vector<SPHParticle *> particles;
    std::vector<const SPHParticle *> grid[GRID_SIZE * GRID_SIZE];
    SPHPlane **planes;
};

#endif
