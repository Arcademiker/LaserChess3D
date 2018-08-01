//
// Created by arcademiker on 22.07.18.
//

#include "CJumpship.h"

CJumpship::CJumpship(int typ, int x, int y, CMap &map) : CUnit_Player(typ, x, y, map) {
    this->health = 2;
    this->damage = 2;
}

void CJumpship::calc_move_area() {
    this->player_options.clear();
    unsigned int size = this->map->get_size();
    this->player_options = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
    /// generate move options:
    /// 0 1 0 1 0
    /// 1 0 0 0 1
    /// 0 0 1 0 0
    /// 1 0 0 0 1
    /// 0 1 0 1 0
    this->player_options[this->y][this->x] = true;
    std::vector<std::pair<int,int>> dir;
    dir.push_back(std::make_pair( 2, 1));
    dir.push_back(std::make_pair( 1, 2));
    dir.push_back(std::make_pair( 2,-1));
    dir.push_back(std::make_pair( 1,-2));
    dir.push_back(std::make_pair(-2,-1));
    dir.push_back(std::make_pair(-1,-2));
    dir.push_back(std::make_pair(-2, 1));
    dir.push_back(std::make_pair(-1, 2));
    for(auto d: dir) {
        int d_x = d.first;
        int d_y = d.second;
        if (this->map->is_inbound(this->x+d_x, this->y+d_y) && this->map->get(this->x+d_x, this->y+d_y) == 0) {
            this->player_options[this->y+d_y][this->x+d_x] = true;
        }
    }
}

bool CJumpship::calc_attack_options() {
    return true;
}

void CJumpship::calc_attack_area() {
    /// genrate attack options:
    /// 0 0 1 0 1 0 0
    /// 0 1 X 1 X 1 0
    /// 1 X 1 1 1 X 1
    /// 0 1 1 X 1 1 0
    /// 1 X 1 1 1 X 1
    /// 0 1 X 1 X 1 0
    /// 0 0 1 0 1 0 0
    this->attack_range.clear();
    unsigned int size = this->map->get_size();
    this->attack_range = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));

    for(int y = 0; y < size ; ++y) {
        for(int x = 0; x < size ; ++x) {
            if(this->player_options[y][x]) {
                if(this->map->is_inbound(x,y+1)) {this->attack_range[y+1][x] = true;}
                if(this->map->is_inbound(x+1,y)) {this->attack_range[y][x+1] = true;}
                if(this->map->is_inbound(x,y-1)) {this->attack_range[y-1][x] = true;}
                if(this->map->is_inbound(x-1,y)) {this->attack_range[y][x-1] = true;}
            }
        }
    }
}

bool CJumpship::do_move(GLFWwindow* window) {
    /// move
    if(this->user_input(window)) {
        if(this->abort) {
            return false;
        }else if (!this->player_options[this->do_y][this->do_x]) {
            return false;
        } else {
            this->move(this->do_x, this->do_y);
            return true;
        }
    }
}

bool CJumpship::do_attack(GLFWwindow* window) {
    /// attack
    this->attack(this->x+1,this->y);
    this->attack(this->x,this->y+1);
    this->attack(this->x-1,this->y);
    this->attack(this->x,this->y-1);
    return true;
}

