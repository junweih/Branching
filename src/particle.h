#ifndef PARTICLE_H
#define PARTICLE_H
#include "la.h"

struct Segment {
    glm::vec3 start;
    glm::vec3 end;
};

struct Path {
    std::vector<Segment> seg;
    float energy;
    Path* next = nullptr;
};
class Particle
{
private:
    glm::vec3 pos;
    glm::vec3 dir;
    float energy;
    int id;
    bool alive;
    Path path;

public:
    Particle();
    Particle(int id, glm::vec3 pos, Path p, float energy = 1.f, glm::vec3 dir = glm::vec3(0,0,0));
    glm::vec3 getPos();
    void setPos(glm::vec3 p);
    glm::vec3 getDir();
    void setDir(glm::vec3 d);
    void setEnergy(float e);
    float getEnergy();

    void setID(int i);
    int getID();
    void setAlive(bool i);
    bool isAlive();
    Path getPath();
    void setPath(Path p);
};

Segment createSegment(glm::vec3 start, glm::vec3 end);

void appendPath(Path &p, Segment seg);

void appendPath(Path &p, glm::vec3 pos, float energy);

#endif // PARTICLE_H
