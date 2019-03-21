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

void Branch::setFloretTest(int num, float radius) {
    Sampler sampler;
    sampler.thetaMax = 120.f;
    sampler.generateSamples(num, STRATIFIED, SPHERE_CAP);
    int size = m_Particles.size();
    for (int i = 0; i < num; i++) {
        Particle particle(particleID++);
        glm::vec3 pos(radius * sampler.peekSamples()[i * 3 + 0],
                      radius * sampler.peekSamples()[i * 3 + 2],
                      radius * sampler.peekSamples()[i * 3 + 1]);
        particle.setPos(pos);
        m_Particles.push_back(particle);
        m_florets.push_back(pos);

    }
}

void Branch::createBranchTimeNewMethod() {
    for (int i = 0; i < time; i++) {
        updateBranchNewMethod();
    }
}

bool Branch::findNearestParticle(Particle particle, Particle *nearestPaticle) {
    float distance = 100000.f;
    Particle np;
    bool flag = false;
    glm::vec3 baseDir = base - particle.getPos();
    for (Particle p : m_Particles) {
        float dis = glm::distance(p.getPos(), particle.getPos());
        if (p.getID() == particle.getID()) continue;
        glm::vec3 particleDir = p.getPos() - particle.getPos();
        if (glm::dot(baseDir, particleDir) <= 0) continue;
        if (dis < distance) {
            np = p;
            distance = dis;
            flag = true;
        }
    }
    if (flag) {
        *nearestPaticle = np;
    }
    return flag;
}

void Branch::resetParticle() {
    m_Particles.clear();
    int size = m_florets.size();
    m_Particles.reserve(size);
    particleID = 0;
    for (int i = 0; i < size; i++) {
        glm::vec3 pos = m_florets[i];
        Particle p(particleID++);
        p.setPos(pos);
        m_Particles.push_back(p);
    }
    m_paths.clear();
}

void Branch::mergeParticles(Particle &p1, Particle &p2, int &newsize) {
    if (p1.isAlive() && p2.isAlive()) {
        int size = m_Particles.size();
        Particle p(particleID++);
        float energy1 = p1.getEnergy();
        float energy2 = p2.getEnergy();
        p.setEnergy(energy1 + energy2);
        glm::vec3 pos1 = p1.getPos();
        glm::vec3 pos2 = p2.getPos();
        p.setPos((pos1 + pos2) / 2.f);
        p.setDir(base - p.getPos());
        p.setStepSize(p1.getStepSize());

        for (int i = size - 1; i >= 0; i--) {
            if (m_Particles[i].getID() == p1.getID() ||
                m_Particles[i].getID() == p2.getID())
            {
                m_Particles.erase(m_Particles.begin() + i);
            }
        }
        m_Particles.push_back(p);
        newsize--;
    }
    else if (p1.isAlive()) {
        p1.setAlive(false);
        p1.setPos(p2.getPos());
    }
    else if (p2.isAlive()) {
        p2.setAlive(false);
        p2.setPos(p1.getPos());
    }

}

void Branch::addFloret(glm::vec3 pos) { m_florets.push_back(pos); }

void Branch::addParticle(Particle p) { m_Particles.push_back(p); }

void Branch::updateBranchNewMethod() {
    float tolorance = 0.05;
    int size = m_Particles.size();
    for (int i = 0; i < size; i++) {
        if (!m_Particles[i].isAlive()) continue;
        if (glm::distance(base, m_Particles[i].getPos()) < 0.1) {
            m_Particles[i].setAlive(false);
            continue;
        }
        Particle nearestParticle(-2);
        glm::vec3 oldPos = m_Particles[i].getPos();
        glm::vec3 baseDir = glm::normalize(base - oldPos);
        glm::vec3 newDir(0,0,0);
        glm::vec3 oldDir = m_Particles[i].getDir();
        if (glm::length(oldDir) != 0) oldDir = glm::normalize(oldDir);
        if (findNearestParticle(m_Particles[i], &nearestParticle)) {
            float dis = glm::distance(m_Particles[i].getPos(), nearestParticle.getPos());
            if (dis < tolorance) {
                mergeParticles(m_Particles[i], nearestParticle, size);
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
        Particle newParticle(particleID++);
        newParticle.setPos(newPos);
        newParticle.setDir(newDir);
        newParticle.setEnergy(m_Particles[i].getEnergy());
        newParticle.setStepSize(m_Particles[i].getStepSize());
        m_Particles[i].setAlive(false);
        m_Particles.push_back(newParticle);
        Path path;
        path.pos.push_back(oldPos);
        path.pos.push_back(newPos);
        path.color.push_back(getParticleColor(m_Particles[i]));
        path.color.push_back(getParticleColor(newParticle));
        m_paths.push_back(path);

    }

}

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
