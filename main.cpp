/* todo:
// Projekt Strukturieren
// Licht + texturen
// enter skip, right click move choose another
// Bewegungspfade animieren
// HP darstellen
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

#include <GL/glxew.h> //for vsync


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

    context.resWidth = 1920;//1024;
    context.resHeight = 1080;//768;
            // Open a context.window and create its OpenGL context
    context.window = glfwCreateWindow( context.resWidth, context.resHeight, "LASER CHESS", NULL, NULL);
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

    //activating vsync on linux
    Display *dpy = glXGetCurrentDisplay();
    GLXDrawable drawable = glXGetCurrentDrawable();
    const int interval = 1;
    if (drawable) {
        glXSwapIntervalEXT(dpy, drawable, interval);
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(context.window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //glfwSetInputMode(context.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // todo: comment out

    // Set the mouse at the center of the screen
    glfwPollEvents();

    glfwSetCursorPos(context.window, context.resWidth/2, context.resHeight/2);

    // Dark blue background
    glClearColor(0.1f, 0.0f, 0.2f, 0.0f);



    ///MSAA
    int msaaCount = 4;
    // create a MSAA framebuffer object
    // NOTE: All attachment images must have the same # of samples.
    // Ohterwise, the framebuffer status will not be completed.
    glGenFramebuffers(1, &context.fboMsaaId);
    glBindFramebuffer(GL_FRAMEBUFFER, context.fboMsaaId);

    // create a MSAA renderbuffer object to store color info
    glGenRenderbuffers(1, &context.rboColorId);
    glBindRenderbuffer(GL_RENDERBUFFER, context.rboColorId);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaCount, GL_RGB8, context.resWidth, context.resHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // create a MSAA renderbuffer object to store depth info
    // NOTE: A depth renderable image should be attached the FBO for depth test.
    // If we don't attach a depth renderable image to the FBO, then
    // the rendering output will be corrupted because of missing depth test.
    // If you also need stencil test for your rendering, then you must
    // attach additional image to the stencil attachement point, too.
    glGenRenderbuffers(1, &context.rboDepthId);
    glBindRenderbuffer(GL_RENDERBUFFER, context.rboDepthId);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaCount, GL_DEPTH_COMPONENT, context.resWidth, context.resHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // attach msaa RBOs to FBO attachment points
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, context.rboColorId);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, context.rboDepthId);

    /*
    // create a normal (no MSAA) FBO to hold a render-to-texture
    glGenFramebuffers(1, &context.fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, context.fboId);

    glGenRenderbuffers(1, &context.rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, context.rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, context.resWidth, context.resHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // attach a texture to FBO color attachement point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, context.textureId, 0);

    // attach a rbo to FBO depth attachement point
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, context.rboId);

    //@@ disable color buffer if you don't attach any color buffer image,
    //@@ for example, rendering the depth buffer only to a texture.
    //@@ Otherwise, glCheckFramebufferStatus will not be complete.
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);

    // check FBO status
    //printFramebufferInfo(fboMsaaId);
    //bool status = checkFramebufferStatus(fboMsaaId);
    //if(!status) {
    //    std::cout << "fboUsed = false" << std::endl;
    //}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    */
    ///end MSAA



    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);



    /// Create and compile our GLSL program from the shaders
    CShader *myShader = CShader::createShaderProgram("../StandardShading.vertexshader", nullptr, nullptr, nullptr, "../StandardShading.fragmentshader" );
    context.programID = myShader->getID();

    // Get a handle for our "MVP" uniform
    context.MatrixID = glGetUniformLocation(context.programID, "MVP");
    context.ViewMatrixID = glGetUniformLocation(context.programID, "V");
    context.ModelMatrixID = glGetUniformLocation(context.programID, "M");

    // Load the texture
    glGenTextures(4, context.textures);
    loadImage_SOIL(context.textures,"../chessboard.jpg",0);
    // Get a handle for our "myTextureSampler" uniform
    context.TextureID[0]  = glGetUniformLocation(context.programID, "myTextureSampler");
    loadImage_SOIL(context.textures,"../units/white2.jpg",1);
    context.TextureID[1]  = glGetUniformLocation(context.programID, "myTextureSampler");
    loadImage_SOIL(context.textures,"../units/black2.jpeg",2);
    context.TextureID[2]  = glGetUniformLocation(context.programID, "myTextureSampler");
    loadImage_SOIL(context.textures,"../units/target2.jpg",3);
    context.TextureID[3]  = glGetUniformLocation(context.programID, "myTextureSampler");

    glGenVertexArrays(8, context.VertexArrayID);

    std::vector<glm::vec3> indexed_vertices[8];
    std::vector<glm::vec2> indexed_uvs[8];
    std::vector<glm::vec3> indexed_normals[8];

    GLuint vertexbuffer[8];
    GLuint uvbuffer[8];
    GLuint normalbuffer[8];

    auto vaaV = new std::vector<CVAA*>;
    auto vaaU = new std::vector<CVAA*>;
    auto vaaN = new std::vector<CVAA*>;
    auto vaaE = new std::vector<CVAA*>;



    /// load units + chessboard
    for(int i = 0; i < 8; ++i) {
        glBindVertexArray(context.VertexArrayID[i]);

        // Read our .obj file
        // todo: read 6 different objects!
        if(i==0) {
            bool res = loadAssImp("../chessboard.obj", context.indices[i], indexed_vertices[i], indexed_uvs[i],
                                  indexed_normals[i]);
        } else if(i == 1) {
            bool res = loadAssImp("../units/drone.obj", context.indices[i], indexed_vertices[i], indexed_uvs[i],
                                  indexed_normals[i]);
        } else if(i == 2) {
            bool res = loadAssImp("../units/dreadnought.obj", context.indices[i], indexed_vertices[i], indexed_uvs[i],
                                  indexed_normals[i]);
        } else if(i == 3) {
            bool res = loadAssImp("../units/commandUnit.obj", context.indices[i], indexed_vertices[i], indexed_uvs[i],
                                  indexed_normals[i]);
        } else if(i == 4) {
            bool res = loadAssImp("../units/grunt.obj", context.indices[i], indexed_vertices[i], indexed_uvs[i],
                                  indexed_normals[i]);
        } else if(i == 5) {
            bool res = loadAssImp("../units/jumpship.obj", context.indices[i], indexed_vertices[i], indexed_uvs[i],
                                  indexed_normals[i]);
        } else if(i == 6){
            bool res = loadAssImp("../units/tank.obj", context.indices[i], indexed_vertices[i], indexed_uvs[i],
                                  indexed_normals[i]);
        } else {
            bool res = loadAssImp("../options2.obj", context.indices[i], indexed_vertices[i], indexed_uvs[i],
                                  indexed_normals[i]);
        }
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
    CView view(-30.0f,38.0f,-30.0f,3.14f/4.0f,-3.14f/5.1f);

    // Compute the MVP
    // glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(view.FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    context.ProjectionMatrix =  glm::ortho(10.0f*-context.resWidth/context.resHeight,10.0f*context.resWidth/context.resHeight,-10.0f,10.0f,0.1f, 100.0f);
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
        int next = game.gameloop();
        if (next == 1) {
            delete map;
            break;
        }
        Level = Level+next;
        delete map;
    }


    /// delete grafic buffers:
    // Cleanup VBO and shader
    glDeleteBuffers(8, vertexbuffer);
    glDeleteBuffers(8, uvbuffer);
    glDeleteBuffers(8, normalbuffer);
    glDeleteBuffers(8, context.elementbuffer);
    glDeleteProgram(context.programID);
    glDeleteTextures(4, context.textures);
    glDeleteVertexArrays(8, context.VertexArrayID);

    glDeleteFramebuffers(1, &context.fboId);
    context.fboId = 0;
    glDeleteRenderbuffers(1, &context.rboId);
    glDeleteRenderbuffers(1, &context.rboColorId);
    glDeleteRenderbuffers(1, &context.rboDepthId);
    context.rboId = context.rboColorId = context.rboDepthId = 0;

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

            map->add_unit(1,4,0);
            map->add_unit(1,4,7);
            map->add_unit(1,5,1);
            map->add_unit(1,5,6);
            map->add_unit(1,6,2);
            map->add_unit(1,6,5);
            map->add_unit(1,7,2);
            map->add_unit(1,7,5);
            map->add_unit(2,6,3);
            map->add_unit(2,6,4);
            map->add_unit(2,7,4);
            map->add_unit(3,7,3);

            break;
        case 2:
            map->add_unit(4,1,0);
            map->add_unit(4,1,2);
            map->add_unit(4,1,3);
            map->add_unit(4,1,4);
            map->add_unit(4,1,5);
            map->add_unit(4,1,7);
            map->add_unit(5,0,2);
            map->add_unit(5,0,1);
            map->add_unit(5,0,5);
            map->add_unit(5,0,6);
            map->add_unit(6,0,0);
            map->add_unit(6,0,7);


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
            map->add_unit(6,1,3);
            map->add_unit(6,1,4);
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
            map->add_unit(2,6,3);
            map->add_unit(2,6,4);
            map->add_unit(1,6,5);
            map->add_unit(1,6,6);
            map->add_unit(1,6,7);

            map->add_unit(1,4,2);
            map->add_unit(1,4,3);
            map->add_unit(1,4,4);
            map->add_unit(1,4,5);
            map->add_unit(1,5,2);
            map->add_unit(2,5,3);
            map->add_unit(2,5,4);
            map->add_unit(1,5,5);

            map->add_unit(3,7,3);
            map->add_unit(2,7,1);
            map->add_unit(2,7,2);
            map->add_unit(2,7,0);
            map->add_unit(2,7,7);
            map->add_unit(2,7,5);
            map->add_unit(2,7,6);
            map->add_unit(3,7,4);

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