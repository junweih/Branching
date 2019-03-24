#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <map>
#include <QFileDialog>
#include <QMessageBox>
#include <QDataStream>
#include <fstream>
#include <sstream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <vector>
using namespace std;


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      m_geomCylinder(this), m_geomSphere(this),
      m_progLambert(this), m_progFlat(this),
      m_progSkeleton(this),
      m_glCamera(),geom_mesh(this)
{}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_geomCylinder.destroy();
    m_geomSphere.destroy();
    geom_mesh.destroy();

}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

//    //Create the instances of Cylinder and Sphere.
//    m_geomCylinder.create();

//    Particle p1(0), p2(1);
//    glm::vec3 pos1(glm::vec3(2.f, 2.f, 2.f)), pos2(glm::vec3(3.f, 2.1f, 2.f));
//    p1.setPos(pos1);
//    p2.setPos(pos2);
//    branch.addFloret(pos1);
//    branch.addFloret(pos2);
//    branch.addParticle(p1);
//    branch.addParticle(p2);

    glm::vec3 basePos (0,0,0);
    Path p2;

    for (int i = 0; i < 5; i++) {
//        glm::vec3 pos (i-3, glm::sqrt((float)i)+2, 0);
        glm::vec3 pos (i, glm::pow((float)(i-1), 1.5f) - 3, 0);
        appendPath(p2, pos, 10.f);
    }

    Path p3;
    branch.pushBackPath(p3);
    for (int i = -5; i < 0; i++) {
        glm::vec3 pos (i+2.5, 3, 0);
//        appendPath(p3, pos, 1.f);
        branch.updatePath(p3, pos, 1.f);
    }
    Path p1;
    Particle base(0, basePos, p1);
    base.setEnergy(2.f);
    Particle pIsect;

    glm::vec3 isect;
    float dis;
    Path tmpPath;
    int tmpSeg;

//    branch.pushBackPath(p2);


    if (branch.findNearestParticle(base, pIsect, tmpPath, tmpSeg)) {
        appendPath(p1, base.getPos(), base.getEnergy());
        appendPath(p1, pIsect.getPos(), base.getEnergy());
        Path newPath;
        branch.splitPath(pIsect, tmpPath, newPath, tmpSeg);
        branch.pushBackPath(newPath);
    }
//    branch.mergeParticles(base, pIsect, 10);
    branch.pushBackPath(p1);
    appendPath(p1, basePos, 1.f);
    appendPath(p1, isect, 1.f);




//    branch.setFloretTest(1, 2.f);
//    branch.setFloretTest(49, 4.5f);

//    branch.setFloretTest(33, 3.5f);
//    branch.setFloretTest(19, 2.f);
//    branch.createBranch(UMBEL);
    time = 0;
    branch.setStepSize(0.1f);
    branch.setTime(time);
    branch.createBranchTimeNewMethod();
    for (Path p : branch.getPaths()) {
        LineMesh* pathLine = new LineMesh(this);
        pathLine->createPath(p);
        m_selectedLineMesh.push_back(pathLine);
    }
    // base
    Sphere* baseSphere = new Sphere(this);
    baseSphere->createSphere(branch.getBasePos(), 0.5, glm::vec4(0,0,1,1));
    m_sphereMesh.push_back(baseSphere);

    // floret
    for (glm::vec3 i : branch.getFlorets()) {
        Sphere* floretSphere = new Sphere(this);
        floretSphere->createSphere(i, 0.2, glm::vec4(0.5, 0,0,0.5));
        m_sphereMesh.push_back(floretSphere);
    }

    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progFlat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());

//#define NOPE
#ifndef NOPE
    //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
//    glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(1,2,-2)) * glm::scale(glm::mat4(1.0f), glm::vec3(3,3,3));
//    glm::mat4 model = glm::translate(glm::mat4(2.0f), glm::vec3(0,0,0))
//            * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f) , glm::vec3(3,3,3))
//                          * glm::scale(glm::mat4(1.0f), glm::vec3(2,2,2));

//    glm::mat4 model = glm::translate(glm::mat4(2.0f), glm::vec3(0,0,0))
//                      * glm::scale(glm::mat4(1.0f), glm::vec3(2,2,2));

    //Send the geometry's transformation matrix to the shader

    glEnable(GL_DEPTH_TEST);

//    m_progLambert.elMatrix(model);
//    m_progFlat.setModelMatrix(model);
    m_progFlat.setModelMatrix(glm::mat4(1.0f));

//    m_progLambert.draw(geom_mesh);

    glDisable(GL_DEPTH_TEST);

    for (Sphere* i : m_sphereMesh) {
        m_progFlat.draw(*i);
    }

    for (FaceMesh* i : m_selectedFaceMesh) {
        m_progFlat.draw(*i);
    }

    for (LineMesh* i : m_selectedLineMesh) {
        m_progFlat.draw(*i);
    }
    for (VertexMesh* i : m_selectedVertexMesh) {
        m_progFlat.draw(*i);
    }


    //Draw the example sphere using our lambert shader
