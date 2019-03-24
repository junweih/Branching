#include "branch.h"

Branch::Branch()
    : base(glm::vec3(0,0,0)), m_florets(), m_paths(), stepSize(.5f), mergeDis(0.1f),
      particleID(0), wa(1.f), wb(1.f), wh(1.f)
{}

void Branch::setBasePoint(glm::vec3 p) { base = p; }

void Branch::setFloretPoints(std::vector<glm::vec3> f) { m_florets = f; }

void Branch::setStepSize(float i) { stepSize = i; }

glm::vec3 Branch::getBasePos() { return base; }

std::vector<glm::vec3> Branch::getFlorets() { return m_florets; }

std::vector<Path> Branch::getPaths() { return m_paths; }

std::vector<Particle> Branch::getParticles() { return m_Particles; }

void Branch::setTime(int t) { time = t; }

void Branch::setWa(float i) { wa = i; }

void Branch::setWb(float i) { wb = i; }

void Branch::setWh(float i) { wh = i; }

glm::vec3 Branch::getParticleColor(Particle particle) {
    float weight = 0.5f + particle.getEnergy() * 0.1f;
    return glm::vec3(0,weight,0);
}

int Branch::numAlive() {
    int sum = 0;
    int size = m_Particles.size();
    for (int i = 0; i < size; i++) {
        if (m_Particles[i].isAlive())
            sum++;
    }
    return sum;
}

void Branch::setFloretTest(int num, float radius) {
    Sampler sampler;
    sampler.thetaMax = 120.f;
    sampler.generateSamples(num, STRATIFIED, SPHERE_CAP);
    for (int i = 0; i < num; i++) {
        glm::vec3 pos(radius * sampler.peekSamples()[i * 3 + 0],
                      radius * sampler.peekSamples()[i * 3 + 2],
                      radius * sampler.peekSamples()[i * 3 + 1]);
        Path path;
        appendPath(path, pos, 1.f);
        m_paths.push_back(path);
        Particle particle(particleID++, pos, path);
        m_Particles.push_back(particle);
        m_florets.push_back(pos);
    }
}

void Branch::resetParticle() {
    m_Particles.clear();
    int size = m_florets.size();
    m_Particles.reserve(size);
    particleID = 0;
    m_paths.clear();
    for (int i = 0; i < size; i++) {
        glm::vec3 pos = m_florets[i];
        Path path;
        appendPath(path, pos, 1.f);
        m_paths.push_back(path);
        Particle p(particleID++, pos, path);
        m_Particles.push_back(p);

    }
}

void Branch::createBranchTimeNewMethod() {
    for (int i = 0; i < time; i++) {
        updateBranchNewMethod();
    }
}

bool Branch::findNearestParticle(Particle particle, Particle &nearestParticle, Path &nearestPath, int &segmentNum) {
    float minDis = 100000.f;
    glm::vec3 minIsect;
    Particle np;
    Path minPath;
    int segNum = -1;
    bool flag = false;
    float energy = particle.getEnergy();
    glm::vec3 particlePos = particle.getPos();
//    glm::vec3 baseDir = base - particlePos;
    for (Path p : m_paths) {
        if (inPath(particle, p)) continue;
        glm::vec3 isect;
        float dis;
        int tmpSegNum;
        if (findNearestPathPos(particlePos, p, isect, dis, tmpSegNum)) {
//            glm::vec3 particleDir = isect - particlePos;
//            if (glm::dot(particleDir, baseDir) <= 0) continue;
            if (dis < minDis) {
                minDis = dis;
                minIsect = isect;
                minPath = p;
                segNum = tmpSegNum;
                flag = true;
            }
        }
    }
    if (flag) {
        segmentNum = segNum;
        nearestPath = minPath;
        Particle particle(particleID++, minIsect, nearestPath);
        nearestParticle = particle;
        nearestParticle.setEnergy(energy);
    }
    return flag;
}



