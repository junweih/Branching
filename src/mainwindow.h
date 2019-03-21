#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "vertexmesh.h"
#include "joint.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    double x_position = 0;
    double y_position = 0;
    double z_position = 0;

    Vertex* currentVertex = nullptr;
    HalfEdge* currentHalfEdge = nullptr;
    Joint* currentJoint = nullptr;

private slots:
    void on_actionQuit_triggered();

    void on_actionCamera_Controls_triggered();

    void slot_cubeInit();

    void slot_getVertexQListWidgetItem(QListWidgetItem*);

    void slot_getHalfEdgeQListWidgetItem(QListWidgetItem*);

    void slot_getFaceQListWidgetItem(QListWidgetItem*);

    void slot_getVertexPosition(QListWidgetItem*);

    void slot_setVertex_x_position(double);
    void slot_setVertex_y_position(double);
    void slot_setVertex_z_position(double);

    void slot_setHalfEdgeQListWidgetItem(QListWidgetItem*);
    void slot_set_Face_QListWidgetItem(QListWidgetItem*);
    void slot_set_Vertex_QListWidgetItem(QListWidgetItem*);
    void slot_addVertex();
    void slot_triangulate();
    void slot_color();
    void slot_subdivision();
    void slot_extrude();
    void slot_loadOBJ();
    void slot_loadJson();
    void slot_addTopLevelItem(Joint*);

    void slot_getJointItem(QTreeWidgetItem*);

    void slot_setJoint_x_position(double);
    void slot_setJoint_y_position(double);
    void slot_setJoint_z_position(double);

    void slot_joint_x_rotation_positive();
    void slot_joint_x_rotation_minus();
    void slot_joint_y_rotation_positive();
    void slot_joint_y_rotation_minus();
    void slot_joint_z_rotation_positive();
    void slot_joint_z_rotation_minus();
    void slot_bindMesh();

    void joint_transformation_textEdit_update(Joint*);

    void slot_setTime(int);
    void slot_updateBranch();
    void slot_setWa(double);
    void slot_setWb(double);
    void slot_setWh(double);

signals:
    void signal_send_x_position(double);
    void signal_send_y_position(double);
    void signal_send_z_position(double);
    void signal_noSelection();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
