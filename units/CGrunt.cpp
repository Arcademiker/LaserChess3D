//
// Created by arcademiker on 22.07.18.
//

#include "CGrunt.h"

CGrunt::CGrunt(int typ, int x, int y, CMap &map) : CUnit_Player(typ, x, y, map) {
    this->health = 2;
    this->damage = 1;
}

void CGrunt::calc_move_area() {
    this->player_options.clear();
    unsigned int size = this->map->get_size();
    this->player_options = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
    /// generate move options:
    /// 0 1 0
    /// 1 1 1
    /// 0 1 0
    this->player_options[this->y][this->x] = true;
    if (this->map->is_inbound(this->x, this->y+1) && this->map->get(this->x, this->y+1) == 0) {
        this->player_options[this->y+1][this->x] = true;
    }
    if (this->map->is_inbound(this->x+1, this->y) && this->map->get(this->x+1, this->y) == 0) {
        this->player_options[this->y][this->x+1] = true;
    }
    if (this->map->is_inbound(this->x, this->y-1) && this->map->get(this->x, this->y-1) == 0) {
        this->player_options[this->y-1][this->x] = true;
    }
    if (this->map->is_inbound(this->x-1, this->y) && this->map->get(this->x-1, this->y) == 0) {
        this->player_options[this->y][this->x-1] = true;
    }
}

bool CGrunt::calc_attack_options() {
    /// attack
    /// reset player options
    this->player_options.clear();
    unsigned int size = this->map->get_size();
    this->player_options = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
    CUnit* target1_unit=this->map->get_unit(this->shot(-1,-1));
    CUnit* target2_unit=this->map->get_unit(this->shot(-1, 1));
    CUnit* target3_unit=this->map->get_unit(this->shot( 1,-1));
    CUnit* target4_unit=this->map->get_unit(this->shot( 1, 1));
    bool hit = false;
    if(target1_unit || target2_unit || target3_unit || target4_unit) {
        if (target1_unit && target1_unit->get_type() < 4) { this->player_options[target1_unit->get_y()][target1_unit->get_x()] = true; hit = true;}
        if (target2_unit && target2_unit->get_type() < 4) { this->player_options[target2_unit->get_y()][target2_unit->get_x()] = true; hit = true;}
        if (target3_unit && target3_unit->get_type() < 4) { this->player_options[target3_unit->get_y()][target3_unit->get_x()] = true; hit = true;}
        if (target4_unit && target4_unit->get_type() < 4) { this->player_options[target4_unit->get_y()][target4_unit->get_x()] = true; hit = true;}
    }
    return hit;
}

void CGrunt::calc_attack_area() {
    /// genrate attack options:
    /// 1 1 0 0 0 1 1
    /// 1 1 1 0 1 1 1
    /// 0 1 1 1 1 1 0
    /// 0 0 1 X 1 0 0
    /// 0 1 1 1 1 1 0
    /// 1 1 1 0 1 1 1
    /// 1 1 0 0 0 1 1
    this->attack_range.clear();
    unsigned int size = this->map->get_size();
    this->attack_range = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
    std::vector<std::pair<int,int>> dir;
    dir.push_back(std::make_pair( 1, 1));
    dir.push_back(std::make_pair(-1, 1));
    dir.push_back(std::make_pair( 1,-1));
    dir.push_back(std::make_pair(-1,-1));

    for(int y = 0; y < size ; ++y) {
        for(int x = 0; x < size ; ++x) {
            if(this->player_options[y][x]) {
                for(auto d : dir) {
                    int at_x = x + d.first;
                    int at_y = y + d.second;
                    int target_id;
                    while (this->map->is_inbound(at_x, at_y)) {
                        this->attack_range[at_y][at_x] = true;
                        target_id = this->map->get(at_x, at_y);
                        if (target_id < 0) { /// player could move own units out of the line of fire
                            break;
                        }
                        at_x = at_x + d.first;
                        at_y = at_y + d.second;
                    }
                }
            }
        }
    }
}

bool CGrunt::do_move(GLFWwindow* window) {
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

bool CGrunt::do_attack(GLFWwindow* window) {
    /// attack
    if(this->user_input(window)) {
        if (!this->player_options[this->do_y][this->do_x] && !(this->do_x == this->x && this->do_y == this->y)) {
            return false;
        } else {
            this->attack(this->do_x, this->do_y);
            return true;
        }
    }
}

