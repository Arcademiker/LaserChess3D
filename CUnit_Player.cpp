//
// Created by arcademiker on 22.07.18.
//

#include "CUnit_Player.h"

CUnit_Player::CUnit_Player(int typ, int x, int y, CMap &map) : CUnit(typ, x, y, map) {
    this->xy = 0;
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

bool CUnit_Player::user_input() {
    if(this->xy == 0) {
        this->do_x = -1;
        //while (this->do_x < 0 || this->do_x > 7 || do_y < 0 || do_y > 7)) {
        if (glfwGetKey(this->map->window, GLFW_KEY_0) == GLFW_PRESS) {
            this->do_x = 0;
            std::cout << 0 << std::endl;
            this->xy++;
            return false;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_1) == GLFW_PRESS) {
            this->do_x = 1;
            std::cout << 1 << std::endl;
            this->xy++;
            return false;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_2) == GLFW_PRESS) {
            this->do_x = 2;
            std::cout << 2 << std::endl;
            this->xy++;
            return false;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_3) == GLFW_PRESS) {
            this->do_x = 3;
            std::cout << 3 << std::endl;
            this->xy++;
            return false;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_4) == GLFW_PRESS) {
            this->do_x = 4;
            std::cout << 4 << std::endl;
            this->xy++;
            return false;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_5) == GLFW_PRESS) {
            this->do_x = 5;
            std::cout << 5 << std::endl;
            this->xy++;
            return false;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_6) == GLFW_PRESS) {
            this->do_x = 6;
            std::cout << 6 << std::endl;
            this->xy++;
            return false;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_7) == GLFW_PRESS) {
            this->do_x = 7;
            std::cout << 7 << std::endl;
            this->xy++;
            return false;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_9) == GLFW_PRESS) {
            this->do_x = 9;
            std::cout << 9 << std::endl;
            this->xy++;
            return false;
        }
        if (this->do_x == 9) {
            this->do_x = this->x;
            this->do_y = this->y;
            return true;
        }
    } else {
        this->do_y = -1;
        if (glfwGetKey(this->map->window, GLFW_KEY_0) == GLFW_PRESS) {
            this->do_y = 0;
            std::cout << 0 << std::endl;
            this->xy = 0;
            return true;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_1) == GLFW_PRESS) {
            this->do_y = 1;
            std::cout << 1 << std::endl;
            this->xy = 0;
            return true;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_2) == GLFW_PRESS) {
            this->do_y = 2;
            std::cout << 2 << std::endl;
            this->xy = 0;
            return true;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_3) == GLFW_PRESS) {
            this->do_y = 3;
            std::cout << 3 << std::endl;
            this->xy = 0;
            return true;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_4) == GLFW_PRESS) {
            this->do_y = 4;
            std::cout << 4 << std::endl;
            this->xy = 0;
            return true;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_5) == GLFW_PRESS) {
            this->do_y = 5;
            std::cout << 5 << std::endl;
            this->xy = 0;
            return true;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_6) == GLFW_PRESS) {
            this->do_y = 6;
            std::cout << 6 << std::endl;
            this->xy = 0;
            return true;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_7) == GLFW_PRESS) {
            this->do_y = 7;
            std::cout << 7 << std::endl;
            this->xy = 0;
            return true;
        }
        if (glfwGetKey(this->map->window, GLFW_KEY_9) == GLFW_PRESS) {
            this->do_y = 9;
            std::cout << 9 << std::endl;
            this->xy = 0;
            return true;
        }
        if (this->do_y == 9) {
            this->do_x = this->x;
            this->do_y = this->y;
            this->xy = 0;
            return true;
        }
    }
    return false;
}