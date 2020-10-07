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
	int nearest = 0;
    if(!this->surrounding.empty()) {
        auto it = this->surrounding.begin(); /// nearest enemy
        int highestValue = 0;
        int type = 0;
        /// look in all equally near enemies for the most valuable
        for(int i = 0; i < this->surrounding.count(this->surrounding.begin()->first); ++i) {
            type = this->map->get_unit(it->second)->get_type();
            if(highestValue < type) {
                highestValue = type;
                nearest = it->second;
            }
            it++;
        }
    }
    return nearest;
}

void CDreadnought::calc_move_area() {

}

void CDreadnought::calc_attack_area() {

}

bool CDreadnought::do_move(GLFWwindow* window) {
    /// move:
    /// smater AI if it avoids damage while closing in?
    if(this->get_nearest()!=0) {
        CUnit *enemy = this->map->get_unit(this->get_nearest()); /// get nearest enemy

        int x_dir = std::max(std::min(enemy->get_x() - this->x, 1),-1); /// move -1,0 or 1 in x direction to minimize distance to enemy
        int y_dir = std::max(std::min(enemy->get_y() - this->y, 1),-1); /// move -1,0 or 1 in y direction to minimize distance to enemy
        if(this->map->get(this->x + x_dir,this->y + y_dir) == 0) {
            this->move(this->x + x_dir, this->y + y_dir);
        } else if (this->map->get(this->x + x_dir,this->y) == 0) {
            this->move(this->x + x_dir, this->y);
        } else if (this->map->get(this->x ,this->y + y_dir) == 0) {
            this->move(this->x, this->y + y_dir);
        } else if (x_dir == 0 && this->map->is_inbound(this->x + 1, this->y + y_dir) && this->map->get(this->x + 1, this->y + y_dir) == 0 ) {
            this->move(this->x + 1, this->y + y_dir);
        } else if (x_dir == 0 && this->map->is_inbound(this->x - 1, this->y + y_dir) && this->map->get(this->x - 1, this->y + y_dir) == 0) {
            this->move(this->x - 1, this->y + y_dir);
        } else if (y_dir == 0 && this->map->is_inbound(this->x + x_dir, this->y + 1) && this->map->get(this->x + x_dir, this->y + 1) == 0) {
            this->move(this->x + x_dir, this->y + 1);
        } else if (y_dir == 0 && this->map->is_inbound(this->x + x_dir, this->y - 1) && this->map->get(this->x + x_dir, this->y - 1) == 0) {
            this->move(this->x + x_dir, this->y - 1);
        } else {
            this->move(this->x, this->y);
        }

    }
    return true;
}

bool CDreadnought::do_attack(GLFWwindow* window) {
    /// attack:
    for(int d_y = -1; d_y <= 1; ++d_y) {
        for(int d_x = -1; d_x <= 1; ++d_x) {
            this->attack(this->x+d_x,this->y+d_y);
        }
    }
    return true;
}




