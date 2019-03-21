#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <QKeyEvent>
#include <QColor>
#include <QColorDialog>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    connect(ui->mygl, SIGNAL(signal_createCube()),
            this, SLOT(slot_cubeInit()));


    connect(ui->mygl, SIGNAL(signal_setCurrentHalfEdgeListWidgetItem(QListWidgetItem*)),
            this, SLOT(slot_setHalfEdgeQListWidgetItem(QListWidgetItem*)));

    connect(ui->mygl, SIGNAL(signal_setCurrent_Face_ListWidgetItem(QListWidgetItem*)),
            this, SLOT(slot_set_Face_QListWidgetItem(QListWidgetItem*)));

    connect(ui->mygl, SIGNAL(signal_setCurrent_Vertex_ListWidgetItem(QListWidgetItem*)),
            this, SLOT(slot_set_Vertex_QListWidgetItem(QListWidgetItem*)));


    connect(ui->mygl, SIGNAL(signal_loadJsonRoot(Joint*)),
            this, SLOT(slot_addTopLevelItem(Joint*)));

    connect(ui->timeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(slot_setTime(int)));

    connect(ui->updatePushButton, SIGNAL(clicked(bool)),
            this, SLOT(slot_updateBranch()));

    connect(ui->waDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(slot_setWa(double)));

    connect(ui->wbDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(slot_setWb(double)));

    connect(ui->whDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(slot_setWh(double)));

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::slot_cubeInit() {
//    for (Vertex* i : ui->mygl->geom_mesh.m_Vertices) {
//        ui->vertexListWidget->addItem(i);
//    }

//    for (Face* i : ui->mygl->geom_mesh.m_Faces) {
//        ui->faceListWidget->addItem(i);
//    }

//    for (HalfEdge* i : ui->mygl->geom_mesh.m_HalfEdges) {
//         ui->halfedgeListWidget->addItem(i);
//    }
}

void MainWindow::slot_getVertexQListWidgetItem(QListWidgetItem* i) {
    Vertex* v = dynamic_cast<Vertex*> (i);
    currentVertex = v;
    ui->mygl->selected_Vertex = v;
    ui->mygl->highlightVertex(v);
//    ui->x_translateDoubleSpinBox->setEnabled(true);
//    ui->y_translateDoubleSpinBox->setEnabled(true);
//    ui->z_translateDoubleSpinBox->setEnabled(true);
//    ui->mygl->setFocus();
}

void MainWindow::slot_getHalfEdgeQListWidgetItem(QListWidgetItem *item ) {
    HalfEdge* he = dynamic_cast<HalfEdge*> (item);
    ui->mygl->selected_HalfEdge = he;
    ui->mygl->highlightLine(he);
//    ui->addVertexPushButton->setEnabled(true);
    ui->mygl->setFocus();
}

void MainWindow::slot_getFaceQListWidgetItem(QListWidgetItem* item) {
    Face* face = dynamic_cast<Face*> (item);
    ui->mygl->selected_Face = face;
    ui->mygl->highlightFace(face);
//    ui->colorPushButton->setEnabled(true);
//    ui->extrudePushButton->setEnabled(true);
//    ui->triangulatePushButton->setEnabled(true);
    ui->mygl->setFocus();
}

void MainWindow::slot_getVertexPosition(QListWidgetItem * item) {
    Vertex* vertex = dynamic_cast<Vertex*> (item);
    x_position = vertex->position.x;
    y_position = vertex->position.y;
    z_position = vertex->position.z;
    emit signal_send_x_position(x_position);
    emit signal_send_y_position(y_position);
    emit signal_send_z_position(z_position);
    ui->mygl->setFocus();
}

