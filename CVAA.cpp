//
// Created by arcademiker on 27.07.18.
//

#include "CVAA.h"

CVAA::CVAA(GLuint buffer,GLuint id, GLint size, int stride, int buffer_offset) {
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
