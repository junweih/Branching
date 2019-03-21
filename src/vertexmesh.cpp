#include "vertexmesh.h"

VertexMesh::VertexMesh(GLWidget277 *context) : Drawable(context)
{}

VertexMesh::~VertexMesh() {
    delete vertex;
}

GLenum VertexMesh::drawMode() {
    return GL_POINTS;
}

void VertexMesh::create() {

    int size = m_vertices.size();

    std::vector<GLuint> vert_idx;
    vert_idx.reserve(size);

    for (int i = 0; i < m_vertices.size(); i++) {
        pnt_vert_pos.push_back(glm::vec4(m_vertices[i]->position,1));
        vert_idx[i] = i;
        pnt_vert_col.push_back(glm::vec4(1,1,1,1));
    }

    count = size;

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), vert_idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(glm::vec4), pnt_vert_pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(glm::vec4), pnt_vert_col.data(), GL_STATIC_DRAW);

}
