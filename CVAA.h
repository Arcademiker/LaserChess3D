//
// Created by arcademiker on 27.07.18.
//

#ifndef LASERCHESS3D_CVAA_H
#define LASERCHESS3D_CVAA_H

// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class CVAA {
public:
    int id;
    int size;
    int stride;
    int buffer_offset;
    CVAA(GLuint &buffer, std::vector<glm::vec3> &indexed_vec, GLuint id, GLint size, int stride, int buffer_offset);
    CVAA(GLuint &buffer, std::vector<glm::vec2> &indexed_vec, GLuint id, GLint size, int stride, int buffer_offset);
    CVAA(GLuint &buffer, std::vector<unsigned short> &indexed_vec);
};


#endif //LASERCHESS3D_CVAA_H
