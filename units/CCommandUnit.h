//
// Created by arcademiker on 22.07.18.
//

#ifndef LASERCHESS_CCOMMANDUNIT_H
#define LASERCHESS_CCOMMANDUNIT_H

#include <vector>

#include "../CUnit_AI.h"

class CCommandUnit : public CUnit_AI {
private:
    std::vector<int> evasion_area;
public:
    CCommandUnit(int typ, int x, int y, CMap &map);
    void calc_move_area() override;
    void calc_attack_area() override;
    bool do_move(GLFWwindow* window) override;
    bool do_attack(GLFWwindow* window) override;
};


#endif //LASERCHESS_CCOMMANDUNIT_H
