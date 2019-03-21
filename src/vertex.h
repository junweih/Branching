#ifndef VERTEX_H
#define VERTEX_H
#include "halfedge.h"
#include "face.h"
#include <la.h>
#include <QListWidgetItem>
#include "joint.h"
class HalfEdge;

class Vertex : public QListWidgetItem
{
public:
    glm::vec3 position;
    HalfEdge* halfedge;
    int id_vertex;

    std::pair<int, float> influence[2];


    Vertex();
    Vertex(int id);
    ~Vertex();
};

#endif // VERTEX_H
