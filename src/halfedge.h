#ifndef HALFEDGE_H
#define HALFEDGE_H
#include "face.h"
#include "vertex.h"
#include "la.h"
#include "QListWidgetItem"

class Vertex;
class Face;

class HalfEdge : public QListWidgetItem
{
public:
    HalfEdge* next_he;
    HalfEdge* sym_he;
    Face* face;
    Vertex* vertex;
//    Vertex* vert_before;
    int id_he;

    HalfEdge();
    HalfEdge(int id);
    ~HalfEdge();
};

#endif // HALFEDGE_H