//    m_progLambert.draw(m_geomSphere);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
//    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
//    m_progLambert.setModelMatrix(model);
//    m_progLambert.draw(m_geomCylinder);
#endif
}


void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        m_glCamera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    } else if (e->key() == Qt::Key_N) {
        on_action_N_nexthalfedge();
    } else if (e->key() == Qt::Key_M) {
        on_action_M_SYM();
    } else if (e->key() == Qt::Key_F){
        on_action_F_faceOfHalfEdge();
    } else if (e->key() == Qt::Key_V) {
        on_action_V_vertexOfHalfEdge();
    } else if ((e->key() == Qt::Key_H) && (e->modifiers() == Qt::ShiftModifier)) {
        on_action_H_Shift_halfedgeOfFace();
    } else if (e->key() == Qt::Key_H) {
        on_action_H_halfedgeOfVertex();
    }
    m_glCamera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::emitCubeCreateSignal() {
    emit signal_createCube();
}

void MyGL::highlightVertex(Vertex *item) {
    for (VertexMesh* v : m_selectedVertexMesh) {
        v->destroy();
    }
    m_selectedVertexMesh.clear();
    VertexMesh* selected = new VertexMesh(this);
    selected->m_vertices.push_back(item);
    selected->create();
    m_selectedVertexMesh.push_back(selected);
    update();
}

void MyGL::highlightLine(HalfEdge *line) {
    for (LineMesh* v : m_selectedLineMesh) {
        v->destroy();
    }
    m_selectedLineMesh.clear();
    LineMesh* selected = new LineMesh(this);
    selected->m_halfedges_mesh.push_back(line);
    selected->create();
    m_selectedLineMesh.push_back(selected);
    update();
}

void MyGL::highlightFace(Face* face) {
    for (FaceMesh* v : m_selectedFaceMesh) {
        v->destroy();
    }
    m_selectedFaceMesh.clear();
    FaceMesh* selected = new FaceMesh(this);
    selected->m_faces_mesh.push_back(face);
    selected->create();
    m_selectedFaceMesh.push_back(selected);
    update();
}

void MyGL::on_action_N_nexthalfedge() {
    if (nullptr == selected_HalfEdge) {
        return ;
    }
    else {
        selected_HalfEdge = selected_HalfEdge->next_he;
        highlightLine(selected_HalfEdge);
        emit signal_setCurrentHalfEdgeListWidgetItem(selected_HalfEdge);
    }
}

void MyGL::on_action_M_SYM() {
    if (nullptr == selected_HalfEdge) {
        return ;
    }
    if (nullptr == selected_HalfEdge->sym_he) {
        return ;
    }    else {
        selected_HalfEdge = selected_HalfEdge->sym_he;
        highlightLine(selected_HalfEdge);
        emit signal_setCurrentHalfEdgeListWidgetItem(selected_HalfEdge);
    }
}

void MyGL::on_action_F_faceOfHalfEdge() {
    if (nullptr == selected_HalfEdge) {
        return ;
    }
    else {
        selected_Face = selected_HalfEdge->face;
        MyGL::highlightFace(selected_Face);
        emit signal_setCurrent_Face_ListWidgetItem(selected_Face);
    }
}

void MyGL::on_action_V_vertexOfHalfEdge() {
    if (nullptr == selected_HalfEdge) {
        return ;
    }
    else {
        selected_Vertex = selected_HalfEdge->vertex;
        MyGL::highlightVertex(selected_Vertex);
        emit signal_setCurrent_Vertex_ListWidgetItem(selected_Vertex);
    }
}

void MyGL::on_action_H_halfedgeOfVertex() {
    if (nullptr == selected_Vertex) {
        return ;
    }
    else {
        selected_HalfEdge = selected_Vertex->halfedge;
        MyGL::highlightLine(selected_HalfEdge);
        emit signal_setCurrentHalfEdgeListWidgetItem(selected_HalfEdge);
    }
}

void MyGL::on_action_H_Shift_halfedgeOfFace() {
    if (nullptr == selected_Face) {
        return ;
    }
    else {
        selected_HalfEdge = selected_Face->start_edge;
        MyGL::highlightLine(selected_HalfEdge);
        emit signal_setCurrentHalfEdgeListWidgetItem(selected_HalfEdge);
    }
}

