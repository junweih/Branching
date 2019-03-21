#include "mesh.h"

Mesh::Mesh(GLWidget277 *context) : Drawable(context)
{}

int Mesh::getIndex() {
    int num = 0;

    for (unsigned int i = 0; i < m_Faces.size(); i++) {
        HalfEdge* startEdge = m_Faces[i]->start_edge;
        HalfEdge* next = startEdge->next_he;
        while (next != startEdge) {
            next = next->next_he;
            num++;
        }
    }
    return 3 * (num-m_Faces.size());
}

void Mesh::create() {
//    int halfedge_size = m_HalfEdges.size();
//    mesh_vert_col.clear();
//    mesh_vert_nor.clear();
//    mesh_vert_pos.clear();
//    mesh_idx.clear();
//    mesh_jointID.clear();
//    mesh_weights.clear();


//    mesh_vert_col.reserve(halfedge_size);
//    mesh_vert_nor.reserve(halfedge_size);
//    mesh_vert_pos.reserve(halfedge_size);
//    int n = 0;
//    for (Face* face : m_Faces) {
//        HalfEdge* startEdge = face->start_edge;
//        HalfEdge* he;
//        int num_he = 0;
//        for (he = startEdge; he->next_he != startEdge; he = he->next_he) {
//            mesh_vert_pos.push_back(glm::vec4(he->vertex->position, 1));
//            mesh_vert_col.push_back(glm::vec4(face->color, 1));
//            glm::vec3 nor = glm::cross((he->next_he->vertex->position - he->vertex->position),
//                                       (he->next_he->next_he->vertex->position - he->next_he->vertex->position));
//            mesh_vert_nor.push_back(glm::vec4(nor, 1));

//            if(he->vertex->influence[0].first != -1) {
//                mesh_jointID.push_back(glm::ivec2(he->vertex->influence[0].first,
//                                                 he->vertex->influence[1].first));
//                mesh_weights.push_back(glm::vec2(he->vertex->influence[0].second,
//                                                 he->vertex->influence[1].second));
//            }
//            num_he++;
//        }
//        mesh_vert_pos.push_back(glm::vec4(he->vertex->position, 1));
//        mesh_vert_col.push_back(glm::vec4(face->color, 1));
//        glm::vec3 nor = glm::cross((he->next_he->vertex->position - he->vertex->position),
//                                   (he->next_he->next_he->vertex->position - he->next_he->vertex->position));
//        mesh_vert_nor.push_back(glm::vec4(nor, 1));
//        if(he->vertex->influence[0].first != -1) {
//            mesh_jointID.push_back(glm::ivec2(he->vertex->influence[0].first,
//                                             he->vertex->influence[1].first));
//            mesh_weights.push_back(glm::vec2(he->vertex->influence[0].second,
//                                             he->vertex->influence[1].second));
//        }

//        num_he++;
//        for (int i = 0; i < num_he-2; i++) {
//            mesh_idx.push_back(n);
//            mesh_idx.push_back(n+i+1);
//            mesh_idx.push_back(n+i+2);
//        }
//        n += num_he;
//    }
//    count = mesh_idx.size();

//    generateIdx();
//    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
//    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), mesh_idx.data(), GL_STATIC_DRAW);

//    generatePos();
//    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufPos);
//    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_vert_pos.size() * sizeof(glm::vec4), mesh_vert_pos.data(), GL_STATIC_DRAW);

//    generateNor();
//    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufNor);
//    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_vert_nor.size() * sizeof(glm::vec4), mesh_vert_nor.data(), GL_STATIC_DRAW);

//    generateCol();
//    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufCol);
//    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_vert_col.size() * sizeof(glm::vec4), mesh_vert_col.data(), GL_STATIC_DRAW);

}

