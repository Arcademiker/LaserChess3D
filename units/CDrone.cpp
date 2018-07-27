//
// Created by arcademiker on 22.07.18.
//

#include "CDrone.h"

CDrone::CDrone(int typ, int x, int y, CMap &map) : CUnit_AI(typ, x, y, map) {
    this->health = 2;
    this->damage = 1;
}

void CDrone::calc_move_area() {

}

void CDrone::calc_attack_area() {

}


void CDrone::do_move() {
    if(this->x == 0) {
        this->map->get_unit_list()->clear();
    }
    else {
        ///move:
        this->move(this->x-1, this->y);
    }
}

void CDrone::do_attack() {
    ///attack:
    int target1_id = this->shot(-1,-1);
    int target2_id = this->shot(-1,-1);
    if(target1_id && target2_id) { /// both shots would hit a target. Hit the most valuable
        if(this->map->get_unit(target1_id)->get_type() > this->map->get_unit(target2_id)->get_type()) {
            if(this->map->get_unit(target1_id)->get_type() > 3) {
                this->attack(target1_id);
            }
        }
        else {
            if(this->map->get_unit(target1_id)->get_type() > 3) {
                this->attack(target2_id);
            }
        }
    }
    else if(target1_id && this->map->get_unit(target1_id)->get_type() > 3) {
        this->attack(target1_id);
    }
    else if(target2_id && this->map->get_unit(target1_id)->get_type() > 3) {
        this->attack(target2_id);
    }
}
