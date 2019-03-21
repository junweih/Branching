#include "face.h"

Face::Face() : start_edge(nullptr), color(glm::vec3(0,0,0)), id_face(-1)
{
    QString Qface = QString("Face: ")+QString().number(id_face);
    setText(Qface);
}

Face::Face(int id) : start_edge(nullptr), color(glm::vec3(0,0,0)), id_face(id)
{
    QString Qface = QString("Face: ")+QString().number(id_face);
    setText(Qface);
}

Face::~Face() {
//    delete start_edge;
}

void Face::setColor(glm::vec3 c) {
    color = c;
}
