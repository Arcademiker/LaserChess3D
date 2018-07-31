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

bool CUnit_Player::user_input(GLFWwindow* window) {
    int newState = GLFW_RELEASE;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) {
        newState = GLFW_PRESS;
        this->do_y = static_cast<int>((xpos-511.0f)/108.0f+(681.0f-ypos)/63.0f);
        this->do_x = static_cast<int>((681.0f-ypos)/63.0f-(xpos-511.0f)/108.0f);
    }
    if (this->do_y>=0 && this->do_y<8 && this->do_x >=0 && this->do_x < 8 &&newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
        std::cout << "( " << do_x << ", " << do_y << ")" << std::endl;
        oldState = newState;
        return true;
    }
    oldState = newState;
    return false;
}



