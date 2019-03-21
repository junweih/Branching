#pragma once

#include "drawable.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <la.h>

class Sphere : public Drawable
{
public:
    Sphere(GLWidget277 *mp_context);
    virtual void create();
    void createSphere(glm::vec3 pos, float radius, glm::vec4 col);
};