void Mesh::loadCube()
{
    Vertex *V0 = new Vertex(0), *V1 = new Vertex(1), *V2 = new Vertex(2), *V3 = new Vertex(3),
            *V4 = new Vertex(4), *V5 = new Vertex(5),*V6 = new Vertex(6), *V7 = new Vertex(7);

    float scale = 0.6;

    //set pos
    V0->position = glm::vec3( 0.5, 0.5, 0.5);
    V1->position = glm::vec3( 0.5, 0.5, -0.5);
    V2->position = glm::vec3(-0.5, 0.5, -0.5);
    V3->position = glm::vec3(-0.5, 0.5, 0.5);
    V4->position = glm::vec3( 0.5, -0.5, 0.5);
    V5->position = glm::vec3( 0.5, -0.5,-0.5);
    V6->position = glm::vec3(-0.5, -0.5, -0.5);
    V7->position = glm::vec3(-0.5, -0.5, 0.5);

    //top face:
    Face *F0 = new Face(0);
    F0->color = glm::vec3(0,0,1) * scale;
    HalfEdge *HE0 = new HalfEdge(0), *HE1 = new HalfEdge(1), *HE2 = new HalfEdge(2), *HE3 = new HalfEdge(3);
    F0->start_edge = HE0;
    //set next
    HE0->next_he = HE1;
    HE1->next_he = HE2;
    HE2->next_he = HE3;
    HE3->next_he = HE0;
    //set face
    HE0->face = F0;
    HE1->face = F0;
    HE2->face = F0;
    HE3->face = F0;
    //set Vertex
    HE0->vertex = V0;
    HE1->vertex = V1;
    HE2->vertex = V2;
    HE3->vertex = V3;

    //buttom face:
    Face *F1 = new Face(1);
    F1->color = glm::vec3(0,1,0) * scale;
    HalfEdge *HE4 = new HalfEdge(4), *HE5 = new HalfEdge(5), *HE6 = new HalfEdge(6), *HE7 = new HalfEdge(7);
    F1->start_edge = HE4;
    //set next
    HE4->next_he = HE7;
    HE5->next_he = HE4;
    HE6->next_he = HE5;
    HE7->next_he = HE6;
    //set face
    HE4->face = F1;
    HE5->face = F1;
    HE6->face = F1;
    HE7->face = F1;
    //set Vertex
    HE4->vertex = V4;
    HE5->vertex = V5;
    HE6->vertex = V6;
    HE7->vertex = V7;

    //left face:
    Face *F2 = new Face(2);
    HalfEdge *HE8 = new HalfEdge(8), *HE9 = new HalfEdge(9), *HE10 = new HalfEdge(10), *HE11 = new HalfEdge(11);
    F2->color = glm::vec3(1,0,0) * scale;
    F2->start_edge = HE8;
    //set next
    HE8->next_he = HE11;
    HE9->next_he = HE8;
    HE10->next_he = HE9;
    HE11->next_he = HE10;
    //set face
    HE8->face = F2;
    HE9->face = F2;
    HE10->face = F2;
    HE11->face = F2;
    //set Vertex
    HE8->vertex = V3;
    HE9->vertex = V7;
    HE10->vertex = V6;
    HE11->vertex = V2;

    //right face:
    Face *F3 = new Face(3);
    HalfEdge *HE12 = new HalfEdge(12), *HE13 = new HalfEdge(13), *HE14 = new HalfEdge(14), *HE15 = new HalfEdge(15);
    F3->color = glm::vec3(0,1,1) * scale;
    F3->start_edge = HE12;
    //set next
    HE12->next_he = HE13;
    HE13->next_he = HE14;
    HE14->next_he = HE15;
    HE15->next_he = HE12;
    //set face
    HE12->face = F3;
    HE13->face = F3;
    HE14->face = F3;
    HE15->face = F3;
    //set Vertex
    HE12->vertex = V0;
    HE13->vertex = V4;
    HE14->vertex = V5;
    HE15->vertex = V1;

    //front face:
    Face *F4 = new Face(4);
    HalfEdge *HE16 = new HalfEdge(16), *HE17 = new HalfEdge(17), *HE18 = new HalfEdge(18), *HE19 = new HalfEdge(19);
    F4->color = glm::vec3(1,0,1) * scale;
    F4->start_edge = HE16;
    //set next
    HE16->next_he = HE17;
    HE17->next_he = HE18;
    HE18->next_he = HE19;
    HE19->next_he = HE16;
    //set face
    HE16->face = F4;
    HE17->face = F4;
    HE18->face = F4;
    HE19->face = F4;
    //set Vertex
    HE16->vertex = V0;
    HE17->vertex = V3;
    HE18->vertex = V7;
    HE19->vertex = V4;

    //back face:
    Face *F5 = new Face(5);
    HalfEdge *HE20 = new HalfEdge(20), *HE21 = new HalfEdge(21), *HE22 = new HalfEdge(22), *HE23 = new HalfEdge(23);
    F5->color = glm::vec3(1,1,0) * scale;
    F5->start_edge = HE20;
    //set next
    HE20->next_he = HE23;
    HE21->next_he = HE20;
    HE22->next_he = HE21;
    HE23->next_he = HE22;
    //set face
    HE20->face = F5;
    HE21->face = F5;
    HE22->face = F5;
    HE23->face = F5;
    //set Vertex
    HE20->vertex = V1;
    HE21->vertex = V2;
    HE22->vertex = V6;
    HE23->vertex = V5;

    //set SYM:
    HE0->sym_he = HE17;
    HE1->sym_he = HE12;
    HE2->sym_he = HE20;
    HE3->sym_he = HE11;
    HE4->sym_he = HE14;
    HE5->sym_he = HE22;
    HE6->sym_he = HE9;
    HE7->sym_he = HE19;
    HE8->sym_he = HE18;
    HE9->sym_he = HE6;
    HE10->sym_he = HE21;
    HE11->sym_he = HE3;
    HE12->sym_he = HE1;
    HE13->sym_he = HE16;
    HE14->sym_he = HE4;
    HE15->sym_he = HE23;
    HE16->sym_he = HE13;
    HE17->sym_he = HE0;
    HE18->sym_he = HE8;
    HE19->sym_he = HE7;
    HE20->sym_he = HE2;
    HE21->sym_he = HE10;
    HE22->sym_he = HE5;
    HE23->sym_he = HE15;

    V0->halfedge = HE0;
    V1->halfedge = HE1;
    V2->halfedge = HE2;
    V3->halfedge = HE3;
    V4->halfedge = HE4;
    V5->halfedge = HE5;
    V6->halfedge = HE6;
    V7->halfedge = HE7;

    //push_back:
    //Faces
    m_Faces.push_back(F0);
    m_Faces.push_back(F1);
    m_Faces.push_back(F2);
    m_Faces.push_back(F3);
    m_Faces.push_back(F4);
    m_Faces.push_back(F5);
    //Vertices
    m_Vertices.push_back(V0);
    m_Vertices.push_back(V1);
    m_Vertices.push_back(V2);
    m_Vertices.push_back(V3);
    m_Vertices.push_back(V4);
    m_Vertices.push_back(V5);
    m_Vertices.push_back(V6);
    m_Vertices.push_back(V7);
    //HalfEdges
    m_HalfEdges.push_back(HE0);
    m_HalfEdges.push_back(HE1);
    m_HalfEdges.push_back(HE2);
    m_HalfEdges.push_back(HE3);
    m_HalfEdges.push_back(HE4);
    m_HalfEdges.push_back(HE5);
    m_HalfEdges.push_back(HE6);
    m_HalfEdges.push_back(HE7);
    m_HalfEdges.push_back(HE8);
    m_HalfEdges.push_back(HE9);
    m_HalfEdges.push_back(HE10);
    m_HalfEdges.push_back(HE11);
    m_HalfEdges.push_back(HE12);
    m_HalfEdges.push_back(HE13);
    m_HalfEdges.push_back(HE14);
    m_HalfEdges.push_back(HE15);
    m_HalfEdges.push_back(HE16);
    m_HalfEdges.push_back(HE17);
    m_HalfEdges.push_back(HE18);
    m_HalfEdges.push_back(HE19);
    m_HalfEdges.push_back(HE20);
    m_HalfEdges.push_back(HE21);
    m_HalfEdges.push_back(HE22);
    m_HalfEdges.push_back(HE23);
}
