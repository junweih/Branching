#include "linemesh.h"

LineMesh::LineMesh(GLWidget277 *context) : Drawable(context)
{}

LineMesh::~LineMesh() {
}

GLenum LineMesh::drawMode() {
    return GL_LINES;
}

void LineMesh::create()
{
    int size = m_halfedges_mesh.size();

    lne_idx.reserve(size);
    int n = 0;
    for (int i = 0; i < size; i++) {
        HalfEdge* he_previous = m_halfedges_mesh[i];
        while (he_previous->next_he != m_halfedges_mesh[i]) {
            he_previous = he_previous->next_he;
        }


        lne_vert_pos.push_back(glm::vec4(he_previous->vertex->position, 1));
        lne_vert_pos.push_back(glm::vec4(m_halfedges_mesh[i]->vertex->position, 1));

        lne_idx.push_back(n++);
        lne_vert_col.push_back(glm::vec4(1,1,0,1));

        lne_idx.push_back(n++);
        lne_vert_col.push_back(glm::vec4(1,0,0,1));
    }

    count = size * 2;

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*2 * sizeof(GLuint), lne_idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, size*2 * sizeof(glm::vec4), lne_vert_pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, size*2 * sizeof(glm::vec4), lne_vert_col.data(), GL_STATIC_DRAW);
}

glm::vec3 energyToColor(float energy) {
    float weight = 0.5f + energy * 0.1f;
    return glm::vec3(0,weight,0);
}

void LineMesh::createBranch(Branch branch) {
    int size = 0;
    int n = 0;
    for (int i = 0; i < branch.getPaths().size(); i++) {

        Path path = branch.getPaths()[i];
        for (int j = 0; j < path.seg.size(); j++) {
            lne_vert_pos.push_back(glm::vec4(path.seg[j].start, 1));
            glm::vec3 color1 = energyToColor(path.energy);
            lne_vert_col.push_back(glm::vec4(color1,1));
            lne_idx.push_back(n++);

            lne_vert_pos.push_back(glm::vec4(path.seg[j].end, 1));
            glm::vec3 color2 = energyToColor(path.energy);
            lne_vert_col.push_back(glm::vec4(color2,1));
            lne_idx.push_back(n++);
            size++;
        }
    }

    count = size * 2;

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*2 * sizeof(GLuint), lne_idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, size*2 * sizeof(glm::vec4), lne_vert_pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, size*2 * sizeof(glm::vec4), lne_vert_col.data(), GL_STATIC_DRAW);

}

void LineMesh::createPath(Path path) {
    int size = 0;
    int n = 0;
    for (int j = 0; j < path.seg.size(); j++) {
        lne_vert_pos.push_back(glm::vec4(path.seg[j].start, 1));
        glm::vec3 color1 = energyToColor(path.energy);
        lne_vert_col.push_back(glm::vec4(color1,1));
        lne_idx.push_back(n++);

        lne_vert_pos.push_back(glm::vec4(path.seg[j].end, 1));
        glm::vec3 color2 = energyToColor(path.energy);
        lne_vert_col.push_back(glm::vec4(color2,1));
        lne_idx.push_back(n++);
        size++;
    }

    count = size * 2;

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*2 * sizeof(GLuint), lne_idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, size*2 * sizeof(glm::vec4), lne_vert_pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, size*2 * sizeof(glm::vec4), lne_vert_col.data(), GL_STATIC_DRAW);

}
