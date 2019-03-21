#include "particle.h"

Particle::Particle()
    : pos(glm::vec3(0,0,0)), dir(glm::vec3(0,0,0)), energy(1.f), stepSize(0.5f), id(-1),
      alive(true), parent(nullptr)
{}

Particle::Particle(int id)
    : pos(glm::vec3(0,0,0)), dir(glm::vec3(0,0,0)), energy(1.f), stepSize(0.5f), id(id),
      alive(true), parent(nullptr)
{}

glm::vec3 Particle::getPos() { return pos; }

void Particle::setPos(glm::vec3 p) { pos = p; }

void Particle::setDir(glm::vec3 d) { dir = d; }

glm::vec3 Particle::getDir() { return dir; }

void Particle::setEnergy(float e) { energy = e; }

float Particle::getEnergy() { return energy; }

float Particle::getStepSize() { return stepSize; }

void Particle::setStepSize(float s) { stepSize = s; }

void Particle::setID(int i) { id = i; }

int Particle::getID() { return id; }

void Particle::setAlive(bool i) { alive = i; }

bool Particle::isAlive() { return alive; }
