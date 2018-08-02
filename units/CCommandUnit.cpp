//
// Created by arcademiker on 22.07.18.
//

#include "CCommandUnit.h"
#include <iostream>

CCommandUnit::CCommandUnit(int typ, int x, int y, CMap &map) : CUnit_AI(typ, x, y, map) {
    this->health = 5;
    this->damage = 0;
}

void CCommandUnit::calc_move_area() {

}

void CCommandUnit::calc_attack_area() {

}


bool CCommandUnit::do_move(GLFWwindow* window) {
    this->evasion_area.clear();
    this->evasion_area = std::vector<int>(3,0);

    /// move command unit temporary out of the way, because it cannot block shot for itself on another field
    int tmp_x = this->x;
    int tmp_y = this->y;
    this->x = -1;
    this->y = -1;

    for(auto U: *this->map->get_unit_list()) {
        U.second->calc_move_area();
        U.second->calc_attack_area();
        std::vector<std::vector<bool>>* UAttack_range;
        for(int i = -1; i <= 1; ++i) {
            UAttack_range = U.second->get_attack_range();
            if(this->map->is_inbound(tmp_x,tmp_y+i) && UAttack_range->at(tmp_y+i).at(tmp_x)) {
                this->evasion_area[i+1] = this->evasion_area[i+1]+U.second->get_damage();
            }
        }
    }

    this->x = tmp_x;
    this->y = tmp_y;

    //std::cout << "evasion" << std::endl;
    //std::cout << this->evasion_area[0] << " " << this->evasion_area[1] << " " << this->evasion_area[2] << std::endl;

    if(this->map->is_inbound(this->x,this->y-1) && this->map->get(this->x,this->y-1) == 0
       && this->evasion_area[0]  < this->evasion_area[1] && (this->evasion_area[0] < this->evasion_area[2] || !this->map->is_inbound(this->x,this->y+1))) {
        this->move(this->x,this->y-1);
    }
    else if (this->map->is_inbound(this->x,this->y+1) && this->map->get(this->x,this->y+1) == 0
             && this->evasion_area[1] > this->evasion_area[2] ) {
        this->move(this->x,this->y+1);
    }
    else {
        this->move(this->x,this->y);
    }
    return true;
}

bool CCommandUnit::do_attack(GLFWwindow* window) {
    return true;
}

