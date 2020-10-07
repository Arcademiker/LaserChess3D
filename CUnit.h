//
// Created by arcademiker on 20.07.18.
//


#ifndef LASERCHESS_CUNIT_H
#define LASERCHESS_CUNIT_H

#include <algorithm>
#include <functional>
#include <cmath>
#include "CMap.h"



class CMap;

class CUnit {
protected:
    CMap* map;
    int type;
    int health;
    int damage;
    int x;
    int y;
public:
    CUnit(int typ, int x, int y, CMap& map);
    int get_type();
    int get_x();
    int get_y();
    int get_health();
    int get_damage();
    bool loose_health(int damage);
    bool move(int to_x, int to_y);
    int shot(int dir_x, int dir_y);
    virtual bool attack(int to_x, int to_y) = 0;
    virtual bool attack(int target_id) = 0;
    virtual void calc_move_area() = 0;
    virtual bool calc_attack_options() = 0;
    virtual void calc_attack_area() = 0;
    virtual std::vector<std::vector<bool>>* get_player_optons() = 0;
    virtual std::vector<std::vector<bool>>* get_attack_range() = 0;
    virtual bool do_move(GLFWwindow* window) = 0;
    virtual bool do_attack(GLFWwindow* window) = 0;
    virtual bool is_abort() = 0;
};


#endif //LASERCHESS_CUNIT_H
