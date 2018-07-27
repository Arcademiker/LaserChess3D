//
// Created by arcademiker on 27.07.18.
//

#include "CView.h"




CView::CView(float x, float y, float z, float dir_h, float dir_v) {
    this->FoV = 45.0f;
    /// Initial position : on +Z
    this->position = glm::vec3( x, y, z );


    /// Initial horizontal angle : toward -Z
    this->horizontalAngle = dir_h;
    /// Initial vertical angle : none
    this->verticalAngle = dir_v;

    /// Direction : Spherical coordinates to Cartesian coordinates conversion
    this->direction = glm::vec3(
            cos(this->verticalAngle) * sin(this->horizontalAngle),
            sin(this->verticalAngle),
            cos(this->verticalAngle) * cos(this->horizontalAngle)
    );

    /// Right vector
    this->right = glm::vec3(
            sin(this->horizontalAngle - 3.14f/2.0f),
            0,
            cos(this->horizontalAngle - 3.14f/2.0f)
    );

    /// Up vector
    this->up = glm::cross( this->right, this->direction );
}
