//
// Created by arcademiker on 22.07.18.
//

#include "CUnit_Player.h"

CUnit_Player::CUnit_Player(int typ, int x, int y, CMap &map) : CUnit(typ, x, y, map) {
}

bool CUnit_Player::attack(int to_x, int to_y) {
    if(this->map->is_inbound(to_x,to_y)) {
        int target_id = this->map->get(to_x,to_y);
        return this->attack(target_id);
    }
    else
    {
        return false;
    }
}

bool CUnit_Player::attack(int target_id) {
    CUnit* target_unit = this->map->get_unit(target_id);
    if(target_unit && target_unit->get_type() < 4) {
        if(target_unit->loose_health(this->damage)){
            this->map->kill_unit(target_id);
        }
        return true; /// attack hits target
    }

    return false; /// attack doesn't hit target
}

std::vector<std::vector<bool>> *CUnit_Player::get_attack_range() {
    return &this->attack_range;
}

std::vector<std::vector<bool>> *CUnit_Player::get_player_optons() {
    return &this->player_options;
}

/*
std::pair<int,int> CUnit_Player::user_input() {
    ///replace with mouse input
    int do_x = -1;
    int do_y = -1;
    while (do_x < 0 || do_x > 7 || do_y < 0 || do_y > 7) {
        std::cin >> do_x;
        if ( do_x == 9 ) {
            do_x = this->x;
            do_y = this->y;
            break;
        }
        std::cin >> do_y;
        if ( do_y == 9 ) {
            do_x = this->x;
            do_y = this->y;
            break;
        }
    }
    return std::make_pair(do_x,do_y);
}
*/

/*
bool CUnit_Player::user_input(GLFWwindow* window) {
    int newState = GLFW_RELEASE;
    if(this->xy == 0) {
        //while (this->do_x < 0 || this->do_x > 7 || do_y < 0 || do_y > 7)) {
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
            this->do_x = 0;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            this->do_x = 1;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            this->do_x = 2;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            this->do_x = 3;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            this->do_x = 4;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
            this->do_x = 5;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
            this->do_x = 6;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
            this->do_x = 7;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
            this->do_x = 9;
            newState = GLFW_PRESS;
        }
        if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
            std::cout << this->do_x << std::endl;
            oldState = newState;
            this->xy++;
        }
    } else if(this->xy == 1){
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
            this->do_y = 0;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            this->do_y = 1;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            this->do_y = 2;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            this->do_y = 3;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            this->do_y = 4;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
            this->do_y = 5;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
            this->do_y = 6;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
            this->do_y = 7;
            newState = GLFW_PRESS;
        }
        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
            this->do_y = 9;
            newState = GLFW_PRESS;
        }
        if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
            std::cout << this->do_y << std::endl;
            oldState = newState;
            this->xy++;
            return true;
        }
    }
    oldState = newState;
    return false;
}
 */

bool CUnit_Player::user_input(GLFWwindow* window) {
    int newState = GLFW_RELEASE;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->do_y = static_cast<int>((xpos-511.0f)/108.0f+(681.0f-ypos)/63.0f);
        this->do_x = static_cast<int>((681.0f-ypos)/63.0f-(xpos-511.0f)/108.0f);
    }
    if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
        std::cout << "( " << do_x << ", " << do_y << ")" << std::endl;
        oldState = newState;
        return true;
    }
    oldState = newState;
    return false;
}



