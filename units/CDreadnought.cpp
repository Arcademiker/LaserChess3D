//
// Created by arcademiker on 21.07.18.
//

#include "CDreadnought.h"

void CDreadnought::scanSurrounding() {
    /// calculate the distance to all enemies and sort them using a map datastructure
    this->surrounding.clear();
    for(auto U: *this->map->get_unit_list()) {
        this->surrounding.insert( {this->map->distance(this->x, this->y, U.second->get_x(), U.second->get_y()), U.first});
    }
}

CDreadnought::CDreadnought(int typ, int x, int y, CMap &map) : CUnit_AI(typ, x, y, map) {
    this->health = 5;
    this->damage = 2;
}

int CDreadnought::get_nearest() {
    this->scanSurrounding();
    auto it = this->surrounding.begin(); /// nearest enemy
    int highestValue = 0;
    int type = 0;
    int nearest = 0;
    /// look in all equally near enemies for the most valuable
    for(int i = 0; i < this->surrounding.count(this->surrounding.begin()->first); ++i) {
        type = this->map->get_unit(it->second)->get_type();
        if(highestValue < type) {
            highestValue = type;
            nearest = it->second;
        }
        it++;
    }
    return nearest;
}

void CDreadnought::calc_move_area() {

}

void CDreadnought::calc_attack_area() {

}

void CDreadnought::do_move() {
    /// move:
    /// smater AI if it avoids damage while closing in?
    /// what if it runs into own unit?
    CUnit* enemy = this->map->get_unit(this->get_nearest()); /// get nearest enemy
    int x_dir = std::max(std::min(enemy->get_x()-this->x,1),-1); /// move -1,0 or 1 in x direction to minimize distance to enemy
    int y_dir = std::max(std::min(enemy->get_y()-this->y,1),-1); /// move -1,0 or 1 in y direction to minimize distance to enemy
    this->move(this->x+x_dir,this->y+y_dir);
}

void CDreadnought::do_attack() {
    /// attack:
    for(int d_y = -1; d_y <= 1; ++d_y) {
        for(int d_x = -1; d_x <= 1; ++d_x) {
            this->attack(this->x+d_x,this->y+d_y);
        }
    }
}




