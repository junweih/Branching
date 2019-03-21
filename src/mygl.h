#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/sphere.h>
#include "camera.h"
#include "face.h"
#include "mesh.h"
#include "halfedge.h"
#include "vertex.h"
#include "vertexmesh.h"
#include "linemesh.h"
#include "facemesh.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QColor>
#include <QColorDialog>
#include <joint.h>


class MyGL
    : public GLWidget277
{
    Q_OBJECT
private:
    Cylinder m_geomCylinder;// The instance of a unit cylinder we can use to render any cylinder
    Sphere m_geomSphere;// The instance of a unit sphere we can use to render any sphere
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)

    ShaderProgram m_progSkeleton;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;


public:
    bool newFileImport;
    Mesh geom_mesh;
    bool loadNewObj = false;
    bool drawFlat = true;
    Joint* joint;
    QString filename = nullptr;

    int curJointID = 0;
    bool BindMesh = false;

    std::vector<VertexMesh*> m_selectedVertexMesh;
    std::vector<LineMesh*> m_selectedLineMesh;
    std::vector<FaceMesh*> m_selectedFaceMesh;
    std::vector<Joint*> m_jointMesh;
    std::vector<Sphere*> m_sphereMesh;

    Branch branch;

    int time = 0;

    HalfEdge* selected_HalfEdge = nullptr;
    Vertex* selected_Vertex = nullptr;
    Face* selected_Face = nullptr;
    Joint* selected_Joint = nullptr;


public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void emitCubeCreateSignal();

    void highlightVertex(Vertex* v);
    void highlightLine(HalfEdge* line);
    void highlightFace(Face* face);
    void highlightJoint(Joint* joint);

    void on_action_N_nexthalfedge();
    void on_action_M_SYM();
    void on_action_F_faceOfHalfEdge();
    void on_action_V_vertexOfHalfEdge();
    void on_action_H_halfedgeOfVertex();
    void on_action_H_Shift_halfedgeOfFace();

    void addVertex();
    void triangulate();

    void setFaceColor(QColor);

    void subdivision();

    Vertex* search_map_Centroid(std::map<Vertex*, Face*> & mapOfWords, Face* face);

    bool search_map_is_Line(std::map<bool, HalfEdge*> & mapOfWords, HalfEdge* face);

    void extrude();

    int search_map_vertex(std::map<int, Vertex*> & maps, Vertex* vertex);

    void loadOBJ();

    void noHightlightVertex();

    void noHightlightLine();

    void noHightlightFace();

    bool loadJson();

    Joint* getRootJoint(QString s);

    Joint* initializeJson(QJsonObject jsonObject, Joint* parent);

    void updateJointTransform(ShaderProgram);

    void updateBranch(Branch *branch);

    void updateNextTimeBranch();

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void signal_createCube();
    void signal_setCurrentHalfEdgeListWidgetItem(QListWidgetItem*);
    void signal_setCurrent_Face_ListWidgetItem(QListWidgetItem*);
    void signal_setCurrent_Vertex_ListWidgetItem(QListWidgetItem*);
    void signal_noNewFileImport();
    void signal_loadJsonRoot(Joint* jointRoot);
};


#endif // MYGL_H
