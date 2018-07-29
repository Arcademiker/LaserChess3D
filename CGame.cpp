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

int CGame::gameloop() {
    int step = 0;
    do {
        step = this->logic_step(step);
        this->drawGame();
    } while ( step != -1 && step != -2 && glfwGetKey(this->context->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
              glfwWindowShouldClose(this->context->window) == 0 ); // Check if the ESC key was pressed or the->window was closed

    if(step == -1) {
        return -1;
    } else if(step == -2) {
        return 0;
    }
    return 1;
}

int CGame::logic_step(int step) {
    switch(step) {
        case 0: {
            /// player turn
            for(auto& Unit: *this->map->get_unit_list()) {
                this->UMap->insert(Unit);
            }
            this->map->print(UMap);
            this->oldState = GLFW_RELEASE;
            step = 1;
            break;
        }
        case 1: {
            /// user input;
            if(user_input()) {
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
            } else {
                step = 1;
            }
            break;
        }
        case 2: {
            this->map->print(UMap);
            this->U->calc_move_area();
            this->print_options(U);
            step = 3;
            break;
        }
        case 3: {
            /// user input
            if (this->U->do_move(this->context->window)) {
                step = 4;
            } else {
                step = 3;
            }
            break;
        }
        case 4: {
            if (this->U->calc_attack_options()) {
                this->print_options(U);
                step = 5;
            } else {
                step = 6;
            }
            break;
        }
        case 5: {
            /// possible user input
            if (this->U->do_attack(this->context->window)) {
                step = 6;
            } else {
                step = 5;
            }
            break;
        }
        case 6: {
            this->UMap->erase(this->id);
            if (!this->UMap->empty()) {
                this->map->print(UMap);
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
            if (!this->EMap->empty()) {
                this->EMap->begin()->second->do_move(this->context->window);
                this->EMap->begin()->second->do_attack(this->context->window);
                this->EMap->clear();
                step = 8;
            } else {
                step = 9;
            }
            break;
        }
        case 9: {
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

    /// unit 1
    glBindVertexArray(this->context->VertexArrayID[0]);

    this->context->ModelMatrix = glm::translate(glm::mat4(1.0),glm::vec3(1,0,2)) ;//glm::mat4(1.0);
    this->context->MVP = this->context->ProjectionMatrix *  this->context->ViewMatrix *  this->context->ModelMatrix;

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


    //glBindVertexArray(this->context->VertexArrayID[0]);

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

    /// unit 2
    glBindVertexArray(this->context->VertexArrayID[1]);

    this->context->ModelMatrix = glm::translate(glm::mat4(1.0),glm::vec3(0,0,0)) ;//glm::mat4(1.0);
    this->context->MVP = this->context->ProjectionMatrix *  this->context->ViewMatrix *  this->context->ModelMatrix;

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(this->context->MatrixID, 1, GL_FALSE, &this->context->MVP[0][0]);
    glUniformMatrix4fv(this->context->ModelMatrixID, 1, GL_FALSE, &this->context->ModelMatrix[0][0]);
    glUniformMatrix4fv(this->context->ViewMatrixID, 1, GL_FALSE, &this->context->ViewMatrix[0][0]);

    lightPos = glm::vec3(4,4,4);
    glUniform3f(this->context->LightID, lightPos.x, lightPos.y, lightPos.z);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->context->textures[0]);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(this->context->TextureID, 0);


    //glBindVertexArray(this->context->VertexArrayID[0]);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->context->elementbuffer1);

    // Draw the triangles !
    glDrawElements(
            GL_TRIANGLES,      // mode
            this->context->indices1.size(),    // count
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

bool CGame::user_input() {
    int newState = GLFW_RELEASE;

    if (glfwGetKey(this->context->window, GLFW_KEY_0) == GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->id = 0;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_1) == GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->id = 1;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_2) == GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->id = 2;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_3) == GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->id = 3;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_4) == GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->id = 4;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_5) == GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->id = 5;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_6) == GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->id = 6;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_7) == GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->id = 7;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_8) == GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->id = 8;
    }
    if (glfwGetKey(this->context->window, GLFW_KEY_9) == GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->id = 9;
    }
    if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
        std::cout << this->id << std::endl;
        oldState = newState;
        return true;
    }
    oldState = newState;
    return false;
}



void CGame::print_options(CUnit* unit) {
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
