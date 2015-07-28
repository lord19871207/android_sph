#include "SPHSimulation.h"

struct FluidVars {
    double timeDiff = 1.0 / 360.0;
    double restDensity = 600.0;
    double internalStiffness = 1.5;
    double viscosity = 0.35;

    double boundaryStiffness = 10000.0;
    double boundaryDampening = 256.0;

    double accelerationLimit = 150.0;
    double velocityLimit = 600.0;
    double simulationScale = 1.0;

    Vector2d gravity = {0.0, -9.8};

    double particleMass = 0.00020543;
    double smoothingRadius = 0.04;
    double boundaryRadius = 0.04;

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

    planes = new SPHPlane *[4];
    planes[0] = new SPHPlane(Vector2d(1, 0), 1);
    planes[1] = new SPHPlane(Vector2d(-1, 0), 1);
    planes[2] = new SPHPlane(Vector2d(0, -1), 1);
    planes[3] = new SPHPlane(Vector2d(0, 1), 1);
}

SPHSimulation::~SPHSimulation() {
    std::vector<SPHParticle *>::iterator iter = particles.begin();
    while (iter != particles.end()) {
        delete *iter++;
    }

    if (planes) {
        for (int i = 0; i < 4; ++i) {
            delete planes[i];
        }
        delete planes;
        planes = nullptr;
    }
}

void SPHSimulation::initialize(double left, double right, double top, double bottom) {
    planes[0]->offset = abs(left);
    planes[1]->offset = abs(right);
    planes[2]->offset = abs(top);
    planes[3]->offset = abs(bottom);

    std::vector<SPHParticle *>::const_iterator iter = particles.begin();
    while (iter != particles.end()) {
        double x = left + (right - left) * rand() / (double) RAND_MAX;
        double y = bottom + (top - bottom) * rand() / (double) RAND_MAX;
        SPHParticle *particle = *iter++;
        particle->position.v[0] = x;
        particle->position.v[1] = y;
    }
}

void SPHSimulation::simulate() {
    calcGrids();
    calcPressure();
    calcForce();
    calcAdvance();
}

void SPHSimulation::calcGrids() {
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        grid[i].clear();
    }

    double sw = planes[0]->offset + planes[1]->offset;
    double sh = planes[2]->offset + planes[3]->offset;

    std::vector<SPHParticle*>::const_iterator iter = particles.begin();
    while (iter != particles.end()) {
        const SPHParticle* P = *iter++;

        double tx = (P->position.v[0] + planes[0]->offset) / sw;
        double ty = (P->position.v[1] + planes[3]->offset) / sh;

        int gx = (int)round(tx * GRID_SIZE);
        int gy = (int)round(ty * GRID_SIZE);

        if (gx < 0) {
            gx = 0;
        }
        if (gx >= GRID_SIZE) {
            gx = GRID_SIZE - 1;
        }
        if (gy < 0) {
            gy = 0;
        }
        if (gy >= GRID_SIZE) {
            gy = GRID_SIZE - 1;
        }

        for (int ii = 0; ii < 9; ++ii) {
            int dx = ii % 3 - 1;
            int dy = ii / 3 - 1;

            int ggx = gx + dx;
            int ggy = gy + dy;

            if (ggx >= 0 && ggx < GRID_SIZE && ggy >= 0 && ggy < GRID_SIZE) {
                grid[ggy * GRID_SIZE + ggx].push_back(P);
            }
        }
    }
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
        SPHParticle *P = *iter++;

        Vector2d position = P->position;
        Vector2d velocity = P->velocity;
        Vector2d velocityEval = P->velocityEval;
        Vector2d acceleration = P->force;

        acceleration *= fluidVars.particleMass;

        for (int i = 0; i < 4; ++i) {
            Vector2d normal(planes[i]->normal);
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

        Vector2d vnext = acceleration * fluidVars.timeDiff + velocity;
        P->velocityEval = (velocity + vnext) * 0.5f;
        P->velocity = vnext;
        P->position = position + vnext * (fluidVars.timeDiff / fluidVars.simulationScale);
    }


}

double SPHSimulation::applyPressure(const SPHParticle *Pi) {
    int gridIndex = getGridIndex(Pi);
    double sum = 0;
    Vector2d ri = Pi->position;
    std::vector<const SPHParticle *>::const_iterator iter = grid[gridIndex].begin();
    while (iter != grid[gridIndex].end()) {
        const SPHParticle *Pj = *iter++;
        Vector2d rj = Pj->position;
        Vector2d rij = ri - rj;
        double r2 = rij.dot(rij);
        if (r2 > 0.0 && r2 < fluidVars.rd2) {
            sum += pow((fluidVars.rd2 - r2) * fluidVars.d2, 3.0);
        }
    }
    sum = fluidVars.particleMass * fluidVars.poly6Kern * sum;
    if (sum < 1.0) {
        sum = 1.0;
    }
    return sum;
}

Vector2d SPHSimulation::applyForce(const SPHParticle *Pi) {
    int gridIndex = getGridIndex(Pi);

    Vector2d fp;
    Vector2d fv;

    Vector2d ri = Pi->position;
    Vector2d vi = Pi->velocityEval;
    double pi = Pi->pressure;
    double di = Pi->density;

    std::vector<const SPHParticle *>::const_iterator iter = grid[gridIndex].begin();
    while (iter != grid[gridIndex].end()) {
        const SPHParticle *Pj = *iter++;

        Vector2d rj = Pj->position;
        Vector2d rij = ri - rj;
        double r2 = rij.dot(rij);
        if (r2 > 0.0 && r2 < fluidVars.rd2) {
            double r = sqrt(r2 * fluidVars.d2);
            double c = fluidVars.smoothingRadius - r;
            Vector2d vj = Pj->velocityEval;
            double pj = Pj->pressure;
            double dj = Pj->density;

            fp += rij * (c * c * (pi + pj) * dj / r);
            fv += (vj - vi) * c * dj;
        }
    }

    return (fp * 0.5 * fluidVars.spikyKern * fluidVars.simulationScale +
            fv * fluidVars.viscosity * fluidVars.lapKern) * di;
}

int SPHSimulation::getGridIndex(const SPHParticle *P) {
    double sw = planes[1]->offset + planes[0]->offset;
    double sh = planes[2]->offset + planes[3]->offset;
    double tx = (P->position.v[0] + planes[0]->offset) / sw;
    double ty = (P->position.v[1] + planes[3]->offset) / sh;
    int gx = (int)round(tx * GRID_SIZE);
    int gy = (int)round(ty * GRID_SIZE);
    if (gx < 0) {
        gx = 0;
    }
    if (gx >= GRID_SIZE) {
        gx = GRID_SIZE - 1;
    }
    if (gy < 0) {
        gy = 0;
    }
    if (gy >= GRID_SIZE) {
        gy = GRID_SIZE - 1;
    }
    return gy * GRID_SIZE + gx;
}
