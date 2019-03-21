#ifndef FACE_H
#define FACE_H
#include "halfedge.h"
#include "vertex.h"
#include "la.h"
#include "QListWidgetItem"

class HalfEdge;

class Face : public QListWidgetItem
{
public:
    HalfEdge* start_edge;
    glm::vec3 color;
    int id_face;

    Face();
    Face(int id);
    ~Face();
    void setColor (glm::vec3 c);
};

#endif // FACE_H
