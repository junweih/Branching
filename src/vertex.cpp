#include "vertex.h"

Vertex::Vertex() : position(glm::vec3(0,0,0)), halfedge(nullptr), id_vertex(-1)
{
    QString Qvert = QString("Vertex: ")+QString().number(id_vertex);
    setText(Qvert);
    influence[0] = std::pair<int, float> (-1, 0);
    influence[1] = std::pair<int, float> (-1, 0);
}

Vertex::Vertex(int id) : position(glm::vec3(0,0,0)), halfedge(nullptr), id_vertex(id)
{
    QString Qvert = QString("Vertex: ")+QString().number(id);
    setText(Qvert);
}

Vertex::~Vertex() {
//    delete halfedge;
}
