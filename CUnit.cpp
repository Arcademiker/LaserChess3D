//
// Created by arcademiker on 20.07.18.
//

#include "CUnit.h"

CUnit::CUnit(int type, int x, int y, CMap& map) {
    this->type = type;
    this-> x = x;
    this-> y = y;
    this->map = &map;
}

int CUnit::get_type() {
    return this->type;
}

int CUnit::get_x() {
    return this->x;
}

int CUnit::get_y() {
    return this->y;
}

int CUnit::get_health() {
    return this->health;
}

int CUnit::get_damage() {
    return this->damage;
}

bool CUnit::move(int to_x, int to_y) {
    if( this->map->move(this->x, this->y, to_x, to_y) ) {
        this->x = to_x;
        this->y = to_y;
        return true;
    }

    return false;
}

bool CUnit::loose_health(int damage) {
    this->health = this->health - damage;
    return this->health < 1; /// Unit dies by the damage
}

int CUnit::shot(int dir_x, int dir_y) {
    int at_x = this->x+dir_x;
    int at_y = this->y+dir_y;
    int target_id;
    while(this->map->is_inbound(at_x,at_y)) {
        target_id = this->map->get(at_x,at_y);
        if( target_id != 0 ) {
            return target_id;
        }
        at_x = at_x+dir_x;
        at_y = at_y+dir_y;
    }
    return 0;
}