void MyGL::addVertex() {
    if (nullptr == selected_HalfEdge) {
        return ;
    }
    else {
        //step 1 : create new vertex v3
        HalfEdge* he1 = selected_HalfEdge;
        HalfEdge* he2 = selected_HalfEdge->sym_he;
        Vertex* v1 = he1->vertex;
        Vertex* v2 = he2->vertex;
        int id_vertex = geom_mesh.m_Vertices.size();
        Vertex* v3 = new Vertex(id_vertex++);
        v3->position = glm::vec3((v1->position.x + v2->position.x)/2,
                                 (v1->position.y + v2->position.y)/2,
                                 (v1->position.z + v2->position.z)/2);

        //step 2: create two new halfedge he1b, he2b
        int id_he = geom_mesh.m_HalfEdges.size();
        HalfEdge* he1b = new HalfEdge(id_he++);
        HalfEdge* he2b = new HalfEdge(id_he++);
        he1b->vertex = v1;
        he2b->vertex = v2;
        he1b->face = he1->face;
        he2b->face = he2->face;

        //step 3:
        he1b->next_he = he1->next_he;
        he2b->next_he = he2->next_he;
        he1->next_he = he1b;
        he2->next_he = he2b;
        he1->vertex = v3;
        he2->vertex = v3;
        he1->sym_he = he2b;
        he2b->sym_he = he1;
        he2->sym_he = he1b;
        he1b->sym_he = he2;

        geom_mesh.m_HalfEdges.push_back(he1b);
        geom_mesh.m_HalfEdges.push_back(he2b);
        geom_mesh.m_Vertices.push_back(v3);
    }
}

void MyGL::triangulate() {
    if (nullptr == selected_Face) {
        return ;
    }
    else {
        // step 1: create two new half-edges he_a and he_b
        Face* face1 = selected_Face;
        int id_he = geom_mesh.m_HalfEdges.size();
        HalfEdge* halfEdge_a = new HalfEdge(id_he++);
        HalfEdge* halfEdge_b = new HalfEdge(id_he++);

        HalfEdge* halfEdge_0 = face1->start_edge;
        halfEdge_a->vertex = halfEdge_0->vertex;
        halfEdge_b->vertex = halfEdge_0->next_he->next_he->vertex;
        halfEdge_a->sym_he = halfEdge_b;
        halfEdge_b->sym_he = halfEdge_a;

        //step 2: create a second face Face2
        int id_face = geom_mesh.m_Faces.size();
        Face* face2 = new Face(id_face);
        face2->color = face1->color;

        halfEdge_a->face = face2;
        halfEdge_0->next_he->face = face2;
        halfEdge_0->next_he->next_he->face = face2;
        halfEdge_b->face = face1;
        face2->start_edge = halfEdge_a;

        //step 3: fix up the next points for hes
        halfEdge_b->next_he = halfEdge_0->next_he->next_he->next_he;
        halfEdge_0->next_he->next_he->next_he = halfEdge_a;
        halfEdge_a->next_he = halfEdge_0->next_he;
        halfEdge_0->next_he = halfEdge_b;

        geom_mesh.m_HalfEdges.push_back(halfEdge_a);
        geom_mesh.m_HalfEdges.push_back(halfEdge_b);
        geom_mesh.m_Faces.push_back(face2);

    }
}

void MyGL::setFaceColor(QColor color) {
    if (nullptr == selected_Face) {
        return ;
    }
    else {
        int *r = new int(0);
        int *g = new int(0);
        int *b = new int(0);
        color.getRgb(r, g, b);

        float normR, normG, normB;

        normR = (float) (*r) / 255;
        normG = (float) (*g) / 255;
        normB = (float) (*b) / 255;
        glm::vec3 rgb(normR, normG, normB);
        selected_Face->color = rgb;

    }
}

