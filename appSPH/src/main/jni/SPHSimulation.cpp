#include "SPHSimulation.h"

SPHSimulation::SPHSimulation(size_t particleCount) {
    for (int i = 0; i < particleCount; ++i) {
        particles.push_back(new SPHParticle());
    }

    planes = new SPHPlane *[6];
    planes[0] = new SPHPlane(Vector3d(1, 0, 0), -1);
    planes[1] = new SPHPlane(Vector3d(-1, 0, 0), 1);
    planes[2] = new SPHPlane(Vector3d(0, -1, 0), 1);
    planes[3] = new SPHPlane(Vector3d(0, 1, 0), -1);
    planes[4] = new SPHPlane(Vector3d(0, 0, 1), -1);
    planes[5] = new SPHPlane(Vector3d(0, 0, -1), 1);
}

SPHSimulation::~SPHSimulation() {
    std::vector<SPHParticle *>::iterator iter = particles.begin();
    while (iter != particles.end()) {
        delete *iter++;
    }

    if (planes) {
        for (int i = 0; i < 6; ++i) {
            delete planes[i];
        }
        delete planes;
        planes = nullptr;
    }
}

void SPHSimulation::initialize(double left, double right, double top, double bottom,
                               double znear, double zfar) {
    planes[0]->offset = left;
    planes[1]->offset = right;
    planes[2]->offset = top;
    planes[3]->offset = bottom;
    planes[4]->offset = znear;
    planes[5]->offset = zfar;

    std::vector<SPHParticle *>::const_iterator iter = particles.begin();
    while (iter != particles.end()) {
        double x = left + (right - left) * rand() / (double) RAND_MAX;
        double y = bottom + (top - bottom) * rand() / (double) RAND_MAX;
        double z = znear + (zfar - znear) * rand() / (double) RAND_MAX;

        (*iter)->position.v[0] = x;
        (*iter)->position.v[1] = y;
        (*iter)->position.v[2] = z;

        ++iter;
    }
}
