//
// Created by arcademiker on 27.07.18.
//

#ifndef LASERCHESS3D_CVAA_H
#define LASERCHESS3D_CVAA_H

// Include GLEW
#include <GL/glew.h>

class CVAA {
public:
    int id;
    int size;
    int stride;
    int buffer_offset;
    CVAA(GLuint buffer, GLuint id, GLint size, int stride, int buffer_offset);

};


#endif //LASERCHESS3D_CVAA_H