void MyGL::subdivision() {
    std::map<Vertex*, Face*> centroid_map;
    int id_vert = geom_mesh.m_Vertices.size();
    int id_he = geom_mesh.m_HalfEdges.size();
    std::vector<bool> origin_vertex(geom_mesh.m_Vertices.size(), true);
    std::vector<Vertex*> m_old_verts = geom_mesh.m_Vertices;

     //step 1: add centroid
    for (Face* face : geom_mesh.m_Faces){
        HalfEdge* startEdge = face->start_edge;
        HalfEdge* he;
        float num_he = 0;
        glm::vec3 position_sum(0,0,0);
        for (he = startEdge; he->next_he != startEdge; he = he->next_he) {
            num_he++;
            position_sum = he->vertex->position + position_sum;
        }
        num_he++;
        position_sum = he->vertex->position + position_sum;

        Vertex* centroid = new Vertex(id_vert++);
        centroid->position = position_sum / num_he;
        geom_mesh.m_Vertices.push_back(centroid);
        centroid_map.insert(std::make_pair(centroid, face));
    }

    //step 2: add mid point
    std::vector<bool> splited (geom_mesh.m_HalfEdges.size(), false);
    std::vector<HalfEdge*> m_old_hes = geom_mesh.m_HalfEdges;

    for (HalfEdge* he : m_old_hes) {
        if (splited[he->id_he]) continue;

        splited[he->id_he] = true;
        splited[he->sym_he->id_he] = true;

        // //////////////////////////
        HalfEdge* he1 = he;
        HalfEdge* he2 = he->sym_he;
        Vertex* v1 = he1->vertex;
        Vertex* v2 = he2->vertex;
        Vertex* v3 = new Vertex(id_vert++);
        v3->position = (he->vertex->position + he->sym_he->vertex->position
                        + search_map_Centroid(centroid_map, he->face)->position
                        + search_map_Centroid(centroid_map, he->sym_he->face)->position) / (float) 4.0;

        //step 2: create two new halfedge he1b, he2b
        HalfEdge* he1b = new HalfEdge(id_he++);
        HalfEdge* he2b = new HalfEdge(id_he++);
        he1b->vertex = v1;
        he2b->vertex = v2;
        he1b->face = he1->face;
        he2b->face = he2->face;

        //step 3:
        he1b->next_he = he1->next_he;
        he2b->next_he = he2->next_he;
        he1->next_he = he1b;
        he2->next_he = he2b;
        he1->vertex = v3;
        he2->vertex = v3;
        he1->sym_he = he2b;
        he2b->sym_he = he1;
        he2->sym_he = he1b;
        he1b->sym_he = he2;

        v1->halfedge = he1b;
        v2->halfedge = he2b;
        v3->halfedge = he1;

        geom_mesh.m_HalfEdges.push_back(he1b);
        geom_mesh.m_HalfEdges.push_back(he2b);
        geom_mesh.m_Vertices.push_back(v3);
    }

    // step 3: smooth the original vertices
    for(Vertex* v : m_old_verts) {
        if (origin_vertex[v->id_vertex] == false) continue;

        HalfEdge* startEdge = v->halfedge;
        HalfEdge* he = startEdge;
        glm::vec3 midPoint_pos_sum = glm::vec3(0,0,0);
        glm::vec3 centroid_pos_sum = glm::vec3(0,0,0);
        float num_adj_vert = 0;
        while(he->next_he->sym_he != startEdge) {
            he = he->next_he;
            midPoint_pos_sum = midPoint_pos_sum + he->vertex->position;
            num_adj_vert++;
            centroid_pos_sum = centroid_pos_sum + search_map_Centroid(centroid_map, he->face)->position;
            he = he->sym_he;
        }
        he = he->next_he;
        midPoint_pos_sum = midPoint_pos_sum + he->vertex->position;
        num_adj_vert++;
        centroid_pos_sum = centroid_pos_sum + search_map_Centroid(centroid_map, he->face)->position;

        v->position = (num_adj_vert - 2) * v->position / num_adj_vert +
                    midPoint_pos_sum / (num_adj_vert * num_adj_vert) +
                    centroid_pos_sum / (num_adj_vert * num_adj_vert);
    }

//    step 4: quadrangle
    int id_face = geom_mesh.m_Faces.size();
    std::vector<Face*> m_old_Faces = geom_mesh.m_Faces;
    for (Face* face : m_old_Faces) {

        HalfEdge* start_he = face->start_edge;
        HalfEdge* tmp_vert_he = start_he;
        HalfEdge* he = start_he->next_he;
        HalfEdge* tmp_he = he->next_he->next_he;
        HalfEdge* old_he2 = nullptr;
        while (he->next_he != start_he) {
            Face* face_new = new Face(id_face++);
            face_new->start_edge = he->next_he;
            face_new->color = face->color;
            geom_mesh.m_Faces.push_back(face_new);

            HalfEdge* he1 = new HalfEdge(id_he++);
            HalfEdge* he2 = new HalfEdge(id_he++);
            he->next_he->next_he = he1;
            he1->next_he = he2;
            he2->next_he = he;
            he1->face = face_new;
            he2->face = face_new;
            he->face = face_new;
            he->next_he->face = face_new;
            he1->vertex = search_map_Centroid(centroid_map, face);
            he2->vertex = tmp_vert_he->vertex;
            geom_mesh.m_HalfEdges.push_back(he1);
            geom_mesh.m_HalfEdges.push_back(he2);
            if (he == start_he->next_he) {
                old_he2 = he->next_he->next_he->next_he;
                tmp_vert_he = he->next_he;
                he = tmp_he;
                tmp_he = tmp_he->next_he->next_he;
                continue;
            }
            he->next_he->next_he->next_he->sym_he = tmp_vert_he->next_he;
            tmp_vert_he->next_he->sym_he = he->next_he->next_he->next_he;
            tmp_vert_he = he->next_he;
            he = tmp_he;
            tmp_he = tmp_he->next_he->next_he;
        }
        HalfEdge* he1 = new HalfEdge(id_he++);
        HalfEdge* he2 = new HalfEdge(id_he++);
        he->next_he->next_he = he1;
        he1->next_he = he2;
        he2->next_he = he;
        he1->face = face;
        he2->face = face;
        he->face = face;
        he->next_he->face = face;
        he1->vertex = search_map_Centroid(centroid_map, face);
        he2->vertex = tmp_vert_he->vertex;
        geom_mesh.m_HalfEdges.push_back(he1);
        geom_mesh.m_HalfEdges.push_back(he2);
        he->next_he->next_he->next_he->sym_he = tmp_vert_he->next_he;
        tmp_vert_he->next_he->sym_he = he->next_he->next_he->next_he;
        he->next_he->next_he->sym_he = old_he2;
        old_he2->sym_he = he->next_he->next_he;

        search_map_Centroid(centroid_map, he->face)->halfedge = he1;
    }
    m_old_Faces.clear();
    m_old_hes.clear();
    m_old_verts.clear();
    centroid_map.clear();

    geom_mesh.destroy();
    geom_mesh.create();
    update();
}

