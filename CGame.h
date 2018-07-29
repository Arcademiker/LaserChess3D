//
// Created by arcademiker on 28.07.18.
//

#ifndef LASERCHESS3D_CGAME_H
#define LASERCHESS3D_CGAME_H

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CMap.h"

struct graphics_context {
    GLFWwindow* window;
    double lastTime;
    int nbFrames;
    GLuint programID;
    GLuint TextureID;

    GLuint LightID;
    GLuint elementbuffer;

    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;

    glm::mat4 MVP;
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix;

    std::vector<unsigned short> indices;

    GLuint VertexArrayID[1];
    GLuint textures[1];
};

class CGame {
private:
    graphics_context* context;
    std::map<int,CUnit*>* UMap;
    CUnit* U;
    std::multimap<int,CUnit*>* EMap;
    int round;
    int id;
    int oldState;
    CMap* map;
public:
    CGame(CMap* map, graphics_context* context);
    ~CGame();
    bool gameloop();
    int logic_step(int step);
    void drawGame();
    bool user_input();
    void print_options(CUnit* unit);
};


#endif //LASERCHESS3D_CGAME_H
