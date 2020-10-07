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
        ///calculate next game logic step
        step = this->logic_step(step);

        ///draw game state
        if((step==4 || step==8) && !(this->old_x == this->U->get_x() && this->old_y == this->U->get_y())) {
            ///move animation phase:
            for(this->anime = 0; this->anime < 20; ++this->anime) {
                this->drawGame(step);
            }
        } else {
            this->drawGame(step);
        }

        if(step == -1 || step == -2 ) {
            break;
        }
    } while ( glfwGetKey(this->context->window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
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
            //this->map->print(UMap);
            this->oldState = GLFW_RELEASE;
            step = 1;
            break;
        }
        case 1: {
            /// user input;
            if(user_input()) {
                if(this->UMap->count(this->id) > 0){
                    this->U = this->UMap->at(this->id);
                    step = 2;
                } else if (this->id == -1) {
                    this->id = this->UMap->begin()->first;
                    this->U = this->UMap->begin()->second;
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
            //this->map->print(UMap);
            this->U->calc_move_area();
            //this->print_options(U);
            step = 3;
            this->old_x = this->U->get_x();
            this->old_y = this->U->get_y();
            break;
        }
        case 3: {
            /// user input
            if (this->U->do_move(this->context->window)) {
                step = 4;
            } else {
                if(this->U->is_abort()) {
                    step = 1;
                } else {
                    step = 3;
                }
            }
            break;
        }
        case 4: {
            if (this->U->calc_attack_options()) {
                //this->print_options(U);
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
                //this->map->print(UMap);
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
                //this->map->listAllUnits();
                std::cout << std::endl << "!PLAYER WINS!" << std::endl;
                step = -2;
            } else {
                for (auto &E: *this->map->get_enemys_list()) {
                    this->EMap->insert(std::make_pair(E.second->get_type(), E.second));
                }
                step = 8;
            }
            break;
        }
        case 8: {
            if (!this->EMap->empty()) {
                this->old_x = this->EMap->begin()->second->get_x();
                this->old_y = this->EMap->begin()->second->get_y();
                this->E = this->EMap->begin()->second;
                this->EMap->begin()->second->do_move(this->context->window);
                this->EMap->begin()->second->do_attack(this->context->window);
                this->EMap->erase(this->EMap->begin());
                step = 8;
            } else {
                this->E = this->U;
                this->EMap->clear();
                step = 9;
            }
            break;
        }
        case 9: {
            if (this->map->get_unit_list()->empty()) {
                //this->map->listAllUnits();
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

void CGame::drawGame(int step) {
    // Measure speed
    /*
    double currentTime = glfwGetTime();
    this->context->nbFrames++;
    if ( currentTime - this->context->lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
        // printf and reset
        printf("%f ms/frame\n", 1000.0/double(this->context->nbFrames));
        this->context->nbFrames = 0;
        this->context->lastTime += 1.0;
    }
    */



    // set the rendering destination to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, this->context->fboMsaaId);

    /// Clear the screen
    // clear buffer
    glClearColor(0.05f, 0.0f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    /// Use the shader
    glUseProgram(this->context->programID);

    glm::vec3 lightPos = glm::vec3(7.5, 9, 7.5);
    glUniform3f(this->context->LightID, lightPos.x, lightPos.y, lightPos.z);




    /// draw chessboard
    this->context->ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(7, -0.5, 7));//glm::mat4(1.0);
    this->context->MVP = this->context->ProjectionMatrix * this->context->ViewMatrix * this->context->ModelMatrix;

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(this->context->MatrixID, 1, GL_FALSE, &this->context->MVP[0][0]);
    glUniformMatrix4fv(this->context->ModelMatrixID, 1, GL_FALSE, &this->context->ModelMatrix[0][0]);
    glUniformMatrix4fv(this->context->ViewMatrixID, 1, GL_FALSE, &this->context->ViewMatrix[0][0]);



    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->context->textures[0]);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(this->context->TextureID[0], 0);


    glBindVertexArray(this->context->VertexArrayID[0]);
    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->context->elementbuffer[0]);

    // Draw the triangles !
    glDrawElements(
            GL_TRIANGLES,      // mode
            this->context->indices[0].size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void *) 0           // element array buffer offset
    );

    glBindVertexArray(0);

    if(step == 3 || (step == 5 && this->U->get_type() != 5)) {
    /// draw player options
        for(int y = 0; y < 8 ; ++y) {
            for(int x = 0; x < 8 ; ++x) {
                if(this->U->get_player_optons()->at(y).at(x) || (y == this->U->get_y() && x == this->U->get_x())) {
                    this->context->ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(2*x, 0, 2*y));//glm::mat4(1.0);
                    this->context->MVP = this->context->ProjectionMatrix * this->context->ViewMatrix * this->context->ModelMatrix;
                    // Send our transformation to the currently bound shader,
                    // in the "MVP" uniform
                    glUniformMatrix4fv(this->context->MatrixID, 1, GL_FALSE, &this->context->MVP[0][0]);
                    glUniformMatrix4fv(this->context->ModelMatrixID, 1, GL_FALSE, &this->context->ModelMatrix[0][0]);
                    glUniformMatrix4fv(this->context->ViewMatrixID, 1, GL_FALSE, &this->context->ViewMatrix[0][0]);

                    if(step == 3) {
                        // Bind our texture in Texture Unit 0
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, this->context->textures[1]);
                        // Set our "myTextureSampler" sampler to use Texture Unit 0
                        glUniform1i(this->context->TextureID[1], 1);
                    } else {
                        // Bind our texture in Texture Unit 0
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, this->context->textures[3]);
                        // Set our "myTextureSampler" sampler to use Texture Unit 0
                        glUniform1i(this->context->TextureID[3], 3);
                    }

                    glBindVertexArray(this->context->VertexArrayID[7]);
                    // Index buffer
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->context->elementbuffer[7]);

                    // Draw the triangles !
                    glDrawElements(
                            GL_TRIANGLES,      // mode
                            this->context->indices[7].size(),    // count
                            GL_UNSIGNED_SHORT, // type
                            (void *) 0           // element array buffer offset
                    );

                    glBindVertexArray(0);
                }
            }
        }

    }

    /// draw player Units
    for (auto &U: *this->map->get_unit_list()) {
        if(step == 4 && U.first == this->id) {
            ///animate movement:
            if(U.second->get_type() == 5) {
                this->context->ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(
                        2.0f * this->old_x + ((2.0f * U.second->get_x() - 2.0f * this->old_x) * anime) / 20.0f,
                        (-(anime-10.0f)*(anime-10.0f)+100.0f)/50.f,
                        2.0f * this->old_y + ((2.0f * U.second->get_y() - 2.0f * this->old_y) * anime) / 20.0f));
            } else {
                this->context->ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(
                        2.0f * this->old_x + ((2.0f * U.second->get_x() - 2.0f * this->old_x) * anime) / 20.0f,
                        0,
                        2.0f * this->old_y + ((2.0f * U.second->get_y() - 2.0f * this->old_y) * anime) / 20.0f));
                //this->context->ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(2*U.second->get_x(), 0, 2*U.second->get_y()));//glm::mat4(1.0);
            }
        } else {
            this->context->ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(2*U.second->get_x(), 0, 2*U.second->get_y()));

        }
        this->context->MVP = this->context->ProjectionMatrix * this->context->ViewMatrix * this->context->ModelMatrix;
        int i = U.second->get_type();

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(this->context->MatrixID, 1, GL_FALSE, &this->context->MVP[0][0]);
        glUniformMatrix4fv(this->context->ModelMatrixID, 1, GL_FALSE, &this->context->ModelMatrix[0][0]);
        glUniformMatrix4fv(this->context->ViewMatrixID, 1, GL_FALSE, &this->context->ViewMatrix[0][0]);


        if(this->UMap->count(U.first) != 0) {
            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->context->textures[1]);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(this->context->TextureID[1], 1);
        } else {
            // Bind our texture in Texture Unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->context->textures[3]);
            // Set our "myTextureSampler" sampler to use Texture Unit 0
            glUniform1i(this->context->TextureID[3], 3);
        }

        glBindVertexArray(this->context->VertexArrayID[i]);
        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->context->elementbuffer[i]);

        // Draw the triangles !
        glDrawElements(
                GL_TRIANGLES,      // mode
                this->context->indices[i].size(),    // count
                GL_UNSIGNED_SHORT, // type
                (void *) 0           // element array buffer offset
        );

        glBindVertexArray(0);

        /// draw health
        if((U.second->get_type() < 6 && U.second->get_health() != 2) || (U.second->get_type() == 6 && U.second->get_health() != 4)) {
            float high = 0.0f;
            if(U.second->get_type() == 4) {
                high = 0.35f;
            }
            for(int k = 0; k<U.second->get_health() ; ++k) {
                if(step == 4 && U.first == this->id) {
                    ///animate movement:
                    if(U.second->get_type() == 5) {
                        this->context->ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(
                                2.0f * this->old_x + ((2.0f * U.second->get_x() - 2.0f * this->old_x) * anime) / 20.0f + 0.2f,
                                (-(anime-10.0f)*(anime-00.0f)+100.0f)/50.0f + 1.0f+k*0.08f+high,
                                2.0f * this->old_y + ((2.0f * U.second->get_y() - 2.0f * this->old_y) * anime) / 20.0f - 0.4f) );
                    } else {
                        this->context->ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(
                                2.0f * this->old_x + ((2.0f * U.second->get_x() - 2.0f * this->old_x) * anime) / 20.0f + 0.2f,
                                1.0f+k*0.08f+high,
                                2.0f * this->old_y + ((2.0f * U.second->get_y() - 2.0f * this->old_y) * anime) / 20.0f - 0.4f) );
                    }
                } else {
                    this->context->ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(2*U.second->get_x() + 0.2, 1.0f+k*0.08f+high, 2*U.second->get_y() - 0.4f));
                }
                this->context->MVP = this->context->ProjectionMatrix * this->context->ViewMatrix * this->context->ModelMatrix;
                // Send our transformation to the currently bound shader,
                // in the "MVP" uniform
                glUniformMatrix4fv(this->context->MatrixID, 1, GL_FALSE, &this->context->MVP[0][0]);
                glUniformMatrix4fv(this->context->ModelMatrixID, 1, GL_FALSE, &this->context->ModelMatrix[0][0]);
                glUniformMatrix4fv(this->context->ViewMatrixID, 1, GL_FALSE, &this->context->ViewMatrix[0][0]);



                // Bind our texture in Texture Unit 0
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, this->context->textures[4]);
                // Set our "myTextureSampler" sampler to use Texture Unit 0
                glUniform1i(this->context->TextureID[4], 4);


                glBindVertexArray(this->context->VertexArrayID[8]);
                // Index buffer
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->context->elementbuffer[8]);

                // Draw the triangles !
                glDrawElements(
                        GL_TRIANGLES,      // mode
                        this->context->indices[8].size(),    // count
                        GL_UNSIGNED_SHORT, // type
                        (void *) 0           // element array buffer offset
                );

                glBindVertexArray(0);
            }
        }

    }

    //todo: draw used units different
    /// draw enemy Units
    for (auto &E: *this->map->get_enemys_list()) {

        if(step == 8 && this->E == E.second && !(this->old_x == this->E->get_x() && this->old_y == this->E->get_y())) {
            ///animate movement:
            if(E.second->get_type() == 3 ) {
                if(E.second->get_y() < this->old_y) {
                    this->context->ModelMatrix = glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(
                            2.0f * this->old_x + ((2.0f * E.second->get_x() - 2.0f * this->old_x) * anime) / 20.0f,
                            (-(anime - 10.0f) * (anime - 10.0f) + 100.0f) / 250.f,
                            2.0f * this->old_y + ((2.0f * E.second->get_y() - 2.0f * this->old_y) * anime) / 20.0f)),
                                                             anime * -3.14f / 40.0f, glm::vec3(1, 0, 0));
                } else {
                    this->context->ModelMatrix = glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(
                            2.0f * this->old_x + ((2.0f * E.second->get_x() - 2.0f * this->old_x) * anime) / 20.0f,
                            (-(anime - 10.0f) * (anime - 10.0f) + 100.0f) / 250.f,
                            2.0f * this->old_y + ((2.0f * E.second->get_y() - 2.0f * this->old_y) * anime) / 20.0f)),
                                                             anime * 3.14f / 40.0f, glm::vec3(1, 0, 0));
                }
            } else {
                this->context->ModelMatrix = glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(
                        2.0f * this->old_x + ((2.0f * E.second->get_x() - 2.0f * this->old_x) * anime) / 20.0f, 0,
                        2.0f * this->old_y + ((2.0f * E.second->get_y() - 2.0f * this->old_y) * anime) / 20.0f)), 3.14f,
                                                         glm::vec3(0, 1, 0));
            }
        } else {
            this->context->ModelMatrix = glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(2*E.second->get_x(), 0, 2*E.second->get_y())),3.14f,glm::vec3(0,1,0));//glm::mat4(1.0);
        }
       // this->context->ModelMatrix = glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(2*E.second->get_x(), 0, 2*E.second->get_y())),3.14f,glm::vec3(0,1,0));//glm::mat4(1.0);
        this->context->MVP = this->context->ProjectionMatrix * this->context->ViewMatrix * this->context->ModelMatrix;
        int i = E.second->get_type();

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(this->context->MatrixID, 1, GL_FALSE, &this->context->MVP[0][0]);
        glUniformMatrix4fv(this->context->ModelMatrixID, 1, GL_FALSE, &this->context->ModelMatrix[0][0]);
        glUniformMatrix4fv(this->context->ViewMatrixID, 1, GL_FALSE, &this->context->ViewMatrix[0][0]);

        glm::vec3 lightPos = glm::vec3(4, 4, 4);
        glUniform3f(this->context->LightID, lightPos.x, lightPos.y, lightPos.z);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->context->textures[2]);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(this->context->TextureID[2], 2);


        glBindVertexArray(this->context->VertexArrayID[i]);
        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->context->elementbuffer[i]);

        // Draw the triangles !
        glDrawElements(
                GL_TRIANGLES,      // mode
                this->context->indices[i].size(),    // count
                GL_UNSIGNED_SHORT, // type
                (void *) 0           // element array buffer offset
        );

        /// draw health
        if((E.second->get_type() == 1 && E.second->get_health() != 2) || (E.second->get_type() > 1 && E.second->get_health() != 5)) {
            float high = 0.0f;
            float wide = 0.0f;
            if(E.second->get_type() == 3) {
                high = 0.35f;
            }
            if(E.second->get_type() == 2) {
                wide = 0.2f;
            }
            for(int k = 0; k<E.second->get_health() ; ++k) {
                if(step == 8 && this->E == E.second && !(this->old_x == this->E->get_x() && this->old_y == this->E->get_y())) {
                    ///animate movement:
                    this->context->ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(
                            2.0f * this->old_x + ((2.0f * E.second->get_x() - 2.0f * this->old_x) * anime) / 20.0f - 0.3f,
                            1.0f+k*0.08f+high,
                            2.0f * this->old_y + ((2.0f * E.second->get_y() - 2.0f * this->old_y) * anime) / 20.0f - 0.3f-wide) );

                } else {
                    this->context->ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(2*E.second->get_x() - 0.3, 1.0f+k*0.08f+high, 2*E.second->get_y() - 0.3f-wide));
                }
                this->context->MVP = this->context->ProjectionMatrix * this->context->ViewMatrix * this->context->ModelMatrix;
                // Send our transformation to the currently bound shader,
                // in the "MVP" uniform
                glUniformMatrix4fv(this->context->MatrixID, 1, GL_FALSE, &this->context->MVP[0][0]);
                glUniformMatrix4fv(this->context->ModelMatrixID, 1, GL_FALSE, &this->context->ModelMatrix[0][0]);
                glUniformMatrix4fv(this->context->ViewMatrixID, 1, GL_FALSE, &this->context->ViewMatrix[0][0]);



                // Bind our texture in Texture Unit 0
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, this->context->textures[4]);
                // Set our "myTextureSampler" sampler to use Texture Unit 0
                glUniform1i(this->context->TextureID[4], 4);


                glBindVertexArray(this->context->VertexArrayID[8]);
                // Index buffer
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->context->elementbuffer[8]);

                // Draw the triangles !
                glDrawElements(
                        GL_TRIANGLES,      // mode
                        this->context->indices[8].size(),    // count
                        GL_UNSIGNED_SHORT, // type
                        (void *) 0           // element array buffer offset
                );

                glBindVertexArray(0);
            }
        }

        glBindVertexArray(0);
    }


    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    ///MSAA
    // copy rendered image from MSAA (multi-sample) to normal (single-sample) FBO
    // NOTE: The multi samples at a pixel in read buffer will be converted
    // to a single sample at the target pixel in draw buffer.
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->context->fboMsaaId);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glBlitFramebuffer(0, 0, this->context->resWidth, this->context->resHeight,  // src rect
                      0, 0, this->context->resWidth, this->context->resHeight,  // dst rect
                      GL_COLOR_BUFFER_BIT, // buffer mask
                      GL_LINEAR);                           // scale filter

    // trigger mipmaps generation explicitly
    // NOTE: If GL_GENERATE_MIPMAP is set to GL_TRUE, then glCopyTexSubImage2D()
    // triggers mipmap generation automatically. However, the texture attached
    // onto a FBO should generate mipmaps manually via glGenerateMipmap().
    //glBindTexture(GL_TEXTURE_2D, this->context->textureId);
    //glGenerateMipmap(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, 0);

    // back to normal window-system-provided framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind


    /// Swap buffers
    glfwSwapBuffers(this->context->window);
    glfwPollEvents();
}



bool CGame::user_input() {
    int newState = GLFW_RELEASE;
    double xpos, ypos;
    glfwGetCursorPos(this->context->window, &xpos, &ypos);

    if (glfwGetMouseButton(this->context->window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) {
        newState = GLFW_PRESS;
        float diagUpDown = 63.0f*(context->resHeight/768.0f);
        float diagLeRe = 108.0f*(context->resHeight/768.0f);
        float PointZero = 934.0f;
        auto y = static_cast<int>((xpos-context->resWidth/2.0f)/diagLeRe+(PointZero-ypos)/diagUpDown);
        auto x = static_cast<int>((PointZero-ypos)/diagUpDown-(xpos-context->resWidth/2.0f)/diagLeRe);
        if(y>=0 && y<8 && x >=0 && x < 8) {
            this->id = this->map->get(x, y);
        } else {
            this->id = 0;
        }
    } else if (glfwGetKey(this->context->window, GLFW_KEY_ENTER)==GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->id = -1;
    }
    if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
        //std::cout << this->id << std::endl;
        oldState = newState;
        return true;
    }
    oldState = newState;
    return false;
}


/*
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
*/