Vertex* MyGL::search_map_Centroid(std::map<Vertex *, Face *> &mapOfWords, Face *face) {
    std::map<Vertex*, Face*>::iterator it = mapOfWords.begin();
    while(it != mapOfWords.end())
    {
        if(it->second == face){
            return it->first;
        }
        it++;
    }
    Vertex* false_vert = new Vertex(-1);
    return false_vert;
}

void MyGL::extrude() {
    HalfEdge* start_he = selected_Face->start_edge;
    HalfEdge* he1 = start_he;
    float scale = 0.5;
    Vertex* old_v4;
    Vertex* old_v3;
    Vertex* old_v1;
    HalfEdge* old_he4;
    HalfEdge* old_he3;
    glm::vec3 nor = glm::normalize( glm::cross((start_he->next_he->vertex->position - start_he->vertex->position),
                               (start_he->next_he->next_he->vertex->position - start_he->next_he->vertex->position)));
    // first face
    {
        //step 1: add two more vertex
        int id_vertex = geom_mesh.m_Vertices.size();
        HalfEdge* he2 = he1->sym_he;
        Vertex* v1 = he1->vertex;
        Vertex* v2 = he2->vertex;
        Vertex* v3 = new Vertex(id_vertex++);
        Vertex* v4 = new Vertex(id_vertex++);
        v3->position = v1->position + nor * scale;
        v4->position = v2->position + nor * scale;
        geom_mesh.m_Vertices.push_back(v3);
        geom_mesh.m_Vertices.push_back(v4);

        // step 2: adjust he1->vertex = v3; he1->previous->vertex = v4
        HalfEdge* previous_he = he1;
        while (previous_he->next_he != he1) {
            previous_he = previous_he->next_he;
        }
        previous_he->vertex = v4;
        he1->vertex = v3;

        //step 3: create two new halfedge he1b, he2b
        int id_he = geom_mesh.m_HalfEdges.size();
        HalfEdge* he1b = new HalfEdge(id_he++);
        HalfEdge* he2b = new HalfEdge(id_he++);
        he1->sym_he = he1b;
        he2->sym_he = he2b;
        he1b->sym_he = he1;
        he2b->sym_he = he2;
        he1b->vertex = v4;
        he2b->vertex = v1;
        geom_mesh.m_HalfEdges.push_back(he1b);
        geom_mesh.m_HalfEdges.push_back(he2b);

        //step 4: create new face F and another two halfedge HE3 and HE4
        int id_face = geom_mesh.m_Faces.size();
        Face* f = new Face(id_face++);
        f->color = selected_Face->color;
        f->start_edge = he1b;
        HalfEdge* he3 = new HalfEdge(id_he++);
        HalfEdge* he4 = new HalfEdge(id_he++);
        he1b->face = f;
        he2b->face = f;
        he3->face = f;
        he4->face = f;
        he3->vertex = v3;
        he4->vertex = v2;
        he1b->next_he = he4;
        he4->next_he = he2b;
        he2b->next_he = he3;
        he3->next_he = he1b;
        geom_mesh.m_Faces.push_back(f);
        geom_mesh.m_HalfEdges.push_back(he3);
        geom_mesh.m_HalfEdges.push_back(he4);

        //step 5: set sym of he3 and he4
        he3->sym_he = nullptr;
        he3->sym_he = nullptr;
        old_v4 = v3;
        old_v3 = v4;
        old_v1 = v2;
        he1 = he1->next_he;
        old_he4 = he3;
        old_he3 = he4;
    }

    // mid face loop
    while (he1->next_he != start_he) {
        //step 1: add two more vertex
        int id_vertex = geom_mesh.m_Vertices.size();
        HalfEdge* he2 = he1->sym_he;
        Vertex* v1 = he1->vertex;
        Vertex* v2 = he2->vertex;
        Vertex* v3 = new Vertex(id_vertex++);
        Vertex* v4 = old_v4;
        v3->position = v1->position + nor * scale;
        geom_mesh.m_Vertices.push_back(v3);

        // step 2: adjust he1->vertex = v3; he1->previous->vertex = v4
        HalfEdge* previous_he = he1;
        while (previous_he->next_he != he1) {
            previous_he = previous_he->next_he;
        }
        previous_he->vertex = v4;
        he1->vertex = v3;

        //step 3: create two new halfedge he1b, he2b
        int id_he = geom_mesh.m_HalfEdges.size();
        HalfEdge* he1b = new HalfEdge(id_he++);
        HalfEdge* he2b = new HalfEdge(id_he++);
        he1->sym_he = he1b;
        he2->sym_he = he2b;
        he1b->sym_he = he1;
        he2b->sym_he = he2;
        he1b->vertex = v4;
        he2b->vertex = v1;
        geom_mesh.m_HalfEdges.push_back(he1b);
        geom_mesh.m_HalfEdges.push_back(he2b);

        //step 4: create new face F and another two halfedge HE3 and HE4
        int id_face = geom_mesh.m_Faces.size();
        Face* f = new Face(id_face++);
        f->color = selected_Face->color;
        f->start_edge = he1b;
        HalfEdge* he3 = new HalfEdge(id_he++);
        HalfEdge* he4 = new HalfEdge(id_he++);
        he1b->face = f;
        he2b->face = f;
        he3->face = f;
        he4->face = f;
        he3->vertex = v3;
        he4->vertex = v2;
        he1b->next_he = he4;
        he4->next_he = he2b;
        he2b->next_he = he3;
        he3->next_he = he1b;
        geom_mesh.m_Faces.push_back(f);
        geom_mesh.m_HalfEdges.push_back(he3);
        geom_mesh.m_HalfEdges.push_back(he4);

        //step 5: set sym of he3 and he4
//        he3->sym_he = nullptr;
//        he3->sym_he = nullptr;

        he4->sym_he = old_he4;
        old_he4->sym_he = he4;
        old_he4 = he3;


        old_v4 = v3;
        he1 = he1->next_he;
    }

    // last face
    {
        //step 1: add two more vertex
        HalfEdge* he2 = he1->sym_he;
//        Vertex* v1 = he1->vertex;
        Vertex* v1 = old_v1;

        Vertex* v2 = he2->vertex;
        Vertex* v3 = old_v3;
        Vertex* v4 = old_v4;

        // step 2: adjust he1->vertex = v3; he1->previous->vertex = v4
        HalfEdge* previous_he = he1;
        while (previous_he->next_he != he1) {
            previous_he = previous_he->next_he;
        }
        previous_he->vertex = v4;
        he1->vertex = v3;

        //step 3: create two new halfedge he1b, he2b
        int id_he = geom_mesh.m_HalfEdges.size();
        HalfEdge* he1b = new HalfEdge(id_he++);
        HalfEdge* he2b = new HalfEdge(id_he++);
        he1->sym_he = he1b;
        he2->sym_he = he2b;
        he1b->sym_he = he1;
        he2b->sym_he = he2;
        he1b->vertex = v4;
        he2b->vertex = v1;
        geom_mesh.m_HalfEdges.push_back(he1b);
        geom_mesh.m_HalfEdges.push_back(he2b);

        //step 4: create new face F and another two halfedge HE3 and HE4
        int id_face = geom_mesh.m_Faces.size();
        Face* f = new Face(id_face++);
        f->color = selected_Face->color;
        f->start_edge = he1b;
        HalfEdge* he3 = new HalfEdge(id_he++);
        HalfEdge* he4 = new HalfEdge(id_he++);
        he1b->face = f;
        he2b->face = f;
        he3->face = f;
        he4->face = f;
        he3->vertex = v3;
        he4->vertex = v2;
        he1b->next_he = he4;
        he4->next_he = he2b;
        he2b->next_he = he3;
        he3->next_he = he1b;
        geom_mesh.m_Faces.push_back(f);
        geom_mesh.m_HalfEdges.push_back(he3);
        geom_mesh.m_HalfEdges.push_back(he4);

        //step 5: set sym of he3 and he4
        he3->sym_he = nullptr;
        he3->sym_he = nullptr;

        old_v4 = v3;
        he1 = he1->next_he;
        he4->sym_he = old_he4;
        old_he4->sym_he = he4;
//        old_he4 = he3;

        he3->sym_he = old_he3;
        old_he3->sym_he = he3;

    }

    geom_mesh.destroy();
    geom_mesh.create();
    emitCubeCreateSignal();
    update();

}

