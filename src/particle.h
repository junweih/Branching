#ifndef PARTICLE_H
#define PARTICLE_H
#include "la.h"

class Particle
{
private:
    glm::vec3 pos;
    glm::vec3 dir;
    float energy;
    float stepSize;
    int id;
    bool alive;

public:
    Particle();
    Particle(int id);
    glm::vec3 getPos();
    void setPos(glm::vec3 p);
    glm::vec3 getDir();
    void setDir(glm::vec3 d);
    void setEnergy(float e);
    float getEnergy();
    void setStepSize(float s);
    float getStepSize();
    void setID(int i);
    int getID();
    void setAlive(bool i);
    bool isAlive();
};

#endif // PARTICLE_H
