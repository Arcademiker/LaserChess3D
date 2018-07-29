/* todo:
// Projekt Strukturieren
// Laser Chess einbinden
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
    for(int Level = 1; Level <= 3; ++Level) {
        // creat graphic context;
        graphics_context context;

        /// game logic:
        CMap* map = generate_map(Level);
        std::cout << std::endl << "++++++++++++++++++  LEVEL " << Level << "  ++++++++++++++++++" << std::endl;

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
        //GLuint Texture = loadDDS("../uvmap.DDS");

        glGenTextures(1, context.textures);
        loadImage_SOIL(context.textures,"../units/tank.jpeg",0);

        // Get a handle for our "myTextureSampler" uniform
        context.TextureID  = glGetUniformLocation(context.programID, "myTextureSampler");

        // Read our .obj file
        context.indices;
        std::vector<glm::vec3> indexed_vertices;
        std::vector<glm::vec2> indexed_uvs;
        std::vector<glm::vec3> indexed_normals;
        bool res = loadAssImp("../units/tank.obj", context.indices, indexed_vertices, indexed_uvs, indexed_normals);

        // Load it into a VBO

        glGenVertexArrays(1, context.VertexArrayID);
        glBindVertexArray(context.VertexArrayID[0]);

        GLuint vertexbuffer;
        GLuint uvbuffer;
        GLuint normalbuffer;

        CVAA vaa0(vertexbuffer, indexed_vertices, 0,3,0,0); // 1rst attribute buffer : vertices
        CVAA vaa1(uvbuffer, indexed_uvs,         1,2,0,0);  // 2nd attribute buffer : UVs
        CVAA vaa2(normalbuffer, indexed_normals, 2,3,0,0);  // 3rd attribute buffer : normals
        CVAA eaa3(context.elementbuffer, context.indices);                  // Generate a buffer for the indices as well

        // Get a handle for our "LightPosition" uniform
        glUseProgram(context.programID);
        context.LightID = glGetUniformLocation(context.programID, "LightPosition_worldspace");

        // For speed computation
        context.lastTime = glfwGetTime();
        context.nbFrames = 0;
        CView view(-30.0f,30.0f,-30.0f,3.14f/4.0f,-3.14f/5.1f);

        // Compute the MVP
        // glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(view.FoV), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 ProjectionMatrix =  glm::ortho(10.0f*-4.0f/3.0f,10.0f*4.0f/3.0f,-10.0f,10.0f,0.1f, 100.0f);
        context.ViewMatrix = glm::lookAt(
                view.position,           // Camera is here
                view.position+view.direction, // and looks here : at the same position, plus "direction"
                view.up                  // Head is up (set to 0,-1,0 to look upside-down)
        );

        context.ModelMatrix = glm::mat4(1.0);
        context.MVP = ProjectionMatrix * context.ViewMatrix * context.ModelMatrix;

        /*void drawGame(graphics_context context)
        do{
            // Measure speed
            double currentTime = glfwGetTime();
            nbFrames++;
            if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
                // printf and reset
                printf("%f ms/frame\n", 1000.0/double(nbFrames));
                nbFrames = 0;
                lastTime += 1.0;
            }

            /// Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            /// Use the shader
            glUseProgram(programID);



            // Send our transformation to the currently bound shader,
            // in the "MVP" uniform
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
            glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

            glm::vec3 lightPos = glm::vec3(4,4,4);
            glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(TextureID, 0);


            glBindVertexArray(VertexArrayID[0]);

            // Index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

            // Draw the triangles !
            glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT, // type
                    (void*)0           // element array buffer offset
            );

            glBindVertexArray(0);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);

            // Swap buffers
            glfwSwapBuffers(context.window);
            glfwPollEvents();

        } // Check if the ESC key was pressed or the context.window was closed
        while( glfwGetKey(context.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
               glfwWindowShouldClose(context.window) == 0 );

        */ /// ENTER MAIN GAME LOOP:
        // check if player may enter next level

        CGame game(map,  &context);
        if(!game.gameloop()) {
            Level--;
        }
        delete map;


        /// delete grafic buffers:
        // Cleanup VBO and shader
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &uvbuffer);
        glDeleteBuffers(1, &normalbuffer);
        glDeleteBuffers(1, &context.elementbuffer);
        glDeleteProgram(context.programID);
        glDeleteTextures(1, context.textures);
        glDeleteVertexArrays(1, context.VertexArrayID);

        // Close OpenGL context.window and terminate GLFW
        glfwTerminate();


    }
    return 0;
}

