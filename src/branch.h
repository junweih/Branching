#ifndef BRANCH_H
#define BRANCH_H
#include "la.h"
#include "sampler.h"
#include "warpfunctions.h"
#include "particle.h"

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
    void createBranchTimeNewMethod();
    void setFloretTest (int num, float radius);
    bool findNearestParticle (Particle particle, Particle &nearestPaticle, Path &nearestPath,
                              int &segmentNum);
    void resetParticle();
    void mergeParticles(Particle &p1, Particle &p2, int &size, int segNum);
    void addFloret(glm::vec3 pos);
    void updateBranchNewMethod();
    glm::vec3 getParticleColor(Particle particle);
    void setTime(int t);
    int numAlive();
    bool findNearestPathPos(glm::vec3 pos, Path p, glm::vec3 &isect, float &dis, int &segNum);
    // distance of point and segment, return intersection isect
    float disPosSeg(glm::vec3 pt, glm::vec3 begin, glm::vec3 end, glm::vec3 &isect);
    void pushBackPath(Path p);
    void splitPath(Particle particle, Path &oldPath, Path &newPath, int segmentNum);
    bool inPath (Particle particle, Path path);
    void changePathPos(Particle particle, glm::vec3 pos);
    void erasePath(Path path);
    void updatePath(Path p, glm::vec3 pos, float energy);
};

#endif // BRANCH_H
