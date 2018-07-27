//
// Created by arcademiker on 22.07.18.
//

#ifndef LASERCHESS_CJUMPSHIP_H
#define LASERCHESS_CJUMPSHIP_H

#include "../CUnit_Player.h"

class CJumpship : public CUnit_Player {
public:
    CJumpship(int typ, int x, int y, CMap &map);
    void calc_move_area() override;
    bool calc_attack_options() override;
    void calc_attack_area() override;
    void do_move() override;
    void do_attack() override;
};


#endif //LASERCHESS_CJUMPSHIP_H
