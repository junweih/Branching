#ifndef LINEMESH_H
#define LINEMESH_H

#include "drawable.h"
#include "halfedge.h"
#include "branch.h"

class LineMesh : public Drawable
{
public:
    LineMesh(GLWidget277 *context);
    ~LineMesh();
    void create();
    void createPath1(Branch branch);
    void createPath(Path path);
    GLenum drawMode() override;
    std::vector<GLuint> lne_idx;

    std::vector<glm::vec4> lne_vert_pos;
    std::vector<glm::vec4> lne_vert_col;
    std::vector<HalfEdge*> m_halfedges_mesh;
};

#endif // LINEMESH_H
