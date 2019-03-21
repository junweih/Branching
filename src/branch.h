#ifndef BRANCH_H
#define BRANCH_H
#include "la.h"
#include "sampler.h"
#include "warpfunctions.h"
#include "particle.h"

struct Path {
    std::vector<glm::vec3> pos;
    std::vector<glm::vec3> color;
};

enum BRANCHTYPE : unsigned char
{
    UMBEL, MONOPODIAL, SYMPODIAL
};
class Branch
{
private:
    glm::vec3 base;
    std::vector<Particle> m_Particles;
    std::vector<glm::vec3> m_florets;
    std::vector<Path> m_paths;
    float stepSize;
    float mergeDis;
    int particleID;
    int time;
    float wa;
    float wb;
    float wh;


public:
    Branch();
    void setBasePoint(glm::vec3 p);
    void setFloretPoints(std::vector<glm::vec3> f);
    void setStepSize (float i);
    glm::vec3 getBasePos();
    std::vector<Path> getPaths();
    std::vector<glm::vec3> getFlorets();
    std::vector<Particle> getParticles();
    void setWa(float i);
    void setWb(float i);
    void setWh(float i);

    void createTestScene();
    void createBranch(BRANCHTYPE bt);
    void createBranchTimeNewMethod();
    void setFloretTest (int num, float radius);
    bool findNearestParticle (Particle particle, Particle *nearestPaticle);
    void resetParticle();
    void mergeParticles(Particle p1, Particle p2);
    void addParticle(Particle p);
    void addFloret(glm::vec3 pos);
    void updateBranchNewMethod();
    glm::vec3 getParticleColor(Particle particle);
    void setTime(int t);
};

#endif // BRANCH_H
