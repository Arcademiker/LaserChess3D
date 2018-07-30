//
// Created by arcademiker on 27.07.18.
//

#include "CVAA.h"

CVAA::CVAA(GLuint& buffer, std::vector<glm::vec3>& indexed_vec, GLuint id, GLint size, int stride, int buffer_offset) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vec.size() * sizeof(glm::vec3), &indexed_vec[0], GL_STATIC_DRAW);

    this->id = id;
    this->size = size;
    this->stride = stride;
    this->buffer_offset = buffer_offset;
    auto pAttributPointer = reinterpret_cast<int*>(sizeof(float)*this->buffer_offset);

    glEnableVertexAttribArray(id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(
            id,                  // attribute
            size,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            stride,                  // stride
            pAttributPointer    //(void*)0 array buffer offset
    );
}

CVAA::CVAA(GLuint& buffer, std::vector<glm::vec2>& indexed_vec, GLuint id, GLint size, int stride, int buffer_offset) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vec.size() * sizeof(glm::vec2), &indexed_vec[0], GL_STATIC_DRAW);

    this->id = id;
    this->size = size;
    this->stride = stride;
    this->buffer_offset = buffer_offset;
    auto pAttributPointer = reinterpret_cast<int*>(sizeof(float)*this->buffer_offset);

    glEnableVertexAttribArray(id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(
            id,                  // attribute
            size,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            stride,                  // stride
            pAttributPointer    //(void*)0 array buffer offset
    );
}

CVAA::CVAA(GLuint& buffer, std::vector<unsigned short>& indexed_vec) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexed_vec.size() * sizeof(unsigned short), &indexed_vec[0], GL_STATIC_DRAW);
}