void Branch::mergeParticles(Particle &p1, Particle &p2, int &newsize, int segNum) {
    if (p1.isAlive() && p2.isAlive()) {
        glm::vec3 pos1 = p1.getPos();
        glm::vec3 pos2 = p2.getPos();
        glm::vec3 pos = (pos1 + pos2) / 2.f;
        float energy1 = p1.getEnergy();
        float energy2 = p2.getEnergy();
        float totalEnergy = energy1 + energy2;
        Path path;
        appendPath(path, pos, totalEnergy);
        Particle p (particleID++, pos, path, totalEnergy, base - pos);
        m_Particles.push_back(p);
        p1.setAlive(false);
        changePathPos(p1, pos);
        p2.setAlive(false);
        changePathPos(p2, pos);
//        for (int i = size - 1; i >= 0; i--) {
//            if (m_Particles[i].getID() == p1.getID() ||
//                m_Particles[i].getID() == p2.getID())
//            {
//                m_Particles.erase(m_Particles.begin() + i);
//            }
//        }
        newsize--;
    }
    else if (p1.isAlive()) {
        p1.setAlive(false);
        p1.setPos(p2.getPos());
        Path newPath;
        Path tmpOldPath = p2.getPath();
        erasePath(p2.getPath());
        splitPath(p2, tmpOldPath, newPath, segNum);

    }
    else if (p2.isAlive()) {
        mergeParticles(p2, p1, newsize, segNum);
    }

}

void Branch::addFloret(glm::vec3 pos) {
    m_florets.push_back(pos);
    Path path;
    appendPath(path, pos, 1.f);
    m_paths.push_back(path);
    Particle p(particleID++, pos, path);
}

void Branch::updateBranchNewMethod() {
    float tolorance = 0.5f * stepSize;
    int size = m_Particles.size();
    for (int i = 0; i < size; i++) {
        if (!m_Particles[i].isAlive()) continue;
        if (glm::distance(base, m_Particles[i].getPos()) < (2.f * tolorance)) {
            m_Particles[i].setAlive(false);
            m_Particles[i].setPos(base);
            continue;
        }
        Particle nearestParticle;
        glm::vec3 oldPos = m_Particles[i].getPos();
        glm::vec3 baseDir = glm::normalize(base - oldPos);
        glm::vec3 newDir(0,0,0);
        glm::vec3 oldDir = m_Particles[i].getDir();
        if (glm::length(oldDir) != 0) oldDir = glm::normalize(oldDir);
        Path tmpPath;
        int tmpSeg;
        if (findNearestParticle(m_Particles[i], nearestParticle, tmpPath, tmpSeg)) {
            float dis = glm::distance(m_Particles[i].getPos(), nearestParticle.getPos());
            if (dis < tolorance) {
                mergeParticles(m_Particles[i], nearestParticle, size, tmpSeg);
                continue;
            }
            else {
                glm::vec3 particleDir = glm::normalize(nearestParticle.getPos() - oldPos);
                newDir = (1.f / (wa + wb + wh) ) *
                        (wb * particleDir + wa * baseDir + wh * oldDir);
                m_Particles[i].setDir(newDir);
            }
        }
        else {
            newDir = (1.f / (wa + wh)) * (wa * baseDir + wh * oldDir);
            m_Particles[i].setDir(newDir);
        }
        glm::vec3 newPos = stepSize * newDir + oldPos;
        Particle newParticle(particleID++, newPos, m_Particles[i].getPath());
        float newEnergy = m_Particles[i].getEnergy();
        Path oldPath = m_Particles[i].getPath();
        appendPath( oldPath, newPos, newEnergy);
        newParticle.setDir(newDir);
        newParticle.setEnergy(newEnergy);
        m_Particles[i].setAlive(false);
        m_Particles.push_back(newParticle);
    }
}

bool Branch::findNearestPathPos(glm::vec3 pos, Path p, glm::vec3 &isect, float &dis, int &segNum) {
    int size = p.seg.size();
    float minDis = 10000.f;
    glm::vec3 nearPos;
    bool flag = false;
    for (int i = 0; i < size; i++) {
        glm::vec3 pa = p.seg[i].start;
        glm::vec3 pb = p.seg[i].end;
        glm::vec3 isect;
        float tmpdis = disPosSeg(pos, pa, pb, isect);
        if (tmpdis < minDis && tmpdis >= 0) {
            minDis = tmpdis;
            nearPos = isect;
            segNum = i;
            flag = true;
        }
    }
    if (flag) {
        isect = nearPos;
        dis = minDis;
    }
    return flag;
}