int MyGL::search_map_vertex(std::map<int, Vertex*> & maps, Vertex* vertex) {
    std::map<int, Vertex*>::iterator it = maps.begin();
    while(it != maps.end())
    {
        if(it->second == vertex){
            return it->first;
        }
        it++;
    }
    return -1;
}

void MyGL::loadOBJ() {
    QString filename = QFileDialog::getOpenFileName(this,
           tr("Open Object File"), "",
           tr("Object (*.obj)"));

    if (filename.isEmpty()){
        return;
    }
    loadNewObj = true;
    std::ifstream infile(filename.toStdString());

    std::string identifier;
    int i = 0,j = 0, k = 0; //index
    float x,y,z; //vertex pos/normal x,y,z value
    int pos_size;

    std::vector<Vertex*> inputVertex;
    std::vector<Face*> inputFace;
    std::vector<HalfEdge*> inputHalfEdge;
    std::vector<glm::vec4> normal;

    geom_mesh.mesh_vert_col.clear();
    geom_mesh.mesh_vert_nor.clear();
    geom_mesh.mesh_vert_pos.clear();
    geom_mesh.mesh_idx.clear();
    geom_mesh.m_Vertices.clear();
    geom_mesh.m_Faces.clear();
    geom_mesh.m_HalfEdges.clear();

    // give the index of halfedge id, get the previous vertex
    std::map<int, Vertex*> map_vertex_previous;

    //give two vertices, first vertex is previous vertex of he, second is vertex of he, find he
    std::map<std::pair<Vertex*, Vertex*>, HalfEdge*> map_vertexpair_to_he;
    std::map<HalfEdge*, std::pair<Vertex*, Vertex*>> map_he_to_vertexpair;

    for (std::string line; std::getline(infile, line);)
    {
        std::stringstream line_seg;
        line_seg << line;
        line_seg >> identifier;

        if (identifier == "v")
        {
            line_seg >> x >> y >> z;
            Vertex* vertex = new Vertex(i);
            inputVertex.push_back(vertex);

            inputVertex[i]->position = glm::vec3(x,y,z);
            i += 1;
        }
        else if (identifier == "vn")
        {
            line_seg >> x >> y >> z;
            normal.push_back(glm::vec4(x,y,z,1));
            j += 1;
        }
        else if (identifier == "f")
        {
            std::vector<int> pos;
            std::vector<std::string> seg;
            std::string seg_each;
            unsigned int a;

            while (line_seg >> seg_each)
            {
                seg.push_back(seg_each);
            }

            for (a = 0; a < seg.size(); a++)
            {
                int beginIdx = seg[a].find('/');
                std::string tmp = seg[a].substr(0, beginIdx);

                int beginIdx2 = seg[a].rfind('/');
                std::string tmp2 = seg[a].substr(beginIdx2+1, seg[a].length()-beginIdx-1);
                pos.push_back(std::stoi(tmp.c_str()));
                geom_mesh.mesh_vert_nor.push_back(normal[std::stoi(tmp2)-1]);
            }

            Face* face = new Face(k); // set up id
            inputFace.push_back(face);
            inputFace[k]->color = glm::vec3((double)rand()/(RAND_MAX),(double)rand()/(RAND_MAX),(double)rand()/(RAND_MAX)); //random number
            pos_size = pos.size();

            for (a = 0; a < pos.size(); a++)
            {
                HalfEdge* halfedge = new HalfEdge(k*pos.size()+a); // set up id
                inputHalfEdge.push_back(halfedge);
            }

            for (a = 0; a < pos.size(); a++)
            {
                inputHalfEdge[k*pos.size()+a]->face = inputFace[k];
                inputHalfEdge[k*pos.size()+a]->vertex = inputVertex[pos[a]-1];
                inputVertex[pos[a]-1]->halfedge = inputHalfEdge[k*pos.size()+a];
            }
            inputFace[k]->start_edge = inputHalfEdge[k*pos.size()+0];

            for (a = 0; a < pos.size()-1; a++)
            {
                inputHalfEdge[k*pos.size()+a]->next_he = inputHalfEdge[k*pos.size()+a+1];
            }
            inputHalfEdge[k*pos.size()+pos.size()-1]->next_he = inputHalfEdge[k*pos.size()+0];

            //
            map_vertexpair_to_he.insert(std::make_pair(std::pair<Vertex*, Vertex*> (inputVertex[pos[pos.size()-1]-1], inputVertex[pos[0]-1]),
                                                        inputHalfEdge[k*pos.size()]));
            map_he_to_vertexpair.insert(std::make_pair(inputHalfEdge[k*pos.size()],
                                        std::pair<Vertex*, Vertex*> (inputVertex[pos[pos.size()-1]-1], inputVertex[pos[0]-1])));

            for (a = 1; a < pos.size(); a++) {
                map_vertexpair_to_he.insert(std::make_pair(std::pair<Vertex*, Vertex*> (inputVertex[pos[a-1]-1], inputVertex[pos[a]-1]),
                                                inputHalfEdge[k*pos.size()+a]));
                map_he_to_vertexpair.insert(std::make_pair(inputHalfEdge[k*pos.size()+a],
                                            std::pair<Vertex*, Vertex*> (inputVertex[pos[a-1]-1], inputVertex[pos[a]-1])));
            }

            k += 1;
        }
    }
    int size_vertex = i;
    int size_halfedge = k * pos_size;
    int size_face = k;

    for (HalfEdge* he : inputHalfEdge) {
        he->sym_he =  map_vertexpair_to_he[std::pair <Vertex*, Vertex*> (map_he_to_vertexpair[he].second, map_he_to_vertexpair[he].first)];
        map_vertexpair_to_he[std::pair <Vertex*, Vertex*> (map_he_to_vertexpair[he].second, map_he_to_vertexpair[he].first)]->sym_he = he;
    }


    for (i = 0; i < size_vertex; i++)
        geom_mesh.m_Vertices.push_back(inputVertex[i]);
    for (j = 0; j < size_halfedge; j++)
        geom_mesh.m_HalfEdges.push_back(inputHalfEdge[j]);
    for (k = 0; k < size_face; k++)
        geom_mesh.m_Faces.push_back(inputFace[k]);

    geom_mesh.create();
    update();
}

