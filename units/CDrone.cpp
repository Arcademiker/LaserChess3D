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


bool CDrone::do_move(GLFWwindow* window) {
    if(this->x == 0) {
        while (!this->map->get_unit_list()->empty()) {
            this->map->kill_unit(this->map->get_unit_list()->begin()->first);
        }
    }
    else {
        ///move:
        //if(this->map->is_inbound(this->x-1, this->y) && this->map->get(this->x-1, this->y) == 0) {
        this->move(this->x - 1, this->y);
        //}
    }
    return true;
}

bool CDrone::do_attack(GLFWwindow* window) {
    ///attack:
    std::multimap<int,int,std::greater<int>> targets;
    int target1_id = this->shot(-1,-1);
    int target2_id = this->shot(-1, 1);
    int target3_id = this->shot( 1,-1);
    int target4_id = this->shot( 1, 1);
    if(target1_id != 0 && this->map->get_unit(target1_id)->get_type() > 3) {
        targets.insert(std::make_pair(this->map->get_unit(target1_id)->get_type(),target1_id));
    }
    if(target2_id != 0 && this->map->get_unit(target2_id)->get_type() > 3) {
        targets.insert(std::make_pair(this->map->get_unit(target2_id)->get_type(),target2_id));
    }
    if(target3_id != 0 && this->map->get_unit(target3_id)->get_type() > 3) {
        targets.insert(std::make_pair(this->map->get_unit(target3_id)->get_type(),target3_id));
    }
    if(target4_id != 0 && this->map->get_unit(target4_id)->get_type() > 3) {
        targets.insert(std::make_pair(this->map->get_unit(target4_id)->get_type(),target4_id));
    }
    if(!targets.empty()) {
        this->attack(targets.begin()->second);
    }
    targets.clear();

    return true;
}