float Branch::disPosSeg(glm::vec3 pt, glm::vec3 begin, glm::vec3 end, glm::vec3 &isect) {
    glm::vec3 retVal;
    glm::vec3 end2Begin = begin - end;
    float epsilon = 0.00001f;
    if (glm::abs(end2Begin.x) < epsilon && glm::abs(end2Begin.y) < epsilon &&
        glm::abs(end2Begin.z) < epsilon)
    {
        retVal = begin;
        isect = retVal;
        return glm::distance(pt, begin);
    }
    double u = (pt.x - begin.x) * (begin.x - end.x) +
               (pt.y - begin.y) * (begin.y - end.y) +
               (pt.z - begin.z) * (begin.z - end.z);
    u = u / (glm::length2(end2Begin.x) + glm::length2(end2Begin.y) + glm::length2(end2Begin.z));

    retVal.x = begin.x + u * end2Begin.x;
    retVal.y = begin.y + u * end2Begin.y;
    retVal.z = begin.z + u * end2Begin.z;

    glm::vec3 AP = pt - begin;
    glm::vec3 AB = end - begin;
    float d = -1;
    float r = (glm::dot(AP, AB) / glm::length2(AB));
    if (r <= 0) {
        d = glm::distance(pt, begin);
        isect = begin;
        return d;
    }
    else if (r >= 1) {
        d = glm::distance(pt, end);
        isect = end;
        return d;
    }
    else {
        d = glm::distance(pt, retVal);
        isect = retVal;
        return d;
    }
}

void Branch::pushBackPath(Path p) {
    m_paths.push_back(p);
}

bool Branch::inPath(Particle particle, Path path) {
    int size = path.seg.size();
    for (int i = 0; i < size; i++) {
        if (glm::distance(particle.getPos(), path.seg[i].start) < 0.0001f) {
            return true;
        }
    }
    return false;
}

void Branch::splitPath(Particle particle, Path &oldPath, Path &newPath, int segmentNum) {
    Path tmpOldPath;
    Path tmpNewPath;
    int size = oldPath.seg.size();
    for (int i = 0; i <= segmentNum; i++) {
        appendPath(tmpOldPath, oldPath.seg[i]);
    }
    appendPath(tmpOldPath, particle.getPos(), oldPath.energy);

    float newEnergy = particle.getEnergy() + oldPath.energy;
    Segment newSeg = createSegment(particle.getPos(), oldPath.seg[segmentNum].end);
    appendPath(tmpNewPath, newSeg);
    for (int i = segmentNum+1; i < size; i++) {
        appendPath(tmpNewPath, oldPath.seg[i].end, newEnergy);
    }
    oldPath = tmpOldPath;
    newPath = tmpNewPath;
    m_paths.push_back(tmpOldPath);
    m_paths.push_back(newPath);
}

void Branch::changePathPos(Particle particle, glm::vec3 pos) {
    Path path = particle.getPath();
    int size = path.seg.size();
    glm::vec3 parPos = particle.getPos();
    for (int i = 0; i < size; i++) {
        if (glm::distance(parPos, path.seg[i].start) < 0.0001f) {
            path.seg[i].start = pos;
        }
        if (glm::distance(parPos, path.seg[i].end) < 0.0001f) {
            path.seg[i].end = pos;
        }
    }
}

void Branch::erasePath(Path path) {
    int size = m_paths.size();
    int pathSize = path.seg.size();
    float pathEnergy = path.energy;
    glm::vec3 pathStart = path.seg[0].start;
    for (int i = 0; i < size; i++) {
        Path p = m_paths[i];
        if (p.seg.size() != pathSize || p.energy != pathEnergy) continue;
        glm::vec3 pStart = p.seg[0].start;
        if (glm::distance(pathStart, pStart) < 0.001f) {
            m_paths.erase(m_paths.begin() + i);
            break;
        }

    }
}

void Branch::updatePath(Path p, glm::vec3 pos, float energy) {
    Path newPath = p;
    appendPath(newPath, pos, energy);
//    erasePath(p);
    m_paths.push_back(newPath);
}
