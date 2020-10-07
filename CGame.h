//
// Created by arcademiker on 28.07.18.
//

#ifndef LASERCHESS3D_CGAME_H
#define LASERCHESS3D_CGAME_H

// Include GLEW
#include <GL/glew.h>

// Include GLFW
//#include <GLFW/glfw3.h>
#include <external/glfw-3.1.2/include/GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CMap.h"

struct graphics_context {
    GLFWwindow* window;
    double lastTime;
    int nbFrames;

    GLuint fboMsaaId;
    GLuint rboColorId;
    GLuint rboDepthId;
    GLuint fboId;
    GLuint rboId;
    GLuint textureId;


    int resWidth;
    int resHeight;

    GLuint programID;

    GLuint TextureID[5];
    GLuint textures[5];

    GLuint LightID;

    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;

    glm::mat4 ProjectionMatrix;
    glm::mat4 MVP;
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix;

    std::vector<unsigned short> indices[9];
    GLuint elementbuffer[9];

    GLuint VertexArrayID[9];

};

class CGame {
private:
    graphics_context* context;
    std::map<int,CUnit*>* UMap;
    CUnit* U;
    CUnit* E;
    std::multimap<int,CUnit*>* EMap;
    int round;
    int id;
    int oldState;
    int old_x;
    int old_y;
    int anime;
    CMap* map;
public:
    CGame(CMap* map, graphics_context* context);
    ~CGame();
    int gameloop();
    int logic_step(int step);
    void drawGame(int step);
    bool user_input();
    void print_options(CUnit* unit);
};


#endif //LASERCHESS3D_CGAME_H
