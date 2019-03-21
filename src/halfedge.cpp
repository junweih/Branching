#include "halfedge.h"

HalfEdge::HalfEdge()
    : next_he(nullptr), sym_he(nullptr), face(nullptr),
      vertex(nullptr), id_he(-1)
{
    QString Qedge = QString("HalfEdge: ")+QString().number(id_he);
    setText(Qedge);
}

HalfEdge::HalfEdge(int id)
    : next_he(nullptr), sym_he(nullptr), face(nullptr), vertex(nullptr),
       id_he(id)
{
    QString Qedge = QString("HalfEdge: ")+QString().number(id_he);
    setText(Qedge);
}

HalfEdge::~HalfEdge() {
//    delete next_he;
//    delete sym_he;
//    delete face;
//    delete vertex;
}
