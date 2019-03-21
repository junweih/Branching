#ifndef VERTEXMESH_H
#define VERTEXMESH_H
#include "drawable.h"
#include "halfedge.h"
#include "face.h"
#include "vertex.h"


class VertexMesh : public Drawable
{
public:
    VertexMesh(GLWidget277 *context);
    ~VertexMesh();
    Vertex* vertex;
    GLenum drawMode() override;
    void create() override;
    std::vector<glm::vec4> pnt_vert_pos;
    std::vector<glm::vec4> pnt_vert_col;
    std::vector<Vertex*> m_vertices;
};

#endif // VERTEXMESH_H