/*
bool gameloop(CMap* map, graphics_context context) {
    int round = 0;
    do{
        /// player turn
        map->listAllUnits();
        std::cout << std::endl << "******************  ROUND " << round << "  ******************" << std::endl << std::endl;
        ///0
        auto* UMap = new std::map<int,CUnit*>;
        for(auto& U: *map->get_unit_list()) {
            UMap->insert(U);
        }
        int id;
        CUnit* U;
        while(!UMap->empty()) {
            map->print(UMap);
            drawGame(context);
            ///--> 1 u
            id = user_input(UMap);
            if(id == 0) {
                id = UMap->begin()->first;
                U = UMap->begin()->second;
            } else {
                U = UMap->at(id);
            }
            ///2
            U->calc_move_area();
            print_options(U,map);
            ///3 u
            U->do_move();
            ///4
            if(U->calc_attack_options()) {
                print_options(U,map);
                ///5 u
                U->do_attack();
            }
            ///6--> 1
            UMap->erase(id);
        }
        /// 7
        delete UMap;

        /// AI turn

        if(map->get_enemys_list()->empty() || map->get_commandU_counter() <= 0) {
            map->listAllUnits();
            drawGame(context);
            std::cout << std::endl << "!PLAYER WINS!" << std::endl;
            return true;
        }
        auto* EMap = new std::multimap<int,CUnit*>;
        for(auto& E: *map->get_enemys_list()) {
            EMap->insert({E.second->get_type(),E.second});
        }
        for(auto& E: *EMap) {
            drawGame(context);
            ///8-->8
            E.second->do_move();
            E.second->do_attack();
        }
        /// 9
        delete EMap;
        if(map->get_unit_list()->empty()) {
            map->listAllUnits();
            drawGame(context);
            std::cout << std::endl << "!AI WINS!" << std::endl;
            return false;
        }
        round++;
    } while ( glfwGetKey(context.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(context.window) == 0 ); // Check if the ESC key was pressed or the context.window was closed
}
*/

/*
void drawGame(graphics_context context) {
    // Measure speed
    double currentTime = glfwGetTime();
    context.nbFrames++;
    if ( currentTime - context.lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
        // printf and reset
        printf("%f ms/frame\n", 1000.0/double(context.nbFrames));
        context.nbFrames = 0;
        context.lastTime += 1.0;
    }

    /// Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// Use the shader
    glUseProgram(context.programID);



    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(context.MatrixID, 1, GL_FALSE, &context.MVP[0][0]);
    glUniformMatrix4fv(context.ModelMatrixID, 1, GL_FALSE, &context.ModelMatrix[0][0]);
    glUniformMatrix4fv(context.ViewMatrixID, 1, GL_FALSE, &context.ViewMatrix[0][0]);

    glm::vec3 lightPos = glm::vec3(4,4,4);
    glUniform3f(context.LightID, lightPos.x, lightPos.y, lightPos.z);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, context.textures[0]);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(context.TextureID, 0);


    glBindVertexArray(context.VertexArrayID[0]);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.elementbuffer);

    // Draw the triangles !
    glDrawElements(
            GL_TRIANGLES,      // mode
            context.indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void*)0           // element array buffer offset
    );

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    // Swap buffers
    glfwSwapBuffers(context.window);
    glfwPollEvents();
}
*/

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

/*
void print_options(CUnit* unit, CMap* map) {
    std::cout << "   0 1 2 3 4 5 6 7" << std::endl << std::endl;
    for(int y = 0; y < 8 ; ++y) {
        std::cout << y << " ";
        for(int x = 0; x < 8 ; ++x) {
            if(unit->get_player_optons()->at(y).at(x)) {
                std::cout << " " << map->get(x,y);
            }
            else {
                std::cout << " X";
            }
        }
        std::cout << std::endl;
    }
}
*/

/*
int user_input(std::map<int,CUnit*>* UMap) {
    ///replace with mouse input
    int id = -1;
    while (id != 0 ) {
        std::cin >> id;
        for(auto& U: *UMap) {
            if(id == U.first) {
                return id;
            }
        }
    }
    return id;
}
*/

/*
int user_input(std::map<int,CUnit*>* UMap) {
    int id = -1;
    while (id != 0 &&  glfwGetKey(context.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(context.window) == 0) {
        if (glfwGetKey(context.window, GLFW_KEY_0) == GLFW_PRESS) {
            id = 0;
            std::cout << 0 << std::endl;
        }
        if (glfwGetKey(context.window, GLFW_KEY_1) == GLFW_PRESS) {
            id = 1;
            std::cout << 1 << std::endl;
        }
        if (glfwGetKey(context.window, GLFW_KEY_2) == GLFW_PRESS) {
            id = 2;
            std::cout << 2 << std::endl;
        }
        if (glfwGetKey(context.window, GLFW_KEY_3) == GLFW_PRESS) {
            id = 3;
            std::cout << 3 << std::endl;
        }
        if (glfwGetKey(context.window, GLFW_KEY_4) == GLFW_PRESS) {
            id = 4;
            std::cout << 4 << std::endl;
        }
        if (glfwGetKey(context.window, GLFW_KEY_5) == GLFW_PRESS) {
            id = 5;
            std::cout << 5 << std::endl;
        }
        if (glfwGetKey(context.window, GLFW_KEY_6) == GLFW_PRESS) {
            id = 6;
            std::cout << 6 << std::endl;
        }
        if (glfwGetKey(context.window, GLFW_KEY_7) == GLFW_PRESS) {
            id = 7;
            std::cout << 7 << std::endl;
        }
        if (glfwGetKey(context.window, GLFW_KEY_8) == GLFW_PRESS) {
            id = 8;
            std::cout << 8 << std::endl;
        }
        if (glfwGetKey(context.window, GLFW_KEY_9) == GLFW_PRESS) {
            id = 9;
            std::cout << 9 << std::endl;
        }
        for(auto& U: *UMap) {
            if(id == U.first) {
                return id;
            }
        }
    }
    return id;
}
*/

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