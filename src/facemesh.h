#ifndef FACEMESH_H
#define FACEMESH_H

#include "drawable.h"
#include "face.h"
#include <QColor>
#include <QColorDialog>

class FaceMesh : public Drawable
{
public:
    FaceMesh(GLWidget277 *context);
    ~FaceMesh();
    void create();
    GLenum drawMode() override;
    std::vector<GLuint> face_idx;

    std::vector<glm::vec4> face_vert_pos;
    std::vector<glm::vec4> face_vert_col;
    std::vector<Face*> m_faces_mesh;

    void setFaceColor(QColor);
    Face* mp_face;

};

#endif // FACEMESH_H
