//
// Created by arcademiker on 28.07.18.
//

#include "CGame.h"

CGame::CGame(CMap* map, graphics_context* context) {
    this->context = context;
    this->UMap = new std::map<int,CUnit*>;
    this->EMap = new std::multimap<int,CUnit*>;
    int round = 0;
    this->map = map;
}

CGame::~CGame() {
    delete UMap;
    delete EMap;
}

bool CGame::gameloop() {
    int step = 0;
    do {
        step = this->logic_step(step);
        this->drawGame();
    } while ( glfwGetKey(this->context->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
              glfwWindowShouldClose(this->context->window) == 0 ); // Check if the ESC key was pressed or the->window was closed

    return step == -2;
}

int CGame::logic_step(int step) {
    switch(step) {
        case 0: {
            /// player turn
            for(auto& Unit: *this->map->get_unit_list()) {
                this->UMap->insert(Unit);
            }
            step = 1;
            break;
        }
        case 1: {
            /// user input;
            this->id = user_input();
            bool match = false;
            for (auto &Unit: *this->UMap) {
                if (id == Unit.first) {
                    match = true;
                    break;
                }
            }
            if (this->id == 0) {
                this->id = this->UMap->begin()->first;
                this->U = this->UMap->begin()->second;
                step = 2;
            } else if (match) {
                this->U = this->UMap->at(this->id);
                step = 2;
            } else {
                step = 1;
            }
            break;
        }
        case 2: {
            this->U->calc_move_area();
            step = 3;
            break;
        }
        case 3: {
            /// user input
            if (this->U->do_move()) {
                step = 4;
            } else {
                step = 3;
            }
            break;
        }
        case 4: {
            if (this->U->calc_attack_options()) {
                step = 5;
            } else {
                step = 6;
            }
            break;
        }
        case 5: {
            /// possible user input
            if (this->U->do_attack()) {
                step = 6;
            } else {
                step = 5;
            }
            break;
        }
        case 6: {
            this->UMap->erase(this->id);
            if (!this->UMap->empty()) {
                step = 1;
            } else {
                step = 7;
            }
            break;
        }
        case 7: {
            this->UMap->clear();
            /// AI turn
            if (this->map->get_enemys_list()->empty() || this->map->get_commandU_counter() <= 0) {
                this->map->listAllUnits();
                std::cout << std::endl << "!PLAYER WINS!" << std::endl;
                step = -2;
            } else {
                for (auto &E: *this->map->get_enemys_list()) {
                    this->EMap->insert({E.second->get_type(), E.second});
                }
                step = 8;
            }
            break;
        }
        case 8: {
            this->EMap->begin()->second->do_move();
            this->EMap->begin()->second->do_attack();
            if (!this->EMap->empty()) {
                step = 8;
            } else {
                step = 9;
            }
            break;
        }
        case 9: {
            this->EMap->clear();
            if (this->map->get_unit_list()->empty()) {
                this->map->listAllUnits();
                std::cout << std::endl << "!AI WINS!" << std::endl;
                step = -1;
            } else {
                this->round++;
                step = 0;
            }
            break;
        }
            default: {
                break;
        }
    }
    return step;
}

void CGame::drawGame() {
    // Measure speed
    double currentTime = glfwGetTime();
    this->context->nbFrames++;
    if ( currentTime - this->context->lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
        // printf and reset
        printf("%f ms/frame\n", 1000.0/double(this->context->nbFrames));
        this->context->nbFrames = 0;
        this->context->lastTime += 1.0;
    }

    /// Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// Use the shader
    glUseProgram(this->context->programID);



    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(this->context->MatrixID, 1, GL_FALSE, &this->context->MVP[0][0]);
    glUniformMatrix4fv(this->context->ModelMatrixID, 1, GL_FALSE, &this->context->ModelMatrix[0][0]);
    glUniformMatrix4fv(this->context->ViewMatrixID, 1, GL_FALSE, &this->context->ViewMatrix[0][0]);

    glm::vec3 lightPos = glm::vec3(4,4,4);
    glUniform3f(this->context->LightID, lightPos.x, lightPos.y, lightPos.z);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->context->textures[0]);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(this->context->TextureID, 0);


    glBindVertexArray(this->context->VertexArrayID[0]);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->context->elementbuffer);

    // Draw the triangles !
    glDrawElements(
            GL_TRIANGLES,      // mode
            this->context->indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void*)0           // element array buffer offset
    );

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    // Swap buffers
    glfwSwapBuffers(this->context->window);
    glfwPollEvents();
}

int CGame::user_input() {
    int id = -1;
    if (glfwGetKey(this->context->window, GLFW_KEY_0) == GLFW_PRESS) {
        id = 0;
        std::cout << 0 << std::endl;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_1) == GLFW_PRESS) {
        id = 1;
        std::cout << 1 << std::endl;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_2) == GLFW_PRESS) {
        id = 2;
        std::cout << 2 << std::endl;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_3) == GLFW_PRESS) {
        id = 3;
        std::cout << 3 << std::endl;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_4) == GLFW_PRESS) {
        id = 4;
        std::cout << 4 << std::endl;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_5) == GLFW_PRESS) {
        id = 5;
        std::cout << 5 << std::endl;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_6) == GLFW_PRESS) {
        id = 6;
        std::cout << 6 << std::endl;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_7) == GLFW_PRESS) {
        id = 7;
        std::cout << 7 << std::endl;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_8) == GLFW_PRESS) {
        id = 8;
        std::cout << 8 << std::endl;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_9) == GLFW_PRESS) {
        id = 9;
        std::cout << 9 << std::endl;
    }
    return id;
}




