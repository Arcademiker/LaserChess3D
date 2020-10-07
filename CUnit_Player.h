//
// Created by arcademiker on 22.07.18.
//

#ifndef LASERCHESS_CUNIT_PLAYER_H
#define LASERCHESS_CUNIT_PLAYER_H

#include <iostream> ///user input

#include <vector>

#include "CUnit.h"



class CUnit_Player : public CUnit{
protected:
    std::vector<std::vector<bool>> player_options;
    std::vector<std::vector<bool>> attack_range;
    int do_x;
    int do_y;
    int oldState;
    bool abort;
public:
    CUnit_Player(int typ, int x, int y, CMap &map);
    bool attack(int to_x, int to_y) override;
    bool attack(int target_id) override;
    std::vector<std::vector<bool>>* get_player_optons() override;
    std::vector<std::vector<bool>>* get_attack_range() override;
    bool user_input(GLFWwindow* window);
    bool is_abort() override ;
};


#endif //LASERCHESS_CUNIT_PLAYER_H
