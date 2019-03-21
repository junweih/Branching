#include "joint.h"


void createWireFrameIndices(GLuint (&wf_idx)[WF_IDX_COUNT]);
void createWireFramePosition(glm::vec4 (&wf_vert_pos)[WF_VERT_COUNT], Joint* joint);
void createWireFrameColor(glm::vec4 (&wf_col)[WF_COL_COUNT], bool selected);

Joint::Joint(GLWidget277 *context)
    : Drawable(context), id(-1), name("Joint"), parent(nullptr), position(glm::vec3(0,0,0)),
      rotation(glm::quat(1,0,0,0)), bindMatrix(glm::mat4(0))
{}

Joint::~Joint() {
    while (children.size() > 0) {
        children.erase(children.begin());
        delete children[0];
    }
    children.clear();
    if (parent != nullptr) {
        for (int i = 0; i < parent->children.size(); i++) {
            if (this == parent->children[i]) {
                parent->children.erase(parent->children.begin()+i);
            }
        }
    }
}

GLenum Joint::drawMode() {
    return GL_LINES;
}

glm::mat4 Joint::getLocalTransformation()
{
    return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
}

glm::mat4 Joint::getOverallTransformation()
{
    if (parent != nullptr) {
        return parent->getOverallTransformation() * getLocalTransformation();
    }
    else {
        return getLocalTransformation();
    }
}

void Joint::create() {
    GLuint wf_idx[WF_IDX_COUNT];
    glm::vec4 wf_vert_pos[WF_VERT_COUNT];
    glm::vec4 wf_col[WF_COL_COUNT];


    createWireFramePosition(wf_vert_pos, this);

    createWireFrameIndices(wf_idx);
    createWireFrameColor(wf_col, selected);

    count = WF_IDX_COUNT;

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, WF_IDX_COUNT * sizeof(GLuint), wf_idx, GL_STATIC_DRAW);
    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, WF_VERT_COUNT * sizeof(glm::vec4), wf_vert_pos, GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, WF_COL_COUNT * sizeof(glm::vec4), wf_col, GL_STATIC_DRAW);

}

void createWireFrameIndices(GLuint (&wf_idx)[WF_IDX_COUNT]) {
    for(int i = 0; i < 22; i+=2){
        wf_idx[i] = i/2;
        wf_idx[i+1] = i/2+1;
    }
    wf_idx[22] = 11;
    wf_idx[23] = 0;

    for(int i = 24; i < 46; i+=2){
        wf_idx[i] = i/2;
        wf_idx[i+1] = i/2+1;
    }
    wf_idx[46] = 23;
    wf_idx[47] = 12;

    for(int i = 48; i < 70; i+=2){
        wf_idx[i] = i/2;
        wf_idx[i+1] = i/2+1;
    }
    wf_idx[70] = 35;
    wf_idx[71] = 24;

    wf_idx[72] = 36;
    wf_idx[73] = 37;
}

void Joint::createWireFramePosition(glm::vec4 (&wf_vert_pos)[WF_VERT_COUNT], Joint* joint) {
    for (int i= 0; i<12; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(0, 0, 1)) * glm::vec4(0,0.5f,0,1);
        wf_vert_pos[i] = v;
    }
    for (int i= 12; i<24; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(1, 0, 0)) * glm::vec4(0,0.5f,0,1);;
        wf_vert_pos[i] = v;
    }
    for (int i= 24; i<36; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*30.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0,0,1);;
        wf_vert_pos[i] = v;
    }
    wf_vert_pos[36] = glm::vec4(0,0,0,1);
    if (nullptr != joint->parent) {
        wf_vert_pos[37] = glm::inverse(joint->getLocalTransformation()) * glm::vec4(0,0,0,1);
    }
    else {
        wf_vert_pos[37] = glm::vec4(0,0,0,1);
    }
//    for (int i = 0; i < 38; i++) {
//        wf_vert_pos[i] = this->getOverallTransformation() * wf_vert_pos[i];
//    }
}

void createWireFrameColor(glm::vec4 (&wf_col)[WF_COL_COUNT], bool selected) {
    if (!selected){
        for (int i = 0; i < 12; i++) {
            wf_col[i] = glm::vec4 (1,0,0,1);
        }
        for (int i = 12; i < 24; i++) {
            wf_col[i] = glm::vec4 (0,1,0,1);
        }
        for (int i = 24; i < 36; i++) {
            wf_col[i] = glm::vec4 (0,0,1,1);
        }
        wf_col[36] = glm::vec4(1,1,0,1);
        wf_col[37] = glm::vec4(1,0,1,1);
//        wf_col[36] = glm::vec4(1,1,1,1);
//        wf_col[37] = glm::vec4(0,0,0,1);


    }
    else {
        for (int i = 0; i < 36; i++) {
            wf_col[i] = glm::vec4(1,1,1,1);
        }
        wf_col[36] = glm::vec4(1,1,0,1);
        wf_col[37] = glm::vec4(1,0,1,1);

    }
}
