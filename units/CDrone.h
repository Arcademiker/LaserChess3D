//
// Created by arcademiker on 22.07.18.
//

#ifndef LASERCHESS_CDRONE_H
#define LASERCHESS_CDRONE_H

#include "../CUnit_AI.h"

class CDrone : public CUnit_AI {
public:
    CDrone(int typ, int x, int y, CMap &map);
    void calc_move_area() override;
    void calc_attack_area() override;
    bool do_move(GLFWwindow* window) override;
    bool do_attack(GLFWwindow* window) override;
};


#endif //LASERCHESS_CDRONE_H
