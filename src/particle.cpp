#include "particle.h"

Particle::Particle()
    : pos(glm::vec3(0,0,0)), dir(glm::vec3(0,0,0)), energy(1.f), id(-1),
      alive(true)
{}

Particle::Particle(int id, glm::vec3 pos, Path p, float energy, glm::vec3 dir)
    : pos(pos), dir(dir), energy(energy), id(id),
      alive(true), path(p)
{}

glm::vec3 Particle::getPos() { return pos; }

void Particle::setPos(glm::vec3 p) { pos = p; }

void Particle::setDir(glm::vec3 d) { dir = d; }

glm::vec3 Particle::getDir() { return dir; }

void Particle::setEnergy(float e) { energy = e; }

float Particle::getEnergy() { return energy; }

void Particle::setID(int i) { id = i; }

int Particle::getID() { return id; }

void Particle::setAlive(bool i) { alive = i; }

bool Particle::isAlive() { return alive; }

Path Particle::getPath() { return path; }

void Particle::setPath(Path p) { path = p; }

Segment createSegment(glm::vec3 start, glm::vec3 end) {
    Segment s;
    s.start = start;
    s.end = end;
    return s;
}

void appendPath(Path &p, Segment seg) {
    p.seg.push_back(seg);
}

void appendPath(Path &p, glm::vec3 pos, float energy) {
    if (p.seg.size() == 0) {
        Segment s = createSegment(pos, pos);
        appendPath(p, s);
    }
    else {
        int size = p.seg.size();
        glm::vec3 start = p.seg[size-1].end;
        Segment s = createSegment(start, pos);
        appendPath(p, s);
    }
    p.energy = energy;
}
