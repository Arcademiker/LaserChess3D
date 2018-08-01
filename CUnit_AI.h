//
// Created by arcademiker on 20.07.18.
//

#ifndef LASERCHESS_CUNIT_AI_H
#define LASERCHESS_CUNIT_AI_H


#include "CUnit.h"

class CUnit_AI : public CUnit {
protected:
    std::vector<std::vector<bool>> attack_range;
    std::vector<std::vector<bool>> player_options;
public:
    CUnit_AI(int typ, int x, int y, CMap &map);
    bool attack(int to_x, int to_y) override;
    bool attack(int target_id) override ;
    std::vector<std::vector<bool>>* get_player_optons() override;
    bool calc_attack_options() override;
    std::vector<std::vector<bool>>* get_attack_range() override;
    bool is_abort() override ;
};


#endif //LASERCHESS_CUNIT_AI_H
