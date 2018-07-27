//
// Created by arcademiker on 27.07.18.
//

#ifndef LASERCHESS3D_VIEW_H
#define LASERCHESS3D_VIEW_H


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CView {
public:
    float FoV;
    glm::vec3 position;
    float horizontalAngle;
    float verticalAngle;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;
    CView(float x, float y, float z, float dir_h, float dir_v);
};


#endif //LASERCHESS3D_VIEW_H
