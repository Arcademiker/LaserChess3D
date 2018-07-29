//
// Created by arcademiker on 21.07.18.
//

#ifndef LASERCHESS_CDREADNOUGHT_H
#define LASERCHESS_CDREADNOUGHT_H

#include <map>

#include "../CUnit_AI.h"


class CDreadnought : public CUnit_AI {
private:
    std::multimap<int,int> surrounding;

    void scanSurrounding();

public:
    CDreadnought(int typ, int x, int y, CMap &map);
    int get_nearest();
    void calc_move_area() override;
    void calc_attack_area() override;
    bool do_move(GLFWwindow* window) override;
    bool do_attack(GLFWwindow* window) override;

};


#endif //LASERCHESS_CDREADNOUGHT_H
