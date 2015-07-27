#include "SPHSimulation.h"

struct FluidVars {
    double timeDiff = 1.0 / 60.0;
    double restDensity = 600.0;
    double internalStiffness = 1.5;
    double viscosity = 0.35;

    double boundaryStiffness = 300.0;
    double boundaryDampening = 10.0;

    double accelerationLimit = 150.0;
    double velocityLimit = 600.0;
    double simulationScale = 4.0;

    Vector3d gravity = {0.0, -9.8, 0.0};

    double particleMass = 0.020543;
    double smoothingRadius = 0.1;
    double boundaryRadius = smoothingRadius;

    double d2 = simulationScale * simulationScale;
    double rd2 = smoothingRadius * smoothingRadius / d2;
    double poly6Kern = 315.0 / (64.0 * M_PI * pow(smoothingRadius, 9.0));
    double spikyKern = -45.0 / (M_PI * pow(smoothingRadius, 6.0));
    double lapKern = 45.0 / (M_PI * pow(smoothingRadius, 6.0));
} fluidVars;


SPHSimulation::SPHSimulation(size_t particleCount) {
    for (int i = 0; i < particleCount; ++i) {
        particles.push_back(new SPHParticle());
    }

    planes = new SPHPlane *[6];
    planes[0] = new SPHPlane(Vector3d(1, 0, 0), 1);
    planes[1] = new SPHPlane(Vector3d(-1, 0, 0), 1);
    planes[2] = new SPHPlane(Vector3d(0, -1, 0), 1);
    planes[3] = new SPHPlane(Vector3d(0, 1, 0), 1);
    planes[4] = new SPHPlane(Vector3d(0, 0, -1), 1);
    planes[5] = new SPHPlane(Vector3d(0, 0, 1), 0);
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
    planes[0]->offset = abs(left);
    planes[1]->offset = abs(right);
    planes[2]->offset = abs(top);
    planes[3]->offset = abs(bottom);
    planes[4]->offset = abs(znear);
    planes[5]->offset = abs(zfar);

    std::vector<SPHParticle *>::const_iterator iter = particles.begin();
    while (iter != particles.end()) {
        double x = left + (right - left) * rand() / (double) RAND_MAX;
        double y = bottom + (top - bottom) * rand() / (double) RAND_MAX;
        double z = znear + (zfar - znear) * rand() / (double) RAND_MAX;
        SPHParticle *particle = *iter++;
        particle->position.v[0] = x;
        particle->position.v[1] = y;
        particle->position.v[2] = z;
    }
}

void SPHSimulation::simulate() {
    calcPressure();
    calcForce();
    calcAdvance();
}

void SPHSimulation::calcPressure() {
    std::vector<SPHParticle *>::const_iterator iter1 = particles.begin();
    while (iter1 != particles.end()) {
        double sum = 0;

        Vector3d ri = (*iter1)->position;

        std::vector<SPHParticle *>::const_iterator iter2 = particles.begin();
        while (iter2 != particles.end()) {
            Vector3d rj = (*iter2)->position;
            Vector3d rij = ri - rj;
            double r2 = rij.dot(rij);
            if (r2 > 0.0 && r2 < fluidVars.rd2) {
                sum += pow((fluidVars.rd2 - r2) * fluidVars.d2, 3.0);
            }
            ++iter2;
        }

        sum = fluidVars.particleMass * fluidVars.poly6Kern * sum;
        if (sum < 1.0) {
            sum = 1.0;
        }

        (*iter1)->pressure = (sum - fluidVars.restDensity) * fluidVars.internalStiffness;
        (*iter1)->density = 1.0 / sum;
        ++iter1;
    }
}

void SPHSimulation::calcForce() {
    std::vector<SPHParticle *>::const_iterator iter1 = particles.begin();
    while (iter1 != particles.end()) {
        Vector3d fp;
        Vector3d fv;

        Vector3d ri = (*iter1)->position;
        Vector3d vi = (*iter1)->velocity;
        double pi = (*iter1)->pressure;
        double di = (*iter1)->density;

        std::vector<SPHParticle *>::const_iterator iter2 = particles.begin();
        while (iter2 != particles.end()) {
            Vector3d rj = (*iter2)->position;
            Vector3d rij = ri - rj;
            double r2 = rij.dot(rij);
            if (r2 > 0.0 && r2 < fluidVars.rd2) {
                double r = sqrt(r2 * fluidVars.d2);
                double c = fluidVars.smoothingRadius - r;
                Vector3d vj = (*iter2)->velocity;
                double pj = (*iter2)->pressure;
                double dj = (*iter2)->density;

                fp += rij * c * c * (pi + pj) * (dj / r);
                fv += (vj - vi) * c * dj;
            }

            ++iter2;
        }

        (*iter1)->force = (fp * -0.5f * fluidVars.spikyKern * fluidVars.simulationScale +
                           fv * fluidVars.viscosity * fluidVars.lapKern) * di;

        ++iter1;
    }
}

void SPHSimulation::calcAdvance() {
    std::vector<SPHParticle *>::iterator iter = particles.begin();
    while (iter != particles.end()) {
        Vector3d position = (*iter)->position;
        Vector3d velocity = (*iter)->velocity;
        Vector3d acceleration = (*iter)->force;

        acceleration *= fluidVars.particleMass;

        for (int i = 0; i < 6; ++i) {
            Vector3d normal(planes[i]->normal);
            double offset = planes[i]->offset;
            double dist = position.dot(normal) + offset - fluidVars.boundaryRadius;
            if (dist < 0) {
                double adj = fluidVars.boundaryStiffness * dist +
                             fluidVars.boundaryDampening * normal.dot(velocity);
                acceleration -= normal * adj;
            }
        }

        //Vector3d diff = fluidVars.boundaryRadius - (position - fluidVars.boundaryMin) * fluidVars.simulationScale, 0.0f);
        //acceleration += vars.boundaryStiffness * diff - vars.boundaryDampening * velocityEval * sign(diff);
        //diff = max(vars.boundaryRadius - (vars.boundaryMax - position) * vars.simulationScale, 0.0f);
        //acceleration -= vars.boundaryStiffness * diff + vars.boundaryDampening * velocityEval * sign(diff);

        acceleration += fluidVars.gravity;

        double accelerationLen = acceleration.length();
        if (accelerationLen > fluidVars.accelerationLimit) {
            acceleration *= fluidVars.accelerationLimit / accelerationLen;
        }

        double velocityLen = velocity.length();
        if (velocityLen > fluidVars.velocityLimit) {
            velocity *= fluidVars.velocityLimit / velocityLen;
        }

        //acceleration *= vars.accelerationLimit / max(vars.accelerationLimit, length(acceleration));
        //velocity *= vars.velocityLimit / max(vars.velocityLimit, length(velocity));

        velocity += acceleration * fluidVars.timeDiff;
        position += velocity * fluidVars.timeDiff;

        (*iter)->position = position;
        (*iter)->velocity = velocity;

        //Vector3d vnext = acceleration * fluidVars.timeDiff + velocity;
        //velocityEvalBuffer[globalIndex] = (velocity + vnext) * 0.5f;
        //(*iter)->velocity = vnext;
        //(*iter)->position = position + vnext * (fluidVars.timeDiff / fluidVars.simulationScale);

        ++iter;
    }


}