void MainWindow::slot_setVertex_x_position(double i) {
    if (nullptr == currentVertex) {
        ui->mygl->setFocus();
        return;
    }
    else {
      currentVertex->position.x = i;
    }
    ui->mygl->geom_mesh.destroy();
    ui->mygl->geom_mesh.create();

    ui->mygl->highlightVertex(currentVertex);

    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_setVertex_y_position(double i) {
    if (nullptr == currentVertex) {
        ui->mygl->setFocus();
        return;
    }
    else {
      currentVertex->position.y = i;
    }
    ui->mygl->geom_mesh.destroy();
    ui->mygl->geom_mesh.create();

    ui->mygl->highlightVertex(currentVertex);

    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_setVertex_z_position(double i) {
    if (nullptr == currentVertex) {
        ui->mygl->setFocus();
        return;
    }
    else {
      currentVertex->position.z = i;
    }
    ui->mygl->geom_mesh.destroy();
    ui->mygl->geom_mesh.create();

    ui->mygl->highlightVertex(currentVertex);

    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_setHalfEdgeQListWidgetItem(QListWidgetItem *i) {
//    ui->addVertexPushButton->setEnabled(true);
//    ui->halfedgeListWidget->setCurrentItem(i);
}

void MainWindow::slot_set_Face_QListWidgetItem(QListWidgetItem *i) {
//    ui->colorPushButton->setEnabled(true);
//    ui->extrudePushButton->setEnabled(true);
//    ui->triangulatePushButton->setEnabled(true);

//    ui->faceListWidget->setCurrentItem(i);
}

void MainWindow::slot_set_Vertex_QListWidgetItem(QListWidgetItem *i) {
//    ui->x_translateDoubleSpinBox->setEnabled(true);
//    ui->y_translateDoubleSpinBox->setEnabled(true);
//    ui->z_translateDoubleSpinBox->setEnabled(true);
//    ui->vertexListWidget->setCurrentItem(i);
}

void MainWindow::slot_addVertex() {
    if (nullptr == ui->mygl->selected_HalfEdge) {
        ui->mygl->setFocus();
        return ;
    }
    ui->mygl->addVertex();
    ui->mygl->geom_mesh.destroy();
    ui->mygl->geom_mesh.create();
    ui->mygl->emitCubeCreateSignal();
    ui->mygl->highlightLine(ui->mygl->selected_HalfEdge);
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_triangulate() {
    if (nullptr == ui->mygl->selected_Face) {
        ui->mygl->setFocus();
        return;
    }
    else {
        ui->mygl->triangulate();
        ui->mygl->geom_mesh.destroy();
        ui->mygl->geom_mesh.create();
        ui->mygl->emitCubeCreateSignal();
        ui->mygl->highlightFace(ui->mygl->selected_Face);
        ui->mygl->setFocus();
        update();

    }
}

void MainWindow::slot_color() {
    if (nullptr == ui->mygl->selected_Face) {
        ui->mygl->setFocus();
        return;
    }
    QColor color = QColorDialog::getColor(Qt::red, this, "Choose color");
    if (color.isValid()) {
        ui->mygl->setFaceColor(color);
        ui->mygl->geom_mesh.destroy();
        ui->mygl->geom_mesh.create();
        ui->mygl->emitCubeCreateSignal();
        ui->mygl->highlightFace(ui->mygl->selected_Face);
    }
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_subdivision() {
    ui->mygl->subdivision();
    ui->mygl->geom_mesh.destroy();
    ui->mygl->geom_mesh.create();
    ui->mygl->emitCubeCreateSignal();
    if (nullptr != ui->mygl->selected_Face) {
        ui->mygl->highlightFace(ui->mygl->selected_Face);
    }
    if (nullptr != ui->mygl->selected_HalfEdge) {
        ui->mygl->highlightLine(ui->mygl->selected_HalfEdge);
    }
    if (nullptr != ui->mygl->selected_Vertex) {
        ui->mygl->highlightVertex(ui->mygl->selected_Vertex);
    }
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_extrude() {
    if (nullptr == ui->mygl->selected_Face) {
        return ;
    }
    ui->mygl->extrude();
    ui->mygl->geom_mesh.destroy();
    ui->mygl->geom_mesh.create();
    ui->mygl->emitCubeCreateSignal();
    if (nullptr != ui->mygl->selected_Face) {
        ui->mygl->highlightFace(ui->mygl->selected_Face);
    }
    if (nullptr != ui->mygl->selected_HalfEdge) {
        ui->mygl->highlightLine(ui->mygl->selected_HalfEdge);
    }
    if (nullptr != ui->mygl->selected_Vertex) {
        ui->mygl->highlightVertex(ui->mygl->selected_Vertex);
    }

    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_loadOBJ() {
//    ui->mygl->loadOBJ();
//    if (ui->mygl->loadNewObj == false) {
//        return;
//    }
//    ui->vertexListWidget->clear();
//    ui->faceListWidget->clear();
//    ui->halfedgeListWidget->clear();

//    ui->mygl->geom_mesh.destroy();
//    ui->mygl->geom_mesh.create();
//    ui->mygl->emitCubeCreateSignal();

//    emit signal_noSelection();
//    ui->x_translateDoubleSpinBox->setEnabled(false);
//    ui->y_translateDoubleSpinBox->setEnabled(false);
//    ui->z_translateDoubleSpinBox->setEnabled(false);
//    ui->addVertexPushButton->setEnabled(false);
//    ui->colorPushButton->setEnabled(false);
//    ui->extrudePushButton->setEnabled(false);
//    ui->triangulatePushButton->setEnabled(false);

//    ui->mygl->noHightlightVertex();
//    ui->mygl->noHightlightLine();
//    ui->mygl->noHightlightFace();

//    ui->mygl->setFocus();
//    update();
}

void MainWindow::slot_loadJson() {
//    if (ui->mygl->loadJson()) {

//        ui->joint_x_translateDoubleSpinBox_2->setEnabled(false);
//        ui->joint_y_translateDoubleSpinBox_2->setEnabled(false);
//        ui->joint_z_translateDoubleSpinBox_2->setEnabled(false);

//        ui->x_rot_plus_pushButton->setEnabled(false);
//        ui->x_rot_minus_pushButton->setEnabled(false);
//        ui->y_rot_plus_pushButton->setEnabled(false);
//        ui->y_rot_minus_pushButton->setEnabled(false);
//        ui->z_rot_plus_pushButton->setEnabled(false);
//        ui->z_rot_minus_pushButton->setEnabled(false);

//        ui->mygl->setFocus();
//        update();

//    }
}

void MainWindow::slot_addTopLevelItem(Joint* i){
//    ui->jointTreeWidget->clear();
//    ui->jointTreeWidget->addTopLevelItem(i);
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_getJointItem(QTreeWidgetItem* item) {
    Joint* joint= dynamic_cast<Joint*> (item);
    currentJoint = joint;

    //text
    joint_transformation_textEdit_update(joint);

//    ui->joint_x_translateDoubleSpinBox_2->setValue(joint->position.x);
//    ui->joint_y_translateDoubleSpinBox_2->setValue(joint->position.y);
//    ui->joint_z_translateDoubleSpinBox_2->setValue(joint->position.z);

//    ui->joint_x_translateDoubleSpinBox_2->setEnabled(true);
//    ui->joint_y_translateDoubleSpinBox_2->setEnabled(true);
//    ui->joint_z_translateDoubleSpinBox_2->setEnabled(true);

//    ui->x_rot_plus_pushButton->setEnabled(true);
//    ui->x_rot_minus_pushButton->setEnabled(true);
//    ui->y_rot_plus_pushButton->setEnabled(true);
//    ui->y_rot_minus_pushButton->setEnabled(true);
//    ui->z_rot_plus_pushButton->setEnabled(true);
//    ui->z_rot_minus_pushButton->setEnabled(true);

    //let all joint's selected = 0
    for(int i = 0; i < ui->mygl->m_jointMesh.size(); i++) {
        if (ui->mygl->m_jointMesh[i]->selected == true) {
            ui->mygl->m_jointMesh[i]->selected = false;
            ui->mygl->m_jointMesh[i]->destroy();
            ui->mygl->m_jointMesh[i]->create();
            break;
        }
    }
    joint->selected = true;
    currentJoint = joint;
    ui->mygl->highlightJoint(joint);
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_setJoint_x_position(double pos_x) {
    if (nullptr == currentJoint) {
        return;
    }
    currentJoint->position.x = pos_x;
    joint_transformation_textEdit_update(currentJoint);

    currentJoint->destroy();
    currentJoint->create();
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_setJoint_y_position(double pos_y) {
    if (nullptr == currentJoint) {
        return;
    }
    currentJoint->position.y = pos_y;
    joint_transformation_textEdit_update(currentJoint);
    currentJoint->destroy();
    currentJoint->create();
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_setJoint_z_position(double pos_z) {
    if (nullptr == currentJoint) {
        return;
    }
    currentJoint->position.z = pos_z;
    joint_transformation_textEdit_update(currentJoint);
    currentJoint->destroy();
    currentJoint->create();
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_joint_x_rotation_positive() {
    if (nullptr == currentJoint) {
        return;
    }

    double angle = 15*M_PI/180;
    glm::vec3 axis(1,0,0);
    glm::quat q = glm::quat(glm::angleAxis(float(angle), axis));

    currentJoint->rotation = q * currentJoint->rotation ;
    joint_transformation_textEdit_update(currentJoint);
    currentJoint->destroy();
    currentJoint->create();
    ui->mygl->update();
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_joint_x_rotation_minus() {
    if (nullptr == currentJoint) {
        return;
    }

    double angle = -15*M_PI/180;
    glm::vec3 axis(1,0,0);
    glm::quat q = glm::quat(glm::angleAxis(float(angle), axis));

    currentJoint->rotation = q * currentJoint->rotation ;
    joint_transformation_textEdit_update(currentJoint);
    currentJoint->destroy();
    currentJoint->create();
    ui->mygl->update();
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_joint_y_rotation_positive() {
    if (nullptr == currentJoint) {
        return;
    }

    double angle = 15*M_PI/180;
    glm::vec3 axis(0,1,0);
    glm::quat q = glm::quat(glm::angleAxis(float(angle), axis));

    currentJoint->rotation = q * currentJoint->rotation ;
    joint_transformation_textEdit_update(currentJoint);

    currentJoint->destroy();
    currentJoint->create();
    ui->mygl->update();
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_joint_y_rotation_minus() {
    if (nullptr == currentJoint) {
        return;
    }

    double angle = -15*M_PI/180;
    glm::vec3 axis(0,1,0);
    glm::quat q = glm::quat(glm::angleAxis(float(angle), axis));

    currentJoint->rotation = q * currentJoint->rotation ;
    joint_transformation_textEdit_update(currentJoint);

    currentJoint->destroy();
    currentJoint->create();
    ui->mygl->update();
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_joint_z_rotation_positive() {
    if (nullptr == currentJoint) {
        return;
    }

    double angle = 15*M_PI/180;
    glm::vec3 axis(0,0,1);
    glm::quat q = glm::quat(glm::angleAxis(float(angle), axis));

    currentJoint->rotation = q * currentJoint->rotation ;
    joint_transformation_textEdit_update(currentJoint);

    currentJoint->destroy();
    currentJoint->create();
    ui->mygl->update();
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_joint_z_rotation_minus() {
    if (nullptr == currentJoint) {
        return;
    }

    double angle = -15*M_PI/180;
    glm::vec3 axis(0,0,1);
    glm::quat q = glm::quat(glm::angleAxis(float(angle), axis));

    currentJoint->rotation = q * currentJoint->rotation ;
    joint_transformation_textEdit_update(currentJoint);

    currentJoint->destroy();
    currentJoint->create();
    ui->mygl->update();
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_bindMesh() {
//    ui->mygl->drawFlat = false;
//    int vert_size = ui->vertexListWidget->count();
//    for (int vertex_count = 0; vertex_count < vert_size; vertex_count++) {
//        QListWidgetItem* item = ui->vertexListWidget->item(vertex_count);
//        Vertex* vertex = dynamic_cast<Vertex*> (item);
//        float minimal_distance = 1000000;
//        float second_min_distance = 1000000;
//        Joint* minimal_joint;
//        Joint* second_min_joint;
//        // get the smallest and second smallest joint and distance
//        for (Joint* joint : ui->mygl->m_jointMesh) {
//            if (minimal_distance > second_min_distance) {
//                float tmp_distance = minimal_distance;
//                minimal_distance = second_min_distance;
//                second_min_distance = tmp_distance;

//                Joint* tmp_joint = minimal_joint;
//                minimal_joint = second_min_joint;
//                second_min_joint = tmp_joint;
//                //delete tmp_joint;
//            }
//            glm::vec3 joint_position = glm::vec3 (joint->getOverallTransformation() * glm::vec4(0,0,0,1));
//            float vertex_joint_distance = glm::length(vertex->position - joint_position);
//            if (vertex_joint_distance < second_min_distance) {
//                second_min_distance = vertex_joint_distance;
//                second_min_joint = joint;
//            }
//        }
//        float sum_distance = minimal_distance + second_min_distance;
//        vertex->influence[0] = std::pair<int, float> (minimal_joint->id, 1 - minimal_distance / sum_distance);
//        vertex->influence[1] = std::pair<int, float> (second_min_joint->id, 1- second_min_distance / sum_distance);
//    }
//    ui->mygl->BindMesh = true;
//    ui->mygl->geom_mesh.destroy();
//    ui->mygl->geom_mesh.create();
//    ui->mygl->update();
//    ui->mygl->setFocus();
//    update();
}

void MainWindow::joint_transformation_textEdit_update(Joint* joint) {
//    ui->joint_Transfomation_textEdit->clear();
//    QString q1 = "Name: " + joint->name;
//    ui->joint_Transfomation_textEdit->append(q1);
//    QString q2 = "Position:   x: " + QString::number(joint->position.x )  +
//            "     y: " + QString::number(joint->position.y) +
//            "     z: " + QString::number(joint->position.z);
//    ui->joint_Transfomation_textEdit->append(q2);
//    QString q3 = "Rotation:   x: " + QString::number(glm::eulerAngles(joint->rotation).x * 180 / M_PI)  +
//            "°    y: " + QString::number(glm::eulerAngles(joint->rotation).y * 180 / M_PI) +
//            "°    z: " + QString::number(glm::eulerAngles(joint->rotation).z * 180 / M_PI) + "°";
//    ui->joint_Transfomation_textEdit->append(q3);

}

void MainWindow::slot_setTime(int t) {
    ui->mygl->branch.setTime(t);
    ui->mygl->updateBranch(&ui->mygl->branch);
    std::cout << "alive number: " << ui->mygl->branch.numAlive() << std::endl;
    std::cout << "size: " << ui->mygl->branch.getParticles().size() << std::endl;
    std::cout << "--------------------" << std::endl;
    ui->mygl->setFocus();
    update();

}

void MainWindow::slot_updateBranch() {
    int t = ui->timeSpinBox->value();
    ui->timeSpinBox->setValue(t+1);

    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_setWa(double i) {
    ui->mygl->branch.setWa((float)i);
    ui->mygl->updateBranch(&ui->mygl->branch);
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_setWb(double i) {
    ui->mygl->branch.setWb((float)i);
    ui->mygl->updateBranch(&ui->mygl->branch);
    ui->mygl->setFocus();
    update();
}

void MainWindow::slot_setWh(double i) {
    ui->mygl->branch.setWh((float)i);
    ui->mygl->updateBranch(&ui->mygl->branch);
    ui->mygl->setFocus();
    update();
}
