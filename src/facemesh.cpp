#include "facemesh.h"

FaceMesh::FaceMesh(GLWidget277 *context) : Drawable(context)
{}

FaceMesh::~FaceMesh() {
}

GLenum FaceMesh::drawMode() {
    return GL_LINES;
}

void FaceMesh::create() {
    int size = m_faces_mesh.size();
    face_idx.reserve(size);

    int n = 0;
    for (int i = 0; i < size; i++) {
        HalfEdge* he;
        for (he = m_faces_mesh[i]->start_edge; he->next_he != m_faces_mesh[i]->start_edge;
             he = he->next_he) {
            face_vert_pos.push_back(glm::vec4(he->vertex->position, 1));
            face_vert_pos.push_back(glm::vec4(he->next_he->vertex->position, 1));
            face_idx.push_back(n++);
            face_idx.push_back(n++);
            face_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - m_faces_mesh[i]->color, 1));
            face_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - m_faces_mesh[i]->color, 1));
        }
        face_vert_pos.push_back(glm::vec4(he->vertex->position, 1));
        face_vert_pos.push_back(glm::vec4(he->next_he->vertex->position, 1));
        face_idx.push_back(n++);
        face_idx.push_back(n++);
        face_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - m_faces_mesh[i]->color, 1));
        face_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - m_faces_mesh[i]->color, 1));

    }

    count = size * n;

    size *= n;

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), face_idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), face_vert_pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), face_vert_col.data(), GL_STATIC_DRAW);

}

void FaceMesh::setFaceColor(QColor color) {
    int *r = new int(0);
    int *g = new int(0);
    int *b = new int(0);
    color.getRgb(r, g, b);
    float normR, normG, normB;

    normR = (float) (*r) / 255;
    normG = (float) (*g) / 255;
    normB = (float) (*b) / 255;
    glm::vec3 rgb(normR, normG, normB);
    face_vert_col.clear();
//    fce_vert_col.push_back(glm::vec4(glm::vec3(1,1,1) - rgb, 1));
    face_vert_col.push_back(glm::vec4(rgb, 1));
    mp_face->color = rgb;
}
