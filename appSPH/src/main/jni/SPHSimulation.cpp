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

    double particleMass = 0.00020543;
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
    std::vector<SPHParticle *>::const_iterator iter = particles.begin();
    while (iter != particles.end()) {
        SPHParticle *Pi = *iter++;
        double sum = applyPressure(Pi);
        Pi->pressure = (sum - fluidVars.restDensity) * fluidVars.internalStiffness;
        Pi->density = 1.0 / sum;
    }
}

void SPHSimulation::calcForce() {
    std::vector<SPHParticle *>::const_iterator iter = particles.begin();
    while (iter != particles.end()) {
        SPHParticle *Pi = *iter++;
        Pi->force = applyForce(Pi);
    }
}

void SPHSimulation::calcAdvance() {
    std::vector<SPHParticle *>::iterator iter = particles.begin();
    while (iter != particles.end()) {
        Vector3d position = (*iter)->position;
        Vector3d velocity = (*iter)->velocity;
        Vector3d velocityEval = (*iter)->velocityEval;
        Vector3d acceleration = (*iter)->force;

        acceleration *= fluidVars.particleMass;

        for (int i = 0; i < 6; ++i) {
            Vector3d normal(planes[i]->normal);
            double offset = planes[i]->offset;
            double dist = position.dot(normal) + offset - fluidVars.boundaryRadius;
            if (dist < 0) {
                double adj = fluidVars.boundaryStiffness * dist +
                             fluidVars.boundaryDampening * normal.dot(velocityEval);
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

        //velocity += acceleration * fluidVars.timeDiff;
        //position += velocity * fluidVars.timeDiff;

        //(*iter)->position = position;
        //(*iter)->velocity = velocity;

        Vector3d vnext = acceleration * fluidVars.timeDiff + velocity;
        (*iter)->velocityEval = (velocity + vnext) * 0.5f;
        (*iter)->velocity = vnext;
        (*iter)->position = position + vnext * (fluidVars.timeDiff / fluidVars.simulationScale);

        ++iter;
    }


}

double SPHSimulation::applyPressure(const SPHParticle *Pi) {
    double sum = 0;
    Vector3d ri = Pi->position;
    std::vector<SPHParticle *>::const_iterator iter = particles.begin();
    while (iter != particles.end()) {
        const SPHParticle *Pj = *iter++;
        Vector3d rj = Pj->position;
        Vector3d rij = ri - rj;
        double r2 = rij.dot(rij);
        if (r2 > 0.0 && r2 < fluidVars.rd2) {
            sum += pow((fluidVars.rd2 - r2) * fluidVars.d2, 3.0);
        }
    }
    sum = fluidVars.particleMass * fluidVars.poly6Kern * sum;
    if (sum == 0.0) {
        sum = 1.0;
    }
    return sum;
}

Vector3d SPHSimulation::applyForce(const SPHParticle *Pi) {
    Vector3d force;

    Vector3d ri = Pi->position;
    Vector3d vi = Pi->velocityEval;
    double pi = Pi->pressure;
    double di = Pi->density;

    std::vector<SPHParticle *>::const_iterator iter = particles.begin();
    while (iter != particles.end()) {
        const SPHParticle *Pj = *iter++;

        Vector3d rj = Pj->position;
        Vector3d rij = ri - rj;
        double r2 = rij.dot(rij);
        if (r2 > 0.0 && r2 < fluidVars.rd2) {
            double r = sqrt(r2 * fluidVars.d2);
            double c = fluidVars.smoothingRadius - r;
            Vector3d vj = Pj->velocityEval;
            double pj = Pj->pressure;
            double dj = Pj->density;

            double pterm =
                    fluidVars.simulationScale * -0.5 * c * fluidVars.lapKern * (pi + pj) / r;
            force += (rij * pterm + (vj - vi) * fluidVars.viscosity) * c * di * dj;
        }
    }

    return force;
}
