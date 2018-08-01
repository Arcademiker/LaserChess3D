//
// Created by arcademiker on 20.07.18.
//

#include "CUnit_AI.h"



CUnit_AI::CUnit_AI(int typ, int x, int y, CMap &map) : CUnit(typ, x, y, map) {

}

bool CUnit_AI::attack(int to_x, int to_y) {
    if(this->map->is_inbound(to_x,to_y)) {
        int target_id = this->map->get(to_x,to_y);
        return this->attack(target_id);
    }
    else
    {
        return false;
    }
}

bool CUnit_AI::attack(int target_id) {
    CUnit* target_unit = this->map->get_unit(target_id);
    if(target_unit && target_unit->get_type() > 3) {
        if(target_unit->loose_health(this->damage)){
            this->map->kill_unit(target_id);
        }
        return true; /// attack hits target
    }

    return false; /// attack doesn't hit target
}

std::vector<std::vector<bool>> *CUnit_AI::get_player_optons() {
    return &this->player_options;
}

bool CUnit_AI::calc_attack_options() {
    return true;
}

std::vector<std::vector<bool>> *CUnit_AI::get_attack_range() {
    return &this->attack_range;
}

bool CUnit_AI::is_abort() {
    return false;
}