void MyGL::noHightlightVertex() {
    for (VertexMesh* v : m_selectedVertexMesh) {
        v->destroy();
    }
    m_selectedVertexMesh.clear();
    selected_Vertex = nullptr;
    update();
}

void MyGL::noHightlightLine(){
    for (LineMesh* v : m_selectedLineMesh) {
        v->destroy();
    }
    m_selectedLineMesh.clear();
    selected_HalfEdge = nullptr;
    update();
}

void MyGL::noHightlightFace() {
    for (FaceMesh* v : m_selectedFaceMesh) {
        v->destroy();
    }
    m_selectedFaceMesh.clear();
    selected_Face = nullptr;
    update();
}

bool MyGL::loadJson() {
    filename = QFileDialog::getOpenFileName(this,
           tr("Open Json"), "",
           tr("Json (*.json)"));
    if (filename.isEmpty()) {
        return false;
    }

    for (Joint* i : m_jointMesh) {
        i->destroy();
    }
    m_jointMesh.clear();
    joint = getRootJoint(filename);

    std::vector<glm::mat4> bindMatrix;
    bindMatrix.reserve(m_jointMesh.size());
    for (Joint* joint : m_jointMesh) {
        bindMatrix.push_back(glm::inverse(joint->getOverallTransformation()));
    }
    m_progSkeleton.setBindMatrix(bindMatrix);

    emit signal_loadJsonRoot(joint);
    update();
    return true;
}

