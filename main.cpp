/* todo:
// Projekt Strukturieren
// Positionen abfragen + zeichnen
// Schachbrett
// Maus Steuerung (hightlighting)
// Alle 6 modelle
// Licht + texturen
// Bewegungspfade animieren
// HP darstellen
// Schüsse animieren
// Schaden animieren
// You win loose text?
*/

// Include standard headers
#include <cstdio>
#include <cstdlib>

#include <iostream>



// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// texture loading
#include "SOIL.h"

#include "common/Shader.h"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"
#include "common/CView.h"

#include "CVAA.h"

#include <map>

#include "CMap.h"
#include "CGame.h"




CMap* generate_map(int level);
//void print_options(CUnit* unit, CMap* map);
void loadImage_SOIL(GLuint* textures,const char* imagepath, unsigned int texIndex);

int main()
{

    // creat graphic context;
    graphics_context context;



    /// graphics ini:
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // Open a context.window and create its OpenGL context
    context.window = glfwCreateWindow( 1024, 768, "LASER CHESS", NULL, NULL);
    if( context.window == NULL ){
        fprintf( stderr, "Failed to open GLFW context.window. If you have an old Intel GPU, they are not 3.3 compatible.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(context.window);

    // Initialize GLEW
    glewExperimental = 1; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(context.window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //glfwSetInputMode(context.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // todo: comment out

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(context.window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);




    /// Create and compile our GLSL program from the shaders
    CShader *myShader = CShader::createShaderProgram("../StandardShading.vertexshader", nullptr, nullptr, nullptr, "../StandardShading.fragmentshader" );
    context.programID = myShader->getID();

    // Get a handle for our "MVP" uniform
    context.MatrixID = glGetUniformLocation(context.programID, "MVP");
    context.ViewMatrixID = glGetUniformLocation(context.programID, "V");
    context.ModelMatrixID = glGetUniformLocation(context.programID, "M");

    // Load the texture
    glGenTextures(1, context.textures);
    loadImage_SOIL(context.textures,"../units/tank.jpeg",0);

    // Get a handle for our "myTextureSampler" uniform
    context.TextureID  = glGetUniformLocation(context.programID, "myTextureSampler");

    glGenVertexArrays(6, context.VertexArrayID);

    std::vector<glm::vec3> indexed_vertices[6];
    std::vector<glm::vec2> indexed_uvs[6];
    std::vector<glm::vec3> indexed_normals[6];

    GLuint vertexbuffer[6];
    GLuint uvbuffer[6];
    GLuint normalbuffer[6];

    auto vaaV = new std::vector<CVAA*>;
    auto vaaU = new std::vector<CVAA*>;
    auto vaaN = new std::vector<CVAA*>;
    auto vaaE = new std::vector<CVAA*>;

    /// unit0
    for(int i = 0; i < 6; ++i) {
        glBindVertexArray(context.VertexArrayID[i]);

        // Read our .obj file
        // todo: read 6 different objects!
        bool res = loadAssImp("../units/tank.obj", context.indices[i], indexed_vertices[i], indexed_uvs[i],
                              indexed_normals[i]);
        // Load it into a VBO
        vaaV->push_back(new CVAA(vertexbuffer[i], indexed_vertices[i], 0, 3, 0, 0)); // 1rst attribute buffer : vertices
        vaaU->push_back(new CVAA(uvbuffer[i], indexed_uvs[i], 1, 2, 0, 0));  // 2nd attribute buffer : UVs
        vaaN->push_back(new CVAA(normalbuffer[i], indexed_normals[i], 2, 3, 0, 0));  // 3rd attribute buffer : normals
        vaaE->push_back(new CVAA(context.elementbuffer[i], context.indices[i]));  // Generate a buffer for the indices as well
    }

    // Get a handle for our "LightPosition" uniform
    glUseProgram(context.programID);
    context.LightID = glGetUniformLocation(context.programID, "LightPosition_worldspace");

    // For speed computation
    context.lastTime = glfwGetTime();
    context.nbFrames = 0;
    CView view(-30.0f,30.0f,-30.0f,3.14f/4.0f,-3.14f/5.1f);

    // Compute the MVP
    // glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(view.FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    context.ProjectionMatrix =  glm::ortho(10.0f*-4.0f/3.0f,10.0f*4.0f/3.0f,-10.0f,10.0f,0.1f, 100.0f);
    context.ViewMatrix = glm::lookAt(
            view.position,           // Camera is here
            view.position+view.direction, // and looks here : at the same position, plus "direction"
            view.up                  // Head is up (set to 0,-1,0 to look upside-down)
    );



    /// start game:
    for(int Level = 1; Level <= 3; ++Level) {
        /// game logic:
        CMap* map = generate_map(Level);
        std::cout << std::endl << "++++++++++++++++++  LEVEL " << Level << "  ++++++++++++++++++" << std::endl;

        /// main game loop:
        CGame game(map, &context);
        // check if player may enter next level
        if (game.gameloop() == 1) {
            delete map;
            break;
        } else {
            Level = game.gameloop();
        }
        delete map;
    }


    /// delete grafic buffers:
    // Cleanup VBO and shader
    glDeleteBuffers(6, vertexbuffer);
    glDeleteBuffers(6, uvbuffer);
    glDeleteBuffers(6, normalbuffer);
    glDeleteBuffers(6, context.elementbuffer);
    glDeleteProgram(context.programID);
    glDeleteTextures(1, context.textures);
    glDeleteVertexArrays(3, context.VertexArrayID);

    delete vaaE;
    delete vaaN;
    delete vaaU;
    delete vaaV;
    // Close OpenGL context.window and terminate GLFW
    glfwTerminate();



    return 0;
}

CMap* generate_map(int level) {
    auto map = new CMap();
    switch(level){
        case 1:
            map->add_unit(4,1,2);
            map->add_unit(4,1,3);
            map->add_unit(4,1,4);
            map->add_unit(4,1,5);
            map->add_unit(5,0,3);
            map->add_unit(5,0,4);
            map->add_unit(6,0,2);
            map->add_unit(6,0,5);

            map->add_unit(1,7,0);
            map->add_unit(1,7,7);
            map->add_unit(1,6,1);
            map->add_unit(1,6,6);
            map->add_unit(1,5,2);
            map->add_unit(1,5,5);
            map->add_unit(1,4,3);
            map->add_unit(1,4,4);
            map->add_unit(2,7,4);
            map->add_unit(3,7,3);

            break;
        case 2:
            map->add_unit(4,1,1);
            map->add_unit(4,1,3);
            map->add_unit(4,1,5);
            map->add_unit(4,1,7);
            map->add_unit(6,0,0);
            map->add_unit(6,0,2);
            map->add_unit(6,0,4);
            map->add_unit(6,0,6);

            map->add_unit(1,5,0);
            map->add_unit(1,5,1);
            map->add_unit(1,5,2);
            map->add_unit(1,5,4);
            map->add_unit(1,5,5);
            map->add_unit(1,5,6);
            map->add_unit(1,5,7);
            map->add_unit(3,5,3);
            map->add_unit(2,4,3);
            map->add_unit(2,7,0);
            map->add_unit(2,7,1);
            map->add_unit(2,7,2);
            map->add_unit(2,7,3);
            map->add_unit(2,7,4);
            map->add_unit(2,7,5);
            map->add_unit(2,7,6);
            map->add_unit(2,7,7);

            break;
        case 3:
            map->add_unit(4,1,0);
            map->add_unit(4,1,1);
            map->add_unit(4,1,2);
            map->add_unit(4,1,3);
            map->add_unit(4,1,4);
            map->add_unit(4,1,5);
            map->add_unit(4,1,6);
            map->add_unit(4,1,7);
            map->add_unit(6,0,0);
            map->add_unit(5,0,1);
            map->add_unit(5,0,2);
            map->add_unit(6,0,3);
            map->add_unit(6,0,4);
            map->add_unit(5,0,5);
            map->add_unit(5,0,6);
            map->add_unit(6,0,7);

            map->add_unit(1,6,0);
            map->add_unit(1,6,1);
            map->add_unit(1,6,2);
            map->add_unit(1,6,3);
            map->add_unit(1,6,4);
            map->add_unit(1,6,5);
            map->add_unit(1,6,6);
            map->add_unit(1,6,7);
            map->add_unit(1,4,2);
            map->add_unit(1,4,3);
            map->add_unit(1,4,4);
            map->add_unit(1,4,5);
            map->add_unit(1,2,5);
            map->add_unit(1,5,5);
            map->add_unit(2,3,5);
            map->add_unit(2,4,5);
            map->add_unit(3,7,0);
            map->add_unit(2,7,1);
            map->add_unit(2,7,2);
            map->add_unit(2,7,3);
            map->add_unit(2,7,4);
            map->add_unit(2,7,5);
            map->add_unit(2,7,6);
            map->add_unit(3,7,7);

            break;
        default:
            return nullptr;
    }
    return map;
}

void loadImage_SOIL(GLuint* textures,const char* imagepath, unsigned int texIndex) {

    int width, height;
    unsigned char* image;

    glActiveTexture(GL_TEXTURE0 + texIndex);
    glBindTexture(GL_TEXTURE_2D, textures[texIndex]);

    image = SOIL_load_image(imagepath, &width, &height, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (0 == image)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
    }
    SOIL_free_image_data(image);
    //glUniform1i(glGetUniformLocation(shaderProgram, "myTexture"), 0);

    //glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, GL_CLAMP_TO_EDGE); //correct

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}