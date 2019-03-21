#ifndef JOINT_H
#define JOINT_H
#include <string>
#include <vector>
#include "la.h"
#include <string>
#include <QTreeWidgetItem>
#include <drawable.h>
#include <QString>
#include <map>

static const int WF_IDX_COUNT = 74;
static const int WF_VERT_COUNT = 38;
static const int WF_COL_COUNT = 38;

class Joint : public QTreeWidgetItem, public Drawable
{
public:
    Joint(GLWidget277 *context);
 //   Joint(Joint* j);
   // Joint(int id);
    ~Joint();

    int id;
    QString name;
    Joint* parent;
    std::vector<Joint*> children;
    glm::vec3 position;
    glm::quat rotation;
    glm::mat4 bindMatrix;
    bool selected = 0;
    std::map<Joint*, float> influence;


    GLenum drawMode() override;
    void create() override;

    glm::mat4 getLocalTransformation();
    glm::mat4 getOverallTransformation();

    void createWireFramePosition(glm::vec4 (&wf_vert_pos)[WF_VERT_COUNT], Joint* joint);


};

#endif // JOINT_H