Joint* MyGL::getRootJoint(QString filename) {
    QString val;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject object = document.object();
    QJsonObject jointJSON = object["root"].toObject();
    curJointID = 0;
    Joint* root = initializeJson(jointJSON, nullptr);

    return root;
}

Joint* MyGL::initializeJson(QJsonObject jsonObject, Joint *parent) {
    QString jointName = jsonObject["name"].toString();

    QJsonArray jointPosition = jsonObject["pos"].toArray();
    QJsonArray jointRotation = jsonObject["rot"].toArray();
    QJsonArray jointChildren = jsonObject["children"].toArray();

    glm::vec3 position = glm::vec3(jointPosition[0].toDouble(), jointPosition[1].toDouble(), jointPosition[2].toDouble());
    double angle = jointRotation[0].toDouble();
    glm::vec3 axis = glm::vec3(jointRotation[1].toDouble(), jointRotation[2].toDouble(), jointRotation[3].toDouble());
    glm::quat rotateQuat = glm::quat(glm::angleAxis(float(angle), axis));

    Joint* currentJoint = new Joint(this);
    currentJoint->name = jointName;
    currentJoint->position = position;
    currentJoint->rotation = rotateQuat;
    currentJoint->id = curJointID++;
    currentJoint->setText(0, QString(QString::number(currentJoint->id) + " - " + jointName));
    if (parent != nullptr) {
        currentJoint->parent = parent;
        parent->children.push_back(currentJoint);
        parent->addChild(currentJoint);
    }
    for (int i = 0; i < jointChildren.size(); i++) {
        Joint* child = initializeJson(jointChildren[i].toObject(), currentJoint);
    }
    currentJoint->create();
    m_jointMesh.push_back(currentJoint);
    return currentJoint;
}

void MyGL::highlightJoint(Joint* joint) {

    joint->destroy();
    joint->create();
    update();
}

void MyGL::updateJointTransform(ShaderProgram sp) {
    std::vector<glm::mat4> mats_vec;
    mats_vec.reserve(m_jointMesh.size());
    for (Joint* joint : m_jointMesh) {
        mats_vec.push_back(joint->getOverallTransformation());
    }
    sp.setJointTransformation(mats_vec);
}

void MyGL::updateBranch(Branch *branch) {
    for (LineMesh* v : m_selectedLineMesh) {
        v->destroy();
    }
    m_selectedLineMesh.clear();
    branch->resetParticle();
//    branch->createBasicBranch(t);
    branch->createBranchTimeNewMethod();
    for (Path p : branch->getPaths()) {
        LineMesh* pathLine = new LineMesh(this);
        pathLine->createPath(p);
        m_selectedLineMesh.push_back(pathLine);
    }
}

void MyGL::updateNextTimeBranch() {
    for (LineMesh* v : m_selectedLineMesh) {
        v->destroy();
    }
//    branch.updateBranch();
    branch.updateBranchNewMethod();
    for (Path p : branch.getPaths()) {
        LineMesh* pathLine = new LineMesh(this);
        pathLine->createPath(p);
        m_selectedLineMesh.push_back(pathLine);
    }
}
