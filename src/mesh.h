#ifndef MESH_H
#define MESH_H
#include "la.h"
#include "halfedge.h"
#include "vertex.h"
#include "face.h"
#include "drawable.h"
#include <QWidget>
#include <vector>
#include <QListWidget>
#include <QListWidgetItem>
#include <map>




class Mesh : public Drawable
{
public:

    std::vector<HalfEdge*> m_HalfEdges;
    std::vector<Face*> m_Faces;
    std::vector<Vertex*> m_Vertices;

    std::vector<glm::vec4> mesh_vert_col;
    std::vector<glm::vec4> mesh_vert_pos;
    std::vector<glm::vec4> mesh_vert_nor;
    std::vector<GLuint> mesh_idx;
    std::vector<glm::ivec2> mesh_jointID;
    std::vector<glm::vec2> mesh_weights;

    //Mesh();
    Mesh(GLWidget277 *context);

    void create();
    int getIndex();
    void loadCube();
};

#endif // MESH_H